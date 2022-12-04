/*
 * CameraShenyan.c
 *
 *  Created on: 2022Äê10ÔÂ28ÈÕ
 *      Author: 7th Xindong THUIRT
 * Description: Camera Shenyan for Xindong Project
 */

#include "CameraShenyan.h"
#include <Ifx_Fifo.h>
#include <IfxAsclin.h>
#include <IfxAsclin_Asc.h>
#include <IfxAsclin_PinMap.h>

#define MT9V034_IMAGEH  120  /*!< 行 HEIGHT 待采集摄像头图像高度行数 */
#define MT9V034_IMAGEW  188  /*!< 列 WIDTH  待采集摄像头图像宽度列数 */
#define IMAGEH  MT9V034_IMAGEH   /*!< 摄像头采集高度 */
#define IMAGEW  MT9V034_IMAGEW   /*!< 摄像头采集宽度 */

/** UART中断服务函数地址 */
#define ASC_TX_BUFFER_SIZE 64        //发送缓冲区长度
#define ASC_RX_BUFFER_SIZE 64        //接收缓冲区长度

/*! 对比度 高对比度0x03c7  低对比度0x01c7  注意 高对比度会使图像变暗 */
#define CAMERA_CONTRAST          0x01c7

/*! 自动曝光开关 默认打开  设置为0 关闭自动曝光 */
#define CAMERA_AUTO_EXPOSURE     1

/*! 自动曝光模式下 亮度 调节范围 1-64 */
#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  50

/*! 可以在自动曝光的情况下设置固定帧率 */
/*! 自动曝光模式需要设置曝光时间上限 调节范围 1–32765 */
/*! 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/*! 例如使用100帧时 最大曝光时间超过317 会导致帧率下降 */
/*! 曝光时间越长 图像越亮 */
#define CAMERA_MAX_EXPOSURE_TIME  250
#define CAMERA_MIN_EXPOSURE_TIME  1

/*! 非自动曝光模式下 可以调节曝光时间来调节图像整体亮度 调节范围 0–32765 */
/*! 注意 当帧率过高时 如果设置的曝光时间过长 帧率可能会自适应下调 */
/*! 曝光时间越长 图像越亮 */
#define CAMERA_EXPOSURE_TIME  150

#define MT9V034_SCL_Out   PIN_Dir(MT9V034_SCL_PIN, 1);      //配置输出作为SCL_Out
#define MT9V034_SDA_Out   PIN_Dir(MT9V034_SDA_PIN, 1);      //配置作为输出作为SDA_Out
#define MT9V034_SDA_In    PIN_Dir(MT9V034_SDA_PIN, 0);      //配置作为输入作为SDA_In
#define MT9V034_SCL_High  PIN_Write(MT9V034_SCL_PIN, 1);      //配置输出高电平
#define MT9V034_SCL_Low   PIN_Write(MT9V034_SCL_PIN, 0);      //配置输出低电平
#define MT9V034_SDA_High  PIN_Write(MT9V034_SDA_PIN, 1);      //配置输出高电平
#define MT9V034_SDA_Low   PIN_Write(MT9V034_SDA_PIN, 0);      //配置输出低电平
#define MT9V034_SDA_Data  PIN_Read(MT9V034_SDA_PIN)           //读取引脚上的引脚状态

#define MT9V034_I2C_ADDR                        0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT            480
#define MAX_IMAGE_WIDTH             752
#define MT9V034_PIXEL_ARRAY_HEIGHT              492
#define MT9V034_PIXEL_ARRAY_WIDTH               782
#define MT9V034_CHIP_VERSION                0x00
#define MT9V034_CHIP_ID                     0x1324

#define MT9V034_COLUMN_START                        0x01
#define MT9V034_COLUMN_START_MIN                1
#define MT9V034_COLUMN_START_DEF                1
#define MT9V034_COLUMN_START_MAX                752

#define MT9V034_ROW_START                       0x02
#define MT9V034_ROW_START_MIN               4
#define MT9V034_ROW_START_DEF               4
#define MT9V034_ROW_START_MAX               482

