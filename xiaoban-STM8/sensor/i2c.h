#include"iostm8s103F3.h"

#ifndef _I2C_H_
#define _I2C_H_

#define VOICE  0X41
#define TEMP 0xE3
#define HUMI 0xE5

#define PM25  0x3C



#define SCL PB_ODR_ODR4
#define SDA PB_ODR_ODR5
#define SDAIN PB_IDR_IDR5

#define DTIME 100  //已经在源码的基础上乘以2
#define TVOC_1 0xA3  //0xB5
#define PRESS 0XEE
#define LIGHT 0X46

extern void IIC_init(void);
extern void IIC_SDA_READ(void);
extern void IIC_SDA_WRITE(void);
extern void IIC_Tvoc(void);
extern void IIC_Start(unsigned char type);
extern void IIC_Stop(unsigned char type);
extern void IIC_SendACK(char ack,unsigned char type);
extern char IIC_ChkACK(unsigned char type);
extern void IIC_SendByte(char dat,unsigned char type);
extern char IIC_RecvByte(unsigned char type);
extern void IIC_Read(char *pbuf, char cnt, unsigned char type);

#endif
