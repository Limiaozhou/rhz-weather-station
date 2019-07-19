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
  
  asm("rim");//��ȫ���ж� asm("rim");//���жϣ�simΪ���ж�
 // mem_clear((unsigned char*)&Evndat,sizeof(Evndat));
  
 // init_ms5611();                                 //��ѹ������
            //���ò�����
  eeprom_read(0x4000,(u8*)Flash_data,6);          //��flash�������
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
  struct_init();  //WAT�ṹ������water1��water1��ʼΪ0
  io_init();  //�̵�����������
//  pwm_off();
  
  /*if(Flash_data[3]<6)
    UART_Init(Flash_data[3]);                       //��ʼ������ 
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
 // get_data();             //��ȡ�������ɼ���ֵ            
 // IWDG_init();
  while (1)
  {
    if(collect_time%10 == 0)  //20.48ms
    {
//      control();  //�豸�������ű�־λ��ֵ
      io_control();  //���ſ���
  //  delay_ms(1000);
  //  UART_Send("1245678",4);
 //   if(collect_time > )
     
//      if(collect_time>5000)//10.24s
//      {
//        collect_time = 0;
//          wat_data();  //���ڷ�������
//      }
//       if(read485 == 1)
//      {
//        read485 = 0;
//        delay_ms(200);
//        wat_data();   //���ڷ�������
        
 //       collect_time = 0;
//      }
    }
  }
}