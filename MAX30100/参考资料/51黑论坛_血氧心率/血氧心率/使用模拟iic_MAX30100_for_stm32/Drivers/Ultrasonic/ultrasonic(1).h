#ifndef __ULTRASONIC__H__
#define __ULTRASONIC__H__
/***************************************************include*********************************************/
#include "stm32f10x.h"
#include "stm32f10x_tim.h"


/**************************************************�������IO�ڶ���(�û��޸�)***************************/


extern u16 Ultrasonic_Time;	//����������ʱ��
extern u16 Ultrasonic_Number;


extern void Ultrasonic_Init(void);

#endif
