/*
 * File : lcd16x2.c
 *
 * Description:
 * File contains the necessary functions for using the 16 X 2 Alphanumeric Lcd with the Avr microcontroller 
 *
 * Note: 
 * For detail documentation about the working and the use of 16 X 2 LCD refer the header file lcd16x2.h 
 * Refer to E:/Avrprojects/library/coding standards.doc for coding standards
 *
 */

#include <util/delay.h>
#include <stdbool.h> 

/* extra include files
 * NOTE: for files not in the same directory .. it is important to give the relative path
 * at least in the library files as these files would just be linked in the project they 
 * are going to be used in and hence it is up to these files to sort stuff out for them. 
 * At the project level things have to be added differently and can see the documentation
 * in the header file (lcd16x2.h) 
 */
/* for use in AS5 */
/* #include "lcdpinconfig.h"
   #include "lcd16x2.h"
   #include "portconfig.h"*/

/* for use in AS4*/
#include "../../Library/PortConfig/portconfig.h"
#include "lcdpinconfig.h"
#include "lcd16x2.h"

/*TODO character generation*/
/*#include "characters.h"*/

/*
 * Function: LcdInit()
 * 
 * Description: Initializes the Lcd module. for more details see lcd16x2.h
 *
 * Returns: Nothing 
 */
void LcdInit(LCDMODE mode)
{
    /*set respective ports data direction register */
    setportdir(LCD_DDR_DATAPORT,OUTPUT);
	setportpindiroutput(LCD_DDR_CONTROLPORT,RS);
    setportpindiroutput(LCD_DDR_CONTROLPORT,RW);
    setportpindiroutput(LCD_DDR_CONTROLPORT,En);

	/*After powering up the Lcd wait for around 15ms, if the LcdInit is not the first function
	   to be called then it is fine */
    _delay_ms(15);

switch(mode)
{
    case  Lines2_5X7_8Bit_mode :

        /* To initialize the LCD to 5 x 7 matrix and 8 bit    
		   send the following commands to the Lcd, additional commands can be added 
		   based on the need 
    
		   1) 0x38  initialize the display to 5x7 matrix 2 lines and 4 bit mode */
		LcdSendCommand( Lines2Bit8_5x7 );
		_delay_ms(2);
	
		/*2) 0x0e  Display on cursor blink */
		LcdSendCommand( DispOnCurrBlink );
		_delay_ms(2);
    
		/*3) 0x01  Clear display */
		LcdSendCommand( ClearDisplay );
		_delay_ms(2);

		LcdSendCommand( IncCursor );
		_delay_ms(2);
break;
case Lines2_5X7_4Bit_mode:
    /* #warning "not implementwed"*/
break;

}
}

/*
 * Function: LcdSendString()
 * 
 * Description: Displays the string of characters passed in, on the LCD module 
 *
 * Returns: Nothing 
 */         
void LcdSendString(char *str)
{
    while (*str != '\0' )
	    {
		    LcdSendByte( (uint8_t) *str, true);
		    str++;	
	    }  
}

/*
 * Function: LcdSendByte()
 * 
 * Description: Sends the byte passed in as a parameter to the LCD and does the 
 * respective setting and clearing of LCd pins depending on if the byte is data 
 * or a LCd command  
 *
 * Returns: Nothing 
 * TODO* consider making this function as static
 */         
void LcdSendByte(uint8_t byte, bool isdata)
{
    /*Set pin RS (register select ) of the Lcd to 1 if it is for command else set to 0  */
    if (!isdata )
        ClearRS(); //RS = 0 for command register    
    else
        SetRS(); //RS = 1 for data register 
		 
    /*Set pin R/W (read/ write ) of the Lcd to 0 for writing  */
    ClearRW();
	
    /* Put the data on the data lines(make respective ports direction as output)*/
    /* This definitely needs a bit of touch up here */
    lcd_data = byte;
    
	/*  Send the low to high pulse on the E pin
    This can be combined in a macro names sendpulse() oe something similar  */
    SendPulseOnpinE();
    /*  After sending the data wait for around 100us to let the Lcd module write data 
        to the Lcd */
    _delay_us(100);
}

/*
 * Function: LcdGoToXY()
 * 
 * Description: Positions the cursor to the respective X,Y coordinates passed in 
 * where x is the column number and y is the line number 
 * 
 * Returns: Nothing 
 */         
void LcdGoToXY(uint8_t x, uint8_t y)
{ 
	uint8_t address; 
    if (y > 1 || x > 16) // y = 0 (line 1) y =1 (line 2 ) , x = (0-15) coulmn number  
    {
		LcdSendString("Check X,Y coordinate");        
	}
	else
	{
	    if ( y ) //if y is 1 which is first line ??		
		{
		    address = 0x80;			
		}
	    else
		{ 
		    address = 0xC0;
		}
		
		if (x<16)
		{
			address = address + x;
		}	
	    LcdSendCommand(address);
	}
}	

/*
 * Function: LcdSendInteger()
 * 
 * Description: Dispalys the Integer value passed in on the Lcd module 
 *
 * Returns: Nothing 
 */
void LcdSendInteger(int16_t val)
{
    /* The variable number stores the remainder of the integer value being passed
	 */
	uint8_t number[5]= {0,0,0,0,0}; 
    int8_t i;
    bool is_val_neg = false;
    if(val < 0)
    {
       LcdSendData('-');
	   is_val_neg = true; 
    }
     
	for(i= 0;i<=4;i++)
    {
        number[i] = val % 10;
        val= val / 10;
    }
    /* Display the stored numbers in reverse order, as they are stored inreverse order
     * in the upper for loop 
     */        
	for(i= 4; i>= 0; i--)
	{
        if(is_val_neg)
        {
            LcdSendData((48 + (number[i] * -1)));						
        }
        else
        {
            LcdSendData(48 + number[i]);			
        }
	}
}