#define MT9V034_WINDOW_HEIGHT               0x03
#define MT9V034_WINDOW_HEIGHT_MIN               1
#define MT9V034_WINDOW_HEIGHT_DEF               64
#define MT9V034_WINDOW_HEIGHT_MAX               480

#define MT9V034_WINDOW_WIDTH                        0x04
#define MT9V034_WINDOW_WIDTH_MIN                1
#define MT9V034_WINDOW_WIDTH_DEF                64
#define MT9V034_WINDOW_WIDTH_MAX                752

#define MINIMUM_HORIZONTAL_BLANKING     91 // see datasheet

#define MT9V034_HORIZONTAL_BLANKING             0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN     43
#define MT9V034_HORIZONTAL_BLANKING_MAX     1023

#define MT9V034_VERTICAL_BLANKING               0x06
#define MT9V034_VERTICAL_BLANKING_MIN       4
#define MT9V034_VERTICAL_BLANKING_MAX       3000

#define MT9V034_CHIP_CONTROL                        0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE         (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE         (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL     (1 << 8)

#define MT9V034_SHUTTER_WIDTH1              0x08
#define MT9V034_SHUTTER_WIDTH2              0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL       0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH     0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN     1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF     480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX     32767

#define MT9V034_RESET                       0x0C

#define MT9V034_READ_MODE                       0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK      (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT     0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK        (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT       2
#define MT9V034_READ_MODE_ROW_BIN_2         (1<<0)
#define MT9V034_READ_MODE_ROW_BIN_4         (1<<1)
#define MT9V034_READ_MODE_COL_BIN_2         (1<<2)
#define MT9V034_READ_MODE_COL_BIN_4         (1<<3)
#define MT9V034_READ_MODE_ROW_FLIP              (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP       (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS      (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS             (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE                0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR       (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR         (1 << 6)

#define MT9V034_V1_CTRL_REG_A           0x31
#define MT9V034_V2_CTRL_REG_A           0x32
#define MT9V034_V3_CTRL_REG_A           0x33
#define MT9V034_V4_CTRL_REG_A           0x34

#define MT9V034_ANALOG_GAIN                     0x35
#define MT9V034_ANALOG_GAIN_MIN             16
#define MT9V034_ANALOG_GAIN_DEF             16
#define MT9V034_ANALOG_GAIN_MAX             64

#define MT9V034_MAX_ANALOG_GAIN             0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX             127

#define MT9V034_FRAME_DARK_AVERAGE              0x42
#define MT9V034_DARK_AVG_THRESH             0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK         (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT        0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK       (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT      8

#define MT9V034_ROW_NOISE_CORR_CONTROL      0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE       (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG      (1 << 7)

#define MT9V034_PIXEL_CLOCK                 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE            (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME       (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE            (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE       (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK     (1 << 4)

#define MT9V034_TEST_PATTERN                    0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK      (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT     0
#define MT9V034_TEST_PATTERN_USE_DATA       (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK      (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE      (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL      (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL        (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL      (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE         (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP           (1 << 14)

#define MT9V034_AEC_AGC_ENABLE          0xAF
#define MT9V034_AEC_ENABLE                  (1 << 0)
#define MT9V034_AGC_ENABLE                  (1 << 1)
#define MT9V034_THERMAL_INFO                    0xc1
#define MT9V034_ANALOG_CTRL                     (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE                 (1<<7)
#define MT9V034_MAX_GAIN                        (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A      (0xD5)
#define MT9V034_HDR_ENABLE_REG          0x0F
#define MT9V034_ADC_RES_CTRL_REG            0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG        0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG  0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG            0xAB
#define MT9V034_MIN_EXPOSURE_REG        0xAC  // datasheet min coarse shutter width
#define MT9V034_MAX_EXPOSURE_REG        0xAD  // datasheet max coarse shutter width
#define MT9V034_AEC_AGC_ENABLE_REG  0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0

