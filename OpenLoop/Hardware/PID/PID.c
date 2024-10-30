#include "sys.h"

extern float Velocity_Kp,  Velocity_Ki,  Velocity_Kd; //����ٶ�PID����

void xianfu(uint16_t max, uint16_t min, uint16_t val)
{
	if(val > max) val = max;
	if(val < min) val = min;
}


uint16_t Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //������ر���
		static int ControlVelocity, Last_Bias; //��̬�������������ý�������ֵ��Ȼ����
		
		Bias = TargetVelocity - CurrentVelocity; //���ٶ�ƫ��
		 
		ControlVelocity += Velocity_Kp * (Bias - Last_Bias) + Velocity_Ki*Bias;  //����ʽPI������
		Last_Bias = Bias;	
		if(ControlVelocity > 3000) ControlVelocity = 3000;
		if(ControlVelocity < -3000) ControlVelocity = -3000;
		

		return ControlVelocity / 3600; //�����ٶȿ���ֵ
}
