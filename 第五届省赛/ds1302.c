#include "ds1302.h"
/********************************************************************/ 
/*单字节写入一字节数据*/
uchar code Write_Add[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
uchar code Read_Add[]  = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
uchar TIME[7] = {0x00, 0x30, 0x08, 0x01, 0x01, 0x01, 0x18};

void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	_nop_();
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
			_nop_();
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
			_nop_();
		}		 
		SCK_SET;
		_nop_();
		SCK_CLR;
		_nop_();		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
		{
			dat |= 0x80;
		}
		else 
		{
			dat &= 0x7F;
		}
		SCK_SET;
		_nop_();
		SCK_CLR;
		_nop_();
	}
	return dat;
}

/********************************************************************/ 
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	_nop_();
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/
	_nop_();
	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
	_nop_();
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/	
	Write_Ds1302_Byte(dat);	 /*写入数据：dat*/
	RST_CLR;				 /*停止DS1302总线*/
	_nop_();
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	_nop_();
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/
	_nop_();
	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
	_nop_();
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/		
	RST_CLR;	/*停止DS1302总线*/
	_nop_();

	SCK = 1;
	_nop_();
	SD = 1;
	_nop_();
	SD = 0;
	_nop_();
	return temp;
}

void DS1302_Init()
{
	uchar i;
	Ds1302_Single_Byte_Write(0x8e, 0x00);
	for(i=0; i<7; i++)
		Ds1302_Single_Byte_Write(Write_Add[i], TIME[i]);
	Ds1302_Single_Byte_Write(0x8e, 0x80);
}

void Read_Time()
{
	uchar i;
	for(i=0; i<7; i++)
		TIME[i] = Ds1302_Single_Byte_Read(Read_Add[i]);
}
