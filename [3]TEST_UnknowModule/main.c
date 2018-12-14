#include <STC15W.H>
#include <intrins.h>
/*P1��0-5��������IO��ģ��SPIЭ��̽���ܷ��ȡ��ģ����Ϣ*/
//CPHA=0 CPOL=0  SPI��һ��������������Ч��SPI��״̬��Ϊ�ߵ�ƽ
char IO=0xff;
char MOSI_Bit=0, MISO_Bit=0, SCLK_Bit=0, CS_Bit=0;//ʣ����������ſ��Բ��� ��CE IRQ

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
		SCON = 0x50;			//8λ���ݣ��ɱ䲨����
		AUXR |= 0x01;			
		AUXR |= 0x04;			//��ʱ��ʱ��ΪFosc��1Tģʽ
		T2L = 0xE0;				//��ʱ����ֵ���ڲ�ʱ��11.0592M��������9600
		T2H = 0xFE;				//��ʱ����ֵ
		AUXR |= 0x10;			//������ʱ��
    //ES=1; 					//�����жϿ���,���ò�ѯ��ʱ���ô��ж�      
    REN=1;      			//����Ϊ������ʽ1�������������
    SM0=0;						//SM0 SM1���ڹ�����ʽѡ��01��8λ�첽�շ����������ɶ�ʱ������
    SM1=1;
}

void SneString(char *Str)
{
		while(*Str!=0)
		{
			SBUF=*Str;
			while(!TI); 					//�ȴ��������
			TI=0; 							//���㷢�ͱ�־λ
			Str++;
		}
}


int main()
{
	char i,j,k,l;
	UartInit();
	P1=0xff;//�������Ŷ����1
	/*��ʼ̽��IO���Ƿ���ȷ,�з�Ӧ������:оƬ��FIFO��Ϊ�ջ��������������ţ�CE��IRQ�����ͣ�*/
	while(1)
	{
		for(i=0;i<=5;i++)
		{
			/*ѡ��һ��IO�����ѡ��*/
			if(i==0)
				MOSI_Bit=0;
			else
				MOSI_Bit++;
			
			for(j=0;j<=4;j++)
				{
					MOSI_Bit=0;

				for(k=0;k<=3;k++)
					{
						for(l=0;l<=2;l++)//�ڴ˽���̽�⣬��ͨ�����ڷ��ͽ��
						{
							
						}
					}
				}
		};
		Delay1000ms();
		SneString("Hello world");
	}
}