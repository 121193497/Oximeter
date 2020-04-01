#include "ultrasonic.h"

u16 Ultrasonic_Time=0;	//����������ʱ��
u16 Ultrasonic_Number=0;

/***************************************************
����  ����������ж���������
����  : ��
���  ����
����  ���ڲ�����
***************************************************/
static void Ultrasonic_NVIC_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//�ȶ���ṹ�����
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����ж����ȼ�����  P.83
  
  /* �ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);//�ж�������ʼ��
}
/***************************************************
����  ���������������ʱ��
����  : ������������Ƶ��
���  ����
����  ���ڲ�����
***************************************************/
void Ultrasonic_TIM_Config(void)
{	
	/* ���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM5,ENABLE);
	
	Ultrasonic_NVIC_TIM_Configuration();//�����ж�
	
	TIM_DeInit(TIM5);//����ȱʡֵ
	TIM_TimeBaseInitStruct.TIM_Period = 2000;//��������
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(TIM5,ENABLE); //������װ TIM_Period
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);   
}

/*************************************************
����: NVIC_ConfigurationCoder(void)
����: ����Ƕ�������жϿ�����NVIC 
����: ��
����: ��
**************************************************/
void Ultrasonic_NVIC_GPIO_ConfigurationCoder(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);           //��ռʽ���ȼ�������Ϊ����ռ���ȼ�
	/*�ⲿ�ж�0��PA0��*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ָ���ж�Դ	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //ָ����Ӧ���ȼ���0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);				
}
void Ultrasonic_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		/*����LED������ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE); 

//PA11--ECHO
		/*ѡ��Ҫ���Ƶ�GPIOA11����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOA11*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
			
//PA10--TRIG
		/*ѡ��Ҫ���Ƶ�GPIOA10����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIOA10*/
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
		/* �رճ�����ģ��*/
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);

/* ����PA11Ϊ�ⲿ�ж�11����ͨ����EXIT11�� */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource11);
		EXTI_InitStructure.EXTI_Line = EXTI_Line11;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
		EXTI_Init(&EXTI_InitStructure);
		
		Ultrasonic_NVIC_GPIO_ConfigurationCoder();
	
}


void Ultrasonic_Init(void)
{
	Ultrasonic_TIM_Config();
	Ultrasonic_GPIO_Config();
}







