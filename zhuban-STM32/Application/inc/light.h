#ifndef __LIGHT_H
#define __LIGHT_H
#include "sys.h"

void  Light_ADC_Init(void);
u16 Get_light_Adc(u8 ch);
double Get_light_Average(u8 ch,u8 times);
void Get_light(float *temp_light);

#endif
