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
#include "motoS.h"
#include <stdio.h>
/************************************��������***********************************/
tMoto Moto;
uint16_t MotoS_Tab[Discrete_Accuracy];		//���߱�(����ΪS���ߵ�Ƶ�ʱ�HZ)
uint16_t *pMotoS_Tab;											//ָ�����߱��ָ��
__IO uint16_t Frq_Change_Count = 0;				//�ı�Ƶ�ʵĴ���
__IO enum STATE Change_Frq_Flag = FALSE;	//�ı�Ƶ�ʵı�־
__IO uint32_t Pulse_Count = 0;						//�����������
__IO enum MotoStat	Moto_Stat	=	STOP;			//���״̬
#ifdef POSITION_MODE
	__IO enum STATE AddState = FALSE;				//���������־
	__IO enum STATE StaticSpeedStage = FALSE;//���ٽ׶α�־λ
#endif
/*************************************��������*********************************/

/***************************************************
����  ����������ж���������
����  : ��
���  ����
����  ���ڲ�����
***************************************************/
static void NVIC_PULSE_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//�ȶ���ṹ�����
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����ж����ȼ�����  P.83
  
  /* �ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = 
	#ifdef PULSE_USE_TIM2
	TIM2_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM3
	TIM3_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM4
	TIM4_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM5
	TIM5_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM6
	TIM6_IRQn ;
	#endif
	#ifdef PULSE_USE_TIM7
	TIM7_IRQn ;
	#endif
	
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
void PULSE_TIM_Mode_Config(uint16_t frq)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(
	#ifdef PULSE_USE_TIM2
	RCC_APB1Periph_TIM2,
	#endif
	#ifdef PULSE_USE_TIM3
	RCC_APB1Periph_TIM3,
	#endif
	#ifdef PULSE_USE_TIM4
	RCC_APB1Periph_TIM4,
	#endif
	#ifdef PULSE_USE_TIM5
	RCC_APB1Periph_TIM5,
	#endif
	#ifdef PULSE_USE_TIM6
	RCC_APB1Periph_TIM6,
	#endif
	#ifdef PULSE_USE_TIM7
	RCC_APB1Periph_TIM7,
	#endif
	ENABLE);
	
	NVIC_PULSE_TIM_Configuration();
	
	TIM_DeInit(PULSE_TIM);//����ȱʡֵ
	TIM_TimeBaseInitStruct.TIM_Period = (uint16_t)(10000000/(frq*4.16667))-1;//��������
	TIM_TimeBaseInitStruct.TIM_Prescaler = 15-1;//10us�ļ�������Tcnt=72M/15=0.41666us
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(PULSE_TIM,&TIM_TimeBaseInitStruct);
	TIM_ARRPreloadConfig(PULSE_TIM,ENABLE); //������װ TIM_Period
	TIM_ClearFlag(PULSE_TIM, TIM_FLAG_Update);							    		/* �������жϱ�־ */
	TIM_ITConfig(PULSE_TIM,TIM_IT_Update,ENABLE);   
	//TIM_Cmd(PULSE_TIM, ENABLE);
}
/***************************************************
����  ����������Ƶ���¼��ж���������
����  : ��
���  ����
����  ���ڲ�����
***************************************************/
static void NVIC_PULSE_CHANGE_TIM_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;//�ȶ���ṹ�����
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�����ж����ȼ�����  P.83
  
  /* �ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = 
	#ifdef PULSE_CHANGE_USE_TIM2
	TIM2_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM3
	TIM3_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM4
	TIM4_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM5
	TIM5_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM6
	TIM6_IRQn;
	#endif
	#ifdef PULSE_CHANGE_USE_TIM7
	TIM7_IRQn;
	#endif
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
  NVIC_Init(&NVIC_InitStructure);//�ж�������ʼ��
}
/***************************************************
����  �����������Եĸ�������Ƶ��
����  : �ж�Ƶ��
���  ����
����  ���ڲ�����
***************************************************/
void PULSE_CHANGE_TIM_Mode_Config(uint16_t frq)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(
		#ifdef PULSE_CHANGE_USE_TIM2
			RCC_APB1Periph_TIM2 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM3
			RCC_APB1Periph_TIM3 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM4
			RCC_APB1Periph_TIM4 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM5
			RCC_APB1Periph_TIM5 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM6
			RCC_APB1Periph_TIM6 , 
		#endif
		#ifdef PULSE_CHANGE_USE_TIM7
			RCC_APB1Periph_TIM7 , 
		#endif
		ENABLE);
	
		NVIC_PULSE_CHANGE_TIM_Configuration();//�����ж�
	
    TIM_DeInit(PULSE_CHANGE_TIM);//����ȱʡֵ
    TIM_TimeBaseStructure.TIM_Period=(uint16_t)(100000/frq)-1;		/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= 720 - 1;				    /* ʱ��Ԥ��Ƶ�� 72M/720 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
	
    TIM_TimeBaseInit(PULSE_CHANGE_TIM, &TIM_TimeBaseStructure);
    TIM_ClearFlag(PULSE_CHANGE_TIM, TIM_FLAG_Update);							    		/* �������жϱ�־ */
    TIM_ITConfig(PULSE_CHANGE_TIM,TIM_IT_Update,ENABLE);
    //TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);																		/* ����ʱ�� *
}
/***************************************************
��飺Moto����˿�����
��������
���: ��
***************************************************/
void Moto_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOX������ʱ��*/
	#ifdef MOTO_GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	#endif
	#ifdef MOTO_GPIOB
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	#endif
	#ifdef MOTO_GPIOC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	#endif
	#ifdef MOTO_GPIOD
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	#endif
	#ifdef MOTO_GPIOE
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	#endif
	#ifdef MOTO_GPIOF
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	#endif	
	#ifdef JTAGDisable
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	#endif
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_PULSE_PIN ;	
	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	/*���ÿ⺯������ʼ��GPIOC*/
  	GPIO_Init(MOTO_PULSE_PORT, &GPIO_InitStructure);		
	
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_DIRECTION_PIN ;	
	/*���ÿ⺯������ʼ��GPIOx*/
  	GPIO_Init(MOTO_DIRCTION_PORT, &GPIO_InitStructure);
	
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_ENABLE1_PIN ;	 
	/*���ÿ⺯������ʼ��GPIOx*/
  	GPIO_Init(MOTO_ENABLE1_PORT, &GPIO_InitStructure);
		
	/*ѡ��Ҫ���Ƶ�GPIOx����*/	
		GPIO_InitStructure.GPIO_Pin = MOTO_ENABLE2_PIN ;	
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14 ;
	/*���ÿ⺯������ʼ��GPIOx*/
  	GPIO_Init(MOTO_ENABLE2_PORT, &GPIO_InitStructure);
