#ifndef __PRESSURE_GY_H
#define __PRESSURE_GY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_iic_master.h"
#include "data.h"

#define PRES_DEVICE_ADDRESS 0xEE  //�豸��ַ
#define DELAY_CONVERSION 12  //��ȡADCת��ʱ��

void Pressure_Init(void);  //��ѹ��������ʼ��
void Pressure_Convert(void);  //��ѹ���������ݶ�ȡ�ͼ���
unsigned char Pressure_Reset(void);  //��������λ

#ifdef __cplusplus
}
#endif

#endif