#include"iostm8s103F3.h"
#ifndef CCS811_H
#define CCS811_H

#define PM25  0x5A
#define PM10  0x3D
#define uchar unsigned char

void IIC_CCS811Write(uchar address,uchar reg,uchar *txbuf,uchar len);
void  IIC_CCS811Read(uchar address,uchar reg,uchar *rxbuf,uchar len);
int CCS811Init(void);
void IIC_CCS811(void);
#endif 