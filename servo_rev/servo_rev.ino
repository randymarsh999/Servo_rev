/*
 Name:		servo_rev.ino
 Created:	09.06.2018 10:46:38
 Author:	root
*/
int servoPin = 9;            // ���� ����������� �����
int KeyPin = 8;
int temp = 1;
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
}

// the loop function runs over and over again until power down or reset
void loop() {
	int analogValue = analogRead(0); // ������ ���������� analogValue ��� ���������� ��������� 
	float voltageValue = analogValue * Step; // ��������� � ������ (��������� * ���) 
	delay(500); // ���� ��� ������� 

				//�������� ���� ��� ���������� ���� ����
	if (alarm == 'n' && voltageValue>3.0)
	{
		StopRelay();
		Serial.write('a');
		alarm = 'y';
	}
	if (alarm == 'y')
	{
		StopRelay();
		Serial.write('a');
	}

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
