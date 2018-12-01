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
#include "inc/hw_adc.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
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

#include "utils/uartstdio.h"

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
unsigned char flag;
		//
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
uint32_t pui32ADC0Value[8],sum;
uint32_t anvenrage,value;
uint32_t  lastvalue,newvalue,temp;
//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, SysCtlClockGet());
}

void ADC0Sequence0Handler(void)
{

uint16_t i;
	
	   ADCIntDisable(ADC0_BASE, 0);
	
        //
        // Clear the ADC interrupt flag.
        //
        ADCIntClear(ADC0_BASE, 0);

        //
        // Read ADC Value.
        //
        ADCSequenceDataGet(ADC0_BASE, 0, pui32ADC0Value);
	
	 // y[i] = y[i-1] + a*(x[i] - y[i-1])
	      
   for(i = 0;i < 8;i ++)
    {
     sum= sum+pui32ADC0Value[i];
    }
		
		anvenrage=sum/8;
		
		sum=0;
		
		flag=1;
		
		ADCIntEnable(ADC0_BASE, 0);
		
}

void pwm_play_next(void)
{ 
	
	  PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 40000/value);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 20000/value);

		PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

}

//*****************************************************************************
//
// The main application.  Example demonstrating the internal ADC temperature sensor.
// ADC0 to read the internal temperature sensor.
// 
//
//*****************************************************************************
int
main(void)
{

	  //
    // The FPU should be enabled because some compilers will use floating-
    // point registers, even for non-floating-point code. If the FPU is not
    // enabled this will cause a fault.  This also ensures that floating-
    // point operations could be added to this application and would work
    // correctly and use the hardware floating-point unit.  Finally, lazy
    // stacking is enabled for interrupt handlers.  This allows floating-
    // point instructions to be used within interrupt handlers, but at the
    // expense of extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    //
    // Set the system clock to run at 200/5=40MHz using the PLL.  When
    // using the ADC, you must either use the PLL or supply a 16 MHz clock
    // source.
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
	
    //
    // enable processer interrupt
    //		
    IntMasterEnable();
			
    //
    // Set up the serial console to use for displaying messages.  This is just
    // for this example program and is not needed for ADC operation.
    //
    ConfigureUART();	

    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // Set the PWM clock to the system clock.
    //
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //
    // The PWM peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	 
    //
    // Display the setup on the console.
    //
    UARTprintf("ADC ->\n");
    UARTprintf("  Input Pin: AIN2/PE1\n\n");

    //
    // example ADC0 is used with AIN3 on port E0.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // For this example PWM0 is used with PortB Pin6.  The actual port and pins
    // used may be different on your part, consult the data sheet for more
    // information.  GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Select the analog ADC function for these pins.
    //
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);


    //
    // Enable sample sequence 0 with a processor signal trigger.  Sequence 0
    // will do a single sample when the processor sends a singal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 0.
    //
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0--7 on sequence 0.  
    //
		ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH2 );
		ADCSequenceStepConfigure(ADC0_BASE, 0, 6, ADC_CTL_CH2 );
    ADCSequenceStepConfigure(ADC0_BASE, 0, 7, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END);

    //
    // enable ADC0 sequence 0 interrupt
    //
    ADCIntEnable(ADC0_BASE, 0);
    IntEnable(INT_ADC0SS0);

    //
    // Since sample sequence 0 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 0);
		
    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntClear(ADC0_BASE, 0);

    //
    // Configure the GPIO pin muxing to select PWM00 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PF3_M1PWM7);

    //
    // Configure the PWM function for this pin.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Configure the PWM0 to count down without synchronization.
    //
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);		
		
    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 1000Hz) * 40MHz = 40000 cycles.  Note that
    // the maximum period you can set is 2^16.
    //
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, 400);

    //
    // For this example the PWM0 duty cycle will be variable.  
    //
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 200);

    //
    // Enable the PWM0 output signal (PD0).
    //
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);

    //
    // Enables the PWM generator block.
    //
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);


    //
    // Sample the temperature sensor forever.  Display the value on the
    // console.
    //
    while(1)
    {
        //
        // Trigger the ADC conversion.
        //
        ADCProcessorTrigger(ADC0_BASE, 0);

//		    //
//        // Wait for conversion to be completed.
//        //
//        while(!ADCIntStatus(ADC0_BASE, 0, false))
//        {
//        }
           
        if(flag==1)
				{
				  //UARTprintf("%d\n",avenrage);(40000-(avenrage-1)*400*99/4095)/400
					//UARTprintf("%d\n",(100-(avenrage-1)*99/4095));
					newvalue=anvenrage;
					if((newvalue-lastvalue)>=3 || (newvalue-lastvalue)<=3)
					{	
					value=(anvenrage*99/4095)+1;
					}	
					UARTprintf("%d\n",value);
					pwm_play_next();
				  flag=0;
					lastvalue=anvenrage;
				}

//				//
//				//sample delay
//				//
//        SysCtlDelay(SysCtlClockGet() *5/ 24);		
    }
   
}





