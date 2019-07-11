#ifndef __PRESSURE_GY_H
#define __PRESSURE_GY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_iic_master.h"
#include "data.h"

#define PRES_DEVICE_ADDRESS 0xEE  //设备地址
#define DELAY_CONVERSION 12  //读取ADC转化时间

void Pressure_Init(void);  //气压传感器初始化
void Pressure_Convert(void);  //气压传感器数据读取和计算
unsigned char Pressure_Reset(void);  //传感器复位

#ifdef __cplusplus
}
#endif

#endif