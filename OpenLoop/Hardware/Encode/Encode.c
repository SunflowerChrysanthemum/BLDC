#include "sys.h"

void Time_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_InternalClockConfig(TIM4);
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 720 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 100  - 1;   //PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM4,ENABLE);
}


/**
 * @brief       检测输入信号是否发生变化
 * @param       val :输入信号
 * @note        测量速度使用，获取输入信号状态翻转情况，计算速度
 * @retval      0：计算高电平时间，1：计算低电平时间，2：信号未改变
 */
uint8_t check_edge(uint8_t val)
{
    /* 主要是检测val信号从0 - 1 在从 1 - 0的过程，即高电平所持续的过程 */
    static uint8_t oldval=0;
    if(oldval != val)
    {
        oldval = val;
        if(val == 0) return 0;
        else return 1;
    }
    return 2;
}
