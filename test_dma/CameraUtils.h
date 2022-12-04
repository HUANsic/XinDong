#ifndef CAMERAUTILS_H_
#define CAMERAUTILS_H_

#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxPort_regdef.h>
#include <IfxScu_PinMap.h>
#include <IfxScuEru.h>
#include <IfxSrc.h>
#include <IfxSrc_reg.h>
#include <IfxSrc_regdef.h>
#include <IfxPort.h>
#include <IfxPort_reg.h>
#include <Platform_Types.h>
#include <stdlib.h>
#include <IfxAsclin_Asc.h>
#include "IfxCpu.h"
#include "IfxDma_Dma.h"
#include "IfxDma.h"
#include "IfxDma_reg.h"
#include "IfxDma_regdef.h"
#include "CompilerTasking.h"

/** DMA中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  DMA_PRIORITY    255

/** DMA中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define DMA_VECTABNUM 0

/* GPIO模式 */
#define PIN_MODE_OUTPUT          IfxPort_Mode_outputPushPullGeneral    /*!< 推挽输出  */
#define PIN_MODE_OUTPUT_OD       IfxPort_Mode_outputOpenDrainGeneral   /*!< 开漏输出  */
#define PIN_MODE_INPUT           IfxPort_Mode_inputNoPullDevice        /*!< 浮空输入  */
#define PIN_MODE_INPUT_PULLUP    IfxPort_Mode_inputPullUp              /*!< 上拉输入  */
#define PIN_MODE_INPUT_PULLDOWN  IfxPort_Mode_inputPullDown            /*!< 下拉输入  */

/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             IfxPort_InputMode_pullDown     /*!< 上升沿（下拉）触发中断 */
#define PIN_IRQ_MODE_FALLING            IfxPort_InputMode_pullUp       /*!< 下降沿（上拉）触发中断 */
#define PIN_IRQ_MODE_RISING_FALLING     IfxPort_InputMode_noPullDevice /*!< 双边沿（开漏）触发中断 */

/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(GPIO_NAME) (Ifx_P*)(0xF0030000u | (GPIO_NAME & 0xFF00))

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(GPIO_NAME)   (uint8)(GPIO_NAME & 0x000F)

/** GPIO外部中断 组0中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 vsync */
#define PIN_INT0_PRIORITY    253

/** GPIO外部中断 组0中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT0_VECTABNUM    0

/** GPIO外部中断 组1中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT1_PRIORITY     101

/** GPIO外部中断 组1中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT1_VECTABNUM    0

/** GPIO外部中断 组2中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复  pixel clock  */
#define PIN_INT2_PRIORITY     40

/** GPIO外部中断 组2中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT2_VECTABNUM    3

/** GPIO外部中断 组3中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT3_PRIORITY     100

/** GPIO外部中断 组3中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT3_VECTABNUM    0

/** 摄像头 数据采集完成标志位 */
#define MT9V034_IMAGEH  120  /*!< 行 HEIGHT 待采集摄像头图像高度行数 */
#define MT9V034_IMAGEW  188  /*!< 列 WIDTH  待采集摄像头图像宽度列数 */
#define IMAGEH  MT9V034_IMAGEH   /*!< 摄像头采集高度 */
#define IMAGEW  MT9V034_IMAGEW   /*!< 摄像头采集宽度 */

/**
 * 摄像头 DMA 配置结构体
 */
typedef struct
{
        Ifx_DMA_CH linkedList[2];               //总是报错，去掉了IFX_ALIGN(256)，实测没有影响
        //IFX_ALIGN(256) Ifx_DMA_CH linkedList[2]; //链表存储。必须与256位地址对齐，否则DMA无法读取
        IfxDma_Dma_Channel chn;                 // Dma channel handle
} Dma_Camera_t;

// GPIO 枚举端口编号
typedef enum //
{
    P00_0 = 0xA000,
    P00_1 = 0xA001,
    P00_2 = 0xA002,
    P00_3 = 0xA003,
    P00_4 = 0xA004,
    P00_5 = 0xA005,
    P00_6 = 0xA006,
    P00_7 = 0xA007,
    P00_8 = 0xA008,
    P00_9 = 0xA009,
    P00_11 = 0xA00B,
    P00_12 = 0xA00C,

    P02_0 = 0xA200,
    P02_1 = 0xA201,
    P02_2 = 0xA202,
    P02_3 = 0xA203,
    P02_4 = 0xA204,
    P02_5 = 0xA205,
    P02_6 = 0xA206,
    P02_7 = 0xA207,
    P02_8 = 0xA208,

    P10_1 = 0xB001,
    P10_2 = 0xB002,
    P10_3 = 0xB003,
    P10_5 = 0xB005,
    P10_6 = 0xB006,

    P11_2 = 0xB102,
    P11_3 = 0xB103,
    P11_6 = 0xB106,
    P11_9 = 0xB109,
    P11_10 = 0xB10A,
    P11_11 = 0xB10B,
    P11_12 = 0xB10C,

    P13_0 = 0xB300,
    P13_1 = 0xB301,
    P13_2 = 0xB302,
    P13_3 = 0xB303,

    P14_0 = 0xB400,
    P14_1 = 0xB401,
    P14_2 = 0xB402,
    P14_3 = 0xB403,
    P14_4 = 0xB404,
    P14_5 = 0xB405,
    P14_6 = 0xB406,

    P15_0 = 0xB500,
    P15_1 = 0xB501,
    P15_2 = 0xB502,
    P15_3 = 0xB503,
    P15_4 = 0xB504,
    P15_5 = 0xB505,
    P15_6 = 0xB506,
    P15_7 = 0xB507,
    P15_8 = 0xB508,

    P20_0 = 0xC000,
    P20_2 = 0xC002,
    P20_3 = 0xC003,
    P20_6 = 0xC006,
    P20_7 = 0xC007,
    P20_8 = 0xC008,
    P20_9 = 0xC009,
    P20_10 = 0xC00A,
    P20_11 = 0xC00B,
    P20_12 = 0xC00C,
    P20_13 = 0xC00D,
    P20_14 = 0xC00E,

    P21_2 = 0xC102,
    P21_3 = 0xC103,
    P21_4 = 0xC104,
    P21_5 = 0xC105,
    P21_6 = 0xC106,
    P21_7 = 0xC107,

    P22_0 = 0xC200,
    P22_1 = 0xC201,
    P22_2 = 0xC202,
    P22_3 = 0xC203,

    P23_1 = 0xC301,

    P32_0 = 0xD200,
    P32_4 = 0xD204,

    P33_4 = 0xD304,
    P33_5 = 0xD305,
    P33_6 = 0xD306,
    P33_7 = 0xD307,
    P33_8 = 0xD308,
    P33_9 = 0xD309,
    P33_10 = 0xD30A,
    P33_11 = 0xD30B,
    P33_12 = 0xD30C,
    P33_13 = 0xD30D,

} GPIO_Name_t;

