/**
  ******************************************************************************
  * @file    motoS.c
  * @author  roc.li
  * @version V3.5.2
  * @date    24-December-2015
  * @brief   �������S������������
  *
	*          26-December-2014 
	*					  �ڴ��ļ��У�����V3.5.0�����޸ģ���Ҫ�޸����ٶ�ģʽ�µĵ���ٶȵı仯
	*            1.���ӵ����������Ƶ����˿���ݾ���������
	*            2.�������ٶȣ�mm/min��ת����Ƶ�ʣ�Hz���ĺ���
	*            3.���ӵ���������ٶ�����ı�ĺ���
	*
	*					 24-December-2015 
	*						�ڴ��ļ��У�����V3.5.1�����޸ģ���Ҫ�޸���motoӦ�����Ŷ��巽ʽ��λ��
	*						ģʽ�µĵ��������ʽ
	*            1.moto���������õ�GPIOA��GPIOD��GPIOE����ʱ��ԭ��ֻ���漰GPIOA��
	*							 ������һ����ͬʱ���������õ�JTAG����ʱ������
	*            2.��λ��ģʽ�µ������ٶȺ�����ٶ���������
	*            3.��λ��ģʽ��Ҫ���������ڼ��ٺͼ����з�������
  ******************************************************************************
**/
#ifndef __MOTOS__H__
#define __MOTOS__H__
/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include <math.h>
#include <stdio.h>

/**************************************************�������IO�ڶ���(�û��޸�)***************************/
#define MOTO_GPIOA																		//������õ�GPIO
#define MOTO_GPIOD																		//������õ�GPIO
#define MOTO_GPIOE																		//������õ�GPIO
#define JTAGDisable                                   //JTAG����
#define MOTO_PULSE_PORT					GPIOE
#define MOTO_PULSE_PIN					GPIO_Pin_2						//�������IO
#define MOTO_DIRCTION_PORT			GPIOD
#define MOTO_DIRECTION_PIN			GPIO_Pin_3				  	//�������IO
#define MOTO_ENABLE1_PORT				GPIOD									
#define MOTO_ENABLE1_PIN				GPIO_Pin_1						//���1ʹ��IO
#define MOTO_ENABLE2_PORT				GPIOA							
#define MOTO_ENABLE2_PIN				GPIO_Pin_15						//���2ʹ��IO
/*���ö�ʱ����Χ��TIM2-TIM7*/
#define PULSE_USE_TIM3																//����������������õ�ʱ��
#define PULSE_CHANGE_USE_TIM4													//��������Ƶ�ʸ������õ�ʱ��
#define PULSE_TIM								TIM3									//��������Ķ�ʱ��
#define PULSE_CHANGE_TIM				TIM4									//�ı�Ƶ�ʵĶ�ʱ��
/***************************************************�������壨�û��޸ģ�********************************/
#define Discrete_Accuracy 500							//��ɢ����
#define ACC								5000						//�Ӽ��ٶ�,�̶�ֵ��ͨ������ʵ��ȷ��,��ֵ�����˵���Ŀ���ٶȵ�ʱ��
#define DIVIDE						160						//�����������Ƶ��
#define WIREROD           1               //˿���ݾ�(mm)
#define POSITION_MODE									  	//����λ��ģʽ
//#define SPEED_MODE										 	//�����ٶ�ģʽ  ���϶�ѡһ

/* �˺��������õ���Ŀ����ߣ������޸�
 * 1 - ��
 - 0 - ��
 */
#define HIGH  		0
#define LOW 			1
#define TOGGLE 		2
#define Moto_Pluse_Port(a)	if (a==HIGH)	\
														GPIO_SetBits(MOTO_PULSE_PORT,MOTO_PULSE_PIN);\
														if(a==LOW)		\
														GPIO_ResetBits(MOTO_PULSE_PORT,MOTO_PULSE_PIN);\
														if(a==TOGGLE)		\
														GPIO_WriteBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN,(BitAction)(1-GPIO_ReadOutputDataBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN)))
						
#define Moto_Direction_Port(a)	if (a==HIGH)	\
																GPIO_SetBits(MOTO_DIRCTION_PORT,MOTO_DIRECTION_PIN);\
																if(a==LOW)		\
																GPIO_ResetBits(MOTO_DIRCTION_PORT,MOTO_DIRECTION_PIN)

#define Moto_Enable1_Port(a)	if (a==HIGH)	\
															GPIO_SetBits(MOTO_ENABLE1_PORT,MOTO_ENABLE1_PIN);\
															if(a==LOW)		\
															GPIO_ResetBits(MOTO_ENABLE1_PORT,MOTO_ENABLE1_PIN)

