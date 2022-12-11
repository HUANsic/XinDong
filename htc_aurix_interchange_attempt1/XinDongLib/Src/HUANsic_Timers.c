#include "../Inc/HUANsic_Timers.h"

#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <IfxGtm.h>
#include <IfxGtm_Tim.h>
#include <IfxGtm_Tom.h>
#include <IfxGtm_Cmu.h>
#include <IfxGtm_reg.h>
#include <IfxGtm_PinMap.h>

#include "XinDong_Config.h"

/* STM */
void delay_us(uint32 time){
	IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0, time));
}

void delay_ms(uint32 time){
	IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMilliseconds(&MODULE_STM0, time));
}

/* GTM */
void gtm_init(void){
	uint32 dummy;
	uint16 password;
	password = IfxScuWdt_getCpuWatchdogPassword();

	IfxScuWdt_clearCpuEndinit(password);
	GTM_CLC.U = 0x00000000; /* enable GTM module */
	dummy = GTM_CLC.U; /* need to read back before accessing SFRs */
	IfxScuWdt_setCpuEndinit(password);

	IfxGtm_Cmu_setGclkFrequency(&MODULE_GTM, 100000000); /* Set global clock to 100MHz */

	IfxGtm_Cmu_enableClocks(&MODULE_GTM, 0x00800000); /* Enable the fixed clock */

	// Set GTM CMU_CLKx clock frequency = 10MHz
	IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, 10000000);

	//Enable Configurable Clock 0
	IfxGtm_Cmu_enableClocks(&MODULE_GTM, (0x00000002 << (IfxGtm_Cmu_Clk_0 * 2)));

//	if(!IfxGtm_isEnabled(&MODULE_GTM)) IfxGtm_enable(&MODULE_GTM);
//	while(!IfxGtm_isEnabled(&MODULE_GTM)){
//		__debug();
//	}
}

/* TIM */
uint32 ultrasonic_PulseWidth;    //the duty of captured PWM signal
uint8 ultrasonic_isNew = 0;

void tim_init(void){
	/* TBU Channel configuration */
// No api so far
	Ifx_GTM_TBU tempGtmTbuCfg;
	tempGtmTbuCfg.CH0_CTRL.B.CH_CLK_SRC = 0;	//0: CMU_CLK0; ...; 7: CMU_CLK7.
	tempGtmTbuCfg.CH0_CTRL.B.LOW_RES = 0;		//1: upper 24 bits(0:23). 0: lower 24 bits(3:26).
	tempGtmTbuCfg.CH1_CTRL.B.CH_CLK_SRC = 0;	//0: CMU_CLK0; ...; 7: CMU_CLK7.
	tempGtmTbuCfg.CH1_CTRL.B.CH_MODE = 0;		//1: forward and backward counter. 0: free running counter mode.
	tempGtmTbuCfg.CH2_CTRL.B.CH_CLK_SRC = 0;	//0: CMU_CLK0; ...; 7: CMU_CLK7.
	tempGtmTbuCfg.CH2_CTRL.B.CH_MODE = 0;		//1: forward and backward counter. 0: free running counter mode.

// Assign to Registers
	MODULE_GTM.TBU.CH0_CTRL.U = tempGtmTbuCfg.CH0_CTRL.U;
	MODULE_GTM.TBU.CH1_CTRL.U = tempGtmTbuCfg.CH1_CTRL.U;
	MODULE_GTM.TBU.CH2_CTRL.U = tempGtmTbuCfg.CH2_CTRL.U;

// Enable TBU Channel 0
	IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_0);
	IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_1);
	IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_2);

	/* TIM Channel Configuration */
	Ifx_GTM_TIM_CH tempGtmTimChCfg;
