#ifndef __SYS_H
#define __SYS_H	 

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "Time.h"
#include "Key.h"
#include "ADC.h"


#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef void(*pctr) (void);


void NVIC_Config(void);

#endif
