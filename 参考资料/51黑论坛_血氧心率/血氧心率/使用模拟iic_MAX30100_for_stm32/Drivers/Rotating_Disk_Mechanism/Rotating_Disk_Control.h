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
#ifndef __ROTATING_DISK_CONTROL__H_
#define __ROTATING_DISK_CONTROL__H_


/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "motos.h"
/***************************************************IO�ڶ���(�û��޸�)**********************************/
#define ROTATING_DISK_GPIOA																		//ת�����õ�GPIO
#define ROTATING_DISK_LEFT_ZERO_POINT_PORT		GPIOA
#define ROTATING_DISK_LEFT_ZERO_POINT_PIN     GPIO_Pin_1			//��ת�����IO
#define ROTATING_DISK_RIGHT_ZERO_POINT_PORT   GPIOA
#define ROTATING_DISK_RIGHT_ZERO_POINT_PIN    GPIO_Pin_2      //��ת�����IO
/***************************************************�������壨�û��޸ģ�********************************/
#define ROTATING_DISK_TOTAL_PULSE					307200              //ת��һȦ��������
#define ROTATING_DISK_LENS_NUMBER					9	                  //ת�־�Ƭ��
#define ROTATING_DISK_MOTO_START_SPEED		100									//ת�ֵ��������ʼ�ٶ�
#define ROTATING_DISK_MOTO_END_SPEED			20000								//ת�ֵ����������ٶ�

/* �˺��������ת����λ״̬�������޸�
 * 1 - ��
 * 0 - ��
 */
#define ROTATING_DISK_LEFT_ZERO_POINT		GPIO_ReadInputDataBit(ROTATING_DISK_LEFT_ZERO_POINT_PORT,ROTATING_DISK_LEFT_ZERO_POINT_PIN)//��ת�����
#define ROTATING_DISK_RIGHT_ZERO_POINT		GPIO_ReadInputDataBit(ROTATING_DISK_RIGHT_ZERO_POINT_PORT,ROTATING_DISK_RIGHT_ZERO_POINT_PIN)//��ת�����

/***************************************************���Ͷ���********************************************/
typedef struct
{
	u8 Left_Number; 					//��ת�ֱ��
	u8 Right_Number;					//��ת�ֱ��
	u8 Left_Dirction;					//��ת�ַ���
	u8 Right_Dirction;				//��ת�ַ���
	u32 Target_Left_Pulse;		//��ת��Ŀ��������
	u32 Target_Right_Pulse;		//��ת��Ŀ��������
	
}tRotating_Disk;

/***************************************************�������********************************************/

/***************************************************�������********************************************/
/*************************************************
����: void Rotating_Disk_Init(void)
����: ת����λIO����
����: ��
����: ��
**************************************************/
extern void Rotating_Disk_Init(void);
/*************************************************
����: u16 Rotating_Disk_Init(u8 Left_Number, u8 Right_Number)
����: ת������ת������  
����: ��
����: ��
**************************************************/
extern u8 Rotating_Disk_Control(u8 Left_Number, u8 Right_Number);



#endif
