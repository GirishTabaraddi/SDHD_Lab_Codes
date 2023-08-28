/*******************************************************************************
* File Name: UART_log.c
* Version 2.50
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_log.h"
#if (UART_log_INTERNAL_CLOCK_USED)
    #include "UART_log_IntClock.h"
#endif /* End UART_log_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8 UART_log_initVar = 0u;

#if (UART_log_TX_INTERRUPT_ENABLED && UART_log_TX_ENABLED)
    volatile uint8 UART_log_txBuffer[UART_log_TX_BUFFER_SIZE];
    volatile uint8 UART_log_txBufferRead = 0u;
    uint8 UART_log_txBufferWrite = 0u;
#endif /* (UART_log_TX_INTERRUPT_ENABLED && UART_log_TX_ENABLED) */

#if (UART_log_RX_INTERRUPT_ENABLED && (UART_log_RX_ENABLED || UART_log_HD_ENABLED))
    uint8 UART_log_errorStatus = 0u;
    volatile uint8 UART_log_rxBuffer[UART_log_RX_BUFFER_SIZE];
    volatile uint8 UART_log_rxBufferRead  = 0u;
    volatile uint8 UART_log_rxBufferWrite = 0u;
    volatile uint8 UART_log_rxBufferLoopDetect = 0u;
    volatile uint8 UART_log_rxBufferOverflow   = 0u;
    #if (UART_log_RXHW_ADDRESS_ENABLED)
        volatile uint8 UART_log_rxAddressMode = UART_log_RX_ADDRESS_MODE;
        volatile uint8 UART_log_rxAddressDetected = 0u;
    #endif /* (UART_log_RXHW_ADDRESS_ENABLED) */
#endif /* (UART_log_RX_INTERRUPT_ENABLED && (UART_log_RX_ENABLED || UART_log_HD_ENABLED)) */


/*******************************************************************************
* Function Name: UART_log_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  UART_log_Start() sets the initVar variable, calls the
*  UART_log_Init() function, and then calls the
*  UART_log_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The UART_log_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time UART_log_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the UART_log_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void UART_log_Start(void) 
{
    /* If not initialized then initialize all required hardware and software */
    if(UART_log_initVar == 0u)
    {
        UART_log_Init();
        UART_log_initVar = 1u;
    }

    UART_log_Enable();
}


