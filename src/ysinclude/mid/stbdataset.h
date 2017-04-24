/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataset.h

    Description : STB�������ù���

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSTBDATASET_
#define _YSSTBDATASET_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define STB_SET_CHANNEL_SEARCHING_TASK_STACK_SIZE 		(USER_WS_SIZE*16)
#define STB_SET_CHANNEL_SEARCHING_TASK_PARTITION_SIZE 	(NORMAL_PART_SIZE*16)

/*��������User��ϢMsgCode����*/
#define STBSET_STOP_CHANNEL_SEARCHING   				CHSRCH_USER_BASE 
#define STB_SET_CHANNEL_SEARCHING_TS_CHANGED 			(CHSRCH_USER_BASE+0x01)
#define STB_SET_CHANNEL_SEARCHING_FINISHED 				(CHSRCH_USER_BASE+0x02)
#define STB_SET_CHANNEL_SEARCHING_AUTO 					(CHSRCH_USER_BASE+0x03)
#define STB_SET_CHANNEL_SEARCHING_HANDWORK 				(CHSRCH_USER_BASE+0x04)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_TEST 		(CHSRCH_USER_BASE+0x05)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_SUCCESS 		(CHSRCH_USER_BASE+0x06)
#define STB_SET_CHANNEL_SEARCHING_FULL_FRE_SUCCESS_FAKE (CHSRCH_USER_BASE+0x07)

/*͸����Ĭ�϶���*/
#define STBSET_MIN_COLOR_TRANSPARENCE					37
#define STBSET_MAX_COLOR_TRANSPARENCE 					63
#define STBSET_DEFAULT_COLOR_TRANSPARENCE 				63

/*���ȡ�ɫ�ȺͶԱȶ�Ĭ�϶���*/
#define STBSET_DEFAULT_BRIGHT							5
#define STBSET_DEFAULT_CONTRAST							5
#define STBSET_DEFAULT_SATURATION						5
#define STBSET_MAX_BRIGHT								10
#define STBSET_MAX_CONTRAST								10
#define STBSET_MAX_SATURATION							10

/*���ܿ�Ĭ�Ͽ�ֵ*/
#define DEFAULT_CA_BATID 								0x101

/*����ģʽ��ʶֵ*/
#define FACTORY_MODE_FLAG								0xabccddef   

/*�Զ����ػ���ʶֵ*/
#define STB_POWEROFF_REPEAT_MODE						0xab
#define STB_POWEROFF_FLAG								0xa3
#define STB_POWERON_FLAG								0xb3

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum ChsrchMode
{
	SEARCHING_CHANNEL_AUTO,
	SEARCHING_CHANNEL_FULL_FREQUENCY,
	SEARCHING_CHANNEL_HANDWORK	
}ChsrchMode_e;

/*������������(��LanguageCode_e��Ӧ)*/
typedef enum LanguageType
{
	CHINESE = 0 ,
	ENGLISH = 1 ,
	LANGUAGE_TYPE_COUNT
}LanguageType_e;

typedef struct STBSYSInfo
{
	char scHardwareInfo[15];
	char scSoftInfo[15];
	char scNetworkSoftInfo[15];
}STBSYSInfo_t;

typedef struct STBSoundSet
{
	signed char scTVSoundChannel;
	signed char scRadioSoundChannel;
}STBSoundSet_t;

typedef struct STBDisplaySet
{
	signed short ssBackgroundTransparence;
	signed char scDisplayProportion;
}STBDisplaySet_t;

/*!!!ע�⣬�����ݽṹ�޸�����bios��Ӧ����ע��汾����!!!*/
typedef struct 
{
	unsigned int 	CfgMark;	/* 0xbeebebee*/

	/*��������ȱʡ������*/	
	unsigned int 	FreUpgrade;	
	unsigned short	SymUpgrade;
	unsigned short	QamUpgrade;
	unsigned short	UMT_pid;
	unsigned short	UMT_tableid;
	
	/* ���ӽ�ĿĬ������*/			
	unsigned char 	TVSoundChannel ;
	/*�㲥��ĿĬ������*/
	unsigned char 	RadioSoundChannel ;
	/*��ʾ����*/
	unsigned char 	DisplayProportion ;
	/*����͸����*/
	unsigned char 	BackgroundTransparence ;
	
	/*����ȱʡ������*/
	unsigned int	FreAuto;	
	unsigned short	SymAuto;
	unsigned short	QamAuto;
	/* �ֶ�*/
	unsigned int	FreHand;	
	unsigned short	SymHand;
	unsigned short	QamHand;
	/*ȫƵ*/
	unsigned short	SymFull;
	unsigned short	QamFull;

	/*����ģʽ*/
	unsigned char 	ChannelSearchMode ;
	/*������ʾ��־*/
	unsigned char 	UpgradeTag;
	/*��������汾��*/
	unsigned short 	NetworkSoftWareVersion;
	
	/*Ӧ�����ݰ汾��*/
	unsigned char 	ucApplicationDataVersion;
	
	unsigned int 	Reserved[4];
}APP_Config_t;


