#ifndef GPRS_H
#define GPRS_H
#include "common.h"



void GPRS_Reset(void);
void SIM800_RxDeal(void);
void GPRS_Deal(void);
//void GPRS_Send(unsigned char *pdat, char len);
int Is_Connect(void);
void GPRS_init(void);
void get_csq(void);
void GPRS_SendCMD(unsigned char port,unsigned char *str,unsigned short int len);
extern void GPRS_Stat(void);
extern void send_3gData(unsigned char *ptmp,unsigned char len);
extern void Config_SendSetUp(char *pdat, char *pret,unsigned char time);

#endif 


