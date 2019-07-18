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
  unsigned char len; //data  �ؼ���

//  ST_ITEM shade;
//    ST_ITEM co2;  
//    ST_ITEM pm25; 
//    ST_ITEM ultvio;//������
    ST_ITEM temp; 
    ST_ITEM light; 
    ST_ITEM humi;
//    ST_ITEM tvoc;
    ST_ITEM fx;  //����
    ST_ITEM fs;  //����
//    ST_ITEM so2;  //so2
//    ST_ITEM pressure;  //��ѹ
    ST_ITEM yuxue;  //�Ƿ�����ѩ
}DATA;
//Ҫ���͵����ݽṹ��
typedef struct
{
	short int head;  //
	unsigned char len;   //
	char ver;   //
	unsigned char addr[8]; // device code
	DATA data;     //����������
	char fill[2];
	short int crc16;     //crc16
}ST_EVNBALEDAT;
extern ST_EVNBALEDAT *psenddat;
/******************@DESCRIPTION: -- ��⻷�������������*************/
typedef struct
{
//  float shade ; //shade ����
  float co2; //
  float pm25;//PM2.5
  float ultvio;//������
  float light;//����
  float temp;//�¶�
  float humi;//ʪ��
  float tvoc;//tvoc
  float fx;//����
  float fs;//����
//  float so2;  //so2
  float pressure;  //��ѹ
  float yuxue;  //�Ƿ�����ѩ
}SENSORTYPE;
extern void send();
extern void package_send(unsigned char *pdat, unsigned char len, unsigned char flag,unsigned char flag1);
extern SENSORTYPE sensor;
#endif
