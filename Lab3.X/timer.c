/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 timer initialization func.   */
/*                                                  */
/****************************************************/


#include "timer.h"

//period in ms
void set_timer1(uint16_t period){
    //enable LPOSCEN
    __builtin_write_OSCCONL(OSCCONL | 2); // 
    T1CONbits.TON = 0; //Disable Timer
    T1CONbits.TCS = 1; //Select external clock
    T1CONbits.TSYNC = 0; //Disable Synchronization
    T1CONbits.TCKPS = 0b00; //Select 1:1 Prescaler
    TMR1 = 0x00; //Clear timer register
    PR1 = period; //Load the period value, to 1 second
    
    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;// Enable Timer1 interrupt
}

void set_timer2() {
    CLEARBIT(T2CONbits.TON); // turn off timer2
    CLEARBIT(T2CONbits.TCS); // internal clk
    CLEARBIT(T2CONbits.TGATE); //disable gated timer mode
    TMR2 = 0x00; // reset Timer 2 counter
    T2CONbits.TCKPS = 0b11; // use 1:256 prescaler
    CLEARBIT(IFS0bits.T2IF); // clear timer2 interrupt flag
    CLEARBIT(IEC0bits.T2IE);  // disable timer2 interrupt enable control bit
    PR2 = 50000; // set period to 1s
    
    IPC1bits.T2IP = 0x01; // set Timer 2 IPL to 1
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;// Enable Timer2 interrupt
}

void start_timer2() {
    TMR2 = 0x00; // reset Timer 2 counter
    
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;// Enable Timer2 interrupt
    T2CONbits.TON = 1;  // turn on timer
}

void stop_timer2() {
    IEC0bits.T2IE = 0;
    T2CONbits.TON = 0;
}

