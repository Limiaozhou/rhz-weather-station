#include"iostm8s103F3.h"
#include"delay.h"
#include"Clock.h"
#include"UART.h"
#include"eeprom.h"
//#include "i2c.h"
#include"config.h"
#include"timer.h"
#include "Iwdg.h"
//#include "sht20.h"
#include "data.h"
#include "mem.h"
#include "485protocol.h"
//#include "co2.h"
//#include "CCS811.h"
//#include "ms5611.h"
//#include "light-BHVI.h"

#include "control.h"

short int collect_time=0;
extern char Flash_data[6];

void float_to_str(uchar *pbuf,float num,char count)
{

     *pbuf++ = (int)num/1000 + 0x30;
     *pbuf++ = (int)num/100%10 + 0x30;
     *pbuf++ = (int)num/10%10 +0x30;
     *pbuf++ = (int)num%10 +0x30;
     *pbuf++ = '.';
     
     
     float dat = num- (int)num;
     while(count--)
     {
        dat *= 10;
        *pbuf++ = (char)dat + 0x30;
        dat = dat-(int)dat;
     }
     
     *pbuf++ = '\n';
}


int cunt1 = 0;
void main()
{
  Clock_Init();  //HSI-16MHz
  Init_Timer4();  //T = 2.048ms
  Init_Timer2();  //T=0.1ms
  
  asm("rim");//开全局中断 asm("rim");//开中断，sim为关中断
 // mem_clear((unsigned char*)&Evndat,sizeof(Evndat));
  
 // init_ms5611();                                 //气压传感器
            //设置波特率
  eeprom_read(0x4000,(u8*)Flash_data,6);          //读flash里的数据
  if(Flash_data[2] == 0 || Flash_data[2] >= 200)
  {
    Flash_data[2] = 5;
  /*  cunt1=10;
                  while(cunt1--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  } */
  }
  if(Flash_data[3] == 0 || Flash_data[3] >= 200)
  {
    Flash_data[3] = 5;
 /*   cunt1=10;
                  while(cunt1--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  }   */
  }
  if(Flash_data[4] == 0 || Flash_data[4] >= 100)
  {
    Flash_data[4] = 50;
    cunt1=10;
                  while(cunt1--)
                  {
                    if( eeprom_write(0x4000,(u8*)Flash_data,6))
                      break;
                  }   
  }
  WAT.water1time = Flash_data[2] ;
  WAT.water2time = Flash_data[3];
  WAT.valve4flow = Flash_data[4];
  struct_init();  //WAT结构体数据water1、water1初始为0
  io_init();  //继电器控制引脚
//  pwm_off();
  
  /*if(Flash_data[3]<6)
    UART_Init(Flash_data[3]);                       //初始化串口 
  else 
  {
    char cunt = 10;
    UART_Init(1);
    Flash_data[0]=0;
    Flash_data[1]=1;
    while(cunt--)
    {
      if( eeprom_write(0x4003,(u8*)Flash_data+2,2))
        break;
    }
  }*/
  UART_Init(1);  //9600
 // get_data();             //获取传感器采集的值            
 // IWDG_init();
  while (1)
  {
    if(collect_time%10 == 0)  //20.48ms
    {
//      control();  //设备控制引脚标志位赋值
      io_control();  //引脚控制
  //  delay_ms(1000);
  //  UART_Send("1245678",4);
 //   if(collect_time > )
     
//      if(collect_time>5000)//10.24s
//      {
//        collect_time = 0;
//          wat_data();  //串口发送数据
//      }
//       if(read485 == 1)
//      {
//        read485 = 0;
//        delay_ms(200);
//        wat_data();   //串口发送数据
        
 //       collect_time = 0;
//      }
    }
  }
}