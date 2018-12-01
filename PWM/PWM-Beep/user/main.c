//*****************************************************************************
//
// 
// 
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_pwm.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"

//*****************************************************************************
//Attention
//TM4C123 NMI unlock - To those who want to use PF0 and PD7, be reminded that these pins defaults as NMI ! ! !
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// Global variables
// 
// 
//
//*****************************************************************************
typedef struct __Music_Note
{
    uint8_t pitch;
    uint16_t duration;   // ms
} Music_Note;

//C low
#define L1  262    //"L1"262Hz			
#define L2  286    //"L2"286Hz
#define L3  311    //"L3"311Hz
#define L4  349    //"L4"349Hz
#define L5  392    //"L5"392Hz
#define L6  440    //"L6"440Hz
#define L7  494    //"L7"494Hz
//C middle
#define Z1    523     //"Z1"523Hz
#define Z2    587     //"Z2"587Hz
#define Z3 	  659     //"Z3"659Hz
#define Z4    698     //"Z4"698Hz
#define Z5    784     //"Z5"784Hz
#define Z6    880     //"Z6"880Hz
#define Z7    987     //"Z7"523H      
//C high
#define H1  1046    //"H1"1046Hz
#define H2  1174    //"H2"1174Hz
#define H3  1318    //"H3"1318Hz
#define H4  1396    //"H4"1396Hz
#define H5  1567    //"H5"1567Hz
#define H6  1760    //"H6"1760Hz
#define H7  1975    //"H7"1975Hz
//sky 
unsigned  int fre[]={
	Z6,Z7,/**/             
	H1,Z7,H1,H3,/**/
	Z7,Z3,Z3,   /**/
	Z6,Z5,Z6,H1,/**/
	Z5,Z3,Z3,   /**/
	Z4,Z3,Z4,H1,Z3,H1,H1,H1,/**/
	Z7,Z4,Z4,Z7,/**/
	Z7,Z6,Z7,   /**/
	H1,Z7,H1,H3,/**/
	Z7,Z3,Z3,   /**/
	Z6,Z5,Z6,H1,Z5,Z3,/**/
	Z4,H1,Z7,H1,/**/
	H2,H2,H2,H1,/**/
	H1,Z6,Z7,Z5,/**/
	Z6,H1,H2,   /**/
	H3,H2,H3,H5,H2,Z5,Z5,/**/
	H1,Z7,H1,H3,/**/
	H3,         /**/
	Z6,H1,Z7,H2,/**/
	H1,Z5,Z5,   /**/
	H4,H3,H2,H1,H3,/**/
	Z3,H3,      /**/
	H6,H5,H5,   /**/
	H3,H2,H1,H1,/**/
	H2,H1,H2,H5,/**/
	H3,H3,H6,H5,/**/
  H3,H2,H1,H1,/**/
  H2,H1,H2,Z7,/**/
  Z6,Z6,Z7,   /**/
  Z6, 
};  
unsigned char beat[]={            
	4,4,  /**/
	12,4,8,8,   /**/
	20,4,4,     /**/
	12,4,8,8,   /**/
	20,4,4,     /**/
	12,4,4,12,20,4,4,4,   /**/
	12,4,8,8,   /**/
	20,4,4,     /**/
	12,4,8,8,   /**/
	20,4,4,     /**/
	12,4,8,8,32,4,/**/
	8,4,6,8,    /**/
	4,4,4,16,   /**/
	8,4,6,8,    /**/
	24,4,4,     /**/
	12,4,8,8,24,4,4,/**/
	4,4,8,8,    /**/
	32,			    /**/
	4,8,8,6,    /**/
	12,4,16,    /**/
	8,8,8,8,30,     /**/
	20,8,       /**/
	16,8,8,     /**/
	4,4,16,8,   /**/
	8,4,4,8,    /**/
	20,8,16,16, /**/
  4,20,8,8,   /**/
  8,4,4,8,    /**/
  20,4,4,     /**/
  32,
};

uint8_t note_ptr;
//*****************************************************************************
//
//   
// PWM BEEP
// 
//
//*****************************************************************************

void sleep(uint32_t ms)
{
    uint32_t delay;
    while (ms --)
        for (delay = 18000; delay; delay --)
            ;
}

void pwm_play_next(void)
{

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/fre[note_ptr]);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/fre[note_ptr]*4/5);
    
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

    sleep(beat[note_ptr]*200);
    
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);

    note_ptr ++;
    if( (note_ptr%112)==0 )
    {
        note_ptr = 0;
    }
   
}

//*****************************************************************************
//
// The main application.  
// M0PWMG3--PWMOUT7----PC5
//
//*****************************************************************************
int
main(void)
{

    //
    // Set the system clock to run at 40MHz
    //
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);
    //
    // Set the PWM clock to the system clock.
    //
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
		
    //
    // The PWM peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //
    // For this example PWM0 is used with PortB Pin6.  The actual port and pins
    // used may be different on your part, consult the data sheet for more
    // information.  GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		
    //
    // Configure the GPIO pin muxing to select PWM00 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PC5_M0PWM7);

    //
    // Configure the PWM function for this pin.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5);

    //
    // Configure the PWM0 to count down without synchronization.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);		
		
    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 1000Hz) * 40MHz = 40000 cycles.  Note that
    // the maximum period you can set is 2^16.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 40000);

    //
    // For this example the PWM0 duty cycle will be variable.  
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 10);

    //
    // Enable the PWM0 output signal (PD0).
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);

    //
    // Enables the PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
		
    while(1)
    {
			   pwm_play_next();
    }
}
