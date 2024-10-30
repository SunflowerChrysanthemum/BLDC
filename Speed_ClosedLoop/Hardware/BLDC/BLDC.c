#include "sys.h"

void pwm_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//定时器1通道1、2、3引脚IO初始化
	//控制正极，pwm控制就可以利用占空比控制电机速度
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//定时器1互补通道引脚IO初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//刹车引脚IO初始化， 浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//定时器1输出PWM模块初始化
	//总频率72mhz, 向上计数， 72mhz/3600=20khz, 即周期50us
	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStructure;
	TIM1_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM1_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM1_TimeBaseInitStructure.TIM_Period = PWM_PERIOD - 1;
	TIM1_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; 
	TIM1_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseInitStructure);
	
	
	//定时器比较输出模式1
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	
	//不要产生更新事件才改变设置;换相会有延迟
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

	
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void BLDC_HALInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}



//霍尔#1	霍尔#1		霍尔#1
//  1		  0		 	  1				5
//  1		  0			  0				4
//  1		  1			  0				6
//  0		  1			  0				2
//  0		  1			  1				3
//  0		  0			  1				1

void BLDC_Stop(void)
{
	TIM_CtrlPWMOutputs(TIM1, DISABLE);
	TIM_SetCompare1(TIM1, 0);
	TIM_SetCompare1(TIM1, 0);
	TIM_SetCompare1(TIM1, 0);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void BLDC_Start(void)
{
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void BLDC_FirstStart(void)
{
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
	TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
	TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	delay_ms(20);
}

unsigned char GetHall(void)
{
	static volatile int8_t HallState_pre = 0;
	unsigned char ret = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) != RESET)
	{
		ret |= 0x01;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) != RESET)
	{
		ret |= 0x02;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) != RESET)
	{
		ret |= 0x04;
	}
	if(HallState_pre != ret)
	{
		step_count++;
	}
	HallState_pre = ret;
	
	return ret;
}

void BLDC_Control(uint8_t dir_t,uint16_t duty)
{
	dir = dir_t;            /* 方向 */
    pwm_duty = duty;      /* 占空比 */
}

void BLDC_Change(uint8_t step_now)
{
	switch(step_now)
	{
		case 6: //b+  c-
		{
			TIM_SetCompare1(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);

			break;
		}
		
		case 2: //b+ a-
		{
			TIM_SetCompare1(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			
			break;
		}
		
		case 3: //c+ a-
		{
			TIM_SetCompare1(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			
			break;
		}
		
		case 1: //c+ b-
		{
			TIM_SetCompare1(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			
			break;
		}
		
		case 5: //a+ b-
		{
			TIM_SetCompare1(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			
			break;
		}
		
		case 4: //a+ c-
		{
			TIM_SetCompare1(TIM1, pwm_duty);
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			
			TIM_SetCompare2(TIM1, 0);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			
			TIM_SetCompare3(TIM1, 0);
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
			
			break;
		}
		
		default:
		{
			
			TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
			TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
			TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
	
			GPIO_SetBits(GPIOB, GPIO_Pin_13);
			GPIO_SetBits(GPIOB, GPIO_Pin_14);
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			printf("0\n");
			break;
		}
	}
}







