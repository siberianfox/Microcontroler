#include "stm32f10x.h"
 void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);

 }
 int main(void)
 {
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PA,PD端口时钟
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED0-->PA.8 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
		GPIO_SetBits(GPIOC,GPIO_Pin_13);						 //PA.8 输出高
		while(1)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay(3000000);
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
			Delay(3000000);
		}
		SysTick->LOAD=1;
 }

