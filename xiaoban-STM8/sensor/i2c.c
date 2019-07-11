#include "delay.h"
#include "i2c.h"
#include "data.h"


#define SCL_H()  SCL = 1
#define SCL_L()  SCL = 0
#define SDA_H()  SDA = 1
#define SDA_L()  SDA = 0

void IIC_init()
{
  PB_DDR|=0x10;//PB4
  PB_CR1|=0x10;//推挽输出
  PB_CR2&=0xEF;  //2M输出   
  
  PB_DDR|=0x20;//PB5
  PB_CR1|=0x20;//推挽输出
  PB_CR2&=0xDf;  //2M输出  
}
void IIC_SDA_READ(void)
{  
  PB_DDR &= ~0x20;//PB5
  PB_CR1 &= ~0x20;//
  PB_CR2 &=0xDf;  //
}

void IIC_SDA_WRITE(void)
{
  PB_DDR|=0x20;//PB5
  PB_CR1|=0x20;//推挽输出
  PB_CR2 &=0xDf;  //2M输出 
}
/*
void TVOC_SDA_READ()
{
  PB_DDR &= ~0x20;//PB5
  PB_CR1 &= ~0x20;//
  PB_CR2 &=  0xDf;  //
}

void TVOC_SDA_WRITE()
{
  PB_DDR|=0x20;//PB7
  PB_CR1|=0x20;//推挽输出
  PB_CR2 &=0xDf;  //2M输出 
}*/
/**************************************

**************************************/
void IIC_Start(unsigned char type)
{
  switch(type)
  {
  case TEMP:
  case HUMI:
  case LIGHT:                     //光照强度
  case PM25:  
  case TVOC_1:
    {
      SDA_H();                    //?????
      SCL_H();                    //?????
      delay_2us(DTIME);                 //??delay_ms (50);    //at least 5ms 
      SDA_L();                   //?????
      delay_2us(DTIME);                 //??
      SCL_L();                    //?????
      delay_2us(DTIME);                 //??
    }
    break;
//  case PRESS:
//    {
//      SDA_H(); 
//      SCL_H();  
//      delay_ms(DTIME);                 //延时
//      SDA_L();   
//      delay_2us(DTIME);                 //延时
//      SCL_L();  
//      delay_2us(DTIME);                 //延时
//    }
//    break;
  }
  
}

/**************************************
????
**************************************/
void IIC_Stop(unsigned char type)
{
  switch(type)
  {
  case HUMI:
  case TEMP:  
  case PRESS:
  case PM25:     
  case LIGHT:             //光照强度
  case TVOC_1:
  {
     // SCL_L(); 
      SDA_L();                    //?????
      delay_2us(DTIME);
      SCL_H();                    //?????
      delay_2us(DTIME);                 //??
      SDA_H();                    //?????
      delay_2us(DTIME);                 //??
      //SCL_L(); 
     // delay_2us(DTIME);                 //??
    }
     break;
  }
}

/**************************************
??????
????:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(char ack,unsigned char type)
{
  switch(type)
  {
  case HUMI:
  case TEMP:
  case PRESS:
  case PM25:   
  case LIGHT:                //光照强度  
  case TVOC_1:
    {
      SDA=ack;
      SCL_H();                   //?????
      delay_2us(DTIME);                 //??
      SCL_L();                     //?????
      delay_2us(DTIME);                 //??
    }
    break;
  }
}

/**************************************
??????
**************************************/
char IIC_ChkACK(unsigned char type)
{   
  int i=500,flag;
  switch(type)
  {
  case HUMI:
  case TEMP:
  case PM25:   
  case LIGHT:                //光照强度  
  case TVOC_1:
    {
      SDA = 1;
      IIC_SDA_READ();
     
      SCL_H();                     //?????
      delay_2us(DTIME);
      while(i)
      {
        if(SDAIN ==0)
        { 
          i=0;
          flag=0;
        }
        else
        {
          i--;
        }
      }
      flag = SDAIN;                   //?????
      SCL_L();                   //?????
      delay_2us(DTIME);                 //??
      IIC_SDA_WRITE();
        
    }
    break;
//  case PRESS:    
//    IIC_SDA_READ();
//  
//    SCL_H(); 
//    delay_2us(DTIME);
//    if(SDAIN==1)
//    {	  
//      delay_2us(DTIME);//unlock the IIC bus  	  
//      if(SDAIN==1)
//      {	
//        flag=0;				//??????????????????????????no handle the ack
//      }
//      else
//      {
//        flag = SDAIN; 
//      }
//    }  
//    else
//    {
//      flag=0;
//    }
//    break;
  }
  IIC_SDA_WRITE();  

  return flag;
}

