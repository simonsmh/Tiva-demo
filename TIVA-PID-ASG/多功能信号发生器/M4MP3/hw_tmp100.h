// copyright pxj 2013

#ifndef __SENSORLIB_HW_TMP100_H__
#define __SENSORLIB_HW_TMP100_H__

//*****************************************************************************
//
// The following are defines for the TMP100 Register Addresses
//
//*****************************************************************************
#define TMP100_O_VOBJECT        0x00        // Raw object voltage measurement
#define TMP100_O_TAMBIENT       0x01        // Die temperature of the TMP100
#define TMP100_O_CONFIG         0x02        // TMP100 Configuration
#define TMP100_O_MFG_ID         0xFE        // TMP100 Manufacture
                                            // Identification
#define TMP100_O_DEV_ID         0xFF        // TMP100 Device Identification

//*****************************************************************************
//
// The following are defines for the bit fields in the TMP100_O_CONFIG
// register.
//
//*****************************************************************************
#define TMP100_CONFIG_RESET_M   0x8000      // TMP100 device reset
#define TMP100_CONFIG_RESET_ASSERT                                            \
                                0x8000      // Reset TMP100; self clearing
#define TMP100_CONFIG_MODE_M    0x7000      // Operation mode
#define TMP100_CONFIG_MODE_PD   0x0000      // Power down
#define TMP100_CONFIG_MODE_CONT 0x7000      // Continuous sampling
#define TMP100_CONFIG_CR_M      0x0E00      // Conversion rate setting
#define TMP100_CONFIG_CR_4      0x0000      // 4Hz conversion rate
#define TMP100_CONFIG_CR_2      0x0200      // 2Hz conversion rate
#define TMP100_CONFIG_CR_1      0x0400      // 1Hz conversion rate
#define TMP100_CONFIG_CR_0_5    0x0600      // 0.5Hz conversion rate
#define TMP100_CONFIG_CR_0_25   0x0800      // 0.25Hz conversion rate
#define TMP100_CONFIG_EN_DRDY_PIN_M                                           \
                                0x0100      // Enable the DRDY output pin
#define TMP100_CONFIG_DIS_DRDY_PIN                                            \
                                0x0000      // DRDY pin disabled
#define TMP100_CONFIG_EN_DRDY_PIN                                             \
                                0x0100      // DRDY pin enabled
#define TMP100_CONFIG_DRDY_M    0x0080      // Data ready flag
#define TMP100_CONFIG_IN_PROG   0x0000      // Conversion in progress
#define TMP100_CONFIG_DRDY      0x0080      // Conversion complete
#define TMP100_CONFIG_RESET_S   15
#define TMP100_CONFIG_MODE_S    12
#define TMP100_CONFIG_CR_S      9
#define TMP100_CONFIG_EN_DRDY_PIN_S                                           \
                                8
#define TMP100_CONFIG_DRDY_S    7

#endif // __SENSORLIB_HW_TMP100_H__
