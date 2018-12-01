////*****************************************************************************
////
////  Example demonstrating use of sound.c for I2S emulation
////              via dual-SPI. 
////
//// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
//// Software License Agreement
//// 
//// Texas Instruments (TI) is supplying this software for use solely and
//// exclusively on TI's microcontroller products. The software is owned by
//// TI and/or its suppliers, and is protected under applicable copyright
//// laws. You may not combine this software with "viral" open-source
//// software in order to form a larger program.
//// 
//// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
//// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
//// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
//// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
//// DAMAGES, FOR ANY REASON WHATSOEVER.
//// 
//// This is part of revision 8049 of the EK-LM4F232 Firmware Package.
////
////*****************************************************************************
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <string.h>

//#include "inc/hw_memmap.h"
//#include "inc/hw_types.h"
//#include "inc/hw_timer.h"
//#include "inc/hw_ints.h"
//#include "inc/hw_gpio.h"
//#include "inc/hw_i2c.h"
//#include "inc/hw_sysctl.h"
//#include "driverlib/timer.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/sysctl.h"
//#include "driverlib/systick.h"
//#include "driverlib/gpio.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/ssi.h"
//#include "driverlib/i2c.h"
//#include "driverlib/udma.h"
//#include "driverlib/fpu.h"
//#include "driverlib/rom.h"
//#include "utils/uartstdio.h"
//#include "utils/ustdlib.h"

//#include "sound.h"
//#include "hw_tlv320aic23b.h"
//#include "tlv320aic23b.h"
//#include "fatfs/ff.h"
//#include "fatfs/diskio.h"

////*****************************************************************************
////
//// Storage for the names of the files in the current directory.  Filenames
//// are stored in format "0:/filename.ext".
////
////*****************************************************************************
//#define NUM_LIST_STRINGS 48
//#define MAX_FILENAME_STRING_LEN (3 + 8 + 1 + 3 + 1)
//char g_pcFilenames[NUM_LIST_STRINGS][MAX_FILENAME_STRING_LEN];

////*****************************************************************************
////
//// The following are data structures used by FatFs.
////
////*****************************************************************************
//static FATFS g_sFatFs;
//static FILINFO g_sFileInfo;
//static unsigned long g_ulWavCount;
//static DIR g_sDirObject;

////*****************************************************************************
////
//// A structure that holds a mapping between an FRESULT numerical code, and a
//// string representation.  FRESULT codes are returned from the FatFs FAT file
//// system driver.
////
////*****************************************************************************
//typedef struct
//{
//    FRESULT iFResult;
//    char *pcResultStr;
//}
//tFResultString;

////*****************************************************************************
////
//// A macro to make it easy to add result codes to the table.
////
////*****************************************************************************
//#define FRESULT_ENTRY(f)        { (f), (#f) }

////*****************************************************************************
////
//// A table that holds a mapping between the numerical FRESULT code and it's
//// name as a string.  This is used for looking up error codes for printing to
//// the console.
////
////*****************************************************************************
//tFResultString g_psFResultStrings[] =
//{
//    FRESULT_ENTRY(FR_OK),
//    FRESULT_ENTRY(FR_DISK_ERR),
//    FRESULT_ENTRY(FR_INT_ERR),
//    FRESULT_ENTRY(FR_NOT_READY),
//    FRESULT_ENTRY(FR_NO_FILE),
//    FRESULT_ENTRY(FR_NO_PATH),
//    FRESULT_ENTRY(FR_INVALID_NAME),
//    FRESULT_ENTRY(FR_DENIED),
//    FRESULT_ENTRY(FR_EXIST),
//    FRESULT_ENTRY(FR_INVALID_OBJECT),
//    FRESULT_ENTRY(FR_WRITE_PROTECTED),
//    FRESULT_ENTRY(FR_INVALID_DRIVE),
//    FRESULT_ENTRY(FR_NOT_ENABLED),
//    FRESULT_ENTRY(FR_NO_FILESYSTEM),
//    FRESULT_ENTRY(FR_MKFS_ABORTED),
//    FRESULT_ENTRY(FR_TIMEOUT),
//    FRESULT_ENTRY(FR_LOCKED),
//    FRESULT_ENTRY(FR_NOT_ENOUGH_CORE),
//    FRESULT_ENTRY(FR_TOO_MANY_OPEN_FILES),
//    FRESULT_ENTRY(FR_INVALID_PARAMETER),
//};

