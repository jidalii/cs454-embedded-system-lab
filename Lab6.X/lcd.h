/*
 * lcd.h
 *
 * Adapted for CS Embedded System Lab - FLEX UI board from:
 * http://sourceforge.net/projects/serialglcdlib/
 *
 * Adapted by: N. Edwards - 3 Aug, 2011 - U of Illinois
 * Modified by: Renato Mancuso (BU) - Feb 2019
 *

SUMMARY:
		A library for use with Sparkfuns Serial Graphic LCD 128x64 
		(http://www.sparkfun.com/products/9351). Flashed with this 
		firmware: (https://sourceforge.net/projects/serialglcd/). 
		By a beginner, for beginners.
 */

#ifndef __LCD_H
#define __LCD_H

#include <p33Fxxxx.h>
#include "types.h"
#include <stdio.h>

#define MAX_ROW 8
#define MAX_COLUMN 25
#define CHAR_HEIGHT 8
#define CHAR_LENGTH 6
#define ROW_LENGTH 160

void lcd_initialize(void);

#define lcd_locate(X,Y)\
	lcd_send_cmd(LCD_CMD_SETCOORD_X);\
	lcd_send((X)*CHAR_LENGTH);\
	lcd_send_cmd(LCD_CMD_SETCOORD_Y);\
	lcd_send((Y)*CHAR_HEIGHT);

#define lcd_printf_d(...)             \
    do {                              \
        uint16_t len;                 \
    	len = printf(__VA_ARGS__);    \
        fflush(stdout);               \
        __delay_ms(len * 15);         \
    } while (0)

#define lcd_printf(...)         \
    	printf(__VA_ARGS__);    \
        fflush(stdout);         

#define lcd_clear() \
	lcd_send_cmd(LCD_CMD_CLRSCR);

#define lcd_clear_block(X1, Y1, X2, Y2) \
	lcd_send_cmd(LCD_CMD_ERASEBLOCK); \
	lcd_send(X1); \
	lcd_send(Y1); \
	lcd_send(X2); \
	lcd_send(Y2); 			

#define lcd_clear_row(Y) \
	lcd_clear_block(0, Y*CHAR_HEIGHT, (ROW_LENGTH - 1), (Y+1)*CHAR_HEIGHT-1); 


void lcd_send(uint8_t const data);
void lcd_send_str(uint8_t const data[], uint8_t chars);
void lcd_send_cmd(uint8_t const data);

/* Clears the screen */
void clearLCD(void);

/* Set debug level, 0=off,1=binary,2=text */
void setDebug(int l);	

/* Set backlight duty cycle integer from 0 to 100 where 0 is off */
void backLight(int x);	

/* Draws a circle with center x,y, radius r and z=1 for write, z=0 for erase */	
void drawCircle(int x, int y, int r, int z);		

/* toggles CR/LF. each time this command is sent, wheather or not a CR automatically
 * executes a LF is toggled. this is saved to EEPROM and is persistant over power 
 * cycles 
 */
void toggleCRLF(void);

/* Erase a block on the screen with top left corner x1,y1, bottom right corner x2,y2 */
void eraseBlock(int x1, int y1, int x2, int y2);	

// Resets the LCD, clears the screen and sets x and y offset to 0,0
void resetLCD(void);

// Changes the baudrate. 1=4800,2=9600,3=19200,4=38400,5=57600,6=115200. (Persistant)
void changeBaud(int x);		

// Toggles between the default and aux font. does not affect previosly written text.
void toggleFont();	

// Sets the mode for drawing text, default is 7=Copy, and is the only mode that respects reverse color.
void setFontMode(int x);	

// Draws a sprite stored in the backpack. Uses same modes as when drawing text
void drawSprite(int x, int y, int n, int mode);	

// Draws a line from x1,y1 to x2,y2. z=1 to write, z=0 to erase.	
void drawLine(int x1, int y1, int x2, int y2, int z);		

/* Uploads sprites for use with the drawSprite function (not persistant), MUST be 
 * 32 bytes or the display will go out of sync. Consult with firmware README for 
 * more info. 
 */
void uploadSprite(int n, int w, int h, uint8_t *data);		

/* Draws a box with top left corner x1,y1, bottom right corner x2,y2. z=1 to 
 * write, z=0 to erase 
 */
void drawBox(int x1, int y1, int x2, int y2, int z);

// Toggles a pixel. x,y-coords and z=1 to write, z=0 to erase
void togglePixel(int x, int y, int z);	

/* Draws a filled box with top left corner x1,y1, bottom right corner x2,y2. 
 * the last byte is the fill byte 0x00 for empty 0xFF for filled, other 
 * values can be used.
 */
