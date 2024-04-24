
#ifndef __SORTING_H
#define __SORTING_H

#include <p33Fxxxx.h>
#include "types.h"

void swap(int16_t* a, int16_t* b);

int16_t quickselect(int16_t* arr, int8_t l, int8_t r, int8_t k);

void quicksort(int16_t* arr, int8_t l, int8_t r);

#endif