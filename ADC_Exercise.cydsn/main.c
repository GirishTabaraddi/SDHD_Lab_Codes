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
#include <stdio.h>
#include <stdlib.h>
#define VDDA 5

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    WaveDAC8_Start();
    WaveDAC8_Enable();   
    ADC_Start();
    
    for(;;)
    {
        /* Place your application code here. */
         WaveDAC8_SetRange(WaveDAC8_VDAC8_RANGE_4V);
        // WaveDAC8_SetValue(5);
        ADC_StartConvert();
        ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    
        int32 adc_result = ADC_GetResult32();
        
        int16 adc_millivolts = ADC_CountsTo_mVolts(adc_result);
    
        
        if( (adc_millivolts <= 600) )       // 0.088 * 4.7 = 0.413 V = 413 mV // R1 = 100ohm , R2 = 1k
        {
            Green_LED_Write(1);
            Yellow_LED_Write(0);
            Red_LED_Write(0);
        }
        
        else if((adc_millivolts > 600) && ( adc_millivolts <= 2700)) // 0.5 * 4.7 = 2.35 V = 2350mV //R1=R2 = 2.2k
        {
            Green_LED_Write(0);
            Yellow_LED_Write(1);
            Red_LED_Write(0);
        }
        
        else if((adc_millivolts > 2700)) // 0.911 * 4.7 = 4.281 V = 4281 mV //R1 = 10k R2 = 100k
        {
            Green_LED_Write(0);
            Yellow_LED_Write(0);
            Red_LED_Write(1);
        }
    }
}

/* [] END OF FILE */
