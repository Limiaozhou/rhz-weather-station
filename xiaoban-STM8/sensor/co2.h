#ifndef CO2_H
#define CO2_H
#include "data.h"

#define EXTIC_vector           0x07
//#define u16 unsigned short int
#define u8  unsigned char 
#define u32 unsigned long 

void co2_init();
void Get_CO2(float *tempfs);

extern u8 CO2_flage;
extern u16 CO2_cnt;
extern u16 CO2_cnt_save;
extern u32 CO2temp_s;
#endif