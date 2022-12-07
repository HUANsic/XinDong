#ifndef		__HUANSIC_UART1_BLE__
#define		__HUANSIC_UART1_BLE__

void uart1_init(void);

void uart1_dumpImage(void);

void UART1_TX_IRQHandler(void);

void UART1_ER_IRQHandler(void);

void UART1_RX_IRQHandler(void);

#endif		// __HUANSIC_UART1_BLE__
