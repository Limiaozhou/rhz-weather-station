#include "usart.h"
#include "string.h"
#include "dma.h"
#include "gprs.h"
#include "mem.h"
#include "data.h"
#include "check.h"
#include "led.h"
#include "gpio.h"
#include "tea.h"
#include "sys.h"
#include "proc_sd.h"
#include "sys.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "data.h"
#include "fattester.h"
#include "stmflash.h"
#include "update.h"
#include "rs485_deal.h"
#include "wifi.h"

#include "stm32f10x_usart.h"
extern void IWDG_Feed(void);



int USART1_RX_cnt=0;
extern const char HEAD[2];


void uart_init(int pclk2,int bound,USART_TypeDef* USARTx)
{  	 
  float temp;
  int mantissa;
  int fraction;	   
  
  temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
  mantissa=temp;				 //得到整数部分
  fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
  mantissa+=fraction; 
  //波特率设置
  USARTx->BRR=mantissa; // 波特率设置	 
  
  //使能接收中断
  //	USARTx->CR1|=1<<8;    //PE中断使能
  USARTx->CR1=0X0000000C;  //1位停止,无校验位.
  USARTx->CR3|=1<<6;	
  //	USARTx->CR1|=1<<5;    //接收缓冲区非空中断使能	    
  USARTx->CR1|=1<<13;
}

void USART_Config(unsigned char Usart_Num,u32 bound)
{
 /* switch(Usart_Num)
  {
  case 1:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    GPIOA->CRH &=~0X0000FFF0; 
    GPIOA->CRH |= 0X000034B0;
    AFIO->MAPR&=0XFFFFFFF4;  //USART3没有重映射	
    RCC->APB2RSTR|=1<<14;   //复位串口3
    RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
    uart_init(36,bound,USART1);
    
    break;
  case 2:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    GPIOA->CRL &=~0X0000FF00; 
    GPIOA->CRL |= 0X00004B00;
    AFIO->MAPR&=0XFFFFFFF8;  //USART3没有重映射	
    RCC->APB1RSTR|=1<<17;   //复位串口3
    RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
    uart_init(36,bound,USART2);
    
    break;
  case 3: 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIOB->CRH &=~0X0000FF00; 
    GPIOB->CRH |= 0X00004B00;
    AFIO->MAPR&=~0XFFFFFF3F;  //USART3没有重映射	
    RCC->APB1RSTR|=1<<18;   //复位串口3
    RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
    uart_init(36,bound,USART3);
    
    break;
    
  case 4:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    GPIOC->CRH &=~0X0000FF00;
    GPIOC->CRH |= 0X00004B00;
    RCC->APB1RSTR|=1<<19;
    RCC->APB1RSTR&=~(1<<19);
    uart_init(36,bound,UART4);
    
    break;
  default:
    break;
  }*/
    USART_TypeDef* USARTx;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  switch(Usart_Num)
  {
  case 1:
    USARTx=USART1;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    /* Configure USART1_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    break;
  case 2:
    USARTx=USART2;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    /* Configure USART1_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    break;
  case 3:
    USARTx=USART3;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    /* Configure USART1_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART1_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    break;
  default:
    break;
  }
    //USART1¤×÷?úòì2??￡ê???
		USART_DeInit(USARTx);
    USART_InitStructure.USART_BaudRate = bound;//2¨ì??ê
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//êy?Y??êy
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//ò???í￡?1??
    USART_InitStructure.USART_Parity = USART_Parity_No ;//?T????D￡?é??
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//?Tó2?t????á÷
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//·￠?í?óê??ùê1?ü

    /* Configure the USARTx */ 
    USART_Init(USARTx, &USART_InitStructure);
  USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);

    /* Enable the USARTx */
    USART_Cmd(USARTx, ENABLE);
}

void DMA_Usart_Receive_init(unsigned char Usart_Num)
{
  switch(Usart_Num)
  {
  case 1:
    MYDMA_Config(DMA1_Channel5,(u32)(&USART1->DR),(u32)USART1_RX_BUF,UART_RX_LEN);
    break;
  case 2:
    MYDMA_Config(DMA1_Channel6,(u32)(&USART2->DR),(u32)USART2_RX_BUF,UART_RX_LEN);
    break;
  case 3:
    MYDMA_Config(DMA1_Channel3,(u32)(&USART3->DR),(u32)USART3_RX_BUF,UART_RX_LEN);
    break;
/*  case 4:
    MYDMA_Config(DMA2_Channel3,(u32)(&UART4->DR),(u32)UART4_RX_BUF,UART_RX_LEN);
    break;*/
  default:
    break;
  }
}

void Usart_init(unsigned char Num,u32 bound)
{
  DMA_Usart_Receive_init(Num);
  USART_Config(Num,bound);
}

void USART_Puts(USART_TypeDef* USARTx,unsigned char * str,int len)
{
  while(len--)
  {
    while(!(USARTx->SR & 0x80));//等待发送寄存器为空,(否则连续发送时数据易丢失 )
    USARTx->DR = *str++;//发送数据
    while(!(USARTx->SR & 0x40));//等待TC = 1;也就是发送完成
    USARTx->SR &= ~(1 << 6);//清除发送完成标志
  }
}

