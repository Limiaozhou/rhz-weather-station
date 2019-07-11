#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "set_info.h"
//extern char yang_on[7];
//extern char yang_off[7];
extern void update_sd_485(INFO_MUL sourse_info);
extern void update_flash();
extern void update_flash_485();
extern void Deal_Uart(unsigned char *uart_data,int len,int flag1);
extern int compare_vision(char newV[25],char oldV[25]);
extern unsigned char * deal_error(unsigned char key, float error);

#define Num_485 20

#endif