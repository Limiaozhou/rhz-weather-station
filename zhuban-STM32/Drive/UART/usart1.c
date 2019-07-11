#include "includes.h"					

////////////////////////////////////////////////////////////////////////////////// 
//UART��ʼ������д����
//������mfg
//�汾��V1.0
//�޸�����:2015/8/03
//Copyright(C) ��������ǵ��ӿƼ����޹�˾  	 
////////////////////////////////////////////////////////////////////////////////// 

 	
uchar USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uchar RX_count=0;       //����״̬���	 

/*
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;        
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////
*/
/***************************************
*@DESCRIPTION: --1�ֽ����ݷ���
*
*@Input: --��������
*
*@Out: --��
*
*@Return: --��
****************************************/	  

void UART3_SendByte(uchar data)
{
   while(!(USART3->SR & 0x80));//�ȴ����ͼĴ���Ϊ��,(������������ʱ�����׶�ʧ )
   USART3->DR = data;//��������
   while(!(USART3->SR & 0x40));//�ȴ�TC = 1;Ҳ���Ƿ������
   USART3->SR &= ~(1 << 6);//���������ɱ�־
}

/***************************************
*@DESCRIPTION: --����ֽ����ݷ���
*
*@Input: --pdat:��������ָ��  len:���ݳ���
*
*@Out: --��
*
*@Return: --��
****************************************/	  

void UART3_SendStr(uchar *pdat, uchar len)
{
	 uchar i;
   for(i = 0;i < len;i++)//ѭ������,ֱ���������
   {
     UART3_SendByte(*pdat++);
   }
}

 
/***************************************
*@DESCRIPTION: --�жϴ���
*
*@Input: --��
*
*@Out: --��
*
*@Return: --��
****************************************/	  
void USART3_IRQHandler(void)
{
	uchar data;	

	if(USART3->SR&(1<<5))//���յ�����
	{	 
		//USART_RX_BUF[RX_count++] = USART1->DR; 
     data = USART1->DR;
     USART3->DR = data;
     while(!(USART3->SR & 0x40));//�ȴ�TC = 1;Ҳ���Ƿ������
     USART3->SR &= ~(1 << 6);//���������ɱ�־
   }
		
} 
									 
/***************************************
*@DESCRIPTION: --��ʼ������3
*
*@Input: --pclk2:PCLK2ʱ��Ƶ��(Mhz)  bound:������
*
*@Out: --��
*
*@Return: --��
****************************************/	

void UART3_init(uint pclk2,uint bound)
{  	 
	float temp;
	ushort mantissa;
	ushort fraction;	   
	
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	AFIO->MAPR&=0XFFFFFFCF;  //USART3û����ӳ��
  //AFIO->MAPR|=0x00000000;
	
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	
	GPIOB->CRH&=0XFFFF00FF;//IO״̬����
	GPIOB->CRH|=0X00008B00;//IO״̬����
		  
	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1=0X0000200C;  //1λֹͣ,��У��λ.
	//USART3->CR2|=0X200C;  //1λֹͣ,��У��λ.

	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	NVIC_Init(3,3,USART3_IRQn,2);//��2��������ȼ� 

}
