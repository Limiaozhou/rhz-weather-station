#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "malloc.h"
#include "string.h"
#include "mem.h"
#include "sys.h"
#include "timer.h"
#include "wdg.h"
#include "gpio.h"
#include "dma.h"
#include "sdio_sdcard.h"
#include "exfuns.h"
#include "ff.h"
#include "sys.h"
#include "fattester.h"
#include "stmflash.h"
#include "set_info.h"
#include "rs485_deal.h"
#include "data.h"
#include "gprs.h"
#include "led.h"
#include "tea.h"
#include "check.h"
#include "update.h"
#include "data.h"
#include "wifi.h"
#include "rtc.h"
#include "air_cond_swi.h"
#include "stm32f10x_usart.h"
#include "wat_fer.h"
#include "rs485_deal.h"

#include "hmi.h"
#include "relay_board.h"

////////////////////////////////////////////////////////////////////////////////// 
//������
//xj
//�汾��V1.0
//�޸�����:2015/8/31
//�޸�����: 14:31 2018/10/11 author LK
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 

extern char Ana_Flag; 			//200msʱ���������ݣ���ֵ��timer.c��ʱ���ļ�������
extern char send_flag; 
extern char send485_flag; 
extern int USART2_RX_cnt;
extern int USART1_RX_cnt;
extern int Num_Rea_485;

//extern int motor_flag; 
//extern int air_hum_flag; 
WAT_FER wat_fer;
extern ST_EVNBALEDAT *psenddat;
unsigned char *send_dat;
unsigned char *sensor_rtc;
extern ST_ITEM *st_item;

//
//
//unsigned char DATA_FILE[]= "0://save.dat";
//unsigned char INFO_FILE[]="0://info.dat";
//unsigned char UPDATE_FILE[30]="0://update.inf";
//unsigned char READ_FILE[]= "0://READ.dat";
//


INFO_MUL *Info_Flash;
INFO_MUL *Info_Sd;

INFO_485 *Flash_485;
INFO_485 *Sd_485;

//#define Num_485 20

//ÿ����ӷ���һ��DNS�����������һ�ν�����IP��ַ���Ƚϣ������ν�����IP��ַ��ͬ����������ͣ�����ͬ����Ҫ���·��������������������ͺ��������޻�Ӧ���������豸
int t=0;
char wifi_flag=0;//���汾�и���ʱ���ñ�־λ��1������WIFI

void WIFI_GPRS_INIT()//GPRS1_RST pb8
{
  GPIOB->BRR |= 0X100;
  delay_s(1); 
  GPIOB->BSRR |= 0X100;
  GPRS_Reset();
}

void WIFI_INIT()//WIFI_RST pa1,0��λ������1����ӦPCB WIFI2
{
  GPIOA->BRR |= 0X02;//�˿�λ����Ĵ���,1�������Ӧ��ODRyλΪ0
  delay_s(1); 
  GPIOA->BSRR |= 0X02;//�˿�λ����/����Ĵ���,1�����ö�Ӧ��ODRyλΪ1
}


/***************************************
*@DESCRIPTION: --SD��info.dat�ļ�������ȡ��������Info_Sd�ṹ����
*       ��SD���ļ��򿪴���ʱ������Flash���ж�������λ��Ϊ0xff��
        ��������д��SD����
       SD���ļ��򿪳ɹ�����SD�����ݽ�����Info_sd�ṹ����
*@Input: 
*
*@Return: --��
****************************************/	
//void get_SD_Info()
//{
//  char SDStatus=0,len;
//  SDStatus+=mf_open(INFO_FILE,FA_READ);
//  //���û�и��ļ����򴴽�һ����ͬʱ��FLASH�е����������䵽���ļ���
//  if(SDStatus!=SD_OK && (Info_Flash->Addr[0]!=0xff))
//  {
//    mem_copy((unsigned char*)Info_Sd,(unsigned char*)Info_Flash,sizeof(INFO_MUL));
//    mem_copy((unsigned char*)Sd_485,(unsigned char*)Flash_485,sizeof(INFO_485));
//    if(SDStatus==FR_NO_FILE)
//      update_sd_485(*Info_Flash);
//  }
//  else if(SDStatus==SD_OK)//�и��ļ����򿪲�����������Ϣ��䵽�ṹ����
//  {
//    mf_lseek(0);
//    len=mf_size();
//    mf_read(len);
//    mf_close();
//    Deal_Uart(fatbuf,len,0);
//  }  
//}

/***************************************
*@DESCRIPTION: --FLASH������ȡ��������Info_Flash�ṹ����
*
*@Input: --��
*
*@Return: --��
****************************************/	
void get_Flash_Info()
{
  STMFLASH_Read(FLASH_INFO_MUL,(unsigned short *)(Info_Flash->Addr),(sizeof(INFO_MUL)-50)/2);
  STMFLASH_Read(FLASH_INFO_VER,(unsigned short *)(Info_Flash),25);
  
  STMFLASH_Read(FLASH_INFO_485,(unsigned short *)(Flash_485),sizeof(INFO_485)*Num_485/2);
}

