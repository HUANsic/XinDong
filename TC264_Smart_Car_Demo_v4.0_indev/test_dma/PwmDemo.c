/**
 * \file Ccu6TpwmDemo.c
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/


#include "PwmDemo.h"
#include "Hardware.h"
#include "ServeSource.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
#define PWM_FREQUECNY0 50   /*Hz*/
#define PWM_DUTY_CYCLE0 10   /*1%*/

#define PWM_FREQUECNY1 4000   /*Hz*/
#define PWM_DUTY_CYCLE1 15   /*1%*/

#define PWM_FREQUECNY2 4000   /*Hz*/
#define PWM_DUTY_CYCLE2 20   /*1%*/

//#define IFX_INTPRIO_GTM_TOM0_CH2 10
//#define IFX_INTPRIO_GTM_TOM0_CH2  11
//#define IFX_INTPRIO_GTM_TOM1_CH15 12



//#define ISR_PRIORITY_TIM0_CH2    15

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
//uint32 PWM_DUTY_CYCLE0;
//uint32 PWM_DUTY_CYCLE1;
//uint32 PWM_DUTY_CYCLE2;

uint32 PWM_Duty0;
uint32 PWM_Duty1;
uint32 PWM_Duty2;

uint32 PWM_Period0;
uint32 PWM_Period1;
uint32 PWM_Period2;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
void GTM_Clocks_Init(void)
{
    uint32 dummy ;
    uint16 password;
    password = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(password);
    GTM_CLC.U = 0x00000000 ;                            /* enable GTM module */
    dummy = GTM_CLC.U ;                                 /* need to read back before accessing SFRs */
    IfxScuWdt_setCpuEndinit(password);

    IfxGtm_Cmu_setGclkFrequency(&MODULE_GTM,100000000);   /* Set global clock to 100MHz */

    IfxGtm_Cmu_enableClocks(&MODULE_GTM,  0x00800000);    /* Enable the fixed clock */

    // Set GTM CMU_CLKx clock frequency = 10MHz
    IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, 10000000);

    //Enable Configurable Clock 0
    IfxGtm_Cmu_enableClocks(&MODULE_GTM, (0x00000002 << (IfxGtm_Cmu_Clk_0 * 2)));

}

void TOM0_init(void)
{

    /*Software reset for TOM channels */
    IfxGtm_Tom_Tgc_resetChannels (IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1),   0x80);

    /* Initialize the timer part */
    IfxGtm_Tom_Ch_setSignalLevel  ( &MODULE_GTM.TOM[1],  IfxGtm_Tom_Ch_15,  Ifx_ActiveState_high );                  /* SL : high level */
    IfxGtm_Tom_Ch_setClockSource  ( &MODULE_GTM.TOM[1],  IfxGtm_Tom_Ch_15,  IfxGtm_Tom_Ch_ClkSrc_cmuFxclk2);             /* CLK_SRC_SR : div by 1  */
    IfxGtm_Tom_Ch_setResetSource  ( &MODULE_GTM.TOM[1],  IfxGtm_Tom_Ch_15,  IfxGtm_Tom_Ch_ResetEvent_onCm0);             /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */
    //IfxGtm_Tom_Ch_setTriggerOutput( tc26x_tom,  IfxGtm_Tom_Ch_2,  IfxGtm_Tom_Ch_OutputTrigger_generate);     /* TRIGOUT : trigger out = CCU0 match */

    /*Write to SR0 and SR1 shadow registers for used channels */
    PWM_Period0 = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_2, TRUE)/PWM_FREQUECNY0;
    PWM_Duty0   = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_2, TRUE)*PWM_DUTY_CYCLE0/(PWM_FREQUECNY0*100);
    IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[1],  IfxGtm_Tom_Ch_15, PWM_Period0, PWM_Duty0);

    /*configure GPIO ports for TOM outputs */
    IfxGtm_PinMap_setTomTout (&IfxGtm_TOM1_15_TOUT67_P20_11_OUT, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);

    /* OUTEN : enable channel output on an update trigger for *all* channels */
    IfxGtm_Tom_Tgc_enableChannelsOutput ( IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1),  0x80, 0,  FALSE);

    /* ENDIS : enable channel on an update trigger for all channels  */
    IfxGtm_Tom_Tgc_enableChannels(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1), 0x80, 0, FALSE);


    /* FUPD : Enable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1), 0x80, 0, 0x00, 0);
    /* HOST_TRIG : Raise the trigger for the channel enable/disable settings, output enable settings, and force update event */
    IfxGtm_Tom_Tgc_trigger(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1));
    /* FUPD : Disable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1), 0, 0x80, 0x00, 0);

    /* Configure the interrupt for the TOM channel */
    volatile Ifx_SRC_SRCR *src;
    IfxGtm_IrqMode irqMode = IfxGtm_IrqMode_pulseNotify;
    IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, irqMode, TRUE, FALSE);
//    src = IfxGtm_Tom_Ch_getSrcPointer(&MODULE_GTM, IfxGtm_Tom_1, IfxGtm_Tom_Ch_15);
//    IfxSrc_init(src, (IfxSrc_Tos)IfxCpu_getCoreId(), IFX_INTPRIO_GTM_TOM1_CH15);
//    IfxSrc_enable(src);
}

