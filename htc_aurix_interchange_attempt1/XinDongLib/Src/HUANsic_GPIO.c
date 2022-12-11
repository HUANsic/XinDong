#include "../Inc/HUANsic_GPIO.h"

#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <IfxPort.h>
#include <IfxPort_reg.h>

/*
 #define		DIP1	&MODULE_P21, 2
 #define		DIP2	&MODULE_P21, 3
 #define		DIP3	&MODULE_P21, 4
 #define		DIP4	&MODULE_P21, 5
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

inline uint8 gpio_getDIPState(void){
	return ((unsigned char)(MODULE_P21.IN.U >> 2)) & 0x0F;
}

inline uint8 gpio_readDIP1(void){
	return gpio_getDIPState() & 0x01;
}

inline uint8 gpio_readDIP2(void){
	return gpio_getDIPState() & 0x02;
}

inline uint8 gpio_readDIP3(void){
	return gpio_getDIPState() & 0x04;
}

inline uint8 gpio_readDIP4(void){
	return gpio_getDIPState() & 0x08;
}

inline uint8 gpio_readKey1(void){
	return IfxPort_getPinState(&MODULE_P15, 1);
}

inline uint8 gpio_readKey2(void){
	return IfxPort_getPinState(&MODULE_P20, 13);
}

inline void gpio_setLED1(uint8 state){
	IfxPort_setPinState(&MODULE_P33, 8, state ? 0 : 1);
}

inline void gpio_setLED2(uint8 state){
	IfxPort_setPinState(&MODULE_P33, 9, state ? 0 : 1);
}

inline void gpio_setLED3(uint8 state){
	IfxPort_setPinState(&MODULE_P33, 10, state ? 0 : 1);
}

inline void gpio_setLED4(uint8 state){
	IfxPort_setPinState(&MODULE_P33, 11, state ? 0 : 1);
}

void ultrasonic_trigger(void){
	IfxPort_setPinState(&MODULE_P02, 6, IfxPort_State_high);
	IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 30));
	IfxPort_setPinState(&MODULE_P02, 6, IfxPort_State_low);
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

