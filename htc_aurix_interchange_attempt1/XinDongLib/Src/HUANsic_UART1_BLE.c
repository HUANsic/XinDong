#include "../Inc/HUANsic_UART1_BLE.h"

#include <IfxAsclin_Asc.h>

#include "../Inc/XinDong_Config.h"
#include "../Inc/CameraShenyan.h"

#if XINDONG_USE_BLUETOOTH
#define UART1_BAUD 9600			// connected to bluetooth module
#else
#define UART1_BAUD 115200		// used as serial port to send image to computer
#endif		// XINDONG_USE_BLUETOOTH

/* private variables */
uint8 uart1_txBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];		// idk why add the tail, but here it is
uint8 uart1_rxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
IfxAsclin_Asc uart1_ascmodule;
extern uint8 g_ImageData[IMAGEH][IMAGEW];

/* methods for the user to call */
void uart1_init(void){
	IfxAsclin_Tx_Out *IfxAsclin_Tx = &IfxAsclin1_TX_P11_12_OUT;
	IfxAsclin_Rx_In *IfxAsclin_Rx = &IfxAsclin1_RXE_P11_10_IN;

	// set up asclin config structure
	IfxAsclin_Asc_Config ascConfig;
	IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);		// give default values
	// add parameters that mostly is already set up just in case
	ascConfig.baudrate.baudrate = (float32)UART1_BAUD;
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
	IfxAsclin_Asc_Pins pins = {0, IfxPort_InputMode_pullUp, /* CTS pin not used */
	IfxAsclin_Rx, IfxPort_InputMode_pullUp, /* Rx pin */
	0, IfxPort_OutputMode_pushPull, /* RTS pin not used */
	IfxAsclin_Tx, IfxPort_OutputMode_pushPull, /* Tx pin */
	IfxPort_PadDriver_cmosAutomotiveSpeed1};
	ascConfig.pins = &pins;

	// execute config method
	IfxAsclin_Asc_initModule(&uart1_ascmodule, &ascConfig);
}

void uart1_dumpImage(void){
	uint16 j, i;

	IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, 0xfe);
	IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, 0xef);

	for(i = 0; i < IMAGEH; i++){
		for(j = 0; j < IMAGEW; j++){
			if(g_ImageData[i][j] == 0xfe) g_ImageData[i][j] = 0xff;
			IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, g_ImageData[i][j]);
		}
	}

	IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, 0xef);
	IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, 0xfe);
}

uint8 ble_readByte(void){
	if(IfxAsclin_Asc_getReadCount(&uart1_ascmodule) > 0) return IfxAsclin_Asc_blockingRead(&uart1_ascmodule);

	return 0;
}

void ble_sendByte(uint8 data){
	IfxAsclin_Asc_blockingWrite(&uart1_ascmodule, data);
}

/* interrupt service routines */
void INT_UART1_TX_IRQHandler(void){
	IfxAsclin_Asc_isrTransmit(&uart1_ascmodule);		// let the default method clear the flags
}

void INT_UART1_ER_IRQHandler(void){
	IfxAsclin_Asc_isrError(&uart1_ascmodule);		// let the default method clear the flags
}

void INT_UART1_RX_IRQHandler(void){
	IfxAsclin_Asc_isrReceive(&uart1_ascmodule);
}
