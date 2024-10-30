#include "LOWPASS_FILTER.h"

float y_prev;

/***********************************************
低通滤波器:
***********************************************/
float LowPass_Filter(float x)
{
	float y = 0.9*y_prev + 0.1*x;
	
	y_prev=y;
	
	return y;
}

/*******************************************
SysTick初始化(模拟micros函数初始化)：

备注：     
	1.从0xFFFFFF到0循环计数
	2.开启后无法再使用delay延时函数
*******************************************/
void Systick_CountMode(void)
{
	SysTick->LOAD = 0xFFFFFF-1;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
}
