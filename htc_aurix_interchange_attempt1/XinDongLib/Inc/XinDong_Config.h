/**
 *
 */
#ifndef		__HUANSIC_XINDONG_CONFIG__
#define		__HUANSIC_XINDONG_CONFIG__

#define     DMA_PRIORITY            	255
#define     DMA_TypeOfService           IfxSrc_Tos_cpu0

#define 	PIN_INT0_PRIORITY    		253
#define 	PIN_INT0_TypeOfService    	IfxSrc_Tos_cpu0

#define 	PIN_INT1_PRIORITY     		101
#define 	PIN_INT1_TypeOfService    	IfxSrc_Tos_cpu0

#define 	PIN_INT2_PRIORITY     		40
#define		PIN_INT2_TypeOfService    	IfxSrc_Tos_dma

#define 	PIN_INT3_PRIORITY     		100
#define 	PIN_INT3_TypeOfService    	IfxSrc_Tos_cpu0

#define		INT_UART1_TX_Priority		131
#define		INT_UART1_RX_Priority		130
#define		INT_UART1_ERR_Priority		132
#define		INT_UART1_TypeOfService		IfxSrc_Tos_cpu0

#define ASC_TX_BUFFER_SIZE 64
#define ASC_RX_BUFFER_SIZE 64

#define MT9V034_SCL_PIN   P02_5
#define MT9V034_SDA_PIN   P02_4


#endif		// __HUANSIC_XINDONG_CONFIG__
