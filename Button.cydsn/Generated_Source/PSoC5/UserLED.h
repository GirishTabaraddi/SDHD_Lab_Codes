/*******************************************************************************
* File Name: UserLED.h  
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

#if !defined(CY_PINS_UserLED_H) /* Pins UserLED_H */
#define CY_PINS_UserLED_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "UserLED_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 UserLED__PORT == 15 && ((UserLED__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    UserLED_Write(uint8 value);
void    UserLED_SetDriveMode(uint8 mode);
uint8   UserLED_ReadDataReg(void);
uint8   UserLED_Read(void);
void    UserLED_SetInterruptMode(uint16 position, uint16 mode);
uint8   UserLED_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the UserLED_SetDriveMode() function.
     *  @{
     */
        #define UserLED_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define UserLED_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define UserLED_DM_RES_UP          PIN_DM_RES_UP
        #define UserLED_DM_RES_DWN         PIN_DM_RES_DWN
        #define UserLED_DM_OD_LO           PIN_DM_OD_LO
        #define UserLED_DM_OD_HI           PIN_DM_OD_HI
        #define UserLED_DM_STRONG          PIN_DM_STRONG
        #define UserLED_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define UserLED_MASK               UserLED__MASK
#define UserLED_SHIFT              UserLED__SHIFT
#define UserLED_WIDTH              1u

/* Interrupt constants */
#if defined(UserLED__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in UserLED_SetInterruptMode() function.
     *  @{
     */
        #define UserLED_INTR_NONE      (uint16)(0x0000u)
        #define UserLED_INTR_RISING    (uint16)(0x0001u)
        #define UserLED_INTR_FALLING   (uint16)(0x0002u)
        #define UserLED_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define UserLED_INTR_MASK      (0x01u) 
#endif /* (UserLED__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define UserLED_PS                     (* (reg8 *) UserLED__PS)
/* Data Register */
#define UserLED_DR                     (* (reg8 *) UserLED__DR)
/* Port Number */
#define UserLED_PRT_NUM                (* (reg8 *) UserLED__PRT) 
/* Connect to Analog Globals */                                                  
#define UserLED_AG                     (* (reg8 *) UserLED__AG)                       
/* Analog MUX bux enable */
#define UserLED_AMUX                   (* (reg8 *) UserLED__AMUX) 
/* Bidirectional Enable */                                                        
#define UserLED_BIE                    (* (reg8 *) UserLED__BIE)
/* Bit-mask for Aliased Register Access */
#define UserLED_BIT_MASK               (* (reg8 *) UserLED__BIT_MASK)
/* Bypass Enable */
#define UserLED_BYP                    (* (reg8 *) UserLED__BYP)
/* Port wide control signals */                                                   
#define UserLED_CTL                    (* (reg8 *) UserLED__CTL)
/* Drive Modes */
#define UserLED_DM0                    (* (reg8 *) UserLED__DM0) 
#define UserLED_DM1                    (* (reg8 *) UserLED__DM1)
#define UserLED_DM2                    (* (reg8 *) UserLED__DM2) 
/* Input Buffer Disable Override */
#define UserLED_INP_DIS                (* (reg8 *) UserLED__INP_DIS)
/* LCD Common or Segment Drive */
#define UserLED_LCD_COM_SEG            (* (reg8 *) UserLED__LCD_COM_SEG)
/* Enable Segment LCD */
#define UserLED_LCD_EN                 (* (reg8 *) UserLED__LCD_EN)
/* Slew Rate Control */
#define UserLED_SLW                    (* (reg8 *) UserLED__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define UserLED_PRTDSI__CAPS_SEL       (* (reg8 *) UserLED__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define UserLED_PRTDSI__DBL_SYNC_IN    (* (reg8 *) UserLED__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define UserLED_PRTDSI__OE_SEL0        (* (reg8 *) UserLED__PRTDSI__OE_SEL0) 
#define UserLED_PRTDSI__OE_SEL1        (* (reg8 *) UserLED__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define UserLED_PRTDSI__OUT_SEL0       (* (reg8 *) UserLED__PRTDSI__OUT_SEL0) 
#define UserLED_PRTDSI__OUT_SEL1       (* (reg8 *) UserLED__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define UserLED_PRTDSI__SYNC_OUT       (* (reg8 *) UserLED__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(UserLED__SIO_CFG)
    #define UserLED_SIO_HYST_EN        (* (reg8 *) UserLED__SIO_HYST_EN)
    #define UserLED_SIO_REG_HIFREQ     (* (reg8 *) UserLED__SIO_REG_HIFREQ)
    #define UserLED_SIO_CFG            (* (reg8 *) UserLED__SIO_CFG)
    #define UserLED_SIO_DIFF           (* (reg8 *) UserLED__SIO_DIFF)
#endif /* (UserLED__SIO_CFG) */

/* Interrupt Registers */
#if defined(UserLED__INTSTAT)
    #define UserLED_INTSTAT            (* (reg8 *) UserLED__INTSTAT)
    #define UserLED_SNAP               (* (reg8 *) UserLED__SNAP)
    
	#define UserLED_0_INTTYPE_REG 		(* (reg8 *) UserLED__0__INTTYPE)
#endif /* (UserLED__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_UserLED_H */


/* [] END OF FILE */
