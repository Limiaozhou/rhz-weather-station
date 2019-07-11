#include "stm32f10x.h"
#ifndef AIR_COND_SWI
#define AIR_COND_SWI



/*??2?·?oê?¨ò??Yê±?′ó?μ?
#define HUMIDITY_OUT1  55   //μ±êòíaêa?è3?1y???μ??3￡?a3yêa?÷￡???1|?üD?′ò?a?àó|μ?1|?ü?a1?￡¨humidity_flag55??1￡?
#define HUMIDITY_OUT2  70   //μ±êòíaêa?è3?1y???μ??3￡?a3yêa?÷￡??T1|?ü?a1?
#define HUMIDITY_IN1  65    //μ±ê±êò?ú???è?úHUMIDITY_IN1oíHUMIDITY_IN2????3yêa?÷1¤×÷2D?ê±1?°?D?ê±
#define HUMIDITY_IN2  55 
#define TEMP_IN1 40  //êò?ú???è′óóú???μ??3￡?a??à?
#define TEMP_IN2 30  //êò?ú???è?úTEMP_IN2oíTEMP_IN3????ê±??à??a°?D?ê±1?°?D?ê±
#define TEMP_IN3 40  
#define TEMP_IN4 -10  //êò?ú???è?úTEMP_IN4oíTEMP_IN5????ê±?óèè?a°?D?ê±1?°?D?ê±
#define TEMP_IN5 10
#define TEMP_IN6 10 //êò?ú???è?úTEMP_IN6oíTEMP_IN7????ê±1?±??óèèoí??à?
#define TEMP_IN7 30

#define TIME_HUMI1   36000   // μ±ê±êò?ú???è?úHUMIDITY_IN1oíHUMIDITY_IN2????3yêa?÷1¤×÷TIME_HUMI1ê±??1?TIME_HUMI2ê±??
#define TIME_HUMI2   9000    
#define TIME_TEMP1   9000    //êò?ú???è?úTEMP_IN2oíTEMP_IN3????ê±??à??aTIME_TEMP11?TIME_TEMP1
#define TIME_TEMP2   9000    //êò?ú???è?úTEMP_IN4oíTEMP_IN5????ê±??à??aTIME_TEMP21?TIME_TEMP2
*/


#define AIR_CON_OUT  PBout(6)//制冷装置控制管脚
#define HUMIDITY_OUT  PBout(7)//除湿装置控制管脚
#define HEATER_OUT  PBout(0)//加热装置控制管脚

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

extern int  humidity_time;//除湿器开关状态计时器
extern char humidity_flag55;//用来控制是否开启大于55便敞开除湿器功能
extern int air_con_time30;//室内温度在30到40度之间的状态计时器
extern int air_con_time10;//室内温度在-10到10度之间的状态计时器
#endif
