#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/udma.h"
#include "driverlib/timer.h"

#include "fb58pid.h"

#define TIME_INTER   1000



extern long Time_zzd;
extern  float LMN;

uint32_t hit_time_count = 0;  //加热占空比计时

uint32_t count_1s = 0;
uint8_t Clocks_1s = 0;	 //1s flag


uint32_t count_01s  = 0;
uint32_t Clocks_01s = 0; //0.1s flag




void timerInit_pid(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	
	  TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / TIME_INTER);
	
	  IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A);
}


void timedisable_pid()
{
	TimerDisable(TIMER1_BASE, TIMER_A);
	
}



void
Timer1AIntHandler(void)
{
// 	static int index = 0;
// 	lcd_printf(0, 2, "%d",index++ );
	
	if(Time_zzd++>=2147483647)
      Time_zzd=0;
	
	count_1s++;
	if(count_1s >= 999)     //1s
	{
			count_1s = 0;
	    Clocks_1s = 1;
			pid();
	}
	
	count_01s++;
	if(count_01s >= 99)     //0.1s
	{
			count_01s = 0;
	    Clocks_01s = 1;
		
			hit_time_count++; 
			if(LMN <= hit_time_count)
			{
					gpio_thermal_on(0x01);  //HEAT_OFF
			}
			else gpio_thermal_on(0x00); //Heat_ON
			
			if(hit_time_count == 100) hit_time_count = 0;
	}
	
	
   TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

