/*
 * File : adc0804.c
 *
 * Description:
 * File contains the necessary functions for using the ADC0804 with the Avr micro controller
 *
 * Note:
 * Detail documentation about the working and the use of ADc0804 refer the header file adc0804.h
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */

/* Including necessary header files */
#include <avr/io.h>
#include <util/delay.h>
/* Including header files */
#ifdef FOR_AVR_STUDIO_5 
#include "adc0804.h"
#include "adcpinconfig.h"
#include "portconfig.h"
#include "lcd16x2.h"
#endif

/* for use in AS 4*/
#include "../../Library/LCD16X2/lcd16x2.h"
#include "../../Library/PortConfig/portconfig.h"
#include "../../Library/ADC0804/adc0804.h"
#include "adcpinconfig.h"


void ADC0804Init()
{
    /*set respective ports data direction register */
    /* 1. set the direction of the data form controller to led as output
	 * Adc module will not care where the LCD is connected as the LCD module would be knowing that and 
	 * the LCd module would set the DDR of that port appropriately. 
	 * The macro would be called only when the display has to be on the LED port rather than the LCD 
	 */
#ifdef DISPLAY_VALUE_ON_LED 	
    setportdir(ADC0804_VALUE_DISPLAY_PORT_DDR,OUTPUT);
#endif    
	/* set the direction of the data register from the adc to controller as input*/
	setportdir(ADC_VALUE_READ_PORT_DDR,INPUT);
    /* cs = output/ WR = output/ rd = output /INTR =input*/
    setportpindiroutput(ADC_CNTRL_LINES_PORT_DDR,CS);
    setportpindiroutput(ADC_CNTRL_LINES_PORT_DDR,WR);
    setportpindiroutput(ADC_CNTRL_LINES_PORT_DDR,RD);
    setportpindirinput(ADC_CNTRL_LINES_PORT_DDR,INTR);
    CSHigh();
    RDHigh(); 
    WRHigh();
}

void StartAdc0804Conversion()
{
	/*1. set pins CS, WR , RD as output 
	  2. set INTR as input
	  3. set the adc port as input
	  */  
    CSLow();
    WRLow(); 
	WRHigh();
    CSHigh();
    /* polling until INTR is pulled low by the ADC
     * would be good to have an interrupt driven as well
     */
    while(ADC_CNTRL_LINES_PORT_INPUT & (1<<INTR));
}

void ReadAdc0804Value()
{
    CSLow();
    RDLow(); 
    g_adc_value =  ADC_VALUE_READ_PORT;
    CSHigh();
    RDHigh();
	
   /* macro definition in the header file
    * to display 8 bit value on the LEDs
    */
   #ifdef DISPLAY_VALUE_ON_LED
    DisplayAdcValueOnLED(ADC0804_VALUE_DISPLAY_PORT,g_adc_value);
   #else 	
    DisplayAdcValueOnLCD(); 
#endif
}
/* will display the value at the cursor location .. the calling
   function needs to take care to see where the cursor is and what
   message to be dispalyed before */
void DisplayAdcValueOnLCD()
{
    LcdSendCommand(ClearDisplay);
    LcdSendString("ADC Value :");
    LcdSendInteger((uint16_t)g_adc_value);
}	
 







