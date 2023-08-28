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

volatile int i = 0;

CY_ISR_PROTO(isr_Button_RisingEdge_Interrupt);

void three_bit_counter();

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    isr_Button_StartEx(isr_Button_RisingEdge_Interrupt);
    
    for(;;)
    {
        /* Place your application code here. */
        three_bit_counter();
        
    }
}

void three_bit_counter()
{    
    Pin_YELLOW_LED_Write(i & 0x01);
    Pin_RED_LED_Write((i>>1) & 0x01);
    Pin_GREEN_LED_Write((i>>2) & 0x01);
}

CY_ISR(isr_Button_RisingEdge_Interrupt)
{
    Pin_Button_ClearInterrupt();
    
    i = (i + 1) & 0x07;
}

/* [] END OF FILE */
