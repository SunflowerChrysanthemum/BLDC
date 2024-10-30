#include "sys.h"

void AD_Init(void)
{
	//����RCC��GPIO��ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);  //periph����
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//6��Ƶ��ADCCLK = 72MHz / 6 = 12MHz
	
	//����GPIO��ģ������
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//���ö�·����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	//����ADCת����(�ṹ��)
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//����ģʽ������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//���ݶ��룺�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//����Դ���������
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;									//����ת��ģʽ��disable����ת��	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;												//ɨ��ģʽ��disable��ɨ��
	ADC_InitStructure.ADC_NbrOfChannel = 1;															//ͨ����Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);

	//���ؿ��ƣ�����ADC
	ADC_Cmd(ADC1,ENABLE);
	
	//ADCУ׼
	ADC_ResetCalibration(ADC1);		//��λУ׼
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);	//��ȡ��λУ׼״̬
	ADC_StartCalibration(ADC1);		//����У׼
	while(ADC_GetCalibrationStatus(ADC1) ==SET);				//��ȡУ׼״̬
	
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //����ת������ɨ��
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//�������
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);	//ADC_FLAG_EOC:������ת����־λ
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

