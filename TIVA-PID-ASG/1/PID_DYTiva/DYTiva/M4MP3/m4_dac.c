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

#include "m4_common.h"

#include "dac7512.h"

#define AMPITUDE            4096
#define SAMPLERATE          48000
#define FREQUENCY           250     // Hz
#define PI                  3.14159265358979323846

uint16_t sinwave_sample[] = {
    0x0000, 0x1F06, 0x500B, 0xC80E, 0x0010, 0xC80E, 0x510B, 0x1F06,
    0x0000, 0xE0F9, 0xAFF4, 0x39F1, 0x00F0, 0x37F1, 0xAFF4, 0xE0F9,
};  // complement code

uint16_t squarewave_sample[] = {
    //0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F,
    //0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFF7F, 0xFF7F, 0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    //0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF, 0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF,
    //0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF, 0xFF7F, 0xFF7F, 0xFFFF, 0xFFFF,
    //0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF,
    //0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF, 0xFF7F, 0xFFFF,
};  // complement code

void dac_init()
{
    DAC7512Init(DAC7512_RATE);
}

void dac_write(uint16_t value)
{
    DAC7512Write(value);
}

void dac_play(char *file_name)
{
    uint16_t wave_freq = 0;

    FIL file;
    FRESULT file_result;

    uint8_t sd_data[SD_BUFFER_LEN];
    uint32_t sd_data_len;

    int pos;

    file_result = f_open(&file, file_name, FA_READ);

    if (file_result == FR_OK)
    {
        // escape wave file header
        file_result = f_lseek(&file, 43);

        // enable mcu>>>dac transfer
        DAC7512EnableTrans();

        while (1)
        { // continuously fetch data from file
            f_read(&file, sd_data, SD_BUFFER_LEN, &sd_data_len);

            DAC7512FillBuffer((uint16_t *)&sd_data);
        }

        // disable transfer
        DAC7512DisableTrans();

        f_close(&file);
    }
}

// generate a "freq" sine wave
void dac_sinwave(uint16_t freq)
{
    float x, sample_count, theta;
    
    uint16_t delay;

    uint16_t data;
    uint32_t sys_ctl_clock, sample_delay;

    sys_ctl_clock = SysCtlClockGet();
    sample_delay = (sys_ctl_clock/6)/SAMPLERATE; //(sys_ctl_clock/(1000*6))*ms;
    sample_count = SAMPLERATE/freq; // freq;
    
    for (x = 0; x < sample_count; x ++)
    {
        theta = x/sample_count;
        data = (AMPITUDE/2 - 1)*sin(2*PI*theta) + AMPITUDE/2;
        //m4_dbgprt("sin=%d\r\n", data);
        
        // delay for 1/freq sec
        //SysCtlDelay(sample_delay);
        delay = sample_delay;
        //while (delay --)
        //    ;
        
        DAC7512Write(data);
    }
}

void dac_sinewave()
{
    DAC7512EnableTrans();
    while (1)
    {
        DAC7512FillBuffer(sinwave_sample);
    }
    DAC7512DisableTrans();
}

void dac_squarewave()
{
    DAC7512EnableTrans();
    while (1)
    {
        DAC7512FillBuffer(squarewave_sample);
    }
    DAC7512DisableTrans();
}
