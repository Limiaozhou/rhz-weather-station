#include "usart.h"
#include "sys.h"
#include "formatconversion.h"	
#include "proc_sd.h"
#include "mem.h"
#include "check.h"
#include "tea.h"
#include "rtc.h"
#include "fattester.h"
#include "set_info.h"
#include "data.h"
#include "update.h"
#include "malloc.h"
#include "stmflash.h"
#include "string.h"
#include "update.h"
#include "rs485_deal.h"
#include "wdg.h"
#include "wat_fer.h"

#include "relay_board.h"

////////////////////////////////////////////////////////////////////////////////// 
//更新及配置操作
//创建：熊婕
//版本：V1.0
//修改日期:2016/11/2
//修改日期: 14:31 2018/10/11 author LK
//Copyright(C) 深圳市睿海智电子科技有限公司 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    

int binSize;
char revBin=0;
char flag=0;//flag为1时表示需要更新FLASH及SD内容
char Is_sim_send=0;//是否模拟发送
unsigned char newVer[25];
unsigned char package_num=0;//接收到的正常包号记录
unsigned int binLen;
int error_num;
unsigned char error_Res[6]={0xC7};
static float error=0;
static unsigned char bro_addr[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
extern INFO_MUL *Info_Sd;
extern INFO_485 *Sd_485;
extern unsigned char INFO_FILE[];
extern unsigned char *send_dat;
extern INFO_485 *Flash_485;
extern int send_time;
extern char ifCB;
extern EVNDAT Evndat;
extern WAT_FER wat_fer;

extern char wat_start_flag;
extern char wat_flag ;
extern char magnetic_start_flag;
extern char magnetic_flag;

extern char blender_start_flag;
extern char blender_flag;
//#define Num_485 20

/***************************************
*@DESCRIPTION: --错误码回应
*
*@Input: --key：当前错误的指令，如是0xff，则是未知指令的错误
--error：错误号，详见错误码说明
*
*@Out: --无
*
*@Return: 无
****************************************/
unsigned char * deal_error(unsigned char key, float error)
{
  *error_Res=0xC7;
  *(error_Res+1)=key;
  mem_copy(error_Res+2,(unsigned char *)&error,4);
  return error_Res;
}

/***************************************
*@DESCRIPTION: --版本比较
*
*@Input: --newV：新版本号
--oldV：旧版本号
*
*@Out: --无
*
*@Return: --0: newV==oldV
--正数：newV>oldV
--负数：newV<oldV
--100：oldV中不符合规则，需要将newV复制给oldV
--101:newV中不符合规则，需要将oldV复制给newV
****************************************/

int compare_vision(char newV[25],char oldV[25])
{
  if(newV[2]!='-'&&oldV[2]!='-')return 102;
  if(newV[2]!='-'&&oldV[2]=='-')return 101;
  if(newV[2]=='-'&&oldV[2]!='-')return 100;
  return strcmp(newV+4,oldV+4);
}

/***************************************
*@DESCRIPTION: --待更新的bin文件包是否接收完全
*
*@Input: --无
*
*@Out: --无
*
*@Return: --void
****************************************/	
void Is_Rev_complete()
{   
  unsigned char len;
  if(revBin==1 && binLen==binSize)
  {
    mf_close();
    len=Get_Bin_len(newVer);
    if(len==binSize)
    {
      update_sd_485(*Info_Sd);
      update_flash();
    }
    else
      package_send(deal_error(0xF1,25),6,0,0);
  }
  
}

/***************************************
*@DESCRIPTION: --更新FLASH中的数据
*
*@Input: --无
*
*@Out: --无
*
*@Return: --void
****************************************/	
void update_flash()
{
  STMFLASH_Write(FLASH_INFO_MUL,(u16*)Info_Sd->Addr,(sizeof(INFO_MUL)-50)/2);
  STMFLASH_Write(FLASH_INFO_VER,(u16*)Info_Sd,25);
}   

/***************************************
*@DESCRIPTION: --更新FLASH中的数据
*
*@Input: --无
*
*@Out: --无
*
*@Return: --void
****************************************/	
void update_flash_485()
{
  //  STMFLASH_Write(FLASH_INFO_MUL,(u16*)Info_Sd,sizeof(INFO_MUL)/2);
  STMFLASH_Write(FLASH_INFO_485,(u16*)Sd_485,sizeof(INFO_485)*Num_485/2);
}   

/***************************************
*@DESCRIPTION: --更新SD卡中的数据
*
*@Input: --无
*
*@Out: --无
*
*@Return: --void
****************************************/	
void update_sd_485(INFO_MUL sourse_info)
{
  unsigned char *tmp,len=0,i=0;
  
  //  len=161+sizeof(INFO_485)*Num_485;
  tmp=(unsigned char *)mymalloc(SRAMIN,300);
  mf_open(INFO_FILE,FA_READ|FA_WRITE|FA_CREATE_NEW);
  
  *tmp=0xF5;len++;
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Addr,8);
  len+=8;
  
  *(tmp+len++)=0xF9;  
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Soft_Ver,25);
  len+=25;
  
  *(tmp+len++)=0xFA;  
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Hard_Ver,25);
  len+=25;
  
  *(tmp+len++)=0xF6;
  *(tmp+len++)=strlen(sourse_info.Apne_Name);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Apne_Name,strlen(sourse_info.Apne_Name));
  len+=strlen(sourse_info.Apne_Name);
  *(tmp+len++)=strlen(sourse_info.Apne_Passwd);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Apne_Passwd,strlen(sourse_info.Apne_Passwd));
  len+=strlen(sourse_info.Apne_Passwd);
  
  *(tmp+len++)=0xF7;
  *(tmp+len++)=strlen(sourse_info.Server_Ip);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Server_Ip,strlen(sourse_info.Server_Ip));
  len+=strlen(sourse_info.Server_Ip);
  *(tmp+len++)=strlen(sourse_info.Server_Port);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Server_Port,strlen(sourse_info.Server_Port));
  len+=strlen(sourse_info.Server_Port);
  
  *(tmp+len++)=0xF8;
  *(tmp+len++)=strlen(sourse_info.Encr_Auth);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Encr_Auth,strlen(sourse_info.Encr_Auth));
  len+=strlen(sourse_info.Encr_Auth);
  *(tmp+len++)=strlen(sourse_info.Encr_Encry);
  mem_copy(tmp+len,(unsigned char*)&sourse_info.Encr_Encry,strlen(sourse_info.Encr_Encry));
  len+=strlen(sourse_info.Encr_Encry);
  
  *(tmp+len++)=0xFD;
  mem_copy(tmp+len,(unsigned char*)&(sourse_info.Send_Fre),4);
  len+=4;
  
  *(tmp+len++)=0xFC;
  for(i=0;i<Num_485;i++)
  {
    if((unsigned char*)(Flash_485+i)->addr_485==0)
      break;
  }
  *(tmp+len++)=i;
  mem_copy(tmp+len,(unsigned char*)Flash_485,sizeof(INFO_485)*i);
  len+=sizeof(INFO_485)*i;
  //错误
  package_send(tmp, len,0,0);
  
  len=len+18-len%4;
  mf_write(send_dat,len+16-(len)%16);
  mf_close();
  myfree(SRAMIN,tmp);
  tmp=NULL;
}   


