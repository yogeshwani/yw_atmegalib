/*
 * File : timer.c
 *
 * Description:
 * File contains the necessary functions for using the internal Timers of the AVR microcontroller 
 *
 * Note: 
 * For detail documentation about the different functions and the use of USART refer the header file timer.h 
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */
 
/* 
   The following are the registers which need addressing in the timer 0
	 initialization routine. Based what parameters are passed in we need to make sure things are set accordingly 
	 TCNT0 and OCR0 are the registers to be used to load a particular value 
	 TIMSK and TIFR are all to do with monitoring events and needs to accessed accordingly 
	 the SFIOR can be dealt with later 
	 TCCRo is counter control and mode setting register and had to dealt accordingly  
   TCCR0 TCNT0 OCR0 TIMSK TIFR SFIOR
*/

#include <inttypes.h>
#include "timer.h"

/* This function should be static here */
void timer_setup_all(TIMER timer)
{
    uint8_t val = 0;
    uint8_t mode = 0;
    uint8_t OC0_mode = 0;
    switch(timer)
    {
        /* set up timer 0 registers */	 
        /* the registers to be used are 
        1) tccro - > to set the cpu clock, timer mode of operation, timer mode of operation 
        2) tcnto -> counter whcih increments decrements every clock tick 
        3) ocr0 - > ouptu comapre reigster to be set in different modes of operation 
        4) timsk -> to be set for interrupts 
        5) tifr -> interrupt flag register.. the bits are set when some condition occurs whcih triggers interupt 
        6) sfior special function io register 
        */
        /* 
        INFO: the ways to reduce this huge line of code is to haev the enums with the ored expression for each mode 
        and tehn we can just see the enum abd assign teh value.. This is now implemented
        */
        case TIMER_0_8_BITS:
            mode = timer_setup[TIMER_0_8_BITS].timer_mode;
            if (mode == NORMAL)
            {
                val |=  ((0 << WGM01) | (0 << WGM00) );
                OC0_mode = timer_setup[TIMER_0_8_BITS].OC_mode;
                val |= OC0_mode;
                
            }
            else if( mode == PWM_PHASE_CORRECT)
            {
                val |= ((0 << WGM01)| (1 << WGM00) );
                OC0_mode = timer_setup[TIMER_0_8_BITS].OC_mode;
                val |= OC0_mode;
            }
            else if( mode == CLEAR_TIMER_ON_COMPARE_MATCH)
            {
                val |=  ((1 << WGM01)| (0 << WGM00) );
                OC0_mode = timer_setup[TIMER_0_8_BITS].OC_mode;
                val |= OC0_mode;
            }
            else if( mode == FAST_PWM)
            {
                val |= ((1 << WGM01)| (1 << WGM00)) ;
                OC0_mode = timer_setup[TIMER_0_8_BITS].OC_mode;
                val |= OC0_mode;
            }  
            else
            {
                /* specify some error here*/
            }   
            
            /* Set the interrupt type set by the user */
            if (timer_setup[TIMER_0_8_BITS].interrupt_enabled == true) 
            {
                if (timer_setup[TIMER_0_8_BITS].type_of_interrupt & TIMER_0_8_BITS_OUTPUT_COMPARE_MATCH)
                {
                    SetIntOvflT0();
                }
                if(timer_setup[TIMER_0_8_BITS].type_of_interrupt & TIMER_0_8_BITS_OVERFLOW)
                {
                    SetIntCompMatchT0();
                }
            }
            else
            {
                /* specify error here*/
            } 
			/* Finally setup the timer counter control register */
			SetTCCR0(val);			
        break;
        
        /* set up timer 1 registers */
        case TIMER_1_16_BITS :
        break;
        
        /* set up timer 2 registers */
		/* NOTE :
		        The asynchronous operation is not yet implemented and needs implementing 
		 */
        case TIMER_2_8_BITS :
		    mode = timer_setup[TIMER_2_8_BITS].timer_mode;
		    if (mode == NORMAL)
		    {
			    val |=  ((0 << WGM21)| (0 << WGM20));
                OC0_mode = timer_setup[TIMER_2_8_BITS].OC_mode;
				val |= OC0_mode;
		    }
		    else if( mode == PWM_PHASE_CORRECT)
		    {
			    val |= ((0 << WGM21)| (1 << WGM20) );
				OC0_mode = timer_setup[TIMER_2_8_BITS].OC_mode;
				val |= OC0_mode;
		    }
		    else if( mode == CLEAR_TIMER_ON_COMPARE_MATCH)
		    {
			    val |= ((1 << WGM21)| (0 << WGM20));
				OC0_mode = timer_setup[TIMER_2_8_BITS].OC_mode;
				val |= OC0_mode;
		    }
		    else if( mode == FAST_PWM)
		    {
			    val |= ((1 << WGM21)| (1 << WGM20));
				OC0_mode = timer_setup[TIMER_2_8_BITS].OC_mode;
				val |= OC0_mode;			    
		    }
		    else
		    {
			    /* specify some error here*/
		    }
		            
		    /* Set the interrupt type set by the user */
		    if (timer_setup[TIMER_2_8_BITS].interrupt_enabled == true)
		    {
			    if (timer_setup[TIMER_2_8_BITS].type_of_interrupt & TIMER_2_8_BITS_OUTPUT_COMPARE_MATCH)
			    {
				    SetIntOvflT2();
			    }
			    if (timer_setup[TIMER_2_8_BITS].type_of_interrupt & TIMER_2_8_BITS_OVERFLOW)
			    {
				    SetIntCompMatchT2();
			    }
		    }
		    else
		    {
			    /* specify error here*/
		    }
		    /* Finally setup the timer counter control register */
		    SetTCCR2(val);
        break;

        /* just to avoid any warnings*/
        default:        
        break;
    }
  }
  

  /* Implementation Note timer init can tak ien the use defien d function pointer for differnt inerruprs may be */

  void TimerInit()
  {
	  uint8_t i;

	  for (i = 0; i < NO_OF_TIMERS; i++)
	  {
		  if(timer_setup[i].activated == true)
		  { 
			  timer_setup_all(timer_setup[i].timer);
		  }
		  else
		  {
			  continue;
		  }
	  }
  }
