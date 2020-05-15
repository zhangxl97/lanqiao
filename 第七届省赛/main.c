#include "module.h"
#include "timer.h"
#include "key.h"
#include "onewire.h"

uchar mode = 1, temper = 0;
uint last_time = 60;

bit led_flag = 0, play_flag = 0, temp_flag = 0;
void main()
{
	uchar key;
	led_on(~0x00);
	module_off();
	Timer2Init();
	Timer1Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		key = Read_Key();
		if(key != 0xff)
		{
			switch(key)
			{
				case 4: 
					mode ++; 
					if(mode >= 4) 
						mode = 1; 
				break;
				case 3:
					last_time += 60;
				break;
				case 2:
					last_time = 0;
				break;
				case 1:
					play_flag = ~play_flag;
			}
		}
		if(led_flag == 1)
		{
			led_flag = 0;
			if(last_time != 0)
			{
				if(mode == 1)
					led_on(~0x01);
				else if(mode == 2)
					led_on(~0x02);
				else if(mode == 3)
					led_on(~0x04);
			}
			else
				led_on(~0x00);
		}

		if(temp_flag == 1)
		{
			temp_flag = 0;
			if(play_flag == 1)
			{
				temper = Read_Tempe();
			}	
		}
	}

}