#ifndef __HMI_H
#define __HMI_H

#include "stdio.h"
#include "usart.h"
#include "data.h"
#include "delay.h"
#include "rs485_deal.h"

void hmi_send(void);
void hmi_read(unsigned char *uartData,unsigned char len);

#endif