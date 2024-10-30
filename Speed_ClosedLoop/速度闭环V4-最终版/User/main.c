#include "sys.h"

//���Ϊ4����
int8_t  MotorRun = 0;


uint16_t pwm_duty = 0;
uint8_t dir = 0;
float Velocity_Kp = 2.400f,  Velocity_Ki = 4.000f,  Velocity_Kd = 0.3000f; //����ٶ�PID����

int TargetVelocity = 20;

int step_count;
uint8_t Encode;
int speed;
int32_t PID_PWM = 0;


int main(void)
{
	uart3_init(115200);//����3��ʼ��
	BLDC_HALInit();
	Time_Init(); 
	NVIC_Config();//�ж����ȼ�����
	delay_init();
	
	pwm_Init();
	
	delay_ms(100);
	BLDC_FirstStart();
	delay_ms(100);
	
	BLDC_Control(ccw, 0);
	
	
//  Systick_CountMode();//Systick��ʱ��ʼ��(ͬʱ������ʱ����)
	
	while(1)
	{
		if(Key_Get() == 1)
		{
			MotorRun = RUN;
			BLDC_Start();
		}
			
	}
}

//1ms
void TIM4_IRQHandler(void)
{
	static uint16_t time_count = 0;
	
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
	/******************************* �������� *******************************/
		if(MotorRun == RUN)
		{
			if(dir == ccw)
			{
				Encode = GetHall();
				if(Encode > 0 & Encode < 7)
				{
					BLDC_Change(Encode);
				}
			}else{
				Encode = GetHall();
				if(Encode > 0 & Encode < 7)
				{
					BLDC_Change(7 - Encode);
				}
			}
			time_count++;
	/******************************* �ٶȼ��� *******************************/
			if(time_count >= 50)  	//50ms
			{
				if(dir == ccw)
					speed = step_count;
				else
					speed = -step_count;
				
	/******************************* PID���� *******************************/
				PID_PWM = Velocity1_FeedbackControl(TargetVelocity, speed);
				printf("%d\n", speed);
				if(PID_PWM >= 0)
				{
					dir = ccw;
					if(PID_PWM < 700)
						PID_PWM = 700;
					pwm_duty = PID_PWM;
				}else{
					dir = cw;
					PID_PWM = -PID_PWM;
					if(PID_PWM < 700)
						PID_PWM = 700;
					pwm_duty = PID_PWM;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				}
				time_count = 0;
				step_count = 0;
			}
			
		}
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