//		GPIO_Init(GPIOA, &GPIO_InitStructure);

}
/***************************************************
�������ܣ�������������
�����������ʼƵ��(��С30HZ)��Ŀ��Ƶ��(���65000HZ)
������أ�NONE
***************************************************/
void Moto_Curve_Create(float Start_Frq,float Destination_Frq)
{
	uint16_t i;
	float T = 0;
	Moto.count = 0;
	pMotoS_Tab = MotoS_Tab;
	/*�Խṹ����и�ֵ*/
	Moto.Start_Frq = Start_Frq;
	Moto.Destination_Frq = Destination_Frq;
	/*������ز���*/
	Moto.Stay_Time = 2.0/Moto.Start_Frq;
	Moto.Inflection_Frq = (Moto.Start_Frq + Moto.Destination_Frq)/2.0;
	Moto.Inflection_Time = sqrt(2*(Moto.Inflection_Frq - Moto.Start_Frq)/(float)ACC);
	/*����S����*/
	while(T < Moto.Inflection_Time)//�յ�ǰ������
	{
		Moto.Frquency = Moto.Start_Frq + ACC*T*T/2.0;
		*pMotoS_Tab = (uint16_t)Moto.Frquency;//��ɢ��Ƶ�ʵ�Ž������
		pMotoS_Tab++;
		T+=Moto.Stay_Time;
		Moto.count++;
	}
	while(Moto.Frquency < Moto.Destination_Frq-1)//�յ�������
	{
		Moto.Frquency = 2*ACC*T*sqrt(2*(Moto.Inflection_Frq - Moto.Start_Frq)/(float)ACC)
										-ACC*T*T/2.0
										-2*Moto.Inflection_Frq
										+3*Moto.Start_Frq;
		*pMotoS_Tab = (uint16_t)Moto.Frquency;//��ɢ��Ƶ�ʵ�Ž������
		pMotoS_Tab++;
		T+=Moto.Stay_Time;
		Moto.count++;
	}
	pMotoS_Tab = MotoS_Tab;//��λָ�뱸��

	/*�Ӵ������Ƶ�ʱ�*/
	/*printf("\r\nƵ��ֵ\r\n");
	for(i=0;i<Moto.count;i++)
	{
		printf("%d,",MotoS_Tab[i]);
	}*/
	/*�ѱ���е�Ƶ��ת���ɶ�ʱ����ֵ*/
	//printf("\r\n��ʱ��ֵ\r\n");
	for(i=0;i<Moto.count;i++)
	{
		MotoS_Tab[i]=(uint16_t)(10000000/(MotoS_Tab[i]*4.16667));
		//printf("%d,",MotoS_Tab[i]);
	}
	/*�����񳤶�*/
	//printf("\r\n ��񳤶�=%d\r\n",Moto.count);
}

