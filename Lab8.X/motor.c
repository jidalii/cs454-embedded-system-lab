/*
 * File:   adc.c
 * Author: team-3b
 *
 * Created on March 29, 2024, 10:25 AM
 */

#include <p33Fxxxx.h>
#include "types.h"

void motor_init(uint8_t chan) {
    //setup Timer 2
    CLEARBIT(T2CONbits.TON); // Disable Timer
    CLEARBIT(T2CONbits.TCS); // Select internal instruction cycle clock
    CLEARBIT(T2CONbits.TGATE); // Disable Gated Timer mode
    TMR2 = 0x00; // Clear timer register
    T2CONbits.TCKPS = 0b10; // Select 1:64 Prescaler
    CLEARBIT(IFS0bits.T2IF); // Clear Timer2 interrupt status flag
    CLEARBIT(IEC0bits.T2IE); // Disable Timer2 interrupt enable control bit
    PR2 = 4000; // Set timer period 20ms:
    
    if (chan == 1) {
        //setup OC8
        CLEARBIT(TRISDbits.TRISD7); /* Set OC8 as output */
        OC8R = 1000; /* Set the initial duty cycle to 5ms*/
        OC8RS = 1000; /* Load OCRS: next pwm duty cycle */
        OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    } else {
        //setup OC7
        CLEARBIT(TRISDbits.TRISD6); /* Set OC7 as output */
        OC7R = (20000-900)/5; /* Set the initial duty cycle to 5ms*/
        OC7RS = (20000-900)/5; /* Load OCRS: next pwm duty cycle */
        OC7CON = 0x0006; /* Set OC7: PWM, no fault check, Timer2 */
    }
    SETBIT(T2CONbits.TON); /* Turn Timer 2 on */
    
  
}

void motor_set_duty(uint16_t duty_us, uint8_t chan) {
    if(chan == 1) {
        CLEARBIT(TRISDbits.TRISD7); /* Set OC8 as output */
        OC8RS = (20000-duty_us)/5; /* Load OCRS: next pwm duty cycle */
        OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    } else {
        //    y: OCR7
        CLEARBIT(TRISDbits.TRISD6); /* Set OC7 as output */
        OC7RS = (20000-duty_us)/5; /* Load OCRS: next pwm duty cycle */
        OC7CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    }
}
