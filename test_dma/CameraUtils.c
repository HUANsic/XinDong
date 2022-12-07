/*
 * CameraUtils.c
 *
 *  Created on: Oct 28, 2022
 *      Author: abcde
 */

#include "CameraUtils.h"

// EXI flags
const unsigned char PinIrqVectabNum[4] =
        {PIN_INT0_TypeOfService, PIN_INT1_TypeOfService, PIN_INT2_TypeOfService, PIN_INT3_TypeOfService};

// EXI priority
const unsigned char PinIrqPriority[4] = {PIN_INT0_PRIORITY, PIN_INT1_PRIORITY, PIN_INT2_PRIORITY, PIN_INT3_PRIORITY};

// ISR pointers
const void *PinIrqFuncPointer[4] = {&PIN_INT0_IRQHandler, &PIN_INT1_IRQHandler, &PIN_INT2_IRQHandler,
        &PIN_INT3_IRQHandler};

/**
 * 摄像头 DMA 配置结构体
 */
Dma_Camera_t g_DmaCameraLinkedList;

/** 摄像头 数据采集完成标志位 */
unsigned char Camera_Flag = 0;
unsigned char Image_Data[IMAGEH][IMAGEW];

/*!
 * @brief    摄像头DMA传输开启
 *
 * @param    channel  : DMA通道
 *
 * @return   无
 *
 * @note     注意   在场中断中再次开启DMA传输
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStart (unsigned long channel)
{
    if (Camera_Flag == 0)   //检测标志位是否清除   如果没有清除则不进行新的数据采集  避免覆盖未使用数据
    {

        /* DMA 设置传输目的地址  */
        IfxDma_setChannelDestinationAddress(&MODULE_DMA, channel,
                (void*) IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), Image_Data));

        /* 开启DMA传输  */
        IfxDma_enableChannelTransaction(&MODULE_DMA, channel);
    }
}

/*!
 * @brief    DMA传输停止
 *
 * @param    channel  : DMA通道
 *
 * @return   无
 *
 * @note     注意   在场中断中再次开启DMA传输
 *
 * @see      DMA_CameraStop(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraStop (unsigned long channel)
{
    /* 开启DMA传输 清除标志位 */
    IfxDma_disableChannelTransaction(&MODULE_DMA, channel);
    Camera_Flag = 0;
}

/*!
 * @brief    摄像头DMA传输中断处理函数
 *
 * @param    channel  : DMA通道
 *
 * @return   无
 *
 * @note     注意    一幅图需要进行两次DMA传输，因此会触发两次DMA中断
 *
 * @see      DMA_CameraIRQ(PIN_INT2_PRIORITY);
 *
 * @date     2020/4/8
 */
IFX_INLINE void DMA_CameraIRQ (unsigned long channel)
{
    static uint8 temp = 1;
    /* DMA 摄像头 中断服务函数 */
    Camera_Flag++;
    if (Camera_Flag == 2)
    {
        /* 摄像头一帧数据传输结束 停止dma传输  在场中断中再次开启DMA传输 */
        IfxDma_disableChannelTransaction(&MODULE_DMA, channel);

    }
    else
    {
        /* 清理dma传输请求  */
        (IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

        /* 设置DMA传输源地址 */
        IfxDma_setChannelDestinationAddress(&MODULE_DMA, channel,
                (void*) IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &Image_Data[60][0]));
    }
    if (temp == 0)
    {
        PIN_Write(P33_9, 0);
        temp = 1;
    }
    else
    {
        PIN_Write(P33_9, 1);
        temp = 0;
    }

}

inline unsigned char CAM_getFlag (void)
{
    return Camera_Flag;
}

inline void CAM_setFlag (unsigned char newFlag)
{
    Camera_Flag = newFlag;
}

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
void PIN_InitConfig (GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
{

    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* 配置 GPIO模式 */
    IfxPort_setPinMode(port, pinIndex, mode);

    IfxPort_setPinPadDriver(port, pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed2);

    /* GPIO输出模式时 输出状态 */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

void PIN_Exti (GPIO_Name_t pin, IfxPort_InputMode mode)
{
    int i, j;
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    // disable interrupts
    unsigned char interruptState = IfxCpu_disableInterrupts();

    IfxScu_Req_In *reqPin = 0;

    /// check pins
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
            {

            }
            else if ((uint32) port == (uint32) IfxScu_Req_In_pinTable[0][i][j]->pin.port
                    && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
            {
                reqPin = IfxScu_Req_In_pinTable[0][i][j];
            }
        }
    }

    if (!reqPin)
    {
        while (1)
            ;
    }

    // pull up/down
    IfxScuEru_initReqPin(reqPin, mode);

    // set channel
    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel) reqPin->channelId;

    // set up interrupt mode
    IfxScuEru_disableFallingEdgeDetection(inputChannel);
    IfxScuEru_disableRisingEdgeDetection(inputChannel);
    if (mode == IfxPort_InputMode_pullUp || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableFallingEdgeDetection(inputChannel);
    }
    if (mode == IfxPort_InputMode_pullDown || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableRisingEdgeDetection(inputChannel);
    }
    IfxScuEru_enableAutoClear(inputChannel);

    // signal destination
    IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer) inputChannel;
    IfxScuEru_OutputChannel outputChannel = (IfxScuEru_OutputChannel) inputChannel;

    // connecting matrix
    IfxScuEru_enableTriggerPulse(inputChannel);
    IfxScuEru_connectTrigger(inputChannel, triggerSelect);

    // output channels
    IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, 0);
    IfxScuEru_disablePatternDetectionTrigger(outputChannel);
    IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);

    uint8 vectabNum = PinIrqVectabNum[(int) inputChannel % 4];
    uint8 Priority = PinIrqPriority[(int) inputChannel % 4];

    // service req config
    {
        volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int) outputChannel % 4];
        IfxSrc_init(src, vectabNum, Priority);
        IfxSrc_enable(src);
    }

    // install int.
    IfxCpu_Irq_installInterruptHandler((void*) PinIrqFuncPointer[(int) inputChannel % 4], Priority);

    // enable interrupts
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  函数名称：unsigned char PIN_Read(GPIO_Name_t pin)
 *  功能说明：读取GPIO电平函数
 *  参数说明：pin      ： 管脚标号
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：uint8_t pinState = PIN_Read(P00_0);  //读取P00_0 的电平状态
 *************************************************************************/
