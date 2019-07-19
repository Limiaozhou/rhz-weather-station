//#include "sys.h"
#include "i2c.h"
#include "sht20.h"
#include "delay.h"
/*
#define noACK 0
#define ACK 1
*/
/****************
*注：本函数中由于使用本工程中的延时函数dalay_2us故已经将全部延时参数值改为源码的
*两倍
*
*********************/



void SHT20Write(unsigned char rxbuf,unsigned char type)
{
  IIC_SendByte(rxbuf,type);
  IIC_ChkACK(type);
}

unsigned char SHT20Read(char ack,unsigned char type)
{
  unsigned char value;
  
  value=IIC_RecvByte(type);
  IIC_SendACK(ack,type);
  
  return value;
}

void SHT20SoftReset(unsigned char type)
{
  IIC_Start(type);
  SHT20Write(0x80,type);
  SHT20Write(0xFE,type);
  IIC_Stop(type);
}


unsigned int SHT20MeasureHM(unsigned char type)
{
  unsigned int value;
  
  IIC_Start(type);
  SHT20Write(0x80,type);
  
  SHT20Write(type,type);
  
  IIC_Start(type);
  SHT20Write(0x81,type);
  delay_ms(500);
  
  value=SHT20Read(0,type);
  value<<=8;
  value|=SHT20Read(0,type);
  SHT20Read(1,type);
  IIC_Stop(type);
  
  return value;
}
//这个才是真正的获取HT20数据的函数
void SHT20(float *temp,float *hum)
{ 
//  unsigned char value;
  unsigned int sRH;
  unsigned int sT;
  float vRH;
  float vT;
  
  sT =SHT20MeasureHM(TEMP);
  sRH=SHT20MeasureHM(HUMI);
  
  if((sRH==0xffff&&sT==0xffff)||((sRH==0&&sT==0)))
  {
    vRH=0;
    vT=0;
  }
  else
  {
    sRH &= ~0x0003;
    vRH = -6.0 + 125.0/65536 * (float)sRH;
    
    sT &= ~0x0003;
    vT = -46.85 + 175.72/65536 *(float)sT;
  }
  *hum=vRH;
  *temp=vT;
}