//	an example , in order to set the other TIMs
	tempGtmTimChCfg.CTRL.B.TOCTRL = 2; /* 0:Timeout feature disabled. 1:Timeout for both edges. 2:Timeout for rising edge. 3: Timeout for falling edge. */
	tempGtmTimChCfg.CTRL.B.EGPR1_SEL = 0; /* Extension of GPR1_SEL bit field. */
	tempGtmTimChCfg.CTRL.B.EGPR0_SEL = 0; /* Extension of GPR0_SEL bit field. */
	tempGtmTimChCfg.CTRL.B.FR_ECNT_OFL = 0; /* Extended Edge counter overflow behavior
	 0: Overflow will be signalled on ECNT bit width = 8
	 1: Overflow will be signalled on EECNT bit width (full range) */
	tempGtmTimChCfg.CTRL.B.CLK_SEL = 0; /* 0...7: CMU_CLK0 ... CMU_CLK7. */
	tempGtmTimChCfg.CTRL.B.FLT_CTR_FE = 0; /* 0:Up/down counter; 1:Hold counter. */
	tempGtmTimChCfg.CTRL.B.FLT_MODE_FE = 0; /* 0:Immediate; 1:De-glitch mode. */
	tempGtmTimChCfg.CTRL.B.FLT_CTR_RE = 0; /* 0:Up/down counter; 1:Hold counter. */
	tempGtmTimChCfg.CTRL.B.FLT_MODE_RE = 0; /* 0:Immediate; 1:De-glitch mode. */
	tempGtmTimChCfg.CTRL.B.EXT_CAP_EN = 0; /* 0:External capture disabled; 1:External capture enabled. */
	tempGtmTimChCfg.CTRL.B.FLT_CNT_FRQ = 0; /* 0:Filter counts with CMU_CLK0; 1:CMU_CLK1; 2:CMU_CLK6; 3:CMU_CLK7. */
	tempGtmTimChCfg.CTRL.B.ECNT_RESET = 0; /* 0:ECNT in wrap round mode; 1:ECNT counter is reset with periodic sampling */
	tempGtmTimChCfg.CTRL.B.ISL = 0; /* 0:Use DSL for active level; 1:ignore DSL and treat both edges
	 [Note: This bit is only applicable in Input Event mode (TIEM and TIPM)]*/
	tempGtmTimChCfg.CTRL.B.DSL = 1; /* 0:Falling edge(low level measurement); 1:Rising edge(high level measurement) */
	tempGtmTimChCfg.CTRL.B.CNTS_SEL = 0; /* 0:Use CNT as input. 1:Use TBU_TS0 as input. */
	tempGtmTimChCfg.CTRL.B.GPR1_SEL = 3; /* If EGPR0_SEL =0: 0:use TBU_TS0; 1:TBU_TS1; 2:TBU_TS2; 3:CNT.
	 If EGPR0_SEL =1: 0:use ECNT */
	tempGtmTimChCfg.CTRL.B.GPR0_SEL = 3; /* If EGPR0_SEL =0: 0:use TBU_TS0; 1:TBU_TS1; 2:TBU_TS2; 3:CNTS.
	 If EGPR0_SEL =1: 0:use ECNT */
	tempGtmTimChCfg.CTRL.B.TBU0_SEL = 0; /* 0:Use TBU_TS0[0:23] to store GPR0 and GPR1; 1:Use TBU_TS0[3:26]. */
	tempGtmTimChCfg.CTRL.B.CICTRL = 0; /* 0:Use TIM_IN(x) as input; 1:Use TIM_IN(x-1) as input. */
	tempGtmTimChCfg.CTRL.B.OSM = 0; /* 0:Continuous mode; 1:One shot mode. */
	tempGtmTimChCfg.CTRL.B.TIM_MODE = 0; /* 0:TPWM; 1:TPIM; 2:TIEM; 3:TIPM; 4:TBCM; 5:TGPS. */
	tempGtmTimChCfg.CTRL.B.TIM_EN = 0; /* 0:Channel disable. 1:Channel enable. */

	tempGtmTimChCfg.TDUV.B.TCS = 0; /* Timeout clock selection, 0~7 CMU_CLK0~7 */
	tempGtmTimChCfg.TDUV.B.TOV = 0x01; /* Timeout duration */

	tempGtmTimChCfg.FLT_FE.B.FLT_FE = 0; /* Filter Parameter */
	tempGtmTimChCfg.FLT_RE.B.FLT_RE = 0; /* Filter Parameter */

	tempGtmTimChCfg.IRQ_MODE.B.IRQ_MODE = 2; /* 0:Level mode; 1:Pulse mode; 2:Pulse-notify mode; 3:Single-pulse mode. */

