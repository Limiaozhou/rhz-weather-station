#ifndef _control_H_
#define _control_

#include"iostm8s103F3.h"

#define valve1io PC_ODR_ODR7  //PC7ㄛ導PC4ㄛCR1
#define valve2io PC_ODR_ODR6  //PC6ㄛ導PC5ㄛCR2
#define valve3io PC_ODR_ODR4  //PC4ㄛ導PC6ㄛCR3
#define valve4io PC_ODR_ODR5  //PC5ㄛCR4
#define valve5io PA_ODR_ODR3  //PA3ㄛ導PC7ㄛCR5
typedef struct
{
  float water1 ;
  float water2 ;
  float water1time ;
  float water2time ;
  float valve4flow ;
  float time;
}CON;
extern char valve1 ;
extern char valve2 ;
extern char valve3 ;
extern char valve4 ;
extern char valve5 ;
extern char read485;
extern CON  WAT;

void struct_init(void);
void io_init(void);
void control(void);
void io_control(void);
void wat_data(void);
void pwm_on(void);
void pwm_off(void);
#endif