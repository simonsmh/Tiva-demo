#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"

#include "FatFs/ff.h"

#include "m4_common.h"

#if defined(M4_RTX)
#include <rtl.h>
#elif defined(M4_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifndef APP_GUI

#define DC_VOLTAGE          3.3
#define MAX_ADC_VALUE       0xFFF

#define MP3_FILE_NAME       "b1.wav"

typedef enum __Demo_State_
{
    S_FIRST,
    S_DEMO = S_FIRST,
    S_TEMP,
    S_ADC_POT,
    S_PWM,
    S_ADC_PHOTO,
    S_BUZZER,
    S_UART,
    S_RTC,
    S_TMR_CCP,
    S_CC1101,
    S_LAST,
} Demo_State;

typedef struct __Demo_Func_
{
    Demo_State state;
    char title[32];
    void (*func)(void);
} Demo_Func;

void demo_start     (void);
void demo_temp      (void);  
void demo_adc_pot   (void);
void demo_pwm       (void);
void demo_adc_photo (void);
void demo_buzzer    (void);
void demo_uart      (void);
void demo_rtc       (void);
void demo_tmr_count (void);
void demo_cc1101    (void);

Demo_Func dy_app[] = {
#if 1
    { S_DEMO        , " TI 大 学 计 划"
                    , demo_start    },
    { S_TEMP        , "   PID 温 控"
                    , demo_temp     },
    { S_ADC_POT     , " 电位器 ADC-DAC"
                    , demo_adc_pot  },
    { S_PWM         , "  PWM 亮度控制"
                    , demo_pwm      },
    { S_ADC_PHOTO   , "  光敏电阻 ADC"
                    , demo_adc_photo},
    { S_BUZZER      , "    蜂 鸣 器"
                    , demo_buzzer   },
    { S_UART        , "     串 口"
                    , demo_uart     },
    { S_RTC         , "    RTC 时钟"
                    , demo_rtc      },
    { S_TMR_CCP     , " 定时器按键计数"
                    , demo_tmr_count},
    { S_CC1101      , "CC1101 无线收发"
                    , demo_cc1101   }
#endif
};

Demo_State current_state;
uint32_t enable_heat = 0;

void ani(uint8_t ms)
{
#define MAX_ANI     4
    char ani_char[] = { "\\-/|" };
    static uint8_t ani_index = 0;
    
    lcd_printf(15, 3, "%c", ani_char[ani_index++]);
    if (ani_index == MAX_ANI)
    {
        ani_index = 0;
    }
    if (ms > 0)
    {
        sleep(ms);
    }
    
#ifdef M4_WDT
    // feed wdt
    wdt_feed();
#endif // M4_WDT
}

void demo_start(void)
{
    lcd_printf(0, 1, "   DY-Tiva-PB");
    lcd_printf(0, 2, "  德研电子科技");
    lcd_printf(0, 3, " www.gototi.com");
    
    while (current_state == S_DEMO)
    {
        gpio_led_runner(1);
        gpio_led_runner(-1);
    }
    gpio_led_all(0);
}


void demo_temp(void)
{
#ifdef M4_TEMP
#define MAX_MCU_TEMP_SAMPLE     20
    float mcu_temp_value = 0;
    uint8_t mcu_temp_count = 0;
		extern uint8_t Clocks_1s;
		extern long Time_zzd;
	
		//pid参数
		{
			extern int PV_PER;     //实测值
			extern float SP_INT;   //设定值
			extern int PVPER_ON;
			extern float LMN;
		}
		
			
    float degree_lm75, degree_mcu;
    
    adc_init(ADC0_BASE, ADC_CTL_TS);
		
	  timerInit_pid();
	
		PVPER_ON = 0x01;
		
		gpio_thermal_on(0x01);
    //lcd_printf(0, 3, "Press K1");
    
    while (current_state == S_TEMP)
    {
			if(Clocks_1s)  //1s测试
			{
				Clocks_1s = 0;
				degree_lm75 = temp_read();
				PV_PER = degree_lm75*10;
				
				//SP_INT = 50.0;
				lcd_printf(0, 2, "Set = %.1fC ", SP_INT);
				lcd_printf(0, 3, "Cycle Duty = %.1f",LMN);	
				lcd_printf(0, 1, "TEMP = %.1f",degree_lm75);
			}
			
			
// 				mcu_temp_value += temp_read();
//         mcu_temp_count ++;
//         
//          
//         if (mcu_temp_count == MAX_MCU_TEMP_SAMPLE)
//         {
// 						degree_lm75 = mcu_temp_value/mcu_temp_count;
// 						lcd_printf(0, 1, "TEMP = %.1f",degree_lm75);	
//             mcu_temp_value = 0;
//             mcu_temp_count = 0;
// 					
//         }
			
			
			
			  
						
			
//         degree_lm75 = temp_read();
//         
// #if 1
//         mcu_temp_value += adc_read(ADC0_BASE);
//         mcu_temp_count ++;
//         
//         degree_mcu = 147.5 - ((75*3.3*mcu_temp_value/mcu_temp_count)/4096);
//         
//         if (mcu_temp_count == MAX_MCU_TEMP_SAMPLE)
//         {
//             mcu_temp_value = 0;
//             mcu_temp_count = 0;
//         }
// #else
//         mcu_temp_value = adc_read(ADC0_BASE);
//         degree_mcu = 147.5 - ((75*3.3*mcu_temp_value)/4096);
// #endif
//         
//         lcd_printf(0, 1, "TMP75=%.4fC ", degree_lm75);
//         lcd_printf(0, 2, "MCU  =%.4fC ", degree_mcu);
//         m4_dbgprt("Temp=%.1fC\r\n", degree_lm75);
//         //sleep(100);
//         ani(100);
//         ani(100);
    }
    timedisable_pid();
    gpio_thermal_on(0x01);
		Time_zzd = 0;
		//禁止TIMER1的中断.

#endif // M4_TEMP
}

