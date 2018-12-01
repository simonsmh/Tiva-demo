// usb msc middleware
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"
#include "driverlib/usb.h"
#include "driverlib/gpio.h"
#include "driverlib/udma.h"
#include "driverlib/pin_map.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdmsc.h"

#include "fatfs/diskio.h"

#include "usb_msc_structs.h"

#include "m4_common.h"

//*****************************************************************************
//
// The number of ticks to wait before falling back to the idle state.  Since
// the tick rate is 100Hz this is approximately 3 seconds.
//
//*****************************************************************************
#define USBMSC_ACTIVITY_TIMEOUT 300

//*****************************************************************************
//
// This enumeration holds the various states that the device can be in during
// normal operation.
//
//*****************************************************************************
volatile enum
{
    //
    // Unconfigured.
    //
    MSC_DEV_DISCONNECTED,

    //
    // Connected but not yet fully enumerated.
    //
    MSC_DEV_CONNECTED,

    //
    // Connected and fully enumerated but not currently handling a command.
    //
    MSC_DEV_IDLE,

    //
    // Currently reading the SD card.
    //
    MSC_DEV_READ,

    //
    // Currently writing the SD card.
    //
    MSC_DEV_WRITE,
}
g_eMSCState;

//*****************************************************************************
//
// The Flags that handle updates to the status area to avoid drawing when no
// updates are required..
//
//*****************************************************************************
#define FLAG_UPDATE_STATUS      1
static uint32_t g_ui32Flags;
static uint32_t g_ui32IdleTimeout;

//******************************************************************************
//
// The DMA control structure table.
//
//******************************************************************************
#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable sDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(sDMAControlTable, 1024)
tDMAControlTable sDMAControlTable[64];
#else
tDMAControlTable sDMAControlTable[64] __attribute__ ((aligned(1024)));
#endif

extern void disk_timerproc (void);

//*****************************************************************************
//
// Handles bulk driver notifications related to the receive channel (data from
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the event we are being notified about.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the receive data channel (the OUT channel carrying
// data from the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
uint32_t
RxHandler(void *pvCBData, uint_fast32_t ui32Event,
               uint_fast32_t ui32MsgValue, void *pvMsgData)
{
    return(0);
}

//*****************************************************************************
//
// Handles bulk driver notifications related to the transmit channel (data to
// the USB host).
//
// \param pvCBData is the client-supplied callback pointer for this channel.
// \param ui32Event identifies the event we are being notified about.
// \param ui32MsgValue is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the bulk driver to notify us of any events
// related to operation of the transmit data channel (the IN channel carrying
// data to the USB host).
//
// \return The return value is event-specific.
//
//*****************************************************************************
uint32_t
TxHandler(void *pvCBData, uint_fast32_t ui32Event, uint_fast32_t ui32MsgValue,
          void *pvMsgData)
{
    return(0);
}

//*****************************************************************************
//
// This function is the call back notification function provided to the USB
// library's mass storage class.
//
//*****************************************************************************
uint32_t
USBDMSCEventCallback(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgParam,
                     void *pvMsgData)
{
    //
    // Reset the time out every time an event occurs.
    //
    g_ui32IdleTimeout = USBMSC_ACTIVITY_TIMEOUT;

    switch(ui32Event)
    {
        //
        // Writing to the device.
        //
        case USBD_MSC_EVENT_WRITING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_WRITE)
            {
                //
                // Go to the write state.
                //
                g_eMSCState = MSC_DEV_WRITE;

                //
                // Cause the main loop to update the screen.
                //
                g_ui32Flags |= FLAG_UPDATE_STATUS;
            }

            break;
        }

        //
        // Reading from the device.
        //
        case USBD_MSC_EVENT_READING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_READ)
            {
                //
                // Go to the read state.
                //
                g_eMSCState = MSC_DEV_READ;

                //
                // Cause the main loop to update the screen.
                //
                g_ui32Flags |= FLAG_UPDATE_STATUS;
            }

            break;
        }
        //
        // The USB host has disconnected from the device.
        //
        case USB_EVENT_DISCONNECTED:
        {
            //
            // Go to the disconnected state.
            //
            g_eMSCState = MSC_DEV_DISCONNECTED;

            //
            // Cause the main loop to update the screen.
            //
            g_ui32Flags |= FLAG_UPDATE_STATUS;

            break;
        }
        //
        // The USB host has connected to the device.
        //
        case USB_EVENT_CONNECTED:
        {
            g_eMSCState = MSC_DEV_CONNECTED;
            g_ui32Flags |= FLAG_UPDATE_STATUS;

            break;
        }
        case USBD_MSC_EVENT_IDLE:
        {
            //
            // Go to the idle state to wait for read/writes.
            //
            g_eMSCState = MSC_DEV_IDLE;
            g_ui32Flags |= FLAG_UPDATE_STATUS;
            
            break;
        }
        default:
        {
            break;
        }
    }

    return(0);
}

