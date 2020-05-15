#include "module.h"
#include "timer.h"
#include "dig.h"
#include "key.h"
#include "iic.h"
uchar rate = 50; // 0.5 Ԫ ��50��
uint volumn = 0, price = 0; // ������λΪÿ10����, �۸�λΪ��
bit start_flag = 0, relay_flag = 0, light_flag = 0; 
extern uint t_1ms;
void main()
{
	uchar temp;
	uint volt;
	led_on(~0x00);
	relay_off();
	ADC_Init();
	Timer2Init();
	Timer0Init();
	EA = 1;
	while(1)
	{
		temp = Read_Key();
		if(temp != 0xff)
		{
			switch(temp)
			{
				case 1:
					if(start_flag == 0)
					{		 
						volumn = 0;
						t_1ms = 0;
						start_flag = 1;	
						TR0 = 1;
					}
				break;

				case 2:
					if(start_flag == 1)
					{
						start_flag = 0;
						price = volumn / 2;
						TR0 = 0;
					}
				break;
			}
		}

		if(relay_flag == 1)
		{
			relay_flag = 0;
			if(start_flag == 1)
			{
				relay_on();
			}
			else
			{
				relay_off();
			}
		}
		if(light_flag == 1)
		{
			light_flag = 0;
			temp = Read_ADC();
			volt = temp * 5000.0 / 255.0;
			if(volt < 1250)
				led_on(~0x01);
			else
				led_on(~0x00);

		}
	}

}
