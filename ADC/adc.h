/**
 * @file adc.h
 * 
 * @brief Header file for the adc.c file
 *  
 * Contains the pin configurations, enums macros and function definitions for the adc   
 *
 *
 *   Important things to know about the adc 
 *   REGISTERS :
 *    ADMUX   
 * ------------------------------------------------------------
 * | REFS1 | REFSO | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUXO | 
 * ------------------------------------------------------------
 * REFSl:0 Bit 7:6 Reference Selection Bits
 *               These bits select the reference voltage for the ADC.
 *	
 * ADLAR Bit 5 ADC Left Adjust Results
 *				This bit dictates either the left bits or the right bits of the result registers ADCH:ADCL
				that are used to store the result. If we write a one to ADLAR, the result will be left
				adjusted; otherwise, the result is right adjusted.
	MUX4:0 Bit 4:0 Analog Channel and Gain Selection Bits
				   The value of these bits selects the gain for the differential
				   channels and also selects which combination of analog inputs
				   are connected to the ADC.
				   
 ADCH and ADCL registers 
               These registers store the 10 bit digital value after the conversion from the ADC 
			   (ADLAR would decide the left/right adjustment. The upper or lower 6 bits out of 16 
			    are unused and hence ADLAR would decide which ones be unused )

ADCSRA 	
--------------------------------------------------------------			
| ADEN | ADSC |  ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPSO |
--------------------------------------------------------------
ADEN Bit 7 ADC Enable
	This bit enables or disables the ADC. Setting this bit to one will enable the ADC, and
	clearing this bit to zero will disable it even while a conversion is in progress.

ADSC Bit 6 ADC Start Conversion
	To start each conversion you have to set this bit to one.

ADATE Bit 5 ADC Auto Trigger Enable
	Auto triggering of the ADC is enabled when you set this bit to one.

ADIF Bit 4 ADC Interrupt Flag
	This bit is set when an ADC conversion completes and the data registers are updated.

ADIE Bit 3 ADC Interrupt Enable
	Setting this bit to one enables the ADC conversion complete interrupt.

ADPS2:0 Bit 2:0 ADC Prescaler Select Bits
	These bits determine the division factor between the XTAL frequency and the input
	clock to the ADC.
*/


#ifndef _ADC_H_
#define _ADC_H_

#define USE_ADC_IN_INTERRUPT_MODE 0
#define MAX_NO_OF_ADC_CHANNELS 0x07

typedef enum adc_clk_prescale
{
	CLK_DIV_BY_2, 
    CLK_DIV_BY_4, 
    CLK_DIV_BY_8,
	CLK_DIV_BY_16,
	CLK_DIV_BY_32, 
    CLK_DIV_BY_64,
	CLK_DIV_BY_128
}ADC_CLK_PRESCALE;


typedef enum reference_voltage
{
    REF_VLTG_AREF = 0,
    REF_VLTG_AVCC = 1,
    REF_VLTG_256_INTERNAL = 3
}REFERENCE_VOLTAGE;


/* MACROS */
#define EnableAdc() ADCSRA |= (1<<ADEN);
#define DisableAdc() DisableInternalADC();
#define StartAdcConversion() ADCSRA |= (1<<ADSC);
#define SetChannel(channel) ADMUX |= (channel);
#define AdcReset() ADCSRA |= (1<<ADIF);

/* USage for using the adc either in polling or using interrupt 
   TBE implement and test properly */

#if USE_ADC_IN_INTERRUPT_MODE
    #define AdcEnableInterrupt() ADCSRA |= (1<<ADIE);
#endif

/** Initialize the ADC 
 * setups the adc based ont eh values provided as parameters to this function
 */
void AdcInit(REFERENCE_VOLTAGE ref_volts, ADC_CLK_PRESCALE adc_prescale,
             bool left_justified, bool interrut_enabled/*,auto_trigger_enable*/);


/** @brief  Read the value after the conversion from the ADC 
 * 
 * Takes the channel from which to read (0-7), the value passed in as bit wise
 * anded with the max no of channel to make sure it doesnt exceed the no of 
 * channels 
 * @param channel The channel number of the ADC to read from 
 * @param true_value The true_value of teh adc read rather than the 8 bit value 
 * @return uint16_t (16bit) value from the analog to digital conversion
 */
uint16_t ReadAdc(uint8_t channel, bool true_value);

#endif 

