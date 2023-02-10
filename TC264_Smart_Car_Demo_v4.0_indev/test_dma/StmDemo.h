/**
 * \file StmDemo.h
 * \brief Demo Template
 * \version iLLD_Demos_1_0_0_11_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_StmDemo_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_StmDemo_SrcDoc
 * \defgroup IfxLld_Demo_StmDemo_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_StmDemo_SrcDoc_Main
 */

#ifndef STMDEMO_H
#define STMDEMO_H 1

#include "Cpu/Std/Platform_Types.h"
#include "Stm/Std/IfxStm.h"
#include "stdint.h"
#include <Bsp.h>
#include <CompilerTasking.h>
#include <Ifx_Types.h>
#include <IfxCpu.h>
#include <IfxCpu_IntrinsicsTasking.h>
#include <IfxCpu_Irq.h>
#include <IfxStm_cfg.h>
#include <IfxStm_reg.h>
#include <IfxStm_regdef.h>
#include "ServeSource.h"
/**
 *  STM模块枚举
 */
typedef enum
{
    STM0 = 0,
    STM1
}STM_t;

/**
 *  STM通道枚举
 */
typedef enum
{
    STM_Channel_0 = 0,
    STM_Channel_1
}STM_Channel_t;

/** STM定时器中断 STM0  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  STM0_CH0_PRIORITY    110

/** STM定时器中断 STM0  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  STM0_CH1_PRIORITY    111

/** STM定时器中断 STM0  中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  STM0_VECTABNUM       0


/** STM定时器中断 STM1  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  STM1_CH0_PRIORITY    112

/** STM定时器中断 STM1  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  STM1_CH1_PRIORITY    113

/** STM定时器中断 STM1   中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  STM1_VECTABNUM       0


extern IfxStm_CompareConfig g_StmCompareConfig[4];
void STM_InitConfig(STM_t STM, STM_Channel_t channel, unsigned long us);
void STM_DelayUs(STM_t stm, unsigned long us);
unsigned long STM_GetNowUs(STM_t stm);
void STM_DisableInterrupt(STM_t stm, STM_Channel_t channel);
void STM_EnableInterrupt(STM_t stm, STM_Channel_t channel);
void delayms(unsigned short stmms);
void delayus(unsigned short stmms);

#endif
