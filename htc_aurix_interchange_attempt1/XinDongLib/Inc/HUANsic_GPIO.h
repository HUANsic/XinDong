#ifndef		__HUANSIC_GPIO__
#define		__HUANSIC_GPIO__

#include <IfxStm.h>
#include <IfxStm_reg.h>
#include <IfxPort.h>
#include <IfxPort_reg.h>

void gpio_init(void);

void ultrasonic_trigger(void);

void encoder_init(void);

void ultrasonic_trigger(void);

inline unsigned char gpio_getDIPState(void){
	return ((unsigned char)(MODULE_P21.IN.U >> 2)) & 0x0F;
}

inline unsigned char gpio_readDIP1(void){
	return gpio_getDIPState() & 0x01;
}

inline unsigned char gpio_readDIP2(void){
	return gpio_getDIPState() & 0x02;
}

inline unsigned char gpio_readDIP3(void){
	return gpio_getDIPState() & 0x04;
}

inline unsigned char gpio_readDIP4(void){
	return gpio_getDIPState() & 0x08;
}

inline unsigned char gpio_readKey1(void){
	return IfxPort_getPinState(&MODULE_P15, 1);
}

inline unsigned char gpio_readKey2(void){
	return IfxPort_getPinState(&MODULE_P20, 13);
}

inline void gpio_setLED1(unsigned char state){
	IfxPort_setPinState(&MODULE_P33, 8, state ? 0 : 1);
}

inline void gpio_setLED2(unsigned char state){
	IfxPort_setPinState(&MODULE_P33, 9, state ? 0 : 1);
}

inline void gpio_setLED3(unsigned char state){
	IfxPort_setPinState(&MODULE_P33, 10, state ? 0 : 1);
}

inline void gpio_setLED4(unsigned char state){
	IfxPort_setPinState(&MODULE_P33, 11, state ? 0 : 1);
}

#endif		// __HUANSIC_GPIO__
