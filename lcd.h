/***************************************************************************//**
 * @file     lcd.h
 * @brief    DESCRIPTION - header file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *
 *
 ***************************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include <LiquidCrystal.h> //Dołączenie bilbioteki
#include <stdint.h>

enum lcdExtraCharacter {
    charStable = 0,
    charArrowUp = 1,
    charArrowDown = 2,
};

void lcdInit();
void ShowText(uint8_t lineNumber, const char * text);
void ShowMenuText(uint8_t lineNumber, uint8_t menuPosition, const char* text);
uint8_t ScreenSelect(const char * title, const char **texts, uint8_t numberOfTexts);
uint8_t ScreenValue(const char* title, int8_t& value, int8_t minValue, int8_t maxValue);



#endif /* LCD_H_ */

// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
