/*
 Name:		servo_rev.ino
 Created:	09.06.2018 10:46:38
 Author:	root
*/
int servoPin = 9;            // порт подключения сервы
int KeyPin = 8;
int temp = 1;
int outVal = 0;
//переменные для угла поворота
int myAngle;                 // будет хранить угол поворота
int pulseWidth;              // длительность импульса
int alarm = 'n';
int Relay = 7;          // Указываем, что вывод реле In1, подключен к реле цифровому выводу 7
int RelayPower = 12;      //питание реле 12
int RelayPolarPower = 6;     // Указываем, что вывод реле In1, подключен к реле цифровому выводу 10
int RelayPolarP = 8;     // Указываем, что вывод реле In1, подключен к реле цифровому выводу 10
int RelayPolarN = 10;        //Указываем, что вывод реле, подключен к реле цифровому выводу 5
							 //1-1 питание 
							 //2-3 сигнал 2
							 //3-2 сигнал 1
							 //4-5 земля

							 //угол поворота серво привода
float Step = 3.34F / 1023; // Вычисляем шаг Uопорн / на градацию 



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
	pulseWidth = (myAngle * 11) + 500;  // конвертируем угол в микросекунды
	digitalWrite(servoPin, HIGH);       // устанавливаем серве высокий уровень
	delayMicroseconds(pulseWidth);      // ждём
	digitalWrite(servoPin, LOW);        // устанавливаем низкий уровень
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

//Управлеяющий сигнал запустить реле положительной полярности
void StopRelayP()
{
	digitalWrite(RelayPolarP, HIGH); // Выключаем реле
	delay(500);
}
//Управлеяющий сигнал закрыть реле положительной полярности
void StartRelayP()
{
	digitalWrite(RelayPolarP, LOW); // Выключаем реле
	delay(500);
}
//Управлеяющий сигнал запустить реле орицательной полярности
void StopRelayN()
{
	digitalWrite(RelayPolarN, HIGH); // Выключаем реле
	delay(500);
}
//---------------------------------------------------------------------------------

//Управлеяющий сигнал закрыть реле отрицательной полярности
void StartRelayN()
{
	digitalWrite(RelayPolarN, LOW); // Выключаем реле
	delay(500);
}

// the setup function runs once when you press reset or power the board
void setup() {
	// put your setup code here, to run once:
	pinMode(servoPin, OUTPUT);          // конфигурируем пин сервы, как выход
	pinMode(KeyPin, OUTPUT);
	pinMode(Relay, OUTPUT);             // сконфигурировали выход для управления реле питания
	pinMode(RelayPower, OUTPUT);        // сконфигурировали выход для питания реле питания
	digitalWrite(RelayPower, HIGH);     // Запитали Реле


	pinMode(RelayPolarPower, OUTPUT);           // сконфигурировали выход для управления реле полярности
	digitalWrite(RelayPolarPower, HIGH);           // сконфигурировали выход для управления реле полярности

	pinMode(RelayPolarP, OUTPUT);           // сконфигурировали выход для управления реле полярности
	pinMode(RelayPolarN, OUTPUT);           // сконфигурировали выход для управления реле полярности

	Serial.begin(9600);                     //установили скорость Com порта
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
			//управление сервоприводом
			//---------------------------------------------------------------------------------
		case '-': //уменьшение угла поворта
		{
			for (myAngle = 0; myAngle <= ang; myAngle++) {
				servoPulse(servoPin, myAngle);
			}
			return;
		}
		case '+': //приращение угла поворота
		{
			for (myAngle = 180; myAngle >= (180 - ang); myAngle--) {
				servoPulse(servoPin, myAngle);
			}
		}
		//конец фрагмента для управления сервоприводом
		//---------------------------------------------------------------------------------


		case 'a': //выключение тревоги
		{
			alarm = 'n';
			return;
		}
		case 's': //включение тревоги
		{
			alarm = 'y';
			return;
		}
		//включение питания на всю установку
		case 'r': //Включение Реле
		{
			alarm = 'n';
			StartRelay();
			return;
		}
		case 'e': //полное отключение Реле
		{
			StopRelay();
			return;
		}
		//реле положительной полярности        
		case 'x': //включение реле положительной полярности
		{
			StopRelayP();
			delay(100); // Ждем 0.1 секунды 
			return;
		}
		case 'z': //отключение реле положительной полярности
		{
			StartRelayP();
			delay(100); // Ждем 0.1 секунды 
			return;
		}




		//реле отрицательной полярности        
		case 'c': //включение реле отрицательной полярности
		{
			StartRelayN();
			delay(100); // Ждем 0.1 секунды 
			return;
		}
		case 'v': //отключение реле отрицательной полярности
		{
			StopRelayN();
			delay(100); // Ждем 0.1 секунды 
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
