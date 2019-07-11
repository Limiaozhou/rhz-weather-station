#ifndef _LIGHT_BHVI_H_
#define _LIGHT_BHVI_H_


#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07

extern float BH1750_ReadDat(void);

#endif
