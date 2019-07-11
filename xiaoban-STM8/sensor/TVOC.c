#include <iostm8s103F3.h>
#include "i2c.h"
#include "light-BHVI.h"
#include "delay.h"
#include "uart.h"
#include "tvoc.h"

#include "bsp_iic_master.h"

//*********************************************************
//��������BTVOC�ڲ�����
//*********************************************************
//void TVOC_Read(char *pbuf)
//{   
//    char i;
//    char flag = 1;
//    IIC_Start(TVOC_1);                          //��ʼ�ź�
//    IIC_SendByte(TVOC_1,TVOC_1);         //�����豸��ַ+���ź�
//    flag = IIC_ChkACK(TVOC_1);
//    if(!flag)
//    {
//        for (i=0; i<=12; i++)                   //������ȡ8�����ݣ��洢��BUF
//        {
//            *pbuf++ = IIC_RecvByte(TVOC_1);         //BUF[0]�洢0x32��ַ�е�����
//            if (i == 12)     IIC_SendACK(1,TVOC_1);   //���һ��������Ҫ��NOACK
//            else            IIC_SendACK(0,TVOC_1);   //��ӦACK
//        }
//    }
//
//    IIC_Stop(TVOC_1);                           //ֹͣ�ź�
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
