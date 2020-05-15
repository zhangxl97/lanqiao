#include "kbd.h"

uint key_pre = 0;
bit key_re = 0;
uchar key_val = 0xff;

uchar Read_Kbd()
{
	uchar col;

	P3 = 0xf0; P42 = 1; P44 = 1;
	
	if((P3!=0xf0)||(P42!=1)||(P44!=1))
		key_pre++;
	else
		key_pre = 0;

	if(key_pre >= 1000)
	{
		key_pre = 0;
		key_re = 1;

		if(P44 == 0) col = 1;
		if(P42 == 0) col = 2;
		if((P3 & 0x20) == 0) col = 3;
		if((P3 & 0x10) == 0) col = 4;

		P3 = 0x0f; P42 = 0; P44 = 0;
		if((P3 & 0x01) == 0) key_val = col - 1;
		if((P3 & 0x02) == 0) key_val = col + 3;
		if((P3 & 0x04) == 0) key_val = col + 7;
		if((P3 & 0x08) == 0) key_val = col + 11;
	}
	P3 = 0xf0; P42 = 1; P44 = 1;
	if((P3 == 0xf0) && (P42 == 1) && (P44 == 1) && (key_re == 1))
	{
		key_re = 0;
		return key_val;
	}

	return 0xff;
}