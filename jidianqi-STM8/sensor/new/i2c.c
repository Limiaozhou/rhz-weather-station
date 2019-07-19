#include "i2c.h"
//#include "sys.h"
#include"iostm8s103K3.h"

extern void IIC_Read(char *pbuf, char cnt,unsigned char type);


//GPIO_InitTypeDef IIC_InitStructure;
void IIC_init(void)
{
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);
  //3?ê??ˉGPIOB￡?ó?óú?y?ˉLED
/*  IIC_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  IIC_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//í?íìê?3?
  IIC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//×???ê?3??ù?è?a50MHz
  GPIO_Init(GPIOB, &IIC_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_6 | GPIO_Pin_7);*/
	

 
	///////////GAI
    PB_DDR|=0x10;//PB4
    PB_CR1|=0x10;//推挽输出
    PB_CR2&=0xEF;  //2M输出   
    
    PB_DDR|=0x20;//PB4
    PB_CR1|=0x20;//推挽输出
    PB_CR2&=0xDf;  //2M输出  
  /*  GPIOB->CRH &=~0X0000FF00; 
	GPIOB->CRH |= 0X00003300;*/
	
	//GPIOB->CRL &=~0X00F00000;
	//GPIOB->CRL |= 0X00300000;
	// GPIOB->CRH &=~0X000000F0; 
  // GPIOB->CRH |= 0X00000030;
	//////////
	
	
	
//    GPIOB->CRH &=~0X000000F0; 
//	GPIOB->CRH |= 0X00000030;
 
	//GPIOB->BSRR = SDA;
	//GPIOB->BSRR = SCL;
 
 
//   P1SEL &= ~0X18;
//   P1DIR |= 0X18;
}
void IIC_SDA_READ(void)
{
  //3?ê??ˉGPIOB￡?ó?óú?y?ˉLED
/*  IIC_InitStructure.GPIO_Pin = SDA;
  IIC_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &IIC_InitStructure);*/
	
	/////////////GAI
   // GPIOB->CRH &=~0X0000F000; 
   // GPIOB->CRH |= 0X00004000; 
	//GPIOB->CRL &=~0X00F00000;
	//GPIOB->CRL |= 0X00400000;//浮空输入
    PB_DDR &= ~0x20;//PB5
    PB_CR1 &= ~0x20;//
    PB_CR2 &=0xDf;  //
//////////////
}

void IIC_SDA_WRITE(void)
{
  //3?ê??ˉGPIOB￡?ó?óú?y?ˉLED
/*  IIC_InitStructure.GPIO_Pin = SDA;
  IIC_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//í?íìê?3?
  IIC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &IIC_InitStructure);*/
	/////////gai
   // GPIOB->CRH &=~0X0000F000; 
    //GPIOB->CRH |= 0X00003000; 
  
	    //GPIOB->CRL &=~0X00F00000;
	  //  GPIOB->CRL |= 0X00300000;//输出模式2M
    PB_DDR|=0x20;//PB7
    PB_CR1|=0x20;//推挽输出
    PB_CR2 &=0xDf;  //2M输出 
    
	/////////////////
}
