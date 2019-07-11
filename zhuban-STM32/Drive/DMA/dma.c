#include "dma.h"																	   	  
#include "delay.h"																	   	  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DMA 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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


u16 DMA_MEM_LEN;//保存DMA每次数据传送的长度 		    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量  
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	if(DMA_CHx==DMA2_Channel3)
		RCC->AHBENR|=1<<1;			//开启DMA2时钟
	else
		RCC->AHBENR|=1<<0;			//开启DMA1时钟
	MYDMA_Config_IFCG(DMA_CHx);
	delay_ms(5);				//等待DMA时钟稳定
	DMA_CHx->CPAR=cpar; 	 	//DMA1 外设地址 
	DMA_CHx->CMAR=(u32)cmar; 	//DMA1,存储器地址
	DMA_MEM_LEN=cndtr;      	//保存DMA传输数据量
	DMA_CHx->CNDTR=cndtr;    	//DMA1,传输数据量
	DMA_CHx->CCR=0X00000000;	//复位
	DMA_CHx->CCR|=0<<4;  		//从存储器读
	DMA_CHx->CCR|=1<<5;  		//普通模式
	DMA_CHx->CCR|=0<<6; 		//外设地址非增量模式
	DMA_CHx->CCR|=1<<7; 	 	//存储器增量模式
	DMA_CHx->CCR|=0<<8; 	 	//外设数据宽度为8位
	DMA_CHx->CCR|=0<<10; 		//存储器数据宽度8位
	DMA_CHx->CCR|=11<<12; 		//中等优先级
	DMA_CHx->CCR|=0<<14; 		//非存储器到存储器模式	
	DMA_CHx->CCR|=1<<0;			//开启DMA传输
} 


//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_CHx->CCR&=~(1<<0);       //关闭DMA传输 
	DMA_CHx->CNDTR=DMA_MEM_LEN; //DMA1,传输数据量 
	DMA_CHx->CCR|=1<<0;          //开启DMA传输
}	  

uint16_t MYDMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_CHx)
{
	return ((uint16_t)(DMA_CHx->CNDTR));
}
 

























