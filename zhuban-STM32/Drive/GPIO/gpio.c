#include "stm32f10x.h"
////////////////////////////////////////////////////////////////////////////////// 
//GPIO操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    
/***************************************
*@DESCRIPTION: --初始化PB12 13 14为输出口.并使能这两个口的时钟		    
*
*@Input: --无
*
*@Out: --无
*
*@Return: --int
****************************************/	

void GPIO_SetOut(GPIO_TypeDef *GPIOx, int GPIO_pin)
{
  uint32_t tmp_gpio=0;
  int i=0;
  if(GPIO_pin < 8)
  {
    tmp_gpio=GPIOx->CRL;
    GPIOx->CRL &= (u32)(~3<<(GPIO_pin*4+2));
    GPIOx->CRL |= (u32)3<<(GPIO_pin*4);
    for(i=0;i<GPIO_pin;i++)
      GPIOx->CRL |= 0x0f<<(i*4)&tmp_gpio;
  }
  else if(GPIO_pin < 16)
  {
    tmp_gpio=GPIOx->CRL;
    GPIOx->CRH &= (u32)~3<<((GPIO_pin-8)*4+2);
    GPIOx->CRH |= (u32)3<<((GPIO_pin-8)*4);
    for(i=0;i<(GPIO_pin-8);i++)
      GPIOx->CRH |= 0x0f<<(i*4)&tmp_gpio;
  }
}
void GPIO_SetIn(GPIO_TypeDef *GPIOx, int GPIO_pin)
{
  u32 tmp_gpio=0;
  int i=0;
  if(GPIO_pin < 8)
  {
    tmp_gpio=GPIOx->CRL;
    GPIOx->CRL &= (u32)(~0X0F)<<(GPIO_pin*4);
    for(i=0;i<GPIO_pin;i++)
      GPIOx->CRL |= 0x0f<<(i*4)&tmp_gpio;
  }
  else if(GPIO_pin < 16)
  {
    tmp_gpio=GPIOx->CRH;
    GPIOx->CRH &= (u32)~(0X0F<<((GPIO_pin-8)*4));
    for(i=0;i<(GPIO_pin-8);i++)
      GPIOx->CRH |= 0x0f<<(i*4)&tmp_gpio;
  }
}
void GPIO_SetValue(GPIO_TypeDef *GPIOx, int GPIO_pin, char value)
{
  if(value)
  {
    GPIOB->ODR|=1<<GPIO_pin;
  }
  else 
  {
    GPIOB->ODR&=~(1<<GPIO_pin);
  }
}
char GPIO_GetValue(GPIO_TypeDef *GPIOx, int GPIO_pin)
{
  return (GPIOx->IDR & (1<<GPIO_pin));
}

void IO_test(void)
{
  GPIO_SetOut(GPIOB, 10);
  GPIO_SetValue(GPIOB, 10, 0);
  GPIO_SetValue(GPIOB, 10, 1);
  
  GPIO_SetIn(GPIOB, 10);
  GPIO_SetOut(GPIOB, 9);
  GPIO_SetValue(GPIOB, 8, GPIO_GetValue(GPIOB, 9));
}






