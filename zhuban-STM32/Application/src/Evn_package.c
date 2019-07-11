#include "Evn_package.h"
#include "tea.h"
#include "check.h"
#include "data.h"
#include "mem.h"
//#include "hal_uart.h"

/***************************************
*@DESCRIPTION: 转换成发送数据格式
*
*@Input: unsigned char *pdest 转换后数据存放位置
         float data, 源数据
         unsigned char len  转换个数
*
*@Out: --无
*
*@Return: --none
****************************************/
void Format_Sendproc(unsigned char *pdest, float data,unsigned char len)
{   long d;
    switch (len)
    { 
      case 0x02:
           *pdest++ = (int)data;
           *pdest++ = (int)((data - (int)data+0.005)*100);
           break;
      case 0x03:
           *pdest++ = (int)data>>8;
           *pdest++ = (int)data&0xFF;
           *pdest++ = (int)((data - (int)data +0.005)*100);
           break;
      case 0x04:
           d = Evndat.ltvoc;
           *pdest++ = (unsigned long)d>>24;
           *pdest++ = (unsigned long)d>>16;
           *pdest++ = (unsigned long)d>>8;
           *pdest++ = (unsigned long)d&0xFF;
           break;
      case 0x05:
           *pdest++ = (unsigned long)data>>24;
           *pdest++ = (unsigned long)data>>16;
           *pdest++ = (unsigned long)data>>8;
           *pdest++ = (unsigned long)data&0xFF;
           *pdest++ = (int)((data - (unsigned long)data +0.005)*100);
           break;
      default:
           break;
    }
}
extern char Flash_code[8];
char address[8]={0x41,0x69,0x72,0x00,0x2,0x00,0x00,0x00};
void Evn_Packagedat(ST_EVNBALEDAT *pdat)
{
	 mem_copy(pdat->addr,Flash_code,8);
   pdat->head = 0xAA55;
   pdat->len = 46;
   pdat->temp.type = 0x04; //温度
   Format_Sendproc(pdat->temp.dat, Evndat.temp,2);
   pdat->humi.type = 0x05; //
   Format_Sendproc(pdat->humi.dat, Evndat.humi,2);
   pdat->co2.type = 0x07; //
   Format_Sendproc(pdat->co2.dat, Evndat.co2,5);
   
   pdat->hcho.type = 0x08; //
   Format_Sendproc(pdat->hcho.dat, Evndat.hcho,2);
   pdat->tvoc.type = 0x09; //
   Format_Sendproc(pdat->tvoc.dat, Evndat.tvocco2,2);
   Format_Sendproc(&pdat->tvoc.dat[2], Evndat.tvoc,4);
   
   pdat->pm25.type = 0x0A; //
   Format_Sendproc(pdat->pm25.dat, Evndat.pm25,3);
   pdat->pm10.type = 0x11; //
   Format_Sendproc(pdat->pm10.dat, Evndat.pm10,3);
   pdat->voice.type = 0x0B; //
   Format_Sendproc(pdat->voice.dat, Evndat.voice,2);

   //Check_CalaCRC16(&data[3],data_len+(4-data_len%4));
   pdat->dat[0]=0;
   pdat->dat[1]=0;
   pdat->dat[2]=0;
/*  if(pdat->temp.dat[0]==0)
  {
    HalUARTWrite(0,"send error:temp=0\n",22);
  }
  if(pdat->humi.dat[0]==0)
  {
    HalUARTWrite(0,"send error:humi=0\n",22);
  }*/
   btea_encrypt(pdat->addr,44);
   Check_CalaCRC16(pdat->addr,44);
   //int crc16;
}

