#include "module.h"

void led_on(uchar led)
{
	P2 = P2 & 0x0f | 0x80;
	P0 = led;
	P2 &= 0x0f;

}
void module_on(uchar mod)
{
	P2 = P2 & 0x0f | 0xa0;
	P0 = mod;
	P2 &= 0x0f;

}
