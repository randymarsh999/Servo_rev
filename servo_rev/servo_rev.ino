/*
Name:		Sketch1.ino
Created:	22.06.2018 12:30:49
Author:	root
*/
int potPin = A5;
float Step = 5.16 / 1024;

int RelayPower = 12;
int RelayPolarPower = 6;
int RelayPolarP = 8;
int RelayPolarN = 10;
int Relay = 7;
int servoPin = 9;

int ang = 3;				//rotate angle
int myAngle;                // angle
int pulseWidth;             // puls width

int total=0;				//1 average
int total1=0;				//2 average
int total2=0;				//3 average

int readIndex=0;			//read index for 1 average
int readIndex1 = 0;			//read index for 2 average
int readIndex2 = 0;			//read index for 3 average

const int numReadings = 10;

void servoPulse(int servoPin, int myAngle)
{
	pulseWidth = (myAngle * 11) + 500;  //calculate puls width for serv
	digitalWrite(servoPin, HIGH);       //set contact RelayPower in status Low (tern On)
	delayMicroseconds(pulseWidth);      //set delay for pulsWidth
	digitalWrite(servoPin, LOW);        //set contact RelayPower in status Low (tern Off)
	delay(20);                          //set delay
}

void StopRelayN()
{
	digitalWrite(RelayPolarPower, LOW);
	delay(500);
}

void StartRelayN()
{
	digitalWrite(RelayPolarPower, HIGH);
	delay(500);
}



// the setup function runs once when you press reset or power the board
void setup() {
	pinMode(RelayPower, OUTPUT);        // set contact RelayPower out
	digitalWrite(RelayPower, HIGH);     // set contact RelayPower in status High

	pinMode(servoPin, OUTPUT);			// set contact ServoPin out


	pinMode(RelayPolarPower, OUTPUT);           // set contact RelayPolarPower out
	digitalWrite(RelayPolarPower, HIGH);        // set contact RelayPolarPower in status High

	//pinMode(RelayPolarP, OUTPUT);           // ñêîíôèãóðèðîâàëè âûõîä äëÿ óïðàâëåíèÿ ðåëå ïîëÿðíîñòè
	//pinMode(RelayPolarN, OUTPUT);           // ñêîíôèãóðèðîâàëè âûõîä äëÿ óïðàâëåíèÿ ðåëå ïîëÿðíîñòè

	delay(1000);
	Serial.begin(9600);
	delay(1000);
	pinMode(RelayPolarP, OUTPUT);
	StopRelayN();

}

/// <summary>
/// Loops this instance.
/// </summary>
/// the loop function runs over and over again until power down or reset
void loop() {
	total = total + analogRead(potPin);
	++readIndex;
	char outstr[10];
	float f_val;
	
	if (readIndex >= numReadings) {
		total1 = total1 + total / numReadings;
		total = 0;
		readIndex = 0;
		++readIndex1;
	}

	if (readIndex1 >= numReadings) {
		readIndex1 = 0;
		total2 = total2 + total1 / numReadings;
		++readIndex2;
		total1 = 0;
		readIndex1 = 0;
	}
	if (readIndex2 >= numReadings) {
		total2 = total2 / numReadings;
		f_val = Step * total2;
		dtostrf(f_val, 7, 4, outstr);
		readIndex2 = 0;
		total2 = 0;
	}

	
	if (Serial.available() > 0)
	{
		int val1 = Serial.read();
		switch (val1)
		{					
		//measurments
		case 'm':
		{
			Serial.print("Last average=");
			Serial.println(outstr);
			delay(100);
			Serial.flush();
			return;
		}		
		//diactivate relay for changing of polarity
		case 'x':
		{
			StopRelayN();
			delay(100);
			return;
		}
		//activate relay for changing of polarity
		case 'z':
		{
			StartRelayN();
			delay(100);
			return;
		}
		//decrease angle of servo engine
		case '-':
		{
			for (myAngle = 0; myAngle <= ang; myAngle++) {
				servoPulse(servoPin, myAngle);
			}
			return;
		}
		//increase angle of servo engine
		case '+':
		{
			for (myAngle = 180; myAngle >= (180 - ang); myAngle--) {
				servoPulse(servoPin, myAngle);
			}
		}
		}
	}
	delay(2);

}

