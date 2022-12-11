#ifndef		__HUANSIC_TIMERS__
#define		__HUANSIC_TIMERS__

void delay_us(unsigned long time);
void delay_ms(unsigned long time);

void gtm_init(void);

void tim_init(void);
unsigned char ultrasonic_hasNewValue(void);
void ultrasonic_setPulseWidth(unsigned long width);
unsigned long ultrasonic_getPulseWidth(void);

void tom_initServo(void);
void tom_initMotor(void);
void tom_setServo(float angle);
void tom_setMotor(float power);

#endif		// __HUANSIC_TIMERS__
