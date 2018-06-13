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
float Step = 5.0F / 1024; // Вычисляем шаг Uопорн / на градацию 



const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readingsF[numReadings];      // the readings from the analog input
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
int inputPin = A5;


void servoPulse(int servoPin, int myAngle)
{
	pulseWidth = (myAngle * 11) + 500;  // конвертируем угол в микросекунды
	digitalWrite(servoPin, HIGH);       // устанавливаем серве высокий уровень
	delayMicroseconds(pulseWidth);      // ждём
	digitalWrite(servoPin, LOW);        // устанавливаем низкий уровень
	delay(20);                          // 
}
//фрагмент кода для управления реле питания
//---------------------------------------------------------------------------------
//Открыть реле питания
void StartRelay()
{
	digitalWrite(Relay, HIGH); // Выключаем реле
	delay(500);
}
//Закрыть реле питания 
void StopRelay()
{
	digitalWrite(Relay, LOW); // Выключаем реле
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






// функция считывает аналоговый вход заданное количество раз
// и возвращает медианное отфильтрованное значение
//int readMedian(int pin, int samples) {
//	// массив для хранения данных
//	int raw[samples];
//	// считываем вход и помещаем величину в ячейки массива
//	for (int i = 0; i < samples; i++) {
//		raw[i] = analogRead(pin);
//	}
//	// сортируем массив по возрастанию значений в ячейках
//	int temp = 0; // временная переменная
//
//	for (int i = 0; i < samples; i++) {
//		for (int j = 0; j < samples - 1; j++) {
//			if (raw[j] > raw[j + 1]) {
//				temp = raw[j];
//				raw[j] = raw[j + 1];
//				raw[j + 1] = temp;
//			}
//		}
//	}
//	// возвращаем значение средней ячейки массива
//	return raw[samples / 2];
//}










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
	delay(1000); // Ждем 0.5 секунды 
	StopRelayP();
	delay(1000); // Ждем 0.5 секунды 
	StopRelayN();
	delay(1000); // Ждем 0.5 секунды 

	pinMode(inputPin, INPUT);	//set port A5 as input read value
	for (int thisReading = 0; thisReading < numReadings; thisReading++) 
	{
			readings[thisReading] = 0;
	}
	analogReference(EXTERNAL);

}

// the loop function runs over and over again until power down or reset
void loop() {
	char outstr[10];
	float f_val;
	int in;

	//in=analogRead(inputPin);
	//total = total - readings[readIndex];
	//readings[readIndex] = in;
	//total = total + readings[readIndex];
	//readIndex = readIndex + 1;
	//if (readIndex >= numReadings) {
	//	readIndex = 0;
	//}
	//average = total / numReadings;
	//Serial.print("Avarage = ");
	//Serial.println(average);

	
	
	in = analogRead(inputPin);
	//total1 = total1 - readingsF[readIndex];
	readingsF[readIndex] = in;
	
	//total1 = total1 + readingsF[readIndex];
	total1 = total1 + analogRead(inputPin);
	readIndex1 = readIndex1 + 1;
	if (readIndex1 >= numReadings) {
		readIndex1 = 0;
		average1 = total1 / numReadings;
		
		totalnew = totalnew + total1/10;
		++readIndexnew;
	/*	Serial.print("total=");
		Serial.println(total1);*/
		//f_val = (3.34 / 1023.00)*average1;
		//dtostrf(f_val, 7, 3, outstr);
		/*Serial.print("avar=");
		Serial.println(outstr);*/
		//delay(100);
		total1 = 0;
	}

	if (readIndexnew >= numReadings)
	{
		totalnew = totalnew / numReadings;
		f_val = (3.34 / 1023.00)*totalnew;
		dtostrf(f_val, 7, 3, outstr);
		Serial.print("avar1=");
		Serial.println(outstr);
		
		totalnew1 = totalnew1 + totalnew;
		totalnew = 0;
		readIndexnew = 0;
		
		++readIndexnew1;
	}

	if (readIndexnew1 >= numReadings)
	{
		totalnew1 = totalnew1 / numReadings;
		f_val = (3.34 / 1023.00)*totalnew1;
		dtostrf(f_val, 7, 3, outstr);
		Serial.print("avar2=");
		Serial.println(outstr);
		totalnew = 0;
		readIndexnew = 0;
		totalnew1 = 0;
		readIndexnew1 = 0;
	}





	//total1 = total1 - readingsf[readIndex];
	//outVal = analogRead(inputPin);
	//f_val = (5.000 / 1024.000)*outVal;
	//
	//
	//readingsF[readIndex] = analogRead(inputPin);
	//total = total + readings[readIndex];
	//readIndex = readIndex + 1;
	//if (readIndex >= numReadings) {
	//	readIndex = 0;
	//}
	//average = total / numReadings;
	//Serial.print("Avarage = ");
	//Serial.println(average);
	//delay(100);
	//Serial.print("Median = ");
	////Serial.println(readMedian(inputPin, 15));
	//delay(100);


	
	//outVal = in;
	//
	////delay(200); // wait 0.100
	//Serial.println(outVal);
	//f_val = 3.34 / 1023.00*outVal;
	//dtostrf(f_val, 7, 3, outstr);

	//Serial.println(outstr);
	//delay(500);
				
				
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
		int ang = 3; //угол поворота
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
