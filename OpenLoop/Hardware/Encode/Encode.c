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
 * @brief       ��������ź��Ƿ����仯
 * @param       val :�����ź�
 * @note        �����ٶ�ʹ�ã���ȡ�����ź�״̬��ת����������ٶ�
 * @retval      0������ߵ�ƽʱ�䣬1������͵�ƽʱ�䣬2���ź�δ�ı�
 */
uint8_t check_edge(uint8_t val)
{
    /* ��Ҫ�Ǽ��val�źŴ�0 - 1 �ڴ� 1 - 0�Ĺ��̣����ߵ�ƽ�������Ĺ��� */
    static uint8_t oldval=0;
    if(oldval != val)
    {
        oldval = val;
        if(val == 0) return 0;
        else return 1;
    }
    return 2;
}
