/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : midware.h

    Description : ����ģ��ײ�ӿڷ�װ����

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	__YSMHP_
#define	__YSMHP_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h" 
#include "dvbtablereceive.h"
#include "dvbreceiveeit.h"
#include "dvbtablemgr.h"
#include "dvbtableparse.h"
#include "dvbtabledata.h"

#include "stbdata.h"
#include "stbdatamgr.h"
#include "stbdatanvm.h"
#include "stbdataReceiveADT.h"
#include "stbdataReceiveADTForKonka.h"
#include "stbdataReceiveADTForSkyWorth.h"
#include "stbdataReceiveAUT.h"
#include "stbdataReceivePDT.h"
#include "stbdataReceivePIC.h"
#include "stbdataReceivePRG.h"
#include "stbdataReceiveXDT.h"
#include "stbdatasearch.h"
#include "stbdataset.h"
#include "stbdataupdate.h"
#include "stbdataupgrade.h"
#include "factcfg.h"

#include "app.h"
#include "booking.h"
#include "msg.h"
#include "guitk.h"
#include "dlg.h"
#include "xmlparser.h"
//#include "../testcmd/dbgtst/midwaredbg.h"
#include "appregion.h"


/*----------------------------------------------------------------------------
 *	Define(�޸Ŀ��ƺ��,����Ҫ�ر�Midware)
 *----------------------------------------------------------------------------*/

/***************����ƽ̨���ƺ�ѡ��(Ĭ��ʹ��Linux����ƽ̨)***************/

/*����ʹ��OS20�Ĳ���ϵͳ���ƺ�(Ĭ�ϲ�����)*/
//#define USE_OS20_OPERATE_SYSTEM

/*����ʹ��OS21�Ĳ���ϵͳ���ƺ�(Ĭ�ϲ�����)*/
//#define USE_OS21_OPERATE_SYSTEM

/*����ʹ��Linux����ϵͳ���ƺ�(Ĭ������)*/
#define USE_LINUX_OPERATE_SYSTEM

/******************�������ƺ�ѡ��(Ĭ��ʹ����оƬ�����)******************/

/*����ʹ�üٴ������ܿ��ƺ�(Ĭ�ϲ�����),���ر�appys, midware*/
//#define USE_FAKE_STANDBY_MODE

/*����ǰ��������ģʽ(Ĭ�Ϲر�), ����OS21��ʹ��*/
//#define USE_PANEL_REAL_STANDBY_MODE

/***************************�������ܿ��ƺ�ѡ��***************************/

/*�����׼�����ݽ����Ƿ����ָ���û�ģʽ�Ŀ��ƺ�(Ĭ������)*/
#define GET_DATA_USE_POINTER_REPLACE

/*���屣���Զ�����ǰ���н�ĿƵ�����ԵĿ��ƺ�(Ĭ�ϲ�����)*/
//#define SAVE_CHANNEL_ATTRIBUTE_BEFORE_SEARCH

/*�������δ��Ȩ��Ŀ���ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define USE_FILTER_NO_AUTHORIZED_PROGRAMME

/*����ʹ�ñ�����չƵ�����ݹ��ܿ��ƺ�(Ĭ������,������Ͷ�Subtitile����Ŀ��鱣��ʱʹ��)*/
#define USE_EXTEND_CHNLINFO_MODE

/*����64KƵ������ģʽ(Ĭ�ϲ�����)*/
//#define USE_DATA_BLOCK_64K_MODE

/************************��׼�����ܿ��ƺ�ѡ��************************/

/*����ʹ��Event��չ��Ϣ���ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define USE_EXTENDED_EVENT_INFO

/*����ʹ��Epg�ڴ�ȱ��ģʽ(Ĭ�ϲ�����,������ǰ�˲���Epg����Epg�ڴ治�������)*/
//#define USE_EPG_MEMORY_LOW_MODE

/*����֧��Epg�����Թ��ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define SUPPORT_MULTY_LANGUAGE_EPG

/************************��̨���¹��ܿ��ƺ�ѡ��************************/

/*�����̨�������ɾ����Ŀ���ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define UPDATE_ADD_AND_DELETE_PROG

/*����ʹ�������������ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define USE_ZONE_UPGRADE

/***************************Ӧ�ù��ܿ��ƺ�ѡ��***************************/

/*����ʹ������Ԥ��IP��S���ӹ��ܿ��ƺ�(Ĭ�ϲ�����)*/
//#define USE_MANUFACTURE_DEFAULT_SET

/*����S���Ӻͷ���������ƺ�(Ĭ�ϲ�����)*/
//#define YC_MUTEX_WITH_YUV

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*����ģ������ö��*/
typedef enum Mid_Task_Id
{
	MID_CHSRCH_TASK=1,
	MID_UPDATE_TASK=2,
	MID_RECEIVE_STANDARD_TASK=3,
	MID_RECEIVE_EIT_TASK=4,
	MID_BOOT_TASK=5,
	MID_FILTER_TASK=6
}Mid_Task_Id_e;

