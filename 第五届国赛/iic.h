#ifndef _IIC_H
#define _IIC_H

#include "stc.h"

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
void AD_Init(uchar ad);
uchar Read_AD();
void Write_eeprom(uchar add, uchar val);
uchar Read_eeprom(uchar add);
#endif
