/******************************************************************************/
/*****************************用户层********************/
/*****************************请不要更改此文件中的代码**********************************/

#include "ServeSource.h"
#include "UserSource.h"
#include "Hardware.h"
#include "Cpu/Irq/IfxCpu_Irq.h"
#include "SysSe/Bsp/Bsp.h"
#include "IfxScuWdt.h"
#include "Ifx_reg.h"
#include "IfxScuCcu.h"
#include "IfxCcu6.h"
#include "IfxCpu.h"
/***********************************gpio相关***********************************************/
int k1 = 1;
int k2 = 1;
int bo1 = 1;
int bo2 = 1;
int bo3 = 1;
int bo4 = 1;

void GpioInit(void)//gpio初始化
{
    //拨码开关
    IfxPort_setPinMode(DIP1, IfxPort_Mode_inputNoPullDevice);
    IfxPort_setPinMode(DIP2, IfxPort_Mode_inputNoPullDevice);
    IfxPort_setPinMode(DIP3, IfxPort_Mode_inputNoPullDevice);
    IfxPort_setPinMode(DIP4, IfxPort_Mode_inputNoPullDevice);

    //LED
    IfxPort_setPinMode(LED1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(LED4, IfxPort_Mode_outputPushPullGeneral);

    //按钮
    IfxPort_setPinMode(K1, IfxPort_Mode_inputNoPullDevice);
    IfxPort_setPinMode(K2, IfxPort_Mode_inputNoPullDevice);
}

int GetDip(int k) //获取某个编码器开关的io值
{
    switch (k)
    {
    case 1:
        return IfxPort_getPinState(DIP1);
    case 2:
        return IfxPort_getPinState(DIP2);
    case 3:
        return IfxPort_getPinState(DIP3);
    case 4:
        return IfxPort_getPinState(DIP4);
    default:
        return 2;
    }
}

int GetKey(int k) //获取某个按钮开关的io值
{
    switch (k)
    {
    case 1:
        return IfxPort_getPinState(K1);
    case 2:
        return IfxPort_getPinState(K2);
    default:
        return 2;
    }
}

void SetLed(int k, int light) //设置led灯
{
    switch (k)
    {
    case 1:
        IfxPort_setPinState(LED1, (!light << 16) | (light << 0));
        break;
    case 2:
        IfxPort_setPinState(LED2, (!light << 16) | (light << 0));
        break;
    case 3:
        IfxPort_setPinState(LED3, (!light << 16) | (light << 0));
        break;
    case 4:
        IfxPort_setPinState(LED4, (!light << 16) | (light << 0));
        break;
    }
}




///***********************************蓝牙串口相关***********************************************/
//详见AsclinApp.h AsclinApp.c


// 以下函数使用串口发送8位数据，数据格式为0x00到0xFF，若要发送字符，请自行转化为ascII码
void Bluetooth_Send_Data(uint8 data)
{
    Asclin_Send_Data(data);
}

// 以下函数使用异步串口ASC读取收到的8位信息，首先判断RX_FIFO里是否有数据，若有则进行数据读取,若没有数据，则返回0；
uint8 Bluetooth_Read_Data(void)
{
    return Asclin_Read_Data();

}
///*****************************************舵机电机相关************************************************************/
////详见PwmDemo.h PwmDemo.c
uint32 SetMotor(float direction, float pwmDuty)//电机操作函数 方向+占空比（%）
{
    if (direction == FORWARD)
    {
        PWM_Duty2 = PWM_Period1;
        if(pwmDuty>50)  //占空比限制
            pwmDuty=50;
        PWM_Duty1 = (int)(PWM_Period1-pwmDuty  * PWM_Period1/100);
    }
    else if (direction == BACKWARD)
    {
        PWM_Duty1 = PWM_Period1;
        if(pwmDuty>50)  //占空比限制
                pwmDuty=50;
        PWM_Duty2 = (int)( PWM_Period1-pwmDuty  * PWM_Period2/ 100);
    }
    return 0;
}

uint32 SetSteer(float direction, float pwmDuty)//舵机操作函数 方向+占空比（%）
{
    if (direction == LEFT)
    {
        PWM_Duty0 = (int)(MIDPWM* PWM_Period0 / 100-(MIDPWM - MAXLEFTPWM) * pwmDuty * PWM_Period0 / 10000);

    }
    else if (direction == RIGHT)
    {
        PWM_Duty0 = (int)(MIDPWM* PWM_Period0 / 100+(MAXRIGHTPWM - MIDPWM) * pwmDuty * PWM_Period0 / 10000);
    }
    else if (direction == MIDDLE)
    {
        PWM_Duty0 =  MIDPWM* PWM_Period0 / 100;
    }
    return 0;
}
///******************************捕获中断相关************************************************/
////以下详见TimDemo.h TimDemo.c
/////************************************编码器相关函数********************************************/
////
//
int GetCodePerid() //获取编码器频率 Hz
{
    if(CoderAflag==0)
        return IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_2, TRUE)/TimPwmPeridB;
    else
        return -1*IfxGtm_Cmu_getFxClkFrequency(&MODULE_GTM , IfxGtm_Cmu_Fxclk_2, TRUE)/TimPwmPeridB;
}
////
///************************************超声相关函数********************************************/
//