//extern unsigned char wifi_flag;//表示是否有接收到上一条数据，0表示没有，此时在PACKET_SEND函数中将该条数据写入文件中

//extern unsigned char wifiSet;

void GenericApp_package_Deal(unsigned char Num)
{
  switch(Num)
  {
  case 1://wifi2 uart1
//    USART_Puts(UART4, (unsigned char *)USART1_RX_BUF, strlen(USART1_RX_BUF));//debug
    if(find_str((const char *)USART1_RX_BUF, HEAD,UART_RX_LEN,2))
    {  
      
      USART1_RX_cnt++;
    }
    IWDG_Feed();
//    WIFI_RxDeal(USART1_RX_BUF, UART_RX_LEN-MYDMA_GetCurrDataCounter(DMA1_Channel5));//检测wifi状态吧
//
    Deal_Uart(USART1_RX_BUF,UART_RX_LEN-MYDMA_GetCurrDataCounter(DMA1_Channel5),1);
    mem_clear(USART1_RX_BUF,UART_RX_LEN);
    MYDMA_Enable(DMA1_Channel5);
    break;
  case 2:
    mem_clear(USART2_RX_BUF,UART_RX_LEN);
    MYDMA_Enable(DMA1_Channel6);
    break;
  case 3://485返回数据
    get_RTU_data(USART3_RX_BUF,UART_RX_LEN-MYDMA_GetCurrDataCounter(DMA1_Channel3));
    
    mem_clear(USART3_RX_BUF,UART_RX_LEN);
    MYDMA_Enable(DMA1_Channel3);
    break;
  default:
    break;
  }
}

unsigned char DMA_Usart_Receive(unsigned char Num)
{
  DMA_Channel_TypeDef* DMAy_Channelx;
  USART_TypeDef* USARTx;
  u32 len;
  switch(Num)
  {
  case 1:
    DMAy_Channelx=DMA1_Channel5;
    USARTx = USART1;
    break;
  case 2:
    DMAy_Channelx=DMA1_Channel6;
    USARTx = USART2;
    break;
  case 3:
    DMAy_Channelx=DMA1_Channel3;
    USARTx = USART3;
    break;
  case 4:
    DMAy_Channelx=DMA2_Channel3;
    USARTx = UART4;
    break;
  default:
    break;
  }
  len = MYDMA_GetCurrDataCounter(DMAy_Channelx);
  if(len!=UART_RX_LEN)
  {
    if((USARTx->SR&2)|| UART_RX_LEN-len<1)
    {
      MYDMA_Enable(DMAy_Channelx);
      return 0;
    }
    return 1;
  }
  else return 0;
}
u8 USART1_RX_BUF[UART_RX_LEN];     //????,??USART_REC_LEN???.
u8 USART2_RX_BUF[UART_RX_LEN];     //????,??USART_REC_LEN???.
u8 USART3_RX_BUF[UART_RX_LEN];     //????,??USART_REC_LEN???.
//u8 UART4_RX_BUF[UART_RX_LEN];



/***************************************
*@DESCRIPTION: --初始化串口4 用于调试使用
*
*@Input: --pclk2:PCLK2时钟频率(Mhz)  bound:波特率
*
*@Out: --无
*
*@Return: --无
****************************************/

void UART4_init(u32 pclk2, u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;

	temp = (float)(pclk2 * 1000000) / (bound * 16);//得到USARTDIV
	mantissa = temp;				 //得到整数部分
	fraction = (temp - mantissa) * 16; //得到小数部分	 
	mantissa <<= 4;
	mantissa += fraction;

	//AFIO->MAPR &= 0XFFFFFFCF;  //USART没有重映射
	//AFIO->MAPR|=0x00000000;

	RCC->APB2ENR |= 1 << 4;   //使能PORTB口时钟  
	RCC->APB1ENR |= 1 << 19;  //使能串口时钟 USARTEN：USART 3时钟使能

	GPIOC->CRH &= 0XFFFF00FF;//c10 tx c11 rx
	GPIOC->CRH |= 0X00004B00;//IO状态设置

	RCC->APB1RSTR |= 1 << 19;   //复位串口
	RCC->APB1RSTR &= ~(1 << 19);//停止复位	   	   
								//波特率设置
	UART4->BRR = mantissa; // 波特率设置	 
	UART4->CR1 = 0X0000000C;  //1位停止,无校验位.
							  //USART3->CR2|=0X200C;  //1位停止,无校验位.

							  //使能接收中断
	UART4->CR1 |= 1 << 8;    //PE中断使能
	UART4->CR1 |= 1 << 5;    //接收缓冲区非空中断使能	    	
	UART4->CR1 |= 1 << 13;    //1：USART模块使能						  // NVIC_Init(3,3,USART3_IRQn,2);//组2，最低优先级 
	MY_NVIC_Init(3, 3, UART4_IRQn, 2);
}
