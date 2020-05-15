#include "module.h"
#include "ds1302.h"
#include "timer.h"
#include "dig.h"
#include "iic.h"
#include "key.h"
uchar sec, min, hour, wet, relay_flag = 0, buzz_flag = 0, wet_thr = 50;
bit play_flag = 0, mode = 0, led_flag = 0, module_flag = 0, adc_flag = 0, buzz_off = 0;
void Delay10ms();
void main()
{			  
	uchar temp;
	led_on(~0x00);
	module_on(0x00);
	Timer2Init();
	DS1302_Init();
	ADC_Init();

//	Write_Eeprom(0x00, 0x00);
	
	wet_thr = Read_Eeprom(0x00);
	Delay10ms();

	EA = 1;
	while(1)
	{
		temp = Read_Key();
		if(temp != 0xff)
		{
			switch(temp)
			{
				case 1: 
					if(play_flag == 0)
						mode = !mode; 
				break;
				case 2:
					if(mode == 1)
					{
						buzz_off = ~buzz_off;
					}
					else
					{
						play_flag = !play_flag;
						if(play_flag == 0)
						{
							Write_Eeprom(0x00, wet_thr);
							Delay10ms();   
						}
					}
				break;
				case 3:
					if(mode == 1) 
					{
						relay_flag = 1;
					}
					else if((mode == 0) && (play_flag == 1))
					{
						wet_thr++;
						if(wet_thr > 99)
							wet_thr = 99;	
					}
				break;
				case 4:
					if(mode == 1) 
					{
						relay_flag = 0;
					}
					else if((mode == 0) && (play_flag == 1))
					{
						if(wet_thr == 0)
							wet_thr = 1;
						wet_thr--;
							
					}
				break;
			}
		}
		if(led_flag == 1)
		{
			led_flag = 0;
			if(mode == 0)
			{
				led_on(~0x01);
			}
			else
			{
				led_on(~0x02);
			}
		}

		if(adc_flag == 1)
		{
			adc_flag = 0;
			temp = Read_ADC();
			wet = temp * 99 / 255;	
			if(mode == 0)
			{
				if(wet < wet_thr)
					relay_flag = 1;
				else
					relay_flag = 0;
			}							   
		}

		if(module_flag ==1)
		{
			module_flag = 0;
			if(mode == 1)
			{
				if((wet < wet_thr) && (buzz_off == 0))
					buzz_flag = 1;
				else
					buzz_flag = 0;
			}
			else
				buzz_flag = 0;
			temp = buzz_flag << 6 | relay_flag << 4;
			module_on(temp);
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