#define Moto_Enable2_Port(a)	if (a==HIGH)	\
															GPIO_SetBits(MOTO_ENABLE2_PORT,MOTO_ENABLE2_PIN);\
															if(a==LOW)		\
															GPIO_ResetBits(MOTO_ENABLE2_PORT,MOTO_ENABLE2_PIN)
/****************************************************���Ͷ���******************************************/
typedef struct
{
	float 	 Start_Frq;					//��ʼƵ��(Hz)
	float 	 Destination_Frq;		//Ŀ��Ƶ��(Hz)							
	float 	 Stay_Time;					//��ɢ���ʱ��
	float		 Inflection_Frq;		//�յ�Ƶ��
	float    Inflection_Time;   //�յ�ʱ��
	float		 Frquency;					//���Ƶ��
	uint16_t count;							//��ɢ�������
	uint16_t count_RED;         //������ٵ��ٶ������߱��е��±�
	uint16_t NowSpeed;					//�����ٶȣ���ʱ����ֵ��
	uint16_t nowSpeed;          //��ǰ�ٶȣ�mm/min��
	#ifdef POSITION_MODE
		uint32_t Step_Total;							//�ܵ�Ҫ�ߵ�������
		uint32_t Steps_Togo;							//������µ�������
		uint32_t Step_Total_Half;					//���Ҫ�ߵ���������һ��
		uint32_t AddSpeed_Steps;					//���ٲ���
		uint32_t StaticSpeed_Steps;				//�����˶��Ĳ���
	#endif
}tMoto;

enum STATE
{
	TRUE = 0,
	FALSE	
};
enum MotoStat
{
	STOP  = 0,				//���ֹͣ
	ADD_SPEED,				//�����������
	DEC_SPEED,				//�������
	RED_SPEED,        //�������
	ACC_SPEED,				//�������
	FULL_SPEED				//�������
};
/***************************************************�������*********************************/
extern uint16_t MotoS_Tab[Discrete_Accuracy];//���߱�(����ΪS���ߵ�Ƶ�ʱ�HZ)
extern tMoto Moto;
extern uint16_t *pMotoS_Tab;
extern uint8_t flag;
extern __IO enum STATE Change_Frq_Flag;
extern __IO uint16_t Frq_Change_Count;			//�ı�Ƶ�ʵĴ���
extern __IO uint8_t	Change_Level_Flag;			//���ķ��������־
extern __IO uint32_t Pulse_Count;						//�����������
extern __IO enum MotoStat	Moto_Stat;				//���״̬
#ifdef POSITION_MODE
	extern __IO enum STATE AddState;				//���������־
	extern __IO enum STATE StaticSpeedStage;//���ٽ׶α�־λ
#endif
/****************************************************�������******************************************/
/***************************************************
�������ܣ������ʼ��
���������NONE
������أ�NONE
***************************************************/
extern void Moto_init(void);
#ifdef SPEED_MODE
/***************************************************
�������ܣ�������� ��Hz�����ٶ�ģʽ���õ������ֹͣ�׶�
���������end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
extern void Work_Moto_Pulse(uint16_t start_speed,uint16_t end_speed);
/***************************************************
�������ܣ�������� ��mm/min�����ٶ�ģʽ���õ������ֹͣ�׶�
���������end_speed:����ٶ�(2000mm/min)
������أ�NONE
***************************************************/
extern void Work_Moto_Speed(uint32_t end_speed);
/***************************************************
�������ܣ��������ֹͣ���ٶ�ģʽ���õ������ֹͣ�׶�
���������NONE
������أ�NONE
***************************************************/
extern void Right_Stop_Moto(void);
#endif

#ifdef POSITION_MODE
/***************************************************
�������ܣ����������λ��ģʽ��
���������step:Ҫ�ߵĲ���(������),start_speed:���ٶ�(50min)��end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
extern void Start_Moto(uint32_t steps,uint16_t start_speed,uint16_t end_speed);
#endif
/***************************************************
��飺��������������ɺ������˺���Ӧ�ñ������ڶ�Ӧ���жϻص���
��������
���: ��
***************************************************/
extern void Moto_Pulse_Create(void);//�����ж����Բ�������
/***************************************************
��飺����Ƶ�ʵ��жϻص���Ӧ�÷����ڶ�Ӧ���ж���
��������
���: ��
***************************************************/
extern void Moto_Pulse_Change(void);//�����ж����Ըı�Ƶ��
#endif
