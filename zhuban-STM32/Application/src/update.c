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
//���¼����ò���
//���������
//�汾��V1.0
//�޸�����:2016/11/2
//�޸�����: 14:31 2018/10/11 author LK
//Copyright(C) ��������ǵ��ӿƼ����޹�˾ 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    

int binSize;
char revBin=0;
char flag=0;//flagΪ1ʱ��ʾ��Ҫ����FLASH��SD����
char Is_sim_send=0;//�Ƿ�ģ�ⷢ��
unsigned char newVer[25];
unsigned char package_num=0;//���յ����������ż�¼
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
*@DESCRIPTION: --�������Ӧ
*
*@Input: --key����ǰ�����ָ�����0xff������δָ֪��Ĵ���
--error������ţ����������˵��
*
*@Out: --��
*
*@Return: ��
****************************************/
unsigned char * deal_error(unsigned char key, float error)
{
  *error_Res=0xC7;
  *(error_Res+1)=key;
  mem_copy(error_Res+2,(unsigned char *)&error,4);
  return error_Res;
}

/***************************************
*@DESCRIPTION: --�汾�Ƚ�
*
*@Input: --newV���°汾��
--oldV���ɰ汾��
*
*@Out: --��
*
*@Return: --0: newV==oldV
--������newV>oldV
--������newV<oldV
--100��oldV�в����Ϲ�����Ҫ��newV���Ƹ�oldV
--101:newV�в����Ϲ�����Ҫ��oldV���Ƹ�newV
****************************************/

int compare_vision(char newV[25],char oldV[25])
{
  if(newV[2]!='-'&&oldV[2]!='-')return 102;
  if(newV[2]!='-'&&oldV[2]=='-')return 101;
  if(newV[2]=='-'&&oldV[2]!='-')return 100;
  return strcmp(newV+4,oldV+4);
}

/***************************************
*@DESCRIPTION: --�����µ�bin�ļ����Ƿ������ȫ
*
*@Input: --��
*
*@Out: --��
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
*@DESCRIPTION: --����FLASH�е�����
*
*@Input: --��
*
*@Out: --��
*
*@Return: --void
****************************************/	
void update_flash()
{
  STMFLASH_Write(FLASH_INFO_MUL,(u16*)Info_Sd->Addr,(sizeof(INFO_MUL)-50)/2);
  STMFLASH_Write(FLASH_INFO_VER,(u16*)Info_Sd,25);
}   

/***************************************
*@DESCRIPTION: --����FLASH�е�����
*
*@Input: --��
*
*@Out: --��
*
*@Return: --void
****************************************/	
void update_flash_485()
{
  //  STMFLASH_Write(FLASH_INFO_MUL,(u16*)Info_Sd,sizeof(INFO_MUL)/2);
  STMFLASH_Write(FLASH_INFO_485,(u16*)Sd_485,sizeof(INFO_485)*Num_485/2);
}   

/***************************************
*@DESCRIPTION: --����SD���е�����
*
*@Input: --��
*
*@Out: --��
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
  //����
  package_send(tmp, len,0,0);
  
  len=len+18-len%4;
  mf_write(send_dat,len+16-(len)%16);
  mf_close();
  myfree(SRAMIN,tmp);
  tmp=NULL;
}   


/***************************************
*@DESCRIPTION: --Ӳ����ַ����
*
*@Input: --pdat���յ���Ӳ����ַ
*
*@Out: --��
*
*@Return: --void
****************************************/	
void deal_addr(unsigned char *pdat)
{
  mem_copy(Info_Sd->Addr,pdat,8);
}

/***************************************
*@DESCRIPTION: --WIFI�û������봦��
*
*@Input: --pdat���յ���WIFI�û�������

*
*@Out: --��
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
*@DESCRIPTION: --������IP���˿ں�����
*
*@Input: --pdat���յ��ķ����������Ϣ

*
*@Out: --��
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
*@DESCRIPTION: --WIFI���ܷ�ʽ����
*
*@Input: --pdat���յ���WIFI���ܷ�ʽ

*
*@Out: --��
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
*@DESCRIPTION: --����汾������
*
*@Input: --pdat���յ�������汾��

*
*@Out: --��
*
*@Return: --void
****************************************/	
void deal_softver(unsigned char *pdat)
{
  mem_clear(Info_Sd->Soft_Ver,25);
  mem_copy(Info_Sd->Soft_Ver,pdat,25);

}

/***************************************
*@DESCRIPTION: --Ӳ���汾������
*
*@Input: --pdat���յ���Ӳ���汾��

*
*@Out: --��
*
*@Return: --void
****************************************/	
void deal_hardver(unsigned char *pdat)
{
  mem_clear(Info_Sd->Hard_Ver,25);
  mem_copy(Info_Sd->Hard_Ver,pdat,25);
}

/***************************************
*@DESCRIPTION: --485������
*
*@Input: --pdat���յ���Ӳ���汾��

*
*@Out: --��
*
*@Return: --void
****************************************/	
void deal_485(unsigned char *pdat)
{
  mem_clear((unsigned char*)Sd_485,Num_485*sizeof(INFO_485));
  mem_copy((unsigned char*)Sd_485,pdat+1,*(pdat)*sizeof(INFO_485));

}

