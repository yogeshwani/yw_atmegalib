/* header file for the timer system for the avr chips 

NOTES:
the file contains various macros and function definitions for the timer subsystem.
the timer files will have to deal with setting up the timer in the mode requested, 
the prescale factor as well as seeing if the interrupts are being required or not. 
the timer will be set and finished in the timer init function adn will be started 
when a call is being made to teh stat timer function/macro . 
This subsystem should also take care of using the timer specified .. so if timer 1 
is asked for use it if timer 2 then use that etc etc.. 
Check differences between starting timers in different modes and also the 
interrupt and non interrupt behaviours. the pwm mode will be encomapsed in here as
well however to make  things easier to read and maintain the pwm libraries can have
nothing but a set of macros which will call the functions int eh timer system

NOTE : 
1) the timer0 for most of the devices(atleast the ones commonly used)have
teh same registers and the same name for the required bits. they do however difer in
positions with the registers and that will be resolved by the device file that is been
included. 

2) if a pin on the atmega has to be used with a timer (pinsa are the timer pins like OC)) etc etc ..)
   then that has to indicated while initialzing the timer  for etc etc.. for PWM the signal is obviously available on the external pin and
   hence that prameter is always true. this can taken care while initilzin the timer if mode is pwm then 
   call some static function with the parameter as true else it is false 

3) Macro Call such as setWGMNBits(mode,location) SetPrescaleBits can be made inside the init function to hide the implemetation even further 
  e.g. setWGMNBits(3,WMG)) 3<<WGM0 more further the 3 can be #defined to say TIMER_0_PWM_PHASECORRECT 
current implemetation will try to make use of the 8 bit timer 0 in all modes possible

*/



/* IMPLEMATAION NOTE 

THE ENUMS MACROS ETC ARE ALL DONE FOR JUST TIMER 0 IN MIND WHEN IMPLEMENTING 
ALL ENUMS CAN BE PREFIXED WITHE THE IDENTIFIER TIMER0 WHEN IMPLEMTATION STARTS FOR TIMER1 AND TIMER 2 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include <avr/io.h>
#include <stdbool.h>
/*-------------
 * HASHDEFINES
 --------------*/
#define NO_OF_TIMERS 3



/*-------
 * ENUMS
 --------*/
/* enum for the timer used .. consider moving this to #defines for memory constraints */
typedef enum timer
{
    TIMER_0_8_BITS,
    TIMER_1_16_BITS,
    TIMER_2_8_BITS
}TIMER;

typedef enum timer_modes
{
    NORMAL,
    CLEAR_TIMER_ON_COMPARE_MATCH,
    PWM_PHASE_CORRECT,
    FAST_PWM        
}TIMER_MODES;

typedef enum oc_pin_mode
{
	NORMAL_CTC_NORMAL_DISCONNECTED =  0,
	NORMAL_CTC_TOGGLE_ON_COMPARE_MATCH = (0 << COM01) | (1 << COM00),
	NORMAL_CTC_CLEAR_ON_COMPARE_MATCH = (1 << COM01) | (0 << COM00),
	NORMAL_CTC_SET_ON_COMPARE_MATCH = (1 << COM01) | (1 << COM00),
	FAST_PWM_NORMAL_DISCONNECTED =  0,
	FAST_PWM_RESERVED = (0 << COM01) | (1 << COM00),
	FAST_PWM_CLEAR_ON_COMPARE_MATCH_SET_AT_BOTTOM = (1 << COM01) | (0 << COM00),
	FAST_PWM_SET_ON_COMPARE_MATCH_CLEAR_AT_BOTTOM = (1 << COM01) | (1 << COM00),
	PHASE_CORRECT_PWM_NORMAL_DISCONNECTED =  0,
	PHASE_CORRECT_PWM_RESERVED = (0 << COM01) | (1 << COM00),
	/* this actually should be read as 
	CLEAR_ON_COMPARE_MATCH_WHEN_UPCOUNTING_SET_AT_BOTTOM_WHEN_DOWNCOUNTING*/
	PHASE_CORRECT_PWM_CLEAR_ON_COMPARE_MATCH_SET_AT_BOTTOM = (1 << COM01) | (0 << COM00),
	/* this actually should be read as 
	SET_ON_COMPARE_MATCH_WHEN_UPCOUNTING_CLEAR_AT_BOTTOM_WHEN_DOWNCOUNTING*/
	PHASE_CORRECT_PWM_SET_ON_COMPARE_MATCH_CLEAR_AT_BOTTOM = (1 << COM01) | (1 << COM00)
}OC_PIN_MODE;

typedef enum cpu_clk_prescale
{
    NO_CLK_SRC,
    CLK_NO_PRESCALE,
    CLK_DIV_8,
    CLK_DIV_64,
    CLK_DIV_256,
    CLK_DIV_1024,
    EXT_CLK_FALLING_EDGE, /* EXTERNAL CLOCK PIN IS ALWAYS 'TO' FOR TIMER 1*/
    EXT_CLK_RISING_EDGE
}CPU_CLK_PRESCALE;

