#ifndef _CONTROL_H_
#define _CONTROL_H_

#define WATER 0
#define FER   1

extern char get_Cmd(void);
extern void set_Cmd(char cmd);
extern void Begin_fer(void);
extern void Close_fer(void);
extern void Clean_buck(void);
extern void control_relay(unsigned char status, unsigned char Num);
extern void Init_control(void);
extern void Protect(void);
extern char get_WL(void);

#endif
