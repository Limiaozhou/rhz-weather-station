#ifndef __BMP180_H

#define __BMP180_H

//#include "sys.h"
//#include"iostm8s103F3.h"

//#define	BMP085_SlaveAddress   0xee	  //定义器件在IIC总线中的从地址     
//u8 BMP180_ID=0;
#define OSS 0	// Oversampling Setting (note: code is not set up to use other OSS values)
//#define ACK 1
//#define NACK 0
//#define BMP180_ID 0x55

#define u8 unsigned char
typedef unsigned char  BYTE;
typedef unsigned short WORD;

void Read_CalibrationData(void);
float Convert_UncompensatedToTrue(long UT,long UP);
long Get_BMP180UP(void);
long Get_BMP180UT(void);
u8 BMP180_ReadOneByte(u8 ReadAddr);
//void IIC_PortInit(void);
void IIC_Init(void);
//void BMP180_Init(void);
//float bmp085Convert(float *pressure);
//u8 Check_Bmp180(void);

#endif

