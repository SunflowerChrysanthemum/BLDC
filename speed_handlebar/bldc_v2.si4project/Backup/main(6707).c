#include "sys.h"

enum
{
    CTRL_MODE_NONE = 0,
    CTRL_MODE_UART,
    CTRL_MODE_RC,
    CTRL_MODE_CAN,
    CTRL_MODE_IO
};

#define CTRL_MODE_SET 	CTRL_MODE_IO

#define debug_force_run_enable	0

uint16_t pwm_receive = 0;
uint16_t pwm_output = 0;

/* --------------------电机状态----------------------- */
uint32_t convert_count = 0;
UPDATE_STATE_T update_state_data;

uint16_t current;


/* --------------------时间相关----------------------- */
static uint32_t count_1s = 0;


/* --------------------异常相关----------------------- */
#define OVER_CURRENT_TIME 		100		//100ms
#define OVER_CURRENT_STOP_TIME	5000	//5second
#define OVER_CURRENT_VALUE		15000	//15A 

#define OVER_LOAD_TIME			5000	//5second
#define OVER_LOAD_EXIT_TIME		1000 	//1second
#define OVER_LOAD_VALUE 		8000	//8A

uint8_t over_load_flag = 0;
uint16_t over_load_count = 0;
uint8_t over_current_flag = 0;
uint32_t over_current_count = 0;


void systick_callback(void)
{
	if(count_1s++ >= 1000)
	{
		count_1s = 0;
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
//		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
//		printf("%d, %d, %d\n", ADC_Buf[0], ADC_Buf[1], ADC_Buf[2]);
		
		update_state_data.speed = convert_count;
        convert_count = 0;
	}

	if(current > OVER_CURRENT_VALUE) {
		if(over_current_flag == 0) {
			over_current_count++;
			if(over_current_count>OVER_CURRENT_TIME) {
				over_current_flag = 1;
				over_current_count = OVER_CURRENT_STOP_TIME;
			}
		}
	}
	else {
		if(over_current_flag == 0) {
			over_current_count = 0;
		}
		else {
			/* wait stop time and exit over current */
			if(over_current_count > 0) {
				over_current_count --;
			}
			else {
				over_current_flag = 0;
			}
		}
	}
	
	if(current > OVER_LOAD_VALUE) {
		if(over_load_flag==0) {
			over_load_count++;
			if(over_load_count > OVER_LOAD_TIME) {
				over_load_flag = 1;
				over_load_count = OVER_LOAD_EXIT_TIME;
			}
		}
	}
	else {
		if(over_load_flag==0) {
			over_load_count = 0;
		}
		else {
			if(over_load_count > 0) {
				over_load_count--;
			}
			else {
				over_load_flag = 0;
			}
		}
	}
}


void HALL_EXTI_Unified_handle(uint8_t flag)
{
	int i = 0;
	uint8_t state;
	hall = hall_get_position();
	bldc_hall_change_callback();
	printf("%d\n", hall);
	for(i = 0; i < 5; i++)
	{
		update_state_data.hall_step[i] = update_state_data.hall_step[i+1];
	}

	update_state_data.hall_step[5] = hall;
	if(flag == 3)
	{
		state = GPIO_ReadInputDataBit(HALL_PORT, HALL1_PIN);
		if(state)
		{
			led_on();
		}else{
			led_off();
		}
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		if(!debug_force_run_enable)
		{
			bldc_convert(pwm_output);
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


static void prvSetupHardware(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_Config();
	
	uart2_init(9600);
	led_init();
	ADC1_init();
	hall_init();

	memset(&update_state_data, 0, sizeof(UPDATE_STATE_T));
	printf("init finished\n");
}


int main(void)
{
	
	prvSetupHardware();
	
	while(1)
	{
		
	}
}


