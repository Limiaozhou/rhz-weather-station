//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include "delay.h"
//#include"ADC.h"
#include"Clock.h"
#include "UART.h"
#include"IO.h"
//#include"eeprom.h"
//#include"I2C_slave.h"
#include "i2c.h"
//#include"config.h"
//#include"package.h"
#include "timer.h"
#include "Iwdg.h"

//unsigned char buf[100];


//extern int j;
//extern unsigned char open_pump[6];

main()
{
  unsigned int t = 0, m = 0;
  Clock_Init();
  //ADC_Init();
  UART_Init();
  IO_Init();
////  Init_I2C_Slave();
  IIC_Init();
  delay_ms(100);
  IWDG_init();
//  //Config_Read();
//  Init_Timer4();  
  asm("rim");//开全局中断 asm("rim");//开中断，sim为关中断
  
  while (1)
  {
//    if(t++ >= 3000)
//    {
//      UART_Send(read_pm25, 5);
//      t = 0;
//    }
//    if(t==1500)
//      {
//        UART_Send(open_pump,6);
//      }
    if(m++ >= 1000)
    {
      IWDG_KR = 0xAA;
      m = 0;
    }
    
    Deal();
 //   Config_Deal();
 //   Package_Deal();
  }
}