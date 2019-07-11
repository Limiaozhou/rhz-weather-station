#ifndef _COMMON_H_
#define _COMMON_H_

/******************@DESCRIPTION: -- GPRS��?��???��? *************/
#define   CONFIG_NULL    0
#define   CONFIG_START   1
#define   CONFIG_WAIT    2
#define   CONFIG_END     3

#define   CONFIG_NLINK    0
#define   CONFIG_INLINK   1
#define   CONFIG_GLINK    2

struct CONFIG_SendInfo
{
  unsigned char *pdat;
  unsigned char *pret;  //gprs?LR|?LRD|I????LR|
  unsigned char status; //!?|I???|I!e!?��0!A?La?Lo??y3!e!e?1!A?La?Lo??L��X?L?3!e!e?
  unsigned char check;  //
  unsigned char time;   //??L??LR|?Lo!A??
};

/******************@DESCRIPTION: -- GPRS��?��??����? *************/
struct Config_Info
{
  char *command;
  char *retComd;  //gprs?LR|?LRD|I????LR|
  unsigned char time;  //??L??LR|?Lo!A??
};

#endif