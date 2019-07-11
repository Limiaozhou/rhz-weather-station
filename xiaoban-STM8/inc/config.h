#ifndef _CONFIG_H_
#define _CONFIG_H_

#define         CONFIGCOUNT     20     //定标数量

typedef  struct DBPPMConfig
{
   float Voltage;
   float dbppm;
}ST_DBPPMConfig;

typedef struct Config
{
   char addr[8];
   char type;     //0 没有配置 1配置完成
   char count;    //已经定标数量
   ST_DBPPMConfig group[CONFIGCOUNT];
   int crc;
}ST_Config;

#define         DEVTYPE     0x41     //声音

//获取配置参数
extern void Config_Read(void);
//发送配置参数
extern void Config_Send(void);
//添加校准点
extern void Config_Add(float dat);
//计算实际输出
extern void Config_Deal(void);

extern void Config_Save(void);

extern void Config_DealADCMaxAve(void);

extern void Config_Init(void);

#endif


