//*****************************************************************************
//
//  Register offsets.
//
//*****************************************************************************
#define TI_LEFT_LINEIN_VC       0x00        // Left line input channel volume
                                            // control.
#define TI_RIGHT_LINEIN_VC      0x02        // Right line input channel volume
                                            // control.
#define TI_LEFT_HP_VC           0x04        // Left channel headphone volume
                                            // control.
#define TI_RIGHT_HP_VC          0x06        // Right channel headphone volume
                                            // control.
#define TI_ANALOG_AP            0x08        // Analog audio path control.
#define TI_DIGITAL_AP           0x0a        // Digital audio path control.
#define TI_POWER_DOWN           0x0c        // Power down control.
#define TI_DIGITAL_AI           0x0e        // Digital audio interface format.
#define TI_SRC                  0x10        // Sample rate control
#define TI_DIGITAL_ACTIVATE     0x12        // Digital interface activation.
#define TI_RESET                0x1e        // Reset register


//*****************************************************************************
//
//  TI_LEFT_LINEIN_VC
//
//*****************************************************************************
#define TI_LEFT_LINEIN_VC_LRS   0x100       // Simultaneous update.
#define TI_LEFT_LINEIN_VC_LIM   0x080       // Left line input mute
#define TI_LEFT_LINEIN_VC_LIV_M 0x01f       // Left line input volume control.+12db

//*****************************************************************************
//
//  TI_RIGHT_LINEIN_VC
//
//*****************************************************************************
#define TI_RIGHT_LINEIN_VC_RLS  0x100       // Simultaneous update.
#define TI_RIGHT_LINEIN_VC_RIM  0x080       // Right line input mute
#define TI_RIGHT_LINEIN_VC_RIV  0x01f       // Right line input volume control.+12db

//*****************************************************************************
//
//  TI_LEFT_HP_VC
//
//*****************************************************************************
#define TI_LEFT_HP_VC_LRS       0x100           // Simultaneous update.
#define TI_LEFT_HP_VC_LZC       0x080           // Left headphone zero cross.
#define TI_LEFT_HP_VC_LHV       0x07f           // Left headphone volume.+6db
#define TI_LEFT_HP_VC_0DB       0x079           // Left headphone volume 0db.

//*****************************************************************************
//
//  TI_RIGHT_HP_VC
//
//*****************************************************************************
#define TI_RIGHT_HP_VC_RLS      0x100           // Simultaneous update.
#define TI_RIGHT_HP_VC_RZC      0x080           // Right headphone mute
#define TI_RIGHT_HP_VC_RHV      0x07f           // Right headphone volume.+6db
#define TI_RIGHT_HP_VC_0DB      0x079           // Right headphone volume 0db.

//*****************************************************************************
//
//  TI_ANALOG_AP
//
//*****************************************************************************
#define TI_ANALOG_AP_STA        0x000           // Side Tone.-6db........
#define TI_ANALOG_AP_STE        0x020           // Side Tone enable.
#define TI_ANALOG_AP_DAC        0x010           // DAC select.
#define TI_ANALOG_AP_BYP        0x008           // Bypass.
#define TI_ANALOG_AP_INSEL      0x004           // Input select for ADC.-->microphone
#define TI_ANALOG_AP_MICM       0x002           // Microphone mute.muted
#define TI_ANALOG_AP_MICB       0x001           // Microphone boost.+20db

//*****************************************************************************
//
//  TI_DIGITAL_AP - Digital Audio Path Control
//
//*****************************************************************************
#define TI_DIGITAL_AP_DACM      0x008           // DAC soft mute
#define TI_DIGITAL_AP_DEEMP_DIS 0x000           // De-emphasis Disabled
#define TI_DIGITAL_AP_DEEMP_32K 0x002           // De-emphasis 32 kHz
#define TI_DIGITAL_AP_DEEMP_44K 0x004           // De-emphasis 44.1 kHz
#define TI_DIGITAL_AP_DEEMP_48K 0x005           // De-emphasis 48 kHz
#define TI_DIGITAL_AP_ADCHP     0x000           // ADC high-pass filter.enable.......

//*****************************************************************************
//
//  TI_POWER_DOWN - Power Down Control
//
//*****************************************************************************
#define TI_POWER_DOWN_OFF       0x080           // Device off
#define TI_POWER_DOWN_CLK       0x040           // Clock               off
#define TI_POWER_DOWN_OSC       0x020           // Oscillator          off
#define TI_POWER_DOWN_OUT       0x010           // Outputs             off
#define TI_POWER_DOWN_DAC       0x008           // DAC                 off
#define TI_POWER_DOWN_ADC       0x004           // ADC                 off
#define TI_POWER_DOWN_MIC       0x002           // Microphone input    off
#define TI_POWER_DOWN_LINE      0x001           // Line input          off

//*****************************************************************************
//
//  TI_DIGITAL_AI - Digital Audio Interface Format
//
//*****************************************************************************
#define TI_DIGITAL_AI_SLAVE     0x000           // Slave mode
#define TI_DIGITAL_AI_MASTER    0x040           // Master mode
#define TI_DIGITAL_AI_LRSWAP    0x020           // DAC left/right swap enable
#define TI_DIGITAL_AI_LRP       0x010           // DAC left/right phase Right channel on, LRCIN low
#define TI_DIGITAL_AI_IWL_16    0x000           // 16 bit data.
#define TI_DIGITAL_AI_IWL_20    0x004           // 20 bit data.
#define TI_DIGITAL_AI_IWL_24    0x008           // 24 bit data.
#define TI_DIGITAL_AI_IWL_32    0x00c           // 32 bit data.
#define TI_DIGITAL_AI_FOR_RA    0x000           // MSB first, right aligned
#define TI_DIGITAL_AI_FOR_LA    0x001           // MSB first, left aligned
#define TI_DIGITAL_AI_FOR_I2S   0x002           // I2S format, MSB first,
                                                // left aligned
#define TI_DIGITAL_AI_FOR_DSP   0x003           // DSP format

//*****************************************************************************
//
// TI_SRC - Sample Rate Control
//
//*****************************************************************************
#define TI_SRC_CLKOUT_DIV2      0x080           // Clock output divider
#define TI_SRC_CLKIN_DIV2       0x040           // Clock input divider
#define TI_SRC_SR               0x03c           // Sampling rate control
#define TI_SRC_SR_48000         0x000           // 12.288MHz MCLK 48KHz Sample
                                                // Rate.
#define TI_SRC_BOSR             0x000           // Base oversampling rate USB mode:1 = 272 fs  Normal mode:1 = 384 fs.......
#define TI_SRC_USB              0x001           // Clock mode select.
#define TI_SRC_NORMAL           0x000           // Clock mode select.

#define TI_SRC_SR_48K_8K        0x008           // ADC=8k, DAC=48k
#define TI_SRC_SR_8K_8K         0x00C           // ADC=8k, DAC=8k    BASR=0

//*****************************************************************************
//
// TI_DIGITAL_ACTIVATE - Digital Interface Activation
//
//*****************************************************************************
#define TI_DIGITAL_ACTIVATE_EN  0x001           // Activate interface

//*****************************************************************************
//
// I2C Addresses for the TI DAC.
//
//*****************************************************************************
#define TI_TLV320AIC23B_ADDR_0  0x01a   //CS=0 7bit address
#define TI_TLV320AIC23B_ADDR_1  0x01b   //CS=1 7bit address
