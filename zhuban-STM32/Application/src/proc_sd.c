#include "proc_sd.h"
#include "ff.h"
#include "fattester.h"
#include "mem.h"
#include "data.h"
#include "rtc.h"
#include "rs485_deal.h"

#define u8 unsigned char 
#define u32 unsigned int
extern unsigned char *sensor_rtc; //带RTC的传感器数据
extern unsigned char DATA_FILE[];
extern unsigned char READ_FILE[];
extern unsigned char *send_dat;
extern ST_ITEM *st_item;
extern Send_485 send_485;


/***************************************
*@DESCRIPTION: --接收到的BIN文件保存于SD卡中
*
*@Input: --name：要保存的文件名，25个字节长
*
*@Out: --无
*
*@Return: --
****************************************/
int BinToSD(unsigned char *name)
{
  u8 SDtatus;
  int len=0;
  u8 tmp[30]="0://  ";
  mem_copy(tmp+4,name,25);
  tmp[29]='\0';
  
  SDtatus=mf_open(tmp,FA_WRITE);
  if(SDtatus== FR_NO_FILE) 
  {
    SDtatus=0;
    SDtatus = mf_open(tmp,FA_WRITE|FA_CREATE_NEW);
  }
  else
  {
    len=mf_size();
    mf_close();
    mf_unlink(tmp);
    SDtatus=0;
    SDtatus = mf_open(tmp,FA_WRITE|FA_CREATE_NEW);
  }
  return SDtatus;
}


/***************************************
*@DESCRIPTION: --获取指定bin文件大小
*
*@Input: --name：待获取bin文件，25个字节长
*
*@Out: --无
*
*@Return: --bin文件大小
****************************************/
int Get_Bin_len(unsigned char *name)
{
  u8 SDtatus;
  int len=0;
  u8 tmp[30]="0://  ";
  mem_copy(tmp+4,name,25);
  tmp[29]='\0';
  
  SDtatus=mf_open(tmp,FA_READ);
  if(SDtatus== FR_OK) 
  {
    len=mf_size();
    mf_close();
  }
  return len;
}
    

/***************************************
*@DESCRIPTION: --RTC时间转换成HEX格式
*
*@Input: --time：RTC格式时间
          *pdat：转换后的HEX格式时间
*
*@Return: --无
****************************************/	
/**/
void rtc_To_hex(_calendar_obj time,unsigned char *pdat)
{
  RTC_Get();
  *pdat++ = (unsigned short)(time.w_year>>8);
  *pdat++ = (unsigned char)(time.w_year);
  *pdat++ = (unsigned char)(time.w_month);
  *pdat++ = (unsigned char)(time.w_date);
  *pdat++ = (unsigned char)(time.hour);
  *pdat++ = (unsigned char)(time.min);
  *pdat++ = (unsigned char)(time.sec);
}


/***************************************
*@DESCRIPTION: --RTC数据填充于传感器数据中
*
*  
*
*@Input: --
*
*@Out: --无
*
*@Return: --无
****************************************/
unsigned char *packageRtcDat()
{
  mem_copy(sensor_rtc,(unsigned char*)st_item,sizeof(ST_ITEM)*(send_485.cnt+11));
  *(sensor_rtc+sizeof(ST_ITEM)*(send_485.cnt+11))=0x51;
  rtc_To_hex(calendar,sensor_rtc+sizeof(ST_ITEM)*(send_485.cnt+11)+1);
  mem_clear(sensor_rtc+sizeof(ST_ITEM)*(send_485.cnt+11)+8,16-(sizeof(ST_ITEM)*(send_485.cnt+11)+8)%16);
  return sensor_rtc;
}

/***************************************
*@DESCRIPTION: --串口数据保存在SD卡中
*
*  当离线时调用该函数 保存的文件名固定为DATA_FILE
*
*@Input: --无
*
*@Out: --无
*
*@Return: --无
****************************************/
void FromUartToSD(void)
{
  u8 SDtatus;
  u32 len;
// package_send(packageRtcDat(),sizeof(ST_ITEM)*send_485.cnt+8-sizeof(ST_ITEM)*send_485.cnt)%4,1);
  package_send(packageRtcDat(),sizeof(ST_ITEM)*(send_485.cnt+11),1,1);
//  Evn_PackageRtcDat(psenddat,prtcdat);
  SDtatus=mf_open(DATA_FILE,FA_WRITE);
  if(SDtatus== FR_NO_FILE) 
  {
    SDtatus=0;
    SDtatus = mf_open(DATA_FILE,FA_WRITE|FA_READ|FA_CREATE_NEW);
  }
  len=mf_size();
  mf_lseek(len);
  mf_write(send_dat,sizeof(ST_ITEM)*(send_485.cnt+11)+24+(16-(sizeof(ST_ITEM)*(send_485.cnt+11)+24)%16));
  mf_close();
}

void FromUartToSD2HAO(unsigned char *uart_data,int len)
{
  /*
  u8 SDtatus;
 int a;
// package_send(packageRtcDat(),sizeof(ST_ITEM)*send_485.cnt+8-sizeof(ST_ITEM)*send_485.cnt)%4,1);
//  package_send(packageRtcDat(),sizeof(ST_ITEM)*(send_485.cnt+11),1,1);
//  Evn_PackageRtcDat(psenddat,prtcdat);
  rtc_To_hex(calendar,uart_data+len);
  SDtatus=mf_open(READ_FILE,FA_WRITE);
  if(SDtatus== FR_NO_FILE) 
  {
    SDtatus=0;
    SDtatus = mf_open(READ_FILE,FA_WRITE|FA_READ|FA_CREATE_NEW);
  }
  a=mf_size();
  mf_lseek(a);
  mf_write(uart_data,len + 7 );
  mf_close();
  */
}


/***************************************
*@DESCRIPTION: --取出SD卡数据并通过串口发送到后台
*
*  当在线时调用该函数 取数据的文件名固定为DATA_FILE
*
*@Input: --无
*
*@Out: --无
*
*@Return: --无
****************************************/
void FromSDToUart(void)
{
  u8 SDtatus;
  u32 len;
  SDtatus=mf_open(DATA_FILE,FA_READ);
  if(SDtatus== FR_NO_FILE) 
  {
    SDtatus=0;
    SDtatus = mf_open(DATA_FILE,FA_WRITE|FA_READ|FA_CREATE_NEW);
  }
  mf_lseek(0);
  len=mf_size();
  mf_read(len);
  mf_close();
  mf_unlink(DATA_FILE);
}