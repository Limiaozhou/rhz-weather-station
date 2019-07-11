#include "proc_sd.h"
#include "ff.h"
#include "fattester.h"
#include "mem.h"
#include "data.h"
#include "rtc.h"
#include "rs485_deal.h"

#define u8 unsigned char 
#define u32 unsigned int
extern unsigned char *sensor_rtc; //��RTC�Ĵ���������
extern unsigned char DATA_FILE[];
extern unsigned char READ_FILE[];
extern unsigned char *send_dat;
extern ST_ITEM *st_item;
extern Send_485 send_485;


/***************************************
*@DESCRIPTION: --���յ���BIN�ļ�������SD����
*
*@Input: --name��Ҫ������ļ�����25���ֽڳ�
*
*@Out: --��
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
*@DESCRIPTION: --��ȡָ��bin�ļ���С
*
*@Input: --name������ȡbin�ļ���25���ֽڳ�
*
*@Out: --��
*
*@Return: --bin�ļ���С
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
*@DESCRIPTION: --RTCʱ��ת����HEX��ʽ
*
*@Input: --time��RTC��ʽʱ��
          *pdat��ת�����HEX��ʽʱ��
*
*@Return: --��
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
*@DESCRIPTION: --RTC��������ڴ�����������
*
*  
*
*@Input: --
*
*@Out: --��
*
*@Return: --��
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
*@DESCRIPTION: --�������ݱ�����SD����
*
*  ������ʱ���øú��� ������ļ����̶�ΪDATA_FILE
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
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
*@DESCRIPTION: --ȡ��SD�����ݲ�ͨ�����ڷ��͵���̨
*
*  ������ʱ���øú��� ȡ���ݵ��ļ����̶�ΪDATA_FILE
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
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