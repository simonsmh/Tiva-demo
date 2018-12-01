//*****************************************************************************
//
// tlv320aic23b.c - Driver for the TI TLV320AIC23B DAC
//
// Copyright (c) 2009-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the DK-LM3S9D96 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "tlv320aic23b.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//
// The I2C pins that are used by this application.
//
//*****************************************************************************
#define AIC_I2C_PERIPH              (SYSCTL_PERIPH_I2C1)
#define AIC_I2C_MASTER_BASE         (I2C1_BASE) // (I2C0_MASTER_BASE)

#define AIC_I2CSCL_GPIO_PERIPH      (SYSCTL_PERIPH_GPIOA)
#define AIC_I2CSCL_GPIO_PORT        (GPIO_PORTA_BASE)
#define AIC_I2CSCL_PIN              (GPIO_PIN_6)

#define AIC_I2CSDA_GPIO_PERIPH      (SYSCTL_PERIPH_GPIOA)
#define AIC_I2CSDA_GPIO_PORT        (GPIO_PORTA_BASE)
#define AIC_I2CSDA_PIN              (GPIO_PIN_7)


//*****************************************************************************
//
// Global Volumes are needed because the device is not readable.
//
//*****************************************************************************
static unsigned char g_ucHPVolume = 100;
static unsigned char g_ucEnabled = 0;

//*****************************************************************************
//
// This is the volume control settings table to use to scale the dB settings
// to a 0-100% scale.  There are 13 entries because 100/8 scaling is 12.5 steps
// which requires 13 entries.
//
//*****************************************************************************
static const unsigned char pucVolumeTable[13] =
{
     0x00,
     0x30,
     0x38,
     0x40,
     0x48,
     0x50,
     0x58,
     0x60,
     0x64,
     0x68,
     0x70,
     0x74,
     0x79, // TI_LEFT_HP_VC_0DB,
};

