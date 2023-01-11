#include "../Inc/HUANsic_IT.h"

#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <CompilerTasking.h>
#include <IfxDma.h>
#include <IfxDma_Dma.h>
#include <IfxSrc.h>
#include <IfxSrc_reg.h>
#include <IfxSrc_regdef.h>
#include <IfxScuEru.h>
#include <IfxStm.h>
#include <IfxGtm.h>
#include <IfxGtm_Tim.h>

#include "XinDong_Config.h"
#include "HUANsic_Camera.h"
#include "HUANsic_UART1_BLE.h"
#include "HUANsic_Timers.h"

/* exported function prototype and variables */
extern uint8 g_ImageData[IMAGEH][IMAGEW];

/* exported function prototype and variables (user defined) */
extern void encoder_step(void);
extern void periodicInterrupt_10ms(void);
extern void periodicInterrupt_100ms(void);
extern void periodicInterrupt_1s(void);
extern void reed_triggered(void);
extern void ultrasonic_gotNewValue(void);

/* private variables */
struct {
	Ifx_DMA_CH linkedList[2];
	IfxDma_Dma_Channel chn;
} g_DmaCameraLinkedList;
IfxStm_CompareConfig stmCfg;
uint32 timer_10msTicks;

/* interrupt service routines */
IFX_INTERRUPT(INT_DMA_IRQHandler, INT_DMA_TypeOfService, INT_DMA_PRIORITY){
	camera_incrementFlag();
	if(camera_getFlag() == 2){
		IfxDma_disableChannelTransaction(&MODULE_DMA, INT_EXTI2_PRIORITY);
	}else{
		(IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;// clear requests
		IfxDma_setChannelDestinationAddress(&MODULE_DMA, INT_EXTI2_PRIORITY,
				(void*)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &g_ImageData[60][0]));
		IfxDma_enableChannelTransaction(&MODULE_DMA, INT_EXTI2_PRIORITY);
	}
}

IFX_INTERRUPT(INT_EXTI0_IRQHandler, INT_EXTI0_TypeOfService, INT_EXTI0_PRIORITY){
	IfxPort_togglePin(&MODULE_P33, 8);
	if(camera_getFlag() == 0){
		IfxDma_setChannelDestinationAddress(&MODULE_DMA, INT_EXTI2_PRIORITY,
				(void*)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &g_ImageData[0][0]));
		IfxDma_enableChannelTransaction(&MODULE_DMA, INT_EXTI2_PRIORITY);
	}
}

IFX_INTERRUPT(INT_EXTI1_IRQHandler, INT_EXTI1_TypeOfService, INT_EXTI1_PRIORITY){
	;
}

IFX_INTERRUPT(INT_EXTI2_IRQHandler, INT_EXTI2_TypeOfService, INT_EXTI2_PRIORITY){
	IfxPort_togglePin(&MODULE_P33, 8);
	;// unused; pclk is directed to DMA
}

IFX_INTERRUPT(INT_EXTI3_IRQHandler, INT_EXTI3_TypeOfService, INT_EXTI3_PRIORITY){
	IfxPort_togglePin(&MODULE_P33, 8);
	encoder_step();
}

IFX_INTERRUPT(INT_STM0_IRQHandler, INT_STM0_TypeOfService, INT_STM0_Priority){
	IfxStm_clearCompareFlag(&MODULE_STM0, stmCfg.comparator);
	IfxStm_increaseCompare(&MODULE_STM0, stmCfg.comparator, IfxStm_getFrequency(&MODULE_STM0) / 100);

	timer_10msTicks++;

	periodicInterrupt_10ms();
	if(!(timer_10msTicks % 10)) periodicInterrupt_100ms();
	if(!(timer_10msTicks % 100)) periodicInterrupt_1s();
}

IFX_INTERRUPT(INT_GTM_TIM0CH6_IRQHandler, INT_TIM0CH6_TypeOfService, INT_TIM0CH6_Priority){
	IfxPort_togglePin(&MODULE_P33, 8);
	if(MODULE_GTM.TIM[0].CH6.IRQ_NOTIFY.B.NEWVAL == 1){
		MODULE_GTM.TIM[0].CH6.IRQ_NOTIFY.B.NEWVAL = 1;
		reed_triggered();
	}
}

IFX_INTERRUPT(INT_GTM_TIM2CH0_IRQHandler, INT_TIM2CH0_TypeOfService, INT_TIM2CH0_Priority){
	IfxPort_togglePin(&MODULE_P33, 8);
	if(MODULE_GTM.TIM[2].CH0.IRQ_NOTIFY.B.NEWVAL == 1){
		MODULE_GTM.TIM[2].CH0.IRQ_NOTIFY.B.NEWVAL = 1;
		ultrasonic_setPulseWidth(MODULE_GTM.TIM[2].CH0.GPR0.B.GPR0);
		ultrasonic_gotNewValue();
	}
}

