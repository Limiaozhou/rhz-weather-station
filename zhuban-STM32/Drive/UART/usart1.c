#include "includes.h"					

////////////////////////////////////////////////////////////////////////////////// 
//UART初始化、读写程序
//创建：mfg
//版本：V1.0
//修改日期:2015/8/03
//Copyright(C) 深圳市睿海智电子科技有限公司  	 
////////////////////////////////////////////////////////////////////////////////// 

 	
uchar USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uchar RX_count=0;       //接收状态标记	 

/*
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;        
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////
*/
/***************************************
*@DESCRIPTION: --1字节数据发送
*
*@Input: --发送数据
*
*@Out: --无
*
*@Return: --无
****************************************/	  

void UART3_SendByte(uchar data)
{
   while(!(USART3->SR & 0x80));//等待发送寄存器为空,(否则连续发送时数据易丢失 )
   USART3->DR = data;//发送数据
   while(!(USART3->SR & 0x40));//等待TC = 1;也就是发送完成
   USART3->SR &= ~(1 << 6);//清除发送完成标志
}

/***************************************
*@DESCRIPTION: --多个字节数据发送
*
*@Input: --pdat:发送数据指针  len:数据长度
*
*@Out: --无
*
*@Return: --无
****************************************/	  

void UART3_SendStr(uchar *pdat, uchar len)
{
	 uchar i;
   for(i = 0;i < len;i++)//循环发送,直至发送完毕
   {
     UART3_SendByte(*pdat++);
   }
}

 
/***************************************
*@DESCRIPTION: --中断处理
*
*@Input: --无
*
*@Out: --无
*
*@Return: --无
****************************************/	  
void USART3_IRQHandler(void)
{
	uchar data;	

	if(USART3->SR&(1<<5))//接收到数据
	{	 
		//USART_RX_BUF[RX_count++] = USART1->DR; 
     data = USART1->DR;
     USART3->DR = data;
     while(!(USART3->SR & 0x40));//等待TC = 1;也就是发送完成
     USART3->SR &= ~(1 << 6);//清除发送完成标志
   }
		
} 
									 
/***************************************
*@DESCRIPTION: --初始化串口3
*
*@Input: --pclk2:PCLK2时钟频率(Mhz)  bound:波特率
*
*@Out: --无
*
*@Return: --无
****************************************/	

void UART3_init(uint pclk2,uint bound)
{  	 
	float temp;
	ushort mantissa;
	ushort fraction;	   
	
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	AFIO->MAPR&=0XFFFFFFCF;  //USART3没有重映射
  //AFIO->MAPR|=0x00000000;
	
	RCC->APB2ENR|=1<<3;   //使能PORTB口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	
	GPIOB->CRH&=0XFFFF00FF;//IO状态设置
	GPIOB->CRH|=0X00008B00;//IO状态设置
		  
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1=0X0000200C;  //1位停止,无校验位.
	//USART3->CR2|=0X200C;  //1位停止,无校验位.

	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	NVIC_Init(3,3,USART3_IRQn,2);//组2，最低优先级 

}
