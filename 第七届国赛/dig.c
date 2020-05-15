#include "dig.h"
#include "ds1302.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;

extern uchar hour, min, sec, TIME[7], play, flash, index_hour, index_min, index_sec;
extern uint volt, volt_h, volt_l;
extern long uint fre, per;
extern bit index;
void dig_set()
{	
	Read_Time();
	sec = TIME[0] / 16 * 10 + TIME[0] % 16;
	min = TIME[1] / 16 * 10 + TIME[1] % 16;
	hour = TIME[2] / 16 * 10 + TIME[2] % 16;	

	if(play == 0)
	{
		dis_code[0] = DIG_CODE[hour / 10];
		dis_code[1] = DIG_CODE[hour % 10];
		dis_code[2] = 0xbf;
		dis_code[3] = DIG_CODE[min / 10];
		dis_code[4] = DIG_CODE[min % 10];
		dis_code[5] = 0xbf;
		dis_code[6] = DIG_CODE[sec / 10];
		dis_code[7] = DIG_CODE[sec % 10];

		if(sec%2)
		{
			if(flash)
			{
				dis_code[3 * flash - 3] = 0xff;
				dis_code[3 * flash - 2] = 0xff;	
			}
		}
	}
	else if(play == 1)
	{
		dis_code[0] = 0xbf;
		dis_code[1] = DIG_CODE[1];
		dis_code[2] = 0xbf;
		dis_code[3] = 0xff;
		dis_code[4] = DIG_CODE[volt / 1000];
		dis_code[5] = DIG_CODE[volt % 1000 / 100];
		dis_code[6] = DIG_CODE[volt % 100 / 10];
		dis_code[7] = DIG_CODE[volt % 10];
	}
	else if(play == 2)
	{
		dis_code[0] = DIG_CODE[volt_h / 1000];
		dis_code[1] = DIG_CODE[volt_h % 1000 / 100];
		dis_code[2] = DIG_CODE[volt_h % 100 / 10];
		dis_code[3] = DIG_CODE[volt_h % 10];
		dis_code[4] = DIG_CODE[volt_l / 1000];
		dis_code[5] = DIG_CODE[volt_l % 1000 / 100];
		dis_code[6] = DIG_CODE[volt_l % 100 / 10];
		dis_code[7] = DIG_CODE[volt_l % 10];	
		
		if(sec%2)
		{
			switch(flash)
			{
				case 1:
					dis_code[0] = 0xff;
					dis_code[1] = 0xff;
					dis_code[2] = 0xff;
					dis_code[3]	= 0xff;
				break;
				case 2:
					dis_code[4] = 0xff;
					dis_code[5] = 0xff;
					dis_code[6] = 0xff;
					dis_code[7] = 0xff;
				break;	
			}	
		}
	}
	else if(play == 3)
	{
		dis_code[0] = 0xbf;
		dis_code[1] = DIG_CODE[2];
		dis_code[2] = 0xbf;
		dis_code[3] = 0xff;

		if(flash == 0)
		{
			dis_code[4] = DIG_CODE[fre / 1000];
			dis_code[5] = DIG_CODE[fre % 1000 / 100];
			dis_code[6] = DIG_CODE[fre % 100 / 10];
			dis_code[7] = DIG_CODE[fre % 10];	
		}	
		else
		{
			dis_code[4] = DIG_CODE[per / 1000];
			dis_code[5] = DIG_CODE[per % 1000 / 100];
			dis_code[6] = DIG_CODE[per % 100 / 10];
			dis_code[7] = DIG_CODE[per % 10];		
		}
	}
	else if(play == 4)
	{
		dis_code[0] = 0xff;
		dis_code[1] = 0xff;
		dis_code[2] = 0xff;
		dis_code[3] = 0xff;
		dis_code[4] = 0xff;
		dis_code[5] = 0xff;
		dis_code[6] = DIG_CODE[0];
		dis_code[7] = DIG_CODE[index];	
	}
	else if(play == 5)
	{
		dis_code[0] = DIG_CODE[index_hour / 10];
		dis_code[1] = DIG_CODE[index_hour % 10];
		dis_code[2] = 0xbf;
		dis_code[3] = DIG_CODE[index_min / 10];
		dis_code[4] = DIG_CODE[index_min % 10];
		dis_code[5] = 0xbf;
		dis_code[6] = DIG_CODE[index_sec / 10];
		dis_code[7] = DIG_CODE[index_sec % 10];
	}
	 

}
void display()
{
	dig_set();
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