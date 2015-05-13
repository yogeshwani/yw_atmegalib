/* Name: stepper.c
 * Date modified: 2 April 2011
 * Considerations for the Stepper motor sequence
   in Principle the stepper motor uses a 4 step sequence called as
   the FULL wave sequence. In the motor that is used in this project the Step
   angle is 1.8 deg. So the motor needs all the 4 wave sequence to be executed
   to make one step of 1.8deg.So, what we have is it would take 200(360 deg/1.8deg )
   of the sequences in order to be applied to make one revolution.
   Now, here is the catch. The sequence is a 4 "step" sequence, And after executing all the
   4 "steps" it takes one stepper motor step of 1.8 deg . So if we have a program which loops through
   these 4 step sequences once we would have achieved a step movement of 1.8 deg. Hence
   200 (sequences in order) / 4 = 50.
   hence a block of code as below which is looped through 50 times would give a full rotation of the wheel
   Inputs
             - No. of steps
			 - type of stepping (Full,Half,Quarter)
             - Speed for the motors
			 - direction
			 - turns
             - Port connected to
   Returns
    Function should return void? or errors regarding port availability
	,wrong stepping speed

	TODO
	- Consider having error warnings printed off during the compilation if possible
	  to avoid running into trouble in run time
	- look for micro stepping support for stepper.
	- consider using different ics in the future and addinf them as parameter to
	  the function calls(Assume correct connections in case of ics/ Pins can be fixed
	  keep Port as a parameter if possible)
*/
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>
#include "stepper.h"

uint8_t quarter_step[] = { 0x0D, 0x0F, 0x0C, 0x0A, 0x28, 0x38, 0x20, 
                           0x10, 0x04, 0x06, 0x05, 0x03, 0x21, 0x31,
						   0x29, 0x19 };
uint8_t full_step[] = {0x09, 0x08, 0x00, 0x01};
	
/* This function initializes the port where the stepper is connected to be
 * the output port
 */
void StepperInit()
{
	/* set the port as output port 
     * stepper.h already includes the necessary file(portconfig.h)
     */
    setportdir(STEPPER_1_DDR, OUTPUT);
    setportdir(STEPPER_2_DDR, OUTPUT);
}	

/* This function drives stepper motors for the amount of steps and in the direction 
 * passed as a parameter to this function. 
 */
void DriveStepper(STEPMODE stepmode, uint16_t stepcount, STEPDIRECTION direction, uint16_t speed)
{
    uint16_t stepcounter = 0;
	int8_t i = 0;
    int8_t j = 0;
	switch(stepmode)
    {
        /* observation : maximum delay between steps : 5 seconds !!
         *               minimum delay between steps : 950 us
         */
        case FULLSTEP:
	    {
			for (stepcounter = 0; stepcounter < stepcount; stepcounter++)
            {
                if(direction == FORWARD)  
                {
            		for(i = 0; i <= 3; i++)
            		{
                        STEPPER_1_PORT = full_step[i];
            			STEPPER_2_PORT = full_step[i];
            			_delay_us(speed);
            		}
            	}
            	else if(direction == REVERSE)
            	{
            		for(i = 3; i >= 0; i--)
            		{
            			STEPPER_1_PORT = full_step[i];
            			STEPPER_2_PORT = full_step[i];
            			_delay_us(speed);
            		}					
            	}
                else if(direction == TURNRIGHT)
                {
                    for(i = 0,j = 3; i<=3,j>=0; i++,j--)
            		{
            			STEPPER_1_PORT = full_step[i];
            			STEPPER_2_PORT = full_step[j];
            			_delay_us(speed);
            		}
                }
                else if(direction == TURNLEFT) 				      
                {
                    for(i = 3,j = 0; i >= 0,j <= 3; i--,j++)
            		{
            			STEPPER_1_PORT = full_step[i];
            			STEPPER_2_PORT = full_step[j];
            			_delay_us(speed);
            		}
                }
            }
   	    }
        break;

        case HALFSTEP :
	    {
           /* no half step implementation */  
		}
		break;

	    case QUARTERSTEP:
		{
            /* observations:
             *            minimum delay acceptable : 100 us 
             *            maximum delay acceptable : 45 ms 
             */
	        for (stepcounter = 0; stepcounter < stepcount; stepcounter++) 
		    {
                if(direction == FORWARD)  
			    {
				    for(i = 0; i <= 15; i++)
				    {
					    STEPPER_1_PORT = quarter_step[i];
					    STEPPER_2_PORT = quarter_step[i];
					    _delay_us(speed);
				    }
				}
			    else if(direction == REVERSE)
			    {
				    for(i = 15; i >= 0; i--)
			        {
				        STEPPER_1_PORT = quarter_step[i];
				        STEPPER_2_PORT = quarter_step[i];
				        _delay_us(speed);
				    }					
			    }
                else if(direction == TURNRIGHT)
                {
                    for(i = 0,j = 15; i<=15,j>=0; i++,j--)
            		{
            			STEPPER_1_PORT = quarter_step[i];
            			STEPPER_2_PORT = quarter_step[j];
            			_delay_us(speed);
            		}
                }
                else if(direction == TURNLEFT) 				      
                {
                    for(i = 15,j = 0; i >= 0,j <= 15; i--,j++)
            		{
            			STEPPER_1_PORT = quarter_step[i];
            			STEPPER_2_PORT = quarter_step[j];
            			_delay_us(speed);
            		}
                }				      
		    }
	    }		
		break;

        default:
				//warning "message during compiling";
		break;
       
    }
} 


