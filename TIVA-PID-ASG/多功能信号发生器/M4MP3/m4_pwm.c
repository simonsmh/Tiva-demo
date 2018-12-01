////#include <stdint.h>
////#include <stdbool.h>
////#include <math.h>

////#include "inc/hw_ints.h"
////#include "inc/hw_memmap.h"
////#include "driverlib/adc.h"
////#include "driverlib/debug.h"
////#include "driverlib/fpu.h"
////#include "driverlib/gpio.h"
////#include "driverlib/interrupt.h"
////#include "driverlib/pin_map.h"
////#include "driverlib/rom.h"
////#include "driverlib/sysctl.h"
////#include "driverlib/ssi.h"
////#include "driverlib/rom.h"

////#include "driverlib/pwm.h"

////#include "m4_common.h"

////void spwm_init(void)
////{
////    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
////    
////    PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
////    
////    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/4000);
////    
////    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
////    
////    PWMGenEnable(PWM0_BASE, PWM_GEN_3);
////}

////void spwm_scanfreq(int8_t direction)
////{
////#define PWM_FREQ_LOW    1500
////#define PWM_FREQ_HI     5000
////#define PWM_FREQ_DELTA  25
////    
////    static uint32_t pwm_freq = PWM_FREQ_LOW;
////    int8_t pwm_freq_delta = PWM_FREQ_DELTA*direction;
////    
////    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true);
////    
////    for (pwm_freq += pwm_freq_delta; ((pwm_freq > PWM_FREQ_LOW) && (pwm_freq < PWM_FREQ_HI)); pwm_freq += pwm_freq_delta)
////    {
////        PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SysCtlClockGet()/pwm_freq);
////        PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/2);

////        sleep(1);
////    }
////    
////    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false);
////}

////void pwm_init(void)
////{
////    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
////    
////    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
////    
////    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, SysCtlClockGet()/100);
////    
////    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 0);
////    
////    PWMGenEnable(PWM1_BASE, PWM_GEN_3);
////    
////    PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
////}

////void pwm_set(uint8_t percent)
////{
////    uint32_t width;
////    
////    width = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)/100*percent;
////    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, width);
////}

////void pwm_set_ex(uint16_t adc_value)
////{
////    uint32_t width;

////    width = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_3)*adc_value/10000;
////    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, width);
////}
