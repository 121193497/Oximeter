#include "Other_IO_Settings.h"
/*************************************************
����: void Other_GPIO_Configuration(void��
����: GPIO����
����: ��
����: ��
**************************************************/
void Other_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;        //����GPIO��ʼ���ṹ��
	
	/******ָʾ��******/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_11 |GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		//�ø�
	GPIO_SetBits(GPIOC,GPIO_Pin_12);		//�ø�

	//�������Ͱ�ɴ�ܵ��IO����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_11; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
