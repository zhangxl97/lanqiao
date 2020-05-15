#include "timer.h"
#include "dig.h"

uint t_1ms = 0;
extern bit relay_flag, start_flag, light_flag;
extern uint volumn, price;
void Timer2Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xCD;		//设置定时初值
	T2H = 0xD4;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	IE2  |= 0x04;
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
	t_1ms++;
	if(t_1ms >= 100)
	{
		t_1ms = 0;
		volumn++;

		if(volumn >= 9999)
		{
			volumn = 9999;
			t_1ms = 0;
			price = volumn / 2;
			TR0 = 0;
			start_flag = 0;
		}
	} 

}

void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0;
	i++;
	j++;
	if(i >= 253)
	{
		i = 0;
		relay_flag = 1;
	}
	if(j >= 157)
	{
		j = 0;
		light_flag = 1;
	}
	display();
}
