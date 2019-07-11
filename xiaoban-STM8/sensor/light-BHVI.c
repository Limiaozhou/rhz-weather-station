#include <iostm8s103F3.h>
#include "i2c.h"
#include "light-BHVI.h"
#include "delay.h"
#include "uart.h"

//*********************************
void BH1750_Write(char addr)
{
    IIC_Start(LIGHT);                  //��ʼ�ź�
    IIC_SendByte(LIGHT,LIGHT);   //�����豸��ַ+д�ź�
    IIC_SendByte(addr,LIGHT);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    IIC_Stop(LIGHT);                   //����ֹͣ�ź�
}

//*********************************************************
//��������BH1750�ڲ�����
//*********************************************************
void BH1750_Read(char *pbuf)
{   
    char i, flag = 1; 
    IIC_Start(LIGHT);                          //��ʼ�ź�
    IIC_SendByte(LIGHT+1,LIGHT);         //�����豸��ַ+���ź�
    flag = IIC_ChkACK(LIGHT);
    if(!flag)
    {
        for (i=0; i<=3; i++)                   //������ȡ6����ַ���ݣ��洢��BUF
        {
            *pbuf++ = IIC_RecvByte(LIGHT);         //BUF[0]�洢0x32��ַ�е�����
            if (i == 3)     IIC_SendACK(1,LIGHT);   //���һ��������Ҫ��NOACK
            else            IIC_SendACK(0,LIGHT);   //��ӦACK
        }
    }
    
    IIC_Stop(LIGHT);                           //ֹͣ�ź�
    delay_ms(5);
}

float BH1750_ReadDat(void)
{
  char buf[4]={0,0,0,0};
    int data;
   
    BH1750_Write(0x01);   // power on
    BH1750_Write(0x10);   // H- resolution mode
        
    delay_ms(180);         //��ʱ180ms
    BH1750_Read(buf);         //�����������ݣ��洢��BUF��
    data = (buf[0] << 8) + buf[1];
    
//     float_to_str(ptr,light_data,2);
//     UART_Send("light: ",7);
//     UART_Send(ptr,8);
    
    return (float)data/1.2;
   
}
