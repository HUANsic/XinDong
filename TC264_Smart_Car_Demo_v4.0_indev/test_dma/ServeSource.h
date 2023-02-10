#ifndef _SERVESOURCE_H_
#define _SERVESOURCE_H_
/******************************************************************************/
/*****************************用户层********************/
/*****************************请不要更改此文件中的代码**********************************/
/**********************************头文件支持********************************************/
#include <Ifx_Types.h>
#include <stdio.h>
#include "Bsp/Bsp.h"
#include "_PinMap/IfxGtm_PinMap.h"
#include "ConfigurationIsr.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "Port/Std/IfxPort.h"
#include <Stm/Std/IfxStm.h>
#include <Src/Std/IfxSrc.h>
#include "Gtm/Std/IfxGtm_Cmu.h"
#include "Gtm/Std/IfxGtm_Tom.h"
#include "Cpu/Std/Ifx_Types.h"
#include "Ifxgtm_reg.h"
#include "Configuration.h"
/***********************************gpio相关***********************************************/
IFX_EXTERN void GpioInit(void); //gpio初始化
//void Eru_init(void); //io中断 初始化
/***********************************编码开关相关***********************************************/
#define DIP1 &MODULE_P21, 2  //编码开关0引脚
#define DIP2 &MODULE_P21, 3  //编码开关1引脚
#define DIP3 &MODULE_P21, 4  //编码开关2引脚
#define DIP4 &MODULE_P21, 5  //编码开关3引脚
IFX_EXTERN int GetDip(int k); //获取某个编码器开关的io值
/***********************************LED相关***********************************************/
#define LED1 &MODULE_P33, 8  //LED0引脚
#define LED2 &MODULE_P33, 9  //LED1引脚
#define LED3 &MODULE_P33, 10 //LED2引脚
#define LED4 &MODULE_P33, 11 //LED3引脚
IFX_EXTERN void SetLed(int k, int light); //设置led灯
/***********************************按钮相关***********************************************/
#define K1 &MODULE_P15, 1 //K1
#define K2 &MODULE_P20, 13 //K2
IFX_EXTERN int GetKey(int k);

/**********************************蓝牙串口相关***************************************/
#define BAUDRATE 115200 //蓝牙波特率
IFX_EXTERN uint8 Bluetooth_Read_Data(void);
IFX_EXTERN void Bluetooth_Send_Data(uint8 data);
IFX_EXTERN int GetDip(int k);
IFX_EXTERN void GpioInit(void);
/******************************舵机电机相关************************************************/
#define FORWARD 1
#define BACKWARD 0
#define LEFT 1
#define RIGHT 3
#define MIDDLE 2
#define MAXLEFTPWM 5
#define MIDPWM 7.5
#define MAXRIGHTPWM 10
IFX_EXTERN uint32 SetMotor(float direction, float pwmDuty);//电机操作函数 方向+占空比（%）
IFX_EXTERN uint32 SetSteer(float direction, float pwmDuty);//舵机操作函数 方向+占空比（%）


/******************************捕获中断相关************************************************/

#define ISR_PRIORITY_TIM2_CH0     20     //超声中断优先级   P02.8
#define ISR_PRIORITY_TIM2_CH6     12     //编码器中断优先级P20.9
#define ISR_PRIORITY_TIM0_CH6     9      //IO中断优先级P20.0
///************************************编码器相关函数********************************************/
#define CoderA &MODULE_P20, 9  //正交编码器A引脚
IFX_EXTERN int GetCodePerid(); //获取编码器频率 Hz

///************************************超声相关函数********************************************/
#define Ultratrigger     &MODULE_P02, 6  //超声trigger引脚
IFX_EXTERN void cap_trigger(void);
IFX_EXTERN float get_echo_length(void);

///*********************************定时刷新用服务函数*********************************************/
IFX_EXTERN uint32 ServeInterupt10ms(void); //服务函数10ms
IFX_EXTERN uint32 ServeInterupt100ms(void);//服务函数100ms
IFX_EXTERN uint32 ServeInterupt1000ms(void);//服务函数1000ms
void delay_ms(uint32 time); //延时 单位ms
IFX_EXTERN void delay_us(uint32 time); //延时 单位us
///********************************陀螺仪相关函数*********************************************/
extern float Yaw;
extern float Pitch;
extern float Roll;
//具体执行在cpu1的主函数里
#endif /* 0_SRC_APPSW_TRICORE_MAIN_SERVESOURCE_H_ */
