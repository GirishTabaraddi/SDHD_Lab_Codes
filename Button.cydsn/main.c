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

volatile int toggle = 0;

CY_ISR_PROTO(isr_Button_RisingEdge_Interrupt);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    isr_Button_StartEx(isr_Button_RisingEdge_Interrupt);
    
    for(;;)
    {
        /* Place your application code here. */
        if( toggle == 0)
        {
            Pin_YELLOW_LED_Write(1);
            Pin_GREEN_LED_Write(1);
            Pin_RED_LED_Write(1);
        }
        else
        {
            Pin_YELLOW_LED_Write(0); 
            Pin_GREEN_LED_Write(0);
            Pin_RED_LED_Write(0);
        }
    }
}

CY_ISR(isr_Button_RisingEdge_Interrupt)
{
    Pin_Button_ClearInterrupt();
    
    if( toggle == 0)
        toggle = 1;
    else
        toggle = 0;
    
}
/* [] END OF FILE */
