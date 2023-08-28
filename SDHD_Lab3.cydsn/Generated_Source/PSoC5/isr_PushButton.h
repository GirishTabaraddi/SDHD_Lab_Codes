/*******************************************************************************
* File Name: isr_PushButton.h
* Version 1.71
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_isr_PushButton_H)
#define CY_ISR_isr_PushButton_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr_PushButton_Start(void);
void isr_PushButton_StartEx(cyisraddress address);
void isr_PushButton_Stop(void);

CY_ISR_PROTO(isr_PushButton_Interrupt);

void isr_PushButton_SetVector(cyisraddress address);
cyisraddress isr_PushButton_GetVector(void);

void isr_PushButton_SetPriority(uint8 priority);
uint8 isr_PushButton_GetPriority(void);

void isr_PushButton_Enable(void);
uint8 isr_PushButton_GetState(void);
void isr_PushButton_Disable(void);

void isr_PushButton_SetPending(void);
void isr_PushButton_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr_PushButton ISR. */
#define isr_PushButton_INTC_VECTOR            ((reg32 *) isr_PushButton__INTC_VECT)

/* Address of the isr_PushButton ISR priority. */
#define isr_PushButton_INTC_PRIOR             ((reg8 *) isr_PushButton__INTC_PRIOR_REG)

/* Priority of the isr_PushButton interrupt. */
#define isr_PushButton_INTC_PRIOR_NUMBER      isr_PushButton__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr_PushButton interrupt. */
#define isr_PushButton_INTC_SET_EN            ((reg32 *) isr_PushButton__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr_PushButton interrupt. */
#define isr_PushButton_INTC_CLR_EN            ((reg32 *) isr_PushButton__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr_PushButton interrupt state to pending. */
#define isr_PushButton_INTC_SET_PD            ((reg32 *) isr_PushButton__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr_PushButton interrupt. */
#define isr_PushButton_INTC_CLR_PD            ((reg32 *) isr_PushButton__INTC_CLR_PD_REG)


#endif /* CY_ISR_isr_PushButton_H */


/* [] END OF FILE */
