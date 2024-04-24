/*
 * File:   sorting.c
 * Author: team-3b
 *
 * Created on April 19, 2024, 9:59 AM
 */


#include <p33Fxxxx.h>
#include "types.h"

void swap(uint16_t* a, uint16_t* b){
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}

uint16_t quickselect(uint16_t* arr, int8_t l, int8_t r, int8_t k){
   
    int8_t i = l;
    int8_t j = l;
    uint16_t pivot = arr[r];
    
    while (i<r){
        if(arr[i]<=pivot){
            swap(&arr[i], &arr[j]);
            j++;
        }
        i++;
    }
    swap(&arr[j], &arr[r]);
    if(j==k-1){
        return arr[j];
    } else if(j>k-1){
        return quickselect(arr, l, j-1, k);
    } else{
        return quickselect(arr, j+1, r, k);
    }
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