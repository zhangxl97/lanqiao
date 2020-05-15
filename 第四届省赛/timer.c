#include "timer.h"
#include "dig.h"

uint t_1ms = 0;
extern bit relay_flag, start_flag, light_flag;
extern uint volumn, price;
void Timer2Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xCD;		//���ö�ʱ��ֵ
	T2H = 0xD4;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	IE2  |= 0x04;
}
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ֵ
	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 0;		//��ʱ��0��ʼ��ʱ
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