void TOM1_init(void)
{

    /*Software reset for TOM channels */
    IfxGtm_Tom_Tgc_resetChannels (IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0),   0x04);

    /* Initialize the timer part */
    IfxGtm_Tom_Ch_setSignalLevel  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_2,  Ifx_ActiveState_low );                    /* SL : high level */
    IfxGtm_Tom_Ch_setClockSource  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_2,  IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1);              /* CLK_SRC_SR : div by 1  */
    IfxGtm_Tom_Ch_setResetSource  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_2,  IfxGtm_Tom_Ch_ResetEvent_onCm0);          /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */
    //IfxGtm_Tom_Ch_setTriggerOutput( tc26x_tom,  IfxGtm_Tom_Ch_2,  IfxGtm_Tom_Ch_OutputTrigger_generate);     /* TRIGOUT : trigger out = CCU0 match */

    /*Write to SR0 and SR1 shadow registers for used channels */
    PWM_Period1 = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_1, TRUE)/PWM_FREQUECNY1;
    PWM_Duty1   = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_1, TRUE)*PWM_DUTY_CYCLE1/(PWM_FREQUECNY1*100);
    IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_2, PWM_Period1, PWM_Duty1);

    /*configure GPIO ports for TOM outputs */
    IfxGtm_PinMap_setTomTout (&IfxGtm_TOM0_2_TOUT104_P10_2_OUT, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);

    /* OUTEN : enable channel output on an update trigger for *all* channels */
    IfxGtm_Tom_Tgc_enableChannelsOutput ( IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0),  0x04, 0,  FALSE);

    /* ENDIS : enable channel on an update trigger for all channels  */
    IfxGtm_Tom_Tgc_enableChannels(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x04, 0, FALSE);


    /* FUPD : Enable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x04, 0, 0x00, 0);
    /* HOST_TRIG : Raise the trigger for the channel enable/disable settings, output enable settings, and force update event */
    IfxGtm_Tom_Tgc_trigger(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0));
    /* FUPD : Disable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0, 0x04, 0x00, 0);

    /* Configure the interrupt for the TOM channel */
    volatile Ifx_SRC_SRCR *src;
    IfxGtm_IrqMode irqMode = IfxGtm_IrqMode_pulseNotify;
    IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, irqMode, TRUE, FALSE);
//    src = IfxGtm_Tom_Ch_getSrcPointer(&MODULE_GTM, IfxGtm_Tom_0, IfxGtm_Tom_Ch_2);
//    IfxSrc_init(src, (IfxSrc_Tos)IfxCpu_getCoreId(), IFX_INTPRIO_GTM_TOM0_CH2);
//    IfxSrc_enable(src);

}

void TOM2_init(void)
{

    /*Software reset for TOM channels */
    IfxGtm_Tom_Tgc_resetChannels (IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0),   0x08);

    /* Initialize the timer part */
    IfxGtm_Tom_Ch_setSignalLevel  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_3,  Ifx_ActiveState_low );                    /* SL : high level */
    IfxGtm_Tom_Ch_setClockSource  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_3,  IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1);              /* CLK_SRC_SR : div by 1  */
    IfxGtm_Tom_Ch_setResetSource  ( &MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_3,  IfxGtm_Tom_Ch_ResetEvent_onCm0);          /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */
    //IfxGtm_Tom_Ch_setTriggerOutput( tc26x_tom,  IfxGtm_Tom_Ch_2,  IfxGtm_Tom_Ch_OutputTrigger_generate);     /* TRIGOUT : trigger out = CCU0 match */

    /*Write to SR0 and SR1 shadow registers for used channels */
    PWM_Period2 = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_1, TRUE)/PWM_FREQUECNY2;
    PWM_Duty2   = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_1, TRUE)*PWM_DUTY_CYCLE2/(PWM_FREQUECNY2*100);
    IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[0],  IfxGtm_Tom_Ch_3, PWM_Period2, PWM_Duty2);

    /*configure GPIO ports for TOM outputs */
    IfxGtm_PinMap_setTomTout (&IfxGtm_TOM0_3_TOUT105_P10_3_OUT, IfxPort_OutputMode_pushPull, IfxPort_PadDriver_cmosAutomotiveSpeed1);

    /* OUTEN : enable channel output on an update trigger for *all* channels */
    IfxGtm_Tom_Tgc_enableChannelsOutput ( IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0),  0x08, 0,  FALSE);

    /* ENDIS : enable channel on an update trigger for all channels  */
    IfxGtm_Tom_Tgc_enableChannels(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08, 0, FALSE);


    /* FUPD : Enable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08, 0, 0x00, 0);
    /* HOST_TRIG : Raise the trigger for the channel enable/disable settings, output enable settings, and force update event */
    IfxGtm_Tom_Tgc_trigger(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0));
    /* FUPD : Disable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
    IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0, 0x08, 0x00, 0);

    /* Configure the interrupt for the TOM channel */
    volatile Ifx_SRC_SRCR *src;
    IfxGtm_IrqMode irqMode = IfxGtm_IrqMode_pulseNotify;
    IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, irqMode, TRUE, FALSE);
    //src = IfxGtm_Tom_Ch_getSrcPointer(&MODULE_GTM, IfxGtm_Tom_0, IfxGtm_Tom_Ch_3);
    //IfxSrc_init(src, (IfxSrc_Tos)IfxCpu_getCoreId(), IFX_INTPRIO_GTM_TOM0_CH3);
    //IfxSrc_enable(src);

}
