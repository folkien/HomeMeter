/***************************************************************************//**
 * @file     keys.h
 * @brief    DESCRIPTION - header file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *
 *
 ***************************************************************************/

#ifndef KEYS_H_
#define KEYS_H_

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

eKeyState GetKeyState();


#endif /* KEYS_H_ */

// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
