#ifndef		__HUANSIC_UART1_BLE__
#define		__HUANSIC_UART1_BLE__

void uart1_init(void);

void uart1_dumpImage(void);

unsigned char ble_readByte(void);

void ble_sendByte(unsigned char data);

/*
 * 		METHODS BELOW ARE USED IN SYSTEM CALLS
 *
 * 		DO NOT CALL OR MODIFY UNLESS NECESSARY
 */

void INT_UART1_TX_IRQHandler(void);

void INT_UART1_ER_IRQHandler(void);

void INT_UART1_RX_IRQHandler(void);

#endif		// __HUANSIC_UART1_BLE__