typedef enum interrupt_type
{
    TIMER_0_8_BITS_OVERFLOW = 0x01,
    TIMER_0_8_BITS_OUTPUT_COMPARE_MATCH = 0x02,
    TIMER_1_16_BITS_OVERFLOW = 0x04,
    TIMER_1_16_BITS_1B_OUTPUT_COMPARE_MATCH = 0x08,
    TIMER_1_16_BITS_1A_OUTPUT_COMPARE_MATCH = 0x10,
    TIMER_1_16_BITS_INPUT_COMPARE = 0x20,
    TIMER_2_8_BITS_OVERFLOW = 0x40,
    TIMER_2_8_BITS_OUTPUT_COMPARE_MATCH = 0x80
}INTERRUPT_TYPE;	

/*------------
 * STRUCTURES
 -------------*/
/*the user needs to populate this structure and then just call TimerInit in the main routine which will just initialize
the timer based on the information in the structure. However the timer will not be started until the user wishes and hence he needs to call start timer
with the time specified int eh function macro which will then set the clock and start the timer based on the information provided in this structure
*/

typedef struct timer_stp
{
	TIMER timer; /* identifies which timer to use */
	bool activated; /* set to true if you wish to use the timer else false*/
	TIMER_MODES timer_mode; /* mode in which the timer should operate ..pwm,ctc,icmp etc etc..*/
	OC_PIN_MODE OC_mode; /* set the behavior for the OC pin if connected */
	CPU_CLK_PRESCALE cpu_clk_prescale; /* CPU clock to be used */
	bool interrupt_enabled; /* whether interrupts should be set for this timer*/
	INTERRUPT_TYPE type_of_interrupt;
}TIMER_SETUP;

TIMER_SETUP timer_setup[NO_OF_TIMERS];

/*--------
 * MACROS
 ---------*/
/* TODO complete these macros*/
#define FillTimer0Structure(tim,act,mode,OC,clk,int_en,int_type) do { \
	   timer_setup[0].timer = (tim); \
	   timer_setup[0].activated = (act); \
	   timer_setup[0].timer_mode = (mode); \
	   timer_setup[0].OC_mode = (OC); \
	   timer_setup[0].cpu_clk_prescale = (clk); \
	   timer_setup[0].interrupt_enabled = (int_en); \
	   timer_setup[0].type_of_interrupt = (int_type); } while(0)
	
#define FillTimer1Structure(tim,act,mode,OC,clk,int_en,int_type) do { \
	   timer_setup[1].timer = (tim); \
	   timer_setup[1].activated = (act); \
	   timer_setup[1].timer_mode = (mode); \
	   timer_setup[1].OC_mode = (OC); \
	   timer_setup[1].cpu_clk_prescale = (clk); \
	   timer_setup[1].interrupt_enabled = (int_en); \
	   timer_setup[1].type_of_interrupt = (int_type); } while(0)

#define FillTimer2Structure(tim,act,mode,OC,clk,int_en,int_type) do { \
	   timer_setup[2].timer = (tim); \
	   timer_setup[2].activated = (act); \
	   timer_setup[2].timer_mode = (mode); \
	   timer_setup[2].OC_mode = (OC); \
	   timer_setup[2].cpu_clk_prescale = (clk); \
	   timer_setup[2].interrupt_enabled = (int_en); \
	   timer_setup[2].type_of_interrupt = (int_type); } while(0)
	
#define SetIntOvflT0()  TIMSK |= 1 << TOIE0 /* set the TIMSK regs to the overflow interrupt */
#define SetIntOvflT2()  TIMSK |= 1 << TOIE2
#define SetIntCompMatchT0() TIMSK |= 1 << OCIE0/* set the TIMSK regs to the compare match interrupt with out distrubing the otherws so use or operaion */
#define SetIntCompMatchT2() TIMSK |= 1 << OCIE2

#define SetTCCR0(val) TCCR0 |= (val)/* set the TCCR0 register with the value passed in */ 
#define SetTCCR2(val) TCCR2 |= (val)

#define StartTimer0() TCCR0 |= timer_setup[TIMER_0_8_BITS].cpu_clk_prescale << CS00/* the timer has to be started with the selected cpu prescale */  
#define StartTimer1()
#define StartTimer2() TCCR2 |= timer_setup[TIMER_2_8_BITS].cpu_clk_prescale << CS20)/* the timer has to be started with the selected cpu prescale */  
#define StopTimer0() TCCR0 |= 0 << CS00
#define StopTimer1() 
#define StopTimer2() TCCR2 |= 0 << CS20

#define LoadTCNT() /* same macro for all timers ??*/
#define LoadOCCR() /*same macro for all timers ??*/

/*----------------------
 * FUNCTION DECLARATION 
 -----------------------*/
void TimerInit();
void TimerSetupAll(TIMER timer);

#endif
