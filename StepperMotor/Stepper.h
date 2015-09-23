/************************************************************************
 * Name : stepper.h 
 * 
 * Header file for the stepper.c file
 *  
 * Contains the pin configurations, enums macros and function definitions 
 * the stepper motor library (IC = PBL3717)  
 ************************************************************************/
/* One PBL3717 IC has 3 input pins to be connected to the microcontroller. It 
 * drives one coil of the two coils contained in a BIPOLAR stepper motor. The
 * 3 input pis are phase, Input0 and Input1. So, in order to drive an entire
 * motor we need 2 IC's and 6 pins of a microcontroller, 3 pins for driving 
 * each of the coils. The pins which drive the coils are shown in the figure 
 * below. These pins are connected to the microcontroller while the rest of the 
 * pins of the IC are connected to the power supply ground etc as shown in the 
 * data sheet. The phase pin the controls the direction of current in the coil.
 * from x -> y or vice-versa based on if the phase pin is logic 0 or 1. While 
 * the input pins controls the amount of current through the coil base on the 
 * following truth table. The pins phase, input0, input1 are called as A, I0A,
 * I1A for coil A and same for coils B as well.   
  
 *  Input0 (pin 9)  Input 1 (pin 7)  Current level 
 *     H               H             No Current
 *     L               H             Low Current
 *     H               L             Medium Current       
 *     L               L             High Current         
 *
*  -----                     x -------              -------                  -----
*  | p |---- PHASE (A)	              )            (           PHASE (B) ----| P | 
*  | B |		       coil A )            ( coil B                  | B |
*  | L |---- INPUT0 I0A               )            (           INPUT I0B ----| L |
*  |   |                              )            (                         |   |
*  | 1 |---- INPUT1 I1A	     y -------              -------    INPUT I1B ----| 2 |  
*  -----	                                                                 -----

* The way the pins should be connected to a microcontroller port is in the following
* way. PORTB is an example. The same scheme can be used to connect to another port in 
* order to drive one more stepper.   
* IMPORTANT : Make sure that which ever port is used for connecting the motors, the 
* following sequence of connecting is followed other wise things wont work as expected 
* PIN CONNECTION SCHEME:
* PHASE  (A)   	 PORTB.0
* INPUT0 (I0A) 	 PORTB.1
* INPUT1 (I1A) 	 PORTB.2 
* PHASE  (B)     PORTB.3
* INPUT0 (I0B) 	 PORTB.4
* INPUT1 (I1B) 	 PORTB.5
*
* So, on general the steps for a FULLSTEP sequence for a single stepper motor 
* would look something like

* PORTA=0x09;
* _delay_ms(TIME_BETWEEN_STEPS);
* PORTA=0x08;
* _delay_ms(TIME_BETWEEN_STEPS);
* PORTA=0x00;
* _delay_ms(TIME_BETWEEN_STEPS);
* PORTA=0x01;
* _delay_ms(TIME_BETWEEN_STEPS);
*	
			
* This is Quarterstep

	            PORTA=0x0D;
			    PORTB=0x0D;
			    _delay_us(speed);
			    PORTA=0x0F;
			    PORTB=0x0F;
		     	_delay_us(speed);
				PORTA=0x0C;
				PORTB=0x0C;
				_delay_us(speed);
				PORTA=0x0A;
				PORTB=0x0A;
				_delay_us(speed);
				PORTA=0x28;
				PORTB=0x28;
				_delay_us(speed);
				PORTA=0x38;
				PORTB=0x38;
				_delay_us(speed);
				PORTA=0x20;
				PORTB=0x20;
				_delay_us(speed);
				PORTA=0x10;
				PORTB=0x10;
				_delay_us(speed);
				PORTA=0x04;
				PORTB=0x04;
				_delay_us(speed);
				PORTA=0x06;
				PORTB=0x06;
				_delay_us(speed);
				PORTA=0x05;
				PORTB=0x05;
				_delay_us(speed);
				PORTA=0x03;
				PORTB=0x03;
				_delay_us(speed);
				PORTA=0x21;
				PORTB=0x21;
				_delay_us(speed);
				PORTA=0x31;
				PORTB=0x31;
				_delay_us(speed);
				PORTA=0x29;
				PORTB=0x29;
				_delay_us(speed);
				PORTA=0x19;
				PORTB=0x19;
				_delay_us(speed);
*******************************************************************************************
*/
#ifndef _STEPPER_H_
#define _STEPPER_H_

/*Include necessary files */
#include <avr/io.h>
#include <stdbool.h>
#include "portconfig.h"

#define STEPPER_1_DDR getDDR(A)
#define STEPPER_2_DDR getDDR(B)
#define STEPPER_1_PORT getPORT(A)
#define STEPPER_2_PORT getPORT(B)

typedef enum stepmode 
{
    FULLSTEP,
    HALFSTEP,
    QUARTERSTEP
} STEPMODE;


typedef enum step_direction 
{
    FORWARD,
    REVERSE,
    TURNRIGHT,
    TURNLEFT
} STEPDIRECTION;

void StepperInit();
void DriveStepper(STEPMODE stepmode, uint16_t stepcount, STEPDIRECTION direction, uint16_t speed);
void TurnStepper();

#endif