//串口通信结构体
IfxAsclin_Asc g_UartConfig[4];

static uint8 s_AscTxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 s_AscRxBuffer[4][ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/** Í¼ÏñÔ­Ê¼Êý¾Ý´æ·Å */
extern unsigned char Image_Data[IMAGEH][IMAGEW];
IfxAsclin_Asc g_UartConfig[4];

void Uart_init()
{
    int i, j;
    IfxCpu_disableInterrupts();
    // 0xB10A P11_10 0xB10C P11_12
    Ifx_P *portRx = PIN_GetModule(0xB10A);
    uint8 pinIndexRx = PIN_GetIndex(0xB10A);
    Ifx_P *portTx = PIN_GetModule(0xB10C);
    uint8 pinIndexTx = PIN_GetIndex(0xB10C);
    IfxAsclin_Rx_In *IfxAsclin_Rx = NULL_PTR;
    IfxAsclin_Tx_Out *IfxAsclin_Tx = NULL_PTR;
    for (i = 0; i < IFXASCLIN_PINMAP_NUM_MODULES; i++)
    {
        for (j = 0; j < IFXASCLIN_PINMAP_RX_IN_NUM_ITEMS; j++)
        {
            if (IfxAsclin_Rx_In_pinTable[i][j] == NULL_PTR)
            {

            }
            else if ((unsigned long) portRx == (unsigned long) (IfxAsclin_Rx_In_pinTable[i][j]->pin.port)
                    && pinIndexRx == IfxAsclin_Rx_In_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Rx = IfxAsclin_Rx_In_pinTable[i][j];
            }
        }
        for (j = 0; j < IFXASCLIN_PINMAP_TX_OUT_NUM_ITEMS; j++)
        {
            if (IfxAsclin_Tx_Out_pinTable[i][j] == NULL_PTR)
            {

            }
            else if ((unsigned long) portTx == (unsigned long) (IfxAsclin_Tx_Out_pinTable[i][j]->pin.port)
                    && pinIndexTx == IfxAsclin_Tx_Out_pinTable[i][j]->pin.pinIndex)
            {
                IfxAsclin_Tx = IfxAsclin_Tx_Out_pinTable[i][j];
            }
        }
    }
    if (IfxAsclin_Rx->module != IfxAsclin_Tx->module)
    {
        while (1)
            ;
    }
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, IfxAsclin_Tx->module);
    ascConfig.baudrate.baudrate = (float) 115200;
    ascConfig.frame.frameMode = IfxAsclin_FrameMode_asc;
    ascConfig.frame.dataLength = IfxAsclin_DataLength_8;
    ascConfig.frame.stopBit = IfxAsclin_StopBit_1;
    ascConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
    ascConfig.frame.parityBit = FALSE;
    unsigned char uartNum = IfxAsclin_getIndex(IfxAsclin_Tx->module);
    //中断优先级配置
     ascConfig.interrupt.rxPriority = 130;
     ascConfig.interrupt.txPriority = 131;
     ascConfig.interrupt.erPriority = 132;
     ascConfig.interrupt.typeOfService = 0;

     //接收和发送FIFO的配置
     ascConfig.txBuffer     = &s_AscTxBuffer[uartNum][0];
     ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
     ascConfig.rxBuffer     = &s_AscRxBuffer[uartNum][0];
     ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

     IfxAsclin_Asc_Pins pins =
     {
      NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
      IfxAsclin_Rx,             IfxPort_InputMode_pullUp,        /* Rx pin */
      NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
      IfxAsclin_Tx,             IfxPort_OutputMode_pushPull,     /* Tx pin */
      IfxPort_PadDriver_cmosAutomotiveSpeed1
     };
     ascConfig.pins = &pins;

     //调用上面结构体所预设的参数，完成模块的初始化
     IfxAsclin_Asc_initModule(&g_UartConfig[uartNum], &ascConfig);

     //接收，发送和错误中断配置
     IfxCpu_Irq_installInterruptHandler((void *)&UART1_TX_IRQHandler, 131);
     IfxCpu_Irq_installInterruptHandler((void *)&UART1_ER_IRQHandler, 132);

     //开启CPU中断
     IfxCpu_enableInterrupts();
}

