/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : factcfg.h

    Description : ������������
 
    Others:      
    
    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSFACTCFG_H
#define _YSFACTCFG_H

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#include "stbtype.h"

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif                        

/*!!!ע�⣬�����ݽṹ�޸�����bios��Ӧ����ע��汾����!!!*/
typedef struct FACTORY_Config_s
{
	unsigned int		CfgMark;	/* 0xbeebebee*/
    unsigned int    	HWVersion;	/*Ӳ���汾*/
    unsigned char     	CAType;		/*CA����*/
    unsigned char     	TunerType;	/*��Ƶͷ���� -C -S -T*/
    unsigned char     	VoutType;	/*����/S����*/
    unsigned char     	DencMode;	/*N/P��*/
    unsigned int    	BoxSerial;	/*�������*/
    unsigned int    	ZipCode; 	/*������������Ϊ������*/

    unsigned int		FreAuto;	/*�Զ�������ȱʡƵ��*/
    unsigned short		SymAuto;	/*�Զ�������ȱʡ������*/
    unsigned short		QamAuto;	/*�Զ�������ȱʡ���Ʒ�ʽ*/

    unsigned int    	FreUpgrade;	/*�������ݵ�ȱʡƵ��*/
    unsigned short		SymUpgrade;	/*�������ݵ�ȱʡ������*/
    unsigned short		QamUpgrade;	/*�������ݵ�ȱʡ���Ʒ�ʽ*/
    unsigned short		UMT_pid;	/*�������ݵ�ȱʡUMT Pid*/
    unsigned short		UMT_tableid;/*�������ݵ�ȱʡUMT TableId*/

    unsigned char      	TVSoundChannel;/*���ӽ�ĿĬ������*/
    unsigned char      	RadioSoundChannel;/*�㲥��ĿĬ������*/
    unsigned char      	DisplayProportion;/*��ʾ����*/
    unsigned char      	BackgroundTransparence;/*����͸����*/
    unsigned int		FreHand;	/*�ֶ�����ȱʡƵ��*/
    unsigned short		SymHand;	/*�ֶ�����ȱʡ������*/
    unsigned short		QamHand;	/*�ֶ�����ȱʡ���Ʒ�ʽ*/
    unsigned short		SymFull; 	/*ȫƵ����ȱʡ������*/
    unsigned short		QamFull;	/*ȫƵ����ȱʡ���Ʒ�ʽ*/
    unsigned char      	ChannelSearchMode; /*����ģʽ*/
    unsigned char      	UpgradeTag; /*������ʾ��־*/
    unsigned char      	NetworkSoftWareVersion; /*��������汾��*/
    unsigned char      	ucApplicationDataVersion;/*Ӧ�����ݰ汾��*/
	
    /*�Ѿ�ʹ��60���ֽ�*/
    unsigned char 		Reserved[36];
    unsigned int 		ChkckSum;
	/*TOTAL 100bytes*/
}FACTORY_Config_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Init
  Description     : ��E2P�ж�ȡFactCfg����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Init(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_NVMSetData
  Description     : ����FactCfg����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_NVMSetData(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_GetFactInitStatus
  Description     : ��ȡFactCfg��ʼ��״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Fact_GetFactInitStatus(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_VoutType
  Description     : ��ȡFactCfg�е����ģʽ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char Fact_Get_VoutType(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_VoutType
  Description     : ����FactCfg�е����ģʽ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_VoutType(unsigned char Type);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_BoxSerial
  Description     : ��ȡFactCfg�еĻ��������к�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_BoxSerial(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_BoxSerial
  Description     : ����FactCfg�еĻ��������к�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_BoxSerial(unsigned int Serial);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_ZipCode
  Description     : ��ȡFactCfg�е�������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_ZipCode(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_ZipCode
  Description     : ����FactCfg�е�������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_ZipCode(unsigned int zip);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_AutoSearchTS
  Description     : ��ȡFactCfg�е��Զ�����Ƶ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_AutoSearchTS(unsigned int *pFre,unsigned short *pSym,unsigned short *pQam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_AutoSearchTS
  Description     : ����FactCfg�е��Զ�����Ƶ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_AutoSearchTS(unsigned int Fre,unsigned short Sym,unsigned short Qam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_UpgradeTS
  Description     : ��ȡFactCfg�е�����Ƶ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_UpgradeTS(unsigned int *pFre,unsigned short *pSym,unsigned short *pQam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_UpgradeTS
  Description     : ����FactCfg�е�����Ƶ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_UpgradeTS(unsigned int Fre,unsigned short Sym,unsigned short Qam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_UMTID
  Description     : ��ȡFactCfg�е�����UMTPid
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_UMTID(unsigned short *pPid,unsigned short *pTableid);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_UMTID
  Description     : ����FactCfg�е�����UMTPid
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_UMTID(unsigned short Pid,unsigned short Tableid);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_HWVersion
  Description     : ��ȡFactCfg�е�Ӳ���汾��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_HWVersion(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_GetVoutStatus
  Description     : ��ȡ���������״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Fact_GetVoutStatus(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_CAType
  Description     : ��ȡFactCfg�е�CA����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char Fact_Get_CAType(void);

#ifdef __cplusplus
}
#endif                        

#endif 

