/***************************************************************************//**
 * @file     timeout.cpp
 * @brief    DESCRIPTION - source file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *
 *
 ******************************************************************************/

#include "timeout.h"
#include <wiring_private.h>

bool IsTimeout(sTimeout &t)
{
  return ((millis() - t.start) > t.duration);
}

void SetTimeout(sTimeout &t)
{
  t.start = millis();
}
// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