void UART1_TX_IRQHandler(void)
{
 IfxAsclin_Asc_isrTransmit(&g_UartConfig[1]);

 /* 用户代码 */
}

void UART1_ER_IRQHandler(void)
{
 IfxAsclin_Asc_isrError(&g_UartConfig[1]);

 /* 用户代码 */
}

/**********************************************************************/
/****************************SCCB related******************************/
/**********************************************************************/

/*!
 * @brief    SCCB ³õÊ¼»¯
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     SCCB IOÔÚLQ_MT9V034.hÖÐÍ¨¹ýºê¶¨ÒåÑ¡Ôñ
 *
 * @see      SCCB_Init(); //³õÊ¼»¯SCCB  IO
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void SCCB_Init (void)
{
    PIN_InitConfig(MT9V034_SCL_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(MT9V034_SDA_PIN, PIN_MODE_OUTPUT, 1);
    IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SCL_PIN), PIN_GetIndex(MT9V034_SCL_PIN),
            IfxPort_PadDriver_cmosAutomotiveSpeed1);
    IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SDA_PIN), PIN_GetIndex(MT9V034_SDA_PIN),
            IfxPort_PadDriver_cmosAutomotiveSpeed1);
    MT9V034_SCL_High
    ;
    MT9V034_SDA_High
    ;
}

/*!
 * @brief    SCCB ÑÓÊ±
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_Wait();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void SCCB_Wait (void)
{
    int i;
    for (i = 0; i < 500; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/*!
 * @brief    SCCB ¿ªÊ¼Ê±Ðò
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_Star();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void SCCB_Star (void)
{
    MT9V034_SCL_Out
    ;
    MT9V034_SDA_Out
    ;
    SCCB_Wait();
    MT9V034_SDA_High
    ;
    MT9V034_SCL_High
    ;
    SCCB_Wait();
    MT9V034_SDA_Low
    ;
    SCCB_Wait();
    MT9V034_SCL_Low
    ;
    SCCB_Wait();
}

/*!
 * @brief    SCCB Í£Ö¹Ê±Ðò
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_Star();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void SCCB_Stop (void)
{
    MT9V034_SCL_Out
    ;
    MT9V034_SDA_Out
    ;
    SCCB_Wait();
    MT9V034_SDA_Low
    ;
    SCCB_Wait();
    MT9V034_SCL_High
    ;
    SCCB_Wait();
    MT9V034_SDA_High
    ;
    SCCB_Wait();
}

/*!
 * @brief    SCCB ·¢ËÍÒ»¸ö×Ö½Ú
 *
 * @param    Data  £º·¢ËÍ×Ö½Ú
 *
 * @return   ack Ó¦´ðÐÅºÅ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_SendByte(0x01);
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
unsigned char SCCB_SendByte (unsigned char Data)
{
    unsigned char i;
    unsigned char Ack;
    MT9V034_SDA_Out
    ;
    for (i = 0; i < 8; i++)
    {
        if (Data & 0x80)
        {
            MT9V034_SDA_High
            ;
        }
        else
        {
            MT9V034_SDA_Low
            ;
        }
        Data <<= 1;
        SCCB_Wait();
        MT9V034_SCL_High
        ;
        SCCB_Wait();
        MT9V034_SCL_Low
        ;
        SCCB_Wait();
    }
    MT9V034_SDA_High
    ;
    MT9V034_SDA_In
    ;
    SCCB_Wait();

    MT9V034_SCL_High
    ;
    SCCB_Wait();
    Ack = MT9V034_SDA_Data;
    MT9V034_SCL_Low
    ;
    SCCB_Wait();
    return Ack;
}

/*!
 * @brief    SCCB ½ÓÊÕÒ»¸ö×Ö½Ú
 *
 * @param    ÎÞ
 *
 * @return   ½ÓÊÕµ½µÄ×Ö½Ú
 *
 * @note     ÎÞ
 *
 * @see      unsigned char data = SCCB_ReadByte();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
unsigned char SCCB_ReadByte (void)
{
    unsigned char i;
    volatile unsigned char byte;
    MT9V034_SCL_Out
    ;
    MT9V034_SDA_In
    ; //Ê¹ÄÜÊäÈë
    for (i = 0; i < 8; i++)
    {
        MT9V034_SCL_Low
        ;
        SCCB_Wait();
        MT9V034_SCL_High
        ;
        SCCB_Wait();
        byte = (byte << 1) | (MT9V034_SDA_Data & 1);
    }
    MT9V034_SCL_Low
    ;
    MT9V034_SDA_Out
    ;
    SCCB_Wait();
    return byte;
}

/*!
 * @brief    SCCB Ó¦´ð
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_Ack();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
static void SCCB_Ack (void)
{
    MT9V034_SCL_Out
    ;
    MT9V034_SDA_Out
    ;
    MT9V034_SCL_Low
    ;
    MT9V034_SDA_Low
    ;
    SCCB_Wait();
    MT9V034_SCL_High
    ;
    SCCB_Wait();
    MT9V034_SCL_Low
    ;
    SCCB_Wait();
}

/*!
 * @brief    SCCB ²»Ó¦´ð
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      SCCB_NAck();
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
static void SCCB_NAck (void)
{
    MT9V034_SCL_Out
    ;
    MT9V034_SDA_Out
    ;
    MT9V034_SCL_Low
    ;
    SCCB_Wait();
    MT9V034_SDA_High
    ;
    SCCB_Wait();
    MT9V034_SCL_High
    ;
    SCCB_Wait();
    MT9V034_SCL_Low
    ;
    SCCB_Wait();
}

/*!
 * @brief    SCCB Ð´¼Ä´æÆ÷
 *
 * @param    Device  : µØÖ·
 * @param    reg     : ¼Ä´æÆ÷
 * @param    Data    : Êý¾Ý
 *
 * @return   ack
 *
 * @note     ÎÞ
 *
 * @see      SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void SCCB_RegWrite (unsigned char Device, unsigned char reg, unsigned short Data)
{
    unsigned char i;
    unsigned char Ack;

    for (i = 0; i < 3; i++)
    {
        SCCB_Star();
        Ack = SCCB_SendByte(Device);
        if (Ack == 1)
        {
            continue;
        }

        Ack = SCCB_SendByte(reg);
        if (Ack == 1)
        {
            continue;
        }

        SCCB_SendByte((unsigned char) (Data >> 8));
        Ack = SCCB_SendByte((unsigned char) Data);
        if (Ack == 1)
        {
            continue;
        }

        SCCB_Stop();
        if (Ack == 0)
            break;
    }
}

/*!
 * @brief    SCCB ¶Á¼Ä´æÆ÷
 *
 * @param    Device  : µØÖ·
 * @param    reg     : ¼Ä´æÆ÷
 * @param    Data    : ¶Á³öÊý¾Ý
 *
 * @return   ack
 *
 * @note     ÎÞ
 *
 * @see      SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data)
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
unsigned char SCCB_RegRead (unsigned char Device, unsigned char reg, unsigned short *Data)
{
    unsigned char Ack = 0;
    Device = Device << 1;
    SCCB_Star();
    Ack += SCCB_SendByte(Device);

    Ack += SCCB_SendByte(reg);

    SCCB_Star();
    Ack += SCCB_SendByte(Device + 1);

    *Data = SCCB_ReadByte();
    SCCB_Ack();
    *Data = *Data << 8;

    *Data += SCCB_ReadByte();
    SCCB_NAck();

    SCCB_Stop();

    return Ack;
}

/*!
 * @brief    Ö¡ÂÊÉèÖÃ
 *
 * @param    fps : Ö¡ÂÊ
 *
 * @return
 *
 * @note     Ö¡ÂÊÆäÊµÊÇÍ¨¹ýÔö¼Ó¿Õ°×ÐÐÊµÏÖµÄ£¬ ÏÂÃæÊÇÊµ²âµÄÖ¡ÂÊÓëÐéÄâÐÐÖ®¼äµÄ¹ØÏµ
 * @note     Ö¡ÂÊ	    200    150    100    70.9    50    20    10
 * @note     ÐéÄâÐÐ    39     92     199    330     515   1450  3000
 *
 * @see      MT9V034_SetFrameRate(50);
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void MT9V034_SetFrameRate (unsigned char fps)
{
    float vertical = 0;
    if (fps > 200)
    {
        vertical = 2;
    }
    else if (fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if (fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if (fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if (fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if (fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else
    {
        vertical = -155 * fps + 4550;
    }
    /* ÐéÄâÐÐ ÉèÖÃÖ¡ÂÊÊ±¼ä 2¨C32288 */
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (unsigned short) vertical);
}

