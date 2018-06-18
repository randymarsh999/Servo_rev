/*
 Name:		servo_rev.ino
 Created:	09.06.2018 10:46:38
 Author:	root
*/
int servoPin = 9;            // ���� ����������� �����
int KeyPin = 8;
int temp = 1;
int outVal = 0;
//���������� ��� ���� ��������
int myAngle;                 // ����� ������� ���� ��������
int pulseWidth;              // ������������ ��������
int alarm = 'n';
int Relay = 7;          // ���������, ��� ����� ���� In1, ��������� � ���� ��������� ������ 7
int RelayPower = 12;      //������� ���� 12
int RelayPolarPower = 6;     // ���������, ��� ����� ���� In1, ��������� � ���� ��������� ������ 10
int RelayPolarP = 8;     // ���������, ��� ����� ���� In1, ��������� � ���� ��������� ������ 10
int RelayPolarN = 10;        //���������, ��� ����� ����, ��������� � ���� ��������� ������ 5
							 //1-1 ������� 
							 //2-3 ������ 2
							 //3-2 ������ 1
							 //4-5 �����

							 //���� �������� ����� �������
float Step = 3.34F / 1023; // ��������� ��� U����� / �� �������� 



const int numReadings = 10;

int readIndex = 0;              // the index of the current reading
int readIndex1 = 0;              // the index of the current reading
int readIndexnew = 0;              // the index of the current reading
int readIndexnew1 = 0;
int total = 0;                  // the running total
int total1 = 0;                  // the running total
int totalnew = 0;                  // the running total
int totalnew1= 0;                  // the running total

int average = 0;
int average1 = 0;
int inputPin = A4;


void servoPulse(int servoPin, int myAngle)
{
	pulseWidth = (myAngle * 11) + 500;  // ������������ ���� � ������������
	digitalWrite(servoPin, HIGH);       // ������������� ����� ������� �������
	delayMicroseconds(pulseWidth);      // ���
	digitalWrite(servoPin, LOW);        // ������������� ������ �������
	delay(20);                          // 
}
//Fragment for power relay
//---------------------------------------------------------------------------------
//Open power relay
void StartRelay()
{
	digitalWrite(Relay, HIGH); // Power on
	delay(500);
}
//Close power relay
void StopRelay()
{
	digitalWrite(Relay, LOW); // Power off
	delay(500);
}

//������������ ������ ��������� ���� ������������� ����������
void StopRelayP()
{
	digitalWrite(RelayPolarP, HIGH); // ��������� ����
	delay(500);
}
//������������ ������ ������� ���� ������������� ����������
void StartRelayP()
{
	digitalWrite(RelayPolarP, LOW); // ��������� ����
	delay(500);
}
//������������ ������ ��������� ���� ������������ ����������
void StopRelayN()
{
	digitalWrite(RelayPolarN, HIGH); // ��������� ����
	delay(500);
}
//---------------------------------------------------------------------------------

//������������ ������ ������� ���� ������������� ����������
void StartRelayN()
{
	digitalWrite(RelayPolarN, LOW); // ��������� ����
	delay(500);
}

// the setup function runs once when you press reset or power the board
void setup() {
	// put your setup code here, to run once:
	pinMode(servoPin, OUTPUT);          // ������������� ��� �����, ��� �����
	pinMode(KeyPin, OUTPUT);
	pinMode(Relay, OUTPUT);             // ���������������� ����� ��� ���������� ���� �������
	pinMode(RelayPower, OUTPUT);        // ���������������� ����� ��� ������� ���� �������
	digitalWrite(RelayPower, HIGH);     // �������� ����


	pinMode(RelayPolarPower, OUTPUT);           // ���������������� ����� ��� ���������� ���� ����������
	digitalWrite(RelayPolarPower, HIGH);           // ���������������� ����� ��� ���������� ���� ����������

	pinMode(RelayPolarP, OUTPUT);           // ���������������� ����� ��� ���������� ���� ����������
	pinMode(RelayPolarN, OUTPUT);           // ���������������� ����� ��� ���������� ���� ����������

	Serial.begin(9600);                     //���������� �������� Com �����
	delay(1000); // wait 1 sec
	StopRelayP();
	delay(1000); // wait 1 sec
	StopRelayN();
	delay(1000); // wait 1 sec

	pinMode(inputPin, INPUT);	//set port A5 as input read value
	analogReference(EXTERNAL);

}

