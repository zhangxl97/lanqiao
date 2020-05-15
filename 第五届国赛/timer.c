#include "timer.h"
#include "dig.h"
#include "string.h"
extern uchar receive[7];
extern bit wet_flag, temp_flag, light_flag, send_flag;
extern uint times;
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}
void UartInit(void)		//1200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0x00;		//设定定时初值
	T2H = 0xF7;		//设定定时初值
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x10;		//启动定时器2
	ES = 1;
}
void Timer1Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x40;		//定时器时钟1T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xCD;		//设置定时初值
	TH1 = 0xD4;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 0;		//定时器1开始计时
	ET1 = 1;
}

void Timer1_ISR() interrupt 3
{
	static uint ii = 0;
	ii++;
	if(ii>=999)
	{
		ii = 0;
		times++;	
	}	
}


uchar rec_flag = 0;
void Usart1_ISR() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
		receive[rec_flag] = SBUF;
		if(++rec_flag == 6)
			rec_flag = 0;
	}
}

void Send_String(uchar *str)
{
	uchar *p = str;
	while(*p)
	{
		SBUF = *p;
		while(TI == 0);
		TI = 0;
		p++;
	}
}
void Timer0_ISR() interrupt 1
{					
	static uint i = 0, j = 0;
	if(wet_flag == 0)
		i++;
	if(send_flag == 0)
		j++;
	if(i>=431)
	{
		i = 0;
		wet_flag = 1;
	}
	if(j>=998)
	{
		j = 0;
		send_flag = 1;
	}
	display();
}