#include "timer.h"
#include "dig.h"
#include "module.h"

extern bit time_flag, temp_flag, end_1s;
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ

	IE2 |= 0x04;
}
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
	
	ET0 = 1;
}
void Timer1Init(void)		//50����@11.0592MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0x4C;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}
void Timer1_ISR() interrupt 3
{
	static uint i = 0;
	i++;
	if(i<2)
		led_on(~0x01);
	else if (i<3)
	{
		led_on(~0x00);
	}
	else
	{
		i = 0;
		led_on(~0x01);
	}
}

void Timer0_ISR() interrupt 1
{
	static uint t_1ms = 0;
	t_1ms++;
	if(t_1ms>=999)
	{
		t_1ms = 0;
		end_1s = 1;
	}
}
void Tiemr2_ISR() interrupt 12
{
	static uint i = 0, j = 0;
	i++;
	j++;
	if(j>=200)
	{
		j = 0;
		temp_flag = 1;
	}
	if(i>=400)
	{
		i = 0;
		time_flag = 1;
	}
	display();

}
