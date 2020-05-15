#include "key.h"

uint key_pre = 0;
uchar key_re = 0, key_val = 0xff;

uchar Read_Key()
{
	uchar temp;
	temp = P3 & 0x0f;

	if(temp != 0x0f)
	{
		key_pre++;
	}
	else
		key_pre = 0;

	if(key_pre >= 1000)
	{
		key_pre = 0;
		key_re = 1;

		temp = P3 & 0x0f;
		switch(temp)
		{
			case 0x0e: key_val = 1; break;
			case 0x0d: key_val = 2; break;
			case 0x0b: key_val = 3; break;
			case 0x07: key_val = 4; break;
		}
	}

	
	if((key_re == 1) && ((P3&0x0f)==0x0f))
	{
		key_re = 0;
		return key_val;
	}
	
	return 0x0f;

}
