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


//11.0529MHz ��ʾ1us��ʱ���ڣ���11.0592����
//11.0592/12=0.9216��ָ������,����1us��1��ָ�����ھ���1.08506944us
//��ʱ����ʱ100us��ʵ����Ϊ1.08506944 * 92 =99.826388us
//���Ϊ1-99.826388/100 =1-0.99826388=0.173612%
//��ʱ1000ms��ʱ�����Ϊ����0.17*10000=1700us =1.7ms
//�ֲ����(ÿ1ms��������ָ��)�����Ϊ1-(998.26388+2*1.08506944)/1000=1-0.99956617=0.0434%
//��ʱ1msʵ����Ϊ1.00043402ms ��ʱ1000msΪ1000.43402ms ����0.43ms
void Delayms(int nms)		//@11.0592MHz
{
    int n100us;
		char count;
    TR0=1;
		count=0;
    n100us=nms*10;//100usһ�ζ�ʱ���ж�
    while(n100us--)
    {
			if(count==10)//�������
			{
        while(TF0==0);
				TR0=0;	//�ȹرռ�ʱ�������������
				_nop_();//�������ָ��1
				TR0=1;  //�������ָ��2,������ָ������,������ָ���������¿�ʼ��ʱ
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
	TR0 = 0;		//��ʱ��1��ʼ��ʱ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x02;		//���ö�ʱ��ģʽ
	TL0 = 0xA4;		//���ö�ʱ��ֵ
	TH0 = 0xA4;		//���ö�ʱ����ֵ
	TF0 = 0;		//���TF0��־
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



