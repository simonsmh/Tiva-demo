//#include <stdint.h>
//#include <stdbool.h>

//#include "inc/hw_ints.h"
//#include "inc/hw_memmap.h"
//#include "driverlib/adc.h"
//#include "driverlib/comp.h"
//#include "driverlib/debug.h"
//#include "driverlib/fpu.h"
//#include "driverlib/gpio.h"
//#include "driverlib/interrupt.h"
//#include "driverlib/pin_map.h"
//#include "driverlib/rom.h"
//#include "driverlib/sysctl.h"

////*****************************************************************************
////
//// Configure ADC0 for a single-ended input and a single sample.  Once the
//// sample is ready, an interrupt flag will be set.
////
////*****************************************************************************
//void adc_init(const uint32_t base, const uint32_t channel)
//{
//    //
//    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
//    // will do a single sample when the processor sends a signal to start the
//    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
//    // to sequence 3.  This example is arbitrarily using sequence 3.
//    //
//    ADCSequenceConfigure(base, 3, ADC_TRIGGER_PROCESSOR, 0);
//    //ADCSequenceConfigure(base, 3, ADC_TRIGGER_ALWAYS, 0);
//    
//    //
//    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
//    // single-ended mode (default) and configure the interrupt flag
//    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
//    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
//    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
//    // sequence 0 has 8 programmable steps.  Since we are only doing a single
//    // conversion using sequence 3 we will only configure step 0.  For more
//    // information on the ADC sequences and steps, reference the datasheet.
//    //
//    //ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
//    ADCSequenceStepConfigure(base, 3, 0, channel | ADC_CTL_IE | ADC_CTL_END);

//    //
//    // Since sample sequence 3 is now configured, it must be enabled.
//    //
//    ADCSequenceEnable(base, 3);

//    //
//    // Clear the interrupt status flag.  This is done to make sure the
//    // interrupt flag is cleared before we sample.
//    //
//    ADCIntClear(base, 3);
//}

//uint32_t adc_read(const uint32_t base)
//{
//    //
//    // This array is used for storing the data read from the ADC FIFO. It
//    // must be as large as the FIFO for the sequencer in use.  This example
//    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
//    // was used with a deeper FIFO, then the array size must be changed.
//    //
//    uint32_t pui32ADC0Value[8];     // a maximum of 8 fifo (less than 8 would cause memory corrupt)
//    
//    //
//    // Trigger the ADC conversion.
//    //
//    ADCProcessorTrigger(base, 3);

//    //
//    // Wait for conversion to be completed.
//    //
//    while(!ADCIntStatus(base, 3, false))
//    {
//    }

//    //
//    // Clear the ADC interrupt flag.
//    //
//    ADCIntClear(base, 3);

//    //
//    // Read ADC Value.
//    //
//    ADCSequenceDataGet(base, 3, pui32ADC0Value);
//    
//    return pui32ADC0Value[0];
//}

//void ac_init()
//{
//    ComparatorRefSet(COMP_BASE, COMP_REF_1_65V);
//    
//    ComparatorConfigure(COMP_BASE, 0, (COMP_TRIG_NONE | COMP_INT_BOTH | COMP_ASRCP_REF | COMP_OUTPUT_NORMAL));
//}

//bool ac_get()
//{
//    return ComparatorValueGet(COMP_BASE, 0);
//}