/***************************************
*@DESCRIPTION: --硬件地址处理
*
*@Input: --pdat：收到的硬件地址
*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_addr(unsigned char *pdat)
{
  mem_copy(Info_Sd->Addr,pdat,8);
}

/***************************************
*@DESCRIPTION: --WIFI用户名密码处理
*
*@Input: --pdat：收到的WIFI用户名密码

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_wifiusers(unsigned char *pdat)
{
  mem_clear(Info_Sd->Apne_Name,20);
  mem_clear(Info_Sd->Apne_Passwd,20);
  mem_copy(Info_Sd->Apne_Name,pdat+1,*pdat);
  mem_copy(Info_Sd->Apne_Passwd,pdat + *pdat + 2,*(pdat + *pdat + 1));
}

/***************************************
*@DESCRIPTION: --服务器IP及端口号配置
*
*@Input: --pdat：收到的服务器相关信息

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_servers(unsigned char *pdat)
{
  mem_clear(Info_Sd->Server_Ip,20);
  mem_clear(Info_Sd->Server_Port,6);
  mem_copy(Info_Sd->Server_Ip,pdat+1,*pdat);
  mem_copy(Info_Sd->Server_Port,pdat + *pdat + 2,*(pdat + *pdat + 1));
}

/***************************************
*@DESCRIPTION: --WIFI加密方式配置
*
*@Input: --pdat：收到的WIFI加密方式

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_wifiencrypt(unsigned char *pdat)
{
  mem_clear(Info_Sd->Encr_Auth,10);
  mem_clear(Info_Sd->Encr_Encry,10);
  mem_copy(Info_Sd->Encr_Auth,pdat+1,*pdat);
  mem_copy(Info_Sd->Encr_Encry,pdat + *pdat + 2,*(pdat + *pdat + 1));
}

/***************************************
*@DESCRIPTION: --软件版本号配置
*
*@Input: --pdat：收到的软件版本号

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_softver(unsigned char *pdat)
{
  mem_clear(Info_Sd->Soft_Ver,25);
  mem_copy(Info_Sd->Soft_Ver,pdat,25);

}

/***************************************
*@DESCRIPTION: --硬件版本号配置
*
*@Input: --pdat：收到的硬件版本号

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_hardver(unsigned char *pdat)
{
  mem_clear(Info_Sd->Hard_Ver,25);
  mem_copy(Info_Sd->Hard_Ver,pdat,25);
}

/***************************************
*@DESCRIPTION: --485编号填充
*
*@Input: --pdat：收到的硬件版本号

*
*@Out: --无
*
*@Return: --void
****************************************/	
void deal_485(unsigned char *pdat)
{
  mem_clear((unsigned char*)Sd_485,Num_485*sizeof(INFO_485));
  mem_copy((unsigned char*)Sd_485,pdat+1,*(pdat)*sizeof(INFO_485));

}

