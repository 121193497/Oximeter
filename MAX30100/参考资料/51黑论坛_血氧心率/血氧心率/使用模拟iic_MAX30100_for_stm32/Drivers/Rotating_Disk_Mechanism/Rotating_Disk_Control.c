/**
  ******************************************************************************
  * @file    Rotating_Disk_Control.c
  * @author  roc.li
  * @version V3.5.2
  * @date    24-December-2015
  * @brief   ת��Բ�̿���
	*
	*					 24-December-2015 
	*						��������Ҫ������ӵ�����������ת�ֿ���
  ******************************************************************************
**/

#include "Rotating_Disk_Control.h"


/*************************************��������*********************************/
tRotating_Disk	Rotating_Disk;
/*************************************��������*********************************/

/*************************************************
����: void Rotating_Disk_Control_GPIO_Config(void)
����: ת����λIO����
����: ��
����: ��
**************************************************/
void Rotating_Disk_Control_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#ifdef ROTATING_DISK_GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOD
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOE
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	#endif
	#ifdef ROTATING_DISK_GPIOF
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	#endif
	
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = ROTATING_DISK_LEFT_ZERO_POINT_PIN ;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��GPIOx*/
  	GPIO_Init(ROTATING_DISK_LEFT_ZERO_POINT_PORT, &GPIO_InitStructure);
	
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = ROTATING_DISK_RIGHT_ZERO_POINT_PIN ;	
	/*���ÿ⺯������ʼ��GPIOx*/
  	GPIO_Init(ROTATING_DISK_RIGHT_ZERO_POINT_PORT, &GPIO_InitStructure);
}
/*************************************************
����: void Rotating_Disk_Init(void)
����: ת�ֳ�ʼ��
����: ��
����: ��
**************************************************/
void Rotating_Disk_Init(void)
{
//	Moto_init();
//	/*��������λ��ģʽ����*/	
//	if(ROTATING_DISK_LEFT_ZERO_POINT == 0)
//	{
//		Moto_Enable1_Port(LOW);//����ʹ��
//		Moto_Enable2_Port(HIGH);//�ҵ����ʹ��
//		Moto_Direction_Port(HIGH);//���õ������
//		Start_Moto(ROTATING_DISK_TOTAL_PULSE,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//		while(ROTATING_DISK_LEFT_ZERO_POINT == 0);//�ȴ���ת�ֵ����
//		TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
//		TIM_Cmd(PULSE_TIM, DISABLE);
//		Moto_Stat =	STOP;
//		Frq_Change_Count = 0;
//	}
//	Rotating_Disk.Left_Number = 1;//��ת�ֱ��Ϊ1
//	Rotating_Disk.Left_Dirction = 1;
//	/*�ҵ������λ��ģʽ����*/	
//	if(ROTATING_DISK_RIGHT_ZERO_POINT == 0)
//	{
//		Moto_Enable1_Port(HIGH);//������ʹ��
//		Moto_Enable2_Port(LOW);//�ҵ��ʹ��
//		Moto_Direction_Port(HIGH);//���õ������
//		Start_Moto(ROTATING_DISK_TOTAL_PULSE,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//		while(ROTATING_DISK_RIGHT_ZERO_POINT == 0);//�ȴ���ת�ֵ����
//		TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
//		TIM_Cmd(PULSE_TIM, DISABLE);
//		Moto_Stat =	STOP;
//		Frq_Change_Count = 0;
//	}
//	Rotating_Disk.Right_Number = 1;//��ת�ֱ��Ϊ1
//	Rotating_Disk.Right_Dirction = 1;
}
/*************************************************
����: u16 Rotating_Disk_Init(u8 Left_Number, u8 Right_Number)
����: ת������ת������  
����: ��
����: ��
**************************************************/
u8 Rotating_Disk_Control(u8 Left_Number, u8 Right_Number)
{
//	u8 Roatating_Disk_Number;
	/*�������������*/
	if(Left_Number > 9) Left_Number = 9;
	if(Left_Number < 1) Left_Number = 1;
	if(Right_Number > 9) Right_Number = 9;
	if(Right_Number < 1) Right_Number = 1;
//	
//	/*�Ż�·��*/
//	if(Left_Number - Rotating_Disk.Left_Number <= ROTATING_DISK_LENS_NUMBER/2)//
//	{
//		Left_Number = Left_Number - Rotating_Disk.Left_Number;
//		Rotating_Disk.Left_Dirction = 1;
//	}
//	else 
//	{
//		Left_Number = ROTATING_DISK_LENS_NUMBER - Left_Number + Rotating_Disk.Left_Number;
//		Rotating_Disk.Left_Dirction = 0;
//	}
//	if(Right_Number - Rotating_Disk.Right_Number <= ROTATING_DISK_LENS_NUMBER/2)//
//	{
//		Right_Number = Right_Number -Rotating_Disk.Right_Number;
//		Rotating_Disk.Right_Dirction = 1;
//	}
//	else
//	{
//		Right_Number = ROTATING_DISK_LENS_NUMBER - Right_Number + Rotating_Disk.Right_Number;
//		Rotating_Disk.Right_Dirction = 0;
//	}
//	
//	Rotating_Disk.Target_Left_Pulse = (u32)((float)ROTATING_DISK_TOTAL_PULSE/(float)ROTATING_DISK_LENS_NUMBER*Left_Number);//�������ת��Ŀ��������
//	Rotating_Disk.Target_Right_Pulse = (u32)((float)ROTATING_DISK_TOTAL_PULSE/(float)ROTATING_DISK_LENS_NUMBER*Right_Number);//�������ת��Ŀ��������
//	/*��ת�ֹ���*/
//	Moto_Enable1_Port(LOW);//����ʹ��
//	Moto_Enable2_Port(HIGH);//�ҵ����ʹ��
//	if(Rotating_Disk.Left_Dirction == 1) 
//		Moto_Direction_Port(HIGH);//���õ������
//	else 
//		Moto_Direction_Port(LOW);//���õ������
//	Start_Moto(Rotating_Disk.Target_Left_Pulse,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//	while( Moto_Stat != STOP );//�ȴ����ֹͣ
//	/*��ת�ֹ���*/
//	Moto_Enable1_Port(HIGH);//������ʹ��
//	Moto_Enable2_Port(LOW);//�ҵ��ʹ��
//	if(Rotating_Disk.Right_Dirction == 1)
//		Moto_Direction_Port(HIGH);//���õ������
//	else
//		Moto_Direction_Port(LOW);//���õ������
//	Start_Moto(Rotating_Disk.Target_Right_Pulse,ROTATING_DISK_MOTO_START_SPEED,ROTATING_DISK_MOTO_END_SPEED);
//	while( Moto_Stat != STOP );//�ȴ����ֹͣ
//	
//	Roatating_Disk_Number = Rotating_Disk.Left_Number*10+Rotating_Disk.Right_Number;
//	return Roatating_Disk_Number;
}


