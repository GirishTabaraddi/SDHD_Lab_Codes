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
#include "common.h"

#include "stdlib.h"

typedef enum{
    IDLE,
    SAMPLING,
    UARTTRANSFER
} Test_States_t;

Test_States_t Test_state = IDLE;

static uint8_t test_count = 0;

uint16_t sampleArray[1024] = {0};
uint16_t newDataADC[1024] = {0};

uint8_t lowByte = 0;
uint8_t highByte = 0;
char ch;
uint8_t sChRxd = 0;
uint8_t oCharRxd = 0;

char buffer[100];

CY_ISR_PROTO(isr_pushbutton);
CY_ISR_PROTO(isr_UART);

CY_ISR(isr_pushbutton)
{
    flagPB = 1;
    Test_state = SAMPLING;
    Pin_1_ClearInterrupt();
}

CY_ISR(isr_UART)
{
    ch = UART_log_GetChar();
    if('s'==ch)
    {
        sChRxd = 1;
    }
    if('o'==ch)
    {
        oCharRxd = 1;
    }
 }



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    isr_pushbutton_StartEx(isr_pushbutton);
    isr_UART_StartEx(isr_UART);
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_log_Start();
    DAC_Start();
    ADC_Start();
    ADC_StartConvert();
    
    for(;;)
    {
        /* Place your application code here. */
        switch(Test_state)
        {
            case IDLE:
            {
                test_count = 0;
                Test_state = SAMPLING;
                if(flagPB == 1)
                {
                    flagPB = 0;
                    Test_state = SAMPLING;
                }
                break;
            }
            
            case SAMPLING:
            {
                Power_LED_Write(1);
                for(int nData = 0; nData < 1024; nData++)
                {
          
                    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
                    newDataADC[nData] = (uint16_t)ADC_GetResult32();
                    //int32 ADC_result = ADC_GetResult32();
                    itoa( newDataADC[nData], buffer, 10);
                    UART_log_PutString(buffer);
                    UART_log_PutString("\n");
                }
                
                if(sChRxd)//if('s' == UART_log_GetChar())
                {
                    Test_state = UARTTRANSFER;
                    sChRxd=0;
                }
                break;
            }
            
            case UARTTRANSFER:
            {
                for(int nData = 0; nData < 1024; nData++)
                {
                
                    UART_log_PutChar(newDataADC[nData]);
                    UART_log_PutChar(newDataADC[nData]>>8);
                }
                test_count++;
             
                
                if(oCharRxd && (test_count>=10)) //if(('o' == UART_log_GetChar()) && (test_count >= 10))
                {
                    Test_state = IDLE;
                    oCharRxd=0;
                }
                else
                {
                    Test_state = SAMPLING;
                    oCharRxd=0;
                }
                break;
            }
            
            default:
              break;
        }
        
    }
}


/* [] END OF FILE */
