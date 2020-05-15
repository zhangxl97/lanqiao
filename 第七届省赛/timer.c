#include "timer.h"
#include "dig.h"

sbit PWM = P3^4;

extern uint last_time;
extern uchar mode;
extern bit led_flag, temp_flag;

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
	TR0 = 1;		//��ʱ��0��ʼ��ʱ

	ET0 = 1;
}
void Timer1Init(void)		//20΢��@11.0592MHz
{
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x23;		//���ö�ʱ��ֵ
	TH1 = 0xFF;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
}



void Timer1_ISR() interrupt 3
{
	static uint k = 0;
	if(last_time > 0)
	{
		k++;
		if(mode == 1)
		{
			if(k < 9)
			{
				PWM = 1;
			}
			else if(k < 47)
			{
				PWM = 0;
			} 
			else
			{
				k = 0;
				PWM = 1;
			}
		}
		else if(mode == 2)
		{
			if(k < 14)
			{
				PWM = 1;
			}
			else if(k < 47)
			{
				PWM = 0;
			} 
			else
			{
				k = 0;
				PWM = 1;
			}
		}
		else
		{
			if(k < 33)
			{
				PWM = 1;
			}
			else if(k < 47)
			{
				PWM = 0;
			} 
			else
			{
				k = 0;
				PWM = 1;
			}
		}

	}
	else
	{
		k = 0;
		PWM = 0;
	}
}

void Timer0_ISR() interrupt 1
{
	static uint t_1ms = 0;
	t_1ms++;
	if(t_1ms >= 999)
	{
		t_1ms = 0;
		if(last_time != 0)
		{
			last_time--;
		}
	}
}
void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0;
	i++;
	j++;
	if(i >= 200)
	{
		i = 0;
		led_flag = 1;
	}
	if(j >= 537)
	{
		j = 0;
		temp_flag = 1;
	}
	display();
}
