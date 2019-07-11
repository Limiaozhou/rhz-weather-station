#include "data.h"
#include "sys.h"
#include "delay.h"
#include "temp_voice.h"
#include "timer.h"
#include "i2c.h"
#include "usart.h"
#include "delay.h"

/**************************************
?e��?D?o?
**************************************/
void IIC_Start()
{
    //SCL = 0;                    //��-�̨�����?��??
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;                    //��-??��y?Y??
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //��-??����?��??
    delay_us(DTIME);                 //?������
    GPIO_ResetBits(GPIOB,SDA);//SDA = 0;                    //2������???��??
    delay_us(DTIME);                 //?������
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //��-�̨�����?��??
    delay_us(DTIME);                 //?������
}

/**************************************
����?1D?o?
**************************************/
void IIC_Stop()
{
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0; 
    GPIO_ResetBits(GPIOB,SDA);//SDA = 0;                    //��-�̨���y?Y??
    delay_us(DTIME);
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //��-??����?��??
    delay_us(DTIME);                 //?������
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;                    //2��������?��y??
    delay_us(DTIME);                 //?������
}

/**************************************
����?����|��eD?o?
��??��2?��y:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(char ack)
{
    if (ack)
      GPIO_SetBits(GPIOB,SDA);  
    else
      GPIO_ResetBits(GPIOB,SDA); //    SDA = ack;                  //D�䨮|��eD?o?
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //��-??����?��??
    delay_us(DTIME);                 //?������
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //��-�̨�����?��??
    delay_us(DTIME);                 //?������
}

/**************************************
?����?��|��eD?o?
**************************************/
char IIC_ChkACK()
{   
    int i=500,flag;
    IIC_SDA_READ();
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //��-??����?��??
    delay_us(DTIME);
    while(i)
    {
	    if(GPIO_ReadInputDataBit(GPIOB,SDA)==0)
	    {  i=0;
         flag=0;
      }
	    else
	    {
        i--;
      }
    }
    flag = GPIO_ReadInputDataBit(GPIOB,SDA);//SDA;                   //?����|��eD?o?
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //��-�̨�����?��??
    delay_us(DTIME);                 //?������
    
    IIC_SDA_WRITE();    
    return flag;
}

/**************************************
?��IIC����??����?����???��??����y?Y
**************************************/
void IIC_SendByte(char dat)
{
    char i;

    for (i=0; i<8; i++)         //8????��y?��
    {
        if(dat&0x80)  GPIO_SetBits(GPIOB,SDA);//SDA=1;
        else          GPIO_ResetBits(GPIOB,SDA);//SDA=0;

        dat <<= 1;              //��?3?��y?Y��?��?????
        delay_us(1);             //?������
        GPIO_SetBits(GPIOB,SCL);//SCL = 1;                //��-??����?��??
        delay_us(DTIME);             //?������
        GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                //��-�̨�����?��??
        delay_us(DTIME);             //?������
    }
     
}

/**************************************
�䨮IIC����???����?��???��??����y?Y
**************************************/
char IIC_RecvByte()
{
    char i;
    char dat = 0;
    
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;
    IIC_SDA_READ();
    delay_us(DTIME);             //?������
                             //��1?��?��2?��?��-,��?��??����?��y?Y,
    for (i=0; i<8; i++)      //8????��y?��
    {
        dat <<= 1;
        GPIO_SetBits(GPIOB,SCL);//SCL = 1;                //��-??����?��??
        delay_us(DTIME);            //?������
        dat |= GPIO_ReadInputDataBit(GPIOB,SDA);             //?����y?Y               
        GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                //��-�̨�����?��??
        delay_us(DTIME);            //?������
    }
    
    IIC_SDA_WRITE();
    delay_us(DTIME);             //?������
    
    return dat;
}

//*********************************************************
//��?D??��3??��2?��y?Y
//*********************************************************
void IIC_Read(char *pbuf, char cnt, unsigned char type)
{   
//    EA=0;
    char flag=1; 
    IIC_Start();                          //?e��?D?o?
    IIC_SendByte(type);
    flag = IIC_ChkACK();
    if(flag==0)
    {
       while(cnt--)                   //��?D??����?6??��??����y?Y��?��?���?DBUF
       {
          *pbuf++ = IIC_RecvByte();         //BUF[0]��?���0x32��??��?D��?��y?Y
          if (cnt == 0)   IIC_SendACK(1);   //��?o����???��y?YD����a??NOACK
          else            IIC_SendACK(0);   //??��|ACK
       }

      IIC_Stop(); 
    }
    
     IIC_Stop();                           //����?1D?o?
//     EA=1;
}

//*********************************************************
//��?D??��3?PM2.5?��2?��y?Y
//*********************************************************
void IIC_PMRead(char *pbuf, char cnt, unsigned char type)
{   
//    EA=0;
    
    IIC_Start(); 
    IIC_SendByte(type<<1|0);
    IIC_ChkACK();
    IIC_SendByte(0x01);
    IIC_ChkACK();
    IIC_Start();
    IIC_SendByte(type<<1|1);
    IIC_ChkACK();
   
    while(cnt--)                   //��?D??����?6??��??����y?Y��?��?���?DBUF
    {
       *pbuf++ = IIC_RecvByte();         //BUF[0]��?���0x32��??��?D��?��y?Y
       if (cnt == 0)   IIC_SendACK(1);   //��?o����???��y?YD����a??NOACK
       else            IIC_SendACK(0);   //??��|ACK
    }
   
     IIC_Stop();                           //����?1D?o?
//     EA=1;
}

void Get_voice(float *voice)
{
	float ret = 0;
  char data[2]={0};
  IIC_Read(data,2,VOICE);
  ret = (float)(data[0]+data[1]*0.01);
  if(ret>=0 && ret<=100)
  {
    *voice = ret;
  }
  else
  {
    *voice = 0;
  }
}
