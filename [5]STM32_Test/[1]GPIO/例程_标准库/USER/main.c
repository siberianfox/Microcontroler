#include "stm32f10x.h"
 void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);

 }
 int main(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PA.8 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
		GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PA.8 �����
		while(1)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay(3000000);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Delay(3000000);
		}
		SysTick->LOAD=1;
 }