////*****************************************************************************
////
//// A macro that holds the number of result codes.
////
////*****************************************************************************
//#define NUM_FRESULT_CODES       (sizeof(g_psFResultStrings) /                 \
//                                 sizeof(tFResultString))

////*****************************************************************************
////
//// This function returns a string representation of an error code that was
//// returned from a function call to FatFs.  It can be used for printing human
//// readable error messages.
////
////*****************************************************************************
//const char *
//StringFromFResult(FRESULT iFResult)
//{
//    uint_fast8_t ui8Idx;

//    //
//    // Enter a loop to search the error code table for a matching error code.
//    //
//    for(ui8Idx = 0; ui8Idx < NUM_FRESULT_CODES; ui8Idx++)
//    {
//        //
//        // If a match is found, then return the string name of the error code.
//        //
//        if(g_psFResultStrings[ui8Idx].iFResult == iFResult)
//        {
//            return(g_psFResultStrings[ui8Idx].pcResultStr);
//        }
//    }

//    //
//    // At this point no matching code was found, so return a string indicating
//    // an unknown error.
//    //
//    return("UNKNOWN ERROR CODE");
//}

////*****************************************************************************
////
//// A macro that holds the number of result codes.
////
////*****************************************************************************
//#define NUM_FRESULT_CODES       (sizeof(g_psFResultStrings) /                 \
//                                 sizeof(tFResultString))

////*****************************************************************************
////
//// Header information parsed from an opened .wav file.
////
////*****************************************************************************
//tWaveHeader g_sWaveHeader;

////*****************************************************************************
////
//// Initialize the volume to 50%.
////
////*****************************************************************************
//unsigned long g_DACVolume = 20;

////*****************************************************************************
////
//// Initialize index to array of .wav files.
////
////*****************************************************************************
//unsigned long g_ulWaveIndex = 0;

////*****************************************************************************
////
//// The error routine that is called if the driver library encounters an error.
////
////*****************************************************************************
//#ifdef DEBUG
//void
//__error__(char *pcFilename, unsigned long ulLine)
//{
//}
//#endif

////*****************************************************************************
////
//// Configure the UART and its pins.  This must be called before UARTprintf().
////
////*****************************************************************************
//void
//ConfigureUART(void)
//{
//    //
//    // Enable the GPIO Peripheral used by the UART.
//    //
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//    //
//    // Enable UART0
//    //
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

//    //
//    // Configure GPIO Pins for UART mode.
//    //
//    GPIOPinConfigure(GPIO_PA0_U0RX);
//    GPIOPinConfigure(GPIO_PA1_U0TX);
//    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

//    //
//    // Initialize the UART for console I/O.
//    //
//    UARTStdioConfig(0, 115200, SysCtlClockGet());
//}

////*****************************************************************************
////
//// Defines the possible states for the audio player.
////
////*****************************************************************************
//typedef enum
//{
//    STATE_STOPPED,
//    STATE_PLAYING,
//} tSoundState;

////*****************************************************************************
////
//// Initialize state machine to stopped.
////
////*****************************************************************************
//tSoundState g_sSoundState = STATE_STOPPED;

////*****************************************************************************
////
//// This function is called to read the contents of the root directory on
//// a given FAT logical drive and fill the listbox containing the names of all
//// audio WAV files found.
////
////*****************************************************************************
//static int
//FindWaveFilesOnDrive(const char *pcDrive, int iStartIndex)
//{
//    FRESULT fresult;
//    int iCount;

//    //
//    // Open the current directory for access.
//    //
//    fresult = f_opendir(&g_sDirObject, pcDrive);

//    //
//    // Check for error and return if there is a problem.
//    //
//    if(fresult != FR_OK)
//    {
//        //
//        // Ensure that the error is reported.
//        //
//        return(0);
//    }

//    //
//    // Start by inserting at the next entry in the list box.
//    //
//    iCount = iStartIndex;

//    //
//    // Enter loop to enumerate through all directory entries.
//    //
//    while(1)
//    {
//        //
//        // Read an entry from the directory.
//        //
//        fresult = f_readdir(&g_sDirObject, &g_sFileInfo);

//        //
//        // Check for error and return if there is a problem.
//        //
//        if(fresult != FR_OK)
//        {
//            return(0);
//        }

