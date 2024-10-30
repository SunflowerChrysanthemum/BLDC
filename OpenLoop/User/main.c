#include "sys.h"

//电机为4极对
int8_t  MotorRun = 0;

uint8_t hall_single_sta = 0;

uint16_t pwm_duty = 0;
uint8_t dir = cw;
float Velocity_Kp = 100,  Velocity_Ki = 0,  Velocity_Kd = 0; //相关速度PID参数
uint16_t TargetVelocity=600;
int32_t Speed;

uint32_t count_j;		/* 计算速度用 */
uint32_t   no_single;
uint8_t Encode;
uint8_t hall_sta_edge;  /* 单个霍尔状态跳变 */
int32_t PID_PWM = 0.0; 


int main(void)
{
	uart3_init(115200);//串口3初始化
	BLDC_HALInit();
	Time_Init();
	NVIC_Config();//中断优先级分配
	delay_init();
	
	pwm_Init();
	
	delay_ms(100);
	BLDC_FirstStart();
	delay_ms(100);
	
	BLDC_Control(cw, 28);
	
	
//  Systick_CountMode();//Systick计时初始化(同时禁用延时函数)
	
	while(1)
	{
		
		
	}
}

//1ms
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		int16_t temp_speed=0;
		
	/******************************* 六步换向 *******************************/
		if(dir == ccw)
		{
			Encode = GetHall();
			if(Encode > 0 & Encode < 7)
			{
				BLDC_Change(Encode);
			}
		}else{
			Encode = GetHall();
			if(Encode > 0 & Encode < 7)
			{
				BLDC_Change(7 - Encode);
			}
		}
		
	/******************************* 速度计算 *******************************/
		count_j++;
		hall_sta_edge = check_edge(hall_single_sta);
		if(hall_sta_edge == 0)
		{
			if(dir == ccw)
				temp_speed = ((1000 / 4) * 60) / count_j;
			else
				temp_speed = -(((1000 / 4) * 60) / count_j);
			FirstOrderRC_LPF(Speed,temp_speed,0.2379f);   /* 一阶滤波 */
			no_single = 0;
			count_j = 0;
			
		}
		if(hall_sta_edge == 1)    /* 当采集到下降沿时数据清0 */
		{
			count_j = 0;
			no_single = 0;
		}
		if(hall_sta_edge == 2)    /* 霍尔值一直不变代表未换向 */
		{
			no_single++;          /* 不换相时间累计 超时则判定速度为0 */
                
                if(no_single > 15000) 
                {
                    no_single = 0;
                    Speed = 0;        /* 超时换向 判定为停止 速度为0 */
                }
		}
//		printf("%d\n", Speed);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
