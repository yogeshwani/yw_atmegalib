/*
 * File : adc.c
 *
 * Description:
 * File contains the necessary functions for using the internal adc of the AVR microcontroller 
 *
 * Note: 
 * For detail documentation about the different functions and the use of ADC refer the header file adc.h 
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */
#include <avr/io.h>
#include <stdbool.h>
#include "adc.h"

/*
 * Function: AdcInit()
 * 
 * Description: Initializes the Adc module. For more details see adc.h
 *
 * Returns: Nothing 
 */

/* implementation note :
   Also things like the auto trigger and interrupt handling is still missing 
   may be that can be implemented int he future 
   An additional thing that needs implementing is the differential channel 
   selection with adjustable gain 
   for this implementation the positive and the negative channel number can be taken 
   as input parameter along with the gain required !!
*/ 
void AdcInit(REFERENCE_VOLTAGE ref_volts, ADC_CLK_PRESCALE adc_prescale,
             bool left_justified, bool interrut_enabled/*,auto_trigger_enable*/)
{
     /* ADC Usage : */
     /* Make the port of the adc as teh input ?? really is this needed ??*?*/
	 
	 /*Enable the Adc */
	 EnableAdc();
	
	 /*Select the prescale (conversion speed)
	   THe FCPU in a normal case is 8MHz/16MHz a conversion speed of 8MHZ by 128 
	   is good enough.This pre-scaling factor is kept as default if nothing or 
	   wrong prescale is provided which should not be the case as it needs an 
	   enum which has to have a value which is defined.
     */
	 switch(adc_prescale)
     {
		 case CLK_DIV_BY_2 :
	         ADCSRA |= (1<<ADPS0);	 
		 break;
		 case CLK_DIV_BY_4 :
		     ADCSRA |= (2<<ADPS0);
		 break;
		 case CLK_DIV_BY_8 :
		     ADCSRA |= (3<<ADPS0);
		 break;
		 case CLK_DIV_BY_16 :
		     ADCSRA |= (4<<ADPS0);
		 break;
         case CLK_DIV_BY_32 :
		     ADCSRA |= (5<<ADPS0);
		 break;
         case CLK_DIV_BY_64 :
		     ADCSRA |= (6<<ADPS0);
		 break;
		 case CLK_DIV_BY_128 :
		     ADCSRA |= (7<<ADPS0);
		 break;
         default:
		     ADCSRA |= (7<<ADPS0);
		 break;  
	 }	 
	    
	/*Select the reference voltage for the ADC 
      Bits REFS1 and REFS0 in ADMUX reg 
      Can be internal 2.56/ AVCC / or AREF (connected to external reference source)
	  the default is the internal 2.56v 
	  */
	 switch (ref_volts)
	 {
		 case REF_VLTG_AREF :
		     ADMUX |= (REF_VLTG_AREF<<REFS0);
		 break;
		 case REF_VLTG_AVCC :
		     ADMUX |= (REF_VLTG_AVCC<<REFS0);
		 break;
		 case REF_VLTG_256_INTERNAL :
		     ADMUX |= (REF_VLTG_256_INTERNAL<<REFS0);
		 break;
		 default:
		     ADMUX |= (REF_VLTG_256_INTERNAL<<REFS0);
		 break;
		 
	 }
	/* Se the respective bit if left justified result is asked else right justify the result */
	  if (left_justified)
	      ADMUX |= (1<<ADLAR); // set the ADLAR bit 
	  else 
	      ADMUX &= ~(1<<ADLAR); // clear the ADLAR bit
}

uint16_t ReadAdc(uint8_t channel,bool shift_by_6_to_right)
{
	 uint16_t adc_value;
	
	channel = (channel & MAX_NO_OF_ADC_CHANNELS);
    /* Masthe higher nibble in order to prevent it form getting overwritten and clear the channel 
    in order to write the new one */
    ADMUX &= 0xE0; 
    SetChannel(channel);
	
	/*Start the conversion and wait until ADIF is high (or interrupts can be used)*/
	StartAdcConversion();
    while(!(ADCSRA & (1<<ADIF)));
	
	/* Get data from the ADCH and ADCL register
	 * This can be directly accessed by using ADC 
	 */
	adc_value = ADC;
	
	/* Write one back to the ADIF bit to clear it */
	AdcReset();
	if(!shift_by_6_to_right)
    {
        return(adc_value);
    }
    else
    {
        return(adc_value >> 6);
    }
}

void DisableInternalADC()
{
    /* Clearing the ADC Enable bit would disable the adc */	
    ADCSRA &= ~(1<<ADEN);
}