//        //
//        // If the file name is blank, then this is the end of the
//        // listing.
//        //
//        if(!g_sFileInfo.fname[0])
//        {
//            break;
//        }

//        //
//        // Add the information as a line in the listbox widget if there is
//        // space left and the filename contains ".wav".
//        //
//        if((g_ulWavCount < NUM_LIST_STRINGS) &&
//           ((ustrstr(g_sFileInfo.fname, ".wav")) ||
//            (ustrstr(g_sFileInfo.fname, ".WAV"))))
//        {
//            //
//            // Ignore directories.
//            //
//            if((g_sFileInfo.fattrib & AM_DIR) == 0)
//            {
//                usnprintf(g_pcFilenames[iCount], MAX_FILENAME_STRING_LEN,
//                          "%s", g_sFileInfo.fname);
//                // If folder name is needed.
//                //usnprintf(g_pcFilenames[iCount], MAX_FILENAME_STRING_LEN,
//                //          "%s%s", pcDrive, g_sFileInfo.fname);

//                //
//                // Move on to the next entry in the list box.
//                //
//                iCount++;
//            }
//        }
//    }

//    //
//    // Made it to here, return the number of files we found.
//    //
//    return(iCount);
//}

////*****************************************************************************
////
//// This function is called to read the contents of the current directory on
//// the SD card and fill the listbox containing the names of all files.
////
////*****************************************************************************
//static int
//PopulateFileList()
//{
//    //
//    // How many files can we find on the SD card (if present)?
//    //
//    g_ulWavCount = (unsigned long)FindWaveFilesOnDrive("0:/", 0);

//    //
//    // Did we find any files at all?
//    //
//    return(g_ulWavCount ? 0 : FR_NO_FILE);
//}




////*****************************************************************************
////
//// Interrupt handler for the Timer 0 subtimer A.  This handler will refresh 
//// the on-screen playback time and procession bar.
////
////*****************************************************************************
//void
//Timer0AIntHandler(void)
//{
//    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
////    updatePlaybackTime();
//}

////*****************************************************************************
////
//// The main application.  It configures the board and enters a state machine
//// waiting for the user to select an audio file. It then uses the sound.c API
//// for audio playback.
////
////*****************************************************************************
//int
//main(void)
//{
////    int nStatus;
//    FRESULT iFResult;

////	unsigned int i;
//    //
//    // Enable lazy stacking for interrupt handlers.  This allows floating-point
//    // instructions to be used within interrupt handlers, but at the expense of
//    // extra stack usage.
//    //
//    FPULazyStackingEnable();

//    //
//    // Set the system clock to run at 50MHz from the PLL.
//    //
//    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
//                       SYSCTL_XTAL_16MHZ);

//    //
//    // Enable the peripherals used by this example.
//    //
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

//    //
//    // Configure SysTick for a 100Hz interrupt.  The FatFs driver wants a 10 ms
//    // tick.
//    //
//    SysTickPeriodSet(SysCtlClockGet() / 100);
//    SysTickEnable();
//    SysTickIntEnable();

//    //
//    // Enable Interrupts
//    //
//    IntMasterEnable();

//    //
//    // Initialize the UART as a console for text I/O.
//    //
//    ConfigureUART();


//    //
//    // Print hello message to user.
//    //
//    UARTprintf("\n\nSD Card Example Program\n");

//    //
//    // Mount the file system, using logical disk 0.
//    //
//    iFResult = f_mount(&g_sFatFs,"0:",1);
//    if(iFResult != FR_OK)
//    {
//        UARTprintf("f_mount error: %s\n", StringFromFResult(iFResult));
// //       return(1);
//    }
//		else
//		{
//		    UARTprintf("f_mount OK: %s\n", StringFromFResult(iFResult));
//		
//		}
//		

//    //
//    // Initialize the sound driver
//    //
//    SoundInit();

//    //
//    // Get all the .wav filenames from the SD card,
//    // and place them into a list.
//    //
//    PopulateFileList();

//    //
//    // Get the initial volume.
//    //
//    g_DACVolume = SoundVolumeGet();

//    SoundPlay();
//		
//    while(1)
//    {

//    }
//}


//*****************************************************************************
//
// sd_card.c - Example program for reading files from an SD card.
//
// Copyright (c) 2011-2014 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 2.1.0.12573 of the EK-LM4F232 Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/hw_memmap.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "utils/cmdline.h"
#include "utils/uartstdio.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"