/***************************************
*@DESCRIPTION: --����Ƶ������
*
*@Input: --pdat���յ��ķ���Ƶ��

*
*@Out: --��
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
*@DESCRIPTION: --��ȡ�Ƿ�ģ�����ݷ���
*
*@Input: --��

*
*@Out: --��
*
*@Return: --char  1��ʾ��ǰʹ��ģ�ⷢ��
****************************************/	
char get_issimsend()
{
  return Is_sim_send;
}

/***************************************
*@DESCRIPTION: --��ȡ����汾��
*
*@Input: --��

*
*@Out: --��
*
*@Return: --char  *  ���ص���25���ֽڵ�����汾��
****************************************/	
unsigned char *get_softver()
{
  return Info_Sd->Soft_Ver;
}


/***************************************
*@DESCRIPTION: --���ڴ��������汾���¼����ò���
*
*@Input: --uart_data�����ڽ�������
--len      �����ڽ��ճ���
--flag     :��־λ��Ϊ1ʱ��ʾ�Ǵ��ڴ���Ϊ0ʱ��ʾSD�����ݻ�ȡ
*
*@Out: --��
*       
*@Return: --void
****************************************/	
    /*���տ����55 fe fe 03 02 f9 25 close
55 fe fe 03 01 b9 24 open
55 FE FE 03 03 38 E5 stop*/
unsigned char yang_on[7]= {0X55,0XFE,0XFE,0X03,0X01,0XB9,0X24};

unsigned char yang_off[7]= {0X55,0XFE,0XFE,0X03,0X02,0XF9,0X25};
//���д���Ų���
extern char wifi_flag;

/*
wifi2���շ��������ص�ָ����������������֡ͷ��2�ֽڣ������ȣ�1�������ܱ�־��1����Ӳ����ţ�8������12�ֽ�ͷ�ж�
15:14 2018/10/12
LK
*/
void Deal_Uart(unsigned char *uart_data,int len,int flag1)
{
  int i=0;
  int j=0;
  int flag =0;
//  float shadedat = 0;
//  unsigned char addr[8] = { 0x4f,0x45,0x41,0x00,0x00,0x0E,0x00,0x00 };//С������������
//  unsigned char rece[UART_RX_LEN]={0};
//  char a=0,num;
  unsigned char dat[4]={0};
//  USART_Puts(UART4, "\n", 1);//debug
//               for(int t=0;t<5;t++)
//    {
//                UART4->DR= 0x55;
//                while((UART4->SR&0X40)==0);//�ȴ����ͽ���
//            }
//            USART_Puts(UART4, "\n", 1);//debug
    
  while(1)
  {
    
    //�׸��ֽ�Ϊ0���˳�ѭ��
    IWDG_Feed();
//    if((*(uart_data+i)==0)) 
//      break;
    if(i>=len)
      break;
    
    if(*(uart_data+i)==0x55 && *(uart_data+i+1)==0xAA &&uart_data[2+i]>=2)//֡ͷУ�飬����У�����2
    {
      /*�յ�����������������������485�����*/
 //     FromUartToSD2HAO(uart_data,len);
//       ifCB=0;
       IWDG_Feed();
//       USART_Puts(UART4, (uart_data+i),1);//debug
//       UART4->DR= i;
//           while((UART4->SR&0X40)==0);//�ȴ����ͽ���
//       USART_Puts(UART4, &uart_data[4+i],uart_data[2+i]-3);//debug
       if(Check_CheckCRC16(&uart_data[3+i],uart_data[2+i]-2))  //CRCУ��
       {
         if(*(uart_data+3+i)>=100)  //buf[3]Ϊ�Ƿ���ܱ�־������100Ϊ���ܣ������
         {
           btea_decrpyt(&uart_data[4+i],uart_data[2+i]-3);  //���ݽ���
         }
          
         USART_Puts(UART4, &uart_data[i], uart_data[2+i]+3);//debug
         j=*(uart_data+2+i)+i-11;
//         if(((memcmp(&uart_data[i+4], addr, 8)) || (memcmp(&uart_data[i+4],bro_addr,8))))
//         {
//           //����Ǻ�̨���������ݣ����ж�Ӳ����ַ�Ƿ�һ��
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
           if(i>=j)  //���ݳ���С��11ֱ���˳�
             break;
          
           switch(*(uart_data+i+12))  //����λ֡ͷ�ж�
           {
               case 0xB3: 
//                   USART_Puts(UART4, (unsigned char *)uart_data+i+12, 7);//debug
                  if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x01))  //���
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.ventilate = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x03))  //��Ĥ
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.juanmo = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x05))  //ˮ��
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.wat4 = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x07))  //�ڣ�����1
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.yang = chartofloat(dat);
                  }
                  else if((*(uart_data+i+13) == 0x00) && (*(uart_data+i+14) == 0x09))  //�⣬����2
                  {
                    mem_copy(dat, uart_data+i+15, 4);
                    wat_fer.yang1 = chartofloat(dat);
                  }
                  
                  send();	//�������ݵ�������
                  
                  relay_board_control();  //���Ϳ��Ƽ̵���������
                  
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
//                    send();	//��������
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