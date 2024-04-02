/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file to implement basic  */
/*                macros and functions to interface */
/*                with the on-board LEDs.           */
/*                                                  */
/****************************************************/

#ifndef __LED_H
#define __LED_H

// Tri-state registers
#define LEDTRIS		TRISA
#define LED1_TRIS	TRISAbits.TRISA4
#define LED2_TRIS	TRISAbits.TRISA5
#define LED3_TRIS	TRISAbits.TRISA9
#define LED4_TRIS	TRISAbits.TRISA10
#define LED5_TRIS	TRISAbits.TRISA0

// Port registers using predefined structs
#define LEDPORT		PORTA
#define	LED1_PORT	PORTAbits.RA4
#define	LED2_PORT	PORTAbits.RA5
#define LED3_PORT	PORTAbits.RA9
#define LED4_PORT	PORTAbits.RA10
#define LED5_PORT	PORTAbits.RA0

// LEDPORT Bitwise definitions
#define LED1	4
#define LED2	5
#define LED3	9
#define LED4	10
#define LED5	0

//LED init
#define led_initialize() \
    do {                 \
        CLEARLED(LED1_TRIS); \
        CLEARLED(LED2_TRIS); \
        CLEARLED(LED3_TRIS); \
        CLEARLED(LED4_TRIS); \
        CLEARLED(LED5_TRIS); \
        LEDPORT &= ~(BV(4) | BV(5)| BV(9)| BV(10) | BV(0)); \
    } while (0)

#endif