#include "sound.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>SD card using FAT file system (sd_card)</h1>
//!
//! This example application demonstrates reading a file system from an SD
//! card.  It makes use of FatFs, a FAT file system driver.  It provides a
//! simple command console via a serial port for issuing commands to view and
//! navigate the file system on the SD card.
//!
//! The first UART, which is connected to the USB debug virtual serial port on
//! the evaluation board, is configured for 115,200 bits per second, and 8-N-1
//! mode.  When the program is started a message will be printed to the
//! terminal.  Type ``help'' for command help.
//!
//! For additional details about FatFs, see the following site:
//! http://elm-chan.org/fsw/ff/00index_e.html
//
//*****************************************************************************

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;
static DIR g_sDirObject;
static FILINFO g_sFileInfo;
static FIL g_sFileObject;

//*****************************************************************************
//
// A structure that holds a mapping between an FRESULT numerical code, and a
// string representation.  FRESULT codes are returned from the FatFs FAT file
// system driver.
//
//*****************************************************************************
typedef struct
{
    FRESULT iFResult;
    char *pcResultStr;
}
tFResultString;

//*****************************************************************************
//
// A macro to make it easy to add result codes to the table.
//
//*****************************************************************************
#define FRESULT_ENTRY(f)        { (f), (#f) }

//*****************************************************************************
//
// A table that holds a mapping between the numerical FRESULT code and it's
// name as a string.  This is used for looking up error codes for printing to
// the console.
//
//*****************************************************************************
tFResultString g_psFResultStrings[] =
{
    FRESULT_ENTRY(FR_OK),
    FRESULT_ENTRY(FR_DISK_ERR),
    FRESULT_ENTRY(FR_INT_ERR),
    FRESULT_ENTRY(FR_NOT_READY),
    FRESULT_ENTRY(FR_NO_FILE),
    FRESULT_ENTRY(FR_NO_PATH),
    FRESULT_ENTRY(FR_INVALID_NAME),
    FRESULT_ENTRY(FR_DENIED),
    FRESULT_ENTRY(FR_EXIST),
    FRESULT_ENTRY(FR_INVALID_OBJECT),
    FRESULT_ENTRY(FR_WRITE_PROTECTED),
    FRESULT_ENTRY(FR_INVALID_DRIVE),
    FRESULT_ENTRY(FR_NOT_ENABLED),
    FRESULT_ENTRY(FR_NO_FILESYSTEM),
    FRESULT_ENTRY(FR_MKFS_ABORTED),
    FRESULT_ENTRY(FR_TIMEOUT),
    FRESULT_ENTRY(FR_LOCKED),
    FRESULT_ENTRY(FR_NOT_ENOUGH_CORE),
    FRESULT_ENTRY(FR_TOO_MANY_OPEN_FILES),
    FRESULT_ENTRY(FR_INVALID_PARAMETER),
};

//*****************************************************************************
//
// A macro that holds the number of result codes.
//
//*****************************************************************************
#define NUM_FRESULT_CODES       (sizeof(g_psFResultStrings) /                 \
                                 sizeof(tFResultString))

//*****************************************************************************
//
// This function returns a string representation of an error code that was
// returned from a function call to FatFs.  It can be used for printing human
// readable error messages.
//
//*****************************************************************************
const char *
StringFromFResult(FRESULT iFResult)
{
    uint_fast8_t ui8Idx;

    //
    // Enter a loop to search the error code table for a matching error code.
    //
    for(ui8Idx = 0; ui8Idx < NUM_FRESULT_CODES; ui8Idx++)
    {
        //
        // If a match is found, then return the string name of the error code.
        //
        if(g_psFResultStrings[ui8Idx].iFResult == iFResult)
        {
            return(g_psFResultStrings[ui8Idx].pcResultStr);
        }
    }

    //
    // At this point no matching code was found, so return a string indicating
    // an unknown error.
    //
    return("UNKNOWN ERROR CODE");
}

//*****************************************************************************
//
// A macro that holds the number of result codes.
//
//*****************************************************************************
#define NUM_FRESULT_CODES       (sizeof(g_psFResultStrings) /                 \
                                 sizeof(tFResultString))

