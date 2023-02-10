/*
 * Asclin_app.c
 *
 *  Created on: 2018年1月2日
 *      Author: TEC
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "AsclinApp.h"
#include "Hardware.h"


/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
// used globally
static IfxAsclin_Asc asc;
#define ASC_TX_BUFFER_SIZE 64
static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
#define ASC_RX_BUFFER_SIZE 64
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
//IFX_INTERRUPT(asclin0TxISR, 0, IFX_INTPRIO_ASCLIN0_TX)
//{
//    IfxAsclin_Asc_isrTransmit(&asc);
//}
////IFX_INTERRUPT(asclin0RxISR, 0, IFX_INTPRIO_ASCLIN0_RX)
////{
////    IfxAsclin_Asc_isrReceive(&asc);
////}
//IFX_INTERRUPT(asclin0ErISR, 0, IFX_INTPRIO_ASCLIN0_ER)
//{
//    IfxAsclin_Asc_isrError(&asc);
//}

void AsclinAsc_init(void)
{

    /* disable interrupts */ //该行代码将CUP中断关闭并将状态传给interruptState进行表示
    IfxCpu_disableInterrupts();

    /* create module config */
    IfxAsclin_Asc_Config ascConfig;   //定义一个ascConfig的结构体，用于表示asc设置
    //IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN3); //用内置的ASCLIN1来初始化刚刚定义的asc结构体
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN1); //用内置的ASCLIN1来初始化刚刚定义的asc结构体
    /* set the desired baudrate */
    ascConfig.baudrate.baudrate = (float) BAUDRATE;
    ascConfig.frame.frameMode = IfxAsclin_FrameMode_asc;
    ascConfig.frame.dataLength = IfxAsclin_DataLength_8;
    ascConfig.frame.stopBit = IfxAsclin_StopBit_1;
    ascConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
    ascConfig.frame.parityBit = FALSE;
    // ISR priorities and interrupt target
    ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN0_TX;
    ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN0_RX;
    ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN0_ER;
    ascConfig.interrupt.typeOfService =  0;

    ascConfig.txBuffer = &ascTxBuffer;
    ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;

    ascConfig.rxBuffer = &ascRxBuffer;
    ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;


    /* pin configuration */ //设置引脚
    const IfxAsclin_Asc_Pins pins = {
        NULL,                     IfxPort_InputMode_pullUp,        /* CTS pin not used */
        //&IfxAsclin3_RXE_P00_1_IN, IfxPort_InputMode_pullUp,        /* Rx pin */
//      &IfxAsclin2_RXG_P02_0_IN, IfxPort_InputMode_pullUp,        /* Rx pin */
//        NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
        //&IfxAsclin3_TX_P00_0_OUT, IfxPort_OutputMode_pushPull,
        /* Tx pin */

        &IfxAsclin1_RXE_P11_10_IN, IfxPort_InputMode_pullUp,        /* Rx pin */
                NULL,                     IfxPort_OutputMode_pushPull,     /* RTS pin not used */
        &IfxAsclin1_TX_P11_12_OUT, IfxPort_OutputMode_pushPull,     /* Tx pin */
                IfxPort_PadDriver_cmosAutomotiveSpeed1
//      &IfxAsclin2_TX_P14_2_OUT, IfxPort_OutputMode_pushPull,     /* Tx pin */
//        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;


    /* initialize module */
    IfxAsclin_Asc_initModule(&asc, &ascConfig);
    IfxCpu_Irq_installInterruptHandler((void *)&Asclin_TX_IRQHandler, 131);
    IfxCpu_Irq_installInterruptHandler((void *)&Asclin_ER_IRQHandler, 132);
    IfxCpu_Irq_installInterruptHandler((void *)&Asclin_RX_IRQHandler, 130);


    //IfxCpu_enableInterrupts();
}

void Asclin_TX_IRQHandler(void)
{
 IfxAsclin_Asc_isrTransmit(&asc);

 /* 用户代码 */
}

void Asclin_ER_IRQHandler(void)
{
 IfxAsclin_Asc_isrError(&asc);

 /* 用户代码 */
}
void Asclin_RX_IRQHandler(void)
{
    IfxAsclin_Asc_isrReceive(&asc);
}


// 以下函数使用串口发送8位数据，数据格式为0x00到0xFF，若要发送字符，请自行转化为ascII码
void Asclin_Send_Data(uint8 Asclin_Send_Data)
{
    IfxAsclin_Asc_blockingWrite(&asc,Asclin_Send_Data);
    //调用串口函数发送函数，使用ASC1，发送一个8位数

}

// 以下函数使用异步串口ASC读取收到的8位信息，首先判断RX_FIFO里是否有数据，若有则进行数据读取,若没有数据，则返回0；
uint8 Asclin_Read_Data(void)
{
    if(IfxAsclin_Asc_getReadCount(&asc) >0)
    {
        return IfxAsclin_Asc_blockingRead(&asc);
    }

    return 0;

}


//发送字符串函数(遇 NULL 停止发送)
void Asclin_Send_String(char* Asclin_String)
{
  while(*Asclin_String)
  //如果length>0 说明没有发完 则继续发送 //如果TX的FIFO为空，则可以发送
  {
      Asclin_Send_Data(*Asclin_String++);
  }
}

char* num2str(int num)
{
    char str[10]={0};
    int wei=0;
    for(int inum=num;inum;inum/=10)
    {
        wei++;
    }
    for(int i=wei;num;num/=10,i--)
    {
        str[i]=num%10+48; //整数+48对应整数编号
    }
    return str;
 }

