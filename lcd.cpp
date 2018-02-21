/***************************************************************************//**
 * @file     lcd.cpp
 * @brief    DESCRIPTION - source file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *
 *
 ******************************************************************************/

#include "lcd.h"
#include "keys.h"
#include <wiring_private.h>

/// driver
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Informacja o podłączeniu nowego wyświetlacza

/// one line variable
char line[16];

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


void ShowText(uint8_t lineNumber, const char * text)
{
  sprintf(line,"%-16s",text);
  lcd.setCursor(0,lineNumber);
  lcd.print(line);
}

void ShowMenuText(uint8_t lineNumber, uint8_t menuPosition, const char* text)
{
  sprintf(line,"%d.%-16s",menuPosition,text);
  lcd.setCursor(0,lineNumber);
  lcd.print(line);
}

void lcdInit()
{
  /// init LCD 2x16
  lcd.createChar((uint8_t)charStable, stable);
  lcd.createChar((uint8_t)charArrowUp, arrowUp);
  lcd.createChar((uint8_t)charArrowDown, arrowDown);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
}

uint8_t ScreenSelect(const char * title, const char **texts, uint8_t numberOfTexts)
{
  int currentPosition = 0;
  int maxPosition = numberOfTexts - 1;
  eKeyState key = keyNone;

  lcd.clear();
  ShowText(0,title);
  while (key != keyOK)
  {

    ShowMenuText(1,currentPosition,texts[currentPosition]);
//    delay(100);
    switch (key = GetKeyState())
    {
      case keyUp :
      {
        if (currentPosition == maxPosition)
        {
          currentPosition = 0;
        }
        else
        {
          ++currentPosition;
        }
        break;
      }
      case keyDown :
      {
        if (currentPosition == 0)
        {
          currentPosition = maxPosition;
        }
        else
        {
          --currentPosition;
        }
        break;
      }
    }
  }
  return currentPosition;
}

uint8_t ScreenValue(const char* title, int8_t& value, int8_t minValue,
                    int8_t maxValue)
{
  char valueText[16];
  eKeyState key = keyNone;
  lcd.clear();
  ShowText(0,title);
  while (key != keyOK)
  {
    sprintf(line, "%d [%d..%d]",value, minValue, maxValue);
    ShowText(1,line);
    switch (key = GetKeyState())
    {
      case keyUp :
      {
        if (value < maxValue)
        {
          ++value;
        }
        break;
      }
      case keyDown :
      {
        if (value > minValue)
        {
          --value;
        }
        break;
      }
    }
  }
  return value;
}
// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
