#include "kbd.h"

uint key_pre = 0;
uchar key_re = 0, key_val = 0xff;

uchar Read_KBD()
{
	uchar col;
	P3 = 0xf0; P44 = 1; P42 = 1;
	if((P3 != 0xf0) || (P44 != 1) || (P42 != 1))
	{
		key_pre ++;
	}
	else
		key_pre = 0;

	if(key_pre >= 500)
	{
		key_pre = 0;
		key_re = 1;

		if(P44 == 0) col = 1;
		if(P42 == 0) col = 2;
		if( (P3 & 0x20) == 0) col = 3;
		if( (P3 & 0x10) == 0) col = 4;
		
		P3 = 0x0f; P44 = 0; P42 = 0;
		
		if(( P3 & 0x01) == 0) key_val = col - 1;
		if(( P3 & 0x02) == 0) key_val = col + 3;
		if(( P3 & 0x04) == 0) key_val = col + 7;
		if(( P3 & 0x08) == 0) key_val = col + 11;
	}

	P3 = 0x0f; P44 = 0; P42 = 0;
	if((key_re == 1) && (P3 == 0x0f) )
	{
		key_re = 0;
		return key_val;
	}
	else
		return 0xff;


}