void demo_adc_pot(void)
{
#define ADC_RESOLUTION  0x1000 // 12bit
#ifdef M4_ADC
    uint32_t adc0_value, adc1_value;
    uint16_t dac_value;
    
    float adc0_volt, adc1_volt;
    
    adc_init(ADC0_BASE, ADC_CTL_CH2);
    adc_init(ADC1_BASE, ADC_CTL_CH1);
    
#if 0 // test dac beeper
    dac_value = 0;
    while (1)
    {
        dac_write(dac_value);
        dac_value += 5;
        //sleep(1);
        //adc1_value = adc_read(ADC1_BASE);

        if (dac_value >= ADC_RESOLUTION)
            dac_value = 0;
    }
#endif

    while (current_state == S_ADC_POT)
    {
        adc0_value = adc_read(ADC0_BASE);
        
        dac_value = adc0_value;
        // dac_value = 0x400;
        // dac_value = (dac_value & 0xF00) + ((dac_value & 0x0F0)>>4) + ((dac_value & 0x00F)<<4);
        dac_write(dac_value);
        
        //sleep(100);
        ani(100);
        
        adc1_value = adc_read(ADC1_BASE);
        
        adc0_volt = DC_VOLTAGE*adc0_value/MAX_ADC_VALUE;
        adc1_volt = DC_VOLTAGE*adc1_value/MAX_ADC_VALUE;

#if 0
        lcd_printf(0, 1, "0x%.4X", adc0_value);
        //lcd_printf(0, 2, "0x%.4X", adc1_value);
#else
        lcd_printf(0, 1, "ADC0=%.2fV", adc0_volt);
        lcd_printf(0, 2, "ADC1=%.2fV", adc1_volt);
        //lcd_printf(0, 3, "Delta=%.2fV ", adc1_volt - adc0_volt);    // signed
#endif
        
#ifdef M4_AC
        if (ac_get() == false)
        {
            lcd_printf(0, 3, "AC0 >1.65V");
            gpio_led_all(1);
        }
        else
        {
            lcd_printf(0, 3, "AC0 <1.65V");
            gpio_led_all(0);
        }
#endif // M4_AC
        
        //sleep(100);
        ani(100);
    }
    gpio_led_all(0);
    
#endif // M4_ADC
}

void demo_pwm(void)
{
#define PWM_MAX_PERCENT     40
#define PWM_DELTA           4
    
    int pwm_percent, pwm_delta;
    
    pwm_percent = 0;
    pwm_delta   = PWM_DELTA;
    
    while (current_state == S_PWM)
    {
        pwm_percent += pwm_delta;
        if (pwm_percent >= PWM_MAX_PERCENT)
            pwm_delta = -PWM_DELTA;
        if (pwm_percent <= 0)
        {
            pwm_delta = PWM_DELTA;
            ani(100);
        }

        pwm_set(pwm_percent);
        
        lcd_printf(0, 1, "PWM=%3d%%", pwm_percent);
        ani(50);
    }
    pwm_set(0);
}

#ifdef USE_DY_PB_FINAL
void demo_adc_photo(void)
{
    uint32_t adc0_value;
    
#ifdef USE_DY_PB_FINAL
    adc_init(ADC0_BASE, ADC_CTL_CH3);
#endif // USE_DY_PB_FINAL
    
    while (current_state == S_ADC_PHOTO)
    {
        adc0_value = adc_read(ADC0_BASE);
        
        if (adc0_value > (ADC_RESOLUTION/2))
        {
            gpio_led_all(1);
            lcd_printf(0, 2, "LED On ");
        }
        else
        {
            gpio_led_all(0);
            lcd_printf(0, 2, "LED Off");
        }
        
        lcd_printf(0, 1, "ADC0=%d   ", adc0_value);
        ani(100);
    }
    gpio_led_all(0);
}
#endif // USE_DY_PB_FINAL

