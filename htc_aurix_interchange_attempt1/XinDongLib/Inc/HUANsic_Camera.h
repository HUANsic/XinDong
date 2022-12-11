#ifndef		__HUANSIC_CAMERA__
#define		__HUANSIC_CAMERA__

void camera_init(void);

extern inline unsigned char camera_hasImage(void);

extern inline unsigned char camera_getFlag(void);

extern inline void camera_resetFlag(void);

extern inline void camera_incrementFlag(void);

#endif		// __HUANSIC_CAMERA__
