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

    UART_LOG_Start();
    
    UART_LOG_PutString("Hello PSoC Exercise 2");
    
    uint8_t ch;
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        
        ch = UART_LOG_GetChar();
        
        if(ch != 0)
        {
            UART_LOG_PutChar(toupper(ch));
            
            /* Method 2 for toupper*/
//            if(ch >= 'a' && ch <= 'z') 
//            {
//                ch = ch - 32;
//                UART_LOG_PutChar(ch);
//            }
            
            /* Method 2 for tolower*/
//            if(ch >= 'A' && ch <= 'Z') 
//            {
//                ch = ch + 32;
//                UART_LOG_PutChar(ch);
//            }
        }
    }
}

/* [] END OF FILE */