/**************************************
?IIC??????????
**************************************/
void IIC_SendByte(char dat,unsigned char type)
{
  char i;
  switch(type)
  {
  case HUMI:
  case TEMP:  
  case PM25:   
  case LIGHT:                //光照强度
  case TVOC_1:
    {
      for (i=0; i<8; i++)         //8????
      {
        if(dat & 0x80)
          SDA_H();
        else
          SDA_L();
        
        dat <<= 1;              //????????
        delay_2us(DTIME);             //??
        SCL_H();                //?????
        delay_2us(DTIME);             //??
        SCL_L();                //?????
        delay_2us(DTIME);             //??
      }
      if(type==LIGHT)
        IIC_ChkACK(type);
    }
    break;
//  case PRESS:                //压力
//    for (i=0; i<8; i++)         //8????
//    {
//      SCL=0;                //?????
//      delay_2us(DTIME);             //??
//      if(dat & 0x80)
//        SDA=1;
//      else
//        SDA=0;
//      
//      dat <<= 1;              //????????
//      delay_2us(DTIME);             //??
//      SCL=1;                //?????
//      delay_2us(DTIME);             //??
//    } 
//    SCL=0;                //?????
//    delay_2us(DTIME);             //??
//    SDA=1;
//    delay_2us(DTIME);
//    break;
   }
}

/**************************************
?IIC??????????
**************************************/
char IIC_RecvByte(unsigned char type)
{
  char i;
  char dat = 0;
  
  switch(type)
  {
//  case PRESS:
//    SCL=0;
//    delay_2us(DTIME);
//    SDA=0;
//    delay_2us(DTIME);
  case HUMI:
  case TEMP:  
  case PM25:   
  case LIGHT:                //光照强度
  case TVOC_1:
    {
      SDA_H();
      IIC_SDA_READ();
   
      delay_2us(DTIME);             //??
      for (i=0; i<8; i++)      //8????
      {
        dat <<= 1;
        SCL_H();                //?????
        delay_2us(DTIME);            //??
        dat |= SDAIN;             //???   
        SCL_L();                //?????
        delay_2us(DTIME);            //??
      }
      
      IIC_SDA_WRITE();
   
      delay_2us(DTIME);             //??
    }
    break;
  }
  return dat;
}

//*********************************************************
//????????
//*********************************************************
void IIC_Read(char *pbuf, char cnt, unsigned char type)
{   
   asm("sim");//关全局中断
  char flag=1; 
  IIC_Start(type);    //PM25 0x3C
  IIC_SendByte(type,type);
  flag = IIC_ChkACK(type);
  delay_2us(100);
  if(flag==0)
  {
    while(cnt--)                   //????6?????,???BUF
    {
      
      *pbuf++ = IIC_RecvByte(type);         //BUF[0]??0x32??????
      if (cnt == 0)   IIC_SendACK(1,type);   //?????????NOACK
      else            IIC_SendACK(0,type);   //??ACK
      delay_2us(DTIME); 
      IWDG_Feed();
    }
    
    IIC_Stop(type); 
  }
  IIC_Stop(type);                           //????
  asm("rim");
}

void IIC_WriteByte(char pbuf, unsigned char type)
{
  IIC_Start(type);  
  IIC_SendByte(pbuf,type);
  IIC_Stop(type); 
}
void IIC_Write(char *pbuf, char cnt, unsigned char type)
{   
  IIC_Start(type);                          //????
  IIC_SendByte(type,type);
  if(IIC_ChkACK(type))
  {
    IIC_Stop(type); 
    return;
  }
  
  while(cnt--)                   //????6?????,???BUF
  {
    IIC_SendByte(*pbuf++,type);         //BUF[0]??0x32??????
    if (cnt > 0)   IIC_SendACK(0,type);   //?????????NOACK
    else           IIC_SendACK(1,type);
  }
  
  IIC_Stop(type); 
}


