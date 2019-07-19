#ifndef _SHT10_H_
#define _SHT10_H_
#if 0
extern void get_HT(unsigned char humiture[4]);
#else
extern void get_HT(float *tem, float *rh);
#endif
extern void sht10_init(void);


#endif

