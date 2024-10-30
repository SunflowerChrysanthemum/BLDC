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


//void IC_Init(void)
//{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    
//    TIM_InternalClockConfig(TIM4);//TIM4参数表
//    
//    //配置时基单元
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//定义在定时器时钟(CK_INT)频率与数字滤波器(ETR，TIx)使用的采样频率之间的分频比例
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;       //ARR，防止溢出将寄存器值拉满
//    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;       //PSC，输出频率= 72MHz / 72= 100KHz，即标准频率
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
//    
//    TIM_ICInitTypeDef TIM_ICInitStructure;
//    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//选用TIM4的通道1
//    TIM_ICInitStructure.TIM_ICFilter = 0xF;//用于选择输入捕获的滤波器，一般地值越大滤波效果越好
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//边沿检测极性选择
//    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//此处不分频，不分频就是每次触发都有效，2分频就是每两次上升沿有效一次，以此类推
//    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//此处选择直连通道，配置数据选择器，可以选择直连通道或者交叉通道
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);
//    
//    TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);//配置触发模式TRGI的触发源为TI1FP1
//    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);//配置从模式为Reset，给计数器清零，以便重新计数
//    
//    TIM_Cmd(TIM4, ENABLE);
//}


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
