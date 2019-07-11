#include"iostm8s103K3.h"
//#include"iostm8s103F3.h"
#include"typedef.h"
#include "delay.h"
#include "I2C.h"
//读取STM8S C8051F350模拟设备使用

#define SCL   PB_ODR_ODR6
#define SDAI  PB_IDR_IDR7
#define SDA   PB_ODR_ODR7

//#define SCL P1_3 
//#define SDA P1_4 
//#define IIC_SDA_READ()   P1DIR&=~0x10;
//#define IIC_SDA_WRITE()  P1DIR|=0x10;

#define DTIME 5

static void SDA_out(void)   //配置SDA引脚为输出
{
    PB_DDR|=0x80;//PB7
    PB_CR1|=0x80;//推挽输出
    PB_CR2&=0x7f;        
}

static void SDA_in(void)   //配置SDA引脚为输入
{
    PB_DDR&=0x7f;//PB7
    PB_CR1|=0x80;//上拉输入
    PB_CR2&=0x7f;        
}

static void SCL_out(void)  //配置SCL引脚为输出
{
    PB_DDR|=0x40;//PB6
    PB_CR1|=0x40;//推挽输出
    PB_CR2&=0xbf;        
}

static void SCL_in(void)   //配置SCL引脚为输入
{
    PB_DDR&=0xbf;//PB6
    PB_CR1|=0x40;//上拉输入
    PB_CR2&=0xbf;        
}
/*
//初始化BH1750，根据需要请参考pdf进行修改****
void IIC_init()
{
   P1SEL &= ~0X18;
   P1DIR |= 0X18;
}
*/
/**************************************
起始信号
**************************************/
void IIC_Start()
{
    SCL_out();
    SDA_out();
    //SCL = 0;                    //拉低时钟线
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    delay_us(DTIME);                 //延时
    SDA = 0;                    //产生下降沿
    delay_us(DTIME);                 //延时
    SCL = 0;                    //拉低时钟线
    delay_us(DTIME);                 //延时
}

/**************************************
停止信号
**************************************/
void IIC_Stop()
{
    SDA_out();
    //SCL = 0; 
    SDA = 0;                    //拉低数据线
    delay_us(DTIME);
    SCL = 1;                    //拉高时钟线
    delay_us(DTIME);                 //延时
    SDA = 1;                    //产生上升沿
    delay_us(DTIME);                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(char ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    delay_us(DTIME);                 //延时
    SCL = 0;                    //拉低时钟线
    delay_us(DTIME);                 //延时
}

/**************************************
接收应答信号
**************************************/
char IIC_ChkACK()
{   
    uchar ack;
    SDA=1;
    SDA_in();
    delay_us(DTIME);
    SCL=1;
    ack=SDAI;
    delay_us(DTIME);
    SCL=0;
    SDA_out();
    return ack;
   
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void IIC_SendByte(char dat)
{
    char i;

    for (i=0; i<8; i++)         //8位计数器
    {
        if(dat&0x80)  SDA=1;
        else          SDA=0;

        dat <<= 1;              //移出数据的最高位
        delay_us(1);             //延时
        SCL = 1;                //拉高时钟线
        delay_us(DTIME);             //延时
        SCL = 0;                //拉低时钟线
        delay_us(DTIME);             //延时
    }
     
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
unsigned char IIC_RecvByte()
{
    char i;
    unsigned char dat = 0;
    
    SDA = 1;
    //IIC_SDA_READ();
    SDA_in();
    delay_us(DTIME);             //延时
                             //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)      //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        delay_us(DTIME);            //延时
        dat |= SDAI;             //读数据               
        SCL = 0;                //拉低时钟线
        delay_us(DTIME);            //延时
    }
    
    //IIC_SDA_WRITE();
    SDA_out();
    delay_us(DTIME);             //延时
    
    return dat;
}

//*********************************************************
//连续读出内部数据
//*********************************************************
void IIC_Read(char *pbuf, char cnt, unsigned char type)
{   
    //EA=0;
    char i; 
    IIC_Start();                          //起始信号
    IIC_SendByte(type);
    if(IIC_ChkACK())
    {
       IIC_Stop(); 
       return;
    }
    
    while(cnt--)                   //连续读取6个地址数据，存储中BUF
    {
       *pbuf++ = IIC_RecvByte();         //BUF[0]存储0x32地址中的数据
       if (cnt > 0)   IIC_SendACK(0);   //最后一个数据需要回NOACK
       else           IIC_SendACK(1);
    }

    
    IIC_Stop(); 
}

//*********************************************************
//连续写数据
//*********************************************************
void IIC_Write(char *pbuf, char cnt, unsigned char type)
{   
    //EA=0;
    unsigned char buf[5];
    char i; 
    IIC_Start();                          //起始信号
    IIC_SendByte(type);
    if(IIC_ChkACK())
    {
       IIC_Stop(); 
       return;
    }
    
    while(cnt--)                   //连续读取6个地址数据，存储中BUF
    {
       IIC_SendByte(*pbuf++ );         //BUF[0]存储0x32地址中的数据
       if (cnt >= 0)   IIC_SendACK(0);   //最后一个数据需要回NOACK
       //else           IIC_SendACK(1);
    }

    delay_us(30);            //延时
    IIC_Stop(); 
}



