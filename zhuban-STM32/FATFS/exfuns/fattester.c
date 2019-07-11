#include "fattester.h"	 
#include "sdio_sdcard.h"
#include "gprs.h"
#include "usart.h"
#include "exfuns.h"
#include "malloc.h"		  
#include "ff.h"
//#include "string.h"
#define READ_LEN 256
//Ϊ����ע�Ṥ����
//drv:�̷�
//����ֵ:ִ�н��
u8 mf_mount(u8 drv)
{
  return f_mount(drv,fs[drv]);
}
//��·���µ��ļ�
//path:·��+�ļ���
//mode:��ģʽ
//����ֵ:ִ�н��
u8 mf_open(u8*path,u8 mode)
{
  u8 res;	 
  res=f_open(file,(const TCHAR*)path,mode);//���ļ���
  return res;
} 
//�ر��ļ�
//����ֵ:ִ�н��
u8 mf_close(void)
{
  f_close(file);
  return 0;
}
//��������
//len:�����ĳ���
//����ֵ:ִ�н��
/*void send_SD_data(unsigned char num)
{
switch (num)
{
		case 1:
USART_Puts(USART1,fatbuf,br);
break;
		case 2:
//send_3gData(fatbuf,br);
break;
default:
break;
	}
}*/
u8 mf_read(u16 len)
{
  u16 i;
  u8 res=0;
  for(i=0;i<len/READ_LEN;i++)
  {
    res=f_read(file,fatbuf,READ_LEN,&br);
    if(!res)
    {
      USART_Puts(USART1,fatbuf,br);
      if(Is_Connect())
      {
        send_3gData(fatbuf,br);
      }
    }
  }
  if(len%READ_LEN)
  {
    res=f_read(file,fatbuf,len%READ_LEN,&br);
    if(!res)	//�����ݳ�����
    {
      USART_Puts(USART1,fatbuf,br);
      if(Is_Connect())
      {
        send_3gData(fatbuf,br);
      }
    }	 
  }
  return res;
}
//д������
//dat:���ݻ�����
//len:д�볤��
//����ֵ:ִ�н��
u8 mf_write(u8*dat,u16 len)
{			    
  return f_write(file,dat,len,&bw);
}

//���ļ���
//path:·��
//����ֵ:ִ�н��
u8 mf_opendir(u8* path)
{
  return f_opendir(&dir,(const TCHAR*)path);	
}
//���ȡ�ļ���
//����ֵ:ִ�н��
u8 mf_readdir(void)
{
  u8 res;
  char *fn;			 
#if _USE_LFN
  fileinfo.lfsize = _MAX_LFN * 2 + 1;
  fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
  res=f_readdir(&dir,&fileinfo);//��ȡһ���ļ�����Ϣ
  if(res!=FR_OK||fileinfo.fname[0]==0)
  {
    myfree(SRAMIN,fileinfo.lfname);
    return res;//������.
  }
#if _USE_LFN
  fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
  fn=fileinfo.fname;;
#endif	
  myfree(SRAMIN,fileinfo.lfname);
  return 0;
}			 

//�����ļ�
//path:·��
//����ֵ:ִ�н��
u8 mf_scan_files(u8 * path)
{
  FRESULT res;	  
  char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
  fileinfo.lfsize = _MAX_LFN * 2 + 1;
  fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif		  
  
  res = f_opendir(&dir,(const TCHAR*)path); //��һ��Ŀ¼
  if (res == FR_OK) 
  {	
    //	printf("\r\n"); 
    while(1)
    {
      res = f_readdir(&dir, &fileinfo);                   //��ȡĿ¼�µ�һ���ļ�
      if (res != FR_OK || fileinfo.fname[0] == 0) break;  //������/��ĩβ��,�˳�
      //if (fileinfo.fname[0] == '.') continue;             //�����ϼ�Ŀ¼
#if _USE_LFN
      fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
      fn = fileinfo.fname;
#endif	                                              /* It is a file. */
    } 
  }	  
  myfree(SRAMIN,fileinfo.lfname);
  return res;	  
}
//��ʾʣ������
//drv:�̷�
//����ֵ:ʣ������(�ֽ�)
u32 mf_showfree(u8 *drv)
{
  FATFS *fs1;
  u8 res;
  DWORD fre_clust=0, fre_sect=0, tot_sect=0;
  //�õ�������Ϣ�����д�����
  res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);
  if(res==0)
  {											   
    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//�õ���������
    fre_sect = fre_clust * fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512
    tot_sect*=fs1->ssize/512;
    fre_sect*=fs1->ssize/512;
#endif	  
  }
  return fre_sect;
}		    
//�ļ���дָ��ƫ��
//offset:����׵�ַ��ƫ����
//����ֵ:ִ�н��.
u8 mf_lseek(u32 offset)
{
  return f_lseek(file,offset);
}
//��ȡ�ļ���ǰ��дָ���λ��.
//����ֵ:λ��
u32 mf_tell(void)
{
  return f_tell(file);
}
//��ȡ�ļ���С
//����ֵ:�ļ���С
u32 mf_size(void)
{
  return f_size(file);
} 
//����Ŀ¼
//pname:Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_mkdir(u8*pname)
{
  return f_mkdir((const TCHAR *)pname);
}
//��ʽ��
//drv:�̷�
//mode:ģʽ
//au:�ش�С
//����ֵ:ִ�н��
u8 mf_fmkfs(u8 drv,u8 mode,u16 au)
{
  return f_mkfs(drv,mode,au);//��ʽ��,drv:�̷�;mode:ģʽ;au:�ش�С
}
//ɾ���ļ�/Ŀ¼
//pname:�ļ�/Ŀ¼·��+����
//����ֵ:ִ�н��
u8 mf_unlink(u8 *pname)
{
  return  f_unlink((const TCHAR *)pname);
}

//�޸��ļ�/Ŀ¼����(���Ŀ¼��ͬ,�������ƶ��ļ�Ŷ!)
//oldname:֮ǰ������
//newname:������
//����ֵ:ִ�н��
u8 mf_rename(u8 *oldname,u8* newname)
{
  return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
}

//���ļ������ȡһ���ַ���
//size:Ҫ��ȡ�ĳ���
void mf_gets(u16 size)
{
  TCHAR* rbuf;
  rbuf=f_gets((TCHAR*)fatbuf,size,file);
  if(*rbuf==0)return  ;//û�����ݶ���
}
//��Ҫ_USE_STRFUNC>=1
//дһ���ַ����ļ�
//c:Ҫд����ַ�
//����ֵ:ִ�н��
u8 mf_putc(u8 c)
{
  return f_putc((TCHAR)c,file);
}
//д�ַ������ļ�
//c:Ҫд����ַ���
//����ֵ:д����ַ�������
u8 mf_puts(u8*c)
{
  return f_puts((TCHAR*)c,file);
}

