#include "sys.h"

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef Init_Structure;
	Init_Structure.GPIO_Mode = GPIO_Mode_IPU;
	Init_Structure.GPIO_Pin = GPIO_Pin_12;
	Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Init_Structure);
}

uint8_t Key_Get(void)
{
	uint8_t Key_Num= 0;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12) == 0);
		delay_ms(10);
		Key_Num = 1;
	}
	
	return Key_Num;
}