void usb_msc_timeout_cb(void)
{
    //
    // Call the FatFs tick timer.
    //
    disk_timerproc();

    if(g_ui32IdleTimeout != 0)
    {
        g_ui32IdleTimeout--;
    }
}

void usb_msc_init(void)
{
    volatile uint_fast32_t ui32Retcode;
    
    ROM_uDMAControlBaseSet(&sDMAControlTable[0]);
    ROM_uDMAEnable();
    
    //
    // Initialize the idle timeout and reset all flags.
    //
    g_ui32IdleTimeout = 0;
    g_ui32Flags = 0;
    
    //
    // Initialize the state to idle.
    //
    g_eMSCState = MSC_DEV_DISCONNECTED;
    
    //
    // Draw the status bar and set it to idle.
    //
    m4_dbgprt("Disconnected\r\n");
    
    //
    // Enable the USB controller.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    
    //
    // Set the USB pins to be controlled by the USB controller.
    //
    // WARNING: MUST CONFIGURE AHB FOR USB MSC ON TM4C123GH6PM
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlGPIOAHBEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_AHB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    //
    // Pass our device information to the USB library and place the device
    // on the bus.
    //
    USBDMSCInit(0, &g_sMSCDevice);
    
    //
    // Determine whether or not an SDCard is installed.  If not, print a
    // warning and have the user install one and restart.
    //
    ui32Retcode = disk_initialize(0);

}

int usb_msc_stat(void)
{
    int update_status;
    update_status = (g_ui32Flags & FLAG_UPDATE_STATUS);
    
    switch(g_eMSCState)
    {
        case MSC_DEV_READ:
        {
            //
            // Update the screen if necessary.
            //
            if(g_ui32Flags & FLAG_UPDATE_STATUS)
            {
                m4_dbgprt("Reading\r\n");
                g_ui32Flags &= ~FLAG_UPDATE_STATUS;
            }

            //
            // If there is no activity then return to the idle state.
            //
            if(g_ui32IdleTimeout == 0)
            {
                g_ui32Flags &= FLAG_UPDATE_STATUS;
                g_eMSCState = MSC_DEV_IDLE;
            }

            break;
        }
        case MSC_DEV_WRITE:
        {
            //
            // Update the screen if necessary.
            //
            if(g_ui32Flags & FLAG_UPDATE_STATUS)
            {
                m4_dbgprt("Writing\r\n");
                g_ui32Flags &= ~FLAG_UPDATE_STATUS;
            }

            //
            // If there is no activity then return to the idle state.
            //
            if(g_ui32IdleTimeout == 0)
            {
                g_ui32Flags &= FLAG_UPDATE_STATUS;
                g_eMSCState = MSC_DEV_IDLE;
            }
            break;
        }
        case MSC_DEV_DISCONNECTED:
        {
            //
            // Update the screen if necessary.
            //

            if(g_ui32Flags & FLAG_UPDATE_STATUS)
            {
                m4_dbgprt("Disconnected\r\n");
                g_ui32Flags &= ~FLAG_UPDATE_STATUS;
            }
            break;
        }
        case MSC_DEV_CONNECTED:
        {
            if(g_ui32Flags & FLAG_UPDATE_STATUS)
            {
                m4_dbgprt("Connected\r\n");
                g_ui32Flags &= ~FLAG_UPDATE_STATUS;
            }
            break;
        }
        case MSC_DEV_IDLE:
        {
            if(g_ui32Flags & FLAG_UPDATE_STATUS)
            {
                m4_dbgprt("Idle\r\n");
                g_ui32Flags &= ~FLAG_UPDATE_STATUS;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    
    if (update_status > 0)
    {
        return g_eMSCState;
    }
    else
    {
        return -1;
    }
}
