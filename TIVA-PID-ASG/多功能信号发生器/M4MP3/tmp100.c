// copyright pxj 2013

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "hw_tmp100.h"

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"

#include "driverlib/sysctl.h"
#include "driverlib/rom_map.h"

#include "driverlib/i2c.h"

#include "tmp100.h"

#ifdef DY_I2C_SPEED_FIX
void
_I2CMasterInitExpClk(uint32_t ui32Base, uint32_t ui32I2CClk,
                    bool bFast)
{
    uint32_t ui32SCLFreq;
    uint32_t ui32TPR;

    //
    // Must enable the device before doing anything else.
    //
    I2CMasterEnable(ui32Base);

    //
    // Get the desired SCL speed.
    //
    if(bFast == true)
    {
        ui32SCLFreq = 400000;
    }
    else
    {
        ui32SCLFreq = 1000; //100000;
    }

    //
    // Compute the clock divider that achieves the fastest speed less than or
    // equal to the desired speed.  The numerator is biased to favor a larger
    // clock divider so that the resulting clock is always less than or equal
    // to the desired clock, never greater.
    //
    ui32TPR = ((ui32I2CClk + (2 * 10 * ui32SCLFreq) - 1) /
               (2 * 10 * ui32SCLFreq)) - 1;
    HWREG(ui32Base + I2C_O_MTPR) = ui32TPR;

    //
    // Check to see if this I2C peripheral is High-Speed enabled.  If yes, also
    // choose the fastest speed that is less than or equal to 3.4 Mbps.
    //
    if(HWREG(ui32Base + I2C_O_PP) & I2C_PP_HS)
    {
        ui32TPR = ((ui32I2CClk + (2 * 3 * 3400000) - 1) /
                   (2 * 3 * 3400000)) - 1;
        HWREG(ui32Base + I2C_O_MTPR) = I2C_MTPR_HS | ui32TPR;
    }
}
#endif // DY_I2C_SPEED_FIX

void TMP100Init(void)
{
#ifdef DY_I2C_SPEED_FIX
    _I2CMasterInitExpClk(TMP_PIN_I2C_PORT, SysCtlClockGet(), false);
#else
    I2CMasterInitExpClk(TMP_PIN_I2C_PORT, SysCtlClockGet(), false);
#endif
    
    TMP100ResolutionSet();
}

void TMP100ResolutionSet(void)
{
    uint8_t i2cWriteBuffer;

    uint16_t i2c_read_delay;
    
    // frame 1:
    I2CMasterSlaveAddrSet(TMP_PIN_I2C_PORT, TMP_I2C_ADDR, false);
    
    // frame 2:
    
    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_SEND_START);

    // send to tmp100:    
    i2cWriteBuffer = TMP_CONFIG_REG;
    i2cWriteBuffer &= 0x03;         // 2 bit valid
    
    I2CMasterDataPut(TMP_PIN_I2C_PORT, i2cWriteBuffer);
    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_SEND_CONT);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;
    
    // frame 4:
    i2cWriteBuffer = TMP_12;
    //i2cWriteBuffer &= 0x03;         // 2 bit valid
    
    I2CMasterDataPut(TMP_PIN_I2C_PORT, i2cWriteBuffer);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;
    
    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_SEND_FINISH);
}

uint16_t TMP100DataRead(void)
{
    uint8_t i2cWriteBuffer;
    uint32_t i2cReadBuffer[2];
    
    uint16_t temp_value;
    
    uint16_t i2c_read_delay;
    
    // send to tmp100:    
    i2cWriteBuffer = TMP_TEMP_REG;
    i2cWriteBuffer &= 0x03;         // 2 bit valid

    // frame 1:
    I2CMasterSlaveAddrSet(TMP_PIN_I2C_PORT, TMP_I2C_ADDR, false);
    
    // frame 2:
    I2CMasterDataPut(TMP_PIN_I2C_PORT, i2cWriteBuffer);
    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_SINGLE_SEND);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;
    
    // wait finish
//    while(I2CMasterBusBusy(TMP_PIN_I2C_PORT))
//        ;
    
    // read from tmp100:
    // frame 3:
    I2CMasterSlaveAddrSet(TMP_PIN_I2C_PORT, TMP_I2C_ADDR, true);
    //I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_SINGLE_RECEIVE);

    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_RECEIVE_START);

    // get temperature int value
    // frame 4:
    i2cReadBuffer[0] = I2CMasterDataGet(TMP_PIN_I2C_PORT);
    
    i2c_read_delay = TMP_READ_DELAY;
    while (i2c_read_delay--)
        ;

    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
    
    // get the decimal value
    // frame 5:
    i2cReadBuffer[1] = I2CMasterDataGet(TMP_PIN_I2C_PORT);
//     while(I2CMasterBusBusy(TMP_PIN_I2C_PORT))
//         ;
    
    I2CMasterControl(TMP_PIN_I2C_PORT, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    
    temp_value = i2cReadBuffer[0] | (i2cReadBuffer[1] << 8);
    //temp_value = i2cReadBuffer[1] | (i2cReadBuffer[0] << 8);

    return temp_value;
}
