#ifndef _SPI_H_
#define _SPI_H_

#include"typedef.h"

typedef  struct PMDATA
{
   //double Temperature;
   //double Pressure;
   
   double pm1;
   double pm2_5;
   double pm10;
   
   //double RollMean_PM1;
   //double RollMean_PM2_5;
   //double RollMean_PM10;
   
}ST_PM;

/******************@DESCRIPTION: --初始化 *************/	
extern void SPI_Init(void);

/******************@DESCRIPTION: -- OPC-N2开关电源风扇 *************/	
extern BOOL PM_Control(uchar dat);

/******************@DESCRIPTION: -- OPC-N2连续测量数据  需要先开启电源*************/	
extern BOOL Get_ContinuousPM(ST_PM *pm);

/******************@DESCRIPTION: -- OPC-N2读取测量数据 *************/	
extern BOOL Get_PM(ST_PM *pm);

/******************@DESCRIPTION: -- OPC-N2  功能测试 *************/	
extern void PM_test(void);


#endif
