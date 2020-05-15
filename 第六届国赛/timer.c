#include "timer.h"
#include "dig.h"

uchar over = 0;
uint k = 0;
extern bit ADC_flag, MOD_flag, flash, ultra_flag;
extern uchar led, mode, start_time, mod;
extern bit start;
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2 |= 0x04;
	EA = 1;
}
void Timer1Init(void)		//1����@11.0592MHz
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0;		//���ö�ʱ��ֵ
	TH1 = 0;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 0;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;
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
void Timer0_ISR() interrupt 1
{
	static uint start_i = 0;
	if(start)
	{
		start_i++;
	}

	if(start_i>=999)
	{
		start_time--;
		if(start_time == 0)
		{
			TR0 = 0;
			start = 0;
			mod = mod & 0xef;
		}
		start_i = 0;
	}
}
void Tiner1_ISR() interrupt 3
{
	over++;
}

void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0, flash_i = 0, ultra_i = 0;
	k++;
	if(k>500)
		k = 0;
	if(ADC_flag==0)
		i++;
	if(MOD_flag==0)
		j++;
	if(flash == 1)
	{
		flash_i ++;
	}
	else if(flash==0)
	{	
		flash_i = 0;
	}
	if(i>= 150)
	{
		ADC_flag = 1;
		i = 0;
	}
	if(j>= 200)
	{
		MOD_flag = 1;
		j = 0;
	}

	if(mode == 1)
	{
		ultra_i++;
		if(ultra_i>=200)
		{
			ultra_i = 0;
			ultra_flag = 1;
		}
	}
	else
	{
		ultra_i = 0;
	}
	if(flash == 1)
	{
		if(flash_i<=499)
		{
			if(mode == 2)
				led = led & 0xf8 | 0x04;
			else if(mode == 3)
				led = led & 0xf7 | 0x08;
		}
		else if(flash_i < 999)
		{
			if(mode == 2)
				led = led & 0xf8;
			else
				led = led & 0xf7;
		}
		else
		{
			if(mode == 2)
				led = led & 0xf8;
			else
				led = led & 0xf7;
			flash_i = 0;
		}
	}
	display();
}
