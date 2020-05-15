#include "dig.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;
extern uint ADC_val, k;
extern uchar mode, distance, type, start_time, time_1, time_2, set_flag;
extern bit start, sett;
void set()
{
	if((mode == 0) && (!start) && (!sett))
	{
		dis_code[0] = 0xff;
		dis_code[1] = 0xff;
		dis_code[2] = 0xff;
		dis_code[3] = 0xff;		     
		dis_code[4] = 0xff;
		dis_code[5] = 0xff;
		dis_code[6] = 0xff;
		dis_code[7] = 0xff;
	}
	else if((mode == 1) && (!start) && (!sett))
	{
		dis_code[0] = 0xf9;
		dis_code[1] = 0xff;
		dis_code[2] = DIG_CODE[distance / 10];
		dis_code[3] = DIG_CODE[distance % 10];
		dis_code[4] = 0xff;
		dis_code[5] = 0xff;
		dis_code[6] = 0xff;
		dis_code[7] = DIG_CODE[type];
	}
	else if(start)
	{
		dis_code[0] = 0xa4;
		dis_code[1] = 0xff;
		dis_code[2] = 0xff;
		dis_code[3] = 0xff;
		dis_code[4] = 0xff;
		dis_code[5] = 0xff;
		dis_code[6] = DIG_CODE[start_time / 10];
		dis_code[7] = DIG_CODE[start_time % 10];		
	}
	else if(sett)
	{
		dis_code[0] = 0xb0;
		dis_code[1] = 0xff;
		dis_code[2] = 0xff;
		dis_code[3] = DIG_CODE[time_1 / 10];
		dis_code[4] = DIG_CODE[time_1 % 10];
		dis_code[5] = 0xff;
		dis_code[6] = DIG_CODE[time_2 / 10];
		dis_code[7] = DIG_CODE[time_2 % 10];
		
		if(k<250)
		{
			if(set_flag == 1)
			{
				dis_code[3] = 0xff;
				dis_code[4]	= 0xff;
			}
			else 
			if(set_flag == 2)
			{
				dis_code[6] = 0xff;
				dis_code[7]	= 0xff;
			}
		}	
	}

	 
}

void display()
{
	set();
	P2 = P2 & 0x1f | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = P2 & 0x1f | 0xc0;
	P0 = 0x01<<dis_flag;
	P2 &= 0x1f;
	P2 = P2 & 0x1f | 0xe0;
	P0 = dis_code[dis_flag];
	P2 &= 0x1f;

	if(++dis_flag == 8)
		dis_flag = 0;
}