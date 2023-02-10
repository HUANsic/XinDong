/*
 * CameraShenyan.c
 *
 *  Created on: 2022年10月28日
 *      Author: 7th Xindong THUIRT
 * Description: Camera Shenyan for Xindong Project
 */

#ifndef CAMERASHENYAN_H_
#define CAMERASHENYAN_H_

#include "CameraUtils.h"

/* 摄像头使用SCCB通信 SCCB和IIC基本类似 */
#define MT9V034_SCL_PIN   P02_5   /*!< SCCB SCL 管脚 */
#define MT9V034_SDA_PIN   P02_4   /*!< SCCB SDA 管脚 */

void Uart_init(void);
void UART1_TX_IRQHandler(void);
void UART1_ER_IRQHandler(void);

/**********************************************************************/
/****************************Camera related****************************/
/**********************************************************************/

/*!
 * @brief    摄像头测试例程
 *
 * @param    fps:  帧率
 *
 * @return   无
 *
 * @note     摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
 *
 * @see      CAMERA_Init(50);   //初始化MT9V034  50帧 注意使用白色带与非门版转接座
 *
 * @date     2019/10/22 星期二
 */
void CAMERA_Init (unsigned char fps);

/*!
 * @brief    串口上报上位机
 *
 * @param    无
 *
 * @return   无
 *
 * @note     上位机的帧头可能有所区别
 *
 * @see      CAMERA_Reprot();
 *
 * @date     2019/9/24 星期二
 */
void CAMERA_Reprot (void);

unsigned char CAMERA_GetFlag(void);

void CAMERA_ResetFlag(void);

#endif /* CAMERASHENYAN_H_ */
