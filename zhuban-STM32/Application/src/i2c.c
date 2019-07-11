#include "i2c.h"
#include "sys.h"
#include "delay.h"


GPIO_InitTypeDef IIC_InitStructure;

//#define SCL GPIO_Pin_1
//#define SDA GPIO_Pin_5
#define DTIME 100

void IIC_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO , ENABLE);
  GPIOA->CRL &=~0X00F00000; 
	GPIOA->CRL |= 0X00300000;
  GPIOB->CRL &=~0X000000F0; 
	GPIOB->CRL |= 0X00000030;
  GPIOB->CRH &=~0X0000FF00; 
	GPIOB->CRH |= 0X00003300;
	GPIOA->BSRR = GPIO_Pin_1;
	GPIOB->BSRR = GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;
//	GPIOB->BSRR = GPIO_Pin_5;
}
void IIC_SDA_READ(GPIO_TypeDef* GPIOx_sda,uint16_t sda)
{
	uint16_t tmp=sda;
	while(tmp==1)
	{
		tmp=tmp>>1;
	}
	if(sda& ((uint32_t)0x00FF))
	{
		GPIOx_sda->CRL &=~(0xf<<20); 
		GPIOx_sda->CRL |= (0x4<<20); 
	}
	else
	{
		GPIOx_sda->CRH &=~(0xf<<20); 
		GPIOx_sda->CRH |= (0x4<<20); 
	}
//    GPIOA->CRL &=~0X00F00000; 
//    GPIOA->CRL |= 0X00400000; 
}

void IIC_SDA_WRITE(GPIO_TypeDef* GPIOx_sda,uint16_t sda)
{
	uint16_t tmp=sda;
	while(tmp==1)
	{
		tmp=tmp>>1;
	}
	if(sda& ((uint32_t)0x00FF))
	{
		GPIOx_sda->CRL &=~(0xf<<20); 
		GPIOx_sda->CRL |= (0x3<<20); 
	}
	else
	{
		GPIOx_sda->CRH &=~(0xf<<20); 
		GPIOx_sda->CRH |= (0x3<<20); 
	}
//    GPIOA->CRL &=~0X00F00000; 
//    GPIOA->CRL |= 0X00300000; 
}

void IIC_Start(GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
	GPIOx_sda->BSRR = sda;                    //拉高数据线
	GPIOx_scl->BSRR = scl;                    //拉高时钟线
	delay_us(DTIME);                 //延时
	GPIOx_sda->BRR = sda;                    //拉高数据线
	delay_us(DTIME);                 //延时
	GPIOx_scl->BRR = scl;                    //拉高时钟线
	delay_us(DTIME);                 //延时
}

void IIC_Stop(GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
  GPIOx_sda->BRR = sda;                   //拉低数据线
	GPIOx_scl->BRR = scl;
  delay_us(DTIME);
  GPIOx_scl->BSRR = scl;                    //拉高时钟线
  delay_us(DTIME);                 //延时
  GPIOx_sda->BSRR = sda;                    //产生上升沿
  delay_us(DTIME);                 //延时
}
	
void IIC_SendACK(char ack,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
	if(ack)
		GPIOx_sda->BSRR = sda;
	else
		GPIOx_sda->BRR = sda;
  GPIOx_scl->BSRR = scl;                     //拉高时钟线
  delay_us(DTIME);                 //延时
  GPIOx_scl->BRR = scl;                    //拉低时钟线
  delay_us(DTIME);                 //延时
}	
	
char IIC_ChkACK(GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
	int i=500,flag;
  GPIOx_sda->BSRR = sda;
  IIC_SDA_READ(GPIOx_sda,sda);
  GPIOx_scl->BSRR = scl;                    //拉高时钟线
  delay_us(DTIME);
  while(i)
  {
    if(GPIO_ReadInputDataBit(GPIOx_sda,sda)==0)
    { 
			i=0;
			flag=0;
		}
		else
		{
			i--;
		}
	}
	flag = GPIO_ReadInputDataBit(GPIOx_sda,sda);                   //读应答信号
	GPIOx_scl->BRR = scl;                   //拉低时钟线
  delay_us(DTIME);                 //延时
    
	IIC_SDA_WRITE(GPIOx_sda,sda);  
	return flag;
}	

void IIC_SendByte(char dat,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
	char i;
  for (i=0; i<8; i++)         //8位计数器
  {
    if(dat&0x80)   GPIOx_sda->BSRR = sda;
		else          GPIOx_sda->BRR = sda;
  
		dat <<= 1;              //移出数据的最高位
    delay_us(DTIME);             //延时
    GPIOx_scl->BSRR = scl;                //拉高时钟线
    delay_us(DTIME);             //延时
    GPIOx_scl->BRR = scl;                //拉低时钟线
    delay_us(DTIME);             //延时
  }
}

char IIC_RecvByte(GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
  char i;
  char dat = 0;
  GPIOx_sda->BSRR = sda;
  IIC_SDA_READ(GPIOx_sda,sda);
  delay_us(DTIME);             //延时
    
  for (i=0; i<8; i++)      //8位计数器
  {
		dat <<= 1;
    GPIOx_scl->BSRR = scl;                //拉高时钟线
    delay_us(DTIME);            //延时
    dat |= GPIO_ReadInputDataBit(GPIOx_sda,sda);             //读数据   
    GPIOx_scl->BRR = scl;                 //拉低时钟线
    delay_us(DTIME);            //延时
	}
      
	IIC_SDA_WRITE(GPIOx_sda,sda);
	delay_us(DTIME);             //延时    
	return dat;
}	

void IIC_Read(char *pbuf, char cnt, unsigned char type,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{
    char flag=1; 
    IIC_Start(GPIOx_sda,sda,GPIOx_scl,scl);                          //起始信号
    IIC_SendByte(type,GPIOx_sda,sda,GPIOx_scl,scl);
    flag = IIC_ChkACK(GPIOx_sda,sda,GPIOx_scl,scl);
    if(flag==0)
    {
       while(cnt--)                   //连续读取6个地址数据，存储中BUF
       {
          *pbuf++ = IIC_RecvByte(GPIOx_sda,sda,GPIOx_scl,scl);         //BUF[0]存储0x32地址中的数据
          if (cnt == 0)   IIC_SendACK(1,GPIOx_sda,sda,GPIOx_scl,scl);   //最后一个数据需要回NOACK
          else            IIC_SendACK(0,GPIOx_sda,sda,GPIOx_scl,scl);   //回应ACK
       }

      IIC_Stop(GPIOx_sda,sda,GPIOx_scl,scl); 
    }
 /*   else
    {
      IIC_Stop(type); 
      EA=1;
      return;
    }*/
     IIC_Stop(GPIOA,sda,GPIOB,scl);                           //停止信号
}
char IIC_XPPM25(char reg, unsigned char type,GPIO_TypeDef* GPIOx_sda,uint16_t sda, GPIO_TypeDef* GPIOx_scl,uint16_t scl)
{   
//    EA=0;
    char temp;
    IIC_Start(GPIOx_sda,sda,GPIOx_scl,scl); 
    IIC_SendByte(type<<1|0,GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_ChkACK(GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_SendByte(reg,GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_ChkACK(GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_Start(GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_SendByte(type<<1|1,GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_ChkACK(GPIOx_sda,sda,GPIOx_scl,scl);
    
    temp=IIC_RecvByte(GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_SendACK(1,GPIOx_sda,sda,GPIOx_scl,scl);
    IIC_Stop(GPIOx_sda,sda,GPIOx_scl,scl);
    return temp;
}
