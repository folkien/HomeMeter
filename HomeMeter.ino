/*
 * TODO Digital Inputs buttons
 * TODO Limits of temperature and humidity
 * TODO RTC date/time setting getting
 * TODO Room select
 * TODO Menu 
 */
#include "version.h"
#include <dht.h>
#include "hc05.h"
#include "lcd.h"
#include "timeout.h"
#include "keys.h"
#include "bluetooth.h"

extern LiquidCrystal lcd;
extern SoftwareSerial bluetooth;

#define array_sizeof(x) (sizeof(x)/sizeof(x[0]))





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
sTimeout measureTimeout  = {0,30000}; // 30s

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

enum eScreens {
  screenHome,
  screenMenu,
  screenRoom,
};

enum eMenuOptions {
  menuLimits,
  menuDateTime,
  menuSound,
  menuRoom,
  menuExit,
};

eScreens screen = screenHome;

const char * menu [] = {"Limity", "Ustaw czas", "Dzwiek", "Wybierz pokoj", "Wyjscie"};
const char * rooms [] = {"Pokoj", "Dzieciecy", "Kuchnia", "Lazienka"};
const char * currentRoom = rooms[0];

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

void ScreenMenu()
{
  ScreenSelect("::Menu::",menu,array_sizeof(menu));
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
  lcdInit();
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

  /// start bluetooth
  bluetooth.begin(9600);
  
  /// series of blinks 
  for (int i = 0; i < 20; ++i)
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  digitalWrite(LED, 1);

  currentRoom = rooms[ ScreenSelect("Wybierz pokoj:",rooms,array_sizeof(rooms))];
  lcd.clear();
}

void loop() {
  switch (screen)
  {
    case screenHome:
    {
      ScreenHome();
      if (GetKeyState() == eKeyState::keyOK)
      {
        screen = screenMenu;
      }
      break;
    }
    case screenMenu:
    {
      eMenuOptions menuOption = (eMenuOptions)ScreenSelect("::Menu::",menu,array_sizeof(menu));
      switch (menuOption)
      {
        case menuLimits:
        {
          break;
        }
        case menuSound:
        {
          break;
        }
        case menuDateTime:
        {
          break;
        }
        case menuRoom:
        {
          screen = screenRoom;
          break;
        }
        case menuExit:
        {
          screen = screenHome;
          break;
        }
      }
      break;
    }
    case screenRoom:
    {
      currentRoom = rooms[ ScreenSelect("Wybierz pokoj:",rooms,array_sizeof(rooms))];
      screen = screenMenu;
      break;
    }
  }
  delay(1000); //1s

  if (isTimeout(measureTimeout))
  {
    GetMeasurements();

    // DISPLAY DATA
    Serial.print(humidity.value);
    Serial.print(",\t");
    Serial.println(temperature.value);

    // Bluetooth DATA
    bluetooth.print("E");
    bluetooth.print(humidity.value);
    bluetooth.print(",");
    bluetooth.print(temperature.value);
    bluetooth.print("\n");
  }

}


