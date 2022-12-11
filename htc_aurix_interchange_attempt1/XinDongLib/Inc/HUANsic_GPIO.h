#ifndef		__HUANSIC_GPIO__
#define		__HUANSIC_GPIO__

extern inline unsigned char gpio_getDIPState(void);

extern inline unsigned char gpio_readDIP1(void);
extern inline unsigned char gpio_readDIP2(void);
extern inline unsigned char gpio_readDIP3(void);
extern inline unsigned char gpio_readDIP4(void);
extern inline unsigned char gpio_readKey1(void);
extern inline unsigned char gpio_readKey2(void);

extern inline void gpio_setLED1(unsigned char state);
extern inline void gpio_setLED2(unsigned char state);
extern inline void gpio_setLED3(unsigned char state);
extern inline void gpio_setLED4(unsigned char state);

void ultrasonic_trigger(void);

void encoder_init(void);


#endif		// __HUANSIC_GPIO__
