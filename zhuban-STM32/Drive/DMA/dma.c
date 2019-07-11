#include "dma.h"																	   	  
#include "delay.h"																	   	  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DMA ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
void MYDMA_Config_IFCG(DMA_Channel_TypeDef* DMA_CHx)
{
  if ((DMA_CHx == DMA1_Channel1) || (DMA_CHx == DMA2_Channel1))
  {
    /* Reset interrupt pending bits for DMA1 Channel1 */
    DMA1->IFCR |= ((uint32_t)0x00000001)|((uint32_t)0x00000002)|((uint32_t)0x00000004)|((uint32_t)0x00000008);
  }
  else if ((DMA_CHx == DMA1_Channel2) || (DMA_CHx == DMA2_Channel2))
  {
    /* Reset interrupt pending bits for DMA1 Channel2 */
    DMA1->IFCR |= ((uint32_t)0x00000010)|((uint32_t)0x00000020)|((uint32_t)0x00000040)|((uint32_t)0x00000080);
  }
  else if ((DMA_CHx == DMA1_Channel3) || (DMA_CHx == DMA2_Channel3))
  {
    /* Reset interrupt pending bits for DMA1 Channel3 */
    DMA1->IFCR |= ((uint32_t)0x00000100)|((uint32_t)0x00000200)|((uint32_t)0x00000400)|((uint32_t)0x00000800);
  }
  else if ((DMA_CHx == DMA1_Channel4) || (DMA_CHx == DMA2_Channel4))
  {
    /* Reset interrupt pending bits for DMA1 Channel4 */
    DMA1->IFCR |= ((uint32_t)0x00001000)|((uint32_t)0x00002000)|((uint32_t)0x00004000)|((uint32_t)0x00008000);
  }
  else if ((DMA_CHx == DMA1_Channel5) || (DMA_CHx == DMA2_Channel5))
  {
    /* Reset interrupt pending bits for DMA1 Channel5 */
    DMA1->IFCR |= ((uint32_t)0x00010000)|((uint32_t)0x00020000)|((uint32_t)0x00040000)|((uint32_t)0x00080000);
  }
  else if (DMA_CHx == DMA1_Channel6)
  {
    /* Reset interrupt pending bits for DMA1 Channel6 */
    DMA1->IFCR |= ((uint32_t)0x00100000)|((uint32_t)0x00200000)|((uint32_t)0x00400000)|((uint32_t)0x00800000);
  }
  else if (DMA_CHx == DMA1_Channel7)
  {
    /* Reset interrupt pending bits for DMA1 Channel7 */
    DMA1->IFCR |= ((uint32_t)0x01000000)|((uint32_t)0x02000000)|((uint32_t)0x04000000)|((uint32_t)0x08000000);
  }
	else if (DMA_CHx == DMA2_Channel3)
	{
		DMA2->IFCR |= ((uint32_t)0x00000100)|((uint32_t)0x00000200)|((uint32_t)0x00000400)|((uint32_t)0x00000800);
	}
}


u16 DMA_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 		    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ�����  
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	if(DMA_CHx==DMA2_Channel3)
		RCC->AHBENR|=1<<1;			//����DMA2ʱ��
	else
		RCC->AHBENR|=1<<0;			//����DMA1ʱ��
	MYDMA_Config_IFCG(DMA_CHx);
	delay_ms(5);				//�ȴ�DMAʱ���ȶ�
	DMA_CHx->CPAR=cpar; 	 	//DMA1 �����ַ 
	DMA_CHx->CMAR=(u32)cmar; 	//DMA1,�洢����ַ
	DMA_MEM_LEN=cndtr;      	//����DMA����������
	DMA_CHx->CNDTR=cndtr;    	//DMA1,����������
	DMA_CHx->CCR=0X00000000;	//��λ
	DMA_CHx->CCR|=0<<4;  		//�Ӵ洢����
	DMA_CHx->CCR|=1<<5;  		//��ͨģʽ
	DMA_CHx->CCR|=0<<6; 		//�����ַ������ģʽ
	DMA_CHx->CCR|=1<<7; 	 	//�洢������ģʽ
	DMA_CHx->CCR|=0<<8; 	 	//�������ݿ��Ϊ8λ
	DMA_CHx->CCR|=0<<10; 		//�洢�����ݿ��8λ
	DMA_CHx->CCR|=11<<12; 		//�е����ȼ�
	DMA_CHx->CCR|=0<<14; 		//�Ǵ洢�����洢��ģʽ	
	DMA_CHx->CCR|=1<<0;			//����DMA����
} 


//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_CHx->CCR&=~(1<<0);       //�ر�DMA���� 
	DMA_CHx->CNDTR=DMA_MEM_LEN; //DMA1,���������� 
	DMA_CHx->CCR|=1<<0;          //����DMA����
}	  

uint16_t MYDMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_CHx)
{
	return ((uint16_t)(DMA_CHx->CNDTR));
}
 

























