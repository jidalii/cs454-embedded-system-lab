#define FCY 12800000UL 
#include <stdio.h>
#include <libpic30.h>

#include <p33Fxxxx.h>
#include "types.h"
#include "lcd.h"
#include "timer.h"
#include "math.h"
#include "motor.h"
#include "sorting.h"
#include "touch.h"

#define X_CH 1
#define Y_CH 0

#define Kp_X 0.026
#define Kd_X 0.12 // 0.5
#define Ki_X 0.0001

#define Kp_Y 0.026
#define Kd_Y 0.12 //0.13
#define Ki_Y 0.0001

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

int16_t fx_max = 210; //300
int16_t fx_min = -200;

int16_t fy_max = 230;
int16_t fy_min = -200;

// previous x and y position
int16_t error_x_prev = 0;
int16_t error_y_prev = 0;


// total error of x position and y position
int16_t total_error_x = 0;
int16_t total_error_y = 0;

// middle position of x-axis and y-axis
int16_t x_middle = 0;
int16_t y_middle = 0;

int16_t x_max = 2000;
int16_t x_min = 2000;
int16_t y_max = 2000;
int16_t y_min = 2000;

int16_t y_pos = 0;
int16_t x_pos = 0;

// circular buffer of x and y for filtering
int16_t buffer_x[4];
int16_t buffer_y[4];
int16_t filtered_buffer_x[4];
int16_t filtered_buffer_y[4];



uint16_t duty_x = 0;
uint16_t duty_y = 0;

double a[4] = {1, 0, 0.3333, 0};
double b[4] = {0.16667, 0.5, 0.5, 0.16667};

static uint8_t cnt = 0;
volatile uint8_t sample_x_ready = 0;


double pid_control(uint8_t channel){
    
    double force;
    
    if(channel == X_CH) {
        int16_t error_cur = x_pos - x_middle;
        int16_t derror = (error_cur - error_x_prev);
        error_x_prev = error_cur;
        total_error_x += error_cur;
        force = (-Kp_X * error_cur) - (Kd_X * derror) - (Ki_X * total_error_x);
    } else {
        int16_t error_cur = y_pos - y_middle;
        int16_t derror = (error_cur - error_y_prev);
        error_y_prev = error_cur;
        total_error_y += error_cur;
        force = (-Kp_Y * error_cur) - (Kd_Y * derror) - (Ki_Y * total_error_y);
    }
  
    
    return force;
}

int16_t force_to_duty(double force, uint8_t chan) {
    if(chan == X_CH) {
        return (int16_t)(1200 * (force - fx_min) / (double)(fx_max-fx_min) + 900);
    } else {
        return (int16_t)(1200 * (force - fy_min) / (double)(fy_max-fy_min) + 900);
    }
    
 
    
}

int16_t filter_val(int16_t val, uint8_t chan) {
    double res = 0;
    
    if(chan==X_CH){
        buffer_x[3] = buffer_x[2];
        buffer_x[2] = buffer_x[1];
        buffer_x[1] = buffer_x[0];
        buffer_x[0] = val;

        uint8_t i;
        for(i=0; i<=3; i++){
            res += b[i] * buffer_x[i];
            if(i>0){
                res -= a[i] * filtered_buffer_x[i-1];       
            }
        }
        filtered_buffer_x[3] = filtered_buffer_x[2];
        filtered_buffer_x[2] = filtered_buffer_x[1];
        filtered_buffer_x[1] = filtered_buffer_x[0];
        filtered_buffer_x[0] = (int16_t) res;
  
    }else{
        buffer_y[3] = buffer_y[2];
        buffer_y[2] = buffer_y[1];
        buffer_y[1] = buffer_y[0];
        buffer_y[0] = val;
        
        uint8_t i;
        for(i=0; i<=3; i++){
            res += b[i] * buffer_y[i];
            if(i>0){
                res -= a[i] * filtered_buffer_y[i-1];  
            }
        }
        filtered_buffer_y[3] = filtered_buffer_y[2];
        filtered_buffer_y[2] = filtered_buffer_y[1];
        filtered_buffer_y[1] = filtered_buffer_y[0];
        filtered_buffer_y[0] = (int16_t) res;
    }
    
    return (int16_t) res;
}

