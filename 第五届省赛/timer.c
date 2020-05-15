#include "timer.h"
#include "dig.h"

extern bit led_flag, module_flag, adc_flag;

void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
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