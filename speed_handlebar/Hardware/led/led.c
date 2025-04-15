#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "gpio_allocate.h"

void led_init(void)
{
	LED_PeriphClockCmd;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	led_on();
}

void led_on(void)
{
	GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);
}

void led_off(void)
{
	GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);
}



