#include "i2c.h"
#include "ms5611.h"
#include "delay.h"

/****************
*注：本函数中由于使用本工程中的延时函数dalay_2us故已经将全部延时参数值改为源码的
*两倍
*
*********************/

#define D1_cmd 0x48
#define D2_cmd 0x58
#define WRITE 0x00
#define READ  0X01
unsigned int MS5611_param[8];
int error_flag =0;
unsigned int height_temp;
unsigned long MS_D1;
unsigned long MS_D2;
unsigned long MS_P;
double height;
unsigned char height_H;
unsigned char height_L;
unsigned char ppph;
unsigned char pppm;
unsigned char pppl;
unsigned char temp_H;
unsigned char temp_M;
unsigned char temp_L;

#define zz8        0x100
#define zz23       0x800000
#define zz16       0x10000
#define zz7        0x80
#define zz15       0x8000
#define zz21       0x200000
#define zz23       0x800000
#define zz31       0x80000000

void IIC_MS5611rest(unsigned char sensor_addr)
{
  IIC_Start(PRESS); 
  IIC_SendByte(sensor_addr+WRITE,PRESS);     //csb接地
  if(IIC_ChkACK(PRESS)==0)
  {
    IIC_SendByte(0x1e,PRESS);
    if(IIC_ChkACK(PRESS)==0)
    {
      error_flag=0;
    }
    else   error_flag=1;
  }
  else   error_flag=1;
  IIC_Stop(PRESS);  
} 

unsigned int IIC_prom_read(unsigned char MS5611_addr,unsigned char sensor_addr)
{ 
  unsigned int prom_temp;
  int d1 ,d2;
  IIC_Start(PRESS); 
  IIC_SendByte(sensor_addr+WRITE,PRESS);
  if(IIC_ChkACK(PRESS)==0)
  {
    IIC_SendByte(0xa0+(MS5611_addr*2),PRESS);
    if(IIC_ChkACK(PRESS)==0)
    {
      IIC_Stop(PRESS);
      IIC_Start(PRESS); 
      IIC_SendByte(sensor_addr+READ,PRESS);
      if(IIC_ChkACK(PRESS)==0)
      {
        d1 = IIC_RecvByte(PRESS);
        IIC_SendACK(0,PRESS);
        if(IIC_ChkACK(PRESS)==0)
        {
          d2 = IIC_RecvByte(PRESS);
          prom_temp = ((unsigned int)d1<<8)|d2;
          IIC_SendACK(1,PRESS);
          error_flag=0;
          delay_2us(20);
        }
        else   error_flag=1;
      }
      else   error_flag=1;
    }
    else   error_flag=1;
  }
  else   error_flag=1;
  IIC_Stop(PRESS);
  return  prom_temp;
}


unsigned long MS5611_ADC(unsigned char cmd_byte,unsigned char sensor_addr)
{
  unsigned long MS_temp;
  IIC_Start(PRESS); 
  IIC_SendByte(sensor_addr+WRITE,PRESS);
  if(IIC_ChkACK(PRESS)==0)
  {
    IIC_SendByte(cmd_byte,PRESS);
    if(IIC_ChkACK(PRESS)==0)
    {
      IIC_Stop(PRESS);
      IIC_Start(PRESS); 
      IIC_SendByte(sensor_addr+WRITE,PRESS);
      if(IIC_ChkACK(PRESS)==0)
      {
        IIC_SendByte(0x00,PRESS);
        if(IIC_ChkACK(PRESS)==0)
        {
          IIC_Stop(PRESS);
          IIC_Start(PRESS); 
          IIC_SendByte(sensor_addr+READ,PRESS);       //kaishiduzhuju
          if(IIC_ChkACK(PRESS)==0)
          {
            MS_temp=IIC_RecvByte(PRESS);
            IIC_SendACK(0,PRESS);
            MS_temp=MS_temp*256+IIC_RecvByte(PRESS);
            IIC_SendACK(0,PRESS);
            MS_temp=MS_temp*256+IIC_RecvByte(PRESS);
            IIC_SendACK(1,PRESS);
            error_flag=0;
          }
          else   error_flag=1;
        }
        else   error_flag=1;
      }
      else   error_flag=1;
    }
    else   error_flag=1;
  }
  else   error_flag=1;
  IIC_Stop(PRESS);
  return  MS_temp;
}

void init_ms5611()
{
  int param_count;
  
  IIC_MS5611rest(0xee);										
  
  for(param_count=0;param_count<8;param_count++)
  {
    MS5611_param[param_count]= IIC_prom_read(param_count,0xee);
    
  }  
}

void MS_gain_P_T(float *press_temp,float *press)
{
  double PressureValue;
  double dT;
  double OFF;
  double SENS;
  double temp;
//  unsigned long DD0;
  long temperature;
  unsigned long T2;
  if(MS5611_param[0]!=65535)
  {
  MS_D1=MS5611_ADC(D1_cmd,0xee);  
  MS_D2=MS5611_ADC(D2_cmd,0xee);
  delay_ms(10);
  
  dT=(double)(MS_D2);
  temp=(double)(MS5611_param[5]);
  dT=dT-temp*zz8;
  
  temp=(double)(MS5611_param[6]);
  temperature=dT*temp/zz23+2000;			 //TEMPERATURE results
  if(temperature<2000)
  {
    T2=dT*dT/zz31;
    temperature=temperature-T2;
  }
  temperature/=100;
  if(temperature>=-40 && temperature<=85)
    *press_temp=(float)temperature;
/*  if(*press_temp>85 || *press_temp<-40)
    *press_temp=0;*/
  temperature=temperature+99000;			 //ajust the temp around 100000
  temp_H=temperature>>16;
  temp_M=temperature>>8;
  temp_L=temperature;
  
  temp=(double)(MS5611_param[2]);
  OFF=temp*zz16;
  temp=(double)(MS5611_param[4]);
  OFF=OFF+dT*temp/zz7;
  temp=(double)(MS5611_param[1]);
  SENS=temp*zz15;
  temp=(double)(MS5611_param[3]);
  SENS=SENS+dT*temp/zz8;
  temp=(double)(MS_D1);
  PressureValue=(SENS*temp/zz21-OFF)/zz15;          //PRESSURE results
 // DD0=(unsigned long)(PressureValue);
  PressureValue/=100;
  if(PressureValue>=10 && PressureValue<=1200)
    *press=PressureValue;
//  *press=DD0/100;
  }
  else 
  {
    *press=0;
    *press_temp=0;
  }
 /* MS_P=DD0;
  ppph=DD0>>16;
  pppm=DD0>>8;
  pppl=DD0;*/
}
