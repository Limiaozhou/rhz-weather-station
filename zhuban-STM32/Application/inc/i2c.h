#ifndef _I2C_H_
#define _I2C_H_
#include "sys.h"
#define HCHO  0X31
#define VOICE 0X41
#define TVOC  0xB5
#define PM25  0x3C
#define PM10  0x3D


#define TEMP 0x03
#define HUMI 0x05

extern void IIC_init(void);
//extern void IIC_THRead(float *tem, float* rh);
extern void IIC_Read(char *pbuf, char cnt, unsigned char type,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl);
extern char IIC_PMRead(char reg, unsigned char type);
extern char IIC_XPPM25(char reg, unsigned char type,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl);


#endif
