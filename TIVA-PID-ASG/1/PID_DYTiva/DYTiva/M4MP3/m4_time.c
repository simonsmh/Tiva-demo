#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "m4_config.h"
#include "m4_common.h"

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/hibernate.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/watchdog.h"

#include "utils/ustdlib.h"

void rtc_init(void)
{
    uint32_t rtc_tick;
    
    struct tm rtc_time;
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    
    HibernateEnableExpClk(SysCtlClockGet());
    
    HibernateRTCEnable();
    
    // current time:
    rtc_time.tm_year    = 114;
    rtc_time.tm_mon     = 7;
    rtc_time.tm_mday    = 21;
    rtc_time.tm_hour    = 1;
    rtc_time.tm_min     = 0;
    rtc_time.tm_sec     = 0;
    
    rtc_tick = umktime(&rtc_time);
    HibernateRTCSet(rtc_tick);
    
    memset(&rtc_time, 0, sizeof(struct tm));
    
}

void rtc_get(struct tm *rtc_time)
{
    uint32_t rtc_tick;
    
    rtc_tick = HibernateRTCGet();
    ulocaltime(rtc_tick, rtc_time);
}

void timer_ccp_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    
    GPIOPinTypeTimer(BUTTON_GPIO_K2, BUTTON_PIN_K2);
    GPIOPinConfigure(GPIO_PF4_T2CCP0);
    
    GPIOPadConfigSet(BUTTON_GPIO_K2, BUTTON_PIN_K2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    
    TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT_UP);
    TimerControlEvent(TIMER2_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerLoadSet(TIMER2_BASE, TIMER_A, 0xFF);
    
    TimerEnable(TIMER2_BASE, TIMER_A);
}

uint32_t timer_ccp_get()
{
    uint32_t press_count;
    
    press_count = TimerValueGet(TIMER2_BASE, TIMER_A);
    
    return press_count;
}

void wdt_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    if(WatchdogLockState(WATCHDOG0_BASE) == true)
    {
        WatchdogUnlock(WATCHDOG0_BASE);
    }
    
    WatchdogStallEnable(WATCHDOG0_BASE);
    WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet()*WDT_TIMEOUT);
    WatchdogResetEnable(WATCHDOG0_BASE);
    WatchdogEnable(WATCHDOG0_BASE);
    WatchdogLock(WATCHDOG0_BASE);
}

void wdt_feed(void)
{
    WatchdogUnlock(WATCHDOG0_BASE);
    WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet()*WDT_TIMEOUT);
    WatchdogLock(WATCHDOG0_BASE);
}
