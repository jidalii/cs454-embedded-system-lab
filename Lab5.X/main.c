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




int main(){
    
    //led_initialize();

	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
	lcd_clear();
	lcd_locate(0,0);
    lcd_printf("Xiaoyu Yang");	
    lcd_locate(0,1);
    lcd_printf("Jida Li");	
    lcd_locate(0,2);
    lcd_printf("Wei-Tse Kao");	
    
    led_initialize();
    LED4_TRIS = 0; // set as output
    
    int i = 0;
    while(i<3){
        LED4_PORT = 1;
        __delay_ms(1000);
        LED4_PORT = 0;
        __delay_ms(1000);
        i++;
    }

    bot_initialize();
    
    uint8_t LED1_count = 0;
    uint8_t LED1_PORT_CURRENT = 0x0;
    uint8_t LED1_CHECK = 0x0;
    clock_t old = clock();
    clock_t new;

    double DEBOUNCE_TIME = 200000; //0.2s
    double time_escaped = 0;


    lcd_locate(0,3);
    lcd_printf("Count:\r");
    
    lcd_locate(6, 4);
    lcd_printf("%u\r", LED1_count);
    
    lcd_locate(6, 5);
    lcd_printf("%x\r", LED1_count);

    
    while(1){
        LED1_CHECK = BTN1_PRESSED();
        LED1_PORT = LED1_CHECK;
        Nop();
        LED2_PORT = BTN2_PRESSED();
        Nop();
        LED3_PORT = (BTN1_PRESSED()) ^ (BTN2_PRESSED());
        Nop();
        if(LED1_CHECK != LED1_PORT_CURRENT){
            new = clock();
            if(LED1_CHECK == 1){
                time_escaped = (double)(new-old);
                if (time_escaped > DEBOUNCE_TIME){
                    LED1_count ++;
                    
                    if(LED1_count==0){
                        lcd_clear_row(4);
                        lcd_clear_row(5);
                    }
                    
                    lcd_locate(6,4);
                    lcd_printf("%u\r", LED1_count);
                    lcd_locate(6,5);
                    lcd_printf("%x\r", LED1_count);
                    
                }
            }
            old = new;
            LED1_PORT_CURRENT = LED1_PORT;
        }
	}
    return 0;
}


