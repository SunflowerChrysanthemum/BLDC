#include "stm32f10x.h"                  // Device header
#include "buzz.h"
#include "gpio_allocate.h"

void buzz_configure(void)
{
	BUZZ_PeriphClockCmd;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = BUZZ_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BUZZ_PORT, &GPIO_InitStructure);

	buzz_off();
}

void buzz_on(void)
{
	GPIO_WriteBit(BUZZ_PORT, BUZZ_PIN, Bit_RESET);
}

void buzz_off(void)
{
	GPIO_WriteBit(BUZZ_PORT, BUZZ_PIN, Bit_SET);
}



