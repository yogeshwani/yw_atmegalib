/*
 * File : pwm.c
 *
 * Description:
 * File contains the necessary functions for using the internal Timers of the AVR micro-controller in the Pulse
 * Width Modulation (PWM) mode  
 *
 * Note: 
 * For detail documentation about the different functions and the use of USART refer the header file timer.h 
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */

#include <inttypes.h>
#include "timer.h"
#include "pwm.h"

/*
 The following function will calculate the values for either OCR1 or icr based on teh frequency passed in
 and also give the best possible prescale to be used. The F_CPU will be determined form the symbol being 
 defined so make sure the correct value is setup otherwise the calculation will go wrong 
 the frequency value should be passed in hz  
 This function is only valid for timer 1 or any other such timer which has a capapbility of having 
 a programmable TOP value i.e by regs OCR or ICR in their respectigve modes
 */ 
void calculate_perscale_for_frequency()
{
	
}