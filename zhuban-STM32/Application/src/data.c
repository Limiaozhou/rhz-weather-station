#include "data.h"
#include "sys.h"
#include "check.h"
#include "rs485_deal.h"
#include "usart.h"
#include "dma.h"
#include "mem.h"
#include "tea.h"
#include "set_info.h"
#include "gprs.h"
#include "proc_sd.h"
#include "wdg.h"
#include "wat_fer.h"
extern WAT_FER wat_fer;
extern ST_ITEM *st_item;
extern unsigned char *send_dat; //Ҫ���͵�����
extern INFO_MUL *Info_Flash;
extern char wifi_flag;//���汾�и���ʱ���ñ�־λ��1������WIFI

unsigned char recv_data_cnt=0;  //���������ζ�ifCB��Ϊ1���򽫸ô����ݱ�����DATA�ļ���
extern Send_485 send_485;
char ifCB=1;

/***************************************
*@DESCRIPTION: --���ݷ��ͺ���
*
*              ��Ҫ���͵������Զ���������ͷ������ַ�����ܼ�CRCУ��
*@Input: --*pat����Ҫ���͵�����
*        -- len:��Ҫ���͵ĳ���
*
*@Return: --��
****************************************/
void  package_send(unsigned char *pdat, unsigned char len, unsigned char flag,unsigned char flag1)
{
  
  unsigned char i=0,j=0;
  unsigned char addr[8] = { 0x4f,0x45,0x41,0x00,0x00,0x0E,0x00,0x00 };//С������������
//  USART_Puts(UART4,"addr",strlen("addr"));
  *send_dat=0x55;
//  USART_Puts(UART4,"send_dat",strlen("send_dat"));
//  USART_Puts(UART4,send_dat, 1);
  
  *(send_dat+1)=0xaa;
  *(send_dat+3)=0x26;
  
//  USART_Puts(UART4, send_dat, 4);
  mem_copy(send_dat+4,addr, 8);
//  USART_Puts(UART4, addr, 8);
  
  if(flag)
  {
    *(send_dat+12+i)=0xD0;
    *(send_dat+i+13)=len;
    i=2;
  }
  if(flag1)
  {
      j=4-(len+i)%4;
      if(j==4) j=0;
      *(send_dat+2)=len+11+i+j+8;
      mem_copy(send_dat+i+12,pdat,len+8);
      mem_clear(send_dat+len+12+8+i,j);
 //     btea_encrypt(send_dat+4, len+8+i+j+8);
      Check_CalaCRC16(send_dat+3,len+9+i+j+8);
      if(Is_Connect())
      {
        send_3gData(send_dat,len+i+14+j+8);
      }
      if(!wifi_flag)
        USART_Puts(USART1,send_dat,len+i+14+j+8);
 //     USART_Puts(USART3,send_dat,len+i+14+j+8);
      
  }
  else
  {
      j=4-(len+i)%4;
      if(j==4) j=0;
      *(send_dat+2)=len+11+i+j;
      mem_copy(send_dat+i+12,pdat,len);
      mem_clear(send_dat+len+12+i,j);
 //     btea_encrypt(send_dat+4, len+8+i+j);
      Check_CalaCRC16(send_dat+3,len+9+i+j);
      if(Is_Connect())
      {
        send_3gData(send_dat,len+i+14+j);
      }
      if(!wifi_flag)
      {
        USART_Puts(USART1,send_dat,len+i+14+j);
 //       USART_Puts(USART3,send_dat,len+i+14+j);
//        USART_Puts(UART4,send_dat,len+i+14+j);
      }
  }     
  IWDG_Feed();
}

/***************************************
*@DESCRIPTION: --�����������ݴ��
*
*@Input: --*pat��������Ĵ�������
*
*@Return: --��
****************************************/	
ST_EVNBALEDAT *psenddat;	//�����͵�����


