#include "timer.h"
#include "dig.h"
#include "module.h"
#include "key.h"
#include "ds1302.h"
#include "onewire.h"

uchar play_flag = 1;
uchar code SET_INT[] = {1, 5, 30, 60};
uchar set_inter = 1, set_flag = 0, sec, min, hour;
uchar temper;
uchar mem_temp[10], index = 0;

bit time_flag = 0, temp_flag = 0, end_1s = 0;

extern uchar TIME[7];
void main()
{
	uchar temp;
	led_on(~0x00);
	module_off();
	DS1302_Init();
	Timer2Init();
	Timer1Init();
	Timer0Init();
	EA = 1;

	while(1)
	{
		temp = Read_Key();
		if(temp != 0xff)
		{
			switch(temp)
			{
				case 4:
					if(play_flag == 1)
					{
						set_flag ++;
						if(set_flag >= 4)
							set_flag = 0;
						set_inter = SET_INT[set_flag];
					}
						
				break;

				case 3:
					if(play_flag == 1)
					{
						play_flag = 2;	
						TR0 = 1;
					}
				break;

				case 2:
					if(play_flag == 3)
					{
						index++;
						if(index >= 10)
							index = 0;
						TR1 = 0;
						led_on(~0x00);
					}
				break;

				case 1:
					if(play_flag == 3)
					{
						index = 0;
						play_flag = 1;
						set_flag = 0;
						TR1 = 0;
						TR0 = 0;
					}
				break;
			}

		}

		if(time_flag == 1)
		{
			time_flag = 0;
			Read_Time();

			sec = TIME[0] / 16 * 10 + TIME[0] % 16;
			min = TIME[1] / 16 * 10 + TIME[1] % 16;
			hour = TIME[2] / 16 * 10 + TIME[2] % 16;
		}

		if((temp_flag == 1) && (play_flag == 2))
		{
			temp_flag = 0;
			temper = Read_Temp();
		}

		if( (end_1s == 1) && (play_flag == 2))
		{
			end_1s = 0;
			set_inter--;
			if(set_inter==0)
			{
				mem_temp[index] = temper;
				set_inter = SET_INT[set_flag];
				index++;
			}
			if(index >= 10)
			{
				TR0 = 0;
				play_flag = 3;
				index = 0;
				TR1 = 1;
			}
		}
	}


}