// the loop function runs over and over again until power down or reset
void loop() {
	char outstr[10];
	float f_val;
	int in;

	in = analogRead(inputPin);
	total1 = total1 + analogRead(inputPin);
	readIndex1 = readIndex1 + 1;
	if (readIndex1 >= numReadings) {
		readIndex1 = 0;
		average1 = total1 / numReadings;
		totalnew = totalnew + total1/10;
		++readIndexnew;
		total1 = 0;
	}

	if (readIndexnew >= numReadings)
	{
		totalnew = totalnew / numReadings;
		f_val = Step*totalnew;
		dtostrf(f_val, 7, 3, outstr);
		//Serial.print("avar1=");
		//Serial.println(outstr);
		totalnew1 = totalnew1 + totalnew;
		totalnew = 0;
		readIndexnew = 0;
		++readIndexnew1;
	}

	if (readIndexnew1 >= numReadings)
	{
		totalnew1 = totalnew1 / numReadings;
		f_val = Step*totalnew1;
		dtostrf(f_val, 7, 3, outstr);
		Serial.print("Last average=");
		Serial.println(outstr);
		totalnew = 0;
		readIndexnew = 0;
		totalnew1 = 0;
		readIndexnew1 = 0;
	}

			
	//full turn off
	/*if (alarm == 'n' && voltageValue>3.0)
	{
		StopRelay();
		Serial.write('a');
		alarm = 'y';
	}
	if (alarm == 'y')
	{
		StopRelay();
		Serial.write('a');
	}*/

	if (Serial.available() > 0)
	{
		int ang = 3; //rotate angle
		int val = Serial.read();
		switch (val)
		{
			//���������� �������������
			//---------------------------------------------------------------------------------
		case '-': //���������� ���� �������
		{
			for (myAngle = 0; myAngle <= ang; myAngle++) {
				servoPulse(servoPin, myAngle);
			}
			return;
		}
		case '+': //���������� ���� ��������
		{
			for (myAngle = 180; myAngle >= (180 - ang); myAngle--) {
				servoPulse(servoPin, myAngle);
			}
		}
		//����� ��������� ��� ���������� �������������
		//---------------------------------------------------------------------------------


		case 'a': //���������� �������
		{
			alarm = 'n';
			return;
		}
		case 's': //��������� �������
		{
			alarm = 'y';
			return;
		}
		//��������� ������� �� ��� ���������
		case 'r': //��������� ����
		{
			alarm = 'n';
			StartRelay();
			return;
		}
		case 'e': //������ ���������� ����
		{
			StopRelay();
			return;
		}
		//���� ������������� ����������        
		case 'x': //��������� ���� ������������� ����������
		{
			StopRelayP();
			delay(100); // ���� 0.1 ������� 
			return;
		}
		case 'z': //���������� ���� ������������� ����������
		{
			StartRelayP();
			delay(100); // ���� 0.1 ������� 
			return;
		}




		//���� ������������� ����������        
		case 'c': //��������� ���� ������������� ����������
		{
			StartRelayN();
			delay(100); // ���� 0.1 ������� 
			return;
		}
		case 'v': //���������� ���� ������������� ����������
		{
			StopRelayN();
			delay(100); // ���� 0.1 ������� 
			return;
		}
		case 'b':

			if (temp == 0)
			{
				temp = 1;
				StopRelay();
			}
			if (temp == 1)
			{
				temp = 0;
				StartRelay();
			}


		default:;
		}
	}

}
