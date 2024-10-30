#include "usart.h"	  

int USAR_RxData;
uint8_t USAR_RxFlag;
uint8_t Receive_Flag;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 

void uart1_init(u32 Baudrate)
{
	//配置串口部分
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//开启GPIOA9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//开启GPIOA10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Baudrate;//设置波特率为Baudrate
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制-不使用流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//设置为发送+接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位：No无校验(√)  Odd奇校验  Even偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位：1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长：8位(因为此程序中无校验位)
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启RXNE标志位到NVIC的输出
	
	USART_Cmd(USART1, ENABLE);//使能串口 
}

void uart3_init(u32 Baudrate)
{
	//配置串口部分
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//开启GPIOB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//开启GPIOB11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = Baudrate;//设置波特率为Baudrate
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制-不使用流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//设置为发送+接收模式
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验位：No无校验(√)  Odd奇校验  Even偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位：1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长：8位(因为此程序中无校验位)
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启RXNE标志位到NVIC的输出
	
	USART_Cmd(USART3, ENABLE);//使能串口 
}
/*----------------------以下为发送(Txd)有关函数-----------------------------*/

void USAR_SendByte(uint8_t Byte)//从Tx引脚发送一个字节数据-(只支持16进制数据如0x41='A')
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//等待数据传输结束
}

void USAR_SendByte_One(uint8_t Byte)//从Tx引脚发送一个字节数据-(只支持16进制数据如0x41='A')
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);//等待数据传输结束
}

void USAR_SendArray(uint8_t *Array, uint16_t Length)//从Tx引脚发送提前定义的数组的数据 USAR_SendArray(Array,5)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		USAR_SendByte(Array[i]);
	}
}

void USAR_SendString(char *String)//从Tx引脚发送字符串
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		USAR_SendByte(String[i]);
	}
}

uint32_t USAR_Pow(uint32_t X, uint32_t Y)//从Tx引脚发送一个数字(辅助)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void USAR_SendNumber(uint32_t Number, uint8_t Length)//从Tx引脚发送一个数字
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		USAR_SendByte(Number / USAR_Pow(10, Length - i - 1) % 10 + '0');
	}
}	
/*----------------------以下为接收(Rxd)有关函数-----------------------------*/

uint8_t USAR_GetRxFlag(void)//得到接收数据的标志位
{
	if(USAR_RxFlag == 1)
	{
		USAR_RxFlag=0;
		return 1;
	}
	return 0;
}

uint8_t USAR_GetRxData(void)//得到接收到的数据的值
{
	return USAR_RxData;
}

void USART3_IRQHandler(void)			 
{
	if (USART_GetITStatus(USART3,USART_IT_RXNE) == SET)//如果串口接收到数据
	{
		USAR_RxData = USART_ReceiveData(USART3);//接收数据赋予全局变量USAR_RxData
		USAR_RxFlag = 1;
		
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}
