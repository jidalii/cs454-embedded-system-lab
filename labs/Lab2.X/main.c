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
#include <uart.h>
#include <xc.h>
#include "led.h"
#include "lcd.h"
#include "types.h"
#include "joystick.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

uint8_t timer2_cnt = 0;
uint32_t static iteration = 0;


volatile unsigned int seconds_counter = 0;
volatile unsigned int ms_counter = 0;

// for Q3
void __attribute__((__interrupt__)) _T1Interrupt(void) {
    // 1/32768 * 32768 = 1s
    LED2_PORT ^= 1;
    seconds_counter ++;
    TMR2 = 0x0;
    IFS0bits.T1IF = 0;
}

// for Q2
void __attribute__((__interrupt__)) _T2Interrupt(void) {
    // interrupt every 1ms, so we accumulate 5 times
    if (++timer2_cnt == 5) {
       LED1_PORT ^= 1;
       timer2_cnt = 0;
    }
    ms_counter ++;
    IFS0bits.T2IF = 0; 
    
}

void __attribute__((__interrupt__)) _INT1Interrupt(void) {
    lcdReset();
    IEC1bits.INT1IE = 0;
}

void setTimer1(){
    //enable LPOSCEN
    __builtin_write_OSCCONL(OSCCONL | 2); // 
    T1CONbits.TON = 0; //Disable Timer
    T1CONbits.TCS = 1; //Select external clock
    T1CONbits.TSYNC = 0; //Disable Synchronization
    T1CONbits.TCKPS = 0b00; //Select 1:1 Prescaler
    TMR1 = 0x00; //Clear timer register
    PR1 = 32768; //Load the period value, to 1 second
    
    IPC0bits.T1IP = 0x01; // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag
    IEC0bits.T1IE = 1;// Enable Timer1 interrupt
}

void setTimer2() {
    CLEARBIT(T2CONbits.TON);
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);
    TMR2 = 0x00; // reset Timer 2 counter
    T2CONbits.TCKPS = 0b11; // use 1:256 prescaler
    CLEARBIT(IFS0bits.T2IF); // clear timer2 interrupt flag
    CLEARBIT(IEC0bits.T2IE);  // disable timer2 interrupt enable control bit
    PR2 = 50; // set period to 1ms
    
    IPC1bits.T2IP = 0x01; // set Timer 2 IPL to 1
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
    IEC0bits.T2IE = 1;// Enable Timer2 interrupt
}

void setExternalInterrupt(){
    CLEARBIT(IEC1bits.INT1IE); 
    CLEARBIT(IPC5bits.INT1IP);
    IEC1bits.INT1IE = 0; // Enable INT1 interrupt
    IPC5bits.INT1IP = 1; // Set INT1 interrupt priority
}

void setTimer3() {
    //initialize Timer 3
    CLEARBIT(T3CONbits.TON);
    CLEARBIT(T3CONbits.TCS);
    CLEARBIT(T3CONbits.TGATE);
    TMR3 = 0x00; // reset Timer 3 counter
    T3CONbits.TCKPS = 0b00; // use 1:1 prescaler
    CLEARBIT(IFS0bits.T3IF); // clear timer3 interrupt flag
    CLEARBIT(IEC0bits.T3IE);  // disable timer3 interrupt enable control bit
    PR3 = 1280000; // set period to 10e-4 ms
    
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
}

void lcdPrintTime() {
    // Print the formatted time
    lcd_locate(0,0);
    lcd_printf("%02u:%02u.%03u\r", seconds_counter/60, seconds_counter%60, ms_counter%1000);
//    clearLCD();
}

void lcdReset() {
//    lcd_clear();
    TMR1 = 0x00;
    TMR2 = 0x00;
    seconds_counter = 0;
    ms_counter = 0;
    lcd_locate(0,0);
    lcd_printf("%02u:%02u.%03u\r", seconds_counter/60, seconds_counter%60, ms_counter%1000);
}

void lcdPrintCounter(uint16_t diff) {
    lcd_locate(0,1);
    lcd_printf("Cycle: %u\r", diff);
    lcd_locate(0,2);
    lcd_printf("Time: %.4f ms\r", (float)diff/10000);
}

int main(){
    // initialize Timer 1
    setTimer1();
    // initialize Timer 2
    setTimer2();
    // initialize Timer 3
    setTimer3();
    // initialize external interrupt
    setExternalInterrupt();
    
    // start timer
    T1CONbits.TON = 1;// Start Timer 1
    T2CONbits.TON = 1; //start timer 2
    T3CONbits.TON = 1; //start timer 3
    
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
    
    led_initialize();
    LED4_TRIS = 0; // set as output
    LED1_TRIS = 0; // set as output
    LED2_TRIS = 0; // set as output
    
    // init joystick
    bot_initialize();
    uint8_t bnt1_state_current = 0x0;
    uint8_t bnt1_state_previous = 0x0;
    uint8_t bnt1_state_count = 0;
    
    uint16_t pre_TMR3 = 0x0;
    uint16_t current_TMR3 = 0x0;
    
    while(1) {
        current_TMR3 = TMR3;
        
        // for Q1 LED
        LED4_PORT ^= 1;
        
        // for Q4
        if(++iteration==25000){
            lcdPrintTime(); //Q4
            lcdPrintCounter(current_TMR3 - pre_TMR3); //Q6
            iteration = 0;
        }
        bnt1_state_previous = bnt1_state_current;
        bnt1_state_current  = BTN1_PRESSED();
        
        //debouncing
        if(bnt1_state_current != bnt1_state_previous){
            if (bnt1_state_count > 8){
                if (bnt1_state_current == 1){
                    IEC1bits.INT1IE = 1;
                }
            }
            bnt1_state_count = 0;
        }else{
            bnt1_state_count += 1;
        }
        
           
        //LED4_PORT ^= 0;
        //__delay_ms(100);
        pre_TMR3 = current_TMR3;
    }
    return 0;
}


