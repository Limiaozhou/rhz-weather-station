#include "data.h"
#include "uart.h"
#include "485protocol.h"
#include "mem.h"
#include "check.h"
#include "eeprom.h"
#include "iostm8s103F3.h"

//char Flash_addr[2]={0,1};
unsigned char Flash_data[4]={0,0x66, 0,1};//前两个是地址，后两个表示波特率
unsigned char send_485_data[160];
short start_addr=104;//0x68 co2
#define LIGHT_ADDR      0X009A




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

int pro485(unsigned char *cmd)
{
  int i=0,j=0,cunt=10;
  int Num;
  char t=3;
  int data;
//  UART_Send(cmd, pos);//测试小板485功能
//    while((UART1_SR & 0x80)==0x00);
//    { 
//      UART1_DR=*cmd;
//    }
//     while((UART1_SR & 0x80)==0x00);
//    { 
//      UART1_DR=Flash_data[1];
//    }
  if((*cmd==Flash_data[1]||*cmd==0)&&(Check_CheckCRC16(cmd, 6)))
  {
//    UART_Send("UVWXY", 5);
    Num=get_short(cmd+2);           // 起始地址
    data=get_short(cmd+4);          //读取数据个数或者写入的数据
    switch(*(cmd+1))
    {
        case 0x03: 

          //读寄存器地址
          if(error)//?
          {
            send_error(Flash_data[1],3);
          }
          *(send_485_data)=Flash_data[1];  //66          //本机地址
          *(send_485_data+1)=0x03;
          *(send_485_data+2)=data*2;   //4     data=2       //回传数据位数

//          UART_Send(cmd+2,2);
         
//          mem_copy(send_485_data+2,cmd+2,2);         //起始地址
         
          if(cmd[2] == 0 && cmd[3]== 0x7a)
          {     
            //Big end to little end
            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.co2),4);      //co2
            IWDG_Feed();       
//            UART_Send(send_485_data+3,4);
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
            mem_clear(send_485_data,data*2+5);
          }
          
          if(cmd[2] == 0 && cmd[3]== 0x80)
          {     
            //Big end to little end
            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.temp20),4);      //temp20
            IWDG_Feed();       
//            UART_Send(send_485_data+3,4);
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
            mem_clear(send_485_data,data*2+5);
          }
          
          if(cmd[2] == 0 && cmd[3]== 0x82)
          {     
            //Big end to little end
            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.humi20),4);      //humi20
            IWDG_Feed();       
//            UART_Send(send_485_data+3,4);
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
            mem_clear(send_485_data,data*2+5);
          }
          
          
          
          if(cmd[2] == 0 && cmd[3]== 0x9a)
          {     
            //Big end to little end
            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.light_bhvi),4);      //light_bhvi
            IWDG_Feed();       
//            UART_Send(send_485_data+3,4);
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
            mem_clear(send_485_data,data*2+5);
          }
          
          
          
          
//            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.pm25),4);      //pm25
//  UART_Send(send_485_data+3,4);
          if(cmd[2] == 0 && cmd[3]== 0x68)
          {     
            //Big end to little end
            mem_copy_convert_port(send_485_data+3, (unsigned char*)&(Evndat.pm25),4);      //pm25
//            mem_copy(send_485_data+3, (unsigned char*)&(Evndat.pm25),4);      //pm25
//            UART_Send(send_485_data+3,4);
            IWDG_Feed();       
            Check_CalaCRC16(send_485_data, data*2+3);
            UART_Send(send_485_data,data*2+5);
            mem_clear(send_485_data,data*2+5);
          }
//          *(send_485_data+2)=data*2;   //4     data=2       //回传数据位数
//          j=(Num-start_addr)/2;//9
//          if(!(Num%2)&&!(data%2))
//          {	
//            if(Num==0)                                                         //本机编号以及波特率
//            {
//              mem_copy(&send_485_data[3],(unsigned char *)&Flash_data,4);
//              t=7;
//              j=0;
//            }
//            
//            if(j<=(32-((Num-start_addr)/2)))   //23                         //CO2 到气压连续的地址空间读取
//            {
//              for(;i<data/2;i++)                                        
//              {
//                mem_copy_convert_port(&send_485_data[t+4*i],((unsigned char*)((unsigned char*)(&Evndat.pm25))+4*j++),4);
//                send_485_data[3]= send_485_data[3];
//              }
//            }
//            //IWDG_Feed();       
//            Check_CalaCRC16(send_485_data, data*2+3);
////                      UART_Send(send_485_data, 9);
//            UART_Send(send_485_data,data*2+5);
//          }
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
      }
   }
  else 
  {
    return 1;
  }
  return 0;
}	
