/**
* \file statemachine
* \author <author-name>
* \date <date>
*
* \brief <Symbolic File name>
*
* \copyright Copyright ©2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "statemachine.h"
#include "stdlib.h"
/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

volatile uint8_t push_button_toggle = 0;        // A toggle variable to store the status of isr_PushButton.

uint8_t sChRx = 0;                              // sChRx is a flag that stores the status of 's' character sent from Matlab or Termite.
uint8_t oChRx = 0;                              // oChRx is a flag that stores the status of 'o' character sent from Matlab or Termite.
char rxDataCh;                                  // rxdataCh receives the character sent from Matlab or Termite.

uint8_t count = 0;                              // A count variable to increment till 10.

// Creating an object of my enum type for handling the different states.
// Initializing the object with IDLE_STATE at first.
myStateMachine_t State = IDLE_STATE;

int32 FFT_SamplesArray[2 * 1024] = {0}; // An array which will contain the result of the FFT calculation

/**
 * Function that initializes all the Hardware used in the top design.
 * @param: None
 * @return: None
 */
void Hardware_Init()
{
    UART_LOG_Start();
    UART_LOG_PutString("UART Started\n");
    //WaveDAC_Start();
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();
    
    isr_PushButton_StartEx(isr_PushButton_Enable_Interrupt);
    isr_UART_StartEx(isr_UART);
}


/**
 * An interrupt function that stores 1 in push button toggle variable if PushButton is pressed.
 */
CY_ISR(isr_PushButton_Enable_Interrupt)
{
    Push_Button_ClearInterrupt();
    
    if(push_button_toggle == 0)
    {
        push_button_toggle = 1;
    }
}


/**
 * An interrupt function from UART that stores 's' or 'o' in their respective flag variables,
 * when the characters are sent from Matlab or Termite.
 */
CY_ISR(isr_UART)
{
    isr_UART_ClearPending();
    
    rxDataCh = UART_LOG_GetChar();
    
    if(rxDataCh == 's')
    {
       //UART_LOG_PutString("S is sent \n");        //Debug statement added while testing UART with Termite.
        sChRx = 1;
    }
    if(rxDataCh == 'o')
    {
        oChRx = 1;
    }
}


void Sequencing(uint16_t *SamplingArray)
{   
    switch(State)
    {
        case IDLE_STATE:
            
            Yellow_LED_Write(1);             // Added as a debug LED, turns on when state is in IDLE.
            Green_LED_Write(0);
            Red_LED_Write(0);
            
            //Initializing count
            count =  0;
            
            if(push_button_toggle == 1)
            {   
                // If push button is pressed then change the state to SAMPLING state.  
                State = SAMPLING_STATE;
                push_button_toggle = 0;
            }
        break;
        
        case SAMPLING_STATE:

            Yellow_LED_Write(0);            //Added as a debug LED; turns off when state changes from IDLE to SAMPLING.
            Green_LED_Write(1);             
            Red_LED_Write(0);
           
            for(uint16_t idx = 0; idx < 1024; idx++)
            {
                //Sample the voltage divider output and store it in an Array.
                ADC_DelSig_IsEndConversion(ADC_DelSig_WAIT_FOR_RESULT);
                SamplingArray[idx] = (uint16_t)ADC_DelSig_GetResult32();
            }
            
            //If 's' is recieved from MATLAB, then change the state to UART TRANSFER state.
            if(sChRx)
            {
                // UART_LOG_PutString("I received S \n");        // Debug statement added when testing the statemachine through termite.
                User_LED_Write(1);                              // Added as a debug pin, turns on when 's' is received from Matlab or termite.
                State = UART_TRANSFER_STATE;
                sChRx = 0;
            }
            
        break;
        
        case UART_TRANSFER_STATE:
            
            Green_LED_Write(0);            //Added as a debug LED; turns off when state changes from IDLE to SAMPLING.
            Red_LED_Write(1);            
            Yellow_LED_Write(0);
            
            //char buffer[100];
            
            
            for(uint16_t idx = 0; idx < 1024; idx++)
            {
                //Send the ADC Sampled data to MATLAB
                UART_LOG_PutChar(SamplingArray[idx]);
                UART_LOG_PutChar(SamplingArray[idx]>>8);
            }
            
            fft_app(SamplingArray,FFT_SamplesArray,1024);
            
            for(int32 idx = 0; idx < 2048; idx++)
            {
                //Send the ADC Sampled data to MATLAB
                UART_LOG_PutChar(FFT_SamplesArray[idx]);
                UART_LOG_PutChar(FFT_SamplesArray[idx]>>8);
                UART_LOG_PutChar(FFT_SamplesArray[idx]>>16);
                UART_LOG_PutChar(FFT_SamplesArray[idx]>>24);
            }
            
           
            count++;                        // Increment the count to repeat the above process for 10 times.
            
            if((count < 10) && oChRx)
            {  
                User_LED_Write(0);          // Added as a debug pin, turns off when 'o' is received from Matlab or termite.
                State = SAMPLING_STATE;
                oChRx = 0;
            }
            else if((count == 10) && oChRx)
            {
                State = IDLE_STATE;  
                oChRx = 0;
            }
        break;
            
        default:
            State = IDLE_STATE; 
            
            Yellow_LED_Write(0);           
            Green_LED_Write(0);
            Red_LED_Write(0);
        break;
    }
}