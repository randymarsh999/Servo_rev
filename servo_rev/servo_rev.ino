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
float Step = 5.0F / 1024; // ��������� ��� U����� / �� �������� 



const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
int inputPin = A5;


void servoPulse(int servoPin, int myAngle)
{
	pulseWidth = (myAngle * 11) + 500;  // ������������ ���� � ������������
	digitalWrite(servoPin, HIGH);       // ������������� ����� ������� �������
	delayMicroseconds(pulseWidth);      // ���
	digitalWrite(servoPin, LOW);        // ������������� ������ �������
	delay(20);                          // 
}
//�������� ���� ��� ���������� ���� �������
//---------------------------------------------------------------------------------
//������� ���� �������
void StartRelay()
{
	digitalWrite(Relay, HIGH); // ��������� ����
	delay(500);
}
//������� ���� ������� 
void StopRelay()
{
	digitalWrite(Relay, LOW); // ��������� ����
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






// ������� ��������� ���������� ���� �������� ���������� ���
// � ���������� ��������� ��������������� ��������
int readMedian(int pin, int samples) {
	// ������ ��� �������� ������
	int raw[samples];
	// ��������� ���� � �������� �������� � ������ �������
	for (int i = 0; i < samples; i++) {
		raw[i] = analogRead(pin);
	}
	// ��������� ������ �� ����������� �������� � �������
	int temp = 0; // ��������� ����������

	for (int i = 0; i < samples; i++) {
		for (int j = 0; j < samples - 1; j++) {
			if (raw[j] > raw[j + 1]) {
				temp = raw[j];
				raw[j] = raw[j + 1];
				raw[j + 1] = temp;
			}
		}
	}
	// ���������� �������� ������� ������ �������
	return raw[samples / 2];
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
	delay(1000); // ���� 0.5 ������� 
	StopRelayP();
	delay(1000); // ���� 0.5 ������� 
	StopRelayN();
	delay(1000); // ���� 0.5 ������� 

	pinMode(inputPin, INPUT);	//set port A5 as input read value
	for (int thisReading = 0; thisReading < numReadings; thisReading++) 
	{
			readings[thisReading] = 0;
	}

}

// the loop function runs over and over again until power down or reset
void loop() {
	char outstr[10];
	float f_val;
	
	int analogValue = analogRead(0); // ������ ���������� analogValue ��� ���������� ��������� 
	float voltageValue = analogValue * Step; // ��������� � ������ (��������� * ���) 
	delay(500); // ���� ��� ������� 





	total = total - readings[readIndex];
	outVal = analogRead(inputPin);
	float f_val = (5.000 / 1024.000)*outVal;
	
	readings[readIndex] = analogRead(inputPin);
	total = total + readings[readIndex];
	readIndex = readIndex + 1;
	
	if (readIndex >= numReadings) {
		readIndex = 0;
	}
	average = total / numReadings;
	Serial.print("Avarage = ");
	Serial.println(average);

	
	
	
	delay(100);
	Serial.print("Median = ");
	Serial.println(readMedian(inputPin, 15));
	delay(100);


	
	outVal = analogRead(A5);
	delay(200); // wait 0.100
	f_val = (5.000 / 1024.000)*outVal;
	dtostrf(f_val, 7, 3, outstr);
	Serial.println(outstr);
	delay(500);
				
				
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
		int ang = 3; //���� ��������
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
