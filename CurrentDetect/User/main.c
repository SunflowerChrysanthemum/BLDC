#include "sys.h"
uint16_t AD_Value;


int main(void)
{
	uart3_init(115200);//����3��ʼ��
	NVIC_Config();//�ж����ȼ�����
	delay_init();
	AD_Init();
	delay_ms(100);
//  Systick_CountMode();//Systick��ʱ��ʼ��(ͬʱ������ʱ����)
	printf("test\r\n");
	
	while(1)
	{
		AD_Value = AD_GetValue();
		printf("%d\r\n",AD_Value);
	}
}

