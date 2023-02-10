#include "TimDemo.h"
#include "Hardware.h"
#include "ServeSource.h"

uint8 a;

uint32 test1=0,test2=0,test3=0,test4=0;  //调试用变量
//uint32 test4=0;



uint32 InPerid;   //the period of captured PWM signal
uint32 InDuty;    //the duty of captured PWM signal
//uint32 speedcountA=0,TimPwmPeridA=0, TimPwmDutyA=0;
uint32 speedcountB=0,TimPwmPeridB=0, TimPwmDutyB=0;
uint8 CoderAflag;
// TIM0_6  P20.0
IFX_INTERRUPT (Tim0Ch6Isr, 0, ISR_PRIORITY_TIM0_CH6)
{
    if(MODULE_GTM.TIM[0].CH6.IRQ_NOTIFY.B.NEWVAL == 1)
    {
        MODULE_GTM.TIM[0].CH6.IRQ_NOTIFY.B.NEWVAL = 1;
        UserInteruptIO();
    }
}


// TIM2_0  P02.8  ultra cap
IFX_INTERRUPT (Tim2Ch0Isr, 0, ISR_PRIORITY_TIM2_CH0)   //超声中断
{
    test4++;
    if(MODULE_GTM.TIM[2].CH0.IRQ_NOTIFY.B.NEWVAL == 1)
    {
        MODULE_GTM.TIM[2].CH0.IRQ_NOTIFY.B.NEWVAL = 1;
        InPerid = MODULE_GTM.TIM[2].CH0.GPR1.B.GPR1;
        InDuty = MODULE_GTM.TIM[2].CH0.GPR0.B.GPR0;
        test4++;
    }
}


// TIM2_6  P20.10
IFX_INTERRUPT (Tim2Ch6Isr, 0, ISR_PRIORITY_TIM2_CH6)
{
    if(MODULE_GTM.TIM[2].CH6.IRQ_NOTIFY.B.NEWVAL == 1)
    {
        MODULE_GTM.TIM[2].CH6.IRQ_NOTIFY.B.NEWVAL = 1;
        speedcountB++;
        CoderAflag=IfxPort_getPinState(CoderA);
        TimPwmPeridB = MODULE_GTM.TIM[2].CH6.GPR1.B.GPR1/100;  //uint: us
        TimPwmDutyB = MODULE_GTM.TIM[2].CH6.GPR0.B.GPR0/100;   //uint: us
    }

}

