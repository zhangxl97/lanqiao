#include "timer.h"
#include "dig.h"
#include "module.h"
extern bit temp_flag, led_flag ;
extern uchar level;
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ

	IE2 |= 0x04;
}
void Timer0Init(void)		//20����@11.0592MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0xB8;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ

	ET0 = 1;
}


void Timer0_ISR() interrupt 1
{
	static uint t_1ms = 0;
	t_1ms++;

	if(level == 0)
	{
		if(t_1ms < 40)
			led_on(~0x01);
		else if(t_1ms < 79)
			led_on(~0x00);
		else
		{
			led_on(~0x01);
			t_1ms = 0;
		}
	}
	else if(level == 1)
	{
		if(t_1ms < 20)
			led_on(~0x01);
		else if(t_1ms < 39)
			led_on(~0x00);
		else
		{
			led_on(~0x01);
			t_1ms = 0;
		}
	}
	else if(level == 2)
	{
		if(t_1ms < 10)
			led_on(~0x01);
		else if(t_1ms < 19)
			led_on(~0x00);
		else
		{
			led_on(~0x01);
			t_1ms = 0;
		}
	}

}


void Timer2_ISR() interrupt 12
{
	static uint i, j;
	i++;
	j++;
	if(i >= 371)
	{
		i = 0;
		temp_flag = 1;
	}
	if(j >= 453)
	{
		j = 0;
		led_flag = 1;
	}
	display();
}

