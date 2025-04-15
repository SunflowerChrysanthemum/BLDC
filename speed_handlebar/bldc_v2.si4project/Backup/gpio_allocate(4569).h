#ifndef __GPIO_ALLOCATE_H
#define __GPIO_ALLOCATE_H


#define LED_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define LED_PORT 					GPIOB
#define LED_PIN	 					GPIO_Pin_12

#define BUZZ_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define BUZZ_PORT 					GPIOA
#define BUZZ_PIN					GPIO_Pin_7

#define ADC_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
#define ADC_PORT 					GPIOA
#define ADC_ROTATE_PIN				GPIO_Pin_3
#define I_ADC_PIN					GPIO_Pin_4
#define V_ADC_PIN					GPIO_Pin_5


#define HALL_PeriphClockCmd			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE)
#define HALL_PORT 					GPIOB
#define HALL1_PIN					GPIO_Pin_3
#define HALL2_PIN					GPIO_Pin_4
#define HALL3_PIN					GPIO_Pin_5


#define HOUT_1_PORT					GPIOB
#define HOUT_1_PIN  				GPIO_Pin_1
#define HOUT_2_PORT					GPIOB
#define HOUT_2_PIN  				GPIO_Pin_0
#define HOUT_3_PORT					GPIOA
#define HOUT_3_PIN  				GPIO_Pin_7

#define LOUT_1_PORT					GPIOA
#define LOUT_1_PIN  				GPIO_Pin_8
#define LOUT_2_PORT					GPIOA
#define LOUT_2_PIN  				GPIO_Pin_9
#define LOUT_3_PORT					GPIOA
#define LOUT_3_PIN  				GPIO_Pin_10


#endif

