#include "../Inc/HUANsic_UART1_BLE.h"
#include "../Inc/XinDong_Config.h"
#include "../Inc/CameraShenyan.h"
;
uint8 uart1_txBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];		// idk why add the tail, but here it is
uint8 uart1_rxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

void USER_uart1_rx_irq(void);

void uart1_init(){
	IfxAsclin_Tx_Out *IfxAsclin_Tx = &IfxAsclin1_TX_P11_12_OUT;
	IfxAsclin_Rx_In *IfxAsclin_Rx = &IfxAsclin1_RXE_P11_10_IN;

	// set up asclin config structure
	IfxAsclin_Asc_Config ascConfig;
	IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);		// give default values
	// add parameters that mostly is already set up just in case
	ascConfig.baudrate.baudrate = (float)115200;
	ascConfig.frame.frameMode = IfxAsclin_FrameMode_asc;
	ascConfig.frame.dataLength = IfxAsclin_DataLength_8;
	ascConfig.frame.stopBit = IfxAsclin_StopBit_1;
	ascConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
	ascConfig.frame.parityBit = FALSE;

	// set up interrupt parameters (can't put outside)
	ascConfig.interrupt.rxPriority = INT_UART1_RX_Priority;
	ascConfig.interrupt.txPriority = INT_UART1_TX_Priority;
	ascConfig.interrupt.erPriority = INT_UART1_ERR_Priority;
	ascConfig.interrupt.typeOfService = INT_UART1_TypeOfService;

	// set up buffer and limits
	ascConfig.txBuffer = uart1_txBuffer;
	ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
	ascConfig.rxBuffer = uart1_rxBuffer;
	ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

	// set up pins
	IfxAsclin_Asc_Pins pins = {
	NULL, IfxPort_InputMode_pullUp, /* CTS pin not used */
	IfxAsclin_Rx, IfxPort_InputMode_pullUp, /* Rx pin */
	NULL, IfxPort_OutputMode_pushPull, /* RTS pin not used */
	IfxAsclin_Tx, IfxPort_OutputMode_pushPull, /* Tx pin */
	IfxPort_PadDriver_cmosAutomotiveSpeed1};
	ascConfig.pins = &pins;

	// execute config method
	IfxAsclin_Asc_initModule(&MODULE_ASCLIN1, &ascConfig);
}

void uart1_dumpImage(){
	uint16 j, i;
	uint8 count = IMAGEW;
	uint8 **ImageData = CAMERA_GetArray();

	IfxAsclin_Asc_blockingWrite(&MODULE_ASCLIN1, 0xfe);
	IfxAsclin_Asc_blockingWrite(&MODULE_ASCLIN1, 0xef);

	for(i = 0; i < IMAGEH; i++){
		for(j = 0; j < IMAGEW; j++)
			if(ImageData[i][j] == 0xfe) ImageData[i][j] = 0xff;

		IfxAsclin_Asc_write(&MODULE_ASCLIN1, &ImageData[i][0], &count, TIME_INFINITE);  // just make it continuous instead of one by one
	}

	IfxAsclin_Asc_blockingWrite(&MODULE_ASCLIN1, 0xef);
	IfxAsclin_Asc_blockingWrite(&MODULE_ASCLIN1, 0xfe);
}

void UART1_TX_IRQHandler(void){
	IfxAsclin_Asc_isrTransmit(&MODULE_ASCLIN1);		// let the default method clear the flags
}

void UART1_ER_IRQHandler(void){
	IfxAsclin_Asc_isrError(&MODULE_ASCLIN1);		// let the default method clear the flags
}

void UART1_RX_IRQHandler(void){
	USER_uart1_rx_irq();
	// if no user function is defined, call the default one
	IfxAsclin_Asc_isrReceive(&MODULE_ASCLIN1);
}

__weak__ void USER_uart1_rx_irq(void){
	;
}