/*!
 * @brief    SCCBÐ´16Î»¼Ä´æÆ÷
 *
 * @param    reg £º ¼Ä´æÆ÷
 * @param    val £º Ð´ÈëµÄÖµ
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      MTV_IICWriteReg16(0x20, 0x0003); //ÏòMT9V034µÄ0x20¼Ä´æÆ÷Ð´Èë0x0003
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void MTV_IICWriteReg16 (unsigned char reg, unsigned short val)
{
    SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}

/*!
 * @brief    ÉèÖÃÍ¼Ïñ´°¿Ú´óÐ¡
 *
 * @param    fps : Ö¡ÂÊ
 *
 * @return   ÎÞ
 *
 * @note     Ò»°ãÊ¹ÓÃÄ¬ÈÏ¼´¿É
 *
 * @see      MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW);
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
static void MT9V034_SetFrameResolution (unsigned short height, unsigned short width)
{
    unsigned short data = 0;

    if ((height * 4) <= MAX_IMAGE_HEIGHT)   //ÅÐ¶ÏÐÐÊÇ·ñ4·ÖÆµ
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if ((height * 2) <= MAX_IMAGE_HEIGHT)  //ÅÐ¶ÏÐÐÊÇ·ñ2·ÖÆµ
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }

    if ((width * 4) <= MAX_IMAGE_WIDTH)   //ÅÐ¶ÏÁÐÊÇ·ñ4·ÖÆµ
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if ((width * 2) <= MAX_IMAGE_WIDTH)   //ÅÐ¶ÏÁÐÊÇ·ñ2·ÖÆµ
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    //         Ë®Æ½·­×ª                     ´¹Ö±·­×ª
    data |= (MT9V034_READ_MODE_ROW_FLIP | MT9V034_READ_MODE_COLUMN_FLIP);  //ÐèÒª·­×ªµÄ¿ÉÒÔ´ò¿ª×¢ÊÍ
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //Ð´¼Ä´æÆ÷£¬ÅäÖÃÐÐ·ÖÆµ

    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH, width);  //¶ÁÈ¡Í¼ÏñµÄÁÐÊý  ¸Ä±ä´Ë´¦Ò²¿É¸Ä±äÍ¼ÏñÊä³ö´óÐ¡£¬²»¹ý»á¶ªÊ§ÊÓ½Ç
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //¶ÁÈ¡Í¼ÏñµÄÐÐÊý  ¸Ä±ä´Ë´¦Ò²¿É¸Ä±äÍ¼ÏñÊä³ö´óÐ¡£¬²»¹ý»á¶ªÊ§ÊÓ½Ç

    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //ÁÐ¿ªÊ¼
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //ÐÐ¿ªÊ¼

}

/*!
 * @brief    ÉèÖÃ×Ô¶¯ÆØ¹âÊ±¼ä
 *
 * @param    enable £º 1 Ê¹ÄÜ×Ô¶¯ÆØ¹â  ×Ô¶¯ÆØ¹âÊ±¼äÍ¨¹ýÉÏÃæµÄºê¶¨ÒåÐÞ¸Ä
 *
 * @return   ÎÞ
 *
 * @note     ÎÞ
 *
 * @see      MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);
 *
 * @date     2019/7/26 ÐÇÆÚÎå
 */
