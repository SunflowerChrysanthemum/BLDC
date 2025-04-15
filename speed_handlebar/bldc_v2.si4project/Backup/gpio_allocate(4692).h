#ifndef __GPIO_ALLOCATE_H
#define __GPIO_ALLOCATE_H


#define LED_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define LED_PORT 					GPIOB
#define LED_PIN	 					GPIO_Pin_12

#define BUZZ_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define BUZZ_PORT 					GPIOA
#define BUZZ_PIN					GPIO_Pin_7



#endif