/* methods for the user to call */
void stm0_installInterrupts(void){
	IfxStm_initCompareConfig(&stmCfg);

	stmCfg.triggerPriority = INT_STM0_Priority;
	stmCfg.typeOfService = INT_STM0_TypeOfService;
	stmCfg.ticks = IfxStm_getFrequency(&MODULE_STM0) * 10 / 1000;
	IfxStm_initCompare(&MODULE_STM0, &stmCfg);
	IfxCpu_Irq_installInterruptHandler(INT_STM0_IRQHandler, INT_STM0_Priority);

	timer_10msTicks = 0;
}

void camera_installInterrupts(void){
	/*		VSYNC: INT0		*/
// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ13_P15_5_IN, IfxPort_InputMode_pullDown);

// set trigger edge
	IfxScuEru_disableFallingEdgeDetection(IfxScuEru_InputChannel_0);
	IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_0);// rising edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_0);

// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_0);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_0, IfxScuEru_InputNodePointer_0);

// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_0, IfxScuEru_InputChannel_0, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_0);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_0, IfxScuEru_InterruptGatingPattern_alwaysActive);

// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[0], INT_EXTI0_TypeOfService, INT_EXTI0_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[0]);
	IfxCpu_Irq_installInterruptHandler(INT_EXTI0_IRQHandler, INT_EXTI0_PRIORITY);

	/*		PCLK: INT0		*/
// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ14_P02_1_IN, IfxPort_InputMode_pullUp);

// set trigger edge
	IfxScuEru_disableRisingEdgeDetection(IfxScuEru_InputChannel_2);
	IfxScuEru_enableFallingEdgeDetection(IfxScuEru_InputChannel_2);// falling edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_2);

// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_2);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_2, IfxScuEru_InputNodePointer_2);

// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_2, IfxScuEru_InputChannel_2, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_2);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_2, IfxScuEru_InterruptGatingPattern_alwaysActive);

// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[2], INT_EXTI2_TypeOfService, INT_EXTI2_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[2]);
	IfxCpu_Irq_installInterruptHandler(INT_EXTI2_IRQHandler, INT_EXTI2_PRIORITY);

	/*		DMA		*/
// create module config
	IfxDma_Dma_Config dmaConfig;
	IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);

// initialize module
	IfxDma_Dma dma;
	IfxDma_Dma_initModule(&dma, &dmaConfig);

// initial configuration for all channels
	IfxDma_Dma_ChannelConfig cfg;
	IfxDma_Dma_initChannelConfig(&cfg, &dma);

// set configuration for a specific channel
	cfg.requestMode = IfxDma_ChannelRequestMode_oneTransferPerRequest;
	cfg.moveSize = IfxDma_ChannelMoveSize_8bit;
	cfg.shadowControl = IfxDma_ChannelShadow_none;
	cfg.operationMode = IfxDma_ChannelOperationMode_continuous;
	cfg.hardwareRequestEnabled = TRUE;
	cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &MODULE_P00.IN.U);
	cfg.sourceCircularBufferEnabled = TRUE;
	cfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_none;
	cfg.channelId = INT_EXTI2_PRIORITY;
	cfg.channelInterruptEnabled = TRUE;
	cfg.channelInterruptPriority = INT_DMA_PRIORITY;
	cfg.channelInterruptTypeOfService = INT_DMA_TypeOfService;
	cfg.destinationAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), g_ImageData);
	cfg.shadowAddress = 0;
	cfg.transferCount = 11280;

	/* transfer first transaction set into DMA channel */
	IfxDma_Dma_initChannel(&g_DmaCameraLinkedList.chn, &cfg);
	IfxDma_Dma_initLinkedListEntry((void*)&g_DmaCameraLinkedList.linkedList[0], &cfg);

	// install interrupt handler
	IfxCpu_Irq_installInterruptHandler((void*)INT_DMA_IRQHandler, INT_DMA_PRIORITY);
	(IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

	IfxDma_clearChannelInterrupt(&MODULE_DMA, INT_EXTI2_PRIORITY);
	IfxDma_disableChannelTransaction(&MODULE_DMA, INT_EXTI2_PRIORITY);
}

void encoder_installInterrupt(void){
// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ11_P20_9_IN, IfxPort_InputMode_noPullDevice);

// set trigger edge
	IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_3);// both edge trigger
	IfxScuEru_enableFallingEdgeDetection(IfxScuEru_InputChannel_3);// both edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_3);

// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_3);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_3, IfxScuEru_InputNodePointer_3);

// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_3, IfxScuEru_InputChannel_3, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_3);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_3, IfxScuEru_InterruptGatingPattern_alwaysActive);

// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[3], INT_EXTI3_TypeOfService, INT_EXTI3_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[3]);
	IfxCpu_Irq_installInterruptHandler(INT_EXTI3_IRQHandler, INT_EXTI3_PRIORITY);
}

void ble_installInterrupts(void){
	IfxCpu_Irq_installInterruptHandler(INT_UART1_RX_IRQHandler, 130);
	IfxCpu_Irq_installInterruptHandler(INT_UART1_TX_IRQHandler, 131);
	IfxCpu_Irq_installInterruptHandler(INT_UART1_ER_IRQHandler, 132);
}

