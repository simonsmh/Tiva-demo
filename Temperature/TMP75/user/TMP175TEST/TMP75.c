/*****************************************************************************
* File Name: TMP75.c
*
* Version: 1.0
*

* Hardware Dependency:
* TMP75 Two-Wire Digital Temperature Sensor 
* (http://www.ti.com/lit/ds/symlink/tmp175.pdf)
*
*****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_i2c.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_adc.h"
#include "inc/hw_ssi.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/i2c.h"
#include "driverlib/udma.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"

#include "utils/uartstdio.h"
#include "I2C_Stellaris_API.h"
#include "TMP75.h" 			/* TMP75 library definitions */

/* Variable to store I2C Slave address */
unsigned char slaveAddr;

/* Byte Array for TMP75 I2C Registers */
unsigned char I2Cregister[3];

/* Variable to store Configuration information */
unsigned char config = 0;

/* Variable to store Resolution information */
unsigned char resolution;

/* Variable to hold temperature read from sensor */
unsigned short result;

/* Byte Array to hold LCD display string */
unsigned short lcdString2[8];


/* Update variable with slave address of device to be selected */
void TMP75_SelectSensor(unsigned char addr)
{
	slaveAddr = addr;
}

/* Update configuration variable */
void TMP75_UpdateConfig(unsigned char param, unsigned char data)
{
	/* Update the appropriate function (selected by bitmask 'param') 
	with corresponding setting ('data') */
	config |= (data << param);
	
	/* Update resolution variable */
	if (param == RESOLUTION) resolution = (data + 9);
}

/* Write configuration data to the sensor */
bool TMP75_SendConfig(void)
{
	bool status;
	
	I2Cregister[POINTER_BYTE] = POINTER_CONFIGREG;
	I2Cregister[CONFIG_BYTE] = config;
  I2CWriteData(I2C1_BASE,slaveAddr,I2Cregister,I2Cm_CompleteXfer,2);	
	
	status = (I2Cm_bWriteBytes(slaveAddr, I2Cregister, I2C_WRITE_SIZE, I2Cm_CompleteXfer));
	
	I2Cregister[POINTER_BYTE] = POINTER_TEMPREG;
	status = (I2Cm_bWriteBytes(slaveAddr, I2Cregister, I2C_WRITE_SIZE, I2Cm_CompleteXfer));
	
	return status;
}

/* Fetch temperature data using I2C Read operation and return calculated temperature value */
unsigned int TMP75_GetTempVal(void)
{
	if (I2Cm_fReadBytes(slaveAddr, (unsigned char *) I2Cregister, I2C_READ_SIZE, I2Cm_CompleteXfer))
	{
		result = (I2Cregister[TEMP_MSB_BYTE] << 8) | I2Cregister[TEMP_LSB_BYTE];
		return (TMP75_CalculateTemp(result));
	}
	else return 0;
}

/* Get sign of temperature from the readback data */
bool TMP75_GetTempSign(void)
{
	return ((result & SIGNBITMASK) >> (resolution - 1));
}

/* Calculate temperature value from the readback data */
unsigned int TMP75_CalculateTemp(unsigned short tempData)
{
	/* Multiplication Factor */
	unsigned short bMult;
	
	/* Selects Resolution * Scaling factor */
	switch(resolution)
	{
	case 9:
	default:
		/* 0.5 * 10 */
		bMult = 5;
		break;
	
	case 10:
		/* 0.25 * 100 */
		bMult = 25;
		break;
		
	case 11:
		/* 0.125 * 1000 */
		bMult = 125;
		break;
		
	case 12:
		/* 0.0625 * 10000 */
		bMult = 625;
		break;
	}
	
	/* Shift bits according to resolution */
	tempData >>= SHIFTBITS;
	
	if (TMP75_GetTempSign())
	{
		tempData = TMP75_TwosComplement(tempData);
	}

	return  ((unsigned int)bMult * (unsigned int)tempData);
}


/* Sets the low & high temperature limit to define an ALERT condition */
bool TMP75_SendAlertLimits(char tLow, char tHigh)
{
	bool status;
	
	I2Cregister[POINTER_BYTE] = POINTER_TLOWREG;
	I2Cregister[ALERT_TLOW_MSB] = tLow;
	I2Cregister[ALERT_TLOW_LSB] = 0x00;
	
	status = (I2Cm_bWriteBytes(slaveAddr, I2Cregister, I2C_WRITE_SIZE, I2Cm_CompleteXfer));
	
	I2Cregister[POINTER_BYTE] = POINTER_THIGHREG;
	I2Cregister[ALERT_THIGH_MSB] = tHigh;
	I2Cregister[ALERT_THIGH_LSB] = 0x00;
	status &= I2Cm_bWriteBytes(slaveAddr, I2Cregister, I2C_WRITE_SIZE, I2Cm_CompleteXfer);
	return status;
}

/* Perform Two's complement operation on a 12-bit number */
unsigned short TMP75_TwosComplement(unsigned short data)
{
	data ^= 0x0FFF;
	data++;
	return data;
}