#include "sys.h"

void AD_Init(void)
{
	//开启RCC：GPIO和ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);  //periph外设
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6分频，ADCCLK = 72MHz / 6 = 12MHz
	
	//配置GPIO：模拟输入
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置多路开关
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	//配置ADC转换器(结构体)
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//工作模式：独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//数据对齐：右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//触发源：软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//连续转换模式：disable单次转换	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//扫描模式：disable非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;															//通道数目
	ADC_Init(ADC1, &ADC_InitStructure);

	//开关控制，开启ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//ADC校准
	ADC_ResetCalibration(ADC1);		//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);	//获取复位校准状态
	ADC_StartCalibration(ADC1);		//启动校准
	while(ADC_GetCalibrationStatus(ADC1) ==SET);				//获取校准状态
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //连续转换、非扫描
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//软件触发
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);	//ADC_FLAG_EOC:规则组转换标志位
	return ADC_GetConversionValue(ADC1);
}


void MYDMA_Init(uint32_t Pending,uint32_t Target,uint16_t number)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_MemoryBaseAddr = Target;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = Pending;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = number;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

void MYMDA_Convious(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	
	DMA_SetCurrDataCounter(DMA1_Channel1,10);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == SET);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

