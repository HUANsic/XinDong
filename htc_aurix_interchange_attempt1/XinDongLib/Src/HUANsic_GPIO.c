#include "../Inc/HUANsic_GPIO.h"

/*
 #define	DIP1	&MODULE_P21, 2
 #define	DIP2	&MODULE_P21, 3
 #define	DIP3	&MODULE_P21, 4
 #define	DIP4	&MODULE_P21, 5
 #define 	LED1 	&MODULE_P33, 8
 #define 	LED2 	&MODULE_P33, 9
 #define 	LED3 	&MODULE_P33, 10
 #define 	LED4 	&MODULE_P33, 11
 #define 	K1 		&MODULE_P15, 1
 #define 	K2 		&MODULE_P20, 13
 */

/* methods for the user to call */
void gpio_init(void){
	// DIP Switches
	IfxPort_setPinMode(&MODULE_P21, 2, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P21, 3, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P21, 4, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P21, 5, IfxPort_Mode_inputNoPullDevice);

	// LEDs
	IfxPort_setPinMode(&MODULE_P33, 8, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 9, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 10, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 11, IfxPort_Mode_outputPushPullGeneral);

	// Keys
	IfxPort_setPinMode(&MODULE_P15, 1, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P20, 13, IfxPort_Mode_inputNoPullDevice);
}

void encoder_init(void){
	IfxPort_setPinMode(&MODULE_P20, 9, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P20, 10, IfxPort_Mode_inputNoPullDevice);
}

void ultrasonic_init(void){
	IfxPort_setPinMode(&MODULE_P02, 6, IfxPort_Mode_outputPushPullGeneral);		// trigger
	IfxPort_setPinMode(&MODULE_P20, 0, IfxPort_Mode_inputNoPullDevice);			// echo
	IfxPort_setPinState(&MODULE_P02, 6, IfxPort_State_low);
}

void ultrasonic_trigger(void){
	IfxPort_setPinState(&MODULE_P02, 6, IfxPort_State_high);
	IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 30));
	IfxPort_setPinState(&MODULE_P02, 6, IfxPort_State_low);
}

