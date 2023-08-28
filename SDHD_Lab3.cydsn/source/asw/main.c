/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdlib.h>

#include "statemachine.h"
#include "fft_application.h"

#define SAMPLES 1024

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    uint16_t ADC_SamplesArray[SAMPLES] = {0};     // An array to store 1024 sampling values of ADC.  
    
    Hardware_Init();    // This function initializes all the hardware from top design .
    
    for(;;)
    {
        Sequencing(ADC_SamplesArray);       // Call the function that handles state machine for sampling and UART transfer.
    }
}

/* [] END OF FILE */
