#ifndef DATA_H
#define DATA_H

#define u16 unsigned short int
#define u8  unsigned char 
#define u32 unsigned long 

typedef struct
{
  float co2;    // CO2
  float TVOC;   //TVOC            δ��
  float tvocco2;//                δ��      
  float temp20; // �����¶�         
  float humi20; //   ����ʪ��
  float press;  //    ��ѹ
  float press_temp;   //         �¶�
  
  float num1;
  float num2;
  float num3;
  float num4;
  float num5;
  float num6;
  float num7;
  float num8;
  float num9;
  
  float light_bhvi;  //����ǿ�� 
}EVNDAT;


extern short int collect_time;
extern EVNDAT Evndat;	//�������ݽṹ��
extern void get_data(void);
//#define u16 unsigned long
#endif