//*****************************************************************************
//
// Write a register in the TLV320AIC23B DAC.
//
// \param ucRegister is the offset to the register to write.
// \param ulData is the data to be written to the DAC register.
//
// This function will write the register passed in /e ucAddr with the value
// passed in to /e ulData.  The data in \e ulData is actually 9 bits and the
// value in /e ucAddr is interpreted as 7 bits.
//
// \return Returns \b true on success or \b false on error.
//
//*****************************************************************************
bool
TLV320AIC23BWriteRegister(unsigned char ucRegister, unsigned long ulData)
{
    //
    // Set the slave address.
    //
    I2CMasterSlaveAddrSet(AIC_I2C_MASTER_BASE, TI_TLV320AIC23B_ADDR_0, false);

    //
    // Write the next byte to the controller.
    //
    I2CMasterDataPut(AIC_I2C_MASTER_BASE, ucRegister | ((ulData >> 8) & 1));

    //
    // Continue the transfer.
    //
    I2CMasterControl(AIC_I2C_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);

#if 1
    while(I2CMasterBusy(AIC_I2C_MASTER_BASE))
        ;
#else
    //
    // Wait until the current byte has been transferred.
    //
    while(I2CMasterIntStatus(AIC_I2C_MASTER_BASE, false) == 0)
    {
    }
#endif

    if(I2CMasterErr(AIC_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE)
    {
        I2CMasterIntClear(AIC_I2C_MASTER_BASE);
        return(false);
    }

    //
    // Wait until the current byte has been transferred.
    //
    while(I2CMasterIntStatus(AIC_I2C_MASTER_BASE, false))
    {
        I2CMasterIntClear(AIC_I2C_MASTER_BASE);
    }

    //
    // Write the next byte to the controller.
    //
    I2CMasterDataPut(AIC_I2C_MASTER_BASE, ulData);

    //
    // End the transfer.
    //
    I2CMasterControl(AIC_I2C_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    //
    // Wait until the current byte has been transferred.
    //
    while(I2CMasterIntStatus(AIC_I2C_MASTER_BASE, false) == 0)
    {
    }

    if(I2CMasterErr(AIC_I2C_MASTER_BASE) != I2C_MASTER_ERR_NONE)
    {
        return(false);
    }

    while(I2CMasterIntStatus(AIC_I2C_MASTER_BASE, false))
    {
        I2CMasterIntClear(AIC_I2C_MASTER_BASE);
    }

    return(true);
}

//*****************************************************************************
//
// Initialize the TLV320AIC23B Control interface. and DAC
//
// This function initializes the I2C interface 
//
// \return Returns \b true on success of \b false if the I2S daughter board
// is not present.
//
//*****************************************************************************
bool
TLV320AIC23BInit(void)
{
    bool bRetcode;

    //
    // Enable the GPIO port containing the I2C pins and set the SDA pin as a
    // GPIO input for now and engage a weak pull-down.  If the daughter board
    // is present, the pull-up on the board should easily overwhelm
    // the pull-down and we should read the line state as high.
    //
    SysCtlPeripheralEnable(AIC_I2C_PERIPH);
	
    GPIOPinTypeGPIOInput(AIC_I2CSCL_GPIO_PORT, AIC_I2CSDA_PIN);
    GPIOPadConfigSet(AIC_I2CSCL_GPIO_PORT, AIC_I2CSDA_PIN, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);

    //
    // Enable the I2C peripheral.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Delay a while to ensure that we read a stable value from the SDA
    // GPIO pin.  If we read too quickly, the result is unpredictable.
    // This delay is around 2mS.
    //
    SysCtlDelay(SysCtlClockGet() / (3*500));

    //
    // Read the current state of the I2C1SDA line.  If it is low, the
    // daughter board must not be present since it should be pulled high.
    //
    if(!(GPIOPinRead(AIC_I2CSCL_GPIO_PORT, AIC_I2CSDA_PIN) & AIC_I2CSDA_PIN))
    {
        return(false);
    }

    //
    // Configure the I2C SCL and SDA pins for I2C operation.
    //
    GPIOPinTypeI2CSCL(AIC_I2CSCL_GPIO_PORT, AIC_I2CSCL_PIN);
    GPIOPinTypeI2C(AIC_I2CSCL_GPIO_PORT, AIC_I2CSDA_PIN);

    //
    // Initialize the I2C master.
    //
    //I2CMasterInitExpClk(AIC_I2C_MASTER_BASE, SysCtlClockGet(), false);

    //
    // Allow the rest of the public APIs to make hardware changes.
    //
    g_ucEnabled = 1;

    //
    // Reset the DAC.  Check the return code on this call since we use it to
    // indicate whether or not the DAC is present.  If the register write
    // fails, we assume the I2S daughter board and DAC are not present and
    // return false.
    //
    bRetcode = TLV320AIC23BWriteRegister(TI_RESET, 0);
    if(!bRetcode)
    {
        return(bRetcode);
    }

    //
    // Power up the device and the DAC.
    //
    TLV320AIC23BWriteRegister(TI_POWER_DOWN, TI_POWER_DOWN_ADC | TI_POWER_DOWN_LINE);

    //
    // Set the sample rate.
    //
    //TLV320AIC23BWriteRegister(TI_SRC, TI_SRC_SR_48000);
    TLV320AIC23BWriteRegister(TI_SRC, TI_SRC_USB | TI_SRC_SR_48K_8K);

    //
    // Unmute the DAC.
    //
    TLV320AIC23BWriteRegister(TI_DIGITAL_AP, TI_DIGITAL_AP_DEEMP_48K |
                                            TI_DIGITAL_AP_ADCHP);

    //
    // Enable the DAC path and insure the Mic input stays muted.
    //
    TLV320AIC23BWriteRegister(TI_ANALOG_AP, TI_ANALOG_AP_DAC |
                                            TI_ANALOG_AP_INSEL);

    //
    // 16 bit I2S slave mode.
    //
    TLV320AIC23BWriteRegister(TI_DIGITAL_AI, TI_DIGITAL_AI_LRP | TI_DIGITAL_AI_IWL_16 |
                                            TI_DIGITAL_AI_FOR_LA | TI_DIGITAL_AI_MASTER);
    //
    // Set the Headphone volume.
    //
    TLV320AIC23BHeadPhoneVolumeSet(50);

    //
    // Unmute the Line input to the ADC.
    //
    //TLV320AIC23BLineInVolumeSet(TLV_LINEIN_VC_0DB);

    //
    // Turn on the digital interface.
    //
    TLV320AIC23BWriteRegister(TI_DIGITAL_ACTIVATE, TI_DIGITAL_ACTIVATE_EN);

    return(true);
}

//*****************************************************************************
//
// Sets the Line In volume.
//
// \param ucVolume is the volume to set for the line input.
//
// This function adjusts the audio output up by the specified percentage.  The
// TI_LEFT_LINEIN_* values should be used for the \e ucVolume parameter.
//
// \return None
//
//*****************************************************************************
void
TLV320AIC23BLineInVolumeSet(unsigned char ucVolume)
{
    //
    // Unmute the line inputs and set the mixer to 0db.
    //
    TLV320AIC23BWriteRegister(TI_LEFT_LINEIN_VC, ucVolume);
    TLV320AIC23BWriteRegister(TI_RIGHT_LINEIN_VC, ucVolume);
}

//*****************************************************************************
//
// Sets the Headphone volume on the DAC.
//
// \param ulVolume is the volume to set, specified as a percentage between 0%
// (silence) and 100% (full volume), inclusive.
//
// This function adjusts the audio output up by the specified percentage.  The
// adjusted volume will not go above 100% (full volume).
//
// \return None
//
//*****************************************************************************
void
TLV320AIC23BHeadPhoneVolumeSet(unsigned long ulVolume)
{
    g_ucHPVolume = (unsigned char)ulVolume;

    //
    // Cap the volume at 100%
    //
    if(g_ucHPVolume >= 100)
    {
        g_ucHPVolume = 100;
    }

    if(g_ucEnabled == 1)
    {
        //
        // Set the left and right volumes with zero cross detect.
        //
        TLV320AIC23BWriteRegister(TI_LEFT_HP_VC,
                                  (TI_LEFT_HP_VC_LZC |
                                   pucVolumeTable[ulVolume >> 3]));
        TLV320AIC23BWriteRegister(TI_RIGHT_HP_VC,
                                  (TI_LEFT_HP_VC_LZC |
                                   pucVolumeTable[ulVolume >> 3]));
    }
}

//*****************************************************************************
//
// Returns the Headphone volume on the DAC.
//
// This function returns the current volume, specified as a percentage between
// 0% (silence) and 100% (full volume), inclusive.
//
// \return Returns the current volume.
//
//*****************************************************************************
unsigned long
TLV320AIC23BHeadPhoneVolumeGet(void)
{
    return(g_ucHPVolume);
}

void TLV320AIC23BDisable(void)
{
    // we just init the codec, and switch it off
    TLV320AIC23BInit();
    TLV320AIC23BWriteRegister(TI_DIGITAL_ACTIVATE, 0);
    TLV320AIC23BWriteRegister(TI_POWER_DOWN, TI_POWER_DOWN_OFF);
}