#ifdef SPEED_MODE
/***************************************************
�������ܣ����������Hz�����ٶ�ģʽ��
���������start_speed:���ٶ�(50min)��end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
void Start_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
		/*�������������*/
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if(Moto_Stat == STOP)//���ֹͣ״̬
	{
		/*�������߱�*/	
		Moto_Curve_Create(start_speed,end_speed);
		PULSE_CHANGE_TIM_Mode_Config((uint16_t)(2.0/Moto.Stay_Time));
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
		TIM_Cmd(PULSE_TIM, ENABLE);//������ʱ��
		Moto_Stat = ADD_SPEED;
	}
}
/***************************************************
�������ܣ�����ڹ���ʱ���٣�Hz�����ٶ�ģʽ��
���������start_speed:���ٶ�(50min)��end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
void ACC_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
	u8 i=0;
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if((Moto_Stat == FULL_SPEED) | (Moto_Stat == ADD_SPEED) | (Moto_Stat == RED_SPEED))
	{	
		Moto_Curve_Create(start_speed,end_speed);
		end_speed=(uint16_t)(10000000/(end_speed*4.16667));
		Frq_Change_Count = 0;
		for(i=0;i<Moto.count;i++)
		{
			if(MotoS_Tab[i]>Moto.NowSpeed)	
			{
				pMotoS_Tab++;
				Frq_Change_Count++;
			}
		}
		//
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��		
		Moto_Stat=ACC_SPEED;	//��������ڵ��ٶȣ���0�����ٶ�
	}
}
/***************************************************
�������ܣ�������٣�mm/min�����ٶ�ģʽ��
���������end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
void RED_Moto_Pulse(uint16_t end_speed)
{
	uint16_t i=0;
	end_speed=(uint16_t)(10000000/(end_speed*4.16667));
	if((Moto_Stat == FULL_SPEED)|(Moto_Stat == ADD_SPEED))//��������ٻ���ȫ������״̬
	{
		for(i=Moto.count;i>0;i--)
		{
			if(MotoS_Tab[i]<end_speed)
				Moto.count_RED=i;
		}
		//printf("%d,",Moto.count_i);
		Moto_Stat = RED_SPEED;//�������
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
	}
}
/***************************************************
�������ܣ����ֹͣ���ٶ�ģʽ���õ������ֹͣ�׶�
���������NONE
������أ�NONE
***************************************************/
void Stop_Moto(void)
{
	if((Moto_Stat == FULL_SPEED)|(Moto_Stat == ADD_SPEED))//��������ٻ���ȫ������״̬
	{
		Moto_Stat = DEC_SPEED;//�������
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
	}
}
/***************************************************
�������ܣ��������ֹͣ���ٶ�ģʽ���õ������ֹͣ�׶�
���������NONE
������أ�NONE
***************************************************/
void Right_Stop_Moto(void)
{
	TIM_Cmd(PULSE_TIM, DISABLE);
	Moto_Stat =	STOP;
	Frq_Change_Count = 0;
	//printf("\r\nstop!");
}