//产生超声触发信号，一个大于20us的高电平
uint8 capflag;
void cap_trigger(void)
{

    IfxPort_setPinState(Ultratrigger, IfxPort_State_high);
    delay_us(30);
    IfxPort_setPinState(Ultratrigger, IfxPort_State_low);
}
//获取超声引脚返回距离
float get_echo_length(void)
{
    float length;
    length= InDuty / 58 ; // mm
    return length;
}

/////************************************MPU6050相关函数********************************************/
float Yaw=0;
float Pitch=0;
float Roll=0;
//
//
//
/////*******************************************摄像头相关函数*******************************************************/
//void imageRead(void)//利用串口发送摄像头数据，与摄像头上位机配合
//{
//    uint16 m=0;
//    uint16 n=0;
//    uint16 k=0;
//
////  //暂时防止中断
//////    unsigned char pic[121][161];   //像素值
////  if((row>=150)&&(flag3>=5))
////  {
////      //uint8 sci_image_start[8]={1,254,1,254,1,254,1,254};
////      //      IfxCpu_disableInterrupts();
////      MODULE_GTM.TIM[0].CH1.CTRL.B.TIM_EN = 0;    /* Enable TIM0 Channle 0. */
////      MODULE_GTM.TIM[0].CH2.CTRL.B.TIM_EN = 0;    /* Enable TIM0 Channle 1. */
////      MODULE_GTM.TIM[1].CH3.CTRL.B.TIM_EN = 0;    /* Enable TIM0 Channle 2. */
////      //      Asclin_Send_String(sci_image_start,8);
//////        for(k=0;k<4;k++)
//////        {
////          //          SCI_Send(0x01);
//////            Asclin_Send_Data(0x01);
//////            delay(0.0001);
////          //          SCI_Send(0xfe);
//////            Asclin_Send_Data(0xfe);
//////            delay(0.0001);
//////        }
////      Asclin_Send_Data(0xff);
////      delay(0.001);
////      for(m=0;m<120;m++)
////      {
////          for(n=0;n<160;n++)
////          {
////              Asclin_Send_Data(pic[m][n]);
////              delay(0.0001);
////          }
////      }
////      flag3=0;
////      //      IfxCpu_enableInterrupts();
////      MODULE_GTM.TIM[0].CH1.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 0. */
////      MODULE_GTM.TIM[0].CH2.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 1. */
////      MODULE_GTM.TIM[1].CH3.CTRL.B.TIM_EN = 1;    /* Enable TIM0 Channle 2. */
////  }
//}
//

///*********************************定时刷新用服务函数*********************************************/

//uint32 test7=0,test8=0;

uint32 ServeInterupt10ms(void)
{
//    //舵机电机刷新函数10ms
    IfxGtm_Tom_Ch_setCompareOne(&MODULE_GTM.TOM[1],  IfxGtm_Tom_Ch_15, PWM_Duty0);
    IfxGtm_Tom_Ch_setCompareOne(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_2, PWM_Duty1);
    IfxGtm_Tom_Ch_setCompareOne(&MODULE_GTM.TOM[0], IfxGtm_Tom_Ch_3, PWM_Duty2);
//
////  test7++;
    return 0;
}

uint32 ServeInterupt100ms(void)//
{
    //陀螺仪100ms积分函数
    //获取数据
     MPU6050_Get_Data();
     //标定，标定后可注释
     MPU6050_Calibrate();
     //积分
     MPU6050_int(0.1);
//     dutyTrans(mpudata_int[6]);
     //航向角
     Roll=mpudata_int[4];
     Pitch=mpudata_int[5];
     Yaw=mpudata_int[6];
//IO测试
//  k1 = GetKey(1);
//  k2 = GetKey(2);
//
//  bo1 = GetDip(1);
//  bo2 = GetDip(2);
//  bo3 = GetDip(3);
//  bo4 = GetDip(4);
//
//  SetLed(1,GetDip(1));
//  SetLed(2,GetDip(2));
//  SetLed(3,GetDip(3));
//  SetLed(4,GetDip(4));
    cap_trigger();
    return 0;
}

uint32 ServeInterupt1000ms(void)
{
    //1s执行的函数
//      cap_trigger();
    //VADCresult_run();

    return 0;
}

void delay_us(uint32 time)
{
    IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0, time));
}

void delay_ms(uint32 time)
{
    IfxStm_waitTicks(&MODULE_STM0, IfxStm_getTicksFromMicroseconds(&MODULE_STM0, 1000*time));
}
