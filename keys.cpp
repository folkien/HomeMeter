/***************************************************************************//**
 * @file     keys.cpp
 * @brief    DESCRIPTION - source file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *

 *
 ******************************************************************************/
#include "keys.h"
#include <stdint.h>
#include <wiring_private.h>

#define keyPressed 1
#define keyReleased 0

/// 1 - pressed, 0 - released
uint8_t keyStates[3] = {0,0,0};

eKeyState GetKeyState()
{
  uint8_t keyState[3];
  eKeyState returnedKeyState = keyNone;
  
  keyState[0] = !digitalRead(KEY_OK);
  keyState[1] = !digitalRead(KEY_UP);
  keyState[2] = !digitalRead(KEY_DOWN);
  delay(20);
  keyState[0] &= !digitalRead(KEY_OK);
  keyState[1] &= !digitalRead(KEY_UP);
  keyState[2] &= !digitalRead(KEY_DOWN);

  /// only one key could be pressed
  if ((keyStates[0] == keyPressed) && (keyState[0] == keyReleased))
  {
    returnedKeyState = keyOK;
  }
  else if ((keyStates[1] == keyPressed) && (keyState[1] == keyReleased))
  {
    returnedKeyState = keyUp;
  }
  else if ((keyStates[2] == keyPressed) && (keyState[2] == keyReleased))
  {
    returnedKeyState = keyDown;
  }
  
  /// copy new keyState to global array
  keyStates[0] = keyState[0];
  keyStates[1] = keyState[1];
  keyStates[2] = keyState[2];
  
  return returnedKeyState;
}



// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
