#ifndef _ADC0804_H_
#define _ADC0804_H_

/* Header file for pin definitions */
#include "adcpinconfig.h"

/* Necessary macros for the ADC0804
 * Chip Select(CS) = 0 / 1
 */
#define CSLow()  ADC_CNTRL_LINES_PORT_OUTPUT &= ~(1<<CS)
#define CSHigh()  ADC_CNTRL_LINES_PORT_OUTPUT |= (1<<CS)

/* Write(WR) = 0/1 
 */
#define WRLow()  ADC_CNTRL_LINES_PORT_OUTPUT &= ~(1<<WR) 
#define WRHigh()  ADC_CNTRL_LINES_PORT_OUTPUT |= (1<<WR)  

/* Read(RD) = 0/1 
 */
#define RDLow()  ADC_CNTRL_LINES_PORT_OUTPUT &= ~(1<<RD)
#define RDHigh() ADC_CNTRL_LINES_PORT_OUTPUT |= ((1<<RD)) 



/* Global Variable to store the ADC0804 reading obtained after conversion */
uint8_t g_adc_value;

/* Function initializes the proper ports in the respective directions for the ADc to work
*/ 
void ADC0804Init();
/* Function to enable the ADC0804 module and to start the conversion process
 * for ad0804 the conversion time is typically 100us and hence nay function using the ADC will have to wait for that time
 * currently the function uses polling to poll the INTR line and can be changed to be interrupt driven by connecting the
 * INTR to the hardware interrupt pin of the micro controller
 * the function would wait until the adc has finish conversation after which a call to Readadc0804value must be made
 */
void StartAdc0804Conversion();

/*function to read the converted digital value from the ADC0804 */
void ReadAdc0804Value();

/* function to display the value on the LCD*/
void DisplayAdcValueOnLCD();

/* Displays the 8-bit value on the port passed in this macro */
#define DisplayAdcValueOnLED(port,value)  (port) = (value)

#endif //end of #ifndef _ADC0804_H_