void MT9V034_SetAutoExposure (char enable)
{
    unsigned short reg = 0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, &reg);
    if (enable)
    {
        /* ¿ªÆô×Ô¶¯ÆØ¹â×Ô¶¯ÔöÒæ */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg | MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE);
        /* ×î´óÆØ¹âÊ±¼ä ÐÞ¸ÄÕâÀï¿ÉÒÔÐÞ¸Ä±È½Ï°µÊ±µÄÍ¼ÏñÕûÌåÁÁ¶È*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* ×îÐ¡ÆØ¹âÊ±¼ä ÐÞ¸ÄÕâÀï¿ÉÒÔÐÞ¸ÄÓöµ½Ç¿¹âÊ±µÄÍ¼ÏñÕûÌåÁÁ¶È*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* ×î´óÔöÒæ Ôö´óÕâÀï Í¼ÏñÆ«°µµÄÇé¿öÏÂ±£Áô¸ü¶àµÄÏ¸½Ú µ«ÊÇ¿ÉÄÜ²úÉúÔëµã 0-60*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  ÓÃÓÚAEC/AGCÖ±·½Í¼ÏñËØÊýÄ¿,22560 ×î´ó44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);

    }
    else
    {
        /* ¹Ø±Õ×Ô¶¯ÆØ¹â */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg & ~(MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE));

        /* 0xAB  ×î´óÄ£ÄâÔöÒæ     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);

        /* 0x0B ÉèÖÃÆØ¹âÊ±¼ä 0¨C32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_TOTAL_SHUTTER_WIDTH, CAMERA_EXPOSURE_TIME);

    }

}

/*!
 * @brief    MT9V034¼Ä´æÆ÷³õÊ¼»¯
 *
 * @param    fps  :Ö¡ÂÊ
 *
 * @return   ÎÞ
 *
 * @note     Ò»°ãÊ¹ÓÃ50Ö¡
 *
 * @see      MT9V034_Init(50);
 *
 * @date     2019/6/12 ÐÇÆÚÈý
 */
