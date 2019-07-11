#include <iostm8s103F3.h>
#include "i2c.h"
#include "light-BHVI.h"
#include "delay.h"
#include "uart.h"

//*********************************
void BH1750_Write(char addr)
{
    IIC_Start(LIGHT);                  //起始信号
    IIC_SendByte(LIGHT,LIGHT);   //发送设备地址+写信号
    IIC_SendByte(addr,LIGHT);    //内部寄存器地址，请参考中文pdf22页 
    IIC_Stop(LIGHT);                   //发送停止信号
}

//*********************************************************
//连续读出BH1750内部数据
//*********************************************************
void BH1750_Read(char *pbuf)
{   
    char i, flag = 1; 
    IIC_Start(LIGHT);                          //起始信号
    IIC_SendByte(LIGHT+1,LIGHT);         //发送设备地址+读信号
    flag = IIC_ChkACK(LIGHT);
    if(!flag)
    {
        for (i=0; i<=3; i++)                   //连续读取6个地址数据，存储中BUF
        {
            *pbuf++ = IIC_RecvByte(LIGHT);         //BUF[0]存储0x32地址中的数据
            if (i == 3)     IIC_SendACK(1,LIGHT);   //最后一个数据需要回NOACK
            else            IIC_SendACK(0,LIGHT);   //回应ACK
        }
    }
    
    IIC_Stop(LIGHT);                           //停止信号
    delay_ms(5);
}

float BH1750_ReadDat(void)
{
  char buf[4]={0,0,0,0};
    int data;
   
    BH1750_Write(0x01);   // power on
    BH1750_Write(0x10);   // H- resolution mode
        
    delay_ms(180);         //延时180ms
    BH1750_Read(buf);         //连续读出数据，存储在BUF中
    data = (buf[0] << 8) + buf[1];
    
//     float_to_str(ptr,light_data,2);
//     UART_Send("light: ",7);
//     UART_Send(ptr,8);
    
    return (float)data/1.2;
   
}