void demo_buzzer(void)
{
#ifdef M4_BUZZER
//    uint16_t freq;
    
    lcd_printf(0, 1, "Beeping..");
    while (current_state == S_BUZZER)
    {
        spwm_play();
//        spwm_scanfreq(1);
//        ani(0);
//        spwm_scanfreq(-1);
//        ani(0);
    }
#endif // M4_BUZZER
}

void demo_uart(void)
{
#ifdef M4_UART
    char uart_data = 'a';
    
    while (current_state == S_UART)
    {
        lcd_printf(0, 1, "Send \"%c\"", uart_data);
        uart_send(UART1_BASE, &uart_data, sizeof(uart_data));\
        
        uart_data ++;
        if (uart_data > 'z')
            uart_data = 'a';
        
        //sleep(500);
        ani(100);ani(100);ani(100);ani(100);
    }
#endif // M4_UART
}

void demo_rtc(void)
{
    struct tm rtc_now;
    
    int rtc_last_sec;
    
    while (current_state == S_RTC)
    {
        rtc_get(&rtc_now);
        lcd_printf(0, 1, "%d-%d-%d", rtc_now.tm_year + 1900, rtc_now.tm_mon, rtc_now.tm_mday);
        lcd_printf(0, 2, "%02d:%02d:%02d", rtc_now.tm_hour, rtc_now.tm_min, rtc_now.tm_sec);
        
        if (rtc_last_sec != rtc_now.tm_sec)
        {
            gpio_led_all(1);
            rtc_last_sec = rtc_now.tm_sec;
        }
        else
        {
            gpio_led_all(0);
        }
        
        ani(100);
    }
    
    gpio_led_all(0);
}

void demo_tmr_count(void)
{
    uint32_t press_count;
    
    while (current_state == S_TMR_CCP)
    {
        press_count = timer_ccp_get();
        
        lcd_printf(0, 1, "Count=%d", press_count);
        lcd_printf(0, 2, "Press K2");
        
        ani(100);
    }
}

void demo_cc1101(void)
{
#ifdef M4_CC1101
    while (current_state == S_CC1101)
    {
        cc1101_send('a');
        lcd_printf(0, 2, "Sending..");
        ani(100);
    }
#endif
}

void uart_char_arrive(char ascii)
{
    if ((ascii >= ' ') && (ascii <= '~'))
    {
        lcd_printf(0, 2, "Got \"%c\"", ascii);
        m4_dbgprt("Got \"%c\"", ascii);
    }
}

void gpio_button_down_demo(Button button)
{
    // filter out the "unstable" status
    static uint32_t last_key_down_tick;

#ifdef M4_FREERTOS
    sys_tick_count = xTaskGetTickCountFromISR();
#endif
    m4_dbgprt("t=%u\r\n", sys_tick_count);
    
    if ((sys_tick_count - last_key_down_tick) < KEY_INTERVAL_THRESHOLD)
    {
        m4_dbgprt("Jitter\r\n");
        return ;
    }
    last_key_down_tick = sys_tick_count;
    
    switch (button)
    {
        case Key_1:
            if (current_state == S_TEMP)
            {
//                 if (enable_heat == 1)
//                 {
//                     lcd_printf(0, 3, "Heating stopped");
//                     enable_heat = 0;
//                 }
//                 else
//                 {
//                     lcd_printf(0, 3, "Heating..      ");
//                     enable_heat = 1;
//                 }
//                 gpio_thermal_on(enable_heat);
							SP_INT = SP_INT+5.0;
							lcd_printf(0, 2, "set =%.1fC ", SP_INT);
            }
            break;
        case Key_2:
            if (current_state == S_TEMP)
            {
//                 if (enable_heat == 1)
//                 {
//                     lcd_printf(0, 3, "Heating stopped");
//                     enable_heat = 0;
//                 }
//                 else
//                 {
//                     lcd_printf(0, 3, "Heating..      ");
//                     enable_heat = 1;
//                 }
//                 gpio_thermal_on(enable_heat);
							SP_INT = SP_INT-5.0;
							lcd_printf(0, 2, "set =%.1fC ", SP_INT);
            }					
            break;
        case Key_3:
            // previous
            if (current_state == S_FIRST)
                current_state = S_LAST;
            current_state --;
            break;
        case Key_4:
            // next
            current_state ++;
            if (current_state == S_LAST)
                current_state = S_FIRST;
    }

    // problem here (we're in isr)
    // TODO: it is prefered to use semaphore here in RTOS
    
}

void app_main_demo(void)
{
    current_state = S_DEMO;
#ifdef DEBUG
    current_state = S_TEMP;
#endif
    
    uart_int_register(&uart_char_arrive);
    gpio_int_register(&gpio_button_down_demo);
    
    while (1) // main function infinite loop
    {
        lcd_clear();
        lcd_scroll(true);
        lcd_printf(0xFF, 0, dy_app[current_state].title);
        
        // call proper app function
        (*dy_app[current_state].func)();
        
        m4_dbgprt("%s\r\n", dy_app[current_state].title);
        
        // delay a while
        sleep(50);
    }
}
#endif // APP_GUI
