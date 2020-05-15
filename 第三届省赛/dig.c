#include "dig.h"


uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;

extern uchar t_max, t_min;
extern uchar tempe;

void set()
{
	dis_code[0] = DIG_CODE[t_max / 10];
	dis_code[1] = DIG_CODE[t_max % 10];
	dis_code[2] = DIG_CODE[t_min / 10];
	dis_code[3] = DIG_CODE[t_min % 10];
	dis_code[4] = 0xff;
	dis_code[5] = 0xff;
	dis_code[6] = DIG_CODE[tempe % 100 / 10];
	dis_code[7] = DIG_CODE[tempe % 10];
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
