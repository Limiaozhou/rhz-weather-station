#ifndef _I2C_H_
#define _I2C_H_

#define HCHO  0X31
#define VOICE 0X41
//#define TVOC  0xB5
#define PM25  0x3C
#define TVOC  0xA1

#define TEMP 0x03
#define HUMI 0x05

extern void IIC_init();
extern void IIC_THRead(float *tem, float* rh);
extern void IIC_Read(char *pbuf, char cnt,unsigned char type);
extern void IIC_Write(char *pbuf, char cnt, unsigned char type);
//extern void IIC_PMRead(char *pbuf, char cnt, unsigned char type);


#endif