/***************************************************
�������ܣ�������� ��Hz�����ٶ�ģʽ���õ������ֹͣ�׶�
���������start_speed:���ٶ�(50min)��end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
void Work_Moto_Pulse(uint16_t start_speed,uint16_t end_speed)
{
	if(Moto_Stat==STOP)
		Start_Moto_Pulse(start_speed,end_speed);
	else if(Moto.nowSpeed > end_speed && end_speed != 0)
		RED_Moto_Pulse(end_speed);
	else if(Moto.nowSpeed < end_speed)
		ACC_Moto_Pulse(start_speed,end_speed);
	else if(end_speed == 0)		
		Stop_Moto();	
	Moto.nowSpeed=end_speed;
}
/***************************************************
�������ܣ�������� ��mm/min�����ٶ�ģʽ���õ������ֹͣ�׶�
���������end_speed:����ٶ�(2900mm/min)
������أ�NONE
***************************************************/
void Work_Moto_Speed(uint32_t end_speed)
{
	if(end_speed>1000)	end_speed = 1000;
	end_speed=end_speed*DIVIDE/60/WIREROD;
	Work_Moto_Pulse(50,end_speed); 
}
#endif
/***************************************************
�������ܣ����������λ��ģʽ��
���������step:Ҫ�ߵĲ���(����ָʵ�ʵ�������,��Ҫ����������ϸ��),
					start_speed:���ٶ�(50min)��end_speed:����ٶ�(65000max)
������أ�NONE
***************************************************/
#ifdef POSITION_MODE
void Start_Moto(uint32_t steps,uint16_t start_speed,uint16_t end_speed)
{
	
	/*�������������*/
	if(start_speed<50)		start_speed = 50;
	if(end_speed>65000)		end_speed = 65000;
	if(Moto_Stat == STOP)//���ֹͣ״̬
	{
	/*�������߱�*/	
		Moto_Curve_Create(start_speed,end_speed);
		Moto.Step_Total = steps;//��ֵ��Ҫ�ߵ��������
		Moto.Steps_Togo = steps;//��ֵ���µ�������
		Moto.Step_Total_Half = (uint32_t)(steps/2);//��ֵ��Ҫ�ߵ����������һ��
		PULSE_CHANGE_TIM_Mode_Config((uint16_t)(2.0/Moto.Stay_Time));
		Moto_Stat = ADD_SPEED;//���ٽ׶�
		TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
		TIM_Cmd(PULSE_TIM, ENABLE);//������ʱ��
	}
}
#endif

/***************************************************
�������ܣ������ʼ��
���������NONE
������أ�NONE
***************************************************/
void Moto_init(void)
{
	Moto_GPIO_Config();
	Moto_Pluse_Port(HIGH);
	Moto_Direction_Port(HIGH);
	Moto_Enable1_Port(HIGH);
	Moto_Enable2_Port(HIGH);
	PULSE_TIM_Mode_Config(50);
}

