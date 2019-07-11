#include "voice.h"
#include "hcho.h"
//P0.7
//unsigned int voice = 0;


float Get_voice(void)
{
	return (Get_Adc_Average(7,2)/4096)*3.3;
//  float tmp=0;
//  unsigned char i=0;
//  for(i=0; i<30; i++)
//    tmp = ADC_ReadValue();
//  float dat;
//  tmp = tmp/30;
//  float tmp_data;
//  tmp_data = (float)(tmp/2047)*100;
//  return (float)(ADC_ReadValue()/2048)*3.3;
//  tmp_data = (float)(tmp/2047);
//  *data=(unsigned char)(tmp_data+0.5);
/*  data[0] = (int)tmp_data;
  tmp = tmp_data - data[0];
  data[1] = (int)(tmp*100);
  */
}
//测量电压范围/(2^AD位数-1)
