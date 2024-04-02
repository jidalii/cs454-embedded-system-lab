/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: abstract common definitions       */
/*                between server, client and troll  */
/*                                                  */
/****************************************************/


#ifndef _LAB_03_H
#define _LAB_03_H

#define MSG_START		0x0		// Start byte value
#define MSG_BYTES_START		1		// Start byte
#define MSG_BYTES_CRC		2		// CRC length
#define MSG_BYTES_MSG_LEN	1		// Message len (N)
#define MSG_BYTES_HEADER	MSG_BYTES_START+MSG_BYTES_CRC+MSG_BYTES_MSG_LEN
#define MSG_BYTES_MSG		255		// Message
#define MSG_MAX_LEN		MSG_BYTES_HEADER+MSG_BYTES_MSG

#define MSG_ACK			1
#define MSG_NACK		0


#endif



