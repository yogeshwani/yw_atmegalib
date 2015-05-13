
#ifndef _ADC0804_PIN_CONFIG_H_
#define _ADC0804_PIN_CONFIG_H_

/*Comment this out to display the value on the LCD */

//#define DISPLAY_VALUE_ON_LED  

/* Currently the configuration is as follows for connecting the control lines of the ADC
 * in the future would be good to have an automatic way on assigning pins and ports and
 * generating error warning if it is used twice
 */
 /* CS connected on pin to be set as output */
 #define CS PB0
 /* RD connected on pin to be set as output*/
 #define RD PB1
 /* WR connected on pin to be set as output*/
 #define WR PB2
 /* INTR connected on pin to be set as input*/
 #define INTR PB3
 
 /* Port form where the converted 8-bit value is to be taken from the ADC to be set as input */
 #define ADC_VALUE_READ_PORT		PINC
 #define ADC_VALUE_READ_PORT_DDR	DDRC
 /*Port on which the data is to be sent out to be displayed on LED only  
   (only for debugging purposes) to be set as output 
    if displaying on the LEDs the port has to changed accordingly 
	Displaying on the LCD would be taken care by the LCd module
	*/
 #ifdef DISPLAY_VALUE_ON_LED 	
     #define ADC0804_VALUE_DISPLAY_PORT			PORTA
     #define ADC0804_VALUE_DISPLAY_PORT_DDR		DDRA
#endif

/* macros used to identify the port names which are used for either input or output */
 #define ADC_CNTRL_LINES_PORT_OUTPUT	 PORTB
 #define ADC_CNTRL_LINES_PORT_INPUT		 PINB 
 #define ADC_CNTRL_LINES_PORT_DDR		 DDRB
#endif
