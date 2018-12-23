#include "reg51.h"
#include "intrins.h"


typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L      //System frequency
#define BAUD 9600           //UART baudrate

sbit bit9 = P2^2;           //P2.2 show UART data bit9

void SendData(BYTE dat);
void SendString(char *s);



void Delay1000ms()		//@11.0592MHz
{
   	unsigned char i, j, k;
	_nop_();
	i = 8;
	j = 1;
	k = 243;
	do
	{
		do
		{
			while (--k);//243
		} while (--j);//1
	} while (--i); //18
}

void Delay1000ms2()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 154;
	k = 122;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay1ms(int nms)		//@11.0592MHz
{
	unsigned char i, j;
	do
	{
	i = 2;
	j = 198;
	do
	{
		while (--j);
	} while (--i);
 }while(--nms);//992.775+(1.085*8)*1000
	//R7 E8
 //R6 03
}

void Timer0Init(void)		//1us@11.0592MHz
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xA4;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器1开始计时
}

//
void SendString(char *s)
{
	while(*s)
	{
		SBUF=*s;
		s++;
		while(TI==0);
		TI=0;
	}
}

int main()
{
    SCON = 0x50;            //8-bit variable UART
    TMOD = 0x20;            //Set Timer1 as   8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1	;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
    Timer0Init();
    while(1)
	{
			Delay1ms(1000);
			//SendString("Hello World\r\n");
		  SBUF='A';
	};
}



