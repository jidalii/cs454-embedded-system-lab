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

#define X_MAX_PROMPT "x-axis max: "
#define X_MIN_PROMPT "x-axis min: "
#define Y_MAX_PROMPT "y-axis max: "
#define Y_MIN_PROMPT "y-axis min: "

#define X_MAX_STATE 0
#define X_MIN_STATE 1
#define Y_MAX_STATE 2
#define Y_MIN_STATE 3

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  


uint16_t x_max = 2000;
uint16_t x_min = 2000;
uint16_t y_max = 2000;
uint16_t y_min = 2000;
uint16_t x_cur_pos = 2000;
uint16_t y_cur_pos = 2000;

uint16_t x_middle_duty;
uint16_t y_middle_duty;
uint16_t X_duty = 20001;
uint16_t Y_duty = 20001;



void print_bound() {
    lcd_clear();
    lcd_locate(0,0);
    if(x_max==2000){
        lcd_printf("%s", X_MAX_PROMPT);
    }
    else{
        lcd_printf("%s %d", X_MAX_PROMPT, x_max);
    }
    
    lcd_locate(0,1);
    if(x_min==2000){
        lcd_printf("%s", X_MIN_PROMPT);
    }
    else{
        lcd_printf("%s %d", X_MIN_PROMPT, x_min);
    }
    
    lcd_locate(0,2);
    if(y_max==2000){
        lcd_printf("%s", Y_MAX_PROMPT);
    }
    else{
        lcd_printf("%s %d", Y_MAX_PROMPT, y_max);
    }
    
    lcd_locate(0,3);
    if(y_min==2000){
        lcd_printf("%s", Y_MIN_PROMPT);
    }
    else{
        lcd_printf("%s %d", Y_MIN_PROMPT, y_min);
    }
    lcd_locate(0,4);
    if(X_duty==20001){
        lcd_printf("%s", "PW X:");
    }
    else{
        lcd_printf("%s %d", "PW X:", X_duty);
    }
    lcd_locate(0,5);
    if(Y_duty==20001){
        lcd_printf("%s", "PW Y:");
    }
    else{
        lcd_printf("%s %d", "PW Y:", Y_duty);
    }
}

void set_bound(uint8_t state){
    uint8_t bnt1_state_current = 0x0;
    uint8_t bnt1_state_previous = 0x0;
    uint8_t bnt1_state_count = 0;
    uint8_t row;
    uint16_t* bound;
    char* prompt;
    
    switch (state) {
    case X_MAX_STATE:
        row = 0;
        bound = &x_max;
        prompt = X_MAX_PROMPT;
        break;
    case X_MIN_STATE:
        row = 1;
        bound = &x_min;
        prompt = X_MIN_PROMPT;
        break;
    case Y_MAX_STATE:
        row = 2;
        bound = &y_max;
        prompt = Y_MAX_PROMPT;
        break;
    case Y_MIN_STATE:
        row = 3;
        bound = &y_min;
        prompt = Y_MIN_PROMPT;
        break;
    }
    
    while(1) {
        if(state==X_MAX_STATE || state==X_MIN_STATE){
            *bound = sample_axis_x();
        } else{
            *bound = sample_axis_y();
        }
        print_bound();
        bnt1_state_previous = bnt1_state_current;
        bnt1_state_current  = BTN1_PRESSED();
        //debouncing
        if(bnt1_state_current != bnt1_state_previous){
            if (bnt1_state_count > 10){
                if (bnt1_state_current == 1){
                    break;
                }
            }
            bnt1_state_count = 0;
        }else{
            bnt1_state_count += 1;
        }
        __delay_ms(100);
    }
}

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
        OC8RS = (20000-x_middle_duty)/5; /* Load OCRS: next pwm duty cycle */
        OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    } else {
        //setup OC7
        CLEARBIT(TRISDbits.TRISD6); /* Set OC7 as output */
        OC7R = 1000; /* Set the initial duty cycle to 5ms*/
        OC7RS = (20000-y_middle_duty)/5; /* Load OCRS: next pwm duty cycle */
        OC7CON = 0x0006; /* Set OC7: PWM, no fault check, Timer2 */
    }
    SETBIT(T2CONbits.TON); /* Turn Timer 2 on */
    

    
}

void motor_set_duty(uint8_t chan, uint16_t duty_us) {
    
//  x-axis: RB15, OCR8
    if (chan == 1) {
        CLEARBIT(TRISDbits.TRISD7); /* Set OC8 as output */
        OC8RS = (20000-duty_us)/5; /* Load OCRS: next pwm duty cycle */
        OC8CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    } 
//    y: OCR7
    else {
        CLEARBIT(TRISDbits.TRISD6); /* Set OC7 as output */
        OC7RS = (20000-duty_us)/5; /* Load OCRS: next pwm duty cycle */
        OC7CON = 0x0006; /* Set OC8: PWM, no fault check, Timer2 */
    }
}

uint16_t pos_to_duty(uint8_t chan, uint16_t cur_pos) {
    
    uint16_t max_pos;
    uint16_t min_pos;
    
    if(chan==1){
        max_pos = x_max;
        min_pos = x_min;
    } else{
        max_pos = y_max;
        min_pos = y_min; 
    }
    uint16_t duty_us = (uint16_t) (900 + (double)(cur_pos - min_pos) / (max_pos - min_pos) * 1200); // in us
    return duty_us;
}

void set_screen(uint8_t state){
    uint8_t bnt1_state_current = 0x0;
    uint8_t bnt1_state_previous = 0x0;
    uint8_t bnt1_state_count = 0;
    
    while(1) {
        if(state==0){
            x_cur_pos = sample_axis_x();
            y_cur_pos = sample_axis_y();
            X_duty = pos_to_duty(1, x_cur_pos);
            motor_set_duty(1, X_duty);
            Y_duty = pos_to_duty(1, y_cur_pos);
            motor_set_duty(2, Y_duty);
        } else{
            y_cur_pos = sample_axis_y();
            Y_duty = pos_to_duty(1, y_cur_pos);
            motor_set_duty(2, Y_duty);
        }
        
        print_bound();
        bnt1_state_previous = bnt1_state_current;
        bnt1_state_current  = BTN1_PRESSED();
        //debouncing
        if(bnt1_state_current != bnt1_state_previous){
            if (bnt1_state_count > 10){
                if (bnt1_state_current == 1){
                    break;
                }
            }
            bnt1_state_count = 0;
        }else{
            bnt1_state_count += 1;
        }
        __delay_ms(100);
    }
}

int main(){

	/* LCD Initialization Sequence */
	lcd_initialize();
	lcd_clear();

    // init joy stick
    bot_initialize();
    init_adc();
    
    print_bound();
    
    uint8_t state = 0; //X_max: 0; X_min: 1; Y_max: 2; Y_min: 3;
    
    // find the bound of X and Y
    while(state<4){
        set_bound(state);
        state++;
    }
    
    x_middle_duty = pos_to_duty(1, (x_max+x_min)/2);
    y_middle_duty = pos_to_duty(2, (y_max+y_min)/2);

   
    motor_init(1);
    motor_init(2);
    
    uint16_t pos_state = 0;
    while(pos_state<2){
        set_screen(pos_state);
        pos_state++;
    }
    
    while(1){}
    return 0;
}

