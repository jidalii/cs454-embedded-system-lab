/*
 * File:   adc.c
 * Author: team-3b
 *
 * Created on March 29, 2024, 10:25 AM
 */

#include <p33Fxxxx.h>
#include "types.h"

void init_adc() {
    //disable ADC
    CLEARBIT(AD2CON1bits.ADON);
    //initialize PIN
    SETBIT(TRISEbits.TRISE8); //set TRISE RE8 to input
    
    CLEARBIT(AD2PCFGLbits.PCFG4); //set x-axis input pin as analog
    CLEARBIT(AD2PCFGLbits.PCFG5); //set y-axis input pin as analog
    //Configure AD1CON1
    SETBIT(AD2CON1bits.AD12B); //set 10b Operation Mode 0:10bits; 1:12bits
    AD2CON1bits.FORM = 0; //set integer output
    AD2CON1bits.SSRC = 0x7; //set automatic conversion
    //Configure AD1CON2
    AD2CON2 = 0; //not using scanning sampling
    //Configure AD1CON3
    CLEARBIT(AD2CON3bits.ADRC); //internal clock source
    AD2CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
    AD2CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
    //Leave AD1CON4 at its default value
    //enable ADC
    SETBIT(AD2CON1bits.ADON);
}

uint16_t sample_axis_x() {
    AD2CHS0bits.CH0SA = 0x004; //set ADC to Sample RB4 pin
    SETBIT(AD2CON1bits.SAMP); //start to sample
    while(!AD2CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD2CON1bits.DONE); //MUST HAVE! clear conversion done bit
    return ADC2BUF0; //return sample
}

uint16_t sample_axis_y() {
    AD2CHS0bits.CH0SA = 0x005; //set ADC to Sample RB5 pin
    SETBIT(AD2CON1bits.SAMP); //start to sample
    while(!AD2CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD2CON1bits.DONE); //MUST HAVE! clear conversion done bit
    return ADC2BUF0; //return sample
}