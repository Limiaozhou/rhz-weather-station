#ifndef _LED_STASTUS_H_
#define _LED_STASTUS_H_
#include "data.h"
//BEGINΪ��ʼ�ɼ���P2_0�����Ƿ��
#define STOP 1
//#define BEGIN 1
#define BEGIN 0

#define REDLIGHT     1
#define GREENLIGHT   2
#define YELLOWLIGHT  3

#define BULECONFIGOPEN   1
#define BULECONFIGCOLSE  2

//N��ʾNСʱ����Wifi����
#define N 0

extern void led_light(EVNDAT *data);
extern void init_ledshow(void);

#endif
