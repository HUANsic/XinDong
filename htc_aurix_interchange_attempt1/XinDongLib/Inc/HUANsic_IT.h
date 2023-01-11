#ifndef		__HUANSIC_IT__
#define		__HUANSIC_IT__

void stm0_installInterrupts(void);

void camera_installInterrupts(void);

void encoder_installInterrupt(void);

void ble_installInterrupts(void);

void ultrasonic_installInterrupt(void);

void reed_installInterrupt(void);

void tim_installInterrupts(void);

void dma_CameraStop(unsigned long channel);

#endif		// __HUANSIC_IT__
