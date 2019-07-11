#include"sys.h"
#include "delay.h"
#include "ds18b20.h"

#define uint unsigned int 
#define uchar unsigned char

#define Ds18b20Data GPIO_Pin_9 //温度传感器引脚

#define ON 0x01  //读取成功返回0x00，失败返回0x01
#define OFF 0x00

void Ds18b20Write(uchar infor);
uchar Ds18b20Read(void);
int DS18B20_ReadDat( void ); //温度读取函数

void DS18B20_delay1us(uint t)
{
    while(t--)
    {
		__asm{
           NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
					 NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
			     NOP;
				 }
    }
}


void Ds18b20InputInitial(void)//设置端口为输入
{
    GPIOB->CRH &=~0X00000F00; 
    GPIOB->CRH |= 0X00000400; 
}

void Ds18b20OutputInitial(void)//设置端口为输出
{
    GPIOB->CRH &=~0X00000F00; 
    GPIOB->CRH |= 0X00000300; 
}

//ds18b20初始化
//初始化成功返回0x00，失败返回0x01
uchar Ds18b20Initial(void)
{
  uchar Status = 0x00;
  uint CONT_1 = 0;
  uchar Flag_1 = ON;
  Ds18b20OutputInitial();
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(260);
  GPIOB->BRR = Ds18b20Data;
  DS18B20_delay1us(750);
  GPIOB->BSRR = Ds18b20Data;
  Ds18b20InputInitial();
  while((GPIO_ReadInputDataBit(GPIOB,Ds18b20Data) != 0)&&(Flag_1 == ON))//等待ds18b20响应，具有防止超时功能
  {                                        //等待约60ms左右
    CONT_1++;
    DS18B20_delay1us(10);
    if(CONT_1 > 8000)Flag_1 = OFF;
    Status = GPIO_ReadInputDataBit(GPIOB,Ds18b20Data);
  }
  Ds18b20OutputInitial();
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(100);
  return Status;
}


void Ds18b20Write(uchar infor)
{
  uint i;
  Ds18b20OutputInitial();
  for(i=0;i<8;i++)
  {
  if((infor & 0x01))
  {
  GPIOB->BRR = Ds18b20Data;
  DS18B20_delay1us(6);
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(50);
  }
  else
  {
  GPIOB->BRR = Ds18b20Data;
  DS18B20_delay1us(50);
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(6);
  }
  infor >>= 1;
  }
}

uchar Ds18b20Read(void)
{
  uchar Value = 0x00;
  uint i;
  Ds18b20OutputInitial();
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(10);
  for(i=0;i<8;i++)
  {
  Value >>= 1; 
  Ds18b20OutputInitial();
  GPIOB->BRR = Ds18b20Data;
  DS18B20_delay1us(3);
  GPIOB->BSRR = Ds18b20Data;
  DS18B20_delay1us(3);
  Ds18b20InputInitial();
  if(GPIO_ReadInputDataBit(GPIOB,Ds18b20Data) == 1) Value |= 0x80;
  DS18B20_delay1us(15);
  } 
  return Value;
}

int DS18B20_ReadDat( void ) 
{
    uchar V1=0;
    uchar V2=0;
    int t=0;
    float temp = 0;
  
    Ds18b20Initial();
    Ds18b20Write(0xcc);
    Ds18b20Write(0x44);
  
    Ds18b20Initial();
    Ds18b20Write(0xcc);
    Ds18b20Write(0xbe);
 
    V1 = Ds18b20Read();
    V2 = Ds18b20Read();
    t = V2;
    t <<= 8;//高字节转换为10进制
    t = t|V1;
    temp = t*0.0625;//转换精度为0.0625/LSB
    t = temp*10+0.5;//保留1位小数并四舍五入****后面除10还原正确温度值）
	  //if(t>300) t=0;
    return t;
}

float get_Ds18b20(void)
{
	return (float)DS18B20_ReadDat()/10.0;
}
/*
int main()
{
  Ds18b20Initial();
  int temp;
  float value=0;
  while(1)
  {
    temp = DS18B20_ReadDat();
    value = (float)temp/10.0;
  }
}
*/
