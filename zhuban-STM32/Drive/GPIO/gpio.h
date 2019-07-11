#ifndef __GPIO_H
#define __GPIO_HIO_H	 
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////// 
//IO操作
//创建：mfg
//版本：V1.0
//修改日期:2015/7/31
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////  

void GPIO_SetOut(GPIO_TypeDef *GPIOx, int GPIO_pin);

void GPIO_SetIn(GPIO_TypeDef *GPIOx, int GPIO_pin);

void GPIO_SetValue(GPIO_TypeDef *GPIOx, int GPIO_pin, char value);

char GPIO_GetValue(GPIO_TypeDef *GPIOx, int GPIO_pin);

void IO_test(void);


#endif






