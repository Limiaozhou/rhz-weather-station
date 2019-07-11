#include "sht20.h"
#include "data.h"
#include "i2c.h"
//#include "CCS811.h"
#include "delay.h"
#include "co2.h"
//#include "ms5611.h"
#include "light-BHVI.h"
#include "check.h"
#include "mem.h"
#include "Iwdg.h"

#include "tvoc.h"
#include "pressure-GY.h"
#include "PM.h"

//#define LK
EVNDAT Evndat;
//unsigned char sbuf[4]={0};
//unsigned char sbuf1[4]={0};
//unsigned char sbuf2[4]={0};
//unsigned char sbuf3[4]={0};
//
//unsigned char data[6]={0};
//unsigned char data1[6]={0};
//typedef union  
//{   
//   float  value;   
//   struct   
//    {  
//        unsigned char low_byte;  
//        unsigned char mlow_byte;  
//        unsigned char mhigh_byte;  
//        unsigned char high_byte;  
//     }float_byte; 
//}Float4Byte; 
//float chartofloat(uchar *pdat)
//{
//  Float4Byte temp;
//  
//  temp.float_byte.low_byte = *pdat++;
//  temp.float_byte.mlow_byte = *pdat++;
//  temp.float_byte.mhigh_byte = *pdat++;
//  temp.float_byte.high_byte = *pdat;
//  return temp.value;
//}
//void IIC_PM25(void)
//{
//  float ret = 0;
//
////  IIC_Read(data,6,PM25);
//  UART_Send(data,6);
//  if(Check_CheckCRC16(data,4))
//  {
////    UART_Send(data,6);
//      mem_copy_convert_port(data1, data,4);      //co2
//    ret = chartofloat(data1);
////    while((UART1_SR & 0x80)==0x00);
////    { 
////      UART1_DR=ret;
////    }
////    ret = (float)(data[0]*100+data[1]);
//    if(ret<=5000)
//    {
//      
//      if(ret>=5)
//      {
//         Evndat.pm25 = ret;
//      }
//      else
//      {
//        Evndat.pm25 = 5;
//      }
//    }
//  
////    while((UART1_SR & 0x80)==0x00);
////    { 
////      UART1_DR=Evndat.pm25;
////    }
//  }
// 
//}


unsigned char send_dat[100]; //要发送的数据
void package_send()
{
  unsigned char len;
 
    *send_dat=0x55;
    *(send_dat+1)=0xaa;
    len = 5 + 6 * 7;
    *(send_dat+2)=len;
    
    *(send_dat+3)=0x00;
    *(send_dat+4)=0x80;
    mem_copy_convert_port(send_dat+5,(unsigned char*)&(Evndat.temp20),4); 
    
    *(send_dat+ 9)=0x00;
    *(send_dat+10)=0x82;
    mem_copy_convert_port(send_dat+11,(unsigned char*)&(Evndat.humi20),4);    
    
    *(send_dat+15)=0x00;
    *(send_dat+16)=0x06;
    mem_copy_convert_port(send_dat+17,(unsigned char*)&(Evndat.light_bhvi),4);   
    
    *(send_dat+21)=0x00;
    *(send_dat+22)=0x7A;
    mem_copy_convert_port(send_dat+23,(unsigned char*)&(Evndat.co2),4);
    
    *(send_dat+27)=0x00;
    *(send_dat+28)=0x7C;
    mem_copy_convert_port(send_dat+29,(unsigned char*)&(Evndat.TVOC),4);
    
    *(send_dat+33)=0x00;
    *(send_dat+34)=0x84;  //气压
    mem_copy_convert_port(send_dat+35,(unsigned char*)&(Evndat.press),4);
    
    *(send_dat+39)=0x00;
    *(send_dat+40)=0x68;
    mem_copy_convert_port(send_dat+41,(unsigned char*)&(Evndat.pm25),4);
    
    Check_CalaCRC16(send_dat, len-2);
    
    UART_Send(send_dat, len);
      
    IWDG_Feed();
}

void get_data(void)
{
  SHT20(&Evndat.temp20,&Evndat.humi20);  //温湿度，IIC
  Get_CO2(&Evndat.co2);                  //CO2，PWM
  Evndat.light_bhvi= BH1750_ReadDat();   //光照，IIC
  TVOC_ReadDat(&Evndat.TVOC, &Evndat.tvocco2);  //TVOC，IIC
  Pressure_Convert();  //气压，IIC
  PM_Read();  //PM，IIC
  
//  delay_ms(10);
//  delay_2us(50);
  
//  mem_copy_convert_port(sbuf, (unsigned char*)&(Evndat.co2),4);      //co2
//  UART_Send(sbuf,4);
//  mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.pm25),4);      //co2
//  UART_Send(send_485_data+3,4);
//  
//  mem_copy_convert_port(sbuf1, (unsigned char*)&(Evndat.temp20),4);      //temp20
//  UART_Send(sbuf1,4);
//  
//  mem_copy_convert_port(sbuf2, (unsigned char*)&(Evndat.humi20),4);      //humi20
//  UART_Send(sbuf2,4);
//  
//  mem_copy_convert_port(sbuf3, (unsigned char*)&(Evndat.light_bhvi),4);      //light_bhvi
//  UART_Send(sbuf3,4);
  
//  Evndat.num1=0;
//  Evndat.num2=0;
//  Evndat.num3=0;
//  Evndat.num4=0;
//  Evndat.num5=0;
//  Evndat.num6=0;
//  Evndat.num7=0;
//  Evndat.num8=0;
    package_send();
 }