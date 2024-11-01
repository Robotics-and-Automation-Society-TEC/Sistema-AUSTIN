// Fuente: https://www.aranacorp.com/en/using-a-ds3231-module-with-arduino/
// https://howtomechatronics.com/tutorials/arduino/arduino-ds3231-real-time-clock-tutorial/


// Instalar la librería "DS3231" o "RTClib" por Adafruit.

//Librerías
#include <Wire.h>//https://www.arduino.cc/en/reference/wire
#include <DS3231.h>//https://github.com/NorthernWidget/DS3231


// Variables
byte Year ;
byte Month ;
byte Date ;
byte DoW ;
byte Hour ;
byte Minute ;
byte Second ;
bool Century  = false;
bool h12 ; // Variable para modo de 12 horas
bool PM ; // Definir si es PM el modo de 12 horas 

//Objects
DS3231 Clock;

void setup() {
  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  Wire.begin();
}

void loop() {
  setDate();
  readRTC();
}

// Leer y muestra la fecha y hora actuales del reloj
void readRTC( ) { 
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(Century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12, PM), DEC); //24-hr
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(Clock.getSecond(), DEC);
  delay(1000);
}

// Configura la fecha y hora actuales del reloj
void setDate( ) {
  
  // Lee los datos del puerto serie
  if (Serial.available()) {

    //int _start = millis();

    GetDateStuff(Year, Month, Date, DoW, Hour, Minute, Second);

    Clock.setClockMode(false);  // Configur en modo 24 horas

    Clock.setSecond(Second);
    Clock.setMinute(Minute);
    Clock.setHour(Hour);
    Clock.setDate(Date);
    Clock.setMonth(Month);
    Clock.setYear(Year);
    Clock.setDoW(DoW);

  }
}

// Lee la fecha y hora en formato establecido desde el puerto serie.

/*
El valor inicial debe ser dado por el usuario a través del monitor serie en este formato: YYMMDDwHHMMSSx

YY: año (2 dígitos)
MM: mes (2 dígitos)
DD: día (2 dígitos)
w: día de la semana (1 dígito, donde 1 = lunes, 2 = martes, etc.)
HH: hora (2 dígitos, en formato 24 horas)
MM: minutos (2 dígitos)
SS: segundos (2 dígitos)
x: indica el fin de la entrada
*/

void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, byte& Hour, byte& Minute, byte& Second) {
  
  boolean GotString = false; // Indica si recibe la cadena de información completa
  char InChar;
  byte Temp1, Temp2;
  char InString[20];

  byte j = 0;
  while (!GotString) {
    if (Serial.available()) {
      InChar = Serial.read();
      InString[j] = InChar;
      j += 1;
      if (InChar == 'x') {
        GotString = true;
      }
    }
  }
  Serial.println(InString);
  
  // Convierte cada valor en un número entero
  // Lee primero el año
  Temp1 = (byte)InString[0] - 48;
  Temp2 = (byte)InString[1] - 48;
  Year = Temp1 * 10 + Temp2;

  // El mes
  Temp1 = (byte)InString[2] - 48;
  Temp2 = (byte)InString[3] - 48;
  Month = Temp1 * 10 + Temp2;

  // El día
  Temp1 = (byte)InString[4] - 48;
  Temp2 = (byte)InString[5] - 48;
  Day = Temp1 * 10 + Temp2;

  // El día de la semana
  DoW = (byte)InString[6] - 48;

  // La hora
  Temp1 = (byte)InString[7] - 48;
  Temp2 = (byte)InString[8] - 48;
  Hour = Temp1 * 10 + Temp2;

  // Los minutos
  Temp1 = (byte)InString[9] - 48;
  Temp2 = (byte)InString[10] - 48;
  Minute = Temp1 * 10 + Temp2;

  // Los segundos
  Temp1 = (byte)InString[11] - 48;
  Temp2 = (byte)InString[12] - 48;
  Second = Temp1 * 10 + Temp2;
}
