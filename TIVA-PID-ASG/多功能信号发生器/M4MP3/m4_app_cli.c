#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/adc.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"

#include "FatFs/ff.h"

#include "m4_common.h"

#if defined(M4_RTX)
#include <rtl.h>
#elif defined(M4_FREERTOS)
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifndef USE_GUI

#define DC_VOLTAGE          3.3
#define MAX_ADC_VALUE       0xFFF

#define MP3_FILE_NAME       "b1.wav"

#define KEY_INTERVAL_THRESHOLD      100     // ms, eliminate jitter


unsigned char fangbobuff1[]={0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0xFC,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x04,
							0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00};

unsigned char fangbobuff2[]={0x04,0x04,0x04,0xFC,0x00,0x00,0x00,0x00,0x00,0xFC,0x04,0x04,0x04,0x04,0x04,0xFC,
							 0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF};

unsigned char fangbobuff3[]={0x00,0x0F,0x00,0x00,0x00,0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char fangbobuff4[]= {0x00,0x00,0x00,0x0F,0x08,0x08,0x08,0x08,0x08,0x0F,0x00,0x00,0x00,0x00,0x00,0x0F,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char sanjiaobuff1[]={0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
							  0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x04};

unsigned char sanjiaobuff2[]={0x00,0x00,0x80,0x40,0x20,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							  0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x08,0x04};

unsigned char juchibuff1[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0xF8,0x00,0x00,0x00,
							0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0xFF,0x40,0x20,0x10};

unsigned char juchibuff2[]={0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							0x08,0x04,0x02,0x01,0x00,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//unsigned char sinbuff1[]={};

//unsigned char sinbuff2[]={};
unsigned char sinbuff1[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xE0,0x30,0x0C,0x06,0x03,0x01,
						  0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x70,0x0E,0x01,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char sinbuff2[]={0x01,0x03,0x06,0x18,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
						 0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char sinbuff3[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x38,0xE0,0x80,0x00,0x00,0x00,0x00,
						  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,0x18,0x60,0xC0};

unsigned char sinbuff4[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x70,0x0E,0x03,0x00,0x00,0x00,0x00,0x00,
						 0x80,0x80,0xC0,0x60,0x30,0x0C,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} ;





//typedef enum __App_State_
//{
//    S_FIRST,
//    S_BUZZER = S_FIRST,
//    S_UART,
//    S_RECORD,
//    S_AUDIO,
//    S_LAST,
//} App_State;

typedef struct __App_Func_
{
    App_State state;
    char title[32];
    void (*func)(void);
} App_Func;

void app_demo       (void);
void app_temp       (void);  
void app_adc_pot    (void);
void app_adc_photo  (void);
void app_sawtoothwave   (void);
void app_triangularwave (void);
void app_squarewave (void);
void app_sinewave   (void);

App_Func dy_app[] = {
#if 1
    { S_BUZZER      , " 当前为锯齿波"
                    , app_sawtoothwave    },
    { S_UART        , " 当前为三角波"
                    , app_triangularwave      },
    { S_RECORD      , " 当前为方波"
                    , app_squarewave    },
    { S_AUDIO       , " 当前为正弦波"
                    , app_sinewave     },
#endif
};

App_State current_state;

uint32_t adc_read(const uint32_t base)
{
    //
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
    uint32_t pui32ADC0Value[8];     // a maximum of 8 fifo (less than 8 would cause memory corrupt)
    
    //
    // Trigger the ADC conversion.
    //
    ADCProcessorTrigger(base, 3);

    //
    // Wait for conversion to be completed.
    //
    while(!ADCIntStatus(base, 3, false))
    {
    }

    //
    // Clear the ADC interrupt flag.
    //
    ADCIntClear(base, 3);

    //
    // Read ADC Value.
    //
    ADCSequenceDataGet(base, 3, pui32ADC0Value);
    
    return pui32ADC0Value[0];
}


void ani(uint8_t ms)
{
#define MAX_ANI     4
    char ani[] = { "\\-/|" };
    static uint8_t ani_index = 0;
    
    lcd_printf(15, 3, "%c", ani[ani_index++]);
    if (ani_index == MAX_ANI)
    {
        ani_index = 0;
    }
    if (ms > 0)
    {
        sleep(ms);
    }
}
void app_sawtoothwave(void)
{
	UC1701FontDisplay(2,0,16,16,juchibuff1);
	UC1701FontDisplay(2,2,16,16,juchibuff2);
//	UC1701FontDisplay(3,0,16,16,juchibuff3);
//	UC1701FontDisplay(3,2,16,16,juchibuff4);
	
	
	lcd_printf(6,6,"频率=1KHz");
//	adc()
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/20000);
    while (current_state == S_BUZZER)
    {
				//	dac_sawtoothwave();
    }
}

void app_triangularwave(void)
{
  	UC1701FontDisplay(2,0,16,16,sanjiaobuff1);
	UC1701FontDisplay(2,2,16,16,sanjiaobuff2);
	
	
//	value2=;
//		if(value2<=1365)
//	{num=0.5;}	
//	else(value2<=2730)
//	{num=0.7;}
//	else(value2<=4096)
//	{num=1;}
	lcd_printf(6,6,"频率=1KHz");
 	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/20000);
    while (current_state == S_UART)
    {
				//	dac_triangularwave();
	}
}

void app_squarewave(void)
{
	UC1701FontDisplay(2, 0, 16, 16,fangbobuff1);
   	UC1701FontDisplay(2, 2, 16, 16,fangbobuff2);
	UC1701FontDisplay(3, 0, 16, 16,fangbobuff3);
	UC1701FontDisplay(3, 2, 16, 16,fangbobuff4);
	lcd_printf(6,6,"频率=1KHz");
	
//	value3=;
//			if(value3<=1365)
//	{num=0.5;}	
//	else(value3<=2730)
//	{num=0.7;}
//	else(value3<=4096)
//	{num=1;}
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2000);
    while (current_state == S_RECORD)
    {
			//	dac_squarewave(); 
    }
}

void app_sinewave(void)
{
	 UC1701FontDisplay(2,0,16,16,sinbuff1);
	 UC1701FontDisplay(2,2,16,16,sinbuff2);
	 UC1701FontDisplay(3,2,16,16,sinbuff3);
	 UC1701FontDisplay(3,4,16,16,sinbuff4);
	
//	value4=;
//			if(value4<=1365)
//	{num=0.5;}	
//	else(value4<=2730)
//	{num=0.7;}
//	else(value4<=4096)
//	{num=1;}
	 lcd_printf(6,6,"频率=500Hz");
	 //TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/10000);
//	TimerLoadSet(TIMER0_BASE, TIMER_A, 625);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 62);
    while (current_state == S_AUDIO)
    {

			//	dac_sinewave();
       
    }
}

void uart_char_arrive(char ascii)
{
    if ((ascii >= ' ') && (ascii <= '~'))
    {
        lcd_printf(0, 2, "Got \"%c\"", ascii);
        m4_dbgprt("Got \"%c\"", ascii);
    }
}

void gpio_button_down(Button button)
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
            //break;
        case Key_2:
            // previous
            if (current_state == S_FIRST)
                current_state = S_LAST;
            current_state --;
            break;
        case Key_3:
            //break;
        case Key_4:
            // next
            current_state ++;
            if (current_state == S_LAST)
                current_state = S_FIRST;
    }
    
}
void adc_init(const uint32_t base, const uint32_t channel)
{
    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(base, 3, ADC_TRIGGER_PROCESSOR, 0);
    //ADCSequenceConfigure(base, 3, ADC_TRIGGER_ALWAYS, 0);
    
    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // information on the ADC sequences and steps, reference the datasheet.
    //
    //ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceStepConfigure(base, 3, 0, channel | ADC_CTL_IE | ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(base, 3);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntClear(base, 3);
}
void app_main_cli(void)
{
    current_state = S_BUZZER;
#ifdef DEBUG
    //current_state = S_TEMP;
#endif
    
    uart_int_register(&uart_char_arrive);
    gpio_int_register(&gpio_button_down);
    
	  adc_init(ADC0_BASE, ADC_CTL_CH2);
	
    while (1) 
    {
        lcd_clear();
        lcd_scroll(true);
        lcd_printf(0xFF, 0, dy_app[current_state].title);
        
        
        (*dy_app[current_state].func)();
        
        m4_dbgprt("%s\r\n", dy_app[current_state].title);
        
        // delay
        sleep(50);
    }
}
#endif // USE_GUI
