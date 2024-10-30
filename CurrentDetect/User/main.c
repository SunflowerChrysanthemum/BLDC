#include "sys.h"
uint16_t AD_Value;


int main(void)
{
	uart3_init(115200);//串口3初始化
	NVIC_Config();//中断优先级分配
	delay_init();
	AD_Init();
	delay_ms(100);
//  Systick_CountMode();//Systick计时初始化(同时禁用延时函数)
	printf("test\r\n");
	
	while(1)
	{
		AD_Value = AD_GetValue();
		printf("%d\r\n",AD_Value);
	}
}

