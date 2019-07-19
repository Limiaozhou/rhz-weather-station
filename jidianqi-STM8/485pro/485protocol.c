#include "data.h"
#include "uart.h"
#include "485protocol.h"
#include "mem.h"
#include "check.h"
#include "eeprom.h"
#include "control.h"

//char Flash_addr[2]={0,1};
unsigned char Flash_data[6]={0,1,0,1,0,0};//前两个是地址，后两个表示波特率
unsigned char send_485_data[160];
short start_addr=122;
#define LIGHT_ADDR      0X009A



typedef union  
{   
   float  value;   
   struct   
    {  
        unsigned char low_byte;  
        unsigned char mlow_byte;  
        unsigned char mhigh_byte;  
        unsigned char high_byte;  
     }float_byte; 
}Float4Byte; 

/***************************************
*@DESCRIPTION: --字符串转float
*
*@Input: --无
*
*@Out: --float
*
*@Return: --float
****************************************/	
float chartofloat(uchar *pdat)
{
  Float4Byte temp;
  
//  temp.float_byte.low_byte = *pdat++;
//  temp.float_byte.mlow_byte = *pdat++;
//  temp.float_byte.mhigh_byte = *pdat++;
//  temp.float_byte.high_byte = *pdat;
    temp.float_byte.high_byte = *pdat++;
  temp.float_byte.mhigh_byte = *pdat++;
  temp.float_byte.mlow_byte = *pdat++;
  temp.float_byte.low_byte = *pdat;
  return temp.value;
}





float error;
void send_error(unsigned char addr,unsigned char key)
{
  *send_485_data=addr;
  *(send_485_data+1)=0xC7;
  *(send_485_data+2)=5;
  *(send_485_data+3)=key;
  mem_copy_convert_port(send_485_data+4,(unsigned char *)&error,4);

  Check_CalaCRC16(send_485_data,8);
  UART_Send(send_485_data,10);
}

short get_short(unsigned char data[2])
{
  short tmp;
  tmp=*data;
  tmp<<=8;
  tmp+=*(data+1);
  return tmp;
}
extern int time1 ;
extern int time2 ;
extern int time3 ;
int pro485(unsigned char *cmd)
{
  int i=0,j=0,cunt=10;

  char sum = 0,a=0;
  int daar = 0;
  float date = 0;
  char dat[4];
  int Num;
 // char t=3;
  int data;
//  if((*cmd==Flash_data[1]||*cmd==0)&&(Check_CheckCRC16(cmd, 6)))
  if((*cmd==0X8A||*cmd==0)&&(Check_CheckCRC16(cmd, 9)))
  {
    Num=get_short(cmd+2);           // 起始地址
    data=get_short(cmd+4);          //读取数据个数或者写入的数据
    switch(*(cmd+1))
    {
        case 0x03:                                     //读寄存器地址
  /*       if(error)
          {
            send_error(Flash_data[1],3);
          }
          *(send_485_data)=Flash_data[1];            //本机地址
          *(send_485_data+1)=0x03;
          mem_copy(send_485_data+2,cmd+2,2);         //起始地址
         
          *(send_485_data+2)=data*2;               //回传数据位数
          j=(Num-start_addr)/2;
          if(!(Num%2)&&!(data%2))
          {	
            if(Num==0)                                                         //本机编号以及波特率
            {
              mem_copy(&send_485_data[3],(unsigned char *)&Flash_data,4);
              t=7;
              j=0;
            }
            
            if(j<=(32-((Num-start_addr)/2)))                            //CO2 到气压连续的地址空间读取
            {
              for(;i<data/2;i++)                                        
              {
                mem_copy_convert_port(&send_485_data[t+4*i],((unsigned char*)((unsigned char*)(&Evndat.co2))+4*j++),4);
                send_485_data[3]= send_485_data[3];
              }
            }
            
            
              
            
//            if(Num==LIGHT_ADDR&&(data/2==1))                                     //读取光照  单个地址读取
//            {
//              mem_copy_convert_port(&send_485_data[t],(unsigned char*)((unsigned char*)(&Evndat.light_bhvi)),4);
//              send_485_data[3]= send_485_data[3];
//             }
            
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
          }*/
          break;
      
          case 0x06:                                //向寄存器写
            switch(Num)
            {
                case 0x00:
                  mem_copy(Flash_data,cmd+4,2);
                  cunt=10;
                  while(cunt--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,4))
                      break;
                  }
                  if(cunt<1)
                    *(send_485_data+2)=0x1;
                  else
                    *(send_485_data+2)=0x0;
                  break;
                case 0x01:
                  mem_copy(Flash_data+2,cmd+4,2);
                  if(data<5)
                  {
                    cunt=10;
                    while(cunt--)
                    {
                      if( eeprom_write(0x4000,(u8*)Flash_data,4))
                        break;
                    }
                    if(cunt<1)
                      *(send_485_data+2)=0x1;
                    else
                      *(send_485_data+2)=0x0;
                  }
                  else *(send_485_data+2)=0x1;
                  break;
            }
            
            *(send_485_data)=Flash_data[1];
            *(send_485_data+1)=0x06;
            
            Check_CalaCRC16(send_485_data, 3);
            UART_Send(send_485_data,5);
            
            break;
        case 0XB3:
   //       UART_Send("1245678",4);
         sum = *(cmd+2)/6;
         read485 = 1;
         for(a=0;a<sum;a++)
         {
           daar = (int)(*(cmd+3+a*6)*256+*(cmd+4+a*6));
           mem_copy(dat,cmd+5+a*6,4);
           date=chartofloat(dat);
           if(daar == 0x0150)
           {
                WAT.water1 = date;
                time2 = 0;
                time1 = 0;
           }
           else if(daar == 0x0152)
           {    
             time3 = 0;
             time1 = 0;
             if(date >= 1)
             {
               WAT.water1 = 0;
             }
              WAT.water2 = date;
           }
           else if(daar == 0x154)
           {
             if(date >= 0 && date <= 100)
             {
               Flash_data[4] = (char)date;
                WAT.valve4flow = date;
                cunt=10;
                  while(cunt--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  }
             }
           }
           else if(daar == 0x156)
           {
             if(date != 0)
             {
                WAT.water1time = date;
                Flash_data[2] = (char)date;
                cunt=10;
                  while(cunt--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  }
             }
           }
           else if(daar == 0x158)
           {
             if(date != 0)
             {
                WAT.water2time = date;
                Flash_data[3] = (char)date;
                cunt=10;
                  while(cunt--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  }
             }
           }
           else if(daar == 0x15A)
           {
             
           }
           else if(daar == 0x15C)
           {
             
           }
         }
          break;
      }
   }
  else 
  {
    return 1;
  }
  return 0;
}	