typedef enum StreamType
{
	INVALID_STREAM_TYPE,
	VIDEO_STREAM_TYPE,
	AUDIO_STREAM_TYPE,
	PRIVATE_STREAM_TYPE,
	STREAM_TYPE_COUNT
}StreamType_e;

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/
extern int DRV_FILTReqFilterForStandard(DRVOS_MsgQueue_t *ReturnQueueId,FiltCallBack pCallbackFunc,sf_filter_mode_t	FilterMode,
										STPTI_Pid_t	ReqPid,int ReqTableId,int ReqProgNo,int	ReqSectionNo,int ReqVersionNo,
										int	nTimeOutCount,int iTsId,int iOriginalNetworkId);
extern BOOLEAN DRV_FILTFreeFilterReq(int SlotIndex);
extern int DRV_FILTReqFilterFree(FiltCallBack pCallbackFunc,sf_filter_mode_t FilterMode,unsigned char *paucFilterData,
								unsigned char *paucFilterMask,STPTI_Pid_t ReqPid,int nTimeOutCount);

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midmemset
  Description     : ����ģ���װ�ײ�memset����,��ӱ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midmemset(void *dst,int value,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midmemcpy
  Description     : ����ģ���װ�ײ�memcpy����,��ӱ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midmemcpy(void *dst,void *src,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrcpy
  Description     : ����ģ���װ�ײ�strcpy����,��ӱ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midstrcpy(char *dst,const char *src);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrncmp
  Description     : ����ģ���װ�ײ�strncmp����,��ӱ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midstrncmp(const char *dst,const char *src,int len);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midstrlen
  Description     : ����ģ���װ�ײ�strncmp����,��ӱ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midstrlen(const void *src);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : strlwr
  Description     : ���ַ�����д��ĸת����Сд��ĸ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
char *strlwr(char *str);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_FindFileExtendName
  Description     : ��������ļ�ȫ����Ѱ����չ��
  Input           : �ļ�ȫ��ָ��
  Outut           : ��չ��ָ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
char *Mid_FindFileExtendName(char *FileName);

/*****************************************************************************
  Date&Time    	  : 2012-11-15
  Author          :   wpeng
  Function Name   : Mid_CheckFileName
  Description     : ��������ļ�ȫ���м��Ƿ��ַ�
  Input           : �ļ�ȫ��ָ��
  Outut           : ���ڷǷ�ָ�� ����FALSE ���򷵻�TRUE
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_CheckFileName(char *FileName);

/*****************************************************************************
  Date&Time       : 2012-03-29
  Author          :   xyan
  Function Name   : Mid_ConvertUnicodeToGBK
  Description     : ��Unicodeת��ΪGBK
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Mid_ConvertUnicodeToGBK(unsigned char *DestData,unsigned char *SrcData,int SrcDataLen);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WriteDataInFlashBlock
  Description     : ����ģ���װ�ײ�дBlock���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_WriteDataInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadDataInFlashBlock
  Description     : ����ģ���װ�ײ��Block���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadDataInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WritePicInFlashBlock
  Description     : ����ģ���װ�ײ�дBlockͼƬ���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_WritePicInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadPicInFlashBlock
  Description     : ����ģ���װ�ײ��BlockͼƬ���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadPicLengthInFlashBlock(unsigned char ucBlockNum,unsigned int *uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadPicInFlashBlock
  Description     : ����ģ���װ�ײ��BlockͼƬ���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadPicInFlashBlock(unsigned char ucBlockNum,unsigned char *pData,unsigned int uiDataLength);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowSTBBootMpeg
  Description     : ��ʾ�����п���ͼƬ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ShowSTBBootMpeg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowSTBGBMpeg
  Description     : ��ʾ�����й㲥����ͼƬ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ShowSTBGBMpeg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadFactCfg
  Description     : ����ģ���װ�ײ��FactCfg���ݵĽӿ�
  Input           : 
  Outut           : ����FALSE: δ��ʼ��; ����TRUE:�Ѿ���ʼ��
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadFactCfg(unsigned char *pData);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ReadAppCfg
  Description     : ����ģ���װ�ײ��AppCfg���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL Mid_ReadAppCfg(unsigned char *pData);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_WriteAppCfg
  Description     : ����ģ���װ�ײ�дAppCfg���ݵĽӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void Mid_WriteAppCfg(unsigned char *pAppCfg);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_JudgeStreamType
  Description     : ���ݴ���StreamType�ж�������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int Mid_JudgeStreamType(unsigned short usStreamType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_GetSTBSerialNumber
  Description     : ��ȡ���û��������к�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char *Mid_GetSTBSerialNumber(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_SetSTBSerialNumber
  Description     : �������û��������к�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Mid_SetSTBSerialNumber(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_GetMidVersion
  Description     : ��ȡ����ģ��汾��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Mid_GetMidVersion(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Mid_ShowMidVersion
  Description     : ������ʾ����ģ��汾�ż�����ʱ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Mid_ShowMidVersion(void);

#endif
