#ifndef		__HUANSIC_XINDONG_CONFIG__
#define		__HUANSIC_XINDONG_CONFIG__

#define		XINDONG_USE_BLUETOOTH		0

#define     INT_DMA_PRIORITY            254
#define     INT_DMA_TypeOfService       IfxSrc_Tos_cpu0

#define 	INT_EXTI0_PRIORITY    		253
#define 	INT_EXTI0_TypeOfService    	IfxSrc_Tos_cpu0

#define 	INT_EXTI1_PRIORITY     		101
#define 	INT_EXTI1_TypeOfService    	IfxSrc_Tos_cpu0

#define 	INT_EXTI2_PRIORITY     		40
#define		INT_EXTI2_TypeOfService    	IfxSrc_Tos_dma

#define 	INT_EXTI3_PRIORITY     		100
#define 	INT_EXTI3_TypeOfService    	IfxSrc_Tos_cpu0

#define		INT_UART1_TX_Priority		131
#define		INT_UART1_RX_Priority		130
#define		INT_UART1_ERR_Priority		132
#define		INT_UART1_TypeOfService		IfxSrc_Tos_cpu0

#define		INT_STM0_Priority			30
#define		INT_STM0_TypeOfService		IfxSrc_Tos_cpu0

//#define		INT_TIM0CH1_Priority		13				// pclk via TIM
//#define		INT_TIM0CH1_TypeOfService	IfxSrc_Tos_cpu1

//#define		INT_TIM0CH2_Priority		15				// VSYNV via TIM
//#define		INT_TIM0CH2_TypeOfService	IfxSrc_Tos_cpu1

#define		INT_TIM0CH6_Priority		9				// reed switch
#define		INT_TIM0CH6_TypeOfService	IfxSrc_Tos_cpu0

//#define		INT_TIM1CH3_Priority		14				// HSYNC via TIM
//#define		INT_TIM1CH3_TypeOfService	IfxSrc_Tos_cpu1

#define		INT_TIM2CH0_Priority		20				// ultrasonic
#define		INT_TIM2CH0_TypeOfService	IfxSrc_Tos_cpu0

//#define		INT_TIM2CH5_Priority		11				// Encoder A
//#define		INT_TIM2CH5_TypeOfService	IfxSrc_Tos_cpu0

//#define		INT_TIM2CH6_Priority		12				// Encoder B
//#define		INT_TIM2CH6_TypeOfService	IfxSrc_Tos_cpu0

#define		ASC_TX_BUFFER_SIZE			64
#define		ASC_RX_BUFFER_SIZE			64

#define		CAM_Address					0x5C		// before shift
#define		CAM_FPS						30

#define 	IMAGEH  					120
#define 	IMAGEW  					188

#define PWM_FREQ_MOTOR 20000   /*Hz*/
#define	SERVO_DC_OFFSET		0.00

#endif		// __HUANSIC_XINDONG_CONFIG__
