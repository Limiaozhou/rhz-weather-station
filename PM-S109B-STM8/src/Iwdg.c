#include "Iwdg.h"
#include"iostm8s103F3.h"
void IWDG_init(void)
{
  IWDG_KR = 0xCC; //启动IWDG    
  IWDG_KR = 0x55; //解除 PR 及 RLR 的写保护    
  IWDG_RLR = 0xff; //看门狗计数器重装载数值          
  IWDG_PR = 0x06; //分频系数为64 1.02s    
  IWDG_KR = 0xAA; //刷新IDDG，避免产生看门狗复位，同时恢复 PR 及 RLR 的写保护状态 
}