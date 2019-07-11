#include "i2c.h"
#include "CCS811.h"
#include "data.h"
#include "delay.h"


char ccs811_init_flag=0;//ccs811初始化成功标志位

void IIC_CCS811Write(uchar address,uchar reg,uchar *txbuf,uchar len)
{
   int i;
  IIC_Start(PM25);
  IIC_SendByte(address<<1|0,PM25);
  IIC_ChkACK(PM25);
  IIC_SendByte(reg,PM25);
  IIC_ChkACK(PM25);

  for(i=0;i<len;i++)
  {
    IIC_SendByte(txbuf[i],PM25);
    IIC_ChkACK(PM25);
  }
  IIC_Stop(PM25);
  
}

void  IIC_CCS811Read(uchar address,uchar reg,uchar *rxbuf,uchar len)
{
  int i;
  
  IIC_Start(PM25);
  IIC_SendByte(address<<1|0,PM25);
  IIC_ChkACK(PM25);
  IIC_SendByte(reg,PM25);
  IIC_ChkACK(PM25);
  
  IIC_Start(PM25);
  IIC_SendByte(address<<1|1,PM25);
  IIC_ChkACK(PM25);
  
  for(i=0;i<len;i++)
  {
    rxbuf[i]=IIC_RecvByte(PM25);
    if(i==(len-1))
      IIC_SendACK(1,PM25);
    else
      IIC_SendACK(0,PM25);
  }
  IIC_Stop(PM25);
}

int CCS811Init(void)
{
  uchar i2c_buff[8]={0x11,0xE5,0x72,0x8A};
  unsigned int TimeOver=0;
  
  
  IIC_CCS811Write(0x5A,0xFF,i2c_buff,4);
  
  while(TimeOver++<10)
  {
    IIC_CCS811Read(0x5A,0x20,i2c_buff,1);
    if(i2c_buff[0]==0x81)
      break;
    delay_ms(100);
  }
  if(TimeOver>=10)
    return 1;
  TimeOver=0;
  
  while(TimeOver++<10)
  {
    IIC_CCS811Read(0x5A,0x00,i2c_buff,1);
    if(i2c_buff[0]&0x10)
      break;
    delay_ms(100);
  }
  if(TimeOver>=10)
    return 1;
  TimeOver=0;

  IIC_CCS811Write(0x5A,0xF4,i2c_buff,0);
  
  while(TimeOver++<10)
  {
    IIC_CCS811Read(0x5A,0x00,i2c_buff,1);
    if((i2c_buff[0]&0x10)&&(i2c_buff[0]&0x80))
      break;
    delay_ms(100);
  }
  if(TimeOver>=10)
    return 1;
  
  i2c_buff[0]=0x18;
  IIC_CCS811Write(0x5A,0x01,i2c_buff,1);
  delay_ms(100);
  ccs811_init_flag=1;
  return 0;
}
float error=0;
char pm_flag=0;
char pm_count=0;
void IIC_CCS811(void)
{
  Evndat.tvocco2=0;
  Evndat.TVOC=0;
  if(ccs811_init_flag==0)  CCS811Init();
  else
  {
    unsigned char i2c_buff[8];
    int data;
    IIC_CCS811Read(0x5A,0x00,i2c_buff,1);
    if(i2c_buff[0]&0x08)
    {
      IIC_CCS811Read(0x5A,0x02,i2c_buff,8);
      if(i2c_buff[5]==0)
      {
      data=i2c_buff[0];
      data<<=8;
      data|=i2c_buff[1];
      if(!(data>5000||data<400))        
        Evndat.tvocco2=(float)data;  //还未调试成功该处暂时注释掉
      data=i2c_buff[2];
      data<<=8;
      data|=i2c_buff[3];
      if(data<=1000) 
        Evndat.TVOC=(float)data;  
      error=0;
      }
      else
      {
        //IIC_CCS811Read(0x5A,0xe0,i2c_buff,1);
        error=(float)i2c_buff[5]+40;
        ccs811_init_flag=0;
      }
    }
  }
}