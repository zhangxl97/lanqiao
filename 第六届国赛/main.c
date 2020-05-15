#include "mod.h"
#include "iic.h"
#include "timer.h"
#include "dig.h"
#include "ultra.h"
#include "key.h"

uchar led = 0x00, mod = 0x00, time_1 = 2, time_2 = 4, mode = 0, distance = 99, type = 0, play = 0;
uchar start_time = 0, set_flag = 0;
uint ADC_val;
bit ADC_flag = 0, MOD_flag = 0, flash = 0, ultra_flag = 0, start = 0, sett = 0;
void main()
{
	uchar key;

//	Write_eeprom(0x00, 0x00);
//	Write_eeprom(0x01, 0x00);
	time_1 = Read_eeprom(0x00);
	time_2 = Read_eeprom(0x01); 

	led_on(~led);
	mod_on(mod);
	ADC_Init();
	Timer0Init();
	Timer1Init();
	Timer2Init();
	while(1)
	{
		key = Read_Key();
		if(key!=0xff)
		{
			switch(key)
			{
				case 4:
					if((mode == 1) && (start == 0))
					{
						TR0 = 1; 
						start = 1;
						mod = mod & 0xef | 0x10; 
						if(type == 1)
							start_time = time_1;
						else
						 	start_time = time_2;
					}
				break;

				case 3:
					if(start == 1)
					{
						TR0 = !TR0;
						if(TR0 == 0)
						{
							mode = 3;
							mod = mod & 0xef; 
							flash = 1;
						}
						else
						{
							mode = 1;
							mod = mod & 0xef | 0x10; 
							flash = 0;
						}
					}
				break;

				case 2:
					if(mode == 0)
					{
						sett = 1;
						set_flag++;
						if(set_flag >= 3)
						{
							set_flag = 0;
							sett = 0;

							Write_eeprom(0x00, time_1);
							Write_eeprom(0x01, time_2);
						}
					}
				break;

				case 1:
					if(sett)
					{
						if(set_flag == 1)
						{
							time_1++;
							if(time_1 > 10)
								time_1 = 1;
						}
						else if(set_flag == 2)
						{
							time_2++;
							if(time_2 > 10)
								time_2 = 1;
						}
					}
				break;

			}
		}
		if((ADC_flag==1) && (start == 0))
		{
			ADC_val = Read_ADC();
			if(ADC_val < 1000)
			{
				led = led & 0xf8 | 0x01;
				mod = mod & 0xbf;
				flash = 0;
				mode = 0;
			}
			else if((ADC_val >= 4000) && (flash == 0))
			{
				led = led & 0xf8;
				flash = 1;
				mod = mod & 0xbf | 0x40; 
				mode = 2;
			}
			else if((ADC_val>=1000) && (ADC_val<4000))
			{	
				led = led & 0xf8 | 0x02;
				mod = mod & 0xbf;
				flash = 0;
				mode = 1;
			}

			ADC_flag = 0;
		}

		if((mode >= 1) && (ultra_flag == 1) && (start == 0))
		{
			Measure();
			if(distance < 30 )
				type = 1;
			else
				type = 2;
			ultra_flag = 0;
		}



		if(MOD_flag == 1)
		{
			led_on(~led);
			mod_on(mod);

			MOD_flag = 0;
		}
	}	
}