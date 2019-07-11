#ifndef _LED_H_
#define _LED_H_

#define REDLIGHT     1
#define GREENLIGHT   2
#define YELLOWLIGHT  3

#define GREENLED  GPIO_Pin_14
#define REDLED    GPIO_Pin_12
#define YELLOWLED  GPIO_Pin_13

void led_init(void);
void led_set(unsigned char stat);

#endif
