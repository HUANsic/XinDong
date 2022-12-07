#include "../Inc/HUANsic_IT.h"
#include "IfxDma.h"

// DMA interrupt
IFX_INTERRUPT(DMA_IRQHandler, DMA_TypeOfService, DMA_PRIORITY);

// external interrupts
IFX_INTERRUPT(PIN_INT0_IRQHandler, PIN_INT0_TypeOfService, PIN_INT0_PRIORITY);
IFX_INTERRUPT(PIN_INT1_IRQHandler, PIN_INT1_TypeOfService, PIN_INT1_PRIORITY);
IFX_INTERRUPT(PIN_INT2_IRQHandler, PIN_INT2_TypeOfService, PIN_INT2_PRIORITY);
IFX_INTERRUPT(PIN_INT3_IRQHandler, PIN_INT3_TypeOfService, PIN_INT3_PRIORITY);

// exported function prototype (uer defined)
extern void encoder_irq();

IfxDma_Dma_Channel camera_dma_channel;

void DMA_IRQHandler(void){
	Camera_Flag++;
	if(Camera_Flag == 2){
		IfxDma_disableChannelTransaction(&MODULE_DMA, PIN_INT2_PRIORITY);
	}else{
		(IfxDma_Dma_getSrcPointer(&camera_dma_channel))->B.CLRR = 1;			// clear requests

		IfxDma_setChannelDestinationAddress(&MODULE_DMA, PIN_INT2_PRIORITY,
				(void*)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &Image_Data[60][0]));
	}
}

void PIN_INT0_IRQHandler(){
	if(Camera_Flag == 0){
		IfxDma_setChannelDestinationAddress(&MODULE_DMA, PIN_INT2_PRIORITY,
				(void*)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), Image_Data));

		IfxDma_enableChannelTransaction(&MODULE_DMA, PIN_INT2_PRIORITY);
	}
}

void PIN_INT1_IRQHandler(){
	;
}

void PIN_INT2_IRQHandler(){
	;
}

void PIN_INT3_IRQHandler(){
	encoder_irq();
}

/*
 * 		methods for the user to call
 */

void camera_installInterrupts(){
	/*		VSYNC: INT0		*/
	// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ13_P15_5_IN, IfxPort_InputMode_pullDown);

	// set trigger edge
	IfxScuEru_disableFallingEdgeDetection(IfxScuEru_InputChannel_0);
	IfxScuEru_disableRisingEdgeDetection(IfxScuEru_InputChannel_0);
	IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_0);		// rising edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_0);

	// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_0);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_0, IfxScuEru_InputNodePointer_0);

	// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_0, IfxScuEru_InputChannel_0, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_0);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_0, IfxScuEru_InterruptGatingPattern_alwaysActive);

	// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[0], PIN_INT0_TypeOfService, PIN_INT0_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[0]);
	IfxCpu_Irq_installInterruptHandler(PIN_INT0_IRQHandler, PIN_INT0_PRIORITY);

	/*		PCLK: INT0		*/
	// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ14_P02_1_IN, IfxPort_InputMode_pullUp);

	// set trigger edge
	IfxScuEru_disableFallingEdgeDetection(IfxScuEru_InputChannel_2);
	IfxScuEru_disableRisingEdgeDetection(IfxScuEru_InputChannel_2);
	IfxScuEru_enableFallingEdgeDetection(IfxScuEru_InputChannel_2);		// falling edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_2);

	// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_2);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_2, IfxScuEru_InputNodePointer_2);

	// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_2, IfxScuEru_InputChannel_2, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_2);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_2, IfxScuEru_InterruptGatingPattern_alwaysActive);

	// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[2], PIN_INT2_TypeOfService, PIN_INT2_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[2]);
	IfxCpu_Irq_installInterruptHandler(PIN_INT2_IRQHandler, PIN_INT2_PRIORITY);

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
	cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (&(MODULE_P00.IN.U)));
	cfg.sourceCircularBufferEnabled = TRUE;
	cfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_none;
	cfg.channelId = PIN_INT2_PRIORITY;
	cfg.channelInterruptEnabled = TRUE;
	cfg.channelInterruptPriority = DMA_PRIORITY;
	cfg.channelInterruptTypeOfService = DMA_TypeOfService;
	cfg.destinationAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), Image_Data);
	cfg.shadowAddress = 0;
	cfg.transferCount = 11280;

	/* transfer first transaction set into DMA channel */
	IfxDma_Dma_initChannel(&camera_dma_channel, &cfg);

	// transfer into linked list storage (not used for now)
	// IfxDma_Dma_initLinkedListEntry(&dma_channel, &cfg);		// equivalent to next line
	// IfxDma_Dma_configureTransactionSet(&dma_channel, &cfg)

	// clear flag and disable for now
	IfxDma_Dma_getSrcPointer(&camera_dma_channel)->B.CLRR = 1;		// clear request
	IfxDma_clearChannelInterrupt(&MODULE_DMA, PIN_INT2_PRIORITY);
	IfxDma_disableChannelTransaction(&MODULE_DMA, PIN_INT2_PRIORITY);

	// install interrupt handler
	IfxCpu_Irq_installInterruptHandler(DMA_IRQHandler, DMA_PRIORITY);
}

void encoder_installInterrupt(){
	// set pin mode
	IfxScuEru_initReqPin(&IfxScu_REQ11_P20_9_IN, IfxPort_InputMode_pullDown);

	// set trigger edge
	IfxScuEru_disableFallingEdgeDetection(IfxScuEru_InputChannel_3);
	IfxScuEru_disableRisingEdgeDetection(IfxScuEru_InputChannel_3);
	IfxScuEru_enableRisingEdgeDetection(IfxScuEru_InputChannel_3);		// rising edge trigger
	IfxScuEru_enableAutoClear(IfxScuEru_InputChannel_3);

	// connecting matrix
	IfxScuEru_enableTriggerPulse(IfxScuEru_InputChannel_3);
	IfxScuEru_connectTrigger(IfxScuEru_InputChannel_3, IfxScuEru_InputNodePointer_3);

	// output channels
	IfxScuEru_setFlagPatternDetection(IfxScuEru_OutputChannel_3, IfxScuEru_InputChannel_3, 0);
	IfxScuEru_disablePatternDetectionTrigger(IfxScuEru_OutputChannel_3);
	IfxScuEru_setInterruptGatingPattern(IfxScuEru_OutputChannel_3, IfxScuEru_InterruptGatingPattern_alwaysActive);

	// install interrupt
	IfxSrc_init(&MODULE_SRC.SCU.SCU.ERU[3], PIN_INT3_TypeOfService, PIN_INT3_PRIORITY);
	IfxSrc_enable(&MODULE_SRC.SCU.SCU.ERU[3]);
	IfxCpu_Irq_installInterruptHandler(PIN_INT3_IRQHandler, PIN_INT3_PRIORITY);
}

void ble_installInterrupts(){
	IfxCpu_Irq_installInterruptHandler(UART1_RX_IRQHandler, 130);
	IfxCpu_Irq_installInterruptHandler(UART1_TX_IRQHandler, 131);
	IfxCpu_Irq_installInterruptHandler(UART1_ER_IRQHandler, 132);
}

void DMA_CameraStop(unsigned long channel){
	IfxDma_disableChannelTransaction(&MODULE_DMA, channel);
	Camera_Flag = 0;
}
