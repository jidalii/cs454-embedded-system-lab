/*
 * File:   time.c
 * Author: team-3b
 *
 * Created on April 12, 2024, 11:01 AM
 */

#include <p33Fxxxx.h>
#include "types.h"

void set_timer3() {
    CLEARBIT(T3CONbits.TON); // turn off timer2
    CLEARBIT(T3CONbits.TCS); // internal clk
    CLEARBIT(T3CONbits.TGATE); //disable gated timer mode
    TMR3 = 0x00; // reset Timer 2 counter
    T3CONbits.TCKPS = 0b11; // use 1:256 prescaler
    CLEARBIT(IFS0bits.T3IF); // clear timer2 interrupt flag
    CLEARBIT(IEC0bits.T3IE);  // disable timer2 interrupt enable control bit
    PR3 = 1250; // set period to 1s 0.025s (25 ms))
    
    IPC2bits.T3IP = 0x01; // set Timer 2 IPL to 1
    IFS0bits.T3IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T3IE = 0;// Enable Timer2 interrupt
}

void start_timer3() {
    TMR3 = 0x00; // reset Timer 2 counter
    IFS0bits.T3IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T3IE = 1;// Enable Timer2 interrupt
    T3CONbits.TON = 1;  // turn on timer
}

void stop_timer3() {
    IEC0bits.T3IE = 0;
    T3CONbits.TON = 0;
}