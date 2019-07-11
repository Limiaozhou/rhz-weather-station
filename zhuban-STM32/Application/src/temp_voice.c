#include "data.h"
#include "sys.h"
#include "delay.h"
#include "temp_voice.h"
#include "timer.h"
#include "i2c.h"
#include "usart.h"
#include "delay.h"

/**************************************
?e那?D?o?
**************************************/
void IIC_Start()
{
    //SCL = 0;                    //角-米赤那㊣?車??
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;                    //角-??那y?Y??
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //角-??那㊣?車??
    delay_us(DTIME);                 //?車那㊣
    GPIO_ResetBits(GPIOB,SDA);//SDA = 0;                    //2迆谷迆???米??
    delay_us(DTIME);                 //?車那㊣
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //角-米赤那㊣?車??
    delay_us(DTIME);                 //?車那㊣
}

/**************************************
赤㏒?1D?o?
**************************************/
void IIC_Stop()
{
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0; 
    GPIO_ResetBits(GPIOB,SDA);//SDA = 0;                    //角-米赤那y?Y??
    delay_us(DTIME);
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //角-??那㊣?車??
    delay_us(DTIME);                 //?車那㊣
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;                    //2迆谷迆谷?谷y??
    delay_us(DTIME);                 //?車那㊣
}

/**************************************
﹞⊿?赤車|∩eD?o?
豕??迆2?那y:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(char ack)
{
    if (ack)
      GPIO_SetBits(GPIOB,SDA);  
    else
      GPIO_ResetBits(GPIOB,SDA); //    SDA = ack;                  //D∩車|∩eD?o?
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //角-??那㊣?車??
    delay_us(DTIME);                 //?車那㊣
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //角-米赤那㊣?車??
    delay_us(DTIME);                 //?車那㊣
}

/**************************************
?車那?車|∩eD?o?
**************************************/
char IIC_ChkACK()
{   
    int i=500,flag;
    IIC_SDA_READ();
    GPIO_SetBits(GPIOB,SCL);//SCL = 1;                    //角-??那㊣?車??
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
    flag = GPIO_ReadInputDataBit(GPIOB,SDA);//SDA;                   //?芍車|∩eD?o?
    GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                    //角-米赤那㊣?車??
    delay_us(DTIME);                 //?車那㊣
    
    IIC_SDA_WRITE();    
    return flag;
}

/**************************************
?辰IIC℅邦??﹞⊿?赤辰???℅??迆那y?Y
**************************************/
void IIC_SendByte(char dat)
{
    char i;

    for (i=0; i<8; i++)         //8????那y?¯
    {
        if(dat&0x80)  GPIO_SetBits(GPIOB,SDA);//SDA=1;
        else          GPIO_ResetBits(GPIOB,SDA);//SDA=0;

        dat <<= 1;              //辰?3?那y?Y米?℅?????
        delay_us(1);             //?車那㊣
        GPIO_SetBits(GPIOB,SCL);//SCL = 1;                //角-??那㊣?車??
        delay_us(DTIME);             //?車那㊣
        GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                //角-米赤那㊣?車??
        delay_us(DTIME);             //?車那㊣
    }
     
}

/**************************************
∩車IIC℅邦???車那?辰???℅??迆那y?Y
**************************************/
char IIC_RecvByte()
{
    char i;
    char dat = 0;
    
    GPIO_SetBits(GPIOB,SDA);//SDA = 1;
    IIC_SDA_READ();
    delay_us(DTIME);             //?車那㊣
                             //那1?邦?迆2?谷?角-,℅?㊣??芍豕?那y?Y,
    for (i=0; i<8; i++)      //8????那y?¯
    {
        dat <<= 1;
        GPIO_SetBits(GPIOB,SCL);//SCL = 1;                //角-??那㊣?車??
        delay_us(DTIME);            //?車那㊣
        dat |= GPIO_ReadInputDataBit(GPIOB,SDA);             //?芍那y?Y               
        GPIO_ResetBits(GPIOB,SCL);//SCL = 0;                //角-米赤那㊣?車??
        delay_us(DTIME);            //?車那㊣
    }
    
    IIC_SDA_WRITE();
    delay_us(DTIME);             //?車那㊣
    
    return dat;
}

//*********************************************************
//芍?D??芍3??迆2?那y?Y
//*********************************************************
void IIC_Read(char *pbuf, char cnt, unsigned char type)
{   
//    EA=0;
    char flag=1; 
    IIC_Start();                          //?e那?D?o?
    IIC_SendByte(type);
    flag = IIC_ChkACK();
    if(flag==0)
    {
       while(cnt--)                   //芍?D??芍豕?6??米??﹞那y?Y㏒?∩?∩⊿?DBUF
       {
          *pbuf++ = IIC_RecvByte();         //BUF[0]∩?∩⊿0x32米??﹞?D米?那y?Y
          if (cnt == 0)   IIC_SendACK(1);   //℅?o車辰???那y?YD豕辰a??NOACK
          else            IIC_SendACK(0);   //??車|ACK
       }

      IIC_Stop(); 
    }
    
     IIC_Stop();                           //赤㏒?1D?o?
//     EA=1;
}

//*********************************************************
//芍?D??芍3?PM2.5?迆2?那y?Y
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
   
    while(cnt--)                   //芍?D??芍豕?6??米??﹞那y?Y㏒?∩?∩⊿?DBUF
    {
       *pbuf++ = IIC_RecvByte();         //BUF[0]∩?∩⊿0x32米??﹞?D米?那y?Y
       if (cnt == 0)   IIC_SendACK(1);   //℅?o車辰???那y?YD豕辰a??NOACK
       else            IIC_SendACK(0);   //??車|ACK
    }
   
     IIC_Stop();                           //赤㏒?1D?o?
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
