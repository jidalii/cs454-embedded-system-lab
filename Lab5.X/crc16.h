/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: crc16 calculation - header file   */
/*                                                  */
/****************************************************/

#ifndef _CRC_16_H
#define _CRC_16_H
#include "types.h"

uint16_t crc_update(uint16_t crc, uint8_t data);

#endif 
