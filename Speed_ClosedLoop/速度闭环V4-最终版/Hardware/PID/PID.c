#include "sys.h"

extern float Velocity_Kp,  Velocity_Ki,  Velocity_Kd; //相关速度PID参数

void xianfu(uint16_t max, uint16_t min, uint16_t val)
{
	if(val > max) val = max;
	if(val < min) val = min;
}


int32_t Velocity1_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
	int Bias;  //定义相关变量
	static int ControlVelocity, Last_Bias, Pre_Bias; //静态变量，函数调用结束后其值依然存在
	
	Bias = TargetVelocity - CurrentVelocity; //求速度偏差
	 
	ControlVelocity += Velocity_Kp * (Bias - Last_Bias) 
						+ Velocity_Ki * Bias 
						+ Velocity_Kd * (Bias - 2 * Last_Bias + Pre_Bias);  //增量式PI控制器
	Pre_Bias = Last_Bias;
	Last_Bias = Bias;	

	if(ControlVelocity > 3600) ControlVelocity = 3600;
	if(ControlVelocity < -3600) ControlVelocity = -3600;
	

	return ControlVelocity; //返回速度控制值
}


int32_t Velocity2_FeedbackControl(int TargetVelocity, int CurrentVelocity)
{
	volatile float Error, ActualValue;
	static volatile float SumError, LastError; 
    Error = (float)(TargetVelocity - CurrentVelocity);                   /* 速度档位偏差*/
	SumError += Error;
    ActualValue = (Velocity_Kp * Error)                       /* E[k]项*/
                       + (Velocity_Ki * SumError)                    /* E[k-1]项*/
                       + (Velocity_Kd * (Error - LastError)); /* E[k-2]项*/
    LastError = Error;
	
	if(ActualValue > 3600) ActualValue = 3600;
	if(ActualValue < -3600) ActualValue = -3600;

    return ((int32_t)ActualValue);                                   /* 返回实际控制数值*/
}