// interrupt configuration
//	// TIM0_1 P02.1  PCLK
//	MODULE_GTM.TIM[0].CH1.TDUV.U = 1;
//	MODULE_GTM.TIM[0].CH1.FLT_FE.U = 0;
//	MODULE_GTM.TIM[0].CH1.FLT_RE.U = 0;
//	MODULE_GTM.TIM[0].CH1.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);
//	// TIM0_2 P02.2  VSYN
//	MODULE_GTM.TIM[0].CH2.TDUV.U = 1;
//	MODULE_GTM.TIM[0].CH2.FLT_FE.U = 0;
//	MODULE_GTM.TIM[0].CH2.FLT_RE.U = 0;
//	MODULE_GTM.TIM[0].CH2.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);
// TIM0_6  P20.0  reed switch
	MODULE_GTM.TIM[0].CH6.TDUV.U = 1;
	MODULE_GTM.TIM[0].CH6.FLT_FE.U = 0;
	MODULE_GTM.TIM[0].CH6.FLT_RE.U = 0;
	MODULE_GTM.TIM[0].CH6.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);  // TOCTRL, DSL, GPR1_SEL, GPR0_SEL
//	// TIM1_3 P02.3  HREF
//	MODULE_GTM.TIM[1].CH3.TDUV.U = 1;
//	MODULE_GTM.TIM[1].CH3.FLT_FE.U = 0;
//	MODULE_GTM.TIM[1].CH3.FLT_RE.U = 0;
//	MODULE_GTM.TIM[1].CH3.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);
			// TIM2_0  P02.8  ultrasonic
	MODULE_GTM.TIM[2].CH0.TDUV.U = 1;
	MODULE_GTM.TIM[2].CH0.FLT_FE.U = 0;
	MODULE_GTM.TIM[2].CH0.FLT_RE.U = 0;
	MODULE_GTM.TIM[2].CH0.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);  // TOCTRL, DSL, GPR1_SEL, GPR0_SEL
//	// TIM2_5  P20.9  Encoder A
//	MODULE_GTM.TIM[2].CH5.TDUV.U = 1;
//	MODULE_GTM.TIM[2].CH5.FLT_FE.U = tempGtmTimChCfg.FLT_FE.U;
//	MODULE_GTM.TIM[2].CH5.FLT_RE.U = tempGtmTimChCfg.FLT_RE.U;
//	MODULE_GTM.TIM[2].CH5.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);
//	// TIM2_6  P20_10  Encoder B
//	MODULE_GTM.TIM[2].CH6.TDUV.U = 1;
//	MODULE_GTM.TIM[2].CH6.FLT_FE.U = 0;
//	MODULE_GTM.TIM[2].CH6.FLT_RE.U = 0;
//	MODULE_GTM.TIM[2].CH6.CTRL.U = (0x02 << 30) | (0x01 << 13) | (0x03 << 10) | (0x03 << 8);

//select the input pin
//	MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH1SEL = 0x2;
//	MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH2SEL = 0x2;
	MODULE_GTM.INOUTSEL.TIM[0].INSEL.B.CH6SEL = 0x7;	// reed switch
//	MODULE_GTM.INOUTSEL.TIM[1].INSEL.B.CH3SEL = 0x2;
	MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH0SEL = 0x2;	// ultrasonic
