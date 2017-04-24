/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatamgr.h

    Description : Ƶ�����ݽӿڹ���

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	__YSSTBDATAMGR_
#define	__YSSTBDATAMGR_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define STBDATA_INVALID_TS_NO				-1
#define STBDATA_INVALID_SERVICE_NO			-1
#define STBDATA_INVALID_TV_CHANNEL_NO		-1
#define STBDATA_DEFAULT_MAX_TV_CHANNEL_NO   0x7fff
#define STBDATA_INVALID_RADIO_CHANNEL_NO	-1

#define STBDATA_FIRST_CHANNELINFO_NO		0
#define DEFAULT_NETWORKID    				0x1FFF
#define DEFAULT_TSID                		0x1FF
#define STBDATA_DEFAULT_VOLUME_OFFSET       0x00

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum STBDATATsInfoStatus
{
	STBDATA_TS_FREE,
	STBDATA_TS_IN_USE
}STBDATATsInfoStatus_e;

typedef enum STBDATABATInfoStatus
{
	STBDATA_BAT_FREE,
	STBDATA_BAT_IN_USE
}STBDATABATInfoStatus_e;

typedef enum STBDATAServiceInfoStatus
{
	STBDATA_SERVICE_FREE,
	STBDATA_SERVICE_IN_USE
}STBDATAServiceInfoStatus_e;

typedef enum STBChannelInfoStatus
{
	STBDATA_CHANNEL_FREE,
	STBDATA_CHANNEL_IN_USE,
	STBDATA_CHANNEL_SKIP
}STBChannelInfoStatus_e;

typedef enum STBChannelInfoAttribute
{
	STBDATA_CHANNEL_NORMAL,
	STBDATA_CHANNEL_FAVORITE
}STBChannelInfoAttribute_e;

typedef enum STBDATATsInversionStatus
{
	STBDATA_TS_NOT_INVERSION,
	STBDATA_TS_INVERSION,
	STBDATA_TS_INVALID_INVERSION
}STBDATATsInversionStatus_e;

typedef enum STBSoundChannel
{
	STB_SOUND_STEREO,
	STB_SOUND_LEFT_CHANNEL,
	STB_SOUND_RIGHT_CHANNEL,
	STB_SOUND_SINGLE_CHANNEL,
	STB_SOUND_COUNT
}STBSoundChannel_e;