void Tim_init()
{
    /*
    IfxGtm_enable(&MODULE_GTM);
    while(! IfxGtm_isEnabled(&MODULE_GTM))
    {
        __debug();
    }
     */

    //IfxGtm_Cmu_Clk TPwmGtmCmuClk = IfxGtm_Cmu_Clk_0;
    //float32 TPwmGtmCmuGclkFreq = 100000000;
    //float32 TPwmGtmCmuClkFreq  = 10000000;

    Ifx_GTM_TBU tempGtmTbuCfg;
    Ifx_GTM_TIM tempGtmTimCfg[2];
    //  Ifx_GTM_TIM_AUX_IN_SRC tempGtmTimAuxInSrc[4];

    Ifx_SRC_SRCR * TPwmGtmTimChSrcR;
    uint16 TPwmGtmTimChPriority;

    /*   Check the Configurable Clock x is enalbed. If enabled, cannot set the clock frequency.
     Else may cause bus trap if clock is enabled!!!
    if(IfxGtm_Cmu_isClkClockEnabled(&MODULE_GTM, TPwmGtmCmuClk) == FALSE)
    {
         Set GTM CMU global clock frequency = 100MHz
        IfxGtm_Cmu_setGclkFrequency(&MODULE_GTM, TPwmGtmCmuGclkFreq);
         Set GTM CMU_CLKx clock frequency = 10MHz
        IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, TPwmGtmCmuClk, TPwmGtmCmuClkFreq);
         Enable Configurable Clock 0
        IfxGtm_Cmu_enableClocks(&MODULE_GTM, (0x00000002 << (TPwmGtmCmuClk * 2)));
    }*/

    /* TBU Channel configuration */
    /* No api so far */
    tempGtmTbuCfg.CH0_CTRL.B.CH_CLK_SRC = 0;    //0: CMU_CLK0; ...; 7: CMU_CLK7.
    tempGtmTbuCfg.CH0_CTRL.B.LOW_RES = 0;       //1: upper 24 bits(0:23). 0: lower 24 bits(3:26).
    tempGtmTbuCfg.CH1_CTRL.B.CH_CLK_SRC = 0;    //0: CMU_CLK0; ...; 7: CMU_CLK7.
    tempGtmTbuCfg.CH1_CTRL.B.CH_MODE = 0;       //1: forward and backward counter. 0: free running counter mode.
    tempGtmTbuCfg.CH2_CTRL.B.CH_CLK_SRC = 0;    //0: CMU_CLK0; ...; 7: CMU_CLK7.
    tempGtmTbuCfg.CH2_CTRL.B.CH_MODE = 0;       //1: forward and backward counter. 0: free running counter mode.

    MODULE_GTM.TBU.CH0_CTRL.U = tempGtmTbuCfg.CH0_CTRL.U;
    MODULE_GTM.TBU.CH1_CTRL.U = tempGtmTbuCfg.CH1_CTRL.U;
    MODULE_GTM.TBU.CH2_CTRL.U = tempGtmTbuCfg.CH2_CTRL.U;

    /* Enable TBU Channel 0 */
    IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_0);
    /* Enable TBU Channel 1 */
    IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_1);
    /* Enable TBU Channel 2 */
    IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_2);


    /* TIM Channel Configuration */
    //  an example , in order to set the other TIMs
    tempGtmTimCfg[0].CH0.CTRL.B.TOCTRL          = 2;    /* 0:Timeout feature disabled. 1:Timeout for both edges. 2:Timeout for rising edge. 3: Timeout for falling edge. */
    tempGtmTimCfg[0].CH0.CTRL.B.EGPR1_SEL       = 0;    /* Extension of GPR1_SEL bit field. */
    tempGtmTimCfg[0].CH0.CTRL.B.EGPR0_SEL       = 0;    /* Extension of GPR0_SEL bit field. */
    tempGtmTimCfg[0].CH0.CTRL.B.FR_ECNT_OFL     = 0;    /* Extended Edge counter overflow behaviour
                                                           0: Overflow will be signalled on ECNT bit width = 8
                                                           1: Overflow will be signalled on EECNT bit width (full range) */
    tempGtmTimCfg[0].CH0.CTRL.B.CLK_SEL         = 0;    /* 0...7: CMU_CLK0 ... CMU_CLK7. */
    tempGtmTimCfg[0].CH0.CTRL.B.FLT_CTR_FE      = 0;    /* 0:Up/down counter; 1:Hold counter. */
    tempGtmTimCfg[0].CH0.CTRL.B.FLT_MODE_FE     = 0;    /* 0:Immediate; 1:De-glitch mode. */
    tempGtmTimCfg[0].CH0.CTRL.B.FLT_CTR_RE      = 0;    /* 0:Up/down counter; 1:Hold counter. */
    tempGtmTimCfg[0].CH0.CTRL.B.FLT_MODE_RE     = 0;    /* 0:Immediate; 1:De-glitch mode. */
    tempGtmTimCfg[0].CH0.CTRL.B.EXT_CAP_EN      = 0;    /* 0:External capture disabled; 1:External capture enabled. */
    tempGtmTimCfg[0].CH0.CTRL.B.FLT_CNT_FRQ     = 0;    /* 0:Filter counts with CMU_CLK0; 1:CMU_CLK1; 2:CMU_CLK6; 3:CMU_CLK7. */
    tempGtmTimCfg[0].CH0.CTRL.B.ECNT_RESET      = 0;    /* 0:ECNT in wrap round mode; 1:ECNT counter is reset with periodic sampling */
    tempGtmTimCfg[0].CH0.CTRL.B.ISL             = 0;    /* 0:Use DSL for active level; 1:ignore DSL and treat both edges
                                                            [Note: This bit is only applicable in Input Event mode (TIEM and TIPM)]*/
    tempGtmTimCfg[0].CH0.CTRL.B.DSL             = 1;    /* 0:Falling edge(low level measurement); 1:Rising edge(high level measurement) */
    tempGtmTimCfg[0].CH0.CTRL.B.CNTS_SEL        = 0;    /* 0:Use CNT as input. 1:Use TBU_TS0 as input. */
    tempGtmTimCfg[0].CH0.CTRL.B.GPR1_SEL        = 3;    /* If EGPR0_SEL =0: 0:use TBU_TS0; 1:TBU_TS1; 2:TBU_TS2; 3:CNT.
                                                           If EGPR0_SEL =1: 0:use ECNT */
    tempGtmTimCfg[0].CH0.CTRL.B.GPR0_SEL        = 3;    /* If EGPR0_SEL =0: 0:use TBU_TS0; 1:TBU_TS1; 2:TBU_TS2; 3:CNTS.
                                                           If EGPR0_SEL =1: 0:use ECNT */
    tempGtmTimCfg[0].CH0.CTRL.B.TBU0_SEL        = 0;    /* 0:Use TBU_TS0[0:23] to store GPR0 and GPR1; 1:Use TBU_TS0[3:26]. */
    tempGtmTimCfg[0].CH0.CTRL.B.CICTRL          = 0;    /* 0:Use TIM_IN(x) as input; 1:Use TIM_IN(x-1) as input. */
    //  tempGtmTimCfg[0].CH0.CTRL.B.ARU_EN          = 0;    /* 1:GPR0, GPR1 values routed to ARU. */