void PIN_INT0_IRQHandler ();
void PIN_INT1_IRQHandler ();
void PIN_INT2_IRQHandler ();
void PIN_INT3_IRQHandler ();

// external interrupts
IFX_INTERRUPT(PIN_INT0_IRQHandler, PIN_INT0_VECTABNUM, PIN_INT0_PRIORITY);
IFX_INTERRUPT(PIN_INT1_IRQHandler, PIN_INT1_VECTABNUM, PIN_INT1_PRIORITY);
IFX_INTERRUPT(PIN_INT2_IRQHandler, PIN_INT2_VECTABNUM, PIN_INT2_PRIORITY);
IFX_INTERRUPT(PIN_INT3_IRQHandler, PIN_INT3_VECTABNUM, PIN_INT3_PRIORITY);

/**
 * 摄像头 DMA 配置结构体
 */
// extern Dma_Camera_t g_DmaCameraLinkedList;
/*************************************************************************
 *  函数名称：void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
 *  功能说明：GPIO初始化函数
 *  参数说明：pin      ： 管脚标号
 *            mode     ： GPIO 模式
 *            output   ： 输出模式时，输出电平 1：高电平  0：低电平
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例： PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0初始化推挽输出 高电平
 *************************************************************************/
void PIN_InitConfig (GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output);

void PIN_Exti (GPIO_Name_t pin, IfxPort_InputMode mode);

/*************************************************************************
 *  函数名称：unsigned char PIN_Read(GPIO_Name_t pin)
 *  功能说明：读取GPIO电平函数
 *  参数说明：pin      ： 管脚标号
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：uint8_t pinState = PIN_Read(P00_0);  //读取P00_0 的电平状态
 *************************************************************************/
unsigned char PIN_Read (GPIO_Name_t pin);

/*************************************************************************
 *  函数名称：void PIN_Write(GPIO_Name_t pin, unsigned char output)
 *  功能说明：设置GPIO输出状态
 *  参数说明：pin      ： 管脚标号
 *            output   ： 输出模式时，输出电平 1：高电平  0：低电平
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Write(P00_0, 1);//P00_0置为高电平
 *************************************************************************/
void PIN_Write (GPIO_Name_t pin, unsigned char output);

/*************************************************************************
 *  函数名称：void PIN_Dir(GPIO_Name_t pin, unsigned char mode)
 *  功能说明：设置GPIO 输入输出方向
 *  参数说明：pin      ： 管脚标号
 *            mode     ： GPIO 模式
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Dir(P00_0, 1);//P00_0设置为输出
 *************************************************************************/
void PIN_Dir (GPIO_Name_t pin, unsigned char mode);

/*************************************************************************
 *  函数名称：void PIN_Reverse(GPIO_Name_t pin)
 *  功能说明：GPIO电平翻转函数，使用前请先初始化
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
 *************************************************************************/
void PIN_Reverse (GPIO_Name_t pin);

/* DMA 中断服务函数  */
void DMA_IRQHandler (void);

/*!
 * @brief    摄像头DMA传输初始化
 *
 * @param    srcStartAddr   ：源地址 IO地址
 * @param    dstStartAddr   ：目的地址 摄像头数据缓冲区
 * @param    channel        ：DMA传输通道 0-47      注意 DMA传输通道需要和PIX像素中断优先级保持一致
 *
 * @return   无
 *
 * @note     注意 DMA传输通道需要和PIX像素中断优先级保持一致    一幅图需要进行两次DMA传输，因此会触发两次DMA中断
 *
 * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
 *
 * @date     2019/10/22 星期二
 */
void DMA_CameraInitConfig (unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel);

extern inline unsigned char CAM_getFlag (void);

extern inline void CAM_setFlag (unsigned char);

#endif /* CAMERAUTILS_H_ */
