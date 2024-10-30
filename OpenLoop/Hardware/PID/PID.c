#include "sys.h"

extern float Velocity_Kp,  Velocity_Ki,  Velocity_Kd; //相关速度PID参数

void xianfu(uint16_t max, uint16_t min, uint16_t val)
{
	if(val > max) val = max;
	if(val < min) val = min;
}


uint16_t Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
		int Bias;  //定义相关变量
		static int ControlVelocity, Last_Bias; //静态变量，函数调用结束后其值依然存在
		
		Bias = TargetVelocity - CurrentVelocity; //求速度偏差
		 
		ControlVelocity += Velocity_Kp * (Bias - Last_Bias) + Velocity_Ki*Bias;  //增量式PI控制器
		Last_Bias = Bias;	
		if(ControlVelocity > 3000) ControlVelocity = 3000;
		if(ControlVelocity < -3000) ControlVelocity = -3000;
		

		return ControlVelocity / 3600; //返回速度控制值
}