#ifdef GTM_ARU_TIM_TPWM_ENABLED
    tempGtmTimCfg[0].CH0.CTRL.B.ARU_EN          = 1;    /* 1:GPR0, GPR1 values routed to ARU. */
#endif
    tempGtmTimCfg[0].CH0.CTRL.B.OSM             = 0;    /* 0:Continuous mode; 1:One shot mode. */
    tempGtmTimCfg[0].CH0.CTRL.B.TIM_MODE        = 0;    /* 0:TPWM; 1:TPIM; 2:TIEM; 3:TIPM; 4:TBCM; 5:TGPS. */
    tempGtmTimCfg[0].CH0.CTRL.B.TIM_EN          = 0;    /* 0:Channel disable. 1:Channel enable. */

    tempGtmTimCfg[0].CH0.TDUV.B.TCS     = 0;    /* Timeout clock selection, 0~7 CMU_CLK0~7 */
    tempGtmTimCfg[0].CH0.TDUV.B.TOV     = 0x1;  /* Timeout duration */

    tempGtmTimCfg[0].CH0.FLT_FE.B.FLT_FE    = 0;    /* Filter Parameter */
    tempGtmTimCfg[0].CH0.FLT_RE.B.FLT_RE    = 0;    /* Filter Parameter */

    tempGtmTimCfg[0].CH0.IRQ_MODE.B.IRQ_MODE        = 2;    /* 0:Level mode; 1:Pulse mode; 2:Pulse-notify mode; 3:Single-pulse mode. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.NEWVAL_IRQ_EN     = 1;    /* 0:disable; 1:enable. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.ECNTOFL_IRQ_EN    = 0;    /* 0:disable; 1:enable. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.CNTOFL_IRQ_EN     = 1;    /* 0:disable; 1:enable. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.GPROFL_IRQ_EN     = 0;    /* 0:disable; 1:enable. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.TODET_IRQ_EN      = 0;    /* 0:disable; 1:enable. */
    tempGtmTimCfg[0].CH0.IRQ_EN.B.GLITCHDET_IRQ_EN  = 0;    /* 0:disable; 1:enable. */

    //interrupt config
//  // TIM0_1 P02.1  PCLK
//  MODULE_GTM.TIM[0].CH1.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
//  MODULE_GTM.TIM[0].CH1.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
//  MODULE_GTM.TIM[0].CH1.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
//  MODULE_GTM.TIM[0].CH1.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;
//  // TIM0_2 P02.2  VSYN
//  MODULE_GTM.TIM[0].CH2.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
//  MODULE_GTM.TIM[0].CH2.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
//  MODULE_GTM.TIM[0].CH2.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
//  MODULE_GTM.TIM[0].CH2.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;
//  // TIM1_3 P02.3  HREF
//  MODULE_GTM.TIM[1].CH3.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
//  MODULE_GTM.TIM[1].CH3.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
//  MODULE_GTM.TIM[1].CH3.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
//  MODULE_GTM.TIM[1].CH3.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;
    // TIM0_6  P20.0 //干簧管
    MODULE_GTM.TIM[0].CH6.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
    MODULE_GTM.TIM[0].CH6.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
    MODULE_GTM.TIM[0].CH6.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
    MODULE_GTM.TIM[0].CH6.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;

    // TIM2_0  P02.8  ultra cap
    MODULE_GTM.TIM[2].CH0.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
    MODULE_GTM.TIM[2].CH0.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
    MODULE_GTM.TIM[2].CH0.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
    MODULE_GTM.TIM[2].CH0.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;

//  // TIM2_5  P20.9
//  MODULE_GTM.TIM[2].CH5.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
//  MODULE_GTM.TIM[2].CH5.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
//  MODULE_GTM.TIM[2].CH5.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
//  MODULE_GTM.TIM[2].CH5.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;
    // TIM2_6  P
    MODULE_GTM.TIM[2].CH6.TDUV.U        = tempGtmTimCfg[0].CH0.TDUV.U;
    MODULE_GTM.TIM[2].CH6.FLT_FE.U      = tempGtmTimCfg[0].CH0.FLT_FE.U;
    MODULE_GTM.TIM[2].CH6.FLT_RE.U      = tempGtmTimCfg[0].CH0.FLT_RE.U;
    MODULE_GTM.TIM[2].CH6.CTRL.U        = tempGtmTimCfg[0].CH0.CTRL.U;

    //select the input pin
    MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH1SEL = 0x2;
    MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH2SEL = 0x2;
    MODULE_GTM.INOUTSEL.TIM[1].INSEL.B.CH3SEL = 0x2;
    MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH0SEL = 0x2;            //TIN8 P02.8