/***************************************
*@DESCRIPTION: --发送频率配置
*
*@Input: --pdat：收到的发送频率

*
*@Out: --无
*
*@Return: --void
****************************************/	
void set_sendfre(unsigned char *pdat)
{
  mem_clear((unsigned char*)&Info_Sd->Send_Fre,4);
  mem_copy((unsigned char*)&Info_Sd->Send_Fre,pdat,4);
  send_time=chartofloat(pdat)*5;

}

/***************************************
*@DESCRIPTION: --获取是否模拟数据发送
*
*@Input: --无

*
*@Out: --无
*
*@Return: --char  1表示当前使用模拟发送
****************************************/	
char get_issimsend()
{
  return Is_sim_send;
}

/***************************************
*@DESCRIPTION: --获取软件版本号
*
*@Input: --无

*
*@Out: --无
*
*@Return: --char  *  返回的是25个字节的软件版本号
****************************************/	
unsigned char *get_softver()
{
  return Info_Sd->Soft_Ver;
}


/***************************************
*@DESCRIPTION: --串口处理，包括版本更新及配置部分
*
*@Input: --uart_data：串口接收数据
--len      ：串口接收长度
--flag     :标志位，为1时表示是串口处理，为0时表示SD卡数据获取
*
*@Out: --无
*       
*@Return: --void
****************************************/	
    /*向日葵电机55 fe fe 03 02 f9 25 close
55 fe fe 03 01 b9 24 open
55 FE FE 03 03 38 E5 stop*/
unsigned char yang_on[7]= {0X55,0XFE,0XFE,0X03,0X01,0XB9,0X24};

unsigned char yang_off[7]= {0X55,0XFE,0XFE,0X03,0X02,0XF9,0X25};
//添加写入编号部分
extern char wifi_flag;

