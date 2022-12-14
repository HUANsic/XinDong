#ifndef		__HUANSIC_CAMERA__
#define		__HUANSIC_CAMERA__

void camera_init(void);

unsigned char camera_hasImage(void);

unsigned char camera_getFlag(void);

void camera_resetFlag(void);

void camera_incrementFlag(void);

#endif		// __HUANSIC_CAMERA__
