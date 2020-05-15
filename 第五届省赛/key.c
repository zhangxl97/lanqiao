#include "key.h"

void Delay4ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	i = 44;
	j = 4;
	do
	{
		while (--j);
	} while (--i);
}


uchar Read_Key()
{
	uchar temp, val = 0xff;
	temp = P3 & 0x0f;
	if(temp != 0x0f)
	{
		Delay4ms();
		temp = P3 & 0x0f;
		if(temp != 0x0f)
		{
			switch(temp)
			{
				case 0x0e: val = 1; break;
				case 0x0d: val = 2; break;
				case 0x0b: val = 3; break;
				case 0x07: val = 4; break;
			}
		}
	}

	temp = P3 & 0x0f;
	while(temp != 0x0f)
	{
		Delay4ms();
		temp = P3 & 0x0f;
	}

	return val;
}