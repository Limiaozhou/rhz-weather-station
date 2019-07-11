#ifndef _CONFIG_H_
#define _CONFIG_H_

//获取配置参数
extern void Config_Read(void);
//发送配置参数
extern void Config_Send(void);
//添加校准点
extern void Config_Add(float dat);
//计算实际输出
extern void Config_Deal(void);

extern void Config_DealADCMaxAve(void);

#endif


