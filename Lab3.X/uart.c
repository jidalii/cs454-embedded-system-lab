/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 UART2 init,TX,RX functions   */
/*                                                  */
/****************************************************/

#include "uart.h"

inline void uart2_init(uint16_t baud)
{
	/* Implement me please. */
    /* Stop UART port */
    CLEARBIT(U2MODEbits.UARTEN); //Disable UART for configuration
    /* Disable Interrupts */
    IEC1bits.U2RXIE = 0;
    IEC1bits.U2TXIE = 0;
    /* Clear Interrupt flag bits */
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    
    /* Set IO pins */
//    TRISFbits.TRISF2 = 0; //set as input UART2 RX pin
//    TRISFbits.TRISF3 = 0; //set as output UART2 TX pin
    TRISFbits.TRISF4 = 1; //set as input UART2 RX pin
    TRISFbits.TRISF5 = 0; //set as output UART2 TX pin
    
    /* baud rate */
    // use the following equation to compute the proper
    // setting for a specific baud rates
    U2MODEbits.BRGH = 0; //Set low speed baud rate

    U2BRG = (uint32_t) 800000 / baud - 1; //Set the baudrate to be at baud
    //U2BRG = (uint32_t) 12800000 / baud /16 - 1; 
//    U2BRG = (uint32_t) 32768 / baud / 4 - 1;
    /* Operation settings and start port */
    U2MODE = 0; // 8-bit, no parity and, 1 stop bit
    U2MODEbits.RTSMD = 0; //select simplex mode
    U2MODEbits.UEN = 0; //select simplex mode
    U2MODE |= 0x00;
    U2MODEbits.UARTEN = 1; //enable UART
    U2STA = 0;
    U2STAbits.UTXEN = 1; //enable UART TX
}

void uart2_send_8(int8_t data)
{
	/* Implement me please. */
    while (U2STAbits.UTXBF);
    U2TXREG = data;
    while(!U2STAbits.TRMT);
    
}

int8_t uart2_recv(uint8_t* data)
{
	/* Implement me please. */
    if (U2STAbits.OERR) {
        U2STAbits.OERR = 0;
//        return -1;
    }
    if (U2STAbits.URXDA) {
        *data = U2RXREG & 0x00FF;
        return 0;
    }
    return -1;
}

void uart2_flush() {
    uint8_t temp = 0;
    while(uart2_recv(&temp) == 0){}
}

int8_t uart2_recv_start_byte() {
    uint8_t start_byte=1;
    while(uart2_recv(&start_byte) != 0){}
    
    if(start_byte == 0) {
        return 0;
    } else {
        return -1;
    }
}