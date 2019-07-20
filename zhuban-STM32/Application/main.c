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
//主函数
//xj
//版本：V1.0
//修改日期:2015/8/31
//修改日期: 14:31 2018/10/11 author LK
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

extern char Ana_Flag; 			//200ms时处理串口数据，该值在timer.c定时器文件中设置
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

//每五分钟发送一个DNS解析命令，跟上一次解析的IP地址作比较，若两次解析的IP地址相同，则继续发送，若不同，需要重新发命令让其连网，若发送后五秒内无回应，则重启设备
int t=0;
char wifi_flag=0;//当版本有更新时，该标志位置1，配置WIFI

void WIFI_GPRS_INIT()//GPRS1_RST pb8
{
  GPIOB->BRR |= 0X100;
  delay_s(1); 
  GPIOB->BSRR |= 0X100;
  GPRS_Reset();
}

void WIFI_INIT()//WIFI_RST pa1,0复位，正常1，对应PCB WIFI2
{
  GPIOA->BRR |= 0X02;//端口位清除寄存器,1：清除对应的ODRy位为0
  delay_s(1); 
  GPIOA->BSRR |= 0X02;//端口位设置/清除寄存器,1：设置对应的ODRy位为1
}


/***************************************
*@DESCRIPTION: --SD卡info.dat文件中数据取出保存在Info_Sd结构体中
*       当SD卡文件打开错误时，并且Flash中有东西（首位不为0xff）
        将该数据写入SD卡中
       SD卡文件打开成功，则将SD卡数据解析到Info_sd结构体中
*@Input: 
*
*@Return: --无
****************************************/	
//void get_SD_Info()
//{
//  char SDStatus=0,len;
//  SDStatus+=mf_open(INFO_FILE,FA_READ);
//  //如果没有该文件，则创建一个，同时把FLASH中的相关数据填充到该文件中
//  if(SDStatus!=SD_OK && (Info_Flash->Addr[0]!=0xff))
//  {
//    mem_copy((unsigned char*)Info_Sd,(unsigned char*)Info_Flash,sizeof(INFO_MUL));
//    mem_copy((unsigned char*)Sd_485,(unsigned char*)Flash_485,sizeof(INFO_485));
//    if(SDStatus==FR_NO_FILE)
//      update_sd_485(*Info_Flash);
//  }
//  else if(SDStatus==SD_OK)//有该文件，打开并读出，将信息填充到结构体中
//  {
//    mf_lseek(0);
//    len=mf_size();
//    mf_read(len);
//    mf_close();
//    Deal_Uart(fatbuf,len,0);
//  }  
//}

/***************************************
*@DESCRIPTION: --FLASH中数据取出保存在Info_Flash结构体中
*
*@Input: --无
*
*@Return: --无
****************************************/	
void get_Flash_Info()
{
  STMFLASH_Read(FLASH_INFO_MUL,(unsigned short *)(Info_Flash->Addr),(sizeof(INFO_MUL)-50)/2);
  STMFLASH_Read(FLASH_INFO_VER,(unsigned short *)(Info_Flash),25);
  
  STMFLASH_Read(FLASH_INFO_485,(unsigned short *)(Flash_485),sizeof(INFO_485)*Num_485/2);
}

/***************************************
*@DESCRIPTION: --空间申请及初始化
*
*@Input: --无
*
*@Return: --无
****************************************/	
void Apply_Space()
{
  mem_init(SRAMIN);	        //初始化内部内存池	 
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
   RCC->APB1RSTR = 0x00000000;//?′???áê?       
  RCC->APB2RSTR = 0x00000000; 
    	
    RCC->AHBENR = 0x00000014;  //?ˉ???￡ê?éá′?oíSRAMê±?óê1?ü.????1?±?.    
    RCC->APB2ENR = 0x00000000; //íaéèê±?ó1?±?.         
    RCC->APB1ENR = 0x00000000;   
  RCC->CR |= 0x00000001;     //ê1?ü?ú2????ùê±?óHSION                                  
  RCC->CFGR &= 0xF8FF0000;   //?′??SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]           
  RCC->CR &= 0xFEF6FFFF;     //?′??HSEON,CSSON,PLLON
  RCC->CR &= 0xFFFBFFFF;     //?′??HSEBYP         
  RCC->CFGR &= 0xFF80FFFF;   //?′??PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
  RCC->CIR = 0x00000000;     //1?±??ùóD?D??     
  //?????òá?±í          
#ifdef  VECT_TAB_RAM
  MY_NVIC_SetVectorTable(0x20000000, 0x0);
#else   
  MY_NVIC_SetVectorTable(0x08000000,0x0);
#endif
}

void Stm32_Clock1_Init(u8 PLL)
{
  unsigned char temp=0;   
  MYRCC_DeInit();      //?′??2￠?????òá?±í
   RCC->CR|=0x00010000;  //ía2????ùê±?óê1?üHSEON
  while(!(RCC->CR>>17));//μè′yía2?ê±?ó?íD÷
  RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
  PLL-=2;//μ???2??μ￥??
  RCC->CFGR|=PLL<<18;   //éè??PLL?μ 2~16
  RCC->CFGR|=1<<16;    //PLLSRC ON 
  FLASH->ACR|=0x32;    //FLASH 2???óê±?ü?ú

  RCC->CR|=0x01000000;  //PLLON
  while(!(RCC->CR>>25));//μè′yPLL???¨
  RCC->CFGR|=0x00000002;//PLL×÷?a?μí3ê±?ó   
  while(temp!=0x02)     //μè′yPLL×÷?a?μí3ê±?óéè??3é1|
  {   
    temp=RCC->CFGR>>2;
    temp&=0x03;
  }    
}

void WIFI2_init(void)
{
    Usart_init(1, 115200);//USR-WIFI232 插针WiFi2 初始化 WIFI_RST PA1 0有效
	GPIO_InitTypeDef wifi_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	wifi_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	wifi_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	wifi_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &wifi_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	
}

/***************************************
*@DESCRIPTION: --主函数
*
*@Input: --无
*
*@Return: --无
****************************************/	

int main()
{
 
  STM32_IAR_SYS_INIT();  //内部36M
    UART4_init(36, 115200);//uart4，115200，debug
    Usart_init(2,9600);// usart2，9600，hmi
  Usart_init(3,9600);  //uart3，9600，485
  TIM4_Int_Init(1999,3599);  //2000 * 3600 / 36M = 200ms

  RTC_Init();
  Apply_Space();          
//  WIFI_GPRS_INIT();//GPRS1_RST
//  WIFI_INIT();
  WIFI2_init();//usart1，WiFi
  IWDG_Init(4, 1250);	
  wifi_flag=0;
  
  get_Flash_Info();


  while(1)
  {
    if(Ana_Flag)  //200ms间隔
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
        GenericApp_package_Deal(2);  //获取屏幕串口数据
      }
      
      if(DMA_Usart_Receive(3))
      {
        GenericApp_package_Deal(3);  //get_RTU_data，485
      }

      if(send_flag)  //10s
      {
        send();	//发送数据到服务器
        send_flag=0;
      }
      if(send485_flag)  //2s
      {
        send485_flag=0;
        send_Cmd();	//发送485命令，读传感器
      }
      
      if(hmi_send_flag)  //5s
      {
        hmi_send_flag = 0;
        hmi_send();  //发送屏幕数据
      }

      if(relay_send_flag)  //5s
      {
        relay_send_flag = 0;
        relay_board_control();  //发送控制继电器板命令
      }
    }
  }
  mem_free(SRAMIN, sizeof(ST_EVNBALEDAT));
}

