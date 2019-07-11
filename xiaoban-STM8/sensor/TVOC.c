#include <iostm8s103F3.h>
#include "i2c.h"
#include "light-BHVI.h"
#include "delay.h"
#include "uart.h"
#include "tvoc.h"

#include "bsp_iic_master.h"

//*********************************************************
//连续读出BTVOC内部数据
//*********************************************************
//void TVOC_Read(char *pbuf)
//{   
//    char i;
//    char flag = 1;
//    IIC_Start(TVOC_1);                          //起始信号
//    IIC_SendByte(TVOC_1,TVOC_1);         //发送设备地址+读信号
//    flag = IIC_ChkACK(TVOC_1);
//    if(!flag)
//    {
//        for (i=0; i<=12; i++)                   //连续读取8个数据，存储中BUF
//        {
//            *pbuf++ = IIC_RecvByte(TVOC_1);         //BUF[0]存储0x32地址中的数据
//            if (i == 12)     IIC_SendACK(1,TVOC_1);   //最后一个数据需要回NOACK
//            else            IIC_SendACK(0,TVOC_1);   //回应ACK
//        }
//    }
//
//    IIC_Stop(TVOC_1);                           //停止信号
//    delay_ms(5);
//}

void TVOC_ReadDat(float * tvoc, float * tovcco2)
{
  unsigned long int ret = 0, ret1 = 0;
  unsigned char data[13] = {0};
//  TVOC_Read(data);
  IIC_Master_Read(2, TVOC_DEVICE_ADDRESS, 0, data, 13);
  
  if((data[0]==0xFF) && (data[3]!=0x02))
  {
    ret1 = data[1];
    ret1 = (ret1<<8)+data[2];
    
    ret = data[8];
    ret = (ret<<8)+data[9];
    
    if(ret < 125)
      ret = 125;
    
    if(ret1>=400 && ret1<=65535)
        *tovcco2 = ret1;
      
    *tvoc = (float)ret / 1000.0;
//    if (ret >= 125 && ret<=65535)
//    {
//      if(ret1>=0 && ret1<=65535)
//      {
//        *tovcco2 = ret1;
//      }
//      *tvoc = (float)ret / 1000.0;
//    }
//    else
//    {
//      *tovcco2 = 0;
//      *tvoc = 0;
//    }
  }
  else
  {
    *tovcco2 = 0;
    *tvoc = 0;
  }
}
