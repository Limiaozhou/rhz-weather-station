#include "mem.h"
//#include "string.h"

void mem_clear(unsigned char *pdest, unsigned char len)
{ 
    for(int i=0;i<len; i++)
    {
      *pdest++ = 0;
    }
}

void mem_copy(unsigned char *pdest, unsigned char *psrc,unsigned char len)
{ 
    for( int i=0;i<len; i++)
    {
      *pdest++ = *psrc++;
    }
}


