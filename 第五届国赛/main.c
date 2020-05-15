#include "mod.h"
#include "dig.h"
#include "timer.h"
#include "ds1302.h"
#include "iic.h"
#include "onewire.h"
#include "KEY.h"
#include "stdio.h"
#include "string.h"

#define VOLT_MODE 0x03
#define LIGHT_MODE 0x01
uchar dis_flag = 0, sec, min, hour, led;
uchar wet, tempe, light, receive[7];
uint times = 0, dis_times = 0;
uint result[5];
bit wet_flag = 0, mode = 0, close = 0, send_flag = 0, first = 0;

extern uchar rec_flag;
void main()
{
	uchar i,key, send[25], index = 0, index_send = 0;
	led_on(~0x00);
	mod_off();

//	Write_eeprom(0x00, 0x01);
//	Write_eeprom(0x01, 0x02);
//	Write_eeprom(0x02, 0x03);
//	Write_eeprom(0x03, 0x04);
//	Write_eeprom(0x04, 0x05);

	for(i=0;i<5;i++)
		result[i] = Read_eeprom(i);

	DS1302_Init();
	UartInit();
	Timer0Init();
	Timer1Init();
	while(1)
	{
		key = Read_Key();
		if(key != 0xff)
		{  
			switch(key)
			{
				case 4: mode = !mode; break;
				case 3: dis_flag++; if(dis_flag>=3) dis_flag = 0; break;
			}
		}
		if(wet_flag == 1)
		{	
			AD_Init(LIGHT_MODE);    
			wet = Read_AD() * 0.389;
			
		    AD_Init(VOLT_MODE);
			light = Read_AD() * 100.0 / 255.0;

			tempe = Read_Temp();
			
			if(mode == 0)
			{
				led = led & 0xfd | 0x01;
			}
			else
				led = led & 0xfe | 0x02;

			if(light < 60)
			{
				led = led | 0x04;
				close = 1;
			}
			else 
			{
				led &= 0xfb;
			 	close = 0;
			 }
			led_on(~led);
			wet_flag = 0;
		}

		if(send_flag == 1)
		{
			if(mode == 0)
			{
				if(strcmp(receive,"AAASSS")==0)
				{
					sprintf(send, "{%d-%d%%}{%d-%d-%d}{%d}\r\n", (uint)tempe, (uint)wet, (uint)hour, (uint)min, (uint)sec, (uint)close);
					Send_String(send);
				}
				else
				{
					rec_flag = 0;
					for(i = 0;i<6;i++)
						receive[i] = '\0';
				}
			}
			else if(mode == 1)
			{
				if(strcmp(receive,"AAASSS")==0)
				{	
					sprintf(send, "{%d-%d%%}{%d-%d-%d}{%d}\r\n", (uint)tempe, (uint)wet, (uint)hour, (uint)min, (uint)sec, (uint)result[index_send]);
					Send_String(send);
					index_send++;
					if(index_send>=5)
						index_send = 0;
				}
				else
				{
					rec_flag = 0;
					for(i = 0;i<6;i++)
						receive[i] = '\0';
				}
			}
			send_flag = 0;
		}

		if(mode == 1)
		{
			if((close == 1) && (first == 0))
			{
				TR1 = 1;
				first = 1;	
			} 
			else if((close == 0) && (first == 1))
			{
				TR1 = 0;
				first = 0;
				result[index] = times;
				dis_times = times;
				times = 0;

				Write_eeprom(index, dis_times);

				index++;
				if(index >= 5)
					index = 0;
			}
		}

	}
}