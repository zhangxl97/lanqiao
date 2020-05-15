#include "key.h"

//uchar key_pre = 0, key_re = 0, key_val = 0xff;
void Delay6ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 65;
	j = 136;
	do
	{
		while (--j);
	} while (--i);
}

uchar Read_Key()
{
	uchar temp, key_val = 0xff;
	temp = P3 & 0x0f;
	if(temp!=0x0f)
	{
		Delay6ms();
		temp = P3 & 0x0f;
		if(temp != 0x0f)
		{
			switch(temp)
			{
			  case 0x0e: key_val = 1; break;
			  case 0x0d: key_val = 2; break;
			  case 0x0b: key_val = 3; break;
			  case 0x07: key_val = 4; break;
			}
		}
	}
	temp = P3 & 0x0f;
	while(temp != 0x0f)
	{
		Delay6ms();
		temp = P3 & 0x0f;
	}
	return key_val;

}