/*
 * CameraShenyan.c
 *
 *  Created on: 2022年10月28日
 *      Author: 7th Xindong THUIRT
 * Description: Camera Shenyan for Xindong Project
 */

#ifndef CAMERASHENYAN_H_
#define CAMERASHENYAN_H_

#include "XinDong_Config.h"

#define IMAGEH  120
#define IMAGEW  188

/**********************************************************************/
/****************************Camera related****************************/
/**********************************************************************/

void CAMERA_Init(unsigned char fps);

void CAMERA_Reprot(void);

uint8_t CAMERA_GetFlag(void);

void CAMERA_ResetFlag(void);

uint8_t** CAMERA_GetArray(void);

#endif /* CAMERASHENYAN_H_ */
