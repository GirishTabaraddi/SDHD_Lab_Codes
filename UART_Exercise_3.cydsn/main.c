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
#include "stdlib.h"

#define METHOD_1_Traingular_Wave    0
#define METHOD_2_Traingular_Wave    0
#define METHOD_1_Saw_Tooth_Wave     0
#define METHOD_2_Saw_Tooth_Wave     0
#define METHOD_1_Step_Signal        0
#define METHOD_1_Square_Signal      0

#define METHOD_chatgpt 0

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_LOG_Start();
    
    //UART_LOG_PutString("Hello PSoC Exercise 3");  // UART Debug statement to check if it is started
    
    uint16_t waveform_array[256] = {0}; // An 256 size array to store the values required to plot waveforms.
    
    
#ifdef METHOD_1_Traingular_Wave
   
//    for(uint16_t i = 0; i < 129; i++)
//    {
//        waveform_array[i] = i;
//              
//    }
//    for(uint16_t i = 129; i < 255; i++)
//    {
//        waveform_array[i] = 255 - i;  
//    }
#endif
    
#ifdef METHOD_2_Traingular_Wave
  
//    for(uint16_t i = 0; i < 256; i++)
//    {
//        waveform_array[i] = i;
//        
//        if(i > 129)
//        {
//            waveform_array[i] = 255 - i;
//        }
//              
//    } 
#endif

#ifdef METHOD_1_Saw_Tooth_Wave
   
//    for(uint16_t i = 0; i < 129; i++)
//    {
//        waveform_array[i] = i;
//              
//    }
//    for(uint16_t i = 1; i < 127; i++)
//    {
//        waveform_array[128 + i] = 128+i;  
//    }
#endif

#ifdef METHOD_2_Saw_Tooth_Wave

//    uint16_t interval = 64; // The interval at which to repeat the saw tooth waveform pattern
//    
//    for (uint16_t i = 0; i < 256; i += interval)
//    {
//        for (uint16_t j = 0; j < interval; j++)
//        {
//            waveform_array[i + j] = j;
//            waveform_array[i + interval + j] = interval - j - 1;
//        }
//    }
#endif
    
#ifdef METHOD_1_Step_Signal
    
//    uint16_t interval = 64; // The interval at which to place the steps
//    
//    for (uint16_t i = 0; i < 256; i += interval)
//    {
//        for (uint16_t j = 0; j < interval; j++)
//        {
//            waveform_array[i + j] = i;
//        }
//    }
#endif

#ifdef METHOD_1_Square_Signal
    
//    uint16_t period = 128; // The period of the square wave
//    uint16_t interval = 64; // Half of the period
//    
//    // Generate the square wave with a period of 128 samples
//    for (uint16_t i = 0; i < 256; i++)
//    {
//        if ((i % period) < interval)
//        {
//            waveform_array[i] = 255;
//        }
//        else
//        {
//            waveform_array[i] = 0;
//        }
//    }
#endif
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        if(UART_LOG_GetChar() == 's')
        {
            for(uint16_t i = 0; i < 256; i++)
            {
                UART_LOG_PutChar(waveform_array[i]);
            }
        }
    }
}

/* [] END OF FILE */
