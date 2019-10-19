#ifndef _RS485_DEAL_H_
#define _RS485_DEAL_H_

#include "update.h"
#include "wat_fer.h"

/*
#define PM1_485                 0x02
#define PM25_485                0x04
#define PM10_485                0x06

#define RAIN_485                0x12

#define SWITCH_AIR_485          0x14
#define SWTICH_DEHUMI_485       0x16

#define CO2_485                 0x14
#define TVOC_485                0x1C
#define TVOCCO2_485             0x1E
#define TEMP_OUTD_485
#define HUMI_OUTD_485
#define PRESS_485               0X06
#define TEMP_485                0x02
#define HUM_485                 0x04

#define NO_485                 0x14
#define HC_485                0x1C
#define NH3_485             0x1E
#define H2S_485
#define HCHO_485

#define CO_485                  0x0A
#define SO2_485                 0x0C
#define O3_485                  0x0E
#define NO2_485                 0x10

#define FS_485                  0x08
#define FX_485                  0x0A
#define Light_485               0x2E
#define Noise_485               0x2A
#define Radia_485              0x30

#define CO_VOL_485
#define SO2_VOL_485
#define O3_VOL_485
#define NO2_VOL_485

#define ZERO_VOL_485
#define SLOPE_485

#define RHZ_OEair_PM 0x03
#define RHZ_OEair_MSensor 0x02
#define RHZ_OEair_MMsensor 0x01
*/

//#define FX_485          0x64
//#define FS_485          0x66
//#define LIGHT_485       0x0065
//#define VOICE_485       0x0002
//#define ULTRA_485       0x006C
//#define RADIA_485       0x006E

#define FX_485          0x0004
#define FS_485          0x0002
//#define LIGHT_485       0x0006
//#define VOICE_485       0x0008
#define ULTRA_485       0x000A  //紫外线
//#define RADIA_485       0x000C
//#define SOILPH_485       0x000E
//#define SOILT_485       0x0010
//#define SOILH_485       0x0012
//#define SOILDIAN_485       0x0014
//#define SOILYAN_485       0x0016
#define  CO2_485 0x7A
#define  PM25_485 0x68
#define Ultraviolet 0x0A
#define temp_485  0x80
#define humi_485 0x82
#define light_485 0x06

#define  SO2_485 0x6E
#define  yuxue_485 0x18  //是否有雨雪
#define  PDX_485 194  //配电箱

typedef struct
{
    float humi;
    float temp_in;//内温
    float humi_in;//内湿度
    float AirConSwi;//空调开关 1开0关
    float DehuSwi;//除湿开关 1开0关

}EVNDAT;
typedef struct 
{
  unsigned char key[2];    //关键字
  unsigned char addr;   //地址
  unsigned char instant_data[4];        //值
}RTU_data;

typedef struct 
{
  unsigned char buf[100];  //缓存区
  unsigned char count;     //缓存区数据大小
  unsigned char index;     //缓存存入地址
  unsigned char num;       //期望大小
}RxDat;

typedef struct 
{
  unsigned char addr;   //要读的地址
  unsigned char count;  //要读的个数，读一个要4个字节
  unsigned char len;    //总长度 如02 04 08，此时count为3，len为4
  unsigned char Reg_addr[20];//对应的寄存器
}Form_485;

typedef struct 
{
  unsigned char num;    //要读的地址的个数
  unsigned char cnt;    //要读的寄存器的总个数
  Form_485 format_485[Num_485];
}Send_485;

typedef struct 
{
  unsigned char cmd[8];//用来传发送的485命令
  unsigned char addr[6];
//  unsigned char type;
}Cmd_send;

extern float LIGHT_DATA ;
extern WAT_FER wat_fer;  //水肥等控制设备结构体
extern void Generate_485_addr();
//extern void send_read_RTU(unsigned char *send,unsigned char Oper_code, unsigned char addr,unsigned char len);
extern void set_RTU_addr(unsigned char *send, unsigned char oldAddr, unsigned char newAddr);
extern void send_Cmd();
extern void get_RTU_data(unsigned char *uartData,unsigned char len);
extern void RTU_RX_Data(unsigned char *pdat,unsigned char len);
extern void send_read_RTU(unsigned char addr,  unsigned char Functioncode, unsigned char Reg_addr, unsigned char len, unsigned char *send );
#endif