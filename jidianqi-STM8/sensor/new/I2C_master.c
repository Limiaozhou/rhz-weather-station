#include"iostm8s103K3.h"
//#include"iostm8s103F3.h"
#include"typedef.h"
#include"delay.h"
//������ȡ���ݣ�ģ�⣩��

#define SCL   PB_ODR_ODR6
#define SDAI  PB_IDR_IDR7
#define SDA   PB_ODR_ODR7

uchar iictemp[12];//�洢��ȡ��������

void SDA_out(void)   //����SDA����Ϊ���
{
    PB_DDR|=0x80;//PB7
    PB_CR1|=0x80;//�������
    PB_CR2&=0x7f;        
}

void SDA_in(void)   //����SDA����Ϊ����
{
    PB_DDR&=0x7f;//PB7
    PB_CR1|=0x80;//��������
    PB_CR2&=0x7f;        
}

void SCL_out(void)  //����SCL����Ϊ���
{
    PB_DDR|=0x40;//PB6
    PB_CR1|=0x40;//�������
    PB_CR2&=0xbf;        
}

void SCL_in(void)   //����SCL����Ϊ����
{
    PB_DDR&=0xbf;//PB6
    PB_CR1|=0x40;//��������
    PB_CR2&=0xbf;        
}

/*��ʼ����*/
void Start(void)
{
        SCL_out();
        SDA_out();
        SCL=1;
        SDA=1;
        delay_us(1);
        SDA=0;
        delay_us(1);
        SCL=0;
}        
/*ֹͣ����*/
void Stop(void)
{
        SDA_out();
        SDA=0;
        delay_us(1);
        SCL=1;
        delay_us(1);
        SDA=1;
}

void IIC_w(uchar Idata)  //дһ���ֽ�
{
        uchar iici;
                for(iici=0;iici<8;iici++)
        {
                SCL=0;
                delay_us(1);
                if(Idata&0x80)
                SDA=1;
                else
                SDA=0;
                delay_us(1);
                SCL=1;
                Idata<<=1;
                delay_us(1);
        }
        SCL=0;
}

void IIC_r(uchar* Idata)  //��һ���ֽ�
{
        uchar iici;
        SDA=1;
        SDA_in();
         for(iici=0;iici<8;iici++)
  {
                SCL=1;
                delay_us(1);
                *Idata<<=1;
                if(SDAI)
                *Idata|=1;
                else
                *Idata|=0;
                delay_us(1);
                SCL=0;
                delay_us(1);
        }
}

void I2C_Send_Ack(void) //����ACK
{
        SDA=0;
        SDA_out();
        SCL=0;
        delay_us(1);
        SDA=0;
        delay_us(1);
        SCL=1;
        delay_us(1);
        SCL=0;
        }
        
unsigned char I2C_Get_Ack(void)  //��ȡACK
{
        uchar ack;
        SDA=1;
        SDA_in();
        delay_us(1);
        SCL=1;
        ack=SDAI;
        delay_us(1);
        SCL=0;
        SDA_out();
        return ack;
        
}


void IICdata_Read(void)  //���øú������ȡ���ӻ�����
{
        uchar i;
        Start();
        
        IIC_w(0xa1);
        if(I2C_Get_Ack())
        {
                Stop();
                return;
        }
        
        for(i=0;i<9;i++)
        {
                delay_us(1);
                IIC_r(&iictemp[i]);
                if(i<8)  I2C_Send_Ack();  //�������Ҫ�����һλ������ACK���Զ�ֹͣͨѶ
                
        }
        Stop();
}