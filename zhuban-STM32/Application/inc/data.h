#ifndef _DATA_H_
#define _DATA_H_

/******************@DESCRIPTION: -- �������ݽṹ�� *************/

typedef struct
{
  unsigned char key[2]; //
  unsigned char dat[4];
}ST_ITEM;

//��������ź�����
//typedef struct
//{
//	unsigned char num[2]; //���������õĵ�ַ
//	unsigned char dat[4];
//}ST_ITEM;
/**********@DESCRIPTION: -- ��������ź����ݣ�cmdָ����ȣ�cmd��D0Ҫ���������ϴ�����ʷ������û��̨��֪������ϸ��*************/
typedef struct
{
  unsigned char cmd; //
  unsigned char len; //���ȣ������ֽ�����6 * n

//  ST_ITEM shade;
//    ST_ITEM co2;  
//    ST_ITEM pm25; 
//    ST_ITEM ultvio;//������
//    ST_ITEM temp; 
//    ST_ITEM light; 
//    ST_ITEM humi;
//    ST_ITEM tvoc;
//    ST_ITEM fx;  //����
//    ST_ITEM fs;  //����
//    ST_ITEM so2;  //so2
//    ST_ITEM pressure;  //��ѹ
//    ST_ITEM yuxue;  //�Ƿ�����ѩ
    
    ST_ITEM ventilate;  //���
//    ST_ITEM juanmo;  //��Ĥ
//    ST_ITEM wat4;  //ˮ��
//    ST_ITEM yang;  //�ڣ�����1
//    ST_ITEM yang1;  //�⣬����2
    ST_ITEM UA;
    ST_ITEM UB;
    ST_ITEM UC;
    ST_ITEM IA;
    ST_ITEM IB;
    ST_ITEM IC;
    ST_ITEM IL;
    ST_ITEM TA;
    ST_ITEM TB;
    ST_ITEM TC;
    ST_ITEM TN;
    ST_ITEM TE;
}DATA;
//Ҫ���͵����ݽṹ��
typedef struct
{
	short int head;  //2��֡ͷ
	unsigned char len;   //1�����ȣ������ֽ��������������ֽ�
	char ver;   //1�������Ƿ���ܱ�־λ
	unsigned char addr[8]; // device code,8���豸��ַ
	DATA data;     //����������,2 + 6 * n
	char fill[2];  //2
	short int crc16;     //crc16,2����ver��ʼУ��
}ST_EVNBALEDAT;
extern ST_EVNBALEDAT *psenddat;
/******************@DESCRIPTION: -- ��⻷�������������*************/
typedef struct
{
//  float shade ; //shade ����
//  float co2; //
//  float pm25;//PM2.5
//  float ultvio;//������
//  float light;//����
//  float temp;//�¶�
//  float humi;//ʪ��
//  float tvoc;//tvoc
//  float fx;//����
//  float fs;//����
//  float so2;  //so2
//  float pressure;  //��ѹ
//  float yuxue;  //�Ƿ�����ѩ
  float UA;  //A���ѹ
  float UB;
  float UC;
  float IA;  //A�����
  float IB;
  float IC;
  float IL;  //©����
  float TA;  //A���¶�
  float TB;
  float TC;
  float TN;
  float TE;  //�����¶�
}SENSORTYPE;
extern void send();
extern void package_send(unsigned char *pdat, unsigned char len, unsigned char flag,unsigned char flag1);
extern SENSORTYPE sensor;
#endif
