/************************************************************
*	
*	date:2018-12-4 
*	author:Jack Lam
*	ver:0.1

***************************************************************/

#include <stdio.h>
#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_Define.h"
#include "Led.h"

sbit P15 =P1^5;
/*
*函数名称：按键中断处理函数
*
*/
void PinInterrupt_ISR (void) interrupt 7
{
	if(PIF&1<<5)
	{
		PIF &= ~(1<<5);
		Timer0_Delay1ms(20);
		if(P15!=0)											/*防抖*/
			return;
		P12=!P12;
		P04=!P04;
	}
}

/*
*函数名称：配置IO脚中断
*
*/
void IntterruptSetting()
{
	P15_Quasi_Mode;
	P1|=1<<5;
	Enable_INT_Port1;
	Enable_BIT5_FallEdge_Trig;
	set_EPI;							// Enable pin interrupt
	set_EA;								// global enable bit
}


void main(void)
{
	Set_All_GPIO_Quasi_Mode;
	IntterruptSetting();
	LED_Init();
	while(1);
}
