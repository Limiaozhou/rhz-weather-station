#ifndef _CONFIG_H_
#define _CONFIG_H_

#define         CONFIGCOUNT     20     //��������

typedef  struct DBPPMConfig
{
   float Voltage;
   float dbppm;
}ST_DBPPMConfig;

typedef struct Config
{
   char addr[8];
   char type;     //0 û������ 1�������
   char count;    //�Ѿ���������
   ST_DBPPMConfig group[CONFIGCOUNT];
   int crc;
}ST_Config;

#define         DEVTYPE     0x41     //����

//��ȡ���ò���
extern void Config_Read(void);
//�������ò���
extern void Config_Send(void);
//���У׼��
extern void Config_Add(float dat);
//����ʵ�����
extern void Config_Deal(void);

extern void Config_Save(void);

extern void Config_DealADCMaxAve(void);

extern void Config_Init(void);

#endif


