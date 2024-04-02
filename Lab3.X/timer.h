/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 timer functions header       */
/*                                                  */
/****************************************************/

#include <p33Fxxxx.h>
#include "types.h"

//period in ms
void set_timer1(uint16_t period);

void set_timer2();
void start_timer2();
void stop_timer2();
