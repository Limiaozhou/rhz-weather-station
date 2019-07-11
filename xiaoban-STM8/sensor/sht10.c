//#include "sys.h"
#include"iostm8s103F3.h"
#include "sht10.h"
#include "delay.h"
#include "i2c.h"
#define noACK 0
#define ACK 1

#define STATUS_REG_W 0x06
#define STATUS_REG_R 0x07
#define MEASURE_TEMP 0x03
#define MEASURE_HUMI 0x05
#define RESET 0x1e
//GPIO_InitTypeDef sht10_InitStructure;

//#define SCL GPIO_Pin_6 //SHT10??
//#define SDA GPIO_Pin_7 //SHT10???


unsigned char d1,d2,d3,d4,d5,d6,d7;
char s_write_byte(unsigned char value);
char s_read_byte(unsigned char ack);
void s_transstart(void);
void s_connectionreset(void);
char s_measure( unsigned char *p_checksum, unsigned char mode);
void sht10_init(void);

/**************************************************************************************************
 * 代码说明：SHT10采集数据代码 延时数值已做相应修改
* 代码来源：移植于STM32工程
 *移植目的：修复旧代码获取的温度总是偏高，湿度偏低的问题
 * 移植人：王志强
 * 移植日期：2016.11.10
**************************************************************************************************/
/*
void sht10_init(void)
{
 //打开相应外设的时钟：GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);
  //初始化GPIOB，用于驱动LED
  sht10_InitStructure.GPIO_Pin = SCL |SDA;
  sht10_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
  sht10_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//最高输出速度为50MHz
  GPIO_Init(GPIOB, &sht10_InitStructure);
//  GPIO_SetBits(GPIOB,GPIO_Pin_0);GPIO_SetBits(GPIOB,GPIO_Pin_1);GPIO_SetBits(GPIOB,GPIO_Pin_2);
  GPIO_SetBits(GPIOB,SDA );
  GPIO_ResetBits(GPIOB,SCL);
}
*/

void DAT_R()
{
  //sht10_InitStructure.GPIO_Pin = SDA;
  //sht10_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //GPIO_Init(GPIOB, &sht10_InitStructure);
  
   PB_DDR &= ~0x20;//PB5
   PB_CR1 |= 0x20;//
   PB_CR2 &=0xDf;  //
}
void DAT_W()
{
  //sht10_InitStructure.GPIO_Pin = SDA;
  //sht10_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  //GPIO_Init(GPIOB, &sht10_InitStructure);
    PB_DDR|=0x20;//PB5
    PB_CR1|=0x20;//推挽输出
    PB_CR2 &=0xDf;  //2M输出 
  
}
/**************************************************************************************************
 * ????:s_write_byte
 *
 * ????:?SHT10????? *
 * ??:value-??????? *
 * ???:error--??????
 **************************************************************************************************/
