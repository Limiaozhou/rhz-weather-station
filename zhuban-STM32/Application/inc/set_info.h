
////////////////////////////////////////////////////////////////////////////////// 
//配置操作
//创建：熊婕
//版本：V1.0
//修改日期:2016/11/2
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    

#ifndef _SET_INFO_H_
#define _SET_INFO_H_

/***************************************
*@DESCRIPTION: --参数设置结构体，包括
  硬件编号
  硬件版本号
  软件版本号
  服务器IP地址
  服务器端口
  WIFI验证方式
  WIFI加密算法
  WIFI连接路由器名字
  WIFI连接路由器密码
  发送频率
  485接入地址
*
****************************************/
typedef struct 
{
  unsigned char  Soft_Ver[25]; //软件版本
  unsigned char  Hard_Ver[25]; //硬件版本
  unsigned char  Addr[8]; //硬件编号
  unsigned char  Server_Ip[20]; //服务器Ip地址
  unsigned char  Server_Port[6]; //服务器端口
  unsigned char  Encr_Auth[10]; //验证
  unsigned char  Encr_Encry[10]; //加密算法
  unsigned char  Apne_Name[20]; //
  unsigned char  Apne_Passwd[20]; //
  float  Send_Fre;//发送频率设置
}INFO_MUL;

typedef struct
{
  unsigned char addr_485;//485地址
  unsigned char regi_485[2];//485寄存器
}INFO_485;


extern char* get_Soft_ver();
extern char* get_Hart_ver();
extern INFO_MUL *Info_Flash;
#endif