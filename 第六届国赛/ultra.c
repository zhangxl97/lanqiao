#include "ultra.h"

sbit TX = P1^0;
sbit RX = P1^1;

extern uchar over, distance;
void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

void send_wave()
{
	uchar i = 8;
	do
	{
		TX = 0;
		Delay10us();
		TX = 1;
		Delay10us();
	}while(i--);
}
void Measure()
{
	send_wave();
	TR1 = 1;
	while((RX==1) && (over<=2)); 
	TR1 = 0;
	if(over == 3)
	{
		distance = 99;
	}	
	else
	{
		distance = ((over << 16) | (TH1 << 8) | (TL1)) * 0.017;
		if(distance>=99)
			distance = 99;
	}
	TH1 = 0;
	TL1 = 0;
	over = 0;

}