//	MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH5SEL = 0x5;
//	MODULE_GTM.INOUTSEL.TIM[2].INSEL.B.CH6SEL = 0x6;

// enable channels
//	MODULE_GTM.TIM[0].CH1.CTRL.B.TIM_EN	= 1;
//	MODULE_GTM.TIM[0].CH2.CTRL.B.TIM_EN	= 1;
	MODULE_GTM.TIM[0].CH6.CTRL.B.TIM_EN = 1;
//	MODULE_GTM.TIM[1].CH3.CTRL.B.TIM_EN	= 1;
	MODULE_GTM.TIM[2].CH0.CTRL.B.TIM_EN = 1;
//	MODULE_GTM.TIM[2].CH5.CTRL.B.TIM_EN	= 1;
//	MODULE_GTM.TIM[2].CH6.CTRL.B.TIM_EN	= 1;
}

uint8 ultrasonic_hasNewValue(void){
	return ultrasonic_isNew;
}

void ultrasonic_setPulseWidth(uint32 width){
	ultrasonic_PulseWidth = width;
	ultrasonic_isNew = 1;
}

uint32 ultrasonic_getPulseWidth(void){
	ultrasonic_isNew = 0;
	return ultrasonic_PulseWidth;
}

/* TOM */
float32 servo_PWM_Period;
float32 motor_PWM_Period;

void tom_initServo(void){
	Ifx_GTM_TOM_TGC *channel = IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[1], 1);

	/*Software reset for TOM channels */
	IfxGtm_Tom_Tgc_resetChannels(channel, 0x80);

	/* Initialize the timer part */
	IfxGtm_Tom_Ch_setSignalLevel(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, Ifx_ActiveState_high); /* SL : high level */
	IfxGtm_Tom_Ch_setClockSource(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, IfxGtm_Tom_Ch_ClkSrc_cmuFxclk2); /* CLK_SRC_SR : div by 1  */
	IfxGtm_Tom_Ch_setResetSource(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, IfxGtm_Tom_Ch_ResetEvent_onCm0); /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */

	/* Write to SR0 and SR1 shadow registers for used channels */
	servo_PWM_Period = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Fxclk_2, TRUE) / 50;		// fixed to 50Hz
	IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, (uint32)servo_PWM_Period,
			(uint32)(servo_PWM_Period * (7.5 + SERVO_DC_OFFSET) / 100));		// 7.5% represents 1.5ms

	/* Configure GPIO ports for TOM outputs */
	IfxGtm_PinMap_setTomTout(&IfxGtm_TOM1_15_TOUT67_P20_11_OUT, IfxPort_OutputMode_pushPull,
			IfxPort_PadDriver_cmosAutomotiveSpeed1);

	/* OUTEN : enable channel output on an update trigger for *all* channels */
	IfxGtm_Tom_Tgc_enableChannelsOutput(channel, 0x80, 0, FALSE);
	/* ENDIS : enable channel on an update trigger for all channels  */
	IfxGtm_Tom_Tgc_enableChannels(channel, 0x80, 0, FALSE);

	/* FUPD : Enable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
	IfxGtm_Tom_Tgc_setChannelsForceUpdate(channel, 0x80, 0, 0x00, 0);
	/* HOST_TRIG : Raise the trigger for the channel enable/disable settings, output enable settings, and force update event */
	IfxGtm_Tom_Tgc_trigger(channel);
	/* FUPD : Disable Force update of operation registers for TOM channel x, Transfer the shadow registers  */
	IfxGtm_Tom_Tgc_setChannelsForceUpdate(channel, 0, 0x80, 0x00, 0);

	/* Configure the interrupt for the TOM channel */
//	IfxGtm_IrqMode irqMode = IfxGtm_IrqMode_pulseNotify;
//	IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, irqMode, TRUE, FALSE);
	IfxGtm_IrqMode irqMode = IfxGtm_IrqMode_pulseNotify;
	IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15, irqMode, FALSE, FALSE);  // don't generate interrupt
}

