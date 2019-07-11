#include "stm32f10x.h"
#ifndef AIR_COND_SWI
#define AIR_COND_SWI



/*??2?��?o��?����??Y����?�䨮?��?
#define HUMIDITY_OUT1  55   //�̡�������a��a?��3?1y???��??3��?a3y��a?�¡�???1|?��D?�䨰?a?����|��?1|?��?a1?�ꡧhumidity_flag55??1��?
#define HUMIDITY_OUT2  70   //�̡�������a��a?��3?1y???��??3��?a3y��a?�¡�??T1|?��?a1?
#define HUMIDITY_IN1  65    //�̡���������?��???��?��HUMIDITY_IN1o��HUMIDITY_IN2????3y��a?��1�����2D?����1?��?D?����
#define HUMIDITY_IN2  55 
#define TEMP_IN1 40  //����?��???���䨮����???��??3��?a??��?
#define TEMP_IN2 30  //����?��???��?��TEMP_IN2o��TEMP_IN3????����??��??a��?D?����1?��?D?����
#define TEMP_IN3 40  
#define TEMP_IN4 -10  //����?��???��?��TEMP_IN4o��TEMP_IN5????����?������?a��?D?����1?��?D?����
#define TEMP_IN5 10
#define TEMP_IN6 10 //����?��???��?��TEMP_IN6o��TEMP_IN7????����1?��??������o��??��?
#define TEMP_IN7 30

#define TIME_HUMI1   36000   // �̡���������?��???��?��HUMIDITY_IN1o��HUMIDITY_IN2????3y��a?��1�����TIME_HUMI1����??1?TIME_HUMI2����??
#define TIME_HUMI2   9000    
#define TIME_TEMP1   9000    //����?��???��?��TEMP_IN2o��TEMP_IN3????����??��??aTIME_TEMP11?TIME_TEMP1
#define TIME_TEMP2   9000    //����?��???��?��TEMP_IN4o��TEMP_IN5????����??��??aTIME_TEMP21?TIME_TEMP2
*/


#define AIR_CON_OUT  PBout(6)//����װ�ÿ��ƹܽ�
#define HUMIDITY_OUT  PBout(7)//��ʪװ�ÿ��ƹܽ�
#define HEATER_OUT  PBout(0)//����װ�ÿ��ƹܽ�

void air_con_dehu_init();
void air_con_open();
void air_con_close();
void humidity_open();
void humidity_close();
void heater_open();
void heater_close();
int humidity_control(float value_in,float value_out);
int air_con_control(float value);
int temp_humi_cont(void);

extern int  humidity_time;//��ʪ������״̬��ʱ��
extern char humidity_flag55;//���������Ƿ�������55�㳨����ʪ������
extern int air_con_time30;//�����¶���30��40��֮���״̬��ʱ��
extern int air_con_time10;//�����¶���-10��10��֮���״̬��ʱ��
#endif
