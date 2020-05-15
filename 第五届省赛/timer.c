#include "timer.h"
#include "dig.h"

extern bit led_flag, module_flag, adc_flag;

void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2 |= 0x04;
}

void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0, k = 0;
	i++;
	j++;
	k++;
	if(i >= 200)
	{
		i = 0;
		led_flag = 1;
	}
	if(j >= 153)
	{
		j = 0;
		adc_flag = 1;
	}
	if(k >= 257)
	{
		k = 0;
		module_flag = 1;
	}
	display();
}