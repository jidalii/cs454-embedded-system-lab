/*
 * File:   adc.c
 * Author: team-3b
 *
 * Created on March 29, 2024, 10:25 AM
 */

#include <p33Fxxxx.h>
#include "types.h"

void init_adc() {
//    AN15
    //disable ADC
    CLEARBIT(AD1CON1bits.ADON);
    
    CLEARBIT(AD1PCFGLbits.PCFG15); //set x-axis input pin as analog
    CLEARBIT(AD1PCFGLbits.PCFG15); //set y-axis input pin as analog
    //Configure AD1CON1
    SETBIT(AD1CON1bits.AD12B); //set 12b Operation Mode
    AD1CON1bits.FORM = 0; //set integer output
    AD1CON1bits.SSRC = 0x7; //set automatic conversion
    //Configure AD1CON2
    AD1CON2 = 0; //not using scanning sampling
    //Configure AD1CON3
    SETBIT(AD1CON3bits.ADRC); //internal clock source
    AD1CON3bits.SAMC = 0x1F; //sample-to-conversion clock = 31Tad
    AD1CON3bits.ADCS = 0x2; //Tad = 3Tcy (Time cycles)
    //Leave AD1CON4 at its default value
    //enable ADC
    SETBIT(AD1CON1bits.ADON);
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