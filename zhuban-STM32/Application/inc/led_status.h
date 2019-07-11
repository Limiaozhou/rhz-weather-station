#ifndef _LED_STASTUS_H_
#define _LED_STASTUS_H_
#include "data.h"
//BEGIN为开始采集，P2_0引脚是否打开
#define STOP 1
//#define BEGIN 1
#define BEGIN 0

#define REDLIGHT     1
#define GREENLIGHT   2
#define YELLOWLIGHT  3

#define BULECONFIGOPEN   1
#define BULECONFIGCOLSE  2

//N表示N小时后让Wifi重启
#define N 0

extern void led_light(EVNDAT *data);
extern void init_ledshow(void);

#endif
