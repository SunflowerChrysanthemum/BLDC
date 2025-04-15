#ifndef __SYS_H
#define __SYS_H	 

#include "stm32f10x.h"
#include "main.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "adc.h"
#include "bldc.h"
#include "buzz.h"
#include "gpio_allocate.h"



#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



void NVIC_Config(void);

#endif
