/*
 * CameraOV7620.h
 *
 *  Created on: 2018Äê1ÔÂ18ÈÕ
 *      Author: HP
 */

#ifndef _CAMERAOV7620_H_
#define _CAMERAOV7620_H_

#include "SysSe/Bsp/Bsp.h"
#include "Scu\Std\IfxScuWdt.h"
#include "Ifx_reg.h"
#include "Ifxgtm_reg.h"
#include "Gtm/Std/IfxGtm_Cmu.h"
#include "Gtm/Std/IfxGtm_Tom.h"
#include "_PinMap/IfxGtm_PinMap.h"
#include "Compilers.h"

IFX_EXTERN uint32 InPerid;   //the period of captured PWM signal
IFX_EXTERN uint32 InDuty;    //the duty of captured PWM signal
//IFX_EXTERN uint32 speedcountA,TimPwmPeridA;
IFX_EXTERN uint32 speedcountB,TimPwmPeridB,TimPwmDutyB;
IFX_EXTERN uint8 CoderAflag;
IFX_EXTERN void Tim_init(void);
IFX_EXTERN void UltrasoundInit(void);
IFX_EXTERN void CoderInit(void);
#endif /* 0_APPSW_TRICORE_MAIN_CAMERAOV7620_H_ */
