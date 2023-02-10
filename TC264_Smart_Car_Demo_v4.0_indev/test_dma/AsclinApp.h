/*
 * Asclin_app.h
 *
 *  Created on: 2018Äê1ÔÂ2ÈÕ
 *      Author: TEC
 */

#ifndef ASCLIN_APP_H
#define ASCLIN_APP_H

#define IFX_INTPRIO_ASCLIN0_TX  131
#define IFX_INTPRIO_ASCLIN0_RX  130
#define IFX_INTPRIO_ASCLIN0_ER  132
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Ifx_Types.h>
#include "Configuration.h"
#include <stdio.h>
#include "Bsp/Bsp.h"
#include "ConfigurationIsr.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "Port/Std/IfxPort.h"
#include <Stm/Std/IfxStm.h>
#include <Src/Std/IfxSrc.h>
#include "Cpu0_Main.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/





/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

void Asclin_TX_IRQHandler(void);
void Asclin_RX_IRQHandler(void);
void Asclin_ER_IRQHandler(void);
IFX_EXTERN void AsclinAsc_init(void);
IFX_EXTERN void Asclin_Send_Data(uint8 Asclin_Send_Data);
IFX_EXTERN uint8 Asclin_Read_Data(void);
IFX_EXTERN void Asclin_Send_String(char* Asclin_String);
#endif /* 0_APPSW_TRICORE_MAIN_ASCLIN_APP_H_ */
