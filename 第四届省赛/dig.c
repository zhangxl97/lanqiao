#include "dig.h"

uchar code DIG_CODE[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
uchar dis_code[8], dis_flag = 0;

extern uchar rate;
extern uint price, volumn;
extern bit start_flag;
void set()
{
	if(start_flag == 0)
	{
		dis_code[0] = 0xff;
		dis_code[1] = DIG_CODE[0] & 0x7f;
		dis_code[2] = DIG_CODE[5];
		dis_code[3] = DIG_CODE[0];
		dis_code[4] = DIG_CODE[price / 1000];
		dis_code[5] = DIG_CODE[price % 1000 / 100] & 0x7f;
		dis_code[6] = DIG_CODE[price % 100 / 10];
		dis_code[7] = DIG_CODE[price % 10];
	}
	else
	{
		dis_code[0] = 0xff;
		dis_code[1] = DIG_CODE[0] & 0x7f;
		dis_code[2] = DIG_CODE[5];
		dis_code[3] = DIG_CODE[0];
		dis_code[4] = DIG_CODE[volumn / 1000];
		dis_code[5] = DIG_CODE[volumn % 1000 / 100] & 0x7f;
		dis_code[6] = DIG_CODE[volumn % 100 / 10];
		dis_code[7] = DIG_CODE[volumn % 10];
	}
}
void display()
{
	set();

	P2 = P2 & 0x0f | 0xe0;
	P0 = 0xff;
	P2 &= 0x0f;
	P2 = P2 & 0x0f | 0xc0;
	P0 = 0x01<<dis_flag;
	P2 &= 0x0f;
	P2 = P2 & 0x0f | 0xe0;
	P0 = dis_code[dis_flag];
	P2 &= 0x0f;

	if(++dis_flag == 8)
		dis_flag = 0;

}
