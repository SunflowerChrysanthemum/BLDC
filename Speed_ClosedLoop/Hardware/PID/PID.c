#include "sys.h"

extern float Velocity_Kp,  Velocity_Ki,  Velocity_Kd; //����ٶ�PID����

void xianfu(uint16_t max, uint16_t min, uint16_t val)
{
	if(val > max) val = max;
	if(val < min) val = min;
}


int32_t Velocity1_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
	int Bias;  //������ر���
	static int ControlVelocity, Last_Bias, Pre_Bias; //��̬�������������ý�������ֵ��Ȼ����
	
	Bias = TargetVelocity - CurrentVelocity; //���ٶ�ƫ��
	 
	ControlVelocity += Velocity_Kp * (Bias - Last_Bias) 
						+ Velocity_Ki * Bias 
						+ Velocity_Kd * (Bias - 2 * Last_Bias + Pre_Bias);  //����ʽPI������
	Pre_Bias = Last_Bias;
	Last_Bias = Bias;	

	if(ControlVelocity > 3600) ControlVelocity = 3600;
	if(ControlVelocity < -3600) ControlVelocity = -3600;
	

	return ControlVelocity; //�����ٶȿ���ֵ
}


int32_t Velocity2_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
	volatile float Error, ActualValue;
	static volatile float SumError, LastError; 
    Error = (float)(TargetVelocity - CurrentVelocity);                   /* �ٶȵ�λƫ��*/
	SumError += Error;
    ActualValue = (Velocity_Kp * Error)                       /* E[k]��*/
                       + (Velocity_Ki * SumError)                    /* E[k-1]��*/
                       + (Velocity_Kd * (Error - LastError)); /* E[k-2]��*/
    LastError = Error;
	
	if(ActualValue > 3600) ActualValue = 3600;
	if(ActualValue < -3600) ActualValue = -3600;

    return ((int32_t)ActualValue);                                   /* ����ʵ�ʿ�����ֵ*/
}
