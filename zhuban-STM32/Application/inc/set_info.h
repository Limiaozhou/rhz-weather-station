
////////////////////////////////////////////////////////////////////////////////// 
//���ò���
//���������
//�汾��V1.0
//�޸�����:2016/11/2
//Copyright(C) ��������ǵ��ӿƼ����޹�˾ 
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////    

#ifndef _SET_INFO_H_
#define _SET_INFO_H_

/***************************************
*@DESCRIPTION: --�������ýṹ�壬����
  Ӳ�����
  Ӳ���汾��
  ����汾��
  ������IP��ַ
  �������˿�
  WIFI��֤��ʽ
  WIFI�����㷨
  WIFI����·��������
  WIFI����·��������
  ����Ƶ��
  485�����ַ
*
****************************************/
typedef struct 
{
  unsigned char  Soft_Ver[25]; //����汾
  unsigned char  Hard_Ver[25]; //Ӳ���汾
  unsigned char  Addr[8]; //Ӳ�����
  unsigned char  Server_Ip[20]; //������Ip��ַ
  unsigned char  Server_Port[6]; //�������˿�
  unsigned char  Encr_Auth[10]; //��֤
  unsigned char  Encr_Encry[10]; //�����㷨
  unsigned char  Apne_Name[20]; //
  unsigned char  Apne_Passwd[20]; //
  float  Send_Fre;//����Ƶ������
}INFO_MUL;

typedef struct
{
  unsigned char addr_485;//485��ַ
  unsigned char regi_485[2];//485�Ĵ���
}INFO_485;


extern char* get_Soft_ver();
extern char* get_Hart_ver();
extern INFO_MUL *Info_Flash;
#endif