void MT9V034_Init (unsigned char fps)
{
    unsigned short data = 0;

    //ÉãÏñÍ·SCCB½Ó¿Ú³õÊ¼»¯
    SCCB_Init();

    if (SCCB_RegRead(MT9V034_I2C_ADDR >> 1, MT9V034_CHIP_VERSION, &data) == 0)  //¶ÁÈ¡ÉãÏñÍ·°æ±¾¼Ä´æÆ÷
    {
        if (data != MT9V034_CHIP_ID)                                    //Ð¾Æ¬ID²»ÕýÈ·£¬ËµÃ÷Ã»ÓÐÕýÈ·¶ÁÈ¡µ¼Êý¾Ý£¬¼ì²é½ÓÏß
        {

            while (1)
                ;
        }
    }
    else
    {
        while (1)
            ;          //ÉãÏñÍ·Ê¶±ðÊ§°Ü£¬Í£Ö¹ÔËÐÐ
    }

    /* ÉèÖÃÉãÏñÍ·Í¼Ïñ4*4·ÖÆµÊä³öPCLK 27/4 = 6.75M ,BIT4,5¾µÏñÉèÖÃ:ÉÏÏÂ×óÓÒ¾ù¾µÏñ */
    MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW);

    /* ÉèÖÃÖ¡ÂÊ */
    MT9V034_SetFrameRate(fps);

    /* ÆØ¹âÉèÖÃ */
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //²Î¿¼µçÑ¹ÉèÖÃ   1.4v
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //·´Ïò¸¯Ê´
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);   //0x1C  here is the way to regulate darkness :)
    ////
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x13, 0x2D2E); //We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x20, CAMERA_CONTRAST); //0x01C7¶Ô±È¶È²î£¬·¢°×£»0x03C7¶Ô±È¶ÈÌá¸ß Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x24, 0x0010); //Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2B, 0x0003);    //Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2F, 0x0003);    //Improves FPN at near-saturation.

    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0103); //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.

    /* 0xA5  Í¼ÏñÁÁ¶È  60  1-64  */
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);

}