SENSORTYPE sensor;
void Evn_Packagedat()
{
//USART_Puts(UART4,"Evn_Packagedat",strlen("Evn_Packagedat"));
	unsigned char addr[8] = { 0x4F,0x45,0x41,0x03,0x00,0x15,0x00,0x00 };

	psenddat->head = 0xAA55;

    /*ʮ����21��  21+3��
    6x(x�Ǵ���������psenddat->data.len )+2��crc16+2��tea+2��cmdlen+8����ַcode+4��ͷ���汾-3
    ���ӵ�������ʼ��������������15+6x*/
	psenddat->len = 15+6*8;

	psenddat->ver = 38;
	mem_copy(psenddat->addr, addr, 8);
	psenddat->data.cmd = 0xD0;
	psenddat->data.len = 6*8;//ʮ����6,�����ּ�������һ��6�ֽ�

//	psenddat->data.shade.key[0] = 0x01;
//	psenddat->data.shade.key[1] = 0x44; // С�������������ϵĵ�ַ
//
////	mem_copy(psenddat->data.shade.dat, (unsigned char*)&(sensor.shade), 4);
//        mem_copy(psenddat->data.shade.dat, (unsigned char*)&(wat_fer.yang), 4);

//        psenddat->data.pm25.key[0] = 0x00;
//	psenddat->data.pm25.key[1] = 0x68; 
//        mem_copy(psenddat->data.pm25.dat, (unsigned char*)&(sensor.pm25), 4);

	
//	psenddat->data.co2.key[0] = 0x00;
//	psenddat->data.co2.key[1] = 0x7A; //CO2
//	mem_copy(psenddat->data.co2.dat, (unsigned char*)&(sensor.co2), 4);
        
    psenddat->data.temp.key[0] = 0x00;
	psenddat->data.temp.key[1] = 0x80; //�¶�
	mem_copy(psenddat->data.temp.dat, (unsigned char*)&(sensor.temp), 4);
    
    psenddat->data.humi.key[0] = 0x00;
	psenddat->data.humi.key[1] = 0x82; //ʪ��
	mem_copy(psenddat->data.humi.dat, (unsigned char*)&(sensor.humi), 4);
    
    psenddat->data.light.key[0] = 0x00;
	psenddat->data.light.key[1] = 0x06; //����
	mem_copy(psenddat->data.light.dat, (unsigned char*)&(sensor.light), 4);
    
//    psenddat->data.tvoc.key[0] = 0x00;
//	psenddat->data.tvoc.key[1] = 0x7C; //TVOC
//	mem_copy(psenddat->data.tvoc.dat, (unsigned char*)&(sensor.tvoc), 4);
    
//    psenddat->data.fx.key[0] = 0x00;
//	psenddat->data.fx.key[1] = 0x04; //����
//	mem_copy(psenddat->data.fx.dat, (unsigned char*)&(sensor.fx), 4);
//    
//    psenddat->data.fs.key[0] = 0x00;
//	psenddat->data.fs.key[1] = 0x02; //����
//	mem_copy(psenddat->data.fs.dat, (unsigned char*)&(sensor.fs), 4);
    
//    psenddat->data.so2.key[0] = 0x00;
//	psenddat->data.so2.key[1] = 0x6E; //so2
//	mem_copy(psenddat->data.so2.dat, (unsigned char*)&(sensor.so2), 4);
    
//    psenddat->data.ultvio.key[0] = 0x00;
//	psenddat->data.ultvio.key[1] = 0x0A; //������
//	mem_copy(psenddat->data.ultvio.dat, (unsigned char*)&(sensor.ultvio), 4);
//    
//    psenddat->data.pressure.key[0] = 0x00;
//	psenddat->data.pressure.key[1] = 0x84; //��ѹ
//	mem_copy(psenddat->data.pressure.dat, (unsigned char*)&(sensor.pressure), 4);
//    
//    psenddat->data.pm25.key[0] = 0x00;
//	psenddat->data.pm25.key[1] = 0x68; //PM2.5
//	mem_copy(psenddat->data.pm25.dat, (unsigned char*)&(sensor.pm25), 4);
    
//    psenddat->data.yuxue.key[0] = 0x00;
//	psenddat->data.yuxue.key[1] = 0x18; //�Ƿ�����ѩ
//	mem_copy(psenddat->data.yuxue.dat, (unsigned char*)&(sensor.yuxue), 4);
    
    
    psenddat->data.ventilate.key[0] = 0x00;
	psenddat->data.ventilate.key[1] = 0x01; //���
	mem_copy(psenddat->data.ventilate.dat, (unsigned char*)&(wat_fer.ventilate), 4);
    
    psenddat->data.juanmo.key[0] = 0x00;
	psenddat->data.juanmo.key[1] = 0x03; //��Ĥ
	mem_copy(psenddat->data.juanmo.dat, (unsigned char*)&(wat_fer.juanmo), 4);
    
    psenddat->data.wat4.key[0] = 0x00;
	psenddat->data.wat4.key[1] = 0x05; //ˮ��
	mem_copy(psenddat->data.wat4.dat, (unsigned char*)&(wat_fer.wat4), 4);
    
    psenddat->data.yang.key[0] = 0x00;
	psenddat->data.yang.key[1] = 0x07; //�ڣ�����1
	mem_copy(psenddat->data.yang.dat, (unsigned char*)&(wat_fer.yang), 4);
    
    psenddat->data.yang1.key[0] = 0x00;
	psenddat->data.yang1.key[1] = 0x09; //�⣬����2
	mem_copy(psenddat->data.yang1.dat, (unsigned char*)&(wat_fer.yang1), 4);
    
	Check_CalaCRC16((unsigned char *)psenddat + 3, psenddat->len - 2);  //��buf[3]��ʼcrcУ��
  
}

void send()
{

    Evn_Packagedat();

    USART_Puts(USART1, (unsigned char *)psenddat, psenddat->len + 3);//WIFI2 send 42byte
//    USART_Puts(USART3, (unsigned char *)psenddat, psenddat->len + 3);//debug

  IWDG_Feed();

}
