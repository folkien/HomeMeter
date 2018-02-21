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

eKeyState GetKeyState()
{
  uint8_t keyState[3];

  keyState[0] = !digitalRead(KEY_OK);
  keyState[1] = !digitalRead(KEY_UP);
  keyState[2] = !digitalRead(KEY_DOWN);
  delay(20);
  keyState[0] &= !digitalRead(KEY_OK);
  keyState[1] &= !digitalRead(KEY_UP);
  keyState[2] &= !digitalRead(KEY_DOWN);

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



// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
