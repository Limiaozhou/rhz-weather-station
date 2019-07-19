//#include <iocc2530.h>
#include "I2C.h"
///#include "co2.h"
#include "I2C_use.h"

//EVNDAT Evndat;
//ÉùÒô
void IIC_Voice(void)
{ 
  float ret = 0;
  char data[2]={0};
  IIC_Read(data,2,VOICE);
  ret = (float)(data[0]+data[1]*0.01);
  
  
}

//¼×È©
void IIC_Hcho(void)
{
  float ret = 0;
  char data[2]={0};
  IIC_Read(data,2,HCHO);
  ret = (float)(data[0]+data[1]*0.01);
  
}

static unsigned char sdat;
//TVOC
void IIC_Tvoc(void)
{
  long ret=0,ret1 = 0;
  char i,data[7]={0},buf[7]={0};
  
  for (i = 0; i < 7; i++)
      {
         buf[i] = sdat; // Define next outgoing byte
         sdat++;
      }
  //IIC_Write(buf, 7,0xA0);
  IIC_Read(data,7,TVOC);
  delay_ms(100);
  IIC_Write(buf, 7,0xA0);
  ret1 = data[0];
  ret1 = (ret1<<8)+data[1];
  
  ret = data[2];
  ret = (ret<<8)+data[3];
  ret = (ret<<8)+data[4];
  ret = (ret<<8)+data[5];
  
}

//PM2.5
void IIC_PM25(void)
{
  float ret = 0;
  char data[7]={0};
  //IIC_PMRead(data,7,PM25);
  ret++; 
}


void Get_Evndat(void)
{
   //float dat = 0,temp=0,humi=0;
   //Get_co2(&Evndat.co2);
   IIC_Voice();
   IIC_Hcho();
   IIC_Tvoc();
   IIC_PM25();
   //IIC_THRead(&Evndat.temp,&Evndat.humi);
   //get_HT(float *tem, float* rh);
}

