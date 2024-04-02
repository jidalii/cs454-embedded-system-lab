/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: crc16 calculation - implement.    */
/*                                                  */
/****************************************************/


#include "crc16.h"


//Polynomial: x^16 + x^15 + x^2 + 1 (0xa001)
//Initial value: 0xffff
uint16_t crc_update(uint16_t crc, uint8_t data)     {
	int16_t i;
	
	crc ^= data;
	for (i = 0; i < 8; ++i)
	{
	    if (crc & 1)
	        crc = (crc >> 1) ^ 0xA001;
	    else
	        crc = (crc >> 1);
	}
	return crc;
}