/***************************************
*@DESCRIPTION: --�ռ����뼰��ʼ��
*
*@Input: --��
*
*@Return: --��
****************************************/	
void Apply_Space()
{
  mem_init(SRAMIN);	        //��ʼ���ڲ��ڴ��	 
    psenddat=(ST_EVNBALEDAT*)mymalloc(SRAMIN,sizeof(ST_EVNBALEDAT));//
  send_dat=(unsigned char *)mymalloc(SRAMIN,300);
  sensor_rtc=(unsigned char *)mymalloc(SRAMIN,200);
  Info_Sd=(INFO_MUL *)mymalloc(SRAMIN,sizeof(INFO_MUL));
  Info_Flash=(INFO_MUL *)mymalloc(SRAMIN,sizeof(INFO_MUL));
  Flash_485=(INFO_485 *)mymalloc(SRAMIN,sizeof(INFO_485)*Num_485);
  Sd_485=(INFO_485 *)mymalloc(SRAMIN,sizeof(INFO_485)*Num_485);
  
  mem_clear((unsigned char *)Info_Sd,(sizeof(INFO_MUL)));
  mem_clear((unsigned char *)Info_Flash,(sizeof(INFO_MUL)));
  
  mem_clear((unsigned char *)Flash_485,(sizeof(INFO_MUL)*Num_485));
  mem_clear((unsigned char *)Sd_485,(sizeof(INFO_MUL)*Num_485));
}

void MYRCC_DeInit(void)
{  
   RCC->APB1RSTR = 0x00000000;//?��???����?       
  RCC->APB2RSTR = 0x00000000; 
    	
    RCC->AHBENR = 0x00000014;  //?��???�꨺?������?o��SRAM����?����1?��.????1?��?.    
    RCC->APB2ENR = 0x00000000; //��a��������?��1?��?.         
    RCC->APB1ENR = 0x00000000;   
  RCC->CR |= 0x00000001;     //��1?��?��2????������?��HSION                                  
  RCC->CFGR &= 0xF8FF0000;   //?��??SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]           
  RCC->CR &= 0xFEF6FFFF;     //?��??HSEON,CSSON,PLLON
  RCC->CR &= 0xFFFBFFFF;     //?��??HSEBYP         
  RCC->CFGR &= 0xFF80FFFF;   //?��??PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
  RCC->CIR = 0x00000000;     //1?��??����D?D??     
  //?????����?����          
#ifdef  VECT_TAB_RAM
  MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
  MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}

void Stm32_Clock1_Init(u8 PLL)
{
  unsigned char temp=0;   
  MYRCC_DeInit();      //?��??2��?????����?����
   RCC->CR|=0x00010000;  //��a2????������?����1?��HSEON
  while(!(RCC->CR>>17));//�̨���y��a2?����?��?��D��
  RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
  PLL-=2;//��???2??�̣�??
  RCC->CFGR|=PLL<<18;   //����??PLL?�� 2~16
  RCC->CFGR|=1<<16;    //PLLSRC ON 
  FLASH->ACR|=0x32;    //FLASH 2???������?��?��

  RCC->CR|=0x01000000;  //PLLON
  while(!(RCC->CR>>25));//�̨���yPLL???��
  RCC->CFGR|=0x00000002;//PLL����?a?�̨�3����?��   
  while(temp!=0x02)     //�̨���yPLL����?a?�̨�3����?������??3��1|
  {   
    temp=RCC->CFGR>>2;
    temp&=0x03;
  }    
}

void WIFI2_init(void)
{
    Usart_init(1, 115200);//USR-WIFI232 ����WiFi2 ��ʼ�� WIFI_RST PA1 0��Ч
	GPIO_InitTypeDef wifi_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	wifi_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	wifi_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	wifi_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &wifi_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	
}

/***************************************
*@DESCRIPTION: --������
*
*@Input: --��
*
*@Return: --��
****************************************/	

int main()
{
 
  STM32_IAR_SYS_INIT();  //�ڲ�36M
    UART4_init(36, 115200);//uart4��115200��debug
    Usart_init(2,9600);// usart2��9600��hmi
  Usart_init(3,9600);  //uart3��9600��485
  TIM4_Int_Init(1999,3599);  //2000 * 3600 / 36M = 200ms

  RTC_Init();
  Apply_Space();          
//  WIFI_GPRS_INIT();//GPRS1_RST
//  WIFI_INIT();
  WIFI2_init();//usart1��WiFi
  IWDG_Init(4, 1250);	
  wifi_flag=0;
  
  get_Flash_Info();


  while(1)
  {
    if(Ana_Flag)  //200ms���
    {
      Ana_Flag = 0;
      IWDG_Feed();

      if(DMA_Usart_Receive(1))//wifi2 uart1
      {
//        USART_Puts(UART4,"DMA_Usart_Receive",strlen("DMA_Usart_Receive"));
        GenericApp_package_Deal(1);
      }
//      USART_Puts(UART4,"485recv",strlen("485recv"));
      if(DMA_Usart_Receive(2))
      {
        GenericApp_package_Deal(2);  //��ȡ��Ļ��������
      }
      
      if(DMA_Usart_Receive(3))
      {
        GenericApp_package_Deal(3);  //get_RTU_data��485
      }

      if(send_flag)  //10s
      {
        send();	//�������ݵ�������
        send_flag=0;
      }
      if(send485_flag)  //2s
      {
        send485_flag=0;
        send_Cmd();	//����485�����������
      }
      
      if(hmi_send_flag)  //5s
      {
        hmi_send_flag = 0;
        hmi_send();  //������Ļ����
      }

      if(relay_send_flag)  //5s
      {
        relay_send_flag = 0;
        relay_board_control();  //���Ϳ��Ƽ̵���������
      }
    }
  }
  mem_free(SRAMIN, sizeof(ST_EVNBALEDAT));
}

