#include "AS5600.h"
 
// ��ֲʱ:  ֻ���޸�
//          AS5600_W_SCL  AS5600_W_SDA  AS5600_R_SDA
//          delay_3us     AS5600_IIC_Init
//          AS5600_IIC_SDA_IO_OUT  AS5600_IIC_SDA_IO_IN
            
#define delay_3us()         delay_us(10)
 
static void AS5600_W_SCL(uint8_t x)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x));
    delay_3us();
}
 
static void AS5600_W_SDA(uint8_t x)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x));
    delay_3us();
}
 
static uint8_t AS5600_R_SDA(void)
{
    uint8_t tmp;
    tmp = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
    delay_3us();
    return tmp;
}
 
/***********************************************************
*@fuction	: myIIC_Init
*@brief		: ���IIC��GPIO��ʼ��
*@param		: None
*@return	: None
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
void AS5600_IIC_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;// ��©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	AS5600_W_SCL(1);
	AS5600_W_SDA(1);
}
 
void AS5600_IIC_SDA_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;// ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
void AS5600_IIC_SDA_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
/***********************************************************
*@fuction	: AS5600_IIC_Start
*@brief		: ���iic��ʼ�ź�
*@param		: None
*@return	: None
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
void AS5600_IIC_Start(void)
{
    AS5600_IIC_SDA_IO_OUT();
    
	AS5600_W_SDA(1);
	AS5600_W_SCL(1);
	AS5600_W_SDA(0);
	AS5600_W_SCL(0);
}
 
/***********************************************************
*@fuction	: AS5600_IIC_Stop
*@brief		: ���iic�����ź�
*@param		: None
*@return	: None
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
void AS5600_IIC_Stop(void)
{
    AS5600_IIC_SDA_IO_OUT();
    
	AS5600_W_SDA(0);
	AS5600_W_SCL(1);
	AS5600_W_SDA(1);
}
 
/***********************************************************
*@fuction	: AS5600_IIC_SendAck
*@brief		: ���IIC����Ӧ��λ
*@param		: Ӧ��λ 0:Ӧ��;  1:��Ӧ��
*@return	: None
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
void AS5600_IIC_SendAck(uint8_t AckBit)
{
    AS5600_IIC_SDA_IO_OUT();
    
    AS5600_W_SDA(AckBit);
    AS5600_W_SCL(1);
    AS5600_W_SCL(0);
}
 
/***********************************************************
*@fuction	: AS5600_IIC_WaitAck
*@brief		: ���IIC�ȴ�Ӧ��λ
*@param		: None
*@return	: Ӧ��λ, 0:Ӧ��;  1:��Ӧ��
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
uint8_t AS5600_IIC_WaitAck(void)
{
    uint8_t AckBit = 1;
    AS5600_IIC_SDA_IO_IN();
    
	AS5600_W_SDA(1);
	AS5600_W_SCL(1);
	AckBit = AS5600_R_SDA();
	AS5600_W_SCL(0);
    return AckBit;
}
 
/***********************************************************
*@fuction	: AS5600_IIC_SendByte
*@brief		: ���IIC����һ���ֽ�
*@param		: ����
*@return	: None
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
void AS5600_IIC_SendByte(uint8_t Byte)
{
	uint8_t i;
    AS5600_IIC_SDA_IO_OUT();
 
	for (i = 0; i < 8; i++)
	{
		AS5600_W_SDA(Byte & (0x80 >> i));
		AS5600_W_SCL(1);
		AS5600_W_SCL(0);
	}
}
 
/***********************************************************
*@fuction	: AS5600_IIC_ReceiveByte
*@brief		: ���IIC����һ���ֽڲ�����ack
*@param		: Ӧ��λ
*@return	: ����
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
uint8_t AS5600_IIC_ReceiveByte(uint8_t Ack)
{
    uint8_t dat;
    AS5600_IIC_SDA_IO_IN();
    
    for(uint8_t i=0; i<8; i++)
    {
        AS5600_W_SCL(0);
        AS5600_W_SCL(1);
        
        dat <<= 1;
        if(AS5600_R_SDA())
            dat |= 0x01;
    }
    
    AS5600_IIC_SendAck(Ack);
    
    return  dat;
}
 
/***********************************************************
*@fuction	: AS5600_WriteReg
*@brief		: д������AS5600�ļĴ���
*@param		: ��ַ, ����
*@return	: None
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
void AS5600_WriteReg(uint16_t addr, uint8_t dat)
{
	AS5600_IIC_Start();
    // ����д����  ���һλ�Ƕ�дλ��1: ����0: д
	AS5600_IIC_SendByte((AS5600_ID_ADDR<<1) & 0xfe);
	AS5600_IIC_WaitAck();	
	AS5600_IIC_SendByte(addr);		
	AS5600_IIC_WaitAck();
    
	AS5600_IIC_Start();
	AS5600_IIC_SendByte(dat);		
	AS5600_IIC_WaitAck();	
	AS5600_IIC_Stop();
    
	delay_3us();		
}
 
 
/***********************************************************
*@fuction	: AS5600_ReadReg
*@brief		: ��������AS5600�ļĴ���
*@param		: ��ַ
*@return	: ����
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
uint8_t AS5600_ReadReg(uint16_t addr)
{
    uint8_t	dat = 0;
    
	AS5600_IIC_Start();
    // ����д����  ���һλ�Ƕ�дλ��1�Ƕ���0��д
	AS5600_IIC_SendByte((AS5600_ID_ADDR<<1) & 0xfe);
	AS5600_IIC_WaitAck();
	AS5600_IIC_SendByte(addr);
	AS5600_IIC_WaitAck();
    
    AS5600_IIC_Start();
	AS5600_IIC_SendByte((AS5600_ID_ADDR<<1) | 0x01);
	AS5600_IIC_WaitAck();
	dat = AS5600_IIC_ReceiveByte(0);
	AS5600_IIC_Stop();
    
	return dat;
}
 
 
/***********************************************************
*@fuction	: AS5600_Check
*@brief		: ���AS5600�Ƿ����
*@param		: None
*@return	: 0: ����
              1: �ӻ��豸����ʧ��
*@author	: HongScholar
*@date		: 2024.02.01
***********************************************************/
uint8_t AS5600_CheckDevice(void)
{
    uint8_t ack;
    AS5600_IIC_Start();
    AS5600_IIC_SendByte(0x36<<1);
    ack = AS5600_IIC_WaitAck();
    AS5600_IIC_Stop();
    return ack;
}
 
 
/***********************************************************
*@fuction	: AS5600_GetBurnTime
*@brief		: ��ȡ AS5600 �ı�̴���, �������
*@param		: None
*@return	: ����
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
uint8_t AS5600_GetBurnTime(void)
{
    return AS5600_ReadReg (AS5600_ZMCO);
}
 
 
/***********************************************************
*@fuction	: AS5600_GetMagnetStatus
*@brief		: ��ȡ AS5600 �ĴŻ�״̬�Ĵ���������
*@param		: None
*@return	: ����״̬
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
uint8_t AS5600_GetMagnetStatus(void)
{
    return AS5600_ReadReg (AS5600_STATUS);
}
 
 
/***********************************************************
*@fuction	: AS5600_CheckMagnet
*@brief		: ����Ƿ���ڴ����ʹų�ǿ��
*@param		: None
*@return	: ����״̬
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
MagnetStatus AS5600_CheckMagnet(void)
{
    uint8_t status;
    status = AS5600_ReadReg(AS5600_STATUS);
    if(status & 0x20)
        return MD;  // ���� �Ҵų��պ�
    if(status&0x10)
        return ML;  // �ų�̫��
    if(status&0x08)
        return MH;  // ̫ǿ
    
        return 0;
}
 
 
/***********************************************************
*@fuction	: AS5600_GetRAWAngleData
*@brief		: ��ȡ�������Ƕ�ֵ
*@param		: None
*@return	: �Ƕ�ԭʼ����, 0-4096
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
uint16_t AS5600_GetRAWAngleData(void)
{
    uint16_t RAW_angle= -1;
    RAW_angle = AS5600_ReadReg(AS5600_RAWAngleADDR_H);
    RAW_angle <<= 8;
    RAW_angle |= AS5600_ReadReg(AS5600_RAWAngleADDR_L);
    return RAW_angle;
}
 
 
/***********************************************************
*@fuction	: AS5600_GetAngleData
*@brief		: ��ȡ�������Ƕ�ԭʼ����
*@param		: None
*@return	: �Ƕ�, 0-4096
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
uint16_t AS5600_GetAngleData(void)
{
    uint16_t angle;
    angle = AS5600_ReadReg(AS5600_AngleADDR_H);
    angle <<= 8;
    angle |= AS5600_ReadReg(AS5600_AngleADDR_L);
//    angle = angle*360 /4096;
    return angle;
}
 
 
/***********************************************************
*@fuction	: AS5600_GetAngle
*@brief		: ��ȡ�������Ƕ�
*@param		: None
*@return	: �Ƕ�, 0-360, ������ 360/4096 = 
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
float AS5600_GetAngle(void)
{
    return AS5600_GetRAWAngleData()/4096.0000000 *360;
}
 
 
/***********************************************************
*@fuction	: AS5600_Init
*@brief		: ������AS5600�ĳ�ʼ��
*@param		: None
*@return	: None
*@author	: HongScholar
*@date		: 2025.03.02
***********************************************************/
void AS5600_Init(void)
{
    AS5600_IIC_Init();
    
    /* ���ӻ���ַ*/
//    while( AS5600_CheckDevice() )
//    {
//        // ...
//    }
    /* ���Ż�*/
//    while( AS5600_GetMagnetStatus() != MD)
//    {
//        
//    }
}
