/************************************************************************
 * Name : lcdpinconfig.h 
 * 
 * Header file for the lcd16x2.c file
 *  
 * Contains the pin configurations for Lcd module..
 * change this file when needed to suit the Avr controller   
 * the Lcd library function
 * TODO* write proper description   
 ************************************************************************/
#ifndef _LCD_PIN_CONFIG_H_
#define _LCD_PIN_CONFIG_H_

#include <avr/io.h>
/* Pin configuration for various LCD pins*/
/* MAcros explicitly used to set the Data Direction Registers (macros in portconfig.h)*/
#define LCD_DDR_CONTROLPORT DDRB
#define LCD_DDR_DATAPORT DDRA

/*these macros give the individual pin numbers and port numbers*/
#define RS PB4
#define RW PB5
#define En PB6
#define control_port PORTB
#define lcd_data PORTA



#endif
