#include "dig.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;

extern bit play_flag;
extern uchar temper, level, set_temp[4], set_flag, Key;
void set()
{
	if(play_flag == 0)
	{
		dis_code[0] = 0xbf;	
		dis_code[1] = DIG_CODE[level];	
		dis_code[2] = 0xbf;	
		dis_code[3] = 0xff;	
		dis_code[4] = 0xff;	
		dis_code[5] = 0xff;	
		dis_code[6] = DIG_CODE[temper / 10];	
		dis_code[7] = DIG_CODE[temper % 10];	
	}
	else if(play_flag == 1)
	{
		dis_code[0] = 0xbf;	
		dis_code[3] = 0xff;	
		dis_code[4] = 0xff;
		dis_code[5] = 0xbf;
		
		if(set_flag == 0)
		{	 
			dis_code[1] = 0xff;
			dis_code[2] = 0xff;
			dis_code[6] = 0xff;
			dis_code[7] = 0xff;

		}
		else if(set_flag == 1)
		{
			dis_code[1] = DIG_CODE[set_temp[0]];
		}
		else if(set_flag == 2)
		{
			 dis_code[1] = DIG_CODE[set_temp[0]];
			 dis_code[2] = DIG_CODE[set_temp[1]];
		}
		else if(set_flag == 3)
		{
			 dis_code[1] = DIG_CODE[set_temp[0]];
			 dis_code[2] = DIG_CODE[set_temp[1]];
			 dis_code[6] = DIG_CODE[set_temp[2]];
		}
		else if(set_flag >= 4)
		{
			 dis_code[1] = DIG_CODE[set_temp[0]];
			 dis_code[2] = DIG_CODE[set_temp[1]];
			 dis_code[6] = DIG_CODE[set_temp[2]];
			 dis_code[7] = DIG_CODE[set_temp[3]];
		}
	}
}

void display()
{
	set();

	P2 = P2 & 0x0f | 0xe0;
	P0 = 0xff;
	P2 &= 0x0f;
	P2 = P2 & 0x0f | 0xc0;
	P0 = 0x01 << dis_flag;
	P2 &= 0x0f;
	P2 = P2 & 0x0f | 0xe0;
	P0 = dis_code[dis_flag];
	P2 &= 0x0f;

	if(++dis_flag == 8)
		dis_flag = 0;
}
