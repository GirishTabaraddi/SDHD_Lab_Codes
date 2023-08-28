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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_LOG_Start();
    
    UART_LOG_PutString("Hello PSoC Board");
    
    uint8_t ch;
    
    for(;;)
    {
        ch = UART_LOG_GetChar();
        
        if( ch != 0u)
        {       
            if(ch >= 'A' && ch <= 'Z') 
            {
                ch = ch + 32;
                UART_LOG_PutChar(ch);
            }
        }
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