//*****************************************************************************
//
// This is the handler for this SysTick interrupt.  FatFs requires a timer tick
// every 10 ms for internal timing purposes.
//
//*****************************************************************************
void
SysTickHandler(void)
{
    //
    // Call the FatFs tick timer.
    //
//   disk_timerproc();
}


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, SysCtlClockGet());
}

//*****************************************************************************
//
// The program main function.  It performs initialization, then runs a command
// processing loop to read commands from the console.
//
//*****************************************************************************
int
main(void)
{
//    int nStatus;
    FRESULT iFResult;

	uint32_t bw;
	unsigned char buff[100];
//	unsigned int i;
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    FPULazyStackingEnable();

    //
    // Set the system clock to run at 50MHz from the PLL.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    //
    // Configure SysTick for a 100Hz interrupt.  The FatFs driver wants a 10 ms
    // tick.
    //
    SysTickPeriodSet(SysCtlClockGet() / 100);
    SysTickEnable();
    SysTickIntEnable();

    //
    // Enable Interrupts
    //
    IntMasterEnable();

    //
    // Initialize the UART as a console for text I/O.
    //
    ConfigureUART();

    SoundInit();
    //
    // Print hello message to user.
    //
    UARTprintf("\n\nSD Card Example Program\n");

    //
    // Mount the file system, using logical disk 0.
    //
    iFResult = f_mount(&g_sFatFs,"0:",1);
    if(iFResult != FR_OK)
    {
        UARTprintf("f_mount error: %s\n", StringFromFResult(iFResult));
 //       return(1);
    }
		else
		{
		    UARTprintf("f_mount OK: %s\n", StringFromFResult(iFResult));
		
		}
		
////?????
//        UARTprintf("write file test......\n\r");
//        iFResult = f_open(&g_sFileObject, "0:/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
//        if(iFResult != FR_OK){
//                UARTprintf("open file error : %s\n\r",StringFromFResult(iFResult));
//        }else{
//                iFResult = f_write(&g_sFileObject, buff, sizeof(buff), &bw);               /* Write it to the dst file */
//                if(iFResult == FR_OK){
//                        UARTprintf("write data ok! %s\n\r",bw);
//                }else{
//                        UARTprintf("write data error : %s\n\r",StringFromFResult(iFResult));
//                }
//                /*close file */
//                f_close(&g_sFileObject);
//        }

//?????
        UARTprintf("read file test......\n\r");
        iFResult = f_open(&g_sFileObject, "0:/hello.txt", FA_OPEN_EXISTING | FA_READ);
        if(iFResult != FR_OK){
                UARTprintf("open file error : %s\n\r",StringFromFResult(iFResult));
        }else{
                iFResult = f_read(&g_sFileObject, buff, sizeof(buff), &bw);     /* Read a chunk of src file */
                if(iFResult==FR_OK){
                        UARTprintf("read data num : %d\n\r",bw);
                        UARTprintf("%s\n\r",buff);
                }else{
                        UARTprintf("read file error : %s\n\r",StringFromFResult(iFResult));
                }
                /*close file */
                f_close(&g_sFileObject);
        }		
		
//iFResult = f_open(&g_sFileObject, "test.txt", FA_READ);
//  
//        UARTprintf("\nType the file content.\n");
//        for (;;) {
//                iFResult = f_read(&g_sFileObject, buff, sizeof(buff), &br);        /* ???? */
//                if (iFResult || !br) break;                                    /* Error or end of file */
//                for (i = 0; i < br; i++) 
//                       UARTprintf("%c",buff);//????
//        }

//        UARTprintf("\nClose the file.\n");
//        iFResult = f_close(&g_sFileObject);
//				UARTprintf("%s\n", StringFromFResult(iFResult));


//        UARTprintf("\nCreate a new file (hello.txt).\n");
//        iFResult = f_open(&g_sFileObject, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
//				UARTprintf("%s\n", StringFromFResult(iFResult));


//        UARTprintf("\nWrite a text data. (Hello world!)\n");
//        iFResult = f_write(&g_sFileObject, "Hello world!\r\n", 14, &br);
//				UARTprintf("%s\n", StringFromFResult(iFResult));

//        UARTprintf("%u bytes written.\n", br);

//        UARTprintf("\nClose the file.\n");
//        iFResult = f_close(&g_sFileObject);
//				UARTprintf("%s\n", StringFromFResult(iFResult));

  
  while (1);

}




