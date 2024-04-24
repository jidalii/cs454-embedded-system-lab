
#ifndef __MOTOR_H
#define __MOTOR_H

#include <p33Fxxxx.h>
#include "types.h"

void motor_init(uint8_t chan);

void motor_set_duty(int16_t duty_us, uint8_t chan);

#endif