/*
wifi2接收服务器返回的指令，并处理分析，接收帧头（2字节）、长度（1）、加密标志（1）、硬件编号（8），共12字节头判断
15:14 2018/10/12
LK
*/
void Deal_Uart(unsigned char *uart_data,int len,int flag1)
{
  int i=0;
  int j=0;
  int flag =0;
//  float shadedat = 0;
//  unsigned char addr[8] = { 0x4f,0x45,0x41,0x00,0x00,0x0E,0x00,0x00 };//小棚电机独立控制
//  unsigned char rece[UART_RX_LEN]={0};
//  char a=0,num;
  unsigned char dat[4]={0};
//  USART_Puts(UART4, "\n", 1);//debug
//               for(int t=0;t<5;t++)
//    {
//                UART4->DR= 0x55;
//                while((UART4->SR&0X40)==0);//等待发送结束
//            }
//            USART_Puts(UART4, "\n", 1);//debug
    
  while(1)
  {
    
    //首个字节为0，退出循环
    IWDG_Feed();
//    if((*(uart_data+i)==0)) 
//      break;
    if(i>=len)
      break;
    
    if(*(uart_data+i)==0x55 && *(uart_data+i+1)==0xAA &&uart_data[2+i]>=2)//帧头校验，长度校验大于2
    {
      /*收到服务器发来打开遮阳，发送485打开命令，*/
 //     FromUartToSD2HAO(uart_data,len);
//       ifCB=0;
       IWDG_Feed();
//       USART_Puts(UART4, (uart_data+i),1);//debug
//       UART4->DR= i;
//           while((UART4->SR&0X40)==0);//等待发送结束
//       USART_Puts(UART4, &uart_data[4+i],uart_data[2+i]-3);//debug
       if(Check_CheckCRC16(&uart_data[3+i],uart_data[2+i]-2))  //CRC校验
       {
         if(*(uart_data+3+i)>=100)  //buf[3]为是否加密标志，大于100为加密，需解密
         {
           btea_decrpyt(&uart_data[4+i],uart_data[2+i]-3);  //数据解密
         }
          
         USART_Puts(UART4, &uart_data[i], uart_data[2+i]+3);//debug
         j=*(uart_data+2+i)+i-11;
//         if(((memcmp(&uart_data[i+4], addr, 8)) || (memcmp(&uart_data[i+4],bro_addr,8))))
//         {
//           //如果是后台传来的数据，需判断硬件地址是否一致
//           if(flag)
//           {     
//             error=28;
//             package_send(deal_error(0xFF,error),6,0,0);
//             break;
//           }
//         }
//         USART_Puts(UART4, (unsigned char *)uart_data+i+12, 7);//debug
//         while(1)
//         {
           if(i>=j)  //数据长度小于11直接退出
             break;
          
           switch(*(uart_data+i+12))  //数据位帧头判断
           {
               case 0xB3: 
//                   USART_Puts(UART4, (unsigned char *)uart_data+i+12, 7);//debug
                  if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x01))  //风机
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.ventilate = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x03))  //卷膜
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.juanmo = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x05))  //水帘
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.wat4 = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x07))  //内，遮阳1
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.yang = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x09))  //外，遮阳2
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.yang1 = chartofloat(dat);
                  }
                  
                  send();	//发送数据到服务器
                  
                  relay_board_control();  //发送控制继电器板命令
                  
//                  if((int)(shadedat) == 1)
//                  {
////                        USART_Puts(UART4, (unsigned char *)uart_data+i+12, 7);//debug
//                    USART_Puts(USART3, yang_on, 7);//debug
//                    USART_Puts(UART4, yang_on, 5);//debug
//                    delay_ms(100);
//                    
//                    
//                  }
//                  if((int)(shadedat) == 0)
//                  {
//          
//                    USART_Puts(USART3, yang_off, 7);//debug
//                    USART_Puts(UART4, yang_off, 5);//debug
//                    delay_ms(100);
//                    send();	//发送数据
//                    
//                  }
//              
                    break;
            
                 default:
                    break;
             }
       }
    }
      i++;
  }//end while
  // if(i&&flag1&&flag) 
  // {
  //   update_sd_485(*Info_Sd);
  //   update_flash();
  //   update_flash_485();
  //   while(1);
  // }
}
//end Deal_Uart