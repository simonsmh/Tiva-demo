//*****************************************************************************
//
// reload_interrupt.c - Example demonstrating the PWM interrupt.
//
// Copyright (c) 2010-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This is part of revision 2.1.0.12573 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


//sin(2*pi*f*N) f=target frequency N=Sample number
//pulse * sin(2*pi*f*N)
uint8_t  i;	
uint16_t sinewave_sample[48] = { 
0x7FF,0x90A,0xA11,0xB0F,0xBFF,0xCDD,0xDA7,0xE57,0xEEC,0xF63,0xFB9,0xFED,0xFFE,0xFED,0xFB9,0xF63
,0xEEC,0xE57,0xDA7,0xCDD,0xBFF,0xB0F,0xA11,0x90A,0x7FF,0x6F4,0x5ED,0x4EF,0x3FF,0x321,0x257,0x1A7
,0x112,0x09B,0x045,0x011,0x000,0x011,0x045,0x09B,0x112,0x1A7,0x257,0x321,0x3FF,0x4EF,0x5ED,0x6F4
};

void spwm_play(void)
{
    int i_sin, delay;
    
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/100000); // 2400Hz

    for (i_sin = 0; i_sin < 48; i_sin ++)
    {
			PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet()/100000*(sinewave_sample[i_sin])*2/5/4096);
			PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
			 delay = 400;    
        while (delay --)
            ;

    }
		
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
		i_sin=0;
}
//*****************************************************************************
//
// Configure PWM0 for a load interrupt.  This interrupt will trigger everytime
// the PWM0 counter gets reloaded.  In the interrupt, 0.1% will be added to
// the current duty cycle.  This will continue until a duty cycle of 75% is
// received, then the duty cycle will get reset to 0.1%.
//
//*****************************************************************************
int
main(void)
{
    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
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
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
    // the maximum period you can set is 2^16.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/4000);

    //
    // For this example the PWM0 duty cycle will be variable.  The duty cycle
    // will start at 0.1% (0.01 * 64000 cycles = 640 cycles) and will increase
    // to 75% (0.5 * 64000 cycles = 32000 cycles).  After a duty cycle of 75%
    // is reached, it is reset to 0.1%.  This dynamic adjustment of the pulse
    // width is done in the PWM0 load interrupt, which increases the duty
    // cycle by 0.1% everytime the reload interrupt is received.
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);

    //
    // Enable the PWM0 output signal (PD0).
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);

    //
    // Enables the PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    //
    // Loop forever while the PWM signals are generated and PWM0 interrupts
    // get received.
    //
    while(1)
    {
        //
        // Print out indication on the console that the program is running.
        //
        //PrintRunningDots();
			spwm_play();
    }
}












