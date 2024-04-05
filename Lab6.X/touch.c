/*
 * File:   touch.c
 * Author: team-3b
 *
 * Created on April 4, 2024, 5:12 PM
 */


#include <p33Fxxxx.h>
#include "types.h"



void touch_init(void) {
    CLEARBIT(AD1CON1bits.ADON);
    
    //set up the I/O pins E1, E2, E3 to be output pins
    CLEARBIT(TRISEbits.TRISE1); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE2); //I/O pin set to output
    CLEARBIT(TRISEbits.TRISE3); //I/O pin set to output
    
    CLEARBIT(AD1PCFGLbits.PCFG15); //set x-axis input pin as analog
    CLEARBIT(AD1PCFGLbits.PCFG9); //set y-axis input pin as analog
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

void touch_select_dim(uint8_t dimension)
{
    // x-axis
    if (dimension == 1) {
        //set up the I/O pins E1, E2, E3 so that the touchscreen X-coordinate pin
        //connects to the ADC
        CLEARBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        SETBIT(LATEbits.LATE3);
        AD1CHS0bits.CH0SA = 0x0F; //set ADC to Sample RB15 pin
        
    }
    // y-axis
    else {
        SETBIT(LATEbits.LATE1);
        CLEARBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);
        AD1CHS0bits.CH0SA = 0x09; //set ADC to Sample RB9 pin
    
    }
}

uint16_t touch_read(void) {
    SETBIT(AD1CON1bits.SAMP); //start to sample
    while(!AD1CON1bits.DONE); //wait for conversion to finish
    CLEARBIT(AD1CON1bits.DONE); //MUST HAVE! clear conversion done bit
    return ADC1BUF0; //return sample
}
