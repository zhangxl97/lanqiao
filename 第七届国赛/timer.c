#include "timer.h"
#include "dig.h"

uint over = 0;
extern bit ad_flag;
extern long uint fre, per;
void Timer2Init(void)		//1??@11.0592MHz
{
	AUXR |= 0x04;		//?????1T??
	T2L = 0xCD;		//??????
	T2H = 0xD4;		//??????
	AUXR |= 0x10;		//???2????
	IE2 |= 0x04;
	EA = 1;
}
void Timer0Init(void)		//1??@11.0592MHz
{
	AUXR &= 0xBF;		//?????12T??
	TMOD &= 0xf0;		//???????
	TMOD |= 0x05;
	TL0 = 0;		//??????
	TH0 = 0;		//??????
	TF0 = 0;		//??TF1??
	TR0 = 0;		//???1????
	ET0 = 1;
}
void Timer0_ISR() interrupt 1
{
	over++;
}
void Timer2_ISR() interrupt 12
{
	static uint i = 0, j = 0;
	
	if(ad_flag == 0)
	{
		i++;
	
		if(i >= 150)
		{
			i = 0;
			ad_flag = 1;
		}
	}
	else
	{
		i = 0;
		ad_flag = 0;
	}

	j++;
	if(j >= 990)
	{
		TR0 = 0;
		fre = (over<<16) | (TH0<<8) | TL0;  
		per = 1000000.0 / fre;
		over = 0;
		TH0 = 0;
		TL0 = 0;   
		j = 0;
		TR0 = 1;
	}
	display();
}