char s_write_byte(unsigned char value)
{  
  unsigned char i,error=1; 
  DAT_W();  
  for (i=0x80;i>0;i/=2) //??????8?????
  {
    if (i & value)
      //GPIO_SetBits(GPIOB,SDA);  
      SDA=1;
    else
      //GPIO_ResetBits(GPIOB,SDA);
       SDA=0;
    //GPIO_SetBits(GPIOB,SCL);    
    SCL=1;
    delay_2us(5);
    //GPIO_ResetBits(GPIOB,SCL);  
    SCL=0;
    delay_2us(5);//
  }
  //GPIO_SetBits(GPIOB,SDA);  
   SDA=1;
   DAT_R();
  //GPIO_SetBits(GPIOB,SCL); 
   SCL=1;
  delay_2us(5);
  //error = GPIO_ReadInputDataBit(GPIOB,SDA); 
  error=SDAIN;
  delay_2us(12);
  DAT_W();
  //GPIO_ResetBits(GPIOB,SCL); 
  SCL=0;
  return error;   
}
/**************************************************************************************************
 * ????:s_read_byte
 *
 *????:?SHT10??????
 * ??:ack--?????,?SHT10??ACK
 *
 * ???:val--??????
**************************************************************************************************/
char s_read_byte(unsigned char ack)
{
  unsigned char i,val=0;
  DAT_W(); 	
  //GPIO_SetBits(GPIOB,SDA);  
  SDA =1;
  DAT_R();
  for (i=0x80;i>0;i/=2)  
  {
    //GPIO_SetBits(GPIOB,SCL);
    SCL =1;
   //if (GPIO_ReadInputDataBit(GPIOB,SDA))
    if (SDAIN)
      val = (val | i);
   // GPIO_ResetBits(GPIOB,SCL);
    SCL=0;
  }
  DAT_W();
  if(ack)
    //GPIO_ResetBits(GPIOB,SDA);
    SDA=0;
  else
   // GPIO_SetBits(GPIOB,SDA);  
    SDA=1;
//  SDA = !ack;
  //GPIO_SetBits(GPIOB,SCL); 
  SCL=1;
  delay_2us(4);
  //GPIO_ResetBits(GPIOB,SCL); 
  SCL=0;
  delay_2us(4);
  //GPIO_SetBits(GPIOB,SCL);  
  SCL=1;
   
  return val; //??????
}
/**************************************************************************************************
 *????:s_transstart
 *
 * ????:??SHT10,???SHT10??
 *
 * ??:? 
 *
 * ???:?
**************************************************************************************************/
void s_transstart(void)
{
  DAT_W(); 
  //GPIO_SetBits(GPIOB,SDA);  
  SDA=1;
  //GPIO_ResetBits(GPIOB,SCL);  
  SCL=0;
  delay_2us(2);
  //GPIO_SetBits(GPIOB,SCL); delay_us(2);
  SCL=1;delay_2us(4);
  //GPIO_ResetBits(GPIOB,SDA);  delay_us(2);
  SDA=0; delay_2us(4);
 // GPIO_ResetBits(GPIOB,SCL);  delay_us(2);
  SCL=0; delay_2us(4);
  //GPIO_SetBits(GPIOB,SCL);  delay_us(2);
  SCL=1;delay_2us(4);
  //GPIO_SetBits(GPIOB,SDA);delay_us(2);
  SDA =1;delay_2us(4);
  //GPIO_ResetBits(GPIOB,SCL);delay_us(2);
  SCL =0;delay_2us(4);
}
/**************************************************************************************************
 * ????:s_connectionreset
 *
 * ????:?SHT10????
 *
 * ??:?
 * ???:?
**************************************************************************************************/
void s_connectionreset(void)
{
  unsigned char i;
  DAT_W(); 
 // GPIO_SetBits(GPIOB,SDA);
  SDA=1;
 // GPIO_ResetBits(GPIOB,SCL);
  SCL=0;
  for(i=0;i<9;i++)
  {
   // GPIO_SetBits(GPIOB,SCL);delay_us(2);
    SCL=1;delay_2us(4);
    //GPIO_ResetBits(GPIOB,SCL);delay_us(2);
    SCL =0;delay_2us(4);
  }
  s_transstart();
}
/**************************************************************************************************
 * ????:s_measure
 *
 * ????:????,??SHT10???????
 * ??:*p_checksum--???
 * mode -- ??????(3???,5???)
 *
 * ???:er--????
 **************************************************************************************************/
char s_measure( unsigned char *p_checksum, unsigned char mode)
{
  unsigned er=0;
  s_transstart(); //????
  switch(mode)
  {
    case 3:er+=s_write_byte(MEASURE_TEMP);break; //????????
    case 5:er+=s_write_byte(MEASURE_HUMI);break; //????????
    default :break;
  }
  DAT_R();
  delay_ms(500);

  //if(GPIO_ReadInputDataBit(GPIOB,SDA)) //SDA????,?????1 
  if(SDAIN)
  {
    er += 1;
  }
  d1 = s_read_byte(ACK); //???? ?????? ???(MSB)
  d2 = s_read_byte(ACK);//?????? ???(LSB)
  d3 = s_read_byte(noACK);//resd CRC???
  return er;
}
/**************************************************************************************************
 * ????:th_read
 *
 * ????:??????,?????????????
 *
 * ??:*t--???
 **h--???
 *???:?
**************************************************************************************************/

void get_HT(float *tem, float *rh)
{
  unsigned char error,checksum;
  float humi,temp;
  s_connectionreset(); //????
  error=0;
  error+=s_measure(&checksum,MEASURE_HUMI); //?????????
  humi = d1*256+d2;
    
  error+=s_measure(&checksum,MEASURE_TEMP); //?????????
  temp = d1*256+d2;
  if(error!=0) 
  {
    *tem = 0.0;
    *rh = 0.0;
  }
  else //????,????
  {   
    temp = temp*0.01 - 39.6 ;
    humi = (temp - 25) * (0.01 + 0.00008 * humi) -0.0000028 * humi * humi + 0.0405 * humi-4;
    if(humi>100)  humi = 100;
    if(humi<0.1)  humi = 0.1;
		if(temp>100.0||temp<-40.0)	temp=0.0;
    *tem = temp;
    *rh = humi;
  }
  
} 

