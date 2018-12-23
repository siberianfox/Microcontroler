#include "reg51.h"
#include "intrins.h"


typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L      //System frequency
#define BAUD 9600           //UART baudrate

/*Define UART parity mode*/
#define NONE_PARITY     0   //None parity
#define ODD_PARITY      1   //Odd parity
#define EVEN_PARITY     2   //Even parity
#define MARK_PARITY     3   //Mark parity
#define SPACE_PARITY    4   //Space parity

#define PARITYBIT EVEN_PARITY   //Testing even parity

sbit bit9 = P2^2;           //P2.2 show UART data bit9
bit busy;

void SendData(BYTE dat);
void SendString(char *s);


//11.0529MHz 表示1us的时间内，有11.0592次震荡
//11.0592/12=0.9216个指令周期,就是1us，1个指令周期就是1.08506944us
//定时器定时100us，实际上为1.08506944 * 92 =99.826388us
//误差为1-99.826388/100 =1-0.99826388=0.173612%
//定时1000ms的时候，误差为少了0.17*10000=1700us =1.7ms
//弥补误差(每1ms增加两个指令)后，误差为1-(998.26388+2*1.08506944)/1000=1-0.99956617=0.0434%
//定时1ms实际上为1.00043402ms 定时1000ms为1000.43402ms 多了0.43ms
void Delayms(int nms)		//@11.0592MHz
{
    int n100us;
		char count;
    TR0=1;
		count=0;
    n100us=nms*10;//100us一次定时器中断
    while(n100us--)
    {
			if(count==10)//修正误差
			{
        while(TF0==0);
				TR0=0;	//先关闭计时器，再修正误差
				_nop_();//修正误差指令1
				TR0=1;  //修正误差指令2,共两个指令周期,第三个指令周期重新开始计时
				count=0;
        TF0=0;
			}
			else
			{
        while(TF0==0);
        TF0=0;
			}
			count++;
    }
		TR0=0;
}
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
			while (--k);
		} while (--j);
	} while (--i);
}


void Timer0Init(void)		//1us@11.0592MHz
{
	TR0 = 0;		//定时器1开始计时
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0xA4;		//设置定时初值
	TH0 = 0xA4;		//设置定时重载值
	TF0 = 0;		//清除TF0标志
}

void main()
{
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50;            //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
#endif

    TMOD = 0x20;            //Set Timer1 as   8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/BAUD); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
    Timer0Init();
    while(1)
	{
			Delay1000ms();
			SBUF='A';
	};
}


/*--`--------------------------
UART interrupt service routine
----------------------------*/
void Uart_Isr() interrupt 4 using 1
{
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag
        P0 = SBUF;          //P0 show UART data
        bit9 = RB8;         //P2.2 show parity bit
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        busy = 0;           //Clear transmit busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(BYTE dat)
{
    while (busy);           //Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {	
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //Set parity bit to 1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //Set parity bit to 0
#endif
    }
    busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}



