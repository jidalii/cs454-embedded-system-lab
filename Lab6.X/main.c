/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: simple HelloWorld application     */
/*                for Amazing Ball platform         */
/*                                                  */
/****************************************************/

#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <time.h>
#include <libpic30.h>
//#include <uart.h>
#include <xc.h>
#include "lcd.h"
#include "types.h"
#include "joystick.h"
#include "adc.h"
#include "touch.h"

#define X_MAX_PROMPT "x-axis max: "
#define X_MIN_PROMPT "x-axis min: "
#define Y_MAX_PROMPT "y-axis max: "
#define Y_MIN_PROMPT "y-axis min: "

#define X_MAX_STATE 0
#define X_MIN_STATE 1
#define Y_MAX_STATE 2
#define Y_MIN_STATE 3

#define X_CH 1
#define Y_CH 0

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

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
        OC7R = 1000; /* Set the initial duty cycle to 5ms*/
        OC7RS = 1000; /* Load OCRS: next pwm duty cycle */
        OC7CON = 0x0006; /* Set OC7: PWM, no fault check, Timer2 */
    }
    SETBIT(T2CONbits.TON); /* Turn Timer 2 on */
    

    
}

void motor_set_duty(uint16_t x_duty_us, uint16_t y_duty_us) {
    
    //  x-axis: RB15, OCR8
    CLEARBIT(TRISDbits.TRISD7); /* Set OC8 as output */
    OC8RS = (20000-x_duty_us)/5; /* Load OCRS: next pwm duty cycle */
    OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */

    //    y: OCR7
    CLEARBIT(TRISDbits.TRISD6); /* Set OC7 as output */
    OC7RS = (20000-y_duty_us)/5; /* Load OCRS: next pwm duty cycle */
    OC7CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
}

void move_servo(uint8_t state) {
    uint16_t x_duty_us, y_duty_us;
    if(state == 0) {
        x_duty_us = 2100; // -90 angle
        y_duty_us = 2100; // -90 angle      
    } 
    else if (state == 1) {
        x_duty_us = 900; // 90 angle
        y_duty_us = 2100; // -90 angle      
    }
    else if (state == 2) {
        x_duty_us = 900; // 90 angle
        y_duty_us = 900; // 90 angle      
    }
    else {
        x_duty_us = 2100; // -90 angle
        y_duty_us = 900; // 90 angle    
    }
    motor_set_duty(x_duty_us, y_duty_us);
    __delay_ms(2000); // delay 2s to settle down
}

void swap(uint16_t* a, uint16_t* b){
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(uint16_t* arr, int8_t l, int8_t r){
    if(r>l) return;
    int8_t i = l;
    int8_t j = l-1;
    uint16_t pivot = arr[r];
    
    while (i<r){
        if(arr[i]<=pivot){
            j++;
            swap(&arr[i], &arr[j]);
        }
        i++;
    }
    j++;
    swap(&arr[j], &arr[r]);
    quicksort(arr, l, j-1);
    quicksort(arr, j+1, r);
}

int main(){

	/* LCD Initialization Sequence */
	lcd_initialize();
	lcd_clear();
    
    // init motor
    motor_init(X_CH);
    motor_init(Y_CH);
    
    // init touch screen
    touch_init();

    uint8_t state = 0;
    while(state<4){
        move_servo(state);
        uint8_t count = 0;
        uint16_t x_pos[5];
        uint16_t y_pos[5];
        
        touch_select_dim(X_CH);
        while(count<5){
            x_pos[count] = touch_read();
            count++;
            __delay_ms(100);
        }
        
        count = 0;
        touch_select_dim(Y_CH);
        while(count<5){
            y_pos[count] = touch_read();
            count++;
            __delay_ms(100);
        }
        quicksort(x_pos, 0, 4);
        quicksort(y_pos, 0, 4);
        
        lcd_clear_row(state);
        lcd_locate(0, state);
        lcd_printf("corner %d : %d, %d", state+1, x_pos[2], y_pos[2]);

        state++;
        state %= 4;
    }
        
    
    return 0;
}