void drawFilledBox(int x1, int y1, int x2, int y2, uint8_t fill);

// Inverts the display, text/graphics written before this command will inverted aswell
void reverseColor(void);

// Toggles the sparkfun logo when powering up (persistent)
void toggleSplash(void);

/* Function to draw bitmaps from an array of hex values, requires the length
 * of the array. Consult with firmware README for more info
 */
void drawData(int x, int y, int mode, int w, int h, uint8_t *data);	

// Set the position before writing text
void gotoPosition(int x, int y);

/* Simpler way of selecting which line to write text on (1-8). Uses the 
 * gotoPosition function, simply splits the screen up in 8 lines
 */
void gotoLine(int line);	

/*
 * From README...
Jenn Holt's updated firmware for sparkfun Graphical LCD backpack

This is only for the 128x64 display!!!! I took out all the code for the larger display.
I only own the small one and this was all I could test. others are welcome to adapt the code if they want.

interface stuff:

default baud is still 115200, and will revert to this when a character is sent during the splash screen

** The display now implements XON/XOFF flow control. If your application supports this, you can re-compile the code with a smaller RX_BUFFER_SIZE
   value to make more room for sprites. The default RX_BUFFER_SIZE is the 256 max. If you are going to BitBlt large images I recommend implementing XON/XOFF or you may overrun the buffer. You could also put a delay in your host code if you overrun the buffer while BitBlt'ing a large image.
 
ASCII characters sent to the display will be printed left to right, top to bottom
commands are still prefaced by 0x7C
** there are basic debug messages that get sent when command mode is entered and exited, these are set with command 0x02 0x## with ##=(0,1,2)
	0= no messages, 1=binary format messages, 2=ascii format messages. the messages are sent out on the uart
** there is an LCD reset command that resets the LCD should things get screwy, command 0x06. (pulls /RESET low, then high, clears the screen and sets x_offset=0, y_offset=0)


Text stuff:

**display now responds to carrige returns and line feeds. by default a carrige return also executes a line feed. you can toggle this with command 0x04
**there is no more demo mode
**text rendering is much faster
**you can define your own fonts, however you must decide what fonts you want at compile time. All font data is in two files font.h and aux_font.h, replace them with your own fonts if you wish.
  There is a utility called bmp2header_font which will create a font.h file from a bitmap(one large image with characters sequential in the x direction), not you must manually rename text_array to aux_text array in the header if you want to use it for aux_font.h
**by default the code builds with the original Sparkfun font in default, and a double sized font in aux, you can switch back and forth with command 0x08
**the text is now rendered using the bitblt function, so you can use all the logical modes(0..7) for drawing text, set the mode with command 0x0A 0x##.


Graphical stuff:
**the coordinate system is now (0,0) in upper left and increasing right and down
**the line and circle algorithms have been replaced to use only integer math (Bresenham's line algorithm and midpoint circle algorithm)
**support for NUM_SPRITES sprites labled 0..(NUM_SPRITES-1). each sprite has SPRITE_SIZE bytes of data dedicated to it. 
	the default #defines in the code set these numbers to 8 sprites of 34 bytes each, you can change this if you want, but be careful not
	to use up all the SRAM, the ATmega168 only has 1K. 34 bytes/sprite is enough for a 16x16 block, although sprites don't have to be square.
	command 0c0B draws a sprite, the sparkfun logo is sprite 0, but this can be overwritten. Command 0x0D uploads a sprite.
**draw filled box command. this draws a box filled with a repeating byte(virtical stripe of data) ie 0x00 would clear every pixel, 0xFF would set them, 0xAA would draw stripes
** bitblt if you want to draw something larger than a sprite.
**raw read/write functions: write_command, write_data, write_block, read_data, read_byte, read_block
**reverse mode now inverts the graphics on the screen instead of clearing it.

Command list:
all commands must start with 0x7C, this tells the display that the following bytes are part of a command
*/

/* 0x00:	clear screen, no other bytes (ex 0x7c 0x00)
	clears/sets all pixels on the screen(depending on if reverse is set)
	sets x_offset and y_offset to 0 */
#define LCD_CMD_CLRSCR		0x00

/* 0x01:	set debug level, follow with byte for debug level(0,1,2)
	level 0 is no messages
	level 1 is binary messages, the display will echo a byte = command when the command is started and a byte = 0x00 when the command is finished
	level 2 is text messages, useful when using a terminal. will echo "Entered Command n" when command n is started
		and will echo "Exited Command n, RX_buffer=y" when the command is done, and will report how many bytes are in the recieve buffer */
#define LCD_CMD_SETDEBUG	0x01