void tom_initMotor(void){
	/*Software reset for TOM channels */
	IfxGtm_Tom_Tgc_resetChannels(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08 | 0x04);

	/* Initialize the timer part */
	IfxGtm_Tom_Ch_setSignalLevel(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, Ifx_ActiveState_low); /* SL : high level */
	IfxGtm_Tom_Ch_setClockSource(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1); /* CLK_SRC_SR : div by 1  */
	IfxGtm_Tom_Ch_setResetSource(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, IfxGtm_Tom_Ch_ResetEvent_onCm0); /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */
	IfxGtm_Tom_Ch_setSignalLevel(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, Ifx_ActiveState_low); /* SL : high level */
	IfxGtm_Tom_Ch_setClockSource(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1); /* CLK_SRC_SR : div by 1  */
	IfxGtm_Tom_Ch_setResetSource(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, IfxGtm_Tom_Ch_ResetEvent_onCm0); /* RST_CCU0 : reset CN0 on local CCU0 CM0 match */

	/*Write to SR0 and SR1 shadow registers for used channels */
	motor_PWM_Period = IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Fxclk_1, TRUE) / PWM_FREQ_MOTOR;
	IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, motor_PWM_Period, 0);  // set DC to 0 by default
	IfxGtm_Tom_Ch_setCompareShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, motor_PWM_Period, 0);

	/*configure GPIO ports for TOM outputs */
	IfxGtm_PinMap_setTomTout(&IfxGtm_TOM0_2_TOUT104_P10_2_OUT, IfxPort_OutputMode_pushPull,
			IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxGtm_PinMap_setTomTout(&IfxGtm_TOM0_3_TOUT105_P10_3_OUT, IfxPort_OutputMode_pushPull,
			IfxPort_PadDriver_cmosAutomotiveSpeed1);

	/* OUTEN : enable channel output on an update trigger for *all* channels */
	IfxGtm_Tom_Tgc_enableChannelsOutput(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08 | 0x04, 0, FALSE);
	/* ENDIS : enable channel on an update trigger for all channels  */
	IfxGtm_Tom_Tgc_enableChannels(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08 | 0x04, 0, FALSE);

	/* FUPD : Enable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
	IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0x08 | 0x04, 0, 0x00, 0);
	/* HOST_TRIG : Raise the trigger for the channel enable/disable settings, output enable settings, and force update event */
	IfxGtm_Tom_Tgc_trigger(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0));
	/* FUPD : Disable Force update of operation registers for TOM channel x,Transfer the shadow registers  */
	IfxGtm_Tom_Tgc_setChannelsForceUpdate(IfxGtm_Tom_Ch_getTgcPointer(&MODULE_GTM.TOM[0], 0), 0, 0x08 | 0x04, 0x00, 0);

	/* Configure the interrupt for the TOM channel */
	IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, IfxGtm_IrqMode_pulseNotify, FALSE, FALSE);
	IfxGtm_Tom_Ch_setNotification(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, IfxGtm_IrqMode_pulseNotify, FALSE, FALSE);
}

void tom_setServo(float32 angle){
	angle = angle > 90 ? 90 : (angle < -90 ? -90 : angle);
	angle /= 36;		// map [-90,90]deg to [-2.5,2.5]%dc

	IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[1], IfxGtm_Tom_Ch_15,
			(uint32)(servo_PWM_Period * (7.5 + angle + SERVO_DC_OFFSET) / 100));
}

void tom_setMotor(float32 power){
	power = power > 1 ? 1 : (power < -1 ? -1 : power);
	if(power > 0){
		IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, (uint32)(motor_PWM_Period * power));
		IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, 0);
	}else{
		IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, 0);
		IfxGtm_Tom_Ch_setCompareOneShadow(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, (uint32)(motor_PWM_Period * -power));
	}
}
