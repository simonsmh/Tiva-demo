#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"

#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/udma.h"

#include "dac7512.h"
#include "m4_common.h"

extern App_State current_state;

//#define DAC7512TimerIsr(void)       timer0a_int_isr(void)

//uint8_t udma_control_table[1024];

// dac7512 circular buffer management
CircularBuffer dac_buffer = { 0 };

int32_t value1,value2,value3,value4;
float num;
uint16_t fangbowave_table[2] = {0x0000,0x0fff};
uint16_t sanjiaowave_table[20] = {0x0000,0x200,0x0400,0x500,0x600,0x0800,0xa00,0xb00,0xc00,0xd00,0x0fff,0xd00,0xc00,0xb00,0xa00,0x0800,0x600,0x0400,0x300,0x0200};
uint16_t juchiwave_table[20]={0x0000,0x050,0x100,0x200,0x250,0x300,0x0400,0x500,0x600,0x700,0x800,0x850,0x900,0xa00,0xb00,0xc00,0xd00,0xd50,0xe00,0x0fff};
//uint16_t sinwave_table[]={0x800,0x0a78,0x0cb3,0x0e78,0x0f9B,0x0fff,0x0f9B,0x0e78,0x0cb3,0x0a78,0x800,0x0587,0x034c,0x0187,0x064,0x0000,0x064,0x0187,0x034c,0x0587};
uint16_t sinwave_table[]={
 0x7FF,0x863,0x8C8,0x92B,0x98E,0x9F1,0xA51,0xAB1,0xB0F,0xB6A,0xBC4,0xC1C,0xC71,0xCC3,0xD12,0xD5E
,0xDA7,0xDEC,0xE2E,0xE6C,0xEA5,0xEDB,0xF0D,0xF3A,0xF63,0xF87,0xFA6,0xFC1,0xFD7,0xFE8,0xFF5,0xFFC
,0xFFE,0xFFC,0xFF5,0xFE8,0xFD7,0xFC1,0xFA6,0xF87,0xF63,0xF3A,0xF0D,0xEDB,0xEA5,0xE6C,0xE2E,0xDEC
,0xDA7,0xD5E,0xD12,0xCC3,0xC71,0xC1C,0xBC4,0xB6A,0xB0F,0xAB1,0xA51,0x9F1,0x98E,0x92B,0x8C8,0x863
,0x7FF,0x79B,0x736,0x6D3,0x670,0x60E,0x5AD,0x54D,0x4EF,0x494,0x43A,0x3E2,0x38D,0x33B,0x2EC,0x2A0
,0x257,0x212,0x1D0,0x192,0x159,0x123,0x0F1,0x0C4,0x09B,0x077,0x058,0x03D,0x027,0x016,0x009,0x002
,0x000,0x002,0x009,0x016,0x027,0x03D,0x058,0x077,0x09B,0x0C4,0x0F1,0x123,0x159,0x192,0x1D0,0x212
,0x257,0x2A0,0x2EC,0x33B,0x38D,0x3E2,0x43A,0x494,0x4EF,0x54D,0x5AD,0x60E,0x670,0x6D3,0x736,0x79B
};
int count = 0;
int i=0;
uint32_t aa;
void timer0a_int_isr(void);
void DAC7512Init(uint32_t spiClock)
{
    // setup circular buffer
    //memset(&dac_buffer, 0, sizeof(CircularBuffer));
    
    MAP_SysCtlPeripheralEnable(DAC_PERIPH_SPI_CS);
    
    MAP_GPIOPinTypeGPIOOutput(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS);
    
    // disable sync
    MAP_GPIOPinWrite(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS, DAC_PIN_SPI_CS);
    
    MAP_SSIConfigSetExpClk(DAC_PIN_SPI_PORT, MAP_SysCtlClockGet(), SSI_FRF_MOTO_MODE_1,
        SSI_MODE_MASTER, spiClock, 16);
    MAP_SSIEnable(DAC_PIN_SPI_PORT);
    
    
#ifdef DAC_USE_DMA
    uDMAEnable();

    uDMAControlBaseSet(&udma_control_table[0]);
    
#else // DAC_USE_DMA
    // setup timer
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);



					  //TIMER_CFG_SPLIT_PAIR 
    TimerConfigure(TIMER0_BASE, TIMER_CFG_A_PERIODIC);



    TimerPrescaleSet(TIMER0_BASE, TIMER_A,0);
    //TimerControlEvent(TIMER4_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
   // TimerControlEvent(DAC_TIMER_PIN_PORT, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());  //1kHz
	IntMasterEnable();
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    IntEnable(INT_TIMER0A);
	TimerIntRegister(TIMER0_BASE,TIMER_A,timer0a_int_isr);
	TimerEnable(TIMER0_BASE, TIMER_A);
#endif // DAC_USE_DMA
}

inline void DAC7512Write(uint16_t data)
{
    uint32_t data_rcv;
    
    uint16_t ssi_read_delay;
    
    MAP_GPIOPinWrite(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS, ~DAC_PIN_SPI_CS);     // ~SYNC
    
    //data = (data >> 8) | (data << 8);
    
    // collect shit data
    while (SSIDataGetNonBlocking(DAC_PIN_SPI_PORT, &data_rcv))
       ;
    
    MAP_SSIDataPut(DAC_PIN_SPI_PORT, data);
    
    ssi_read_delay = 10;
    while (ssi_read_delay--)
        ;
    
//        // TODO: use isr handle this
//    while(MAP_SSIBusy(DAC_PIN_SPI_PORT))
//        ;
    
    // collect shit data
    while (SSIDataGetNonBlocking(DAC_PIN_SPI_PORT, &data_rcv))
       ;
    
    // TODO: use isr handle this
    //while(MAP_SSIBusy(DAC_PIN_SPI_PORT))
    //    ;
    
    MAP_GPIOPinWrite(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS, DAC_PIN_SPI_CS);      // SYNC
		
}

void DAC7512EnableTrans(void)
{
#ifdef DAC_USE_DMA
    // fire up the timer
    TimerEnable(TIMER_PIN_PORT, TIMER_PIN_PIN);
#else // DAC_USE_DMA
    
#endif // DAC_USE_DMA
}

void DAC7512DisableTrans(void)
{
    // stop the timer
    TimerDisable(DAC_TIMER_PIN_PORT, DAC_TIMER_PIN_PIN);
}

void DAC7512FillBuffer(uint16_t *addr)
{
    uint16_t    i_copy,
                copy_dac_buffer_start;      // the start copy pos of dac buffer

    uint16_t    elem_buffer;
    
    // wait for the ring buffer to be empty
    while (dac_buffer.count >= (DAC_BUFFER_LEN/2))
        ;

    // we must disable timer interrupt first before doing write operation
    IntDisable(DAC_TIMER_INT);

#if 0
    // simply copy
    memcpy(&dac_buffer.elems[(dac_buffer.start + dac_buffer.count)%(DAC_BUFFER_LEN/2)], addr, SD_BUFFER_LEN/2);
#else
    // reverse msb && lsb and copy
    copy_dac_buffer_start = (dac_buffer.start + dac_buffer.count)%(DAC_BUFFER_LEN/2);
    for (i_copy = 0; i_copy < (SD_BUFFER_LEN/2); i_copy ++)
    {
        elem_buffer = *(addr + i_copy);
        // lsb -> msb
        elem_buffer = (elem_buffer << 8) | (elem_buffer >> 8);
        elem_buffer = elem_buffer >> 4; // 12-Bit dac

        //  X       X       PD1     PD0     DATA ...
        //  15      14      13      12      11   ...
        
        // signed -> unsigned
        elem_buffer += 0x800;
        
        elem_buffer &= 0xFFF;
        
        //elem_buffer = elem_buffer >> 2;
        
        //elem_buffer = 0xFFFF;
        
        dac_buffer.elems[(copy_dac_buffer_start + i_copy)%(DAC_BUFFER_LEN/2)] = elem_buffer;
    }
#endif

    dac_buffer.count += SD_BUFFER_LEN/2;

    IntEnable(DAC_TIMER_INT);
}

//void DAC7512TimerIsr(void)
void timer0a_int_isr(void)
{
   // count ++;
    //if (count == 8000*15) while(1);
	 value1 = adc_read(ADC0_BASE);//lcd_printf(0,2,"%d",value1);
		if(value1<1365)
	{num=0.5;}	
	else if(value1<2730)
	{num=0.7;}
	else if(value1<4096)
	{num=1;}
	TimerIntClear(DAC_TIMER_PIN_PORT, TIMER_TIMA_TIMEOUT);
    switch(current_state)
	{
	case S_RECORD: //fangbo
	dac_write(fangbowave_table[i++]*num);
		i = i % 2;
		break;

	case S_UART:  //sanjiaobo
		dac_write(sanjiaowave_table[i++]*num);
		 i=i% 20;
	 	 break;
	case S_BUZZER:
		  dac_write(juchiwave_table[i++]*num);
		  i = i%20;
		  break;
    case S_AUDIO:
		dac_write(sinwave_table[i++]);
//		i = i%20;
		i=i%128;
		break;
		
	}
				

}