/***************************************************
��飺��������������ɺ������˺���Ӧ�ñ������ڶ�Ӧ���жϻص���
��������
���: ��
***************************************************/
void Moto_Pulse_Create(void)
{
	/*����������ٶ�ģʽ*/
	#ifdef SPEED_MODE
		Moto_Pluse_Port(TOGGLE);
		if(Change_Frq_Flag == TRUE)//��Ҫ�ı�Ƶ��
		{
			TIM_SetAutoreload(PULSE_TIM,*pMotoS_Tab);//����Ƶ��
			Change_Frq_Flag = FALSE;
		}
	#endif
	/*���������λ��ģʽ*/
	#ifdef POSITION_MODE
		Moto_Pluse_Port(TOGGLE);
		if(GPIO_ReadOutputDataBit(MOTO_PULSE_PORT,MOTO_PULSE_PIN) == 0)//������Ϊ��
		{
			Moto.Steps_Togo--;
			//usart_buf++;
			if((Moto.Steps_Togo == Moto.Step_Total_Half) && (AddState == FALSE))//δ������ٽ׶Σ����µ��������һ��
			{
				Moto_Stat = DEC_SPEED;//�������
				TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
			}
			else if((Moto.Steps_Togo > Moto.Step_Total_Half) && (AddState == TRUE) && (StaticSpeedStage == FALSE))//������ٽ׶Σ����µ��������һ�벽��
			{
				Moto.AddSpeed_Steps = Moto.Step_Total - Moto.Steps_Togo;				//�ó����ٵĲ���
				Moto.StaticSpeed_Steps = Moto.Steps_Togo - Moto.AddSpeed_Steps;	//�õ������˶�����
				StaticSpeedStage = TRUE;
			}
			else if(StaticSpeedStage == TRUE)//�ھ��ٽ׶�
			{
				if(Moto.Steps_Togo == (Moto.AddSpeed_Steps+1))//���¼��ٵ�����
				{
					Moto_Stat = DEC_SPEED;//�������
					TIM_Cmd(PULSE_CHANGE_TIM, ENABLE);//������ʱ��
				}
			}
			else if(Moto.Steps_Togo == 0)//���경��
			{
				TIM_Cmd(PULSE_TIM, DISABLE);//�ر��������
				Moto_Pluse_Port(HIGH);
				//printf("\r\n%d",usart_buf/32);
			}
		}
		if(Change_Frq_Flag == TRUE)//��Ҫ�ı�Ƶ��
		{
			TIM_SetAutoreload(PULSE_TIM,*pMotoS_Tab);//����Ƶ��
			Change_Frq_Flag = FALSE;
		}
	#endif
}
/***************************************************
��飺����Ƶ�ʵ��жϻص���Ӧ�÷����ڶ�Ӧ���ж���
��������
���: ��
***************************************************/
void Moto_Pulse_Change(void)
{
	/*����������ٶ�ģʽ*/
	#ifdef SPEED_MODE
		if(Moto_Stat == ADD_SPEED)//�������
		{
			if(Frq_Change_Count < Moto.count-1)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab++;//��һ����
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				//printf("\r\nStart!");
			}
		}
		else if(Moto_Stat == ACC_SPEED)//����ѷ�0���ٶȼ���
		{
			if(Frq_Change_Count < Moto.count-1)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab++;//��һ����
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				//printf("\r\nAcceleration!");
			}
		}
		else if(Moto_Stat == RED_SPEED)//�������
		{
			if(Frq_Change_Count > Moto.count_RED)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab--;//ָ����һ����
				Moto.NowSpeed=*pMotoS_Tab;
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == Moto.count_RED)//�������
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat =	FULL_SPEED;
				//printf("\r\nDeceleration!");
			}
		}
		else if(Moto_Stat == DEC_SPEED)//�������ֹͣ
		{
			if(Frq_Change_Count > 0)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab--;//ָ����һ����
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == 0)//�������
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				TIM_Cmd(PULSE_TIM, DISABLE);
				Moto_Stat =	STOP;
				//printf("\r\nstop!");
			}
		}
	#endif
	/*���������λ��ģʽ*/
	#ifdef POSITION_MODE
		if(Moto_Stat == ADD_SPEED)
		{
			if(Frq_Change_Count < Moto.count-1)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab++;//��һ����
				Frq_Change_Count++;
			}
			else if(Frq_Change_Count == Moto.count-1)
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				Moto_Stat = FULL_SPEED;
				AddState = TRUE;//��־��ɼ���
				//printf("\r\nadd end!");
			}
		}
		if(Moto_Stat == DEC_SPEED)
		{
			if(Frq_Change_Count > 0)//���ٶ�û����
			{
				Change_Frq_Flag = TRUE;//��־Ƶ����Ҫ����
				pMotoS_Tab--;//ָ����һ����
				Frq_Change_Count--;
			}
			else if(Frq_Change_Count == 0)//�������
			{
				TIM_Cmd(PULSE_CHANGE_TIM, DISABLE);
				TIM_Cmd(PULSE_TIM, DISABLE);
				Moto_Stat =	STOP;
			}
		}
	#endif
}
