#ifndef __BLDC_H
#define __BLDC_H

#include "sys.h"


/* Ft/(6*C)*60 */
#define SPEED_COEFF      (uint32_t)((18000/6)*60)       /* 旋转一圈变化6个信号，2对级永磁体特性，NSNS共6级数*/

#define FirstOrderRC_LPF(Yn_1,Xn,a) Yn_1 = (1-a)*Yn_1 + a*Xn;   /* Yn:out;Xn:in;a:系数 */


extern uint16_t pwm_duty;
extern uint8_t dir;

extern int step_count;

void pwm_Init(void);
void BLDC_HALInit(void);
void BLDC_Stop(void);
void BLDC_Start(void);
void BLDC_FirstStart(void);
unsigned char GetHall(void);
void BLDC_Change(uint8_t step_now);
void BLDC_Control(uint8_t dir_t,uint16_t duty);



#endif