/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ShowBootPicture
  Description	  : ������ʾ��������
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
void STBSet_ShowBootPicture(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ReceiveDataForBoot
  Description	  : �������վ���������Ϣ��NIT��Ϣ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
void STBSet_ReceiveDataForBoot(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_LoadCABATID
  Description	  : �����ܿ��л�ȡ��Ȩҵ��Ⱥ��id ֵ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL STBSet_LoadCABATID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_IsChannelLocked
  Description     : Ƶ�����������жϺ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_IsChannelLocked(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UnlockChannel
  Description     : �ر�Ƶ����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_UnlockChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UnlockChannel
  Description     : ��Ƶ����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_LockChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InitAppSettingData
  Description     : ��ʼ��Ӧ����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_InitAppSettingData( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_LoadAppSettingData
  Description     : ��E2PROM�ж�ȡӦ����������
  Input           : 
  Outut           : true��ȡ�ɹ���false��ȡʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 	
BOOL STBSet_LoadAppSettingData(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_WriteAppSettingData
  Description     : ��Ӧ����������д��E2PROM
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_WriteAppSettingData(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetCABATID
  Description	  : ��ȡ���ܿ��е���Ȩҵ��Ⱥid ֵ  
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
unsigned int STBSet_GetCABATID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetSYSInfo
  Description     : ���Ӳ��������������������Ϣ
  Input           : pSTBSYSInfoϵͳ��Ϣ�ṹָ��
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_GetSYSInfo(STBSYSInfo_t *pSTBSYSInfo);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetApplicationDataVersion
  Description     : ���Ƶ����������ݰ汾��
  Input           : 
  Outut           : �汾��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetApplicationDataVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetApplicationDataVersion
  Description     : ����Ӧ�����ݰ汾��
  Input           : ucVersion �汾��
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetApplicationDataVersion(unsigned char ucVersion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetTVDefaultSoundChannel
  Description     : ���Ĭ�ϵ��ӽ�Ŀ����
  Input           : 
  Outut           : ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetTVDefaultSoundChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetTVDefaultSoundChannel
  Description     : ����Ĭ�ϵ��ӽ�Ŀ����
  Input           : siTVSoundChannel����ֵ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetTVDefaultSoundChannel(int siTVSoundChannel);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetRadioDefaultSoundChannel
  Description     : ���Ĭ�Ϲ㲥��Ŀ����
  Input           : 
  Outut           :  ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetRadioDefaultSoundChannel(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetRadioDefaultSoundChannel
  Description     : ����Ĭ�Ϲ㲥��Ŀ����
  Input           : siRadioSoundChannel����ֵ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetRadioDefaultSoundChannel(int siRadioSoundChannel);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetDisplayProportion
  Description     : �����ʾ����
  Input           : 
  Outut           : ��ʾ����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetDisplayProportion( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetDisplayProportion
  Description     : ������ʾ����
  Input           : siDisplayProportion ��ʾ����
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetDisplayProportion(int siDisplayProportion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetVideoMode
  Description     : ��ȡ����Ƶ��ʽģʽ
  Input           : 
  Outut           : g_VideoMode
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetVideoMode( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetVideoMode
  Description     : ��������Ƶ��ʽģʽ
  Input           : ��ʽģʽsiMode
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetVideoMode( int siMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSET_ResetBackgroundTransparence
  Description     : ���õ�ɫ����ɫ͸����
  Input           : siBackgroundTransparence ͸����
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSET_ResetBackgroundTransparence(int siBackgroundTransparence);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetBackgroundTransparence
  Description     : ��ñ���͸����
  Input           : 
  Outut           : ͸����
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetBackgroundTransparence(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetBackgroundTransparence
  Description     : ���ñ���͸����
  Input           : siBackgroundTransparence ����͸����
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetBackgroundTransparence(int siBackgroundTransparence);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetChannelSearchMode
  Description     : ���Ƶ������ģʽ
  Input           : 
  Outut           : ����ģʽ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBSet_GetChannelSearchMode(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetChannelSearchMode
  Description     : ����Ƶ������ģʽ
  Input           : siChannelSearchMode����ģʽ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetChannelSearchMode(int siChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetDelivery
  Description     : ���Ĭ��Ƶ��ֵ
  Input           : scChannelSearchMode����ģʽ
  Outut           : Ƶ��ṹָ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATADeliveryInfo_t  *STBSet_GetDelivery(unsigned char scChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetDelivery
  Description     : ����Ĭ��Ƶ��ֵ
  Input           : DeliveryOfSet Ƶ������,scChannelSearchMode ����ģʽ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetDelivery(STBDATADeliveryInfo_t DeliveryOfSet,unsigned char scChannelSearchMode);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUpgradeDelivery
  Description     : ���Ĭ������Ƶ��ֵ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATADeliveryInfo_t  *STBSet_GetUpgradeDelivery(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUpgradeDelivery
  Description     : ��������Ƶ��ֵ
  Input           : DeliveryOfSet Ƶ��ֵ
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUpgradeDelivery(STBDATADeliveryInfo_t DeliveryOfSet);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUMTPID
  Description     : ��ȡUMT���PID
  Input           :
  Outut           : PID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetUMTPID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUMTPID
  Description     : ����UMT���PID
  Input           : usUMTPID UMT���PID
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUMTPID(unsigned short usUMTPID);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUMTTableID
  Description     : ��ȡUMT���TABLEID
  Input           : 
  Outut           : TABLEID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetUMTTableID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUMTTableID
  Description     : ����UMT���TABLEID
  Input           : usUMTTableID UMT���TABLEID
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUMTTableID(unsigned short usUMTTableID);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetLanguageSetting
  Description     : ��ȡ��������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetLanguageSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetLanguageSetting
  Description     : ��������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetLanguageSetting(unsigned char ucLanguageSetting);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_ReadLanguageSetting
  Description     : ��FLASH�ж�����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_ReadLanguageSetting( void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetNetworkSoftWareVersion
  Description     : ��ȡ��������汾��
  Input           : 
  Outut           : �汾��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBSet_GetNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetNetworkSoftWareVersion
  Description     : ������������汾��
  Input           : usNetworkSoftWareVersion �汾��
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetNetworkSoftWareVersion(unsigned short usNetworkSoftWareVersion);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetUpgradeTag
  Description     : ��ȡ������־
  Input           :
  Outut           : ������־
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBSet_GetUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetUpgradeTag
  Description     : ����������־
  Input           : ucUpgradeTag ������־
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetUpgradeTag(unsigned char ucUpgradeTag);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetCurrentTSNoCount
  Description     : ���õ�ǰ�Ѿ�������TS����(Ϊ��Ҫ�������ȵ��Զ�������ȫƵ����ʹ��)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetCurrentTSNoCount(signed short siCount);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetCurrentTSNoCount
  Description     : ��ȡ��ǰ�Ѿ�������TS����(Ϊ��Ҫ�������ȵ��Զ�������ȫƵ����ʹ��)  	
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetCurrentTSNoCount(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetTotalTSNoCount
  Description     : ����Ҫ������TS����(Ϊ��Ҫ�������ȵ��Զ�������ȫƵ����ʹ��)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_SetTotalTSNoCount(signed short siCount);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_GetTotalTSNoCount
  Description     : ��ȡҪ������TS����(Ϊ��Ҫ�������ȵ��Զ�������ȫƵ����ʹ��)  	
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBSet_GetTotalTSNoCount(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetManufactureModeVout
  Description     : ��������ģʽ�µ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSet_SetManufactureModeVout(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_SetManufactureModeIP
  Description     : ��������ģʽ�µ�IP
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBSet_SetManufactureModeIP(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_UserModeComeback
  Description     : ������ģʽ�ָ����û�ģʽ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_UserModeComeback(void);

/*****************************************************************************
  Date&Time    :    2004-04-09
  Author          :   xyan
  Function Name   : STBSet_DefaultDataComeback
  Description     : �ָ�Ĭ������
  Input           : siTure �Ƿ�ָ�
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_DefaultDataComeback( int siTure);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUpgradeDeliverySetting
  Description     : ��Ӧ���������ݻ����ж�������Ƶ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUpgradeDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUpgradeDeliverySetting
  Description     : ������Ƶ��д��Ӧ���������ݻ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUpgradeDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUMTID
  Description     : ��Ӧ���������ݻ����ж���UMT��PID��TABLEID
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUMTID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUMTID
  Description     : ��UMT��PID��TABLEIDд��Ӧ���������ݻ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUMTID(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadNetworkSoftWareVersion
  Description     : ��Ӧ���������ݻ����ж�����������汾��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteNetworkSoftWareVersion
  Description     : ����������汾��д��Ӧ���������ݻ�����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteNetworkSoftWareVersion(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadUpgradeTag
  Description     : ��Ӧ���������ݻ����ж���������־
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteUpgradeTag
  Description     : ��������־д��Ӧ���������ݻ�����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteUpgradeTag(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadDeliverySetting
  Description     : ��Ӧ���������ݻ����ж���Ĭ��Ƶ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteDeliverySetting
  Description     : ��Ĭ��Ƶ������д��Ӧ���������ݻ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteDeliverySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadChannelSearchModeSetting
  Description     : ��Ӧ���������ݻ����ж�������ģʽ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadChannelSearchModeSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteChannelSearchModeSetting
  Description     : ������ģʽд��Ӧ���������ݻ�����
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteChannelSearchModeSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadDisplaySetting
  Description     : ��Ӧ���������ݻ����ж�����ʾ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadDisplaySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteDisplaySetting
  Description     : ����ʾ����д��Ӧ���������ݻ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteDisplaySetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMLoadSoundSetting
  Description     : ��Ӧ���������ݻ����ж�����������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMLoadSoundSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_NVMWriteSoundSetting
  Description     : ����������д��Ӧ���������ݻ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBSet_NVMWriteSoundSetting(void);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InsertTVChnl
  Description     : ��ԭƵ������Ŀ��Ƶ��
  Input           : ssTVChnlNOSource ԴƵ����ssTVChnlNODestinationĿ��Ƶ����
  Outut           : true ����ɹ���false����ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_InsertTVChnl(signed short ssTVChnlNOSource,signed short ssTVChnlNODestination);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelStatusChange
  Description     : �޸ĵ���Ƶ������
  Input           : ssTVChannelNO ����Ƶ���� ucStatus Ƶ������
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelStatusChange( signed short ssTVChannelNO, unsigned char ucStatus);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelAttributeChange
  Description     : �޸ĵ���Ƶ������(ϲ��)
  Input           : ssTVChannelNO ����Ƶ���� ucStatus Ƶ������
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelAttributeChange( signed short ssTVChannelNO, unsigned char ucAttribute);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_TVChannelLockChange
  Description     : �޸ĵ���Ƶ������״̬
  Input           : ssTVChannelNO ����Ƶ���� ucLocked ����״̬
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_TVChannelLockChange( signed short ssTVChannelNO, unsigned char ucLocked);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelAttributeChange
  Description     : �޸Ĺ㲥Ƶ������(ϲ��)
  Input           : ssRadioChannelNO �㲥Ƶ���� ucStatus Ƶ������
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelAttributeChange( signed short ssRadioChannelNO, unsigned char ucAttribute);


/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_InsertTVChnl
  Description     : ��ԭƵ������Ŀ��Ƶ��
  Input           : ssTVChnlNOSource ԴƵ����ssTVChnlNODestinationĿ��Ƶ����
  Outut           : true ����ɹ���false����ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_InsertRadioChnl(signed short ssTVChnlNOSource,signed short ssTVChnlNODestination);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelStatusChange
  Description     : �޸ĵ���Ƶ������
  Input           : ssTVChannelNO ����Ƶ���� ucStatus Ƶ������
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelStatusChange( signed short ssTVChannelNO, unsigned char ucStatus);

/*****************************************************************************
  Date&Time       : 2004-04-09
  Author          :   xyan
  Function Name   : STBSet_RadioChannelLockChange
  Description     : �޸Ĺ㲥Ƶ������״̬
  Input           : ssTVChannelNO �㲥Ƶ���� ucLocked ����״̬
  Outut           : true �޸ĳɹ���false�޸�ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBSet_RadioChannelLockChange( signed short ssTVChannelNO, unsigned char ucLocked);

#endif