typedef enum STBDATAChannelLockStatus
{
	STBDATA_CHANNEL_NOT_LOCKED,
	STBDATA_CHANNEL_LOCKED
}STBDATAChannelLockStatus_e;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintEventData
  Description     : ��ӡNVODʱ�ƺͲο�ҵ���Լ���Ŀ����Ϊ0��Service��Event��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintEventData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WatchChnlExtendedEventInfo
  Description     : �鿴Ƶ��������չEvent��Ϣ�ӿ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_WatchChnlExtendedEventInfo(unsigned short usChannelNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintEventData
  Description     : ��TXT��ʽ��ӡ��Service��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintServiceData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_PrintServiceDataNew
  Description     : ��XML��ʽ��ӡ��Service��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_PrintServiceDataNew(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyTsNO
  Description     : ���TS �ŵ���Ч��
  Input           : TS ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyTsNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyBATNo
  Description     : ���TS �ŵ���Ч��
  Input           : TS ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyBATNo(signed short ssBATNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyServiceNO
  Description     : ���service �ŵ���Ч��
  Input           : service ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyMosaicNO
  Description     : ���mosaic �ŵ���Ч��
  Input           : service ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyMosaicNO(signed short  ssMosaicNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyTVChannelNO
  Description     : �����ӽ�ĿƵ�� �ŵ���Ч��
  Input           : ���ӽ�ĿƵ�� ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyTVChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyRadioChannelNO
  Description     : ���㲥��ĿƵ�� �ŵ���Ч��
  Input           : �㲥��ĿƵ�� ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyRadioChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyNVODChannelNO
  Description     : ���㲥��ĿƵ�� �ŵ���Ч��
  Input           : �㲥��ĿƵ�� ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyNVODChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyJZDBCChnlNO
  Description     : ������ݹ㲥��ĿƵ���ŵ���Ч��
  Input           : ���ݹ㲥��ĿƵ�� ��
  Outut           : true:��Ч��false:��Ч
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyJZDBCChnlNO(signed short ssDBChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitTSInfo
  Description     : ��ʼ��TS �б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitTSInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitBATInfo
  Description     : ��ʼ��BAT�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceInfo
  Description     : ��ʼ��service�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceInfo
  Description     : ��ʼ��ָ��ServiceInfo�ṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceInfo(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitTVChannelInfo
  Description     : ��ʼ�����ӽ�Ŀ�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitRadioChannelInfo
  Description     : ��ʼ���㲥��Ŀ�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleChannelInfo
  Description     : ��ʼ��ָ��STBChannelInfo�ṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleChannelInfo(STBDATAChannelInfo_t *pSTBChannelInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitNVODChannelInfo
  Description     : ��ʼ���㲥��Ŀ�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitMosaicChannelInfo
  Description     : ��ʼ�������˽�Ŀ�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitMosaicChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitJZDBCInfo
  Description     : ��ʼ�����ݹ㲥��Ŀ�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitJZDBCInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceAudioInfo
  Description     : ��ʼ��Service�����б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceAudioInfo
  Description     : ��ʼ��ָ��ServiceInfo�����ṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceAudioInfo(STBDATAServiceAudioInfo_t *pSTBServiceAudioInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitServiceSubtitleInfo
  Description     : ��ʼ��Service��Subtitle�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSingleServiceSubtitleInfo
  Description     : ��ʼ��ָ��Service��Subtitle�ṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSingleServiceSubtitleInfo(STBDATAServiceSubtitleInfo_t *pSubtitleInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_InitServiceInformation
  Description     : ��ʼ��serviceInformation�б�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitSTBDATAInfo
  Description     : ��ʼ��STB DATA
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitSTBDATAInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteTsInfoOfNO
  Description     : ���������TS��ɾ��TS����
  Input           : Ҫɾ����TS��TS��
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteTsInfoOfNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteAllTsInfoByTsNO
  Description     : ���������TS��ɾ��TS���ݼ���Ӧ��service ��Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteAllTsInfoByTsNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceInfoOfNO
  Description     : ���������service ���ݺ�ɾ��Ƶ������
  Input           : Ҫɾ����service ��service ��
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceInfoOfNO(signed short ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteTVChannelInfoOfNO
  Description     : ��������ĵ���Ƶ�� ���ݺ�ɾ������Ƶ������
  Input           : Ҫɾ���ĵ���Ƶ�� ���� �ĵ���Ƶ�� ���ݺ�
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteTVChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteRadioChannelInfoOfNO
  Description     : ��������Ĺ㲥Ƶ�� ���ݺ�ɾ���㲥Ƶ������
  Input           : Ҫɾ���Ĺ㲥Ƶ�� ���� �Ĺ㲥Ƶ�� ���ݺ�
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteRadioChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteNVODChannelInfoOfNO
  Description     : ���������NVOD Ƶ����ɾ��NVOD Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteNVODChannelInfoOfNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteMosaicChnlInfoOfNO
  Description     : ���������������Ƶ����ɾ��������Ƶ������
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteMosaicChnlInfoOfNO(signed short ssMosaicNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteJZDbcChannelInfoOfNO
  Description     : ���������DbcƵ�����ݺ�ɾ��DbcƵ������
  Input           : Ҫɾ����DbcƵ�����ݵ�DbcƵ�� ���ݺ�
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteJZDbcChannelInfoOfNO(signed short  ssDBCChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceAudioInfoOfNO
  Description     : ���������AudioNo��ɾ����AudioInfo�����а���
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceAudioInfoOfNO(signed short ssAudioNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_DeleteServiceSubtitleInfoOfNO
  Description     : ���������SubtitleNo��ɾ����SubtitleInfo������Subtitle
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceSubtitleInfoOfNO(signed short ssSubtitleNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_DeleteServiceInformationOfID
  Description     : ���������service ID��ɾ��Ƶ����� ����
  Input           :  
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_DeleteServiceInformationOfID(unsigned short usNetID,unsigned short usTSID,unsigned short usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeTsInfo
  Description     : �õ����е�TS����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t * STBDATA_GetFreeTsInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceInfo
  Description     : �õ����е�service����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t * STBDATA_GetFreeServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeTVChannelInfo
  Description     : �õ����еĵ��ӽ�Ŀ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t * STBDATA_GetFreeTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeRadioChannelInfo
  Description     : �õ����еĹ㲥��Ŀ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t * STBDATA_GetFreeRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeNVODChannelInfo
  Description     : �õ����е�TS����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVODChannelInfo_t * STBDATA_GetFreeNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicChannelInfo
  Description     : �õ����е�����������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t * STBDATA_GetFreeMosaicChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeJZDBCInfo
  Description     : �õ����е����ݹ㲥����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *  STBDATA_GetFreeJZDBCInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeBATInfo
  Description     : �õ����е�BAT���� 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABATInfo_t * STBDATA_GetFreeBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceAudioInfo
  Description     : �õ����е�Service���������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceAudioInfo_t * STBDATA_GetFreeServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeServiceSubtitleInfo
  Description     : �õ����е�Service��Subtitle����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceSubtitleInfo_t * STBDATA_GetFreeServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_GetFreeServiceInformation
  Description     : �õ����е�serviceInformation ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVMServiceInformation_t *STBDATA_GetFreeServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTSInfoInUse
  Description     : ȡ�ôӵ�ǰ�ſ�ʼ��һ��ʹ���е�Ts ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetNextTSInfoInUse(signed short  ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextServiceInfoInUse
  Description     : ȡ�ôӵ�ǰ�ſ�ʼ��һ��ʹ���е�service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t  *STBDATA_GetNextServiceInfoInUse(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfoInUse
  Description     : ȡ�ôӵ�ǰ�ſ�ʼ��һ��ʹ���еĵ���Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfoInUse(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfoInUse
  Description     : ȡ�ôӵ�ǰ�ſ�ʼ��һ��ʹ���еĹ㲥Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfoInUse(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoInUseNo
  Description     : ȡ��ʹ���е�TS ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetTsInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoInUseNo
  Description     : ȡ��ʹ���е�service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
signed short  STBDATA_GetServiceInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoInUseNo
  Description     : ȡ��ʹ���е�service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetServiceInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetBATInfoInUseNo
  Description     : ȡ��ʹ���е�bat ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetBATInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoInUseNo
  Description     : ȡ��ʹ���е�mosaic  ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetMosaicInfoInUseNo (void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoInUseNo
  Description     : ȡ��ʹ���е����ݹ㲥  ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetJZDBCInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoInUseNo
  Description     : ȡ��ʹ���еĵ��ӽ�ĿƵ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetTVChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoInUseNo
  Description     : ȡ��ʹ���еĹ㲥��ĿƵ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetRadioChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNVODChannelInfoInUseNo
  Description     : ȡ��ʹ���е�Nvod��ĿƵ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short  STBDATA_GetNVODChannelInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceAudioInfoInUseNo
  Description     : ͳ��ʹ���е����ж������Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBDATA_GetServiceAudioInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceSubtitleInfoInUseNo
  Description     : ͳ��ʹ���е����ж�Subtitle��Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBDATA_GetServiceSubtitleInfoInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInformationInUseNo
  Description     : ȡ��ʹ���е�ServiceInformation����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short  STBDATA_GetServiceInformationInUseNo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfServiceByTSNO
  Description     : ȡ��ͬһTS ��service����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfTVServiceByTSNO
  Description     : ȡ��ͬһTS �µ���service����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfTVServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfTVServiceByTSNOForSubSection
  Description     : ȡ��ͬһTS �µ���service ����(�ֶ��ֶ�����ר��)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ���жϴ�Service�Ƿ���뵽���ӽ�Ŀ�б���
*****************************************************************************/ 
signed short STBDATA_GetCountOfTVServiceByTSNOForSubSection(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfRadioServiceByTSNO
  Description     : ȡ��ͬһTS �¹㲥service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfRadioServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfRadioServiceByTSNOForSubSection
  Description     : ȡ��ͬһTS �¹㲥service ����(�ֶ��ֶ�����ר��)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ���жϴ�Service�Ƿ���뵽�㲥��Ŀ�б���
*****************************************************************************/ 
signed short STBDATA_GetCountOfRadioServiceByTSNOForSubSection(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfNvodReferenceServiceByTSNO
  Description     : ȡ��ͬһTS ��NVOD service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfNvodReferenceServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetCountOfDBCServiceByTSNO
  Description     : ȡ��ͬһTS �����ݹ㲥service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetCountOfDBCServiceByTSNO(signed short ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfID
  Description     : ����network_id ��ts_id ȡ��TS ����
  Input           : network_id ��ts_id 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfID(unsigned short  usNetID, unsigned short usTsID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfLinkageID
  Description     : ����LinkageTsID ȡ��TS ����(�ڴ�ά�����ʹ��)
  Input           : LinkageTsID 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfLinkageID(unsigned short usLinkageTsID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTsNO
  Description     : ����Ts ��ȡ��Ts����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTsNO(signed short  ssTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfServiceNO
  Description     : ����service ��ȡ��Ts����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTVChannelNO
  Description     : ���ݵ���Ƶ����ȡ��Ts����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTVChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfRadioChannelNO
  Description     : ���ݹ㲥Ƶ����ȡ��Ts����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfRadioChannelNO(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfTVOrRadioChannelNO
  Description     : ���ݹ㲥���ߵ���Ƶ����ȡ��Ts����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t *STBDATA_GetTsInfoOfTVOrRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoByDelivery
  Description     : ����Delivery ȡ��TS ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATsInfo_t * STBDATA_GetTsInfoByDelivery(STBDATADeliveryInfo_t *pDelivery);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetTsInversionStatus
  Description     : ����Delivery ����TS ��ת��־
  Input           : 
  Outut           :    
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char  STBDATA_GetTsInversionStatus(unsigned int    uiFrequency,unsigned int   uiSymbolRate, unsigned short  usModulation);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInversionStatus
  Description     : ����Delivery ȡ��TS ��ת��־
  Input           : 
  Outut           : ��ת��־
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetTsInversionStatus(unsigned int   uiFrequency,unsigned int  uiSymbolRate, unsigned short  usModulation, unsigned char ucInversion);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTsInfoOfID
  Description     : ����network_id ��ts_id ȡ��TS ����
  Input           : network_id ��ts_id 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetFirstServiceInfoOfTsNO(unsigned char ucTsNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceNO
  Description     : ����service ��ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceNO(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfTVChannelNO
  Description     : ���ݵ��ӽ�ĿƵ����ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfTVChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfRadioChannelNO
  Description     : ���ݹ㲥��ĿƵ����ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfRadioChannelNO
  Description     : ���ݹ㲥���ߵ��ӽ�ĿƵ����ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfTVorRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceNameAndType
  Description     : ����Service���ֺ�����ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceNameAndType(char *scServiceName, unsigned char ucServiceType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceName
  Description     : ����Service����ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceName(char * scServiceName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfID
  Description     : ����network_id �tts_id�s��service_idȡ��service ����
  Input           : network_id   ts_id  �� service_id
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfID(unsigned short  usNetID, unsigned short  usTsID, unsigned short  usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoByID
  Description     : ����LinkageTsIDs��LinkageServiceIDȡ��service����(�ڴ�ά�����ʹ��)
  Input           : LinkageTsIDs��LinkageServiceID
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfLinkageID(unsigned short  usLinkageTsID, unsigned short  usLinkageServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceInfoOfServiceID
  Description     : ����service_id   ȡ��service ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceInfo_t *STBDATA_GetServiceInfoOfServiceID(unsigned short usServiceId);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetServiceExtendedData
  Description     : ��ʼ��service����ʹ�õ���չ���ݽṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceExtendedData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_ClearServiceExtendedData
  Description     : ���service����ʹ�õ���չ���ݽṹ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_ClearServiceExtendedData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_TrimOverdueEventData
  Description     : ɾ��service���������й��ڵ�Event��Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_TrimOverdueEventData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfTVChannelNO
  Description     : ���ݵ���Ƶ�� ��ȡ�õ���Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetTVChannelInfoOfTVChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoOfRadioChannelNO
  Description     : ���ݹ㲥Ƶ�� ��ȡ�ù㲥Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetRadioChannelInfoOfRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetChannelInfoOfTVorRadioChannelNO
  Description     : ����Ƶ���Ż�ȡ���ӻ��߹㲥��Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetChannelInfoOfTVorRadioChannelNO(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoOfChnlNo
  Description     : �������ݹ㲥Ƶ���Ż�ȡ���ݹ㲥��Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *STBDATA_GetJZDBCInfoOfChnlNo(signed short ssDBCChnlNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetJZDBCInfoOfServiceNo
  Description     : �������ݹ㲥��ServiceNo��ȡ���ݹ㲥��Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABroadcastInfo_t *STBDATA_GetJZDBCInfoOfServiceNo(signed short ServiceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfServiceNo
  Description     : ����service ��ȡ�õ���Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetTVChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetRadioChannelInfoOfServiceNo
  Description     : ����service ��ȡ�ù㲥Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetRadioChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNVODChannelInfoOfServiceNo
  Description     : ����service ��ȡ�ù㲥Ƶ�� ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVODChannelInfo_t  *STBDATA_GetNVODChannelInfoOfServiceNo(signed short  ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfoForDTV
  Description     : ȡ����һ������Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVChannelInfoForDTV
  Description     : ȡ����һ������Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevTVChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfoForDTV
  Description     : ȡ����һ���㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevRadioChannelInfoForDTV
  Description     : ȡ����һ���㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevRadioChannelInfoForDTV(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVChannelInfo
  Description     : ȡ����һ������Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextTVChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextRadioChannelInfo
  Description     : ȡ����һ���㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetNextRadioChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextTVOrRadioChannelInfo
  Description     : ȡ����һ���㲥���ߵ���Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetNextTVOrRadioChannelInfo(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVChannelInfo
  Description     : ȡ����һ������Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevTVChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevRadioChannelInfo
  Description     : ȡ����һ���㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t  *STBDATA_GetPrevRadioChannelInfo(signed short    ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevTVOrRadioChannelInfo
  Description     : ȡ����һ���㲥���ߵ���Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetPrevTVOrRadioChannelInfo(signed short  ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTime
  Description     : ȡ�ø���ʱ���event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetEventInfoOfTime(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetNextEventInfoOfTime
  Description     : ȡ�ø���ʱ������һ��event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL  STBDATA_GetNextEventInfoOfTime(signed short ssServiceNO,StbTime_t  StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTimeFromPF
  Description     : ��PF ��ȡ�ø���ʱ���event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetEventInfoOfTimeFromPF(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfTimeFromSchedule
  Description     : ��Schedule ��ȡ�ø���ʱ���event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL  STBDATA_GetEventInfoOfTimeFromSchedule(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetEventInfoOfId
  Description     : ����ID ��ȡ��event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAEvent_t *STBDATA_GetEventInfoOfId(signed short ssServiceNO, unsigned short usEventid);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetPrevEventInfoOfTime
  Description     : ȡ�ø���ʱ������һ��event ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_GetPrevEventInfoOfTime(signed short ssServiceNO,StbTime_t StbTm,STBDATAEvent_t  **pSTBEventInUse);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetTVChannelInfoOfEventInfo
  Description     : ����EventInfo ȡ��TV Channel Info
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAChannelInfo_t *STBDATA_GetTVChannelInfoOfEventInfo(STBDATAEvent_t *STBEvent);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVChannelNo
  Description     : ��ȡ��ǰ����Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfTVChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfRadioChannelNo
  Description     : ��ȡ��ǰ�㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVorRadioChannelNo
  Description     : ��ȡ��ǰ���ӻ��߹㲥Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetAttributeOfTVorRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetLockOfTVChannelNo
  Description     : ����Ƶ���Ż�ȡ���ӽ�Ŀ�Ƿ���� 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfTVChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetLockOfRadioChannelNo
  Description     : ����Ƶ���Ż�ȡ�㲥��Ŀ�Ƿ���� 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAttributeOfTVorRadioChannelNo
  Description     : ����Ƶ���Ż�ȡ���ӽ�Ŀ���߹㲥��Ŀ�Ƿ���� 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLockOfTVorRadioChannelNo(signed short ssTVChannelNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetChannelNoOfID
  Description     : ͨ��networkid, tsid, service id  ��ö�Ӧ��Ƶ�����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetChannelNoOfID(unsigned short usNetID, unsigned short usTsID, unsigned short usServiceId);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetBATInfoOfID
  Description     : ����BAT ID��ȡBAT��Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATABATInfo_t * STBDATA_GetBATInfoOfID(unsigned short usBATID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_VerifyServiceContentOfBATNO
  Description     : ������BAT�ź�ServiceNO��ȡ��BAT��Ϣ�Ƿ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_VerifyServiceContentOfBATNO(signed short ssBATNo, signed short ssServiceNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_InitMosaicInfoTemp
  Description     : ��ʼ��Mosaic ����(����Ƶ�����湦����)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_InitMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicInfoTemp
  Description     : �õ����е�Mosaic ����(����Ƶ�����湦����)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATempMosaicInfo_t * STBDATA_GetFreeMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetFreeMosaicInfoTemp
  Description     : �õ����е�Mosaic ����(����Ƶ�����湦����)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATATempMosaicInfo_t * STBDATA_GetFreeMosaicInfoTemp(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_MallocMosaicTmp
  Description     : ΪMosaic ���ݷ����ڴ�(����Ƶ�����湦����) 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_MallocMosaicTmp(DRVOS_Partition_t * pPtn);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_FreeMosaicTmp
  Description     : �ͷ�Mosaic ���ݵ��ڴ�(����Ƶ�����湦����)  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_FreeMosaicTmp(DRVOS_Partition_t * pPtn);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicEntryChnl
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t * STBDATA_GetMosaicEntryChnl(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicCellInfoOfLogicalCellID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicLogicalCellInfo_t * STBDATA_GetMosaicCellInfoOfLogicalCellID(signed short ssMosaicChnlNo, unsigned char ucLogicalCellID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoOfChnlNo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t *STBDATA_GetMosaicInfoOfChnlNo(signed short ssMosaicChnlNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicInfoOfServiceNo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAMosaicInfo_t *STBDATA_GetMosaicInfoOfServiceNo(signed short ssServiceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicLogicalIDofElemID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed char STBDATA_GetMosaicLogicalIDofElemID(signed short ssMosaicChnlNo, unsigned char ucElemID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetMosaicLogicalIDofElemID
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
signed short STBDATA_GetMosaicPFServiceNo(signed short ssMosaicChnlNo,unsigned int MsgCode);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_SetMosaicChnlInfo
  Description     : 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetMosaicChnlInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          :  xyan
  Function Name   : STBDATA_StorageChannelInfo
  Description     : ��������ǰ��Ƶ����Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_StorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetStorageChannelInfo
  Description     : ��������ǰ�����Ƶ����Ϣ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetStorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          :  xyan
  Function Name   : STBDATA_FreeStorageChannelInfo
  Description     : �ͷű�������ǰƵ����Ϣ���ڴ�
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_FreeStorageChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceAudioInfoOfAudioNO
  Description     : ����AudioNO��ȡServiceAudioInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceAudioInfo_t *STBDATA_GetServiceAudioInfoOfAudioNO(signed short ssAudioNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetAudioInfoOfAudioPID
  Description     : ����AudioPID��ȡAudioInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBAudioInfo_t  *STBDATA_GetAudioInfoOfAudioPID(STBDATAServiceAudioInfo_t *pServiceAudioInfo,unsigned short usAudioPID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30 
  Author          : xyan
  Function Name   : STBDATA_FreeStorageChannelInfo
  Description     : �����������ȡ��������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_GetLanguageIndexOfCode(char *Code);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetServiceSubtitleInfoOfSubtitleNO
  Description     : ����SubtitleNO��ȡServiceSubtitleInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAServiceSubtitleInfo_t *STBDATA_GetServiceSubtitleInfoOfSubtitleNO(signed short ssSubtitleNO);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetSubtitleInfoOfSubtitlePID
  Description     : ����SubtitlePID��ȡSubtitleInfo
  Input           :
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBSubtitleInfo_t  *STBDATA_GetSubtitleInfoOfSubtitlePID(STBDATAServiceSubtitleInfo_t *pServiceSubtitleInfo,unsigned short usSubtitlePID,unsigned char *ucSubtitleNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_GetServiceInformationOfID
  Description     : ����network_id �tts_id�s��service_id ȡ��serviceInformation����
  Input           : network_id   ts_id  �� service_id
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATANVMServiceInformation_t *STBDATA_GetServiceInformationOfID(unsigned short  usNetID, unsigned short  usTsID, unsigned short  usServiceID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelStatus
  Description     : ����Ƶ����״̬
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelStatus(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelAttribute
  Description     : ����Ƶ����ϲ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelAttribute(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucAttribute);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetChannelLocked
  Description     : ����Ƶ���ļ�������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetChannelLocked(STBDATAChannelInfo_t *pSTBChannelInfo,unsigned char ucLocked);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetServiceSoundChannel
  Description     : ����Service�ṹ�е�����ֵ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceSoundChannel(STBDATAServiceInfo_t *pSTBServiceInfo,unsigned char ucSoundChannel);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :  xyan
  Function Name   : STBDATA_SetServiceVolumeOffset
  Description     : ����Service�ṹ�е���������ֵ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_SetServiceVolumeOffset(STBDATAServiceInfo_t *pSTBServiceInfo,signed char   scVolumeOffset);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   wrwei
  Function Name   : STBDATA_GetTVChannelListInString
  Description     : ��ȡ����Ƶ����Ϣ�����ַ��е���ʽ��������Ƶ����Ƶ���ź�Ƶ������
  Input           : pChannelList�������ݵ�ָ�룬siDataLength���ݿռ��С��GetStringLength��ȡ���ݵ�ʵ�ʳ���
  Outut           : 1�ɹ�;0ʧ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : For Andriod use
*****************************************************************************/
signed int STBDATA_GetTVChannelListInString(unsigned char *pChannelList,signed int siDataLength,signed int *GetStringLength);

#endif


