/******************************************************************************/
/*----------------------------------引用---------------------------------------*/
/******************************************************************************/
#include "UserSource.h"
#include "stdio.h"
int timecounter10 = 0;

/******************************************************************************/
/*---------------------------------用户变量定义-----------------------------------*/
/******************************************************************************/
uint8 ctldata = 0;
int CodePerid;
float distance;
///******************************************************************************/
///*----------------------------------用户函数------------------------------------*/
///******************************************************************************/
////函数及函数用法
void motor_duty(int duty) //样例用户自身舵机函数
{
    if (duty > 0)
        SetMotor(FORWARD, duty);
    else
        SetMotor(BACKWARD, -duty);
}
void steer_angle(int duty) {
    if (duty > 0)
        SetSteer(LEFT, duty);
    else if (duty < 0)
        SetSteer(RIGHT, -duty);
    else
        SetSteer(MIDDLE, duty);
}

void dutyTrans(int duty) {
    if (duty == 0){
        Bluetooth_Send_Data('0');
    }
    if (duty < 0)
        Bluetooth_Send_Data('-');
    duty=abs(duty);
    int stack[20];
    int stackP = 0;
    while(duty){
        stack[stackP++] = duty % 10;
        duty /= 10;
    }
    stackP--;
    while(stackP >= 0){
        Bluetooth_Send_Data((char)stack[stackP--] + '0');
    }
    Bluetooth_Send_Data(' ');
//    if(duty>=10000)
//    {
//        Bluetooth_Send_Data((char)(duty/10000)+'0');
//        duty=duty%10000;
//    }
//    Bluetooth_Send_Data((char)(duty/1000)+'0');
//    duty=duty%1000;
//    Bluetooth_Send_Data((char)(duty/100)+'0');
//    duty=duty%100;
//    Bluetooth_Send_Data((char)(duty/10)+'0');
//    duty=duty%10;
//    Bluetooth_Send_Data((char)(duty)+'0');
//    Bluetooth_Send_Data(' ');
}

/*****************************主函数***********************************/
//CPU0主函数，置于循环中用户主要逻辑计算区
void UserCpu0Main(void) //样例：蓝牙遥控小车
{
  uint8 a=0;
  int myduty=0,myangle=0;
  motor_duty(myduty);
  steer_angle(myangle);
  while(TRUE)
  {
      a=Bluetooth_Read_Data();
      if (a!=0)
          ctldata=a;
      switch (ctldata)
      {
      case 'W':
          myduty++;
          ctldata='w';
          motor_duty(myduty);
          steer_angle(myangle);
          dutyTrans(myduty);
          break;
      case 'S':
          myduty--;
          ctldata='s';
          motor_duty(myduty);
          steer_angle(myangle);
          dutyTrans(myduty);
          break;
      case 'A':
          myangle+=5;
          ctldata='a';
          motor_duty(myduty);
          steer_angle(myangle);
          dutyTrans(myangle);
          break;
      case 'D':
          myangle-=5;
          ctldata='d';
          motor_duty(myduty);
          steer_angle(myangle);
          dutyTrans(myangle);
          break;
      case 'E':
          myangle=0;
          myduty=0;
          ctldata='e';
          motor_duty(myduty);
          steer_angle(myangle);
          dutyTrans(myduty);
          break;
      case 'Z':
          ctldata='z';
          Bluetooth_Send_Data(ctldata);
          break;
      case 'X':
          ctldata='x';
          Bluetooth_Send_Data(ctldata);
          break;
      case 'C':
          ctldata='c';
          Bluetooth_Send_Data(ctldata);
          break;
      default:
          break;
      }

  }
}

//CPU1主函数，置于循环中，摄像头读写由此核处理，建议用于摄像头相关计算：
//不要写成死循环，后面有AD相关处理
void UserCpu1Main(void) {
//    if (CAMERA_GetFlag() == 2) // 当值为2时，图像数据加载完毕，方可进行图像读取及处理操作
//    {
//      PIN_Write(P33_9, 0);
//      delay_ms(10);
//      PIN_Write(P33_9, 1);
      delay_ms(100);
//      CAMERA_Reprot();  //将图片信息通过串口发送，不用时可注释掉，避免运行周期过长
//      CAMERA_ResetFlag();
//    }
}

/**************************************中断调用函数****************************************/
//该函数每10ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
uint32 UserInterupt10ms(void) {
    return 0;
}
//该函数每100ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
//样例，获取编码器输出频率与超声举例
uint32 UserInterupt100ms(void) {
    // 超声波
    distance = get_echo_length();
//    printf("distance %f",distance);  //可在FSS界面输出log，替代在watch界面查看distance
//    dutyTrans(distance);             //借助dutyTrans通过串口发送distance

    // 编码器
    CodePerid = GetCodePerid();
//    printf("CodePerid %f\n",CodePerid);  //可在FSS界面输出log，替代在watch界面查看CodePerid
//    dutyTrans(CodePerid);             //借助dutyTrans通过串口发送CodePerid

    //MPU6050
    dutyTrans(Roll);
    dutyTrans(Yaw);
    dutyTrans(Pitch);
    return 0;
}
//该函数每1000ms执行一次，请在该函数中书写程序，中断时间有限，不要太长
uint32 UserInterupt1000ms(void) {

    return 0;
}

void UserInteruptIO(void) {
    IfxPort_togglePin(LED1);
}