/*******************************************************************************
* Function Name: UART_log_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call UART_log_Init() because
*  the UART_log_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UART_log_Init(void) 
{
    #if(UART_log_RX_ENABLED || UART_log_HD_ENABLED)

        #if (UART_log_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(UART_log_RX_VECT_NUM, &UART_log_RXISR);
            CyIntSetPriority(UART_log_RX_VECT_NUM, UART_log_RX_PRIOR_NUM);
            UART_log_errorStatus = 0u;
        #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        #if (UART_log_RXHW_ADDRESS_ENABLED)
            UART_log_SetRxAddressMode(UART_log_RX_ADDRESS_MODE);
            UART_log_SetRxAddress1(UART_log_RX_HW_ADDRESS1);
            UART_log_SetRxAddress2(UART_log_RX_HW_ADDRESS2);
        #endif /* End UART_log_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        UART_log_RXBITCTR_PERIOD_REG = UART_log_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        UART_log_RXSTATUS_MASK_REG  = UART_log_INIT_RX_INTERRUPTS_MASK;
    #endif /* End UART_log_RX_ENABLED || UART_log_HD_ENABLED*/

    #if(UART_log_TX_ENABLED)
        #if (UART_log_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(UART_log_TX_VECT_NUM, &UART_log_TXISR);
            CyIntSetPriority(UART_log_TX_VECT_NUM, UART_log_TX_PRIOR_NUM);
        #endif /* (UART_log_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (UART_log_TXCLKGEN_DP)
            UART_log_TXBITCLKGEN_CTR_REG = UART_log_BIT_CENTER;
            UART_log_TXBITCLKTX_COMPLETE_REG = ((UART_log_NUMBER_OF_DATA_BITS +
                        UART_log_NUMBER_OF_START_BIT) * UART_log_OVER_SAMPLE_COUNT) - 1u;
        #else
            UART_log_TXBITCTR_PERIOD_REG = ((UART_log_NUMBER_OF_DATA_BITS +
                        UART_log_NUMBER_OF_START_BIT) * UART_log_OVER_SAMPLE_8) - 1u;
        #endif /* End UART_log_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (UART_log_TX_INTERRUPT_ENABLED)
            UART_log_TXSTATUS_MASK_REG = UART_log_TX_STS_FIFO_EMPTY;
        #else
            UART_log_TXSTATUS_MASK_REG = UART_log_INIT_TX_INTERRUPTS_MASK;
        #endif /*End UART_log_TX_INTERRUPT_ENABLED*/

    #endif /* End UART_log_TX_ENABLED */

    #if(UART_log_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        UART_log_WriteControlRegister( \
            (UART_log_ReadControlRegister() & (uint8)~UART_log_CTRL_PARITY_TYPE_MASK) | \
            (uint8)(UART_log_PARITY_TYPE << UART_log_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End UART_log_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: UART_log_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call UART_log_Enable() because the UART_log_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  UART_log_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void UART_log_Enable(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (UART_log_RX_ENABLED || UART_log_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        UART_log_RXBITCTR_CONTROL_REG |= UART_log_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        UART_log_RXSTATUS_ACTL_REG  |= UART_log_INT_ENABLE;

        #if (UART_log_RX_INTERRUPT_ENABLED)
            UART_log_EnableRxInt();

            #if (UART_log_RXHW_ADDRESS_ENABLED)
                UART_log_rxAddressDetected = 0u;
            #endif /* (UART_log_RXHW_ADDRESS_ENABLED) */
        #endif /* (UART_log_RX_INTERRUPT_ENABLED) */
    #endif /* (UART_log_RX_ENABLED || UART_log_HD_ENABLED) */

    #if(UART_log_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!UART_log_TXCLKGEN_DP)
            UART_log_TXBITCTR_CONTROL_REG |= UART_log_CNTR_ENABLE;
        #endif /* End UART_log_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        UART_log_TXSTATUS_ACTL_REG |= UART_log_INT_ENABLE;
        #if (UART_log_TX_INTERRUPT_ENABLED)
            UART_log_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            UART_log_EnableTxInt();
        #endif /* (UART_log_TX_INTERRUPT_ENABLED) */
     #endif /* (UART_log_TX_INTERRUPT_ENABLED) */

    #if (UART_log_INTERNAL_CLOCK_USED)
        UART_log_IntClock_Start();  /* Enable the clock */
    #endif /* (UART_log_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UART_log_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void UART_log_Stop(void) 
{
    uint8 enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (UART_log_RX_ENABLED || UART_log_HD_ENABLED)
        UART_log_RXBITCTR_CONTROL_REG &= (uint8) ~UART_log_CNTR_ENABLE;
    #endif /* (UART_log_RX_ENABLED || UART_log_HD_ENABLED) */

    #if (UART_log_TX_ENABLED)
        #if(!UART_log_TXCLKGEN_DP)
            UART_log_TXBITCTR_CONTROL_REG &= (uint8) ~UART_log_CNTR_ENABLE;
        #endif /* (!UART_log_TXCLKGEN_DP) */
    #endif /* (UART_log_TX_ENABLED) */

    #if (UART_log_INTERNAL_CLOCK_USED)
        UART_log_IntClock_Stop();   /* Disable the clock */
    #endif /* (UART_log_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (UART_log_RX_ENABLED || UART_log_HD_ENABLED)
        UART_log_RXSTATUS_ACTL_REG  &= (uint8) ~UART_log_INT_ENABLE;

        #if (UART_log_RX_INTERRUPT_ENABLED)
            UART_log_DisableRxInt();
        #endif /* (UART_log_RX_INTERRUPT_ENABLED) */
    #endif /* (UART_log_RX_ENABLED || UART_log_HD_ENABLED) */

    #if (UART_log_TX_ENABLED)
        UART_log_TXSTATUS_ACTL_REG &= (uint8) ~UART_log_INT_ENABLE;

        #if (UART_log_TX_INTERRUPT_ENABLED)
            UART_log_DisableTxInt();
        #endif /* (UART_log_TX_INTERRUPT_ENABLED) */
    #endif /* (UART_log_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: UART_log_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8 UART_log_ReadControlRegister(void) 
{
    #if (UART_log_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(UART_log_CONTROL_REG);
    #endif /* (UART_log_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: UART_log_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  UART_log_WriteControlRegister(uint8 control) 
{
    #if (UART_log_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       UART_log_CONTROL_REG = control;
    #endif /* (UART_log_CONTROL_REG_REMOVED) */
}


#if(UART_log_RX_ENABLED || UART_log_HD_ENABLED)
    /*******************************************************************************
    * Function Name: UART_log_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      UART_log_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      UART_log_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      UART_log_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      UART_log_RX_STS_BREAK            Interrupt on break.
    *      UART_log_RX_STS_OVERRUN          Interrupt on overrun error.
    *      UART_log_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      UART_log_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UART_log_SetRxInterruptMode(uint8 intSrc) 
    {
        UART_log_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: UART_log_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  UART_log_rxBuffer - RAM buffer pointer for save received data.
    *  UART_log_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UART_log_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UART_log_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UART_log_ReadRxData(void) 
    {
        uint8 rxData;

    #if (UART_log_RX_INTERRUPT_ENABLED)

        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UART_log_DisableRxInt();

        locRxBufferRead  = UART_log_rxBufferRead;
        locRxBufferWrite = UART_log_rxBufferWrite;

        if( (UART_log_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UART_log_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= UART_log_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UART_log_rxBufferRead = locRxBufferRead;

            if(UART_log_rxBufferLoopDetect != 0u)
            {
                UART_log_rxBufferLoopDetect = 0u;
                #if ((UART_log_RX_INTERRUPT_ENABLED) && (UART_log_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UART_log_HD_ENABLED )
                        if((UART_log_CONTROL_REG & UART_log_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UART_log_RXSTATUS_MASK_REG  |= UART_log_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UART_log_RXSTATUS_MASK_REG  |= UART_log_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UART_log_HD_ENABLED */
                #endif /* ((UART_log_RX_INTERRUPT_ENABLED) && (UART_log_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = UART_log_RXDATA_REG;
        }

        UART_log_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = UART_log_RXDATA_REG;

    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UART_log_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  UART_log_RX_STS_FIFO_NOTEMPTY.
    *  UART_log_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  UART_log_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   UART_log_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   UART_log_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8 UART_log_ReadRxStatus(void) 
    {
        uint8 status;

        status = UART_log_RXSTATUS_REG & UART_log_RX_HW_MASK;

    #if (UART_log_RX_INTERRUPT_ENABLED)
        if(UART_log_rxBufferOverflow != 0u)
        {
            status |= UART_log_RX_STS_SOFT_BUFF_OVER;
            UART_log_rxBufferOverflow = 0u;
        }
    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: UART_log_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. UART_log_GetChar() is
    *  designed for ASCII characters and returns a uint8 where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  UART_log_rxBuffer - RAM buffer pointer for save received data.
    *  UART_log_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  UART_log_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  UART_log_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8 UART_log_GetChar(void) 
    {
        uint8 rxData = 0u;
        uint8 rxStatus;

    #if (UART_log_RX_INTERRUPT_ENABLED)
        uint8 locRxBufferRead;
        uint8 locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        UART_log_DisableRxInt();

        locRxBufferRead  = UART_log_rxBufferRead;
        locRxBufferWrite = UART_log_rxBufferWrite;

        if( (UART_log_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = UART_log_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= UART_log_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            UART_log_rxBufferRead = locRxBufferRead;

            if(UART_log_rxBufferLoopDetect != 0u)
            {
                UART_log_rxBufferLoopDetect = 0u;
                #if( (UART_log_RX_INTERRUPT_ENABLED) && (UART_log_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( UART_log_HD_ENABLED )
                        if((UART_log_CONTROL_REG & UART_log_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            UART_log_RXSTATUS_MASK_REG |= UART_log_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        UART_log_RXSTATUS_MASK_REG |= UART_log_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end UART_log_HD_ENABLED */
                #endif /* UART_log_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = UART_log_RXSTATUS_REG;
            if((rxStatus & UART_log_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = UART_log_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (UART_log_RX_STS_BREAK | UART_log_RX_STS_PAR_ERROR |
                                UART_log_RX_STS_STOP_ERROR | UART_log_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        UART_log_EnableRxInt();

    #else

        rxStatus =UART_log_RXSTATUS_REG;
        if((rxStatus & UART_log_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = UART_log_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (UART_log_RX_STS_BREAK | UART_log_RX_STS_PAR_ERROR |
                            UART_log_RX_STS_STOP_ERROR | UART_log_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: UART_log_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 UART_log_GetByte(void) 
    {
        
    #if (UART_log_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        UART_log_DisableRxInt();
        locErrorStatus = (uint16)UART_log_errorStatus;
        UART_log_errorStatus = 0u;
        UART_log_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | UART_log_ReadRxData() );
    #else
        return ( ((uint16)UART_log_ReadRxStatus() << 8u) | UART_log_ReadRxData() );
    #endif /* UART_log_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: UART_log_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  uint8: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  UART_log_rxBufferWrite - used to calculate left bytes.
    *  UART_log_rxBufferRead - used to calculate left bytes.
    *  UART_log_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    uint8 UART_log_GetRxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UART_log_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        UART_log_DisableRxInt();

        if(UART_log_rxBufferRead == UART_log_rxBufferWrite)
        {
            if(UART_log_rxBufferLoopDetect != 0u)
            {
                size = UART_log_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(UART_log_rxBufferRead < UART_log_rxBufferWrite)
        {
            size = (UART_log_rxBufferWrite - UART_log_rxBufferRead);
        }
        else
        {
            size = (UART_log_RX_BUFFER_SIZE - UART_log_rxBufferRead) + UART_log_rxBufferWrite;
        }

        UART_log_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((UART_log_RXSTATUS_REG & UART_log_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: UART_log_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_rxBufferWrite - cleared to zero.
    *  UART_log_rxBufferRead - cleared to zero.
    *  UART_log_rxBufferLoopDetect - cleared to zero.
    *  UART_log_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void UART_log_ClearRxBuffer(void) 
    {
        uint8 enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        UART_log_RXDATA_AUX_CTL_REG |= (uint8)  UART_log_RX_FIFO_CLR;
        UART_log_RXDATA_AUX_CTL_REG &= (uint8) ~UART_log_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UART_log_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART_log_DisableRxInt();

        UART_log_rxBufferRead = 0u;
        UART_log_rxBufferWrite = 0u;
        UART_log_rxBufferLoopDetect = 0u;
        UART_log_rxBufferOverflow = 0u;

        UART_log_EnableRxInt();

    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: UART_log_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  UART_log__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  UART_log__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  UART_log__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  UART_log__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  UART_log__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  UART_log_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void UART_log_SetRxAddressMode(uint8 addressMode)
                                                        
    {
        #if(UART_log_RXHW_ADDRESS_ENABLED)
            #if(UART_log_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* UART_log_CONTROL_REG_REMOVED */
                uint8 tmpCtrl;
                tmpCtrl = UART_log_CONTROL_REG & (uint8)~UART_log_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8)(addressMode << UART_log_CTRL_RXADDR_MODE0_SHIFT);
                UART_log_CONTROL_REG = tmpCtrl;

                #if(UART_log_RX_INTERRUPT_ENABLED && \
                   (UART_log_RXBUFFERSIZE > UART_log_FIFO_LENGTH) )
                    UART_log_rxAddressMode = addressMode;
                    UART_log_rxAddressDetected = 0u;
                #endif /* End UART_log_RXBUFFERSIZE > UART_log_FIFO_LENGTH*/
            #endif /* End UART_log_CONTROL_REG_REMOVED */
        #else /* UART_log_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End UART_log_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UART_log_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UART_log_SetRxAddress1(uint8 address) 
    {
        UART_log_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: UART_log_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void UART_log_SetRxAddress2(uint8 address) 
    {
        UART_log_RXADDRESS2_REG = address;
    }

#endif  /* UART_log_RX_ENABLED || UART_log_HD_ENABLED*/


#if( (UART_log_TX_ENABLED) || (UART_log_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: UART_log_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   UART_log_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   UART_log_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   UART_log_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   UART_log_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void UART_log_SetTxInterruptMode(uint8 intSrc) 
    {
        UART_log_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: UART_log_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  UART_log_txBuffer - RAM buffer pointer for save data for transmission
    *  UART_log_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  UART_log_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  UART_log_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UART_log_WriteTxData(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function*/
        if(UART_log_initVar != 0u)
        {
        #if (UART_log_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            UART_log_DisableTxInt();

            if( (UART_log_txBufferRead == UART_log_txBufferWrite) &&
                ((UART_log_TXSTATUS_REG & UART_log_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                UART_log_TXDATA_REG = txDataByte;
            }
            else
            {
                if(UART_log_txBufferWrite >= UART_log_TX_BUFFER_SIZE)
                {
                    UART_log_txBufferWrite = 0u;
                }

                UART_log_txBuffer[UART_log_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                UART_log_txBufferWrite++;
            }

            UART_log_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            UART_log_TXDATA_REG = txDataByte;

        #endif /*(UART_log_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: UART_log_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8 UART_log_ReadTxStatus(void) 
    {
        return(UART_log_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: UART_log_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_txBuffer - RAM buffer pointer for save data for transmission
    *  UART_log_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  UART_log_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  UART_log_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void UART_log_PutChar(uint8 txDataByte) 
    {
    #if (UART_log_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        uint8 locTxBufferWrite;
        uint8 locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            UART_log_DisableTxInt();
        #endif /* (UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = UART_log_txBufferWrite;
            locTxBufferRead  = UART_log_txBufferRead;

        #if ((UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            UART_log_EnableTxInt();
        #endif /* (UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (uint8)(UART_log_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((UART_log_TXSTATUS_REG & UART_log_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            UART_log_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= UART_log_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            UART_log_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3))
            UART_log_DisableTxInt();
        #endif /* (UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3) */

            UART_log_txBufferWrite = locTxBufferWrite;

        #if ((UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3))
            UART_log_EnableTxInt();
        #endif /* (UART_log_TX_BUFFER_SIZE > UART_log_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (UART_log_TXSTATUS_REG & UART_log_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                UART_log_SetPendingTxInt();
            }
        }

    #else

        while((UART_log_TXSTATUS_REG & UART_log_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        UART_log_TXDATA_REG = txDataByte;

    #endif /* UART_log_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: UART_log_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UART_log_PutString(const char8 string[]) 
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UART_log_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                UART_log_PutChar((uint8)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UART_log_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void UART_log_PutArray(const uint8 string[], uint8 byteCount)
                                                                    
    {
        uint8 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(UART_log_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                UART_log_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: UART_log_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void UART_log_PutCRLF(uint8 txDataByte) 
    {
        /* If not Initialized then skip this function */
        if(UART_log_initVar != 0u)
        {
            UART_log_PutChar(txDataByte);
            UART_log_PutChar(0x0Du);
            UART_log_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: UART_log_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  UART_log_txBufferWrite - used to calculate left space.
    *  UART_log_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    uint8 UART_log_GetTxBufferSize(void)
                                                            
    {
        uint8 size;

    #if (UART_log_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART_log_DisableTxInt();

        if(UART_log_txBufferRead == UART_log_txBufferWrite)
        {
            size = 0u;
        }
        else if(UART_log_txBufferRead < UART_log_txBufferWrite)
        {
            size = (UART_log_txBufferWrite - UART_log_txBufferRead);
        }
        else
        {
            size = (UART_log_TX_BUFFER_SIZE - UART_log_txBufferRead) +
                    UART_log_txBufferWrite;
        }

        UART_log_EnableTxInt();

    #else

        size = UART_log_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & UART_log_TX_STS_FIFO_FULL) != 0u)
        {
            size = UART_log_FIFO_LENGTH;
        }
        else if((size & UART_log_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (UART_log_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: UART_log_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_txBufferWrite - cleared to zero.
    *  UART_log_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void UART_log_ClearTxBuffer(void) 
    {
        uint8 enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        UART_log_TXDATA_AUX_CTL_REG |= (uint8)  UART_log_TX_FIFO_CLR;
        UART_log_TXDATA_AUX_CTL_REG &= (uint8) ~UART_log_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (UART_log_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        UART_log_DisableTxInt();

        UART_log_txBufferRead = 0u;
        UART_log_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        UART_log_EnableTxInt();

    #endif /* (UART_log_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UART_log_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8 retMode:  Send Break return mode. See the following table for options.
    *   UART_log_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   UART_log_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   UART_log_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   UART_log_SEND_WAIT_REINIT - Performs both options: 
    *      UART_log_SEND_BREAK and UART_log_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_log_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with UART_log_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The UART_log_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void UART_log_SendBreak(uint8 retMode) 
    {

        /* If not Initialized then skip this function*/
        if(UART_log_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8 tmpStat;

        #if(UART_log_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == UART_log_SEND_BREAK) ||
                (retMode == UART_log_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                UART_log_WriteControlRegister(UART_log_ReadControlRegister() |
                                                      UART_log_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                UART_log_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UART_log_TXSTATUS_REG;
                }
                while((tmpStat & UART_log_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UART_log_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART_log_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UART_log_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UART_log_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UART_log_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART_log_REINIT) ||
                (retMode == UART_log_SEND_WAIT_REINIT) )
            {
                UART_log_WriteControlRegister(UART_log_ReadControlRegister() &
                                              (uint8)~UART_log_CTRL_HD_SEND_BREAK);
            }

        #else /* UART_log_HD_ENABLED Full Duplex mode */

            static uint8 txPeriod;

            if( (retMode == UART_log_SEND_BREAK) ||
                (retMode == UART_log_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (UART_log_PARITY_TYPE != UART_log__B_UART__NONE_REVB) || \
                                    (UART_log_PARITY_TYPE_SW != 0u) )
                    UART_log_WriteControlRegister(UART_log_ReadControlRegister() |
                                                          UART_log_CTRL_HD_SEND_BREAK);
                #endif /* End UART_log_PARITY_TYPE != UART_log__B_UART__NONE_REVB  */

                #if(UART_log_TXCLKGEN_DP)
                    txPeriod = UART_log_TXBITCLKTX_COMPLETE_REG;
                    UART_log_TXBITCLKTX_COMPLETE_REG = UART_log_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = UART_log_TXBITCTR_PERIOD_REG;
                    UART_log_TXBITCTR_PERIOD_REG = UART_log_TXBITCTR_BREAKBITS8X;
                #endif /* End UART_log_TXCLKGEN_DP */

                /* Send zeros */
                UART_log_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = UART_log_TXSTATUS_REG;
                }
                while((tmpStat & UART_log_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == UART_log_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART_log_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = UART_log_TXSTATUS_REG;
                }
                while(((uint8)~tmpStat & UART_log_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == UART_log_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == UART_log_REINIT) ||
                (retMode == UART_log_SEND_WAIT_REINIT) )
            {

            #if(UART_log_TXCLKGEN_DP)
                UART_log_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                UART_log_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End UART_log_TXCLKGEN_DP */

            #if( (UART_log_PARITY_TYPE != UART_log__B_UART__NONE_REVB) || \
                 (UART_log_PARITY_TYPE_SW != 0u) )
                UART_log_WriteControlRegister(UART_log_ReadControlRegister() &
                                                      (uint8) ~UART_log_CTRL_HD_SEND_BREAK);
            #endif /* End UART_log_PARITY_TYPE != NONE */
            }
        #endif    /* End UART_log_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: UART_log_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       UART_log_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       UART_log_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears UART_log_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void UART_log_SetTxAddressMode(uint8 addressMode) 
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( UART_log_CONTROL_REG_REMOVED == 0u )
            UART_log_WriteControlRegister(UART_log_ReadControlRegister() |
                                                  UART_log_CTRL_MARK);
        #endif /* End UART_log_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( UART_log_CONTROL_REG_REMOVED == 0u )
            UART_log_WriteControlRegister(UART_log_ReadControlRegister() &
                                                  (uint8) ~UART_log_CTRL_MARK);
        #endif /* End UART_log_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* EndUART_log_TX_ENABLED */

#if(UART_log_HD_ENABLED)


    /*******************************************************************************
    * Function Name: UART_log_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void UART_log_LoadRxConfig(void) 
    {
        UART_log_WriteControlRegister(UART_log_ReadControlRegister() &
                                                (uint8)~UART_log_CTRL_HD_SEND);
        UART_log_RXBITCTR_PERIOD_REG = UART_log_HD_RXBITCTR_INIT;

    #if (UART_log_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        UART_log_SetRxInterruptMode(UART_log_INIT_RX_INTERRUPTS_MASK);
    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: UART_log_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void UART_log_LoadTxConfig(void) 
    {
    #if (UART_log_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        UART_log_SetRxInterruptMode(0u);
    #endif /* (UART_log_RX_INTERRUPT_ENABLED) */

        UART_log_WriteControlRegister(UART_log_ReadControlRegister() | UART_log_CTRL_HD_SEND);
        UART_log_RXBITCTR_PERIOD_REG = UART_log_HD_TXBITCTR_INIT;
    }

#endif  /* UART_log_HD_ENABLED */


/* [] END OF FILE */
