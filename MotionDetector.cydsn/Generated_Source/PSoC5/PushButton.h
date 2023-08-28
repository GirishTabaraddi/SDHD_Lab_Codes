/*******************************************************************************
* File Name: PushButton.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PushButton_H) /* Pins PushButton_H */
#define CY_PINS_PushButton_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "PushButton_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 PushButton__PORT == 15 && ((PushButton__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    PushButton_Write(uint8 value);
void    PushButton_SetDriveMode(uint8 mode);
uint8   PushButton_ReadDataReg(void);
uint8   PushButton_Read(void);
void    PushButton_SetInterruptMode(uint16 position, uint16 mode);
uint8   PushButton_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the PushButton_SetDriveMode() function.
     *  @{
     */
        #define PushButton_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define PushButton_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define PushButton_DM_RES_UP          PIN_DM_RES_UP
        #define PushButton_DM_RES_DWN         PIN_DM_RES_DWN
        #define PushButton_DM_OD_LO           PIN_DM_OD_LO
        #define PushButton_DM_OD_HI           PIN_DM_OD_HI
        #define PushButton_DM_STRONG          PIN_DM_STRONG
        #define PushButton_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define PushButton_MASK               PushButton__MASK
#define PushButton_SHIFT              PushButton__SHIFT
#define PushButton_WIDTH              1u

/* Interrupt constants */
#if defined(PushButton__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in PushButton_SetInterruptMode() function.
     *  @{
     */
        #define PushButton_INTR_NONE      (uint16)(0x0000u)
        #define PushButton_INTR_RISING    (uint16)(0x0001u)
        #define PushButton_INTR_FALLING   (uint16)(0x0002u)
        #define PushButton_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define PushButton_INTR_MASK      (0x01u) 
#endif /* (PushButton__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define PushButton_PS                     (* (reg8 *) PushButton__PS)
/* Data Register */
#define PushButton_DR                     (* (reg8 *) PushButton__DR)
/* Port Number */
#define PushButton_PRT_NUM                (* (reg8 *) PushButton__PRT) 
/* Connect to Analog Globals */                                                  
#define PushButton_AG                     (* (reg8 *) PushButton__AG)                       
/* Analog MUX bux enable */
#define PushButton_AMUX                   (* (reg8 *) PushButton__AMUX) 
/* Bidirectional Enable */                                                        
#define PushButton_BIE                    (* (reg8 *) PushButton__BIE)
/* Bit-mask for Aliased Register Access */
#define PushButton_BIT_MASK               (* (reg8 *) PushButton__BIT_MASK)
/* Bypass Enable */
#define PushButton_BYP                    (* (reg8 *) PushButton__BYP)
/* Port wide control signals */                                                   
#define PushButton_CTL                    (* (reg8 *) PushButton__CTL)
/* Drive Modes */
#define PushButton_DM0                    (* (reg8 *) PushButton__DM0) 
#define PushButton_DM1                    (* (reg8 *) PushButton__DM1)
#define PushButton_DM2                    (* (reg8 *) PushButton__DM2) 
/* Input Buffer Disable Override */
#define PushButton_INP_DIS                (* (reg8 *) PushButton__INP_DIS)
/* LCD Common or Segment Drive */
#define PushButton_LCD_COM_SEG            (* (reg8 *) PushButton__LCD_COM_SEG)
/* Enable Segment LCD */
#define PushButton_LCD_EN                 (* (reg8 *) PushButton__LCD_EN)
/* Slew Rate Control */
#define PushButton_SLW                    (* (reg8 *) PushButton__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define PushButton_PRTDSI__CAPS_SEL       (* (reg8 *) PushButton__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define PushButton_PRTDSI__DBL_SYNC_IN    (* (reg8 *) PushButton__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define PushButton_PRTDSI__OE_SEL0        (* (reg8 *) PushButton__PRTDSI__OE_SEL0) 
#define PushButton_PRTDSI__OE_SEL1        (* (reg8 *) PushButton__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define PushButton_PRTDSI__OUT_SEL0       (* (reg8 *) PushButton__PRTDSI__OUT_SEL0) 
#define PushButton_PRTDSI__OUT_SEL1       (* (reg8 *) PushButton__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define PushButton_PRTDSI__SYNC_OUT       (* (reg8 *) PushButton__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(PushButton__SIO_CFG)
    #define PushButton_SIO_HYST_EN        (* (reg8 *) PushButton__SIO_HYST_EN)
    #define PushButton_SIO_REG_HIFREQ     (* (reg8 *) PushButton__SIO_REG_HIFREQ)
    #define PushButton_SIO_CFG            (* (reg8 *) PushButton__SIO_CFG)
    #define PushButton_SIO_DIFF           (* (reg8 *) PushButton__SIO_DIFF)
#endif /* (PushButton__SIO_CFG) */

/* Interrupt Registers */
#if defined(PushButton__INTSTAT)
    #define PushButton_INTSTAT            (* (reg8 *) PushButton__INTSTAT)
    #define PushButton_SNAP               (* (reg8 *) PushButton__SNAP)
    
	#define PushButton_0_INTTYPE_REG 		(* (reg8 *) PushButton__0__INTTYPE)
#endif /* (PushButton__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_PushButton_H */


/* [] END OF FILE */
