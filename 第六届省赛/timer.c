#include "timer.h"
#include "dig.h"
#include "module.h"

extern bit time_flag, temp_flag, end_1s;
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
	TR0 = 0;		//定时器0开始计时
	
	ET0 = 1;
}
void Timer1Init(void)		//50毫秒@11.0592MHz
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0x4C;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
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
