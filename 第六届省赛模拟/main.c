#include "module.h"
#include "onewire.h"
#include "timer.h"
#include "dig.h"
#include "kbd.h"
uchar t_max = 30, t_min = 20;
uchar temper, level = 1, set_temp[4] = {0, 0, 0, 0}, set_flag = 0;
bit temp_flag = 0, play_flag = 0, led_flag = 0, setting = 0;
void main()
{
	uchar temp;
	led_on(~0x00);
	relay_on(0x00);
	Timer2Init();
	Timer0Init();
	EA = 1;
	while(1)
	{

		temp = Read_KBD();
		if(temp!= 0xff)
		{
			switch(temp)
			{
				case 13:
					if(play_flag == 0)
					{
				   		play_flag = 1;
						setting = 1;
					}
					else if(play_flag == 1)
					{
						if((set_temp[0] == 0) && (set_temp[1] == 0) && (set_temp[2] == 0) && (set_temp[3] == 0)); 
						else
						{
							t_max = set_temp[0] * 10 + set_temp[1];
							t_min = set_temp[2] * 10 + set_temp[3];
						}
						set_temp[0] = 0;
						set_temp[1] = 0;
						set_temp[2] = 0;
						set_temp[3] = 0;
						set_flag = 0;
						play_flag = 0;
						setting = 0;
					}
				break;
				case 14:
					if(play_flag == 1)
					{
						set_temp[0] = 0;
						set_temp[1] = 0;
						set_temp[2] = 0;
						set_temp[3] = 0;
						set_flag = 0;
					}
				break;
	
				case 0:case 1:case 2: 	  
					set_flag++;
					if( (set_flag<5) && (play_flag == 1))
					{ 
						set_temp[set_flag - 1] = temp;
					} 
				break;
				case 4:case 5:case 6: 
					set_flag++;
					if( (set_flag<5) && (play_flag == 1))
					{ 
						set_temp[set_flag - 1] = temp - 1;
					} 
				break;
				case 8:case 9:case 10: 
					set_flag++;
					if( (set_flag<5) && (play_flag == 1))
					{ 
						set_temp[set_flag - 1] = temp - 2;
					} 
				break; 
				case 12: 
					set_flag++;
					if( (set_flag<5) && (play_flag == 1))
					{ 
						set_temp[set_flag - 1] = 9;
					} 
				break;
	
				default:break;
			}
		}
		if(temp_flag == 1)
		{
			temp_flag = 0;
			temper = Read_Temp();
			if(temper < t_min)
				level = 0;
			else if(temper > t_max)
				level = 2;
			else 
				level = 1;
		}

		if(led_flag == 1)
		{
			led_flag = 0;
			if(t_max < t_min)
			{
				led_on(~0x02);
				TR0 = 0;
			}
			else if(setting == 0)
			{
				TR0 = 1;
				if(level == 2)
					relay_on(0x10);
				else
					relay_on(0x00);
			}
			else
			{
				TR0 = 0;
				led_on(~0x00);
			}
			
		}

	}
}
