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
    0x0000, 0x1F06, 0x500B, 0xC80E, 0x0010, 0xC80E, 0x500B, 0x1F06,
    0x0000, 0xE0F9, 0xAFF4, 0x39F1, 0x00F0, 0x37F1, 0xAFF4, 0xE0F9,
};



uint16_t squarewave_sample[] = {
	
			
    0x0000, 0x0000, 0x0000, 0x0000, 0xFF0F, 0xFF0F, 0xFF0F, 0xFF0F,
		0x0000, 0x0000, 0x0000, 0x0000, 0xFF0F, 0xFF0F, 0xFF0F, 0xFF0F,
   
};  

uint16_t triangularwave_sample[] = {
    0x0000, 0x0004, 0x0008, 0x000C, 0x0010, 0x000C, 0x0008, 0x0004,
    0x0000, 0x0004, 0x0008, 0x000C, 0x0010, 0x000C, 0x0008, 0x0004,
}; 


uint16_t sawtoothwave_sample[] = {
    0x0000, 0x0004, 0x0008, 0x0010, 0x0000, 0x0004, 0x0008,0x0010,
    0x0000, 0x0004, 0x0008, 0x0010, 0x0000, 0x0004, 0x0008,0x0010,
};  

void dac_init()
{
    DAC7512Init(DAC7512_RATE);
}

void dac_write(uint16_t value)
{
    DAC7512Write(value);
}



void dac_sinewave()
{
//    DAC7512EnableTrans();
//    //while (1)
//    {
//        DAC7512FillBuffer(sinwave_sample);
//    }
//		
//    DAC7512DisableTrans();
		int i;
	for(i=0;i<256;i++)
		dac_write(sin(3.1415*2*i/256)*2048+2048);
}

void dac_squarewave()
{

	
//    DAC7512EnableTrans();
//    //while (1)
//    {
//        DAC7512FillBuffer(squarewave_sample);
//    }
//		
//    DAC7512DisableTrans();
	
		int i;
		for (i = 0; i < 16; i ++)
			dac_write(squarewave_sample[i]);
}

void dac_triangularwave()
{
//    DAC7512EnableTrans();
//    //while (1)
//    {
//        DAC7512FillBuffer(triangularwave_sample);
//    }
//    DAC7512DisableTrans();
		
		int i;
		for (i = 0; i < 16; i ++)
			dac_write(triangularwave_sample[i]);
}


void dac_sawtoothwave()
{
//    DAC7512EnableTrans();
//    //while (1)
//    {
//        DAC7512FillBuffer(sawtoothwave_sample);
//    }
//    DAC7512DisableTrans();
		
		int i;
		for (i = 0; i < 16; i ++)
			dac_write(sawtoothwave_sample[i]);
}
