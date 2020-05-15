#include "key.h"
//void Delay5ms()		//@11.0592MHz
//{
//	unsigned char i, j;
//
//	i = 54;
//	j = 199;
//	do
//	{
//		while (--j);
//	} while (--i);
//}
//
//uchar Read_Key()
//{
//	uchar temp, val = 0xff;
//	temp = P3 & 0x0f;
//	if(temp != 0x0f)
//	{
//		Delay5ms();
//		temp = P3 & 0x0f;
//		if(temp != 0x0f)
//		{
//			switch(temp)
//			{
//				case 0x0e: val = 1; break;
//				case 0x0d: val = 2; break;
//				case 0x0b: val = 3; break;
//				case 0x07: val = 4; break;
//			}
//		}
//
//	}
//	temp = P3 & 0x0f;
//	while(temp != 0x0f)
//	{
//		Delay5ms();
//		temp = P3 & 0x0f;
//	}
//
//	return val;
//
//}

uint key_pre = 0;
uchar key_re = 0, key_val = 0xff;

uchar Read_Key()
{
	uchar temp;
	temp = P3 & 0x0f;
	if(temp != 0x0f)
		key_pre ++;
	else
		key_pre = 0;

	if(key_pre >= 300)
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
	temp = P3 & 0x0f;
	if((key_re == 1) && (temp == 0x0f))
	{
		key_re = 0;
		return key_val;
	}
	else
		return 0xff;
}
