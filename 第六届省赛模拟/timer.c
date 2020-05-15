#include "timer.h"
#include "dig.h"
#include "module.h"
extern bit temp_flag, led_flag ;
extern uchar level;
void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时

	IE2 |= 0x04;
}
void Timer0Init(void)		//20毫秒@11.0592MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0xB8;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;		//定时器0开始计时

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

