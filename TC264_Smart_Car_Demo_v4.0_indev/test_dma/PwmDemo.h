/**
 * \file Ccu6TpwmDemo.h
 * \brief Demo Ccu6TpwmDemo
 *
 * \version iLLD_Demos_1_0_0_11_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Demo_Ccu6Tpwm_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_Ccu6Tpwm_SrcDoc
 * \defgroup IfxLld_Demo_Ccu6Tpwm_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_Ccu6Tpwm_SrcDoc_Main
 */

#ifndef CCU6TPWMDEMO_H
#define CCU6TPWMDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <Ifx_Types.h>
#include "Configuration.h"
#include "Ccu6/TPwm/IfxCcu6_TPwm.h"
#include <stdio.h>
#include "Ifxgtm_reg.h"
#include "Gtm/Std/IfxGtm_Cmu.h"
#include "Gtm/Std/IfxGtm_Tom.h"
#include "_PinMap/IfxGtm_PinMap.h"
#include "Compilers.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Asc information */
typedef struct
{
    struct
    {
        IfxCcu6_TPwm tPwm;                      /**< \brief ASC interface */
    }      drivers;

    uint32 count;
} App_Ccu6Tpwm;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

IFX_EXTERN App_Ccu6Tpwm g_Ccu6Tpwm;

extern uint32 PWM_Duty0;
extern uint32 PWM_Duty1;
extern uint32 PWM_Duty2;

extern uint32 PWM_Period0;
extern uint32 PWM_Period1;
extern uint32 PWM_Period2;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/


IFX_EXTERN void GTM_Clocks_Init(void);
IFX_EXTERN void TOM0_init(void);
IFX_EXTERN void TOM1_init(void);
IFX_EXTERN void TOM2_init(void);
#endif
