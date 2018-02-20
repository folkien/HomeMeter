/*
 * TODO Digital Inputs buttons
 * TODO Limits of temperature and humidity
 * TODO RTC date/time setting getting
 * TODO Room select
 * TODO Menu 
 */
#include "version.h"

#include <LiquidCrystal.h> //Dołączenie bilbioteki
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Informacja o podłączeniu nowego wyświetlacza

byte stable[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte arrowUp[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100
};

byte arrowDown[8] = {
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b00100,
	0b11111,
	0b01110,
	0b00100
};


enum lcdExtraCharacter {
    charStable = 0,
    charArrowUp = 1,
    charArrowDown = 2,
};

/// KEYS
/// ------------------------

#define KEY_OK 12
#define KEY_UP 11
#define KEY_DOWN 10

enum eKeyState {
  keyNone = 0,
  keyOK = 1,
  keyUp = 2,
  keyDown = 4,
};

eKeyState GetKeyState() 
{
  byte keyState[3];
  
  keyState[0] = digitalRead(KEY_OK); 
  keyState[1] = digitalRead(KEY_UP);
  keyState[2] = digitalRead(KEY_DOWN);
  delay(10);
  keyState[0] &= digitalRead(KEY_OK); 
  keyState[1] &= digitalRead(KEY_UP);
  keyState[2] &= digitalRead(KEY_DOWN);
  
  /// only one key could be pressed
  if (keyState[0])
  {
    return keyOK;
  }
  else if (keyState[1])
  {
    return keyUp;
  }
  else if (keyState[2])
  {
    return keyDown;
  }
  return keyNone;
}

#include "SoftwareSerial.h"
#include "hc05.h"
SoftwareSerial bluetooth(A0, A1); // RX, TX - odwrotnie

#define BT_KEY A2

void bluetooth_setName(const char * name)
{
  char cmdBuffer[30];
  sprintf(cmdBuffer,"AT+NAME=HomeMeter-%s\r\n",name);
  
  bluetooth.print(cmdBuffer);
}



#include <dht.h>

dht DHT;

#define DHT11_PIN 8
#define LED 13

typedef struct measurement {
    signed char value;
    byte previousValue;
    byte lowLimit;
    byte highLimit;
};

/// default room settings
measurement temperature = {0,0,19,30};
measurement humidity    = {0,0,35,60};

void GetMeasurements()
{
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      //    Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    case DHTLIB_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case DHTLIB_ERROR_ACK_L:
      Serial.print("Ack Low error,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      Serial.print("Ack High error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  temperature.previousValue = temperature.value;
  temperature.value = DHT.temperature;
  humidity.previousValue = humidity.value;
  humidity.value = DHT.humidity;
}

const char * rooms [] = {"Pokoj", "Dzieciecy", "Kuchnia", "Lazienka"};
const char * currentRoom = rooms[0];

void ScreenRoomSelect()
{

}

void ScreenHome()
{
  /// linia z datą
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print(currentRoom);
  lcd.print(" HH:MM.");

  // linia z wartościami
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(temperature.value, 1);
  lcd.print("C");
  if (temperature.value == temperature.previousValue)
  {
    lcd.print(" ");
  }
  else if (temperature.value > temperature.previousValue)
  {
    lcd.write((uint8_t)charArrowUp);
  }
  else 
  {
    lcd.write((uint8_t)charArrowDown);
  }
  lcd.print(" W:");
  lcd.print(humidity.value, 1);
  lcd.print("%");  
  if (humidity.value == humidity.previousValue)
  {
    lcd.print(" ");
  }
  else if (humidity.value > humidity.previousValue)
  {
    lcd.write((uint8_t)charArrowUp);
  }
  else 
  {
    lcd.write((uint8_t)charArrowDown);
  }
}


void setup() {
  // GPIOS
  pinMode(LED, OUTPUT);
  pinMode(KEY_OK, INPUT_PULLUP);
  pinMode(KEY_UP, INPUT_PULLUP);
  pinMode(KEY_DOWN, INPUT_PULLUP);
  pinMode(BT_KEY, OUTPUT);
  digitalWrite(LED, HIGH);
  digitalWrite(BT_KEY, LOW);
  
  /// init LCD 2x16
  lcd.createChar((uint8_t)charStable, stable);
  lcd.createChar((uint8_t)charArrowUp, arrowUp);
  lcd.createChar((uint8_t)charArrowDown, arrowDown);
  lcd.begin(16, 2); 
  lcd.setCursor(0, 0);
  lcd.print("HomeMeter v"); 
  lcd.print(VERSION_COMMIT);
  lcd.setCursor(0, 1);
  /// last commit date
  lcd.print(VERSION_DATE);
  

  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
  
  /// series of blinks 
  for (int i = 0; i < 20; ++i)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  digitalWrite(LED, 1);
 
  bluetooth.begin(9600);
  lcd.clear();
}

void loop() {
  GetMeasurements();

  // DISPLAY DATA
  Serial.print(humidity.value);
  Serial.print(",\t");
  Serial.println(temperature.value);

  ScreenHome();
  
  // Bluetooth
  bluetooth.print("E");
  bluetooth.print(humidity.value);
  bluetooth.print(",");
  bluetooth.print(temperature.value);
  bluetooth.print("\n");

  delay(2000);
}


