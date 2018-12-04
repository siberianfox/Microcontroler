/*Commond Control Of Led */

#include "Led.h"
#include "N76E003.h"
#include "SFR_Macro.h"
#include "Function_Define.h"

#define LED_Vertical_Pin   P03
#define LED_Horizontal_Pin P04
#define LED_OOL_Pin        P01
#define LED_LOCK_Pin       P00

//LED初始化，准双向模式
void LED_Init()
{
	P00_Quasi_Mode;
	P01_Quasi_Mode;
	P03_Quasi_Mode;
	P04_Quasi_Mode;
}

//点亮LED
void Led_On(LED_Type LED)
{
	switch(LED)
	{
		case LED_Vertical : LED_Horizontal_Pin=0; break;
		case LED_Horizontal:LED_Horizontal_Pin=0;break;
		case LED_Lock:			LED_LOCK_Pin=0;break;
		case LED_OOL:				LED_OOL_Pin=0;break;
	}
}

//关闭LED
void Led_Off(LED_Type LED)
{
	switch(LED)
	{
		case LED_Vertical : LED_Horizontal_Pin=1; break;
		case LED_Horizontal:LED_Horizontal_Pin=1;break;
		case LED_Lock:			LED_LOCK_Pin=1;break;
		case LED_OOL:				LED_OOL_Pin=1;break;
	}
}