#include "sys.h"

enum
{
    CTRL_MODE_NONE = 0,
    CTRL_MODE_UART,
    CTRL_MODE_RC,
    CTRL_MODE_CAN,
    CTRL_MODE_IO
};

#define CTRL_MODE_SET CTRL_MODE_IO


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
		printf("%d\n", count_1s);
	}
}


static void prvSetupHardware(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	uart2_init(9600);
	led_init();

	printf("init finished\n");
}


int main(void)
{
	prvSetupHardware();
	
	while(1)
	{
		
	}
}


