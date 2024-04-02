/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 UART2 functions header       */
/*                                                  */
/****************************************************/

#ifndef __UART_H
#define __UART_H

#include <p33Fxxxx.h>
#include "types.h"

#define UART_ERR -1
#define UART_SUC 0

#define BIT9		0x06
#define BIT8_ODD	0x04
#define BIT8_EVEN	0x02
#define BIT8_NO		0x00

#define BIT_STOP_1	0x00
#define BIT_STOP_2	0x01

#define CTRL_SIMPLE	0x00
#define CTRL_FLOW	0x01

#define TX_INT_SINGLE	0x8000
#define TX_INT_EMPTY	0x2000
#define TX_INT_LAST	0x0000

#define RX_INT_FULL	0x00C0
#define RX_INT_3OF4	0x0080
#define RX_INT_SINGLE	0x0040

#define BAUD_9600 9600
#define BAUD_115200 115200

/* function prototypes - implement in uart.c file */
inline void uart2_init(uint16_t baud);
void uart2_send_8(int8_t data);
int8_t uart2_recv(uint8_t* data);
void uart2_flush();
int8_t uart2_recv_start_byte();

#endif
