#include "ds1302.h"
/********************************************************************/ 
/*���ֽ�д��һ�ֽ�����*/
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
		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
			_nop_();
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
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
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
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
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 

	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	_nop_();
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	_nop_();
	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
	_nop_();
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/	
	Write_Ds1302_Byte(dat);	 /*д�����ݣ�dat*/
	RST_CLR;				 /*ֹͣDS1302����*/
	_nop_();
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	_nop_();
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	_nop_();
	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	_nop_();
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/		
	RST_CLR;	/*ֹͣDS1302����*/
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