//  MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH5SEL = 0x5;
    MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH6SEL = 0x6;
    MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH6SEL = 0x7;

//  IfxGtm_TIM2_0_TIN8_P02_8_IN = {IfxGtm_Tim_2, IfxGtm_Tim_Ch_0, {&MODULE_P02, 8}, 2};

    /* TIM channel interrupt configurations */
    /* Enable the NEWVAL event */

    MODULE_GTM.TIM[0].CH6.IRQ_MODE.U    = tempGtmTimCfg[0].CH0.IRQ_MODE.U;
    MODULE_GTM.TIM[0].CH6.IRQ_EN.U      = tempGtmTimCfg[0].CH0.IRQ_EN.U;

    MODULE_GTM.TIM[2].CH0.IRQ_MODE.U    = tempGtmTimCfg[0].CH0.IRQ_MODE.U;
    MODULE_GTM.TIM[2].CH0.IRQ_EN.U      = tempGtmTimCfg[0].CH0.IRQ_EN.U;


    MODULE_GTM.TIM[2].CH6.IRQ_MODE.U    = tempGtmTimCfg[0].CH0.IRQ_MODE.U;
    MODULE_GTM.TIM[2].CH6.IRQ_EN.U      = tempGtmTimCfg[0].CH0.IRQ_EN.U;

    //IO中断优先级P20.0
    TPwmGtmTimChPriority = ISR_PRIORITY_TIM0_CH6;
    /* Get the Service Request Register pointer */
    TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[0][6]);
    /* Initialize the service request register */
    IfxSrc_init(TPwmGtmTimChSrcR, IfxSrc_Tos_cpu0, TPwmGtmTimChPriority);
    /* Enable the TIM channel interrupt */
    IfxSrc_enable(TPwmGtmTimChSrcR);
    IfxCpu_Irq_installInterruptHandler(Tim0Ch6Isr, TPwmGtmTimChPriority);//配置中断函数和中断号

    //超声中断优先级   P02.8
    TPwmGtmTimChPriority = ISR_PRIORITY_TIM2_CH0;
    /* Get the Service Request Register pointer */
    TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[2][0]);
    /* Initialize the service request register */
    IfxSrc_init(TPwmGtmTimChSrcR, IfxSrc_Tos_cpu0, TPwmGtmTimChPriority);
    /* Enable the TIM channel interrupt */
    IfxSrc_enable(TPwmGtmTimChSrcR);
    IfxCpu_Irq_installInterruptHandler(Tim2Ch0Isr, TPwmGtmTimChPriority);//配置中断函数和中断号

    //编码器中断优先级P20.9
    TPwmGtmTimChPriority = ISR_PRIORITY_TIM2_CH6;
    /* Get the Service Request Register pointer */
    TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[2][6]);
    /* Initialize the service request register */
    IfxSrc_init(TPwmGtmTimChSrcR, IfxSrc_Tos_cpu0, TPwmGtmTimChPriority);
    /* Enable the TIM channel interrupt */
    IfxSrc_enable(TPwmGtmTimChSrcR);
    IfxCpu_Irq_installInterruptHandler(Tim2Ch6Isr, TPwmGtmTimChPriority);//配置中断函数和中断号

//  MODULE_GTM.TIM[0].CH1.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 1. */
//  MODULE_GTM.TIM[0].CH2.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 2. */
    MODULE_GTM.TIM[0].CH6.CTRL.B.TIM_EN = 1;
//  MODULE_GTM.TIM[1].CH3.CTRL.B.TIM_EN = 1;    /* Enable TIM1 Channle 3. */

    MODULE_GTM.TIM[2].CH0.CTRL.B.TIM_EN = 1;    /* Enable TIM1 Channle 6. */

    MODULE_GTM.TIM[2].CH5.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 0. */
    MODULE_GTM.TIM[2].CH6.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 1. */
}


void UltrasoundInit(void)        //超声trigger P02.6
{
    IfxPort_setPinMode(Ultratrigger, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(&MODULE_P20, 0, IfxPort_Mode_inputNoPullDevice);
    IfxPort_setPinState(Ultratrigger, IfxPort_State_low);
}

void CoderInit(void)        //编码器CoderA P20.9
{

    IfxPort_setPinMode(CoderA, IfxPort_Mode_inputNoPullDevice);
}
