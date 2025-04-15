#ifndef __BLDC_H
#define __BLDC_H

#define DIR_CW 	0
#define DIR_CCW 1

extern uint8_t hall;
extern uint8_t last_hall;
extern uint8_t direction;
extern uint8_t current_step;


typedef struct UPDATE_STATE
{
	uint16_t vbus;
	uint16_t current;
	uint16_t speed;
	uint8_t over_current;
	uint8_t over_load;
	uint8_t hall_step[6];
} UPDATE_STATE_T;


void LOUT_GPIO_Init(void);
void output_zero(void);
void output_brake(void);
void output_ahbl(uint16_t pwm);
void output_ahcl(uint16_t pwm);
void output_bhcl(uint16_t pwm);
void output_bhal(uint16_t pwm);
void output_chal(uint16_t pwm);
void output_chbl(uint16_t pwm);
void output_step_1(uint16_t pwm);
void output_step_2(uint16_t pwm);
void output_step_3(uint16_t pwm);
void output_step_4(uint16_t pwm);
void output_step_5(uint16_t pwm);
void output_step_6(uint16_t pwm);
void bldc_hall_change_callback(void);
void force_run(void);
void bldc_convert(uint16_t pwm);
void hall_init(void);
uint8_t hall_get_position(void);
uint8_t get_current_step(uint8_t hall);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);




#endif
