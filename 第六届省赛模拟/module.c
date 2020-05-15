#include "module.h"


void led_on(uchar led)
{
	P2 = P2 & 0x0f | 0x80;
	P0 = led;
	P2 &= 0x0f;
}

void relay_on(uchar val)
{
	P2 = P2 & 0x0f | 0xa0;
	P0 = val;
	P2 &= 0x0f;	
}
