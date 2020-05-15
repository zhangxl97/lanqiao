#include "dig.h"
#include "ds1302.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;

extern uchar sec, min, hour, TIME[7], wet, wet_thr;
extern bit play_flag, mode;
void set()
{
	Read_Time();
	sec = TIME[0] / 16 * 10 + TIME[0] % 16;
	min = TIME[1] / 16 * 10 + TIME[1] % 16;
	hour = TIME[2] / 16 * 10 + TIME[2] % 16;
	
	if(play_flag == 0)
	{
		dis_code[0] = DIG_CODE[hour / 10];
		dis_code[1] = DIG_CODE[hour % 10];
		dis_code[2] = 0xbf;
		dis_code[3] = DIG_CODE[min / 10];
		dis_code[4] = DIG_CODE[min % 10];
		dis_code[5] = 0xff;
		dis_code[6] = DIG_CODE[wet / 10];
		dis_code[7] = DIG_CODE[wet % 10];
	}
	else
	{
		dis_code[0] = 0xbf;
		dis_code[1] = 0xbf;
		dis_code[2] = 0xff;
		dis_code[3] = 0xff;
		dis_code[4] = 0xff;
		dis_code[5] = 0xff;
		dis_code[6] = DIG_CODE[wet_thr / 10];
		dis_code[7] = DIG_CODE[wet_thr % 10];
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
	
	if( ++ dis_flag == 8)
		dis_flag = 0;

}

