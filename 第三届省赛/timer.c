#include "timer.h"
#include "dig.h"
sbit PWM = P3^4;
extern bit temp_flag, relay_flag;
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ

	IE2 |= 0x04;
}
void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xAE;		//���ö�ʱ��ֵ
	TH0 = 0xFB;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ

	ET0 = 1;
}

void Timer0_ISR() interrupt 1
{
	static uchar i;
	i++;
	if(i < 3)
	{
		PWM = 1;
	}
	else if(i < 10)
		PWM = 0;
	else
	{
		PWM = 1;
		i = 0;
	}

}

void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0;
	i++;
	j++;
	if(j > 153)
	{
		j = 0;
		relay_flag = 1;
	}
	if(i > 457)
	{
		i = 0;
		temp_flag = 1;
	}
	display();
}