int16_t bound_duty_us(int16_t duty_us) {
    if(duty_us > 2100) {
        duty_us = 2100;
    }
    else if(duty_us < 900) {
        duty_us = 900;
    }
    return duty_us;
}

void __attribute__((__interrupt__)) _T3Interrupt(void) { 
    if(cnt%2==0){
        sample_x_ready = 1;
    } else{
        sample_x_ready = 2;
    }
    cnt++;
    IFS0bits.T3IF = 0;
}

int16_t sample_values(int16_t duty_us, uint8_t channel) {
    int16_t pos[9];
    
    touch_select_dim(channel);
    __delay_ms(10);
    motor_set_duty(duty_us, channel);
    __delay_ms(2000);
    
    uint8_t count = 0;
    while(count<9){
        pos[count] = touch_read();
        count++;
        __delay_ms(100);
    }
    return quickselect(pos, 0, 8, 5);
}

void find_middle(uint8_t chan) {
    if(chan == X_CH) {
        x_max = sample_values(2100, chan);
        x_min = sample_values(900, chan);
        x_middle = (x_max + x_min)/2;
    } else {
        y_max = sample_values(2100, chan);
        y_min = sample_values(900, chan);
        y_middle = (y_max + y_min)/2;
    } 
}

int main(){
    CLEARBIT(T3CONbits.TON);
	/* LCD Initialization Sequence */
	lcd_initialize();
	lcd_clear();
    
    lcd_locate(0, 0);
    lcd_printf("     -- Lab 8 --");
    lcd_locate(0, 1);
    lcd_printf("%.3f %.2f %.4f", Kp_X, Kd_X, Ki_X);
    lcd_locate(0, 2);
    lcd_printf("%.3f %.2f %.4f", Kp_Y, Kd_Y, Ki_Y);
    // init motor
    motor_init(X_CH);
    motor_init(Y_CH);
    
    // init touch screen
    touch_init();
    
    __delay_ms(1000);
    find_middle(X_CH);
    __delay_ms(1000);
    find_middle(Y_CH);
//    x_middle = 1600; y_middle = 1450;
    
    
    lcd_locate(0, 3);
    lcd_printf("setpoint: %d, %d", x_middle, y_middle);
    
    touch_select_dim(X_CH);
    __delay_ms(10);
    
    set_timer3();
    start_timer3();
    
    uint32_t print_cnt = 0;
    
    
    while(1) {
        if(sample_x_ready == 1) {
            x_pos = touch_read();
            touch_select_dim(Y_CH);
            //filter the ball position using a 3-order Butterworth filter 
            x_pos = filter_val(x_pos, X_CH);
            // perform PID computation 
            double force = pid_control(X_CH);
            duty_x = force_to_duty(force, X_CH);
            duty_x = bound_duty_us(duty_x);
            motor_set_duty(duty_x, X_CH);
            sample_x_ready = 0;
            
        }else if(sample_x_ready == 2) {
            y_pos = touch_read();
            touch_select_dim(X_CH);
            //filter the ball position using a 3-order Butterworth filter 
            y_pos = filter_val(y_pos, Y_CH);
            // perform PID computation 
            double force = pid_control(Y_CH);
            duty_y = force_to_duty(force, Y_CH);
            duty_y = bound_duty_us(duty_y);
            motor_set_duty(duty_y, Y_CH);
            sample_x_ready = 0;
            
        } else if (sample_x_ready == 0) {
            if (++print_cnt % 10000 == 0) {
                lcd_locate(0, 4);
                lcd_printf("cur pos: %d, %d  ", x_pos, y_pos);
                sample_x_ready = 3;
            }
            
        }
        
    }
    
    return 0;
}