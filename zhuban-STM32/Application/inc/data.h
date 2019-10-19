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
  unsigned char len; //长度，数据字节数，6 * n

//  ST_ITEM shade;
//    ST_ITEM co2;  
//    ST_ITEM pm25; 
//    ST_ITEM ultvio;//紫外线
//    ST_ITEM temp; 
//    ST_ITEM light; 
//    ST_ITEM humi;
//    ST_ITEM tvoc;
//    ST_ITEM fx;  //风向
//    ST_ITEM fs;  //风速
//    ST_ITEM so2;  //so2
//    ST_ITEM pressure;  //气压
//    ST_ITEM yuxue;  //是否有雨雪
    
    ST_ITEM ventilate;  //风机
//    ST_ITEM juanmo;  //卷膜
//    ST_ITEM wat4;  //水帘
//    ST_ITEM yang;  //内，遮阳1
//    ST_ITEM yang1;  //外，遮阳2
    ST_ITEM UA;
    ST_ITEM UB;
    ST_ITEM UC;
    ST_ITEM IA;
    ST_ITEM IB;
    ST_ITEM IC;
    ST_ITEM IL;
    ST_ITEM TA;
    ST_ITEM TB;
    ST_ITEM TC;
    ST_ITEM TN;
    ST_ITEM TE;
}DATA;
//要发送的数据结构体
typedef struct
{
	short int head;  //2，帧头
	unsigned char len;   //1，长度，后面字节数，不包括本字节
	char ver;   //1，数据是否加密标志位
	unsigned char addr[8]; // device code,8，设备地址
	DATA data;     //传感器数据,2 + 6 * n
	char fill[2];  //2
	short int crc16;     //crc16,2，从ver开始校验
}ST_EVNBALEDAT;
extern ST_EVNBALEDAT *psenddat;
/******************@DESCRIPTION: -- 检测环境参数类别名称*************/
typedef struct
{
//  float shade ; //shade 遮阳
//  float co2; //
//  float pm25;//PM2.5
//  float ultvio;//紫外线
//  float light;//光照
//  float temp;//温度
//  float humi;//湿度
//  float tvoc;//tvoc
//  float fx;//风向
//  float fs;//风速
//  float so2;  //so2
//  float pressure;  //气压
//  float yuxue;  //是否有雨雪
  float UA;  //A相电压
  float UB;
  float UC;
  float IA;  //A相电流
  float IB;
  float IC;
  float IL;  //漏电流
  float TA;  //A相温度
  float TB;
  float TC;
  float TN;
  float TE;  //环境温度
}SENSORTYPE;
extern void send();
extern void package_send(unsigned char *pdat, unsigned char len, unsigned char flag,unsigned char flag1);
extern SENSORTYPE sensor;
#endif
