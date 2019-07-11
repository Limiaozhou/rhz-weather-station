//#include"iostm8s103K3.h"
#include"iostm8s103F3.h"
#include"typedef.h"
#include"delay.h"

#define         EEPMASS1     0xAE                   //����Կ��1 
#define         EEPMASS2     0x56                   //����Կ��2 
#define         EEPADDR      0x4000                  //eep��ַ��ʼ

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

  FLASH_DUKR=EEPMASS1;                //��1��Կ  
  FLASH_DUKR=EEPMASS2;                //��2��Կ 
  while((!(FLASH_IAPSR&0x08)) && count--)         //�ȴ����ܾ���
  {
     delay_ms(1);
  }
 
  if(count<1) return false;
  
  asm("sim");//���ж� 
  FLASH_CR2 |= 0x40;     //               
  FLASH_NCR2 &= ~0x40;   //�����ֱ�� 
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
  asm("rim");            //��ȫ���ж�
  
  return true;
}