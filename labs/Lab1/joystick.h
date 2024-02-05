/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file to implement basic  */
/*                macros and functions to interface */
/*                with the analog joystick.         */
/*                                                  */
/****************************************************/

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <p33Fxxxx.h>
#include "types.h"

/* Please NOTE: */
/* BTN1 (BUTTON 1) is the "fire" button on the koystick */
/* BTN2 (BUTTON 2) is the "thumb" button on the koystick */

#error BTN1_PRESSED is not implemented. Implement it below and delete this line
#define BTN1_PRESSED() \
    (/* insert expression to test if BTN1 is pressed here */)

#error BTN1_RELEASED is not implemented. Implement it below and delete this line
#define BTN1_RELEASED() \
    (/* insert expression to test if BTN1 is released here */)

#error BTN2_PRESSED is not implemented. Implement it below and delete this line
#define BTN2_PRESSED() \
    (/* insert expression to test if BTN2 is pressed here */)

#error BTN2_RELEASED is not implemented. Implement it below and delete this line
#define BTN2_RELEASED() \
    (/* insert expression to test if BTN2 is released here */)

/* === Function prototypes implemented in joystic.h go here === */


#endif /* __JOYSTICK_H */


