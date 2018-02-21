/***************************************************************************//**
 * @file     bluetooth.cpp
 * @brief    DESCRIPTION - source file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *

 *
 ******************************************************************************/

#include "bluetooth.h"
#include <wiring_private.h>

SoftwareSerial bluetooth(A0, A1); // RX, TX - odwrotnie

void bluetooth_setName(const char * name)
{
  char cmdBuffer[30];
  sprintf(cmdBuffer,"AT+NAME=HomeMeter-%s\r\n",name);

  bluetooth.print(cmdBuffer);
}



// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
