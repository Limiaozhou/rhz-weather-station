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
#define ULTRA_485       0x000A  //������
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
#define  yuxue_485 0x18  //�Ƿ�����ѩ
#define  PDX_485 194  //�����

typedef struct
{
    float humi;
    float temp_in;//����
    float humi_in;//��ʪ��
    float AirConSwi;//�յ����� 1��0��
    float DehuSwi;//��ʪ���� 1��0��

}EVNDAT;
typedef struct 
{
  unsigned char key[2];    //�ؼ���
  unsigned char addr;   //��ַ
  unsigned char instant_data[4];        //ֵ
}RTU_data;

typedef struct 
{
  unsigned char buf[100];  //������
  unsigned char count;     //���������ݴ�С
  unsigned char index;     //��������ַ
  unsigned char num;       //������С
}RxDat;

typedef struct 
{
  unsigned char addr;   //Ҫ���ĵ�ַ
  unsigned char count;  //Ҫ���ĸ�������һ��Ҫ4���ֽ�
  unsigned char len;    //�ܳ��� ��02 04 08����ʱcountΪ3��lenΪ4
  unsigned char Reg_addr[20];//��Ӧ�ļĴ���
}Form_485;

typedef struct 
{
  unsigned char num;    //Ҫ���ĵ�ַ�ĸ���
  unsigned char cnt;    //Ҫ���ļĴ������ܸ���
  Form_485 format_485[Num_485];
}Send_485;

typedef struct 
{
  unsigned char cmd[8];//���������͵�485����
  unsigned char addr[6];
//  unsigned char type;
}Cmd_send;

extern float LIGHT_DATA ;
extern WAT_FER wat_fer;  //ˮ�ʵȿ����豸�ṹ��
extern void Generate_485_addr();
//extern void send_read_RTU(unsigned char *send,unsigned char Oper_code, unsigned char addr,unsigned char len);
extern void set_RTU_addr(unsigned char *send, unsigned char oldAddr, unsigned char newAddr);
extern void send_Cmd();
extern void get_RTU_data(unsigned char *uartData,unsigned char len);
extern void RTU_RX_Data(unsigned char *pdat,unsigned char len);
extern void send_read_RTU(unsigned char addr,  unsigned char Functioncode, unsigned char Reg_addr, unsigned char len, unsigned char *send );
#endif