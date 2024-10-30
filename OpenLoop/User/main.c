#include "sys.h"

//���Ϊ4����
int8_t  MotorRun = 0;

uint8_t hall_single_sta = 0;

uint16_t pwm_duty = 0;
uint8_t dir = cw;
float Velocity_Kp = 100,  Velocity_Ki = 0,  Velocity_Kd = 0; //����ٶ�PID����
uint16_t TargetVelocity=600;
int32_t Speed;

uint32_t count_j;		/* �����ٶ��� */
uint32_t   no_single;
uint8_t Encode;
uint8_t hall_sta_edge;  /* ��������״̬���� */
int32_t PID_PWM = 0.0; 


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
	
	BLDC_Control(cw, 28);
	
	
//  Systick_CountMode();//Systick��ʱ��ʼ��(ͬʱ������ʱ����)
	
	while(1)
	{
		
		
	}
}

//1ms
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		int16_t temp_speed=0;
		
	/******************************* �������� *******************************/
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
		
	/******************************* �ٶȼ��� *******************************/
		count_j++;
		hall_sta_edge = check_edge(hall_single_sta);
		if(hall_sta_edge == 0)
		{
			if(dir == ccw)
				temp_speed = ((1000 / 4) * 60) / count_j;
			else
				temp_speed = -(((1000 / 4) * 60) / count_j);
			FirstOrderRC_LPF(Speed,temp_speed,0.2379f);   /* һ���˲� */
			no_single = 0;
			count_j = 0;
			
		}
		if(hall_sta_edge == 1)    /* ���ɼ����½���ʱ������0 */
		{
			count_j = 0;
			no_single = 0;
		}
		if(hall_sta_edge == 2)    /* ����ֵһֱ�������δ���� */
		{
			no_single++;          /* ������ʱ���ۼ� ��ʱ���ж��ٶ�Ϊ0 */
                
                if(no_single > 15000) 
                {
                    no_single = 0;
                    Speed = 0;        /* ��ʱ���� �ж�Ϊֹͣ �ٶ�Ϊ0 */
                }
		}
//		printf("%d\n", Speed);
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
