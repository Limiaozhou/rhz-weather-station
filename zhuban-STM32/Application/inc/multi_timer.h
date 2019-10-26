/*
 * 	  	multi_timer.h
 *
 *      Created on: 20161229
 *      @Author   : 晓宇 <karaxiaoyu@gmail.com>
 *		@id       : 芯片之家
 *      @version  ：V1.0.0
 */
 
#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include "stdint.h"
#include "stdio.h"

typedef struct Timer {
    uint32_t timeout;  //定时
    uint32_t repeat;  //重复时间间隔
    void (*timeout_cb)(void);
    struct Timer* next;
}Timer;

#ifdef __cplusplus  
extern "C" {  
#endif  

void timer_init(struct Timer* handle, void(*timeout_cb)(), uint32_t timeout, uint32_t repeat);
int  timer_start(struct Timer* handle);
void timer_stop(struct Timer* handle);
void timer_ticks(void);
void timer_loop(void);

// void timer_again(struct Timer* handle);
// void timer_set_repeat(struct Timer* handle, uint32_t repeat);

#ifdef __cplusplus
} 
#endif

#endif
