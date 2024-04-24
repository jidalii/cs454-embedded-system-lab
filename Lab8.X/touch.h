#ifndef __TOUCH_H
#define __TOUCH_H

#include <p33Fxxxx.h>
#include "types.h"

void touch_init(void);

void touch_select_dim(uint8_t dimension);

uint16_t touch_read(void);


#endif