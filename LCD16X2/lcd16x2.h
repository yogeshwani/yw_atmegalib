/************************************************************************
 * Name : lcd16x2.h 
 * 
 * Header file for the lcd16x2.c file
 *  
 * Contains the pin configurations, enums macros and function definitions 
 * the Lcd library function  
 ************************************************************************/
#ifndef _LCD_16X2_H_
#define _LCD_16X2_H_

/* USAGE NOTE FOR HEADER FILES IN THE PROJECTS :
   to use the extra .h files :
   open Project->Lcd_TEST properties page and in the Directories option for compiler 
   add the path to the directory where the respective files are stored
   then it would build successfully 
   Also, importantly link the files in the current project by right clicking on the solution explorers project name
   and then add->Existing item..locate the file and click the arrow next to ehe add button and say add link 
   this step is important for successful compilation 
  */ 

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>

/*add the pin configuration file*/
#include "lcdpinconfig.h"

/* Enums of the commands for the LCd module 
   List is limited for the common usage and 
   needs to be expanded as and when  
  */
typedef enum LcdCommands
{
 ClearDisplay       = 0x01,
 ReturnHome         = 0x02, 
 DecCursor          = 0x04, /* shift cursor to left */
 IncCursor          = 0x06, /* shift cursor right*/
 ShiftDispRight     = 0x05,
 ShiftDispLeft      = 0x07,
 DispOffCurrOff     = 0x08,
 DispoffCurrOn      = 0x0A,
 DispOnCurrOff      = 0x0C,
 DispOnCurrBlink    = 0x0E,
 Disp__CurrBlink    = 0x0F,  
 CurrShiftLeft      = 0x10,
 CurrShiftRight     = 0x14,
 DispFullShiftLeft  = 0x18,
 DispFullShiftRight = 0x1C,
 ForceCurrFirstLine = 0x80,
 ForceCurrSecndLine = 0xC0,
 Lines2Bit4_5x7      = 0x28,
 Lines2Bit8_5x7      = 0x38  
}LCDCOMMANDS;

/* Enumeration for initializing the display to various modes */
typedef enum LcdMode
{
 Lines2_5X7_8Bit_mode = 1,
 Lines2_5X7_4Bit_mode = 2,
}LCDMODE;

/* Macros for clearing and setting the RS( Register Select ) pin of the LCD 
 */
#define ClearRS() control_port &= ~(1<<RS)
#define SetRS()   control_port |= (1<<RS)
#define ClearRW() control_port &= ~(1<<RW)
#define SetRW()   control_port |= (1<<RS)

/*high on pin E 
  low on pin E which make s a pulse on pin E */
#define SendPulseOnpinE() do { \
                                 control_port |= (1<<En);\
                                 control_port &= ~(1<<En);\
                             }while(0)
						   						   
/************************************************************************
 * Function to initialize the LCD
 * Steps to initialize the LCD to 5 x 7 matrix and 8 bit are as follows:    
 *		1. Send command 0x38 to initialize the display to 5x7 matrix 2 lines and 4 bit mode 
 *		2. Send command 0x0e for Display on cursor blink 
 *		3. Send command 0x01 to Clear display 
 *
 * Note: After powering up the Lcd wait for around 15ms. If Lcdinit() is 
 *       not the first function to be called then it is fine. 
 ************************************************************************/ 
void LcdInit(LCDMODE);

/***********************************************************************
 * Function to send configuration commands to the Lcd  
 * Steps for sending a command to the Lcd are as follows : 
 *		1. Set pin RS ( Register Select ) of the Lcd  to 0 to select the command register 
 *		2. Set pin R/W ( read/write ) of the Lcd to 0 ( we are writing hence, '0')
 *		3. Put the command on the data lines (make the respective ports direction as output )
 *		4. Send the low to high pulse on pin E of the Lcd 
 * 
 * Note : After each command wait for 100us to let the Lcd module finish running 
 *        the commands.
 *        Clear and return Home are exception to this command
 *        After the 001 and 0x02 should wait for 2 ms                                                       
 ************************************************************************/
#define LcdSendCommand(cmd) LcdSendByte(cmd, false);

/************************************************************************
* Function to send the data to the Lcd
* Steps for sending the data to the Lcd are as follows: 
*		1. Set pin RS ( Register Select ) of the Lcd to 1 to select the data register 
*		2. Set pin R/W (read/write ) of the Lcd to 0
*		3. Put the data on the data lines(make respective ports direction as output)
*		4. Send the low to high pulse on the E pin of the Lcd 
*  
* Note: After sending the data wait for around 100us to let the Lcd module write data 
*        to the Lcd 
*     
************************************************************************/
#define LcdSendData(data) LcdSendByte(data, true);

/************************************************************************
 * Function encapsulates the functionality of sending data as well as commands to
 * the Lcd module.
 *  
 * From the description of the LcdSendCommand and LcdSendData Function we can see that apart form step 1
 * which is for the register selection remaining all steps are identical and hence the following function 
 * can be used to send data/command to the Lcd module by passing in a boolean parameter   
 ************************************************************************/
void LcdSendByte( uint8_t, _Bool);

/************************************************************************
 * Function positions the cursor at he said x,y coordinates 
 * of the Lcd module.
 *  
 * TODO: write the description here  
 ************************************************************************/
void LcdGoToXY(uint8_t, uint8_t);

/************************************************************************
 * Function sends a string of max length 16 characters to be displayed on the LCD 
 *  
 * TODO: write the description here  
 ************************************************************************/
void LcdSendString(char *);

/************************************************************************
 * Function sends a integer of type uint16 (max value 65535) to be displayed on the LCD 
 *  
 * TODO: write the description here  
 ************************************************************************/
void LcdSendInteger(uint16_t);

#endif /*end of #ifndef _LCD_16X2_H_*/  


