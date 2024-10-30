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
//    TIM_InternalClockConfig(TIM4);//TIM4������
//    
//    //����ʱ����Ԫ
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//�����ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR��TIx)ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
//    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;       //ARR����ֹ������Ĵ���ֵ����
//    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;       //PSC�����Ƶ��= 72MHz / 72= 100KHz������׼Ƶ��
//    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
//    
//    TIM_ICInitTypeDef TIM_ICInitStructure;
//    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;//ѡ��TIM4��ͨ��1
//    TIM_ICInitStructure.TIM_ICFilter = 0xF;//����ѡ�����벶����˲�����һ���ֵԽ���˲�Ч��Խ��
//    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//���ؼ�⼫��ѡ��
//    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//�˴�����Ƶ������Ƶ����ÿ�δ�������Ч��2��Ƶ����ÿ������������Чһ�Σ��Դ�����
//    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//�˴�ѡ��ֱ��ͨ������������ѡ����������ѡ��ֱ��ͨ�����߽���ͨ��
//    TIM_ICInit(TIM4, &TIM_ICInitStructure);
//    
//    TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);//���ô���ģʽTRGI�Ĵ���ԴΪTI1FP1
//    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);//���ô�ģʽΪReset�������������㣬�Ա����¼���
//    
//    TIM_Cmd(TIM4, ENABLE);
//}


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
