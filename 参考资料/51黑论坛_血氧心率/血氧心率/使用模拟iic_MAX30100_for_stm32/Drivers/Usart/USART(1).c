#include "usart.h"
#include "delay.h"
#include <stdio.h>

/*******�������ڱ�־λ******/
struct a1 UsartBit;
/*******�����������ݰ�******/
u8 USART1_Data[13];	//����1�Ľ������ݰ�
u8 USART1_i=0;      //����1���ݰ��±�

/*******************************************************************************
* Function Name   : NVIC_Configuration
* Description        : Configures NVIC and Vector Table base location.
* Input                    : None
* Output                 : None
* Return                 : None
*******************************************************************************/
void NVIC_ConfigurationUSART(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;//�ж���Ӧ���ȼ�1  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//���ж�
  NVIC_Init(&NVIC_InitStructure);//��ʼ��
}
/*******************************************************************************
	��������USART1_Configuration
	��  ��:
	��  ��:
	����˵����
	��ʼ������Ӳ���豸�������ж�
	���ò��裺
	(1)��GPIO��USART1��ʱ��
	(2)����USART1�����ܽ�GPIOģʽ
	(3)����USART1���ݸ�ʽ�������ʵȲ���
	(4)ʹ��USART1�����жϹ���
	(5)���ʹ��USART1����
*******************************************************************************/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ��1������GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* ��4��������USART1����
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

    /* ���������ݼĴ�������������ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* ��5����ʹ�� USART1�� ������� */
	USART_Cmd(USART1, ENABLE);

    /* �����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
    USART_ClearFlag(USART1, USART_FLAG_TC);     // ���־
}
/*************************************************
����: Usart_Init(void)
����: ���ڳ�ʼ��
����: ��
����: ��
**************************************************/
void Usart_Init(void)
{
	NVIC_ConfigurationUSART();//����NVIC�����������λ��
	USART1_Configuration();//��ʼ������1Ӳ���豸�������ж�
}
/*************************************************
����: Usart1_PutChar(u8 ch)
����: ����1����1�ֽ�
����: ��
����: ��
**************************************************/
void Usart1_PutChar(u8 ch)
{
  
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, (u8) ch);
	delay_us(100);
} 
/*******************************************************************/
/*                                                                 */
/* STM32�ڴ���1����1�ֽ�                                           */
/* ˵��������1�����ж�                                             */
/*                                                                 */
/*******************************************************************/
void USART1_IRQHandler(void) //���жϷ�������У�����������Ӧ�ж�ʱ����֪�����ĸ��ж�Դ�����ж�������˱������жϷ�������ж��ж�Դ�����б�Ȼ��ֱ���д�����Ȼ�����ֻ�漰��һ���ж������ǲ����������б�ġ���������ʲô������������б��Ǹ���ϰ��
{
	//u8 data;   
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //���������ݼĴ�����
	{     
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	                                          			   
	}
}
