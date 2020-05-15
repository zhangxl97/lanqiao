#include "timer.h"
#include "dig.h"
#include "string.h"
extern uchar receive[7];
extern bit wet_flag, temp_flag, light_flag, send_flag;
extern uint times;
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}
void UartInit(void)		//1200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0x00;		//�趨��ʱ��ֵ
	T2H = 0xF7;		//�趨��ʱ��ֵ
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x10;		//������ʱ��2
	ES = 1;
}
void Timer1Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xCD;		//���ö�ʱ��ֵ
	TH1 = 0xD4;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}

void Timer1_ISR() interrupt 3
{
	static uint ii = 0;
	ii++;
	if(ii>=999)
	{
		ii = 0;
		times++;	
	}	
}


uchar rec_flag = 0;
void Usart1_ISR() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		receive[rec_flag] = SBUF;
		if(++rec_flag == 6)
			rec_flag = 0;
	}
}

void Send_String(uchar *str)
{
	uchar *p = str;
	while(*p)
	{
		SBUF = *p;
		while(TI == 0);
		TI = 0;
		p++;
	}
}
void Timer0_ISR() interrupt 1
{					
	static uint i = 0, j = 0;
	if(wet_flag == 0)
		i++;
	if(send_flag == 0)
		j++;
	if(i>=431)
	{
		i = 0;
		wet_flag = 1;
	}
	if(j>=998)
	{
		j = 0;
		send_flag = 1;
	}
	display();
}