/*!
 * @brief    ´®¿ÚÉÏ±¨ÉÏÎ»»ú
 *
 * @param    ÎÞ
 *
 * @return   ÎÞ
 *
 ÉÏÎ»»úµÄÖ¡Í·¿ÉÄÜÓÐËùÇø±ð
 *
 * @see      CAMERA_Reprot();
 *
 * @date     2019/9/24 ÐÇÆÚ¶þ
 */
void CAMERA_Reprot (void)
{
    short j, i;

    IfxAsclin_Asc_blockingWrite(&g_UartConfig[1], 0xfe);
    IfxAsclin_Asc_blockingWrite(&g_UartConfig[1], 0xef);

    for (i = 0; i < IMAGEH; i++)
    {
        for (j = 0; j < IMAGEW; j++)
        {
            if (Image_Data[i][j] == 0xfe)  //·ÀÖ¹´íÎó·¢ËÍÖ¡Î²
            {
                Image_Data[i][j] = 0xff;
            }
            IfxAsclin_Asc_blockingWrite(&g_UartConfig[1], Image_Data[i][j]); //·¢ËÍÊý¾Ý
        }
    }
    IfxAsclin_Asc_blockingWrite(&g_UartConfig[1], 0xef);
    IfxAsclin_Asc_blockingWrite(&g_UartConfig[1], 0xfe);

}

/*!
 * @brief    ÉãÏñÍ·³õÊ¼»¯
 *
 * @param    fps:  Ö¡ÂÊ
 *
 * @return   ÎÞ
 *
 ÉãÏñÍ·µÄÒ»Ð©²ÎÊý£¬ÔÚLQ_MT9V034.cÖÐµÄºê¶¨ÒåÖÐÐÞ¸Ä
 *
 * @see      CAMERA_Init(50);   //³õÊ¼»¯MT9V034  50Ö¡ ×¢ÒâÊ¹ÓÃ°×É«´øÓë·ÇÃÅ°æ×ª½Ó×ù
 *
 * @date     2019/10/22 ÐÇÆÚ¶þ
 */
void CAMERA_Init (unsigned char fps)
{
    //¹Ø±ÕCPU×ÜÖÐ¶Ï
    IfxCpu_disableInterrupts();

    /* ³õÊ¼»¯ÉãÏñÍ· Êý¾ÝIO */
    PIN_InitConfig(P00_0, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_1, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_2, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_3, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_4, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_5, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_6, PIN_MODE_INPUT_PULLDOWN, 0);
    PIN_InitConfig(P00_7, PIN_MODE_INPUT_PULLDOWN, 0);

    MT9V034_Init(fps);

    /* DMA ´¥·¢Ô´³õÊ¼»¯ pclk */
    PIN_Exti(P02_1, PIN_IRQ_MODE_FALLING);

    /* DMA ³õÊ¼»¯ */
    DMA_CameraInitConfig((unsigned long) (&MODULE_P00.IN.U), (unsigned long) Image_Data, PIN_INT2_PRIORITY);

    /* Ê¹ÄÜ³¡ÖÐ¶Ï */
    PIN_Exti(P15_5, PIN_IRQ_MODE_RISING);

    /* ³õÊ¼»¯Íê³É ´ò¿ªÖÐ¶Ï */
    IfxCpu_enableInterrupts();

}

unsigned char CAMERA_GetFlag(void){
    return CAM_getFlag();
}

void CAMERA_ResetFlag(void){
    CAM_setFlag(0);
}
