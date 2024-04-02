//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#define BUFFER_CAPACITY 256
#include <stdio.h>
#include <libpic30.h>

#include <p33Fxxxx.h>
#include "types.h"
#include "uart.h"
#include "crc16.h"
#include "lab3.h"
#include "lcd_1.h"
#include "timer.h"

// Primary (XT, HS, EC) Oscillator without PLL
_FOSCSEL(FNOSC_PRIPLL);
// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystanl
_FOSC(OSCIOFNC_ON & POSCMD_XT);
// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);
// Disable Code Protection
_FGS(GCP_OFF);

volatile uint8_t interrupt_flag;
char message_buffer[BUFFER_CAPACITY]; 
uint16_t crc;

void __attribute__((__interrupt__)) _T2Interrupt(void) {
    interrupt_flag = 1;    
    TMR2 = 0x00;
    IFS0bits.T2IF = 0;
}

void flush() {
    uint8_t temp = 0;
    start_timer2();
    interrupt_flag = 0;
    while(interrupt_flag==0){
        while(uart2_recv(&temp) != 0 && interrupt_flag==0){}
    }
    stop_timer2();
}



void reset_message_buffer()
{
    int i;
    for(i=0; i<BUFFER_CAPACITY; i++){
        message_buffer[i] = '\0';
    }
}


void print_updated_lcd(uint8_t fail_count, uint16_t crc) {
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("fail count: %d", fail_count);
    lcd_locate(0, 1);
    lcd_printf("CRC value: %x\r", crc);
    lcd_locate(0, 2);
    lcd_printf("message: %s", message_buffer);
}

void print_empty_lcd(uint8_t fail_count) {
    lcd_clear();
    lcd_locate(0, 0);
    lcd_printf("fail count: %d", fail_count);
    lcd_locate(0, 1);
    lcd_printf("CRC value:\r");
    lcd_locate(0, 2);
    lcd_printf("message:\r");
}


int8_t receive_message(){
   // uint8_t msg_length = 0;  // reset msg_length
    uint16_t crc_recv = 0;
    uint8_t c;
    uint8_t index;
        
    // busy-waiting for the starting byte
    if(uart2_recv_start_byte() == -1) {
        return -1;
    }
    
    // if detect incoming byte in register, start timer
    interrupt_flag = 0;
    start_timer2();
    
    

    // store 2-byte CRC & 1-byte message length
    volatile uint8_t crc_high=0;
    while(uart2_recv(&crc_high) != 0 && interrupt_flag == 0){}
    
    volatile uint8_t crc_low=0;
    while(uart2_recv(&crc_low) != 0 && interrupt_flag == 0){}
    
    uint8_t msg_length=0;
    while(uart2_recv(&msg_length)!=0 && interrupt_flag == 0){}
    
    crc_recv = (crc_high<<8)|(crc_low);

    // store message in buffer
    index = 0;
    crc = 0;             
    while(index<msg_length && interrupt_flag == 0){
        while(uart2_recv(&c) != 0 && interrupt_flag == 0){}
        message_buffer[index] = c;
        crc = crc_update(crc, c);
        index++;
    }
    stop_timer2();
    
    // check crc equal to crc_recv
    if(crc == crc_recv && interrupt_flag == 0) {
        return 0;
    }
    return -1;
}


int main(void)
{	
    // init UART
    uint16_t baud = 9600;
    uart2_init(baud);
    
    set_timer2(); // 1s interrupt

    // init LCD
    lcd_initialize();
    lcd_clear();
    print_empty_lcd(0);
    
    // init variable
    uint8_t fail_count = 0;  // count failure times


    
    while(1) {
        // check if ACK or not
        if(receive_message() == 0){
            
            print_updated_lcd(fail_count, crc);       
            fail_count = 0;
            uart2_send_8(MSG_ACK);
        } 
        else {
//            uart2_flush();
            flush();
            fail_count++;
            print_updated_lcd(fail_count, crc);
            uart2_send_8(MSG_NACK);
        }
        reset_message_buffer();
        
    }
     
}	