/* 0x02:	set backling duty cycle. argument is duty cycle in percent ie (0..100) ex: 0x7C 0x02 0x32 will set the backlight to 50% */
#define LCD_CMD_BACKLIGHT	0x02

/* 0x03:	draw circle. arguments are x, y, radius, set/reset. so command (0x7C 0x03 0x20 0x20 0x0A 0x01) will draw a circle at (32,32) with a radius of 10 by setting the pixels */
#define LCD_CMD_DRAWCIR		0x03

/* 0x04:	toggles CR/LF. each time this command is sent, wheather or not a CR automatically executes a LF is toggled. this is saved to EEPROM and is persistant over power cycles */
#define LCD_CMD_TOGGLECRLF		0x04

/* 0x05:	erase block. draws a block on the screen with clear(reverse=0) or set(reverse=1) pixels
	arcuments are x1,y1,x2,y2 the coordinates of two opposite corners of the block. ex: (0x7C 0x05 0x00 0x00 0x10 0x10) clears from (0,0) to (16,16) */
#define LCD_CMD_ERASEBLOCK	0x05

/* 0x06:	LCD reset. resets the LCD, clears the screen, and sets x_offset and y_offset to 0 */
#define LCD_CMD_RESET		0x06

/* 0x07:	change baud rate. 1..6. ex: (0x7C 0x07 0x05) sets baud 57600. this setting is persistant over power cycles.
	1=4800
	2=9600
	3=19200
	4=38400
	5=57600
	6=115200 */
#define LCD_CMD_CHANGEBAUD	0x07

/* 0x08:	toggle aux font. This command toggles which font is used for rendering text(does not effect characters already displayed, just what comes after) */
#define LCD_CMD_TOGGLEFONT	0x08

/* 0x0A:	set font mode. This determines how the font is rendered to the screen. it uses the same modes as bitblt(since the characters are rendered using bitblt)
	mode 7(copy) is default and this mode respects reverse. ie it will clear pixels to draw characters if reverse=1. the other modes do not respect reverse
	you get whatever the logical operation gives. (characters in internal memory are stored set on clear)
	0= AND
	1= NAND
	2= OR
	3= NOR
	4= XOR
	5= NXOR
	6= don't uses this one, will result in corrupted characters
	7= COPY (this is the default, overwrites whatever is in the background, and is the only one that respects reverse) */
#define LCD_CMD_SETFONTMODE	0x0A

/* 0x0B:	draw sprite. draws a sprite to the screen. arguments are x, y, sprite#, mode. 
	ex: (0x7C 0x0B 0x20 0x20 0x00 0x04) will XOR sprite 0 with the background at location (32,32) */
#define LCD_CMD_DRAWSPRITE	0x0B

/* 0x0C:	line. draws a line onm the screen. arguments are x1,y1,x2,y2,S_R with S_R=0 clearing pixels, and S_R=1 setting them. (respects reverse, ie if reverse=1, S_R=1 clears pixels) */
#define LCD_CMD_DRAWLINE	0x0C

/* 0x0D:	upload sprite.(NUM_SPRITES=8, SPRITE_SIZE=34 by default, to change this requires re-compiling the code) takes 1 argument and SPRITE_SIZE data bytes.
	send the sprite number first (0..(NUM_SPRITES-1) ) if the value sent is out of range, it will be coereced to (NUM_SPRITES-1)
	the sprite data is: [width][height][(SPRITE_SIZE-2) bytes of image data]
	image data is stored left to right, top to bottom. each byte defines an 8 pixel vertical stripe. so an nx8 sprite is n bytes long. one byte for each column.
	if the image is taller than 8 pixels, then the next n bytes store the next row. height doesn't have to be a multiple of 8, but there must be enough rows so 
	that (Number of rows)*8 >= (height in pixels). For example a 10x10 sprite would require 10 bytes/row(for x dimension) and 2 rows(2*8=16>10) or 20 bytes total

	an example sprite is the sparkfun logo. to upload this would require :
	0x7C 0x0D 0x0N 0x0A 0x10 0x80 0xc0 0x40 0x0c 0x3e 0xfe 0xf2 0xe0 0xf0 0xe0 0xff 0x7f 0x3f 0x1f 0x1f 0x1f 0x1f 0x0f 0x07 0x03 ...  0x00
                  spr#  W    H  |<---------------------------------------------------------(SPRITE_SIZE-2) bytes of image data--------------->|

	notice that the logo itself is 10x16 and only requires 20 bytes to define, but you must send all (SPRITE_SIZE-2) image data bytes (data after those necessary to draw the sprite are ignored) or the display will get out of sync

	there is a program in the utilities folder to convert a 24bbp bitmap to a sprite. it outputs the exact sprite data in binary format to stdout. */
#define LCD_CMD_UPLOADSPRITE	0x0D

/* 0x0F:	draw box. This command draws an outline of a box with corners x1,y1,x2,y2 and S_R for line type (respects reverse)
	so to draw a box from (5,5) to (15,15) would be (0x7C 0x0F 0x05 0x05 0x0F 0x0F 0x01) */
#define LCD_CMD_DRAWBOX		0x0F

/* 0x10:	set pixel. sets/resets a pixel. arguments x,y,S_R */
#define LCD_CMD_SETCPIXEL	0x10

/* 0x12:	draw filled box. this draws a box much like the previous command, but the inside is filled with the fill byte. the fill byte describes 1 8-pixel high stripe
	that is repeated every x pixels and every 8 y pixels. the most useful are 0x00 to clear the box, and 0xFF to fill it.
	arguments x1,y1,x2,y2,fillbyte */
#define LCD_CMD_DRAWFILLEDBOX	0x12

/* 0x14:	reverse mode. toggles reverse(white on black) mode. the new reverse mode inverts the screen in place, it does not clear the screen or change the text drawing position */
#define LCD_CMD_REVMODE		0x14

/* 0x15:	toggle splash. toggles wheather or not splash screen is displayed on startup. persistant over power cycles. */
#define LCD_CMD_TOGGLESPLASH	0x15

/* 0x16:	bitblt. The bitblt command allows you to draw graphics to the screen like sprites, but the data doesn't have to be uploaded first, and there is no size
	restrictions.(other than the drawable area of the display) the arguments are x, y, mode, w, h, [data]
	the width and height are ordered so you can send sprite data directly to bitblt from a file with a terminal program. sent the x,y,mode bytes first, then
	send the file(ie made with the bmp2bin_sprite program in the utilities folder) */
#define LCD_CMD_BITBLT		0x16

/* 0x18:	set x_offset. sets the x coordinate where the upper left corner where the next character will be drawn. 1 argument. */
#define LCD_CMD_SETCOORD_X	0x18

/* 0x19:	set y_offset. sets the x coordinate where the upper left corner where the next character will be drawn. 1 argument. */
#define LCD_CMD_SETCOORD_Y	0x19

/*
The next commands are exposed intefaces to the internal LCD read/write functions. I would recommend not using them, but if there is something you absolutely can't
do with the standard graphical functions, this gives you direct access to the LCD

0x1A:	write command, writes a command to the LCD controller chips. this does not set the address registers, they must be set before hand.
	takes 2 arguments the byte to write, and 1, 2 or 3 to select which of the 2 KS0108 chips to send it to. (3=both)

0x1B:	write data, writes a data byte to the LCD controller chips. this does not set the address registers, they must be set before hand.
	takes 2 arguments the byte to write, and 1, 2 or 3 to select which of the 2 KS0108 chips to send it to. (3=both)

0x1C:	write block, slightly higher level function. writes a sequence of bytes to the display. automatically crosses chips, and sets proper registers for you.
	takes 3 arguments and data bytes. arguments are: x coordinate, page, and length of data (max 128)
	after the 3 argument bytes are sent send [length] data bytes.

0x1D:	read data, reads a data byte from one of the controller chips. this does not set the address registers, they must be set before hand.
	takes 1 argument {1 or 2} to select which KS0108 to read, returns data byte on uart (if debug_level is 0 or 1 it will be a binary value, otherwise it will be ASCII)

0x1E:	read byte, slightly higher level function. returns a byte at a specific x coordinate(page must be set seperately)
	takes 2 arguments, x-coordinate and side={1,2} to select the chip to read. 
	returns value on uart (if debug_level is 0 or 1 it will be a binary value, otherwise it will be ASCII)

0x1F:	read block, slightly higher level function. returns a sequence of data bytes from a specific page. automatically crosses chips, and sets address registers for you
	takes 3 arguments, x-coordinate, page, length. returns the data on the uart (if debug_level is 0 or 1 it will be a binary value, otherwise it will be ASCII)

0x20:	set y reg. For reasons unknown. the datasheet for the KS0108 uses y to lable the horizontal axis. this sets the y register on the LCD controller chips.
	y counts from 0 to 63 and starts at the left of the display. any value over 63 is handled by the second chip.
	takes 1 argument, y-value and sets it on both chips simultaneously.
	
0x21:	set page. the display is broken up into 8 pages which describe horizontal rows 8 pixels high. pages start at 0 at the top of the display and go to
	7 at the bottom
	takes 1 argument, page, and sets both chips simultaneously.
*/

#endif


