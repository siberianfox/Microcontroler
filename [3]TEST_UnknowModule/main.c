#include <STC15W.H>
#include <intrins.h>
/*P1口0-5用作控制IO，模拟SPI协议探测能否读取到模块信息*/
//CPHA=0 CPOL=0  SPI第一个跳边沿数据有效，SPI闲状态线为高电平
char IO=0xff;
char MOSI_Bit=0, MISO_Bit=0, SCLK_Bit=0, CS_Bit=0;//剩余的两个引脚可以不管 ：CE IRQ

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void UartInit(void)		//9600bps@11.0592MHz
{
		SCON = 0x50;			//8位数据，可变波特率
		AUXR |= 0x01;			
		AUXR |= 0x04;			//定时器时钟为Fosc，1T模式
		T2L = 0xE0;				//定时器初值，内部时钟11.0592M，波特率9600
		T2H = 0xFE;				//定时器初值
		AUXR |= 0x10;			//启动定时器
    //ES=1; 					//串口中断开关,采用查询法时不用打开中断      
    REN=1;      			//串口为工作方式1，允许接收数据
    SM0=0;						//SM0 SM1串口工作方式选择，01：8位异步收发，波特率由定时器决定
    SM1=1;
}

void SneString(char *Str)
{
		while(*Str!=0)
		{
			SBUF=*Str;
			while(!TI); 					//等待发送完成
			TI=0; 							//清零发送标志位
			Str++;
		}
}


int main()
{
	char i,j,k,l;
	UartInit();
	P1=0xff;//所有引脚都输出1
	/*开始探测IO口是否正确,有反应的条件:芯片的FIFO不为空或者其他两个引脚（CE、IRQ被拉低）*/
	while(1)
	{
		for(i=0;i<=5;i++)
		{
			/*选定一个IO，逐个选择*/
			if(i==0)
				MOSI_Bit=0;
			else
				MOSI_Bit++;
			
			for(j=0;j<=4;j++)
				{
					MOSI_Bit=0;

				for(k=0;k<=3;k++)
					{
						for(l=0;l<=2;l++)//在此进行探测，并通过串口发送结果
						{
							
						}
					}
				}
		};
		Delay1000ms();
		SneString("Hello world");
	}
}