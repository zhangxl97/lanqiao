#include "timer.h"
#include "dig.h"
sbit PWM = P3^4;
extern bit temp_flag, relay_flag;
void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时

	IE2 |= 0x04;
}
void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初值
	TH0 = 0xFB;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时

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