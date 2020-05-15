#include "timer.h"
#include "dig.h"

sbit PWM = P3^4;

extern uint last_time;
extern uchar mode;
extern bit led_flag, temp_flag;

void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时

	ET0 = 1;
}
void Timer1Init(void)		//20微秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x23;		//设置定时初值
	TH1 = 0xFF;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
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
