/*******************************************************************************
* File Name: ExtButton.h  
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

#if !defined(CY_PINS_ExtButton_H) /* Pins ExtButton_H */
#define CY_PINS_ExtButton_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "ExtButton_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 ExtButton__PORT == 15 && ((ExtButton__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    ExtButton_Write(uint8 value);
void    ExtButton_SetDriveMode(uint8 mode);
uint8   ExtButton_ReadDataReg(void);
uint8   ExtButton_Read(void);
void    ExtButton_SetInterruptMode(uint16 position, uint16 mode);
uint8   ExtButton_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the ExtButton_SetDriveMode() function.
     *  @{
     */
        #define ExtButton_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define ExtButton_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define ExtButton_DM_RES_UP          PIN_DM_RES_UP
        #define ExtButton_DM_RES_DWN         PIN_DM_RES_DWN
        #define ExtButton_DM_OD_LO           PIN_DM_OD_LO
        #define ExtButton_DM_OD_HI           PIN_DM_OD_HI
        #define ExtButton_DM_STRONG          PIN_DM_STRONG
        #define ExtButton_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define ExtButton_MASK               ExtButton__MASK
#define ExtButton_SHIFT              ExtButton__SHIFT
#define ExtButton_WIDTH              1u

/* Interrupt constants */
#if defined(ExtButton__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in ExtButton_SetInterruptMode() function.
     *  @{
     */
        #define ExtButton_INTR_NONE      (uint16)(0x0000u)
        #define ExtButton_INTR_RISING    (uint16)(0x0001u)
        #define ExtButton_INTR_FALLING   (uint16)(0x0002u)
        #define ExtButton_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define ExtButton_INTR_MASK      (0x01u) 
#endif /* (ExtButton__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define ExtButton_PS                     (* (reg8 *) ExtButton__PS)
/* Data Register */
#define ExtButton_DR                     (* (reg8 *) ExtButton__DR)
/* Port Number */
#define ExtButton_PRT_NUM                (* (reg8 *) ExtButton__PRT) 
/* Connect to Analog Globals */                                                  
#define ExtButton_AG                     (* (reg8 *) ExtButton__AG)                       
/* Analog MUX bux enable */
#define ExtButton_AMUX                   (* (reg8 *) ExtButton__AMUX) 
/* Bidirectional Enable */                                                        
#define ExtButton_BIE                    (* (reg8 *) ExtButton__BIE)
/* Bit-mask for Aliased Register Access */
#define ExtButton_BIT_MASK               (* (reg8 *) ExtButton__BIT_MASK)
/* Bypass Enable */
#define ExtButton_BYP                    (* (reg8 *) ExtButton__BYP)
/* Port wide control signals */                                                   
#define ExtButton_CTL                    (* (reg8 *) ExtButton__CTL)
/* Drive Modes */
#define ExtButton_DM0                    (* (reg8 *) ExtButton__DM0) 
#define ExtButton_DM1                    (* (reg8 *) ExtButton__DM1)
#define ExtButton_DM2                    (* (reg8 *) ExtButton__DM2) 
/* Input Buffer Disable Override */
#define ExtButton_INP_DIS                (* (reg8 *) ExtButton__INP_DIS)
/* LCD Common or Segment Drive */
#define ExtButton_LCD_COM_SEG            (* (reg8 *) ExtButton__LCD_COM_SEG)
/* Enable Segment LCD */
#define ExtButton_LCD_EN                 (* (reg8 *) ExtButton__LCD_EN)
/* Slew Rate Control */
#define ExtButton_SLW                    (* (reg8 *) ExtButton__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define ExtButton_PRTDSI__CAPS_SEL       (* (reg8 *) ExtButton__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define ExtButton_PRTDSI__DBL_SYNC_IN    (* (reg8 *) ExtButton__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define ExtButton_PRTDSI__OE_SEL0        (* (reg8 *) ExtButton__PRTDSI__OE_SEL0) 
#define ExtButton_PRTDSI__OE_SEL1        (* (reg8 *) ExtButton__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define ExtButton_PRTDSI__OUT_SEL0       (* (reg8 *) ExtButton__PRTDSI__OUT_SEL0) 
#define ExtButton_PRTDSI__OUT_SEL1       (* (reg8 *) ExtButton__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define ExtButton_PRTDSI__SYNC_OUT       (* (reg8 *) ExtButton__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(ExtButton__SIO_CFG)
    #define ExtButton_SIO_HYST_EN        (* (reg8 *) ExtButton__SIO_HYST_EN)
    #define ExtButton_SIO_REG_HIFREQ     (* (reg8 *) ExtButton__SIO_REG_HIFREQ)
    #define ExtButton_SIO_CFG            (* (reg8 *) ExtButton__SIO_CFG)
    #define ExtButton_SIO_DIFF           (* (reg8 *) ExtButton__SIO_DIFF)
#endif /* (ExtButton__SIO_CFG) */

/* Interrupt Registers */
#if defined(ExtButton__INTSTAT)
    #define ExtButton_INTSTAT            (* (reg8 *) ExtButton__INTSTAT)
    #define ExtButton_SNAP               (* (reg8 *) ExtButton__SNAP)
    
	#define ExtButton_0_INTTYPE_REG 		(* (reg8 *) ExtButton__0__INTTYPE)
#endif /* (ExtButton__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_ExtButton_H */


/* [] END OF FILE */