unsigned char PIN_Read (GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);
    return IfxPort_getPinState(port, pinIndex);
}

/*************************************************************************
 *  函数名称：void PIN_Write(GPIO_Name_t pin, unsigned char output)
 *  功能说明：设置GPIO输出状态
 *  参数说明：pin      ： 管脚标号
 *            output   ： 输出模式时，输出电平 1：高电平  0：低电平
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Write(P00_0, 1);//P00_0置为高电平
 *************************************************************************/
void PIN_Write (GPIO_Name_t pin, unsigned char output)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* GPIO输出模式时 输出状态 */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

/*************************************************************************
 *  函数名称：void PIN_Dir(GPIO_Name_t pin, unsigned char mode)
 *  功能说明：设置GPIO 输入输出方向
 *  参数说明：pin      ： 管脚标号
 *            mode     ： GPIO 模式
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Dir(P00_0, 1);//P00_0设置为输出
 *************************************************************************/
void PIN_Dir (GPIO_Name_t pin, unsigned char mode)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    if (0 == mode)
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_INPUT);
    }
    else
    {
        /* 配置 GPIO模式 */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_OUTPUT);
    }
}

/*************************************************************************
 *  函数名称：void PIN_Reverse(GPIO_Name_t pin)
 *  功能说明：GPIO电平翻转函数，使用前请先初始化
 *  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
 *************************************************************************/
void PIN_Reverse (GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    IfxPort_togglePin(port, pinIndex);
}

void PIN_INT0_IRQHandler ()
{
    DMA_CameraStart(PIN_INT2_PRIORITY);
}
void PIN_INT1_IRQHandler ()
{
    ;
}
void PIN_INT2_IRQHandler ()
{
    ;
}
void PIN_INT3_IRQHandler ()
{
    ;
}

//安装中断服务函数，向量号和优先级
IFX_INTERRUPT(DMA_IRQHandler, DMA_TypeOfService, DMA_PRIORITY);

//安装中断服务函数
void DMA_IRQHandler (void)
{
    /* 开启CPU中断  否则中断不可嵌套 */
//  IfxCpu_enableInterrupts();
    DMA_CameraIRQ(PIN_INT2_PRIORITY);
}

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
 * @date     2020/4/8
 */
void DMA_CameraInitConfig (unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel)
{
    /* 关闭中断 */
    boolean interruptState = IfxCpu_disableInterrupts();

    /* create module config */
    IfxDma_Dma_Config dmaConfig;
    IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);

    /* initialize module */
    IfxDma_Dma dma;
    IfxDma_Dma_initModule(&dma, &dmaConfig);

    /* initial configuration for all channels */
    IfxDma_Dma_ChannelConfig cfg;
    IfxDma_Dma_initChannelConfig(&cfg, &dma);

//  cfg.busPriority   = IfxDma_ChannelBusPriority_high;                    //优先级最高
    cfg.requestMode = IfxDma_ChannelRequestMode_oneTransferPerRequest;   //请求启动一次传输
    cfg.moveSize = IfxDma_ChannelMoveSize_8bit;                       //一次move 8bit
    cfg.shadowControl = IfxDma_ChannelShadow_none;                         //单次模式
    cfg.operationMode = IfxDma_ChannelOperationMode_continuous;            //传输完成继续传输
    cfg.hardwareRequestEnabled = TRUE;                                     //使能硬件触发传输
    cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), srcStartAddr);  //设置源地址
    cfg.sourceCircularBufferEnabled = TRUE;                                      //保持源地址不变
    cfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_none;
    cfg.channelId = channel;                                                 //设置DMA通道
    cfg.channelInterruptEnabled = TRUE;
    cfg.channelInterruptPriority = DMA_PRIORITY;
    cfg.channelInterruptTypeOfService = DMA_TypeOfService;

    /* 设置传输表述符 */
    cfg.destinationAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), dstStartAddr);

    /* address to next transaction set */
    cfg.shadowAddress = 0;

    cfg.transferCount = 11280;

    /* transfer first transaction set into DMA channel */
    IfxDma_Dma_initChannel(&g_DmaCameraLinkedList.chn, &cfg);

    /* transfer into linked list storage */
    IfxDma_Dma_initLinkedListEntry((void*) &g_DmaCameraLinkedList.linkedList[0], &cfg);

    IfxCpu_Irq_installInterruptHandler((void*) DMA_IRQHandler, DMA_PRIORITY);

    /* clear service request flag */
    (IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

    IfxDma_clearChannelInterrupt(&MODULE_DMA, channel);

    IfxDma_disableChannelTransaction(&MODULE_DMA, channel);

    /* 开启中断 */
    IfxCpu_restoreInterrupts(interruptState);
}

