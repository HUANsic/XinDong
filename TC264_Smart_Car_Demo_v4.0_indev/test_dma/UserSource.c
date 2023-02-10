/******************************************************************************/
/*----------------------------------����---------------------------------------*/
/******************************************************************************/
#include "UserSource.h"
#include "stdio.h"
int timecounter10 = 0;

/******************************************************************************/
/*---------------------------------�û���������-----------------------------------*/
/******************************************************************************/
uint8 ctldata = 0;
int CodePerid;
float distance;
///******************************************************************************/
///*----------------------------------�û�����------------------------------------*/
///******************************************************************************/
////�����������÷�
void motor_duty(int duty) //�����û������������
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

/*****************************������***********************************/
//CPU0������������ѭ�����û���Ҫ�߼�������
void UserCpu0Main(void) //����������ң��С��
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

//CPU1������������ѭ���У�����ͷ��д�ɴ˺˴�����������������ͷ��ؼ��㣺
//��Ҫд����ѭ����������AD��ش���
void UserCpu1Main(void) {
//    if (CAMERA_GetFlag() == 2) // ��ֵΪ2ʱ��ͼ�����ݼ�����ϣ����ɽ���ͼ���ȡ����������
//    {
//      PIN_Write(P33_9, 0);
//      delay_ms(10);
//      PIN_Write(P33_9, 1);
      delay_ms(100);
//      CAMERA_Reprot();  //��ͼƬ��Ϣͨ�����ڷ��ͣ�����ʱ��ע�͵��������������ڹ���
//      CAMERA_ResetFlag();
//    }
}

/**************************************�жϵ��ú���****************************************/
//�ú���ÿ10msִ��һ�Σ����ڸú�������д�����ж�ʱ�����ޣ���Ҫ̫��
uint32 UserInterupt10ms(void) {
    return 0;
}
//�ú���ÿ100msִ��һ�Σ����ڸú�������д�����ж�ʱ�����ޣ���Ҫ̫��
//��������ȡ���������Ƶ���볬������
uint32 UserInterupt100ms(void) {
    // ������
    distance = get_echo_length();
//    printf("distance %f",distance);  //����FSS�������log�������watch����鿴distance
//    dutyTrans(distance);             //����dutyTransͨ�����ڷ���distance

    // ������
    CodePerid = GetCodePerid();
//    printf("CodePerid %f\n",CodePerid);  //����FSS�������log�������watch����鿴CodePerid
//    dutyTrans(CodePerid);             //����dutyTransͨ�����ڷ���CodePerid

    //MPU6050
    dutyTrans(Roll);
    dutyTrans(Yaw);
    dutyTrans(Pitch);
    return 0;
}
//�ú���ÿ1000msִ��һ�Σ����ڸú�������д�����ж�ʱ�����ޣ���Ҫ̫��
uint32 UserInterupt1000ms(void) {

    return 0;
}

void UserInteruptIO(void) {
    IfxPort_togglePin(LED1);
}