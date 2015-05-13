/* 
 * File name:
 *  portconfig.h 
 *
 * Description:
 * File contains all the macros and required definitions for port configurations
 */
#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_
/* Enum for the direction of a port */
typedef enum Direction
{
    OUTPUT = 0xFF,
    INPUT  = 0x00
}DIRECTION;
      

#define getDDR(port)  DDR##port
#define getPORT(port) PORT##port
#define getPIN(port)  PIN##port

/* For setting the direction(output/input) for a particular ports data direction register
 */ 
#define setportdir(port,value) (port) = (value)

/* For setting the direction of a particular pin of a port to output or input
 * same rules apply as above for parameters being passed into the macro*/
#define setportpindirinput(port,pin)  (port) &= ~(1<<(pin)) 
#define setportpindiroutput(port,pin) (port) |= (1<<(pin)) 
  
#endif