void ultrasonic_installInterrupt(void){
	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[2].CH0, IfxGtm_IrqMode_pulseNotify);
	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[2].CH0, 1, 1, 0, 0);
	IfxSrc_init(&(MODULE_SRC.GTM.GTM[0].TIM[2][0]), INT_TIM2CH0_TypeOfService, INT_TIM2CH0_Priority);
	IfxSrc_enable(&(MODULE_SRC.GTM.GTM[0].TIM[2][0]));
	IfxCpu_Irq_installInterruptHandler(INT_GTM_TIM2CH0_IRQHandler, INT_TIM2CH0_Priority);
}

void reed_installInterrupt(void){
	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[0].CH6, IfxGtm_IrqMode_pulseNotify);
	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[0].CH6, 1, 1, 0, 0);
	IfxSrc_init(&(MODULE_SRC.GTM.GTM[0].TIM[0][6]), INT_TIM0CH6_TypeOfService, INT_TIM0CH6_Priority);
	IfxSrc_enable(&(MODULE_SRC.GTM.GTM[0].TIM[0][6]));
	IfxCpu_Irq_installInterruptHandler(INT_GTM_TIM0CH6_IRQHandler, INT_TIM0CH6_Priority);
}

// not introduced to users yet
void tim_installInterrupts(void){
	Ifx_SRC_SRCR *TPwmGtmTimChSrcR;

	/* TIM channel interrupt configurations */
// Enable the NEWVAL and CNTOFLW events
//	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[0].CH1, IfxGtm_IrqMode_pulseNotify);
//	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[0].CH1, 1, 1, 0, 0);
//	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[0].CH2, IfxGtm_IrqMode_pulseNotify);
//	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[0].CH2, 1, 1, 0, 0);
	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[0].CH6, IfxGtm_IrqMode_pulseNotify);
	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[0].CH6, 1, 1, 0, 0);
//	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[1].CH3, IfxGtm_IrqMode_pulseNotify);
//	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[1].CH3, 1, 1, 0, 0);
	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[2].CH0, IfxGtm_IrqMode_pulseNotify);
	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[2].CH0, 1, 1, 0, 0);
//	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[2].CH5, IfxGtm_IrqMode_pulseNotify);
//	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[2].CH5, 1, 1, 0, 0);
//	IfxGtm_Tim_Ch_setNotificationMode(&MODULE_GTM.TIM[2].CH6, IfxGtm_IrqMode_pulseNotify);
//	IfxGtm_Tim_Ch_setChannelNotification(&MODULE_GTM.TIM[2].CH6, 1, 1, 0, 0);

//	/* Get the Service Request Register pointer */
//	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[0][1]);
//	/* Initialize the service request register */
//	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM0CH1_TypeOfService, INT_TIM0CH1_Priority);		// use CPU1
//	/* Enable the TIM channel interrupt */
//	IfxSrc_enable(TPwmGtmTimChSrcR);

//	/* Get the Service Request Register pointer */
//	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[0][2]);
//	/* Initialize the service request register */
//	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM0CH2_TypeOfService, INT_TIM0CH2_Priority);		// use CPU1
//	/* Enable the TIM channel interrupt */
//	IfxSrc_enable(TPwmGtmTimChSrcR);

	/* Get the Service Request Register pointer */
	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[0][6]);
	/* Initialize the service request register */
	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM0CH6_TypeOfService, INT_TIM0CH6_Priority);
	/* Enable the TIM channel interrupt */
	IfxSrc_enable(TPwmGtmTimChSrcR);

//	/* Get the Service Request Register pointer */
//	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[1][3]);
//	/* Initialize the service request register */
//	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM1CH3_TypeOfService, INT_TIM1CH3_Priority);  // use CPU1
//	/* Enable the TIM channel interrupt */
//	IfxSrc_enable(TPwmGtmTimChSrcR);

	/* Get the Service Request Register pointer */
	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[2][0]);
	/* Initialize the service request register */
	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM2CH0_TypeOfService, INT_TIM2CH0_Priority);
	/* Enable the TIM channel interrupt */
	IfxSrc_enable(TPwmGtmTimChSrcR);

//	/* Get the Service Request Register pointer */
//	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[2][5]);
//	/* Initialize the service request register */
//	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM2CH5_TypeOfService, INT_TIM2CH5_Priority);
//	/* Enable the TIM channel interrupt */
//	IfxSrc_enable(TPwmGtmTimChSrcR);

//	/* Get the Service Request Register pointer */
//	TPwmGtmTimChSrcR = &(MODULE_SRC.GTM.GTM[0].TIM[2][6]);
//	/* Initialize the service request register */
//	IfxSrc_init(TPwmGtmTimChSrcR, INT_TIM2CH6_TypeOfService, INT_TIM2CH6_Priority);
//	/* Enable the TIM channel interrupt */
//	IfxSrc_enable(TPwmGtmTimChSrcR);
}

void dma_CameraStop(unsigned long channel){
	IfxDma_disableChannelTransaction(&MODULE_DMA, channel);
	camera_resetFlag();
}
