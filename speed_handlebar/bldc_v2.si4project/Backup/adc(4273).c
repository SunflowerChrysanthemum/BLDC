#include "stm32f10x.h"                  // Device header
#include "adc.h"

uint16_t ADC_Buf[3];

void ADC1_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);  //periph外设
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频，ADCCLK = 72MHz / 6 = 12MHz
	
	//配置GPIO：模拟输入
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置多路开关
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_28Cycles5);
	
	//配置ADC转换器(结构体)
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;								//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;				//触发源：软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;								//连续转换模式：disable单次转换	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;									//扫描模式：扫描
	ADC_InitStructure.ADC_NbrOfChannel = 3;											//通道数目
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC1_DMA_Init();
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_Cmd(ADC1,ENABLE);
	
	//ADC校准
	ADC_ResetCalibration(ADC1);		//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);	//获取复位校准状态
	ADC_StartCalibration(ADC1);		//启动校准
	while(ADC_GetCalibrationStatus(ADC1) ==SET);				//获取校准状态
}


void ADC1_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//外设->内存
	DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);
}



