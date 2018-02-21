/***************************************************************************//**
 * @file     timeout.h
 * @brief    DESCRIPTION - header file
 * @author   Sławomir Paszko <s.paszko@softint.pl>
 * @version  1.00
 * @date     YYYY-MM-DD
 *
 ***************************************************************************/

#ifndef TIMEOUT_H_
#define TIMEOUT_H_


typedef struct {
  unsigned long start;
  unsigned long duration;
} sTimeout;

bool isTimeout(sTimeout &t);
void setTimeout(sTimeout &t);



#endif /* TIMEOUT_H_ */

// -----------------------------------------------------------------------------
// End of file -----------------------------------------------------------------
// -----------------------------------------------------------------------------
