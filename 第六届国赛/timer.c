#include "timer.h"
#include "dig.h"

uchar over = 0;
uint k = 0;
extern bit ADC_flag, MOD_flag, flash, ultra_flag;
extern uchar led, mode, start_time, mod;
extern bit start;
void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;
	EA = 1;
}
void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0;		//设置定时初值
	TH1 = 0;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
	ET1 = 1;
}
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时
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
