#ifndef __SYS_H
#define __SYS_H	 

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "Time.h"
#include "BLDC.h"
#include "PID.h"
#include "Key.h"
#include "Encode.h"


#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RUN			1			
#define STOP		0
#define cw			1		//À≥ ±’Î
#define ccw			0		//ƒÊ ±’Î

typedef void(*pctr) (void);

#define PWM_PERIOD	3600


void NVIC_Config(void);

#endif
