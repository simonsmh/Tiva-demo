#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/rom.h"

#include "driverlib/pwm.h"

#include "m4_common.h"

//uint16_t sinewave_sample[] = {
//    0x0800, 0x0A00, 0x0C00, 0x0E00, 0x0FFE, 0x0E00, 0x0C00, 0x0A00,
//    0x0800, 0x0500, 0x0300, 0x0100, 0x0001, 0x0100, 0x0300, 0x0500,
//};  // complement code
uint16_t sinewave_sample[] = { 0x0824, 0x9B3, 0xB31, 0xC8F, 0xDC1, 0xEBA, 0xF71, 0xFDF,
                               0x1000, 0xFD1, 0xF56, 0xE93, 0xD8F, 0xC54, 0xAEF, 0x96C,
                               0x7DC, 0x64D, 0x4CF, 0x0371, 0x23F, 0x0146, 0x8F, 0x0021,
                               0x0000, 0x2F, 0xAA, 0x16D, 0x0271, 0x3AC, 0x0511, 0x0694
                             };

void spwm_init(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    
    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/4000);
    
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
    
    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}

void spwm_play(void)
{
    int i_sin, delay, ip;
    
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/100000); // 120 kHz
    
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    
    for (i_sin = 0; i_sin < 32; i_sin ++)
    {
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)*(sinewave_sample[i_sin])/4096);
        
        delay = 200;    // 2.6 K
        while (delay --)
            ;
    }
    
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
}

void spwm_scanfreq(int8_t direction)
{
#define PWM_FREQ_LOW    1500
#define PWM_FREQ_HI     5000
#define PWM_FREQ_DELTA  25
    
    static uint32_t pwm_freq = PWM_FREQ_LOW;
    int8_t pwm_freq_delta = PWM_FREQ_DELTA*direction;
    
#if 0 // spwm test
    spwm_play();
    
    //PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 10000);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/100000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3)/100*99);
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SysCtlClockGet() - 1);
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
    
    while (1)
        ;
#endif
    
    for (pwm_freq += pwm_freq_delta; ((pwm_freq > PWM_FREQ_LOW) && (pwm_freq < PWM_FREQ_HI)); pwm_freq += pwm_freq_delta)
    {
        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/pwm_freq);
        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/2);

        sleep(1);
    }
    
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
}

void pwm_init(void)
{
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/100);
    
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0);
    
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
    
    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
}

void pwm_set(uint8_t percent)
{
    uint32_t width;
    
    width = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/100*percent;
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, width);
}

void pwm_set_ex(uint16_t adc_value)
{
    uint32_t width;

    width = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*adc_value/10000;
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, width);
}
