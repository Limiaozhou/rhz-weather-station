#ifndef  YL_H
#define  YL_H

#define EXTIC_vector                 0x07
void exti_init(void);
void Get_rain(float *temprain);
extern char rain_flag;
#endif