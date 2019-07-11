#include"sys.h"
#include "delay.h"
#include "ds18b20.h"

#define uint unsigned int 
#define uchar unsigned char

#define Ds18b20Data GPIO_Pin_9 //�¶ȴ���������

#define ON 0x01  //��ȡ�ɹ�����0x00��ʧ�ܷ���0x01
#define OFF 0x00

void Ds18b20Write(uchar infor);
uchar Ds18b20Read(void);
int DS18B20_ReadDat( void ); //�¶ȶ�ȡ����

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


void Ds18b20InputInitial(void)//���ö˿�Ϊ����
{
    GPIOB->CRH &=~0X00000F00; 
    GPIOB->CRH |= 0X00000400; 
}

void Ds18b20OutputInitial(void)//���ö˿�Ϊ���
{
    GPIOB->CRH &=~0X00000F00; 
    GPIOB->CRH |= 0X00000300; 
}

//ds18b20��ʼ��
//��ʼ���ɹ�����0x00��ʧ�ܷ���0x01
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
  while((GPIO_ReadInputDataBit(GPIOB,Ds18b20Data) != 0)&&(Flag_1 == ON))//�ȴ�ds18b20��Ӧ�����з�ֹ��ʱ����
  {                                        //�ȴ�Լ60ms����
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
    t <<= 8;//���ֽ�ת��Ϊ10����
    t = t|V1;
    temp = t*0.0625;//ת������Ϊ0.0625/LSB
    t = temp*10+0.5;//����1λС������������****�����10��ԭ��ȷ�¶�ֵ��
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
