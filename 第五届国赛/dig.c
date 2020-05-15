#include "dig.h"
#include "ds1302.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dig_flag = 0;
extern uchar TIME[7], sec, min, hour, dis_flag, kkey;
extern uchar wet, tempe, light;
extern uint dis_times;
void set()
{
	Read_Time();
	sec = TIME[0] / 16 * 10 + TIME[0] % 16;
	min = TIME[1] / 16 * 10 + TIME[1] % 16;
	hour = TIME[2] / 16 * 10 + TIME[2] % 16;

	if (dis_flag == 0)
	{
		dis_code[0] = DIG_CODE[tempe / 10];
		dis_code[1] = DIG_CODE[tempe % 10];
		dis_code[2] = 0xc6;
		dis_code[3] = 0xff;
		dis_code[4] = 0xff;
		dis_code[5] = DIG_CODE[wet / 10];
		dis_code[6] = DIG_CODE[wet % 10];
		dis_code[7] = 0x89;
	}
	else if(dis_flag == 1)
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
			dis_code[2] = 0xff;
			dis_code[5] = 0xff;	
		}
	}
	else if(dis_flag == 2)
	{
		dis_code[0] = 0xff;
		dis_code[1] = 0xff;
		dis_code[2] = 0xff;
		dis_code[3] = 0xbf;
		dis_code[4] = DIG_CODE[dis_times / 1000];
		dis_code[5] = DIG_CODE[dis_times % 1000 / 100];
		dis_code[6] = DIG_CODE[dis_times % 100 / 10];
		dis_code[7] = DIG_CODE[dis_times % 10];
	}
}

void display()
{
	set();
	P2 = P2 & 0x0f | 0xe0;
	P0 = 0xff;
	P2 &= 0x0f;

	P2 = P2 & 0x0f | 0xc0;
	P0 = 0x01<<dig_flag;
	P2 &= 0x0f;
	P2 = P2 & 0x0f | 0xe0;
	P0 = dis_code[dig_flag];
	P2 &= 0x0f;

	if(++dig_flag == 8)
		dig_flag = 0;


}