#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "bldc.h"
#include "gpio_allocate.h"

uint8_t step_table_study[6] = {4,5,1,3,2,6};


uint8_t current_step;
uint8_t last_step = 0;

uint8_t direction = DIR_CW;
uint8_t hall = 0;
uint8_t last_hall = 0;

uint32_t bldc_speed_count=0;


/* --------------------调试开环使用----------------------- */
uint16_t test_delay = 0;
uint16_t test_delay_reload=1000 / 50;

void LOUT_GPIO_Init(void)
{
	LOUT_PeriphClockCmd;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LOUT_1_PIN | LOUT_2_PIN | LOUT_3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LOUT_1_PORT, &GPIO_InitStructure);

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
}

void output_zero(void)
{
	TIM3->CCR4 = 0;			//AH
	TIM3->CCR3 = 0;			//BH
	TIM3->CCR2 = 0;			//CH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
}

void output_brake(void)
{
	TIM3->CCR4 = 0;			//AH
	TIM3->CCR3 = 0;			//BH
	TIM3->CCR2 = 0;			//CH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_SET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_SET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_SET);		//CL
}

void output_ahbl(uint16_t pwm)
{
	TIM3->CCR3 = 0;		    //BH
	TIM3->CCR2 = 0; 		//CH
	TIM3->CCR4 = pwm;	    //AH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_SET);		//BL
}

void output_ahcl(uint16_t pwm)
{
	TIM3->CCR3 = 0;			//BH
	TIM3->CCR2 = 0;			//CH
	TIM3->CCR4 = pwm;		//AH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_SET);		//CL
}
void output_bhcl(uint16_t pwm)
{
	TIM3->CCR4 = 0;		    //AH
	TIM3->CCR2 = 0;			//CH
	TIM3->CCR3 = pwm;		//BH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_SET);		//CL
}
void output_bhal(uint16_t pwm)
{
	TIM3->CCR4 = 0;		    //AH
	TIM3->CCR2 = 0;			//CH
	TIM3->CCR3 = pwm;		//BH
	
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_SET);		//AL
}
void output_chal(uint16_t pwm)
{
	TIM3->CCR4 = 0;		    //AH
	TIM3->CCR3 = 0;		    //BH
	TIM3->CCR2 = pwm;		//CH

	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_RESET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_SET);		//AL
}
void output_chbl(uint16_t pwm)
{
	TIM3->CCR4 = 0;		    //AH
	TIM3->CCR3 = 0;		    //BH
	TIM3->CCR2 = pwm;		//CH

	GPIO_WriteBit(LOUT_1_PORT, LOUT_1_PIN, Bit_RESET);		//AL
	GPIO_WriteBit(LOUT_2_PORT, LOUT_2_PIN, Bit_SET);		//BL
	GPIO_WriteBit(LOUT_3_PORT, LOUT_3_PIN, Bit_RESET);		//CL
}


void output_step_1(uint16_t pwm)
{
    current_step = 1;
    output_ahbl(pwm);
}
void output_step_2(uint16_t pwm)
{
    current_step = 2;
    output_ahcl(pwm);
}
void output_step_3(uint16_t pwm)
{
    current_step = 3;
    output_bhcl(pwm);
}
void output_step_4(uint16_t pwm)
{
    current_step = 4;    
    output_bhal(pwm);
}
void output_step_5(uint16_t pwm)
{
    current_step = 5;
    output_chal(pwm);
}
void output_step_6(uint16_t pwm)
{
    current_step = 6;
    output_chbl(pwm);
}


void bldc_hall_change_callback(void)
{
	bldc_speed_count++;
}


void force_run(void)
{
	if(test_delay == 0)
	{
		test_delay = test_delay_reload;
		
	}
}


void bldc_convert(uint16_t pwm)
{
	uint8_t current_step = 0;
	uint8_t next_step = 0;
	uint8_t pre_next_step = 0;
	
	hall = hall_get_position();
    if(hall<1 || hall>6)
    {
        return ;
    }

    current_step = get_current_step(hall);

    if(direction==DIR_CW)	//正转
	{
		pre_next_step = last_step+1;
		if(pre_next_step > 6)
		{
			pre_next_step = 1;
		}
		
		next_step = current_step + 1;
		if(next_step > 6)
		{
			next_step = 1;
		}
	}
	else					//反转
	{
		pre_next_step = last_step - 1;
		if(pre_next_step==0)
		{
			pre_next_step = 6;
		}
		
		next_step = current_step - 1;
		if(next_step==0)
		{
			next_step = 6;
		}
	}

	switch(next_step)
	{
		case 1:	output_step_1(pwm);	break;//A+B-
		case 2:	output_step_2(pwm);	break;//A+C-
		case 3:	output_step_3(pwm);	break;//B+C-
		case 4:	output_step_4(pwm);	break;//B+A-
		case 5:	output_step_5(pwm);	break;//C+A-
		case 6:	output_step_6(pwm);	break;//C+B-
	}	
	
	last_step = next_step;
}








/* --------------------hall相关----------------------- */
void hall_init(void)
{
	HALL_PeriphClockCmd;
	
	// 禁用 JTAG，仅保留 SWD（PA13、PA14 继续当下载器调试口用）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = HALL1_PIN | HALL2_PIN | HALL3_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HALL_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

uint8_t hall_get_position(void)
{
	uint8_t h;
	h = 0;
	if(GPIO_ReadInputDataBit(HALL_PORT, HALL1_PIN) == SET) 	//H1
	{
		h |= 0x04;
	}
	if(GPIO_ReadInputDataBit(HALL_PORT, HALL2_PIN) == SET)	//H2
	{
		h |= 0x02;
	}
	if(GPIO_ReadInputDataBit(HALL_PORT, HALL3_PIN) == SET)	//H3
	{
		h |= 0x01;
	}
	
	return h;
}

uint8_t get_current_step(uint8_t hall)
{
	uint8_t i=0;
	
	for(i=0;i<6;i++)
	{
		if(hall == step_table_study[i])
		{
			return (i+1);
		}
	}
	
	return 0;
}



/* --------------------外部中断回调函数----------------------- */
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		HALL_EXTI_Unified_handle(3);
		EXTI_ClearITPendingBit(EXTI_Line3);
	} 
}

void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		HALL_EXTI_Unified_handle(4);
		EXTI_ClearITPendingBit(EXTI_Line4);
	} 
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		HALL_EXTI_Unified_handle(5);
		EXTI_ClearITPendingBit(EXTI_Line5);
	} 
}


