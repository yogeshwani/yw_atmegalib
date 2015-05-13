/************************************************************************
 * Name : usart.h 
 * 
 * Header file for the usart.c file
 *  
 * Contains the pin configurations, enums macros and function definitions 
 * for the USART library function  
 *
 ************************************************************************/
/************
Important stuff related to USART 
REGISTERS 

	UDR : USART Data register
	      This register consists of the transmit data buffer and receive data buffer. 
		  When we read for the UDR the receive data buffer is read and when we 
		  write to the UDR the transmit data buffer is written with the byte value to be transmitted  
	
	UCSRA 
	------------------------------------------------
	|RXC | TXC | UDRE | FE | DOR | PE | U2X | MPCM |
	------------------------------------------------
	RXC (Bit 7): USART Receive Complete
				 This flag bit is set when there are new data in the receive buffer
				 that are not read yet. It is cleared when the receive buffer is empty. 
				 It also can be used to generate a receive complete interrupt.
	TXC (Bit 6): USART Transmit Complete
	             This flag bit is set when the entire frame in the transmit shift register
				 has been transmitted and there are no new data available in the transmit 
				 data buffer register (TXB). It can be cleared by writing a one to its bit
				 location. Also it is automatically cleared when a transmit complete interrupt
				 is executed. It can be used to generate a transmit complete interrupt.
    UDRE (Bit 5): USART Data Register Empty
				 This flag is set when the transmit data buffer is empty and it is ready to receive
				 new data. If this bit is cleared you should not write to UDR because it overrides
				 your last data. The UDRE flag can generate a data register empty interrupt.
    FE (Bit 4): Frame Error
	             This bit is set if a frame error has occurred in receiving the next character in the
                 receive buffer. A frame error is detected when the first stop bit of the next character
				 in the receive buffer is zero.
    DOR (Bit 3): Data OverRun
                 This bit is set if a data overrun is detected. A data overrun occurs when the
                 receive data buffer and receive shift register are full, and a new start bit is detected.
	PE (Bit 2): Parity Error
			     This bit is set if parity checking was enabled (UPM 1 ~ I) and the next character in the
				 receive buffer had a parity error when received.
	U2X (Bit 1): Double the USART Transmission Speed
				 Setting this bit will double the transfer rate for asynchronous communication.
	MPCM (Bit 0): Multi-processor Communication Mode
                 This bit enables the multi-processor communication mode. The MPCM feature is not
				 discussed in this book.
	UCSRB
	-------------------------------------------------------------			 
    | RXCIE | TXCIE | UDRIE | RXEN | TXEN | UCSZ2 | RXB8 | TXB8 |
	-------------------------------------------------------------
	RXCIE (Bit 7): Receive Complete Interrupt Enable
					To enable the interrupt on the RXC flag in UCSRA you should set this bit to one.
	TXCIE (Bit 6): Transmit Complete Interrupt Enable
					To enable the interrupt on the TXC flag in UCSRA you should set this bit to one.
	UDRIE (Bit 5): USART Data Register Empty Interrupt Enable
					To enable the interrupt on the UDRE flag in UCSRA you should set this bit to one.
	RXEN (Bit 4): Receive Enable
					To enable the USART receiver you should set this bit to one.
	TXEN (Bit 3): Transmit Enable
					To enable the USART transmitter you should set this bit to one.
	UCSZ2 (Bit 2): Character Size
					This bit combined with the UCSZl:O bits in UCSRC sets the number of data bits
					(character size) in a frame.
	RXB8 (Bit 1): Receive data bit 8
					This is the ninth data bit of the received character when using serial frames with nine
					data bits. This bit is not used in this book.
	TXB8 (Bit 0): Transmit data bit 8
					This is the ninth data bit of the transmitted character when using serial frames with nine
					data bits. This bit is not used in this book.

	UCSRC
	--------------------------------------------------------------
	| URSEL | UMSEL | UPM1 | UPMO | USBS | UCSZl | UCSZO | UCPOL |
	--------------------------------------------------------------
	URSEL (Bit 7): Register Select
					This bit selects to access either the UCSRC or the UBRRH register and will be discussed
					more in this section.If URSEL = 0 then UBBRH is selected else UCSRC is selected. 
	UMSEL (Bit 6): USART Mode Select
					This bit selects to operate in either the asynchronous or synchronous mode of operation.
					0 = Asynchronous operation
					1 = Synchronous operation
	UPM1:0 (Bit 5:4): Parity Mode
					These bits disable or enable and set the type of parity generation and check.
					00 = Disabled
					01 = Reserved
					10 = Even Parity
					11 = Odd Parity
	USBS (Bit 3): Stop Bit Select
					This bit selects the number of stop bits to be transmitted.
					0= 1 bit
					1 = 2 bits
	UCSZ1:0 (Bit 2:1): Character Size
					These bits combined with the UCSZ2 bit in UCSRB set the character size in a frame
					and will be discussed more in this section.
	UCPOL (Bit 2): Clock Polarity
					This bit is used for synchronous mode only and will not be covered in this section.


UBBR : 
     This register is 16 bits wide as shown below 
	 --------------------------------------
	 | URSEL | -- | -- | -- | UBRR[11- 0] |
	 --------------------------------------
	 To calculate the value for the baud rate the following formula is used  
	       ______________________________________________________________
		   |	                                                         |
		   |	UBBR (value in dec) = (Fosc/ (16(Desired Baud Rate)) - 1 |
		   |_____________________________________________________________|
	 The URSEL bit selects which register is to be selected between accessing the UBRRH or the UCSRC register
	 as they share the same address 		
*************/ 
#ifndef _USART_H_
#define _USART_H_

/* ENUMS*/

typedef enum PARITY_SETTING
{
	NO_PARITY = 0,
	EVEN_PARITY = 2,
	ODD_PARITY  = 3
}PARITY_SETTING;

typedef enum USART_MODE
{
    ASYNCHRONOUS, /* 0 */
	SYNCHRONOUS   /* 1 */
}USART_MODE;

typedef enum STOP_BITS
{
	ONE_BIT,  /* 0 */  
	TWO_BITS  /* 1 */
}STOP_BITS;

typedef enum CHARACTER_SIZE
{
	CHARACTER_SIZE_5_BITS,
	CHARACTER_SIZE_6_BITS,
	CHARACTER_SIZE_7_BITS,
	CHARACTER_SIZE_8_BITS,
	CHARACTER_SIZE_9_BITS = 7
}CHARACTER_SIZE;

typedef enum BAUD_RATE
{
	BAUD_RATE_38400 = 38400,
	BAUD_RATE_19200 = 19200,
	BAUD_RATE_9600  = 9600, 
	BAUD_RATE_4800  = 4800, 
	BAUD_RATE_2400  = 2400, 
	BAUD_RATE_1200  = 1200
}BAUD_RATE;

/* MACROS*/
//#define FOSC 8000000 
//#define DENOMINATOR 16*FOSC
#define EnableRxTx()  UCSRB |= ((1<<RXEN) | (1<<TXEN))

void UsartInit(USART_MODE mode,PARITY_SETTING parity,STOP_BITS stop_bits,
               CHARACTER_SIZE ch_size, BAUD_RATE baud_rate );
void UsartSend(unsigned char data);
void UsartSendString(char *msg);
void UsartSendInteger(int16_t val);
unsigned char UsartReceive();

#endif

