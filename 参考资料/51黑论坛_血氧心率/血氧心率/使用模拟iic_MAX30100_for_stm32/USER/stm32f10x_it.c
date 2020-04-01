/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "motoS.h"
#include "ultrasonic.h"
#include <stdio.h>

u8 Ultrasonic_UP;	//��������������ر�ʶ



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void EXTI15_10_IRQHandler(void)
{
//�����������ж�	
	//��ʱ��ɣ�������ʱʱ��
	if ( EXTI_GetITStatus(EXTI_Line11) != RESET )
	{
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))//�½���
		{	
			if(Ultrasonic_UP==1)
			{				
				Ultrasonic_UP=0;
				/* ������ǰ����ֵ */
				Ultrasonic_Time = TIM_GetCounter(TIM5);								
				TIM_Cmd(TIM5 , DISABLE);	
				
			}
			else
			{
				TIM_Cmd(TIM5 , DISABLE);					
			}
		}
		//�յ������źŸߵ�ƽ����ʼ��ʱ
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11))//������
		{				
			Ultrasonic_UP=1;		//�����ر�ʶ
			Ultrasonic_Time=0;
			
			Ultrasonic_Number ++;
			TIM_SetCounter(TIM5,0);
			TIM_Cmd(TIM5 , ENABLE);				
		}
		EXTI_ClearFlag(EXTI_Line11);
		EXTI_ClearITPendingBit(EXTI_Line11);		    //���EXTI_Line0��·�����־λ   
	}
	
}
///*************************************************
//����: void TIM3_IRQHandler(void)
//����: ��ʱ��3�жϷ������
//����: ��
//����: ��
//**************************************************/
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) /*���TIM3�����жϷ������*/
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); /*���TIMx�����жϱ�־ */			
//		Moto_Pulse_Create();//�����ж����Բ�������
//	}
//} 

///*************************************************
//����: void TIM3_IRQHandler(void)
//����: ��ʱ��3�жϷ������
//����: ��
//����: ��
//**************************************************/
//void TIM4_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET ) /*���TIM3�����жϷ������*/
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update); /*���TIMx�����жϱ�־ */
//		Moto_Pulse_Change();//�����ж����Ըı�Ƶ��
//	}
//} 

void TIM5_IRQHandler(void)
{		
	if(TIM_GetITStatus(TIM5 , TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);			
	}
	
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
