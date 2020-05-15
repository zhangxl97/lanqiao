#include "module.h"
#include "timer.h"
#include "dig.h"
#include "onewire.h"
#include "key.h"
#include "iic.h"
uchar t_max = 30, t_min = 23;
uchar tempe = 0;
bit temp_flag = 0, relay_flag = 0, relay = 0;

void Delay10ms();
void main()
{
	uchar temp;
	led_off();
	relay_off();
	Timer2Init();
	Timer0Init();
//
//	Write_eeprom(0x00, 30);
//	Delay10ms();
//	Write_eeprom(0x01, 23);
//	Delay10ms();

	t_max = Read_eeprom(0x00);
	Delay10ms();
	t_min = Read_eeprom(0x01);
	Delay10ms();
	tempe = Read_Temp();
	EA = 1;
	while(1)
	{
		temp = Read_Key();
		if(temp != 0xff)
		{
			switch(temp)
			{
				case 4: t_max++; if(t_max >= 99) t_max = 99; Write_eeprom(0x00, t_max); Delay10ms();break;
				case 2: t_max--; if(t_max < t_min) t_max = t_min; Write_eeprom(0x00, t_max); Delay10ms();break;
				case 3: t_min++; if(t_min > t_max) t_min = t_max; Write_eeprom(0x01, t_min); Delay10ms();break;
				case 1:	if(t_min == 0) t_min = 1; t_min-- ; Write_eeprom(0x01, t_min); Delay10ms(); break;
			}
		}
		if(temp_flag == 1)
		{
			temp_flag = 0;
			tempe = Read_Temp();
			if(tempe < t_min)
				relay = 1;
			else
				relay = 0;

			if(tempe > t_max)
				TR0 = 1;
			else 
			{
				TR0 = 0;
				P34 = 1;
			}
		}
		if(relay_flag == 1)
		{
			relay_flag = 0;
			if(relay == 1)
				relay_on();
			else
				relay_off();
		
		}
	}
}
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}
