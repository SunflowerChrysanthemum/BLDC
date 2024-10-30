#ifndef __PID_H
#define __PID_H

#include "sys.h"


void pid_init(void);
void xianfu(uint16_t max, uint16_t min, uint16_t val);
uint16_t Velocity_FeedbackControl(int TargetVelocity, int CurrentVelocity);


#endif
