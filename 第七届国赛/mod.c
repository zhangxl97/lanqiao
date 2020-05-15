#include "mod.h"

void led_off()
{
	P2 = P2 & 0x1f | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
}

void mod_off()
{
	P2 = P2 & 0x1f | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}