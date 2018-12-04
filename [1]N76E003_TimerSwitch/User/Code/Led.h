#ifndef __LED_H
#define __LED_H


typedef enum
{
	LED_Vertical=0,
	LED_Horizontal,
	LED_OOL,
	LED_Lock,
}LED_Type;

void Led_On(LED_Type LED);
void Led_Off(LED_Type LED);
void LED_Init();

#endif