#include "led.h"
#include "sys.h"


void led_init(void)
{

  GPIOA->CRL &=~0X000000F0; 
  GPIOA->CRL |= 0X00000030;//pa1 = 0011 通用推挽输出模式,WIFI_RST

  GPIOB->CRH &=~0X0FFF000F; 
  GPIOB->CRH |= 0X03330003;//pb8=GPRS1_RST;12,13,14=G_LED,Y_LED,R_LED
  
  GPIOB->BSRR |= 0X70;
  GPIOA->BSRR |= 0X02;
  
}

void led_set(unsigned char stat)
{
  switch(stat)
  {
  case REDLIGHT:
    GPIO_ResetBits(GPIOB,REDLED);GPIO_SetBits(GPIOB,GREENLED|YELLOWLED);
    break;
  case GREENLIGHT:
    GPIO_ResetBits(GPIOB,GREENLED);GPIO_SetBits(GPIOB,REDLED|YELLOWLED);
    break;
  case YELLOWLIGHT:
    GPIO_ResetBits(GPIOB,YELLOWLED);GPIO_SetBits(GPIOB,REDLED|GREENLED);
    break;
  }
}
