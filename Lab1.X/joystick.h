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

#define BOT1TRIS	TRISE
#define BOT1_TRIS	TRISEbits.TRISE8
#define BOT2TRIS	TRISD
#define BOT2_TRIS	TRISDbits.TRISD10

#define BOT1PORT	PORTE
#define	BOT1_PORT	PORTEbits.RE8
#define BOT2PORT	PORTD
#define	BOT2_PORT	PORTDbits.RD10

// Set port as input
#define bot_initialize() \
    do {                 \
        AD1PCFGHbits.PCFG20 = 1;\
        INTCON2bits.INT1EP = 1;\
        BOT1_TRIS = 1;\
        BOT2_TRIS = 1;\
        BOT1PORT = 0;\
        BOT2PORT = 0;\
        
    } while (0)



/* Please NOTE: */
/* BTN1 (BUTTON 1) is the "fire" button on the koystick */
/* BTN2 (BUTTON 2) is the "thumb" button on the koystick */

//#error BTN1_PRESSED is not implemented. Implement it below and delete this line
#define BTN1_PRESSED() \
    BOT1_PORT == 0
    /* insert expression to test if BTN1 is pressed here */
   
//#error BTN1_RELEASED is not implemented. Implement it below and delete this line
#define BTN1_RELEASED() \
    BOT1_PORT != 0
    /* insert expression to test if BTN1 is released here */

//#error BTN2_PRESSED is not implemented. Implement it below and delete this line
#define BTN2_PRESSED() \
    BOT2_PORT == 0
    /* insert expression to test if BTN2 is pressed here */

//#error BTN2_RELEASED is not implemented. Implement it below and delete this line
#define BTN2_RELEASED() \
    BOT2_PORT != 0
    /* insert expression to test if BTN2 is released here */

/* === Function prototypes implemented in joystic.h go here === */


#endif /* __JOYSTICK_H */


