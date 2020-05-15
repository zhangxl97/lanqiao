/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台(12MHz)
  日    期: 2011-8-9
*/

#include "iic.h"

void nops()
{
	uchar i = 5;
	do
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();
	}while(i--);
}
//总线启动条件
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	nops();
	SDA = 0;
	nops();
	SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	nops();
	SDA = 1;
}

//应答位控制
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	nops();
	SCL = 1;
	nops();
	SCL = 0;
	SDA = 1; 
	nops();
}

//等待应答
bit IIC_WaitAck(void)
{
	SDA = 1;
	nops();
	SCL = 1;
	nops();
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		nops();
		SCL = 1;
		byt <<= 1;
		nops();
		SCL = 0;
	}
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		nops();
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		nops();
	}
	return da;
}

void ADC_Init()
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();

}

uchar Read_ADC()
{
	uchar temp;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	temp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return temp;
}

void Write_Eeprom(uchar add, uchar val)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();	
	IIC_SendByte(add);
	IIC_WaitAck();	
	IIC_SendByte(val);
	IIC_WaitAck();	
	IIC_Stop();

}

uchar Read_Eeprom(uchar add)
{
	uchar temp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();	
	IIC_SendByte(add);
	IIC_WaitAck();	

	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();

	temp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();

	return temp;
}

