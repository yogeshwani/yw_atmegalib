/*
 * File : usart.c
 *
 * Description:
 * File contains the necessary functions for using the internal USART of the AVR microcontroller 
 *
 * Note: 
 * For detail documentation about the different functions and the use of USART refer the header file usart.h 
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */

#include <avr/io.h>
#include <stdbool.h>
#include "usart.h"

void UsartInit(USART_MODE mode,PARITY_SETTING parity,STOP_BITS stop_bits,CHARACTER_SIZE ch_size, BAUD_RATE baud_rate)
{  
	uint16_t ubrr_val = 0;
    uint8_t value = 0;

	/* USART initialization: 
	 *  Enable the receiver and the transmitter (in UCSRB register) */ 
    EnableRxTx();
 
 	/* Set the baud rate in the UBBRs */ 
	ubrr_val = ( uint16_t ) ( (F_CPU/(16UL * baud_rate))- 1 );
	UBRRH = ubrr_val >> 8;
	UBRRL = ubrr_val;

    /* While writing ot the UCSRC the MSB or the URSEL bit has to be set otherwise
	 * the UCSRC will not be written to 
     */

	/* Select the USART Mode (in UCSRC register )*/ 
	if(mode == ASYNCHRONOUS)
    {
	    /* The use of value variable here is to make sure that all the necessary parameters are 
		 * set which are asked for in the intialization routine and at the very end when everythign
		 * is doen it the value can be used with the MSB or the IRSEl bit as set and can be directly 
		 * written to teh UCSRC
		 */
	    //value |= (0<<UMSEL);
	    value &= ~(1<<UMSEL);
	}
	else if (mode == SYNCHRONOUS)
	{
	  	value |= (1<<UMSEL);
		
	}

    /* Set the character size (in UCSRC register ) 
	 * At the moment only 8 bit mode is supported and is the default.. 
	 * Any other value passed will result in the default being selected 
	 * TODO: Implemet handling different character sizes
	 */
  	switch( ch_size )
	{
		case CHARACTER_SIZE_8_BITS :
		    value |= (3<<UCSZ0);
	    break;

		/* rest of them would fall through and default to 8 bits of character*/ 
		case CHARACTER_SIZE_7_BITS :
		case CHARACTER_SIZE_6_BITS :
		case CHARACTER_SIZE_5_BITS :
		case CHARACTER_SIZE_9_BITS :

		default :
		    value |= (3<<UCSZ0);
		break;    
    }
		  
	/* Set the Parity Mode. 
     * Currently only NO_PARITY option is supported  and is the default one. 
	 * Any other value being passed will result in default being selected 
	 * here the different parity enums have been assigned the value as it would 
	 * be while setting the bits UPM1:0 as per datasheet 
     */
    switch(parity)
	{
	    case NO_PARITY :
		    value |= (NO_PARITY<< UPM0);
		break;
		  
		case EVEN_PARITY :
	        value |= (EVEN_PARITY<< UPM0);
		break;
		  
		case ODD_PARITY	:
		    value |= (ODD_PARITY<< UPM0);
		break;
		  
		default:
            value &= ~((1 << UPM1)|(1 << UPM0));
		break;
	}
	  
    /* Set the stop bit */
	if (stop_bits == ONE_BIT)
	{

	//    value |= (0<<USBS);  
	    value &= ~(1<<USBS);
	}
	else if (stop_bits == TWO_BITS)
	{
	    value |= (1<<USBS); 
	}

	/* Now Load the value to the UCSRC by setting the URSEl bit to 1*/
	UCSRC |= ((value)|(1<<URSEL));		  

}

void UsartSend(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) );
    /* Put data into buffer, sends the data */
    UDR = data;
}

unsigned char UsartReceive(void)
{
	unsigned char data;
    while ( !( UCSRA & (1<<RXC)) );
    /* Put data into buffer, sends the data */
    data = UDR;
    return data;
}

void UsartSendString(char *msg)
{
    while (*msg != '\0' )
    {
	    UsartSend(*msg);
        msg++;	
	}  
}


/*
 * Function: UsartSendInteger()
 * 
 * Description: send the number passed in on the usart
 *
 * Returns: Nothing 
 */
void UsartSendInteger(int16_t val)
{
	/*The variable number stores the remainder of the integer value being passed
	   whereas, the temp_val is used to store the number after the remainder has 
	   been stripped off 
	*/
	uint8_t number[5]= {0,0,0,0,0}; 
    int8_t i;
    bool is_val_neg = false;
    if(val < 0)
    {
       UsartSend('-');
	   is_val_neg = true; 
    }

    for(i= 0;i<=4;i++)
    {
        number[i] = val % 10;
        val = val / 10;
    }
    
	for(i= 4; i>= 0; i--)
	{
        /* This is to compensate for the fact that if the number is negative 
            while sending the final ascii it adds a anegative number which means
            an erronous value will be sent over */ 
	    if(is_val_neg)
        {
            UsartSend((48 + (number[i] * -1)));			
        }
        else
        {
            UsartSend((48 + number[i]));			
        }
	}   
}




