#ifndef _DATA_H_
#define _DATA_H_

/******************@DESCRIPTION: -- 传感数据结构体 *************/

typedef struct
{
  unsigned char key[2]; //
  unsigned char dat[4];
}ST_ITEM;

//检测参数编号和数据
//typedef struct
//{
//	unsigned char num[2]; //服务器上用的地址
//	unsigned char dat[4];
//}ST_ITEM;
/**********@DESCRIPTION: -- 检测参数编号和数据，cmd指令，长度，cmd是D0要加密数据上传，历史遗留，没后台不知道具体细节*************/
typedef struct
{
  unsigned char cmd; //
  unsigned char len; //data  关键字

//  ST_ITEM shade;
//    ST_ITEM co2;  
//    ST_ITEM pm25; 
//    ST_ITEM ultvio;//紫外线
    ST_ITEM temp; 
    ST_ITEM light; 
    ST_ITEM humi;
//    ST_ITEM tvoc;
    ST_ITEM fx;  //风向
    ST_ITEM fs;  //风速
//    ST_ITEM so2;  //so2
//    ST_ITEM pressure;  //气压
    ST_ITEM yuxue;  //是否有雨雪
}DATA;
//要发送的数据结构体
typedef struct
{
	short int head;  //
	unsigned char len;   //
	char ver;   //
	unsigned char addr[8]; // device code
	DATA data;     //传感器数据
	char fill[2];
	short int crc16;     //crc16
}ST_EVNBALEDAT;
extern ST_EVNBALEDAT *psenddat;
/******************@DESCRIPTION: -- 检测环境参数类别名称*************/
typedef struct
{
//  float shade ; //shade 遮阳
  float co2; //
  float pm25;//PM2.5
  float ultvio;//紫外线
  float light;//光照
  float temp;//温度
  float humi;//湿度
  float tvoc;//tvoc
  float fx;//风向
  float fs;//风速
//  float so2;  //so2
  float pressure;  //气压
  float yuxue;  //是否有雨雪
}SENSORTYPE;
extern void send();
extern void package_send(unsigned char *pdat, unsigned char len, unsigned char flag,unsigned char flag1);
extern SENSORTYPE sensor;
#endif
