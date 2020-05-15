#include "mod.h"
#include "timer.h"
#include "dig.h"
#include "kbd.h"
#include "ds1302.h"
#include "iic.h"

uchar hour, min, sec, play = 0, flash = 0, index_hour, index_min, index_sec;
uint volt, volt_h = 2000, volt_l = 1000;
long uint fre = 0, per = 0;
bit ad_flag = 0, index, volt_larger = 0, volt_smaller = 0;
extern uchar TIME[7];
void main()
{
	uchar temp;
	led_off();	
	ADC_Init();
	mod_off();
	DS1302_Init();
	volt = Read_AD();

//	Write_eeprom(0x00, 0x00);
//	Write_eeprom(0x01, 0x00);
//	Write_eeprom(0x02, 0x00);
//	Write_eeprom(0x03, 0x00);
//	Write_eeprom(0x04, 0x00);
//	Write_eeprom(0x05, 0x00);
//	Write_eeprom(0x06, 0x00);
//	Write_eeprom(0x07, 0x00);

	volt_h = Read_eeprom(0x00);
	volt_h = (volt_h << 8) | Read_eeprom(0x01);
	volt_l = Read_eeprom(0x02);
	volt_l = (volt_l << 8) | Read_eeprom(0x03);	
	
	index = Read_eeprom(0x04);
	index_hour = Read_eeprom(0x05); 
	index_min = Read_eeprom(0x06); 
	index_sec = Read_eeprom(0x07); 

	Timer0Init();
	Timer2Init();

	while(1)
	{
		temp = Read_Kbd();
		if(temp != 0xff)
		{
			switch(temp)
			{
				case 0:
					play = 0;
					flash = 0;
				break;

				case 12:
					if(play == 0)
					{
						flash++;
						if(flash > 3)
							flash = 1;
					}
					else if(play == 1)
					{
						flash = 1;
						play = 2;
					}
					else if(play == 2)
					{
						flash++;
						if(flash > 2)
							flash = 1;
					}
					else if(play == 3)
					{
						flash = !flash;
					}
				break;


				case 1:
					if(flash)
					{
						if(play == 0)
						{
							switch(flash)
							{
								case 1: 
									hour++;
									if(hour>=24)
										hour = 0;  
									TIME[2] = hour / 10 * 16 + hour % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x84, TIME[2]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
								case 2:
									min++;
									if(min>=60)
										min = 0;   
									TIME[1] = min / 10 * 16 + min % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x82, TIME[1]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
								case 3:
									sec++;
									if(sec>=60)
										sec = 0;
									TIME[0] = sec / 10 * 16 + sec % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x80, TIME[0]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
							}
						}
						else if(play == 2)
						{
							if(flash == 1)
							{
								volt_h += 500;
								if(volt_h > 5000)
									volt_h = 5000;
							}
							else
							{
								volt_l += 500;
								if(volt_l >= volt_h)
									volt_l = volt_h;
							}
						}
					}
				break;
				case 5:
					if(flash)
					{
						if(play == 0)
						{
							switch(flash)
							{
								case 1: 
									if(hour == 0)
										hour = 24;
									hour--;
									TIME[2] = hour / 10 * 16 + hour % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x84, TIME[2]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
								case 2:
									if(min == 0)
										min = 60;
									min--;
									TIME[1] = min / 10 * 16 + min % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x82, TIME[1]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
								case 3:
									if(sec == 0)
										sec = 60;
									sec--;
									TIME[0] = sec / 10 * 16 + sec % 10;
									Ds1302_Single_Byte_Write(0x8e, 0x00);
									Ds1302_Single_Byte_Write(0x80, TIME[0]);
									Ds1302_Single_Byte_Write(0x8e, 0x80);
								break;
							}
						}
						else if(play == 2)
						{
							if(flash == 1)
							{
								volt_h -= 500;
								if(volt_h <= volt_l)
									volt_h = volt_l;
							}
							else 
							{
								if(volt_l == 0)
									volt_l = 500;
								volt_l -= 500;
							}
						}
					}
				break;



				case 4:
					play = 1;
					if(flash)
					{
						Write_eeprom(0x00, volt_h>>8);
						Write_eeprom(0x01, volt_h&0x00ff);
						Write_eeprom(0x02, volt_l>>8);
						Write_eeprom(0x03, volt_l&0x00ff);

					}
					flash = 0;
				break;


				case 8:
					play = 3;
					flash = 0;
				break;

				case 9:
					if(play == 4)
						play = 5;
					else
						play = 4;
					
					flash = 0;
				break;
			}
		}


		if(ad_flag == 1)
		{
			volt = Read_AD();
			if((volt >= volt_h) && (volt_larger == 0))
			{
				index = 1;
				index_hour = hour;
				index_min = min;
				index_sec = sec;
				volt_smaller = 0;
				volt_larger = 1;

				Write_eeprom(0x04, index);
				Write_eeprom(0x05, index_hour);
				Write_eeprom(0x06, index_min);
				Write_eeprom(0x07, index_sec);

			}
			else if((volt <= volt_l) && (volt_smaller == 0))
			{
				index = 0;
				index_hour = hour;
				index_min = min;
				index_sec = sec;
				volt_larger = 0;
				volt_smaller = 1;

				Write_eeprom(0x04, index);
				Write_eeprom(0x05, index_hour);
				Write_eeprom(0x06, index_min);
				Write_eeprom(0x07, index_sec);
			}

			ad_flag = 0;
		}
		
	}
	

}