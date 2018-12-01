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
#include "TMP75.h"



void TMP75Init(void)
{
	   
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
		    //
		    // 引脚时钟使能
		    //
		    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		    //
		    // I2C 引脚设置
		    //
		    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);	
		    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

		    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
		    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

			  //
			  // I2C1 外设使能
			  //
			  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

			  //
			  // 配置 I2C1 
			  //
			  I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
    
        TMP75ResolutionSet();
}

void TMP75ResolutionSet(void)
{
    uint8_t i2cWriteBuffer;

    uint16_t i2c_read_delay;
    
    // frame 1:
    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_I2C_ADDR, false);
    
    // frame 2:
    
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    // 发送到 tmp75:    
    i2cWriteBuffer = TMP_CONFIG_REG;
    i2cWriteBuffer &= 0x03;         // 2 bit valid
    
    I2CMasterDataPut(I2C1_BASE, i2cWriteBuffer);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;
    
    // frame 4:
    i2cWriteBuffer = TMP_12;
    
    I2CMasterDataPut(TMP_PIN_I2C_PORT, i2cWriteBuffer);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;
    
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
}

uint16_t TMP75DataRead(void)
{
    uint8_t i2cWriteBuffer;
    uint32_t i2cReadBuffer[2];
    
    uint16_t temp_value;
    
    uint16_t i2c_read_delay;
    
    // 发送到 tmp75:    
    i2cWriteBuffer = TMP_TEMP_REG;
    i2cWriteBuffer &= 0x03;         // 2 bit valid

    // frame 1:
    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_I2C_ADDR, false);
    
    // frame 2:
    I2CMasterDataPut(I2C1_BASE, i2cWriteBuffer);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--);
    

    // 从tmp75读取:
    // frame 3:
    I2CMasterSlaveAddrSet(I2C1_BASE, TMP_I2C_ADDR, true);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    // frame 4:
    i2cReadBuffer[0] = I2CMasterDataGet(I2C1_BASE);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    
    // frame 5:
    i2cReadBuffer[1] = I2CMasterDataGet(I2C1_BASE);
    
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    
    temp_value = i2cReadBuffer[0] | (i2cReadBuffer[1] << 8);

    return temp_value;
}

uint8_t TMP75AddrTest(uint8_t addr)
{
    uint32_t i2cErrorState;

    I2CMasterSlaveAddrSet(I2C1_BASE, addr, false);
    I2CMasterDataPut(I2C1_BASE, 0x00);
    
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    SysCtlDelay(500000);

    i2cErrorState = I2CMasterErr(I2C1_BASE);

    if(i2cErrorState & I2C_MASTER_ERR_ADDR_ACK)
    {
        // 没有响应
        return 0;
    }

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    return 1;
}

float temp_read(void)
{
    volatile float temp_degree;
    uint16_t temp_value;
    
    temp_value = TMP75DataRead();
    temp_degree = ((temp_value >> 8) & 0xFF) + ((float)((temp_value >> 4) & 0xF))*0.0625;

    return temp_degree;
}
