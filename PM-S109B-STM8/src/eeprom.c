//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"typedef.h"
#include"delay.h"

#define         EEPMASS1     0xAE                   //密码钥匙1 
#define         EEPMASS2     0x56                   //密码钥匙2 
#define         EEPADDR      0x4000                  //eep地址起始

void eeprom_read(ushort Addr, uchar *pdatas, uchar len) 
{ 
  uchar *eep=(uchar *)Addr; 
  uchar j; 

  for(j=0; j<len; j++) 
  { 
    pdatas[j]=eep[j];         
  } 
} 

bool eeprom_write(ushort Addr, uchar *pdatas, uchar len)
{ 
  uchar *eep=(uchar *)Addr; 
  uchar j;
  int count=500;

  FLASH_DUKR=EEPMASS1;                //锁1密钥  
  FLASH_DUKR=EEPMASS2;                //锁2密钥 
  while((!(FLASH_IAPSR&0x08)) && count--)         //等待解密就绪
  {
     delay_ms(1);
  }
 
  if(count<1) return false;
  
  asm("sim");//关中断 
  FLASH_CR2 |= 0x40;     //               
  FLASH_NCR2 &= ~0x40;   //设置字编程 
  for(j=0; j<len; j++) 
  { 
    eep[j] = pdatas[j]; 
  }
  
  count=500;
  while((!(FLASH_IAPSR&0x04)) && count--)
  {
     delay_ms(1);
  }
  
  if(count<1) return false;
  
  FLASH_IAPSR &=0xf7;    // off the eeprom program
  asm("rim");            //开全局中断
  
  return true;
}