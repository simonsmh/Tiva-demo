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

//#define DAC7512TimerIsr(void)       timer0a_int_isr(void)

//uint8_t udma_control_table[1024];

// dac7512 circular buffer management
CircularBuffer dac_buffer = { 0 };

int count = 0;

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
    MAP_SysCtlPeripheralEnable(DAC_TIMER_PERIPH);

    TimerConfigure(DAC_TIMER_PIN_PORT, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    
    //TimerControlEvent(TIMER4_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(DAC_TIMER_PIN_PORT, TIMER_A, TIMER_EVENT_BOTH_EDGES);
    
    TimerLoadSet(DAC_TIMER_PIN_PORT, DAC_TIMER_PIN_PIN, SysCtlClockGet()/6/SAMPLE_RATE*5);
    
    TimerIntEnable(DAC_TIMER_PIN_PORT, TIMER_TIMA_TIMEOUT);

    IntEnable(DAC_TIMER_INT);
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
    
    ssi_read_delay = 10000;
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
    count ++;
    //if (count == 8000*15) while(1);
    
    TimerIntClear(DAC_TIMER_PIN_PORT, TIMER_TIMA_TIMEOUT);

#ifdef DEBUG
    if (dac_buffer.count > 0)
    {
#endif // DEBUG
#if 0
        MAP_GPIOPinWrite(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS, ~DAC_PIN_SPI_CS);     // ~SYNC
        
        MAP_SSIDataPut(DAC_PIN_SPI_PORT, dac_buffer.elems[dac_buffer.start]);
        
        // TODO: use isr handle this
        while(MAP_SSIBusy(DAC_PIN_SPI_PORT))
            ;
        
        MAP_GPIOPinWrite(DAC_GPIO_SPI_CS, DAC_PIN_SPI_CS, DAC_PIN_SPI_CS);      // SYNC
#else
        DAC7512Write(dac_buffer.elems[dac_buffer.start]);
#endif
        
        dac_buffer.start ++;
        dac_buffer.start %= (DAC_BUFFER_LEN/2);
        dac_buffer.count --;
#ifdef DEBUG
    }
#endif // DEBUG
}
