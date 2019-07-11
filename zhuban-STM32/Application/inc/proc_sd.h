#ifndef _PROC_SD_H_
#define _PROC_SD_H_

extern int BinToSD(unsigned char *name);
extern int Get_Bin_len(unsigned char *name);
extern void FromUartToSD(void);
void FromUartToSD2HAO(unsigned char *uart_data,int len);
extern void FromSDToUart(void);

#endif