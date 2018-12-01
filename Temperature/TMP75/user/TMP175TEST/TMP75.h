//*******************************************************************************
//* File Name: TMP75.h
//*
//* Version: 1.0
//*
//* Description:
//* 	This is header file of TMP75.c;
//*
//********************************************************************************

#ifndef _TMP75_H_
#define _TMP75_H_

/* MACRO definitions */

/* I2C slave address (default = 0x48 on CY8CKIT-036 board */
#define TMP75_ADDR			0x48

/* MACRO constants for I2C transaction size */
#define I2C_READ_SIZE		0x02
#define I2C_WRITE_SIZE		0x02

/* MACROS constants for I2C register sub-addresses */
#define POINTER_BYTE		0x00

#define TEMP_MSB_BYTE	 	0x00
#define TEMP_LSB_BYTE	 	0x01
	
#define CONFIG_BYTE	 		0x01
	
#define ALERT_TLOW_MSB 		0x01
#define ALERT_TLOW_LSB 		0x02
#define ALERT_THIGH_MSB 	0x01
#define ALERT_THIGH_LSB 	0x02

	
#define POINTER_TEMPREG 	0x00
#define POINTER_CONFIGREG 	0x01
#define POINTER_TLOWREG 	0x02
#define POINTER_THIGHREG 	0x03


/* MACRO Bitmasks for configuration register of TMP75 */
#define SHUTDOWNMODE		0x00
	#define SHUTDOWN_DIS		0x00
	#define SHUTDOWN_EN			0x01
	
#define THERMOSTATMODE		0x01
	#define COMPARATOR			0x00
	#define INTERRUPT			0x01	
	
#define POLARITY			0x02
	#define ACTIVELOW			0x00
	#define ACTIVEHIGH			0x01

#define FAULTQUEUE			0x03
	#define FAULTS_1			0x00
	#define FAULTS_2			0x01
	#define FAULTS_4			0x02
	#define FAULTS_6			0x03

#define RESOLUTION			0x05
	#define RES_9_BITS			0x00
	#define RES_10_BITS			0x01
	#define RES_11_BITS			0x02
	#define RES_12_BITS			0x03
	
#define ONESHOTMODE			0x07
	#define ONESHOT_DIS			0x00
	#define ONESHOT_EN			0x01

/* ALERT level definitions (in degrees C) */
#define ALERTLOW		-10
#define ALERTHIGH		40

/* MACROS for temperature calculation and conversion */
#define MAXTEMP		128
#define SHIFTBITS 	((12 - resolution) + 4)
#define SIGNBITMASK	(0x8000 >> (12 - resolution))
#define CALCRESULT(value)	(float) ( (MAXTEMP / FULLSCALETEMP) * value) 
#define ALERTBYTE_MSB(wTmpVar)	 ((wTmpVar & 0xF000) >>)  //((wTmpVar * 16) >> 4)
#define ALERTBYTE_LSB(wTmpVar)	 (((WORD) wTmpVar & 0x0F) << 4)


/* Function Prototypes */
void  TMP75_SelectSensor(unsigned char addr);
void  TMP75_UpdateConfig(unsigned char param, unsigned char data);
bool  TMP75_SendConfig(void);
unsigned int TMP75_GetTempVal(void);
bool  TMP75_GetTempSign(void);
unsigned int TMP75_CalculateTemp(unsigned short tempData);
bool  TMP75_SendAlertLimits(char tLow, char tHigh);
unsigned short  TMP75_TwosComplement(unsigned short data);

#endif