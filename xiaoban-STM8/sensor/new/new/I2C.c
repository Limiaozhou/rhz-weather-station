#include"iostm8s103K3.h"
//#include"iostm8s103F3.h"
#include"typedef.h"
#include "delay.h"
#include "I2C.h"
//��ȡSTM8S C8051F350ģ���豸ʹ��

#define SCL   PB_ODR_ODR6
#define SDAI  PB_IDR_IDR7
#define SDA   PB_ODR_ODR7

//#define SCL P1_3 
//#define SDA P1_4 
//#define IIC_SDA_READ()   P1DIR&=~0x10;
//#define IIC_SDA_WRITE()  P1DIR|=0x10;

#define DTIME 5

static void SDA_out(void)   //����SDA����Ϊ���
{
    PB_DDR|=0x80;//PB7
    PB_CR1|=0x80;//�������
    PB_CR2&=0x7f;        
}

static void SDA_in(void)   //����SDA����Ϊ����
{
    PB_DDR&=0x7f;//PB7
    PB_CR1|=0x80;//��������
    PB_CR2&=0x7f;        
}

static void SCL_out(void)  //����SCL����Ϊ���
{
    PB_DDR|=0x40;//PB6
    PB_CR1|=0x40;//�������
    PB_CR2&=0xbf;        
}

static void SCL_in(void)   //����SCL����Ϊ����
{
    PB_DDR&=0xbf;//PB6
    PB_CR1|=0x40;//��������
    PB_CR2&=0xbf;        
}
/*
//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void IIC_init()
{
   P1SEL &= ~0X18;
   P1DIR |= 0X18;
}
*/
/**************************************
��ʼ�ź�
**************************************/
void IIC_Start()
{
    SCL_out();
    SDA_out();
    //SCL = 0;                    //����ʱ����
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    delay_us(DTIME);                 //��ʱ
    SDA = 0;                    //�����½���
    delay_us(DTIME);                 //��ʱ
    SCL = 0;                    //����ʱ����
    delay_us(DTIME);                 //��ʱ
}

/**************************************
ֹͣ�ź�
**************************************/
void IIC_Stop()
{
    SDA_out();
    //SCL = 0; 
    SDA = 0;                    //����������
    delay_us(DTIME);
    SCL = 1;                    //����ʱ����
    delay_us(DTIME);                 //��ʱ
    SDA = 1;                    //����������
    delay_us(DTIME);                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void IIC_SendACK(char ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    delay_us(DTIME);                 //��ʱ
    SCL = 0;                    //����ʱ����
    delay_us(DTIME);                 //��ʱ
}

/**************************************
����Ӧ���ź�
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
��IIC���߷���һ���ֽ�����
**************************************/
void IIC_SendByte(char dat)
{
    char i;

    for (i=0; i<8; i++)         //8λ������
    {
        if(dat&0x80)  SDA=1;
        else          SDA=0;

        dat <<= 1;              //�Ƴ����ݵ����λ
        delay_us(1);             //��ʱ
        SCL = 1;                //����ʱ����
        delay_us(DTIME);             //��ʱ
        SCL = 0;                //����ʱ����
        delay_us(DTIME);             //��ʱ
    }
     
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
unsigned char IIC_RecvByte()
{
    char i;
    unsigned char dat = 0;
    
    SDA = 1;
    //IIC_SDA_READ();
    SDA_in();
    delay_us(DTIME);             //��ʱ
                             //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)      //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        delay_us(DTIME);            //��ʱ
        dat |= SDAI;             //������               
        SCL = 0;                //����ʱ����
        delay_us(DTIME);            //��ʱ
    }
    
    //IIC_SDA_WRITE();
    SDA_out();
    delay_us(DTIME);             //��ʱ
    
    return dat;
}

//*********************************************************
//���������ڲ�����
//*********************************************************
void IIC_Read(char *pbuf, char cnt, unsigned char type)
{   
    //EA=0;
    char i; 
    IIC_Start();                          //��ʼ�ź�
    IIC_SendByte(type);
    if(IIC_ChkACK())
    {
       IIC_Stop(); 
       return;
    }
    
    while(cnt--)                   //������ȡ6����ַ���ݣ��洢��BUF
    {
       *pbuf++ = IIC_RecvByte();         //BUF[0]�洢0x32��ַ�е�����
       if (cnt > 0)   IIC_SendACK(0);   //���һ��������Ҫ��NOACK
       else           IIC_SendACK(1);
    }

    
    IIC_Stop(); 
}

//*********************************************************
//����д����
//*********************************************************
void IIC_Write(char *pbuf, char cnt, unsigned char type)
{   
    //EA=0;
    unsigned char buf[5];
    char i; 
    IIC_Start();                          //��ʼ�ź�
    IIC_SendByte(type);
    if(IIC_ChkACK())
    {
       IIC_Stop(); 
       return;
    }
    
    while(cnt--)                   //������ȡ6����ַ���ݣ��洢��BUF
    {
       IIC_SendByte(*pbuf++ );         //BUF[0]�洢0x32��ַ�е�����
       if (cnt >= 0)   IIC_SendACK(0);   //���һ��������Ҫ��NOACK
       //else           IIC_SendACK(1);
    }

    delay_us(30);            //��ʱ
    IIC_Stop(); 
}



