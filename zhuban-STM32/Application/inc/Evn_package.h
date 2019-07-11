#ifndef _EVN_PACKAGE_H_
#define _EVN_PACKAGE_H_

typedef struct
{
  unsigned char type; //
  unsigned char dat[2];
}ST_ITEMTWO;

typedef struct
{
  unsigned char type; //
  unsigned char dat[3];
}ST_ITEMTHREE;

typedef struct
{
  unsigned char type; //
  unsigned char dat[5];
}ST_ITEMFIVE;

typedef struct
{
  unsigned char type; //
  unsigned char dat[6];
}ST_ITEMTVOC;

typedef struct
{
  short head;
  unsigned char len;   //
  unsigned char addr[8];//  8
  ST_ITEMTWO temp; //ÎÂ¶È 3
  ST_ITEMTWO humi; //Êª¶È 3
  ST_ITEMFIVE co2; //CO2  6
  ST_ITEMTWO hcho; //¼×È© 3
  ST_ITEMTVOC tvoc;//7
  ST_ITEMTHREE pm25;//4
  ST_ITEMTHREE pm10;//4
  ST_ITEMTWO voice; //3
//  ST_ITEMTHREE light;//4
//  ST_ITEMTHREE co;//4//45
  unsigned char dat[3];//
  short crc16;
}ST_EVNBALEDAT;

extern void Evn_Packagedat(ST_EVNBALEDAT *pdat);

#endif
