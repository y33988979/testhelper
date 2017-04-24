/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveAUT.h

    Description : ����Ƶ�����湦��

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15
                  
*******************************************************************************/
#ifndef _YSstbdata_updataAUT_
#define _YSstbdata_updataAUT_
/*----------------------------------------------------------------------------
*	Include
*----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
*	Define
*----------------------------------------------------------------------------*/
#define APPLICATION_UPDATA_TABLE_PID		0X1da1
#define APPLICATION_UPDATA_TABLE_ID			0Xda    
#define SECTION_WAIT_TIMEOUT_AUT_MS       	2000 

#define PRIVATE_SET_DESC 					0X80
#define PRIVATE_COMPONENT_DESC 				0X01
#define DEFAULT_CHANNEL_REGION_CODE  		1
#define CABLE_DELIVERY_SYSTEM_REGION_DESC 	0X84
#define STREAM_IDENTIFIER_DESC_AUT			0x52

/*----------------------------------------------------------------------------
*	Struct
*----------------------------------------------------------------------------*/
typedef struct AppDataSection
{
    unsigned char 	aucSectionData[1024];	
    unsigned short  usDataLength;
    BOOL 			bInUsed;				
}AppDataSection_t;

enum
{
    CHANNEL_ORDER_NOT_CHANGED,
    CHANNEL_ORDER_CHANGED
};

/*�洢��service ��Ϣ�ṹ*/
typedef struct STBDATAServiceInfoTemp
{
    char 			    scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];		/*service���� */
    unsigned short		usStatus;				/* service��״̬ */
    signed short		ssServiceNO;			/* �����±�*/
    unsigned short		usServiceID;			/* Service ID */

    unsigned short		usVideoPID;				/* ��Ƶpid */
    unsigned short		usAudio1PID;			/* ��Ƶ1pid */
    unsigned short		usPcrPID;				/* pcr pid */

    signed char         scTsNO;					/*��service ��Ӧ��TS���*/
    unsigned char       ucServiceType;                    
    unsigned char       ucVideoType;			/* ��Ƶ����*/
    unsigned char       ucAudio1Type;			/* ��Ƶ1���� */

    unsigned short		usVideoEcmPID;			/*vedio ecm pid */
    unsigned short		usAudio1EcmPID;			/*audio ecm pid */
    unsigned short		usEmmPID;				/* emm pid */
    unsigned short      usCaSID;				/* CA��ʶ�� */

    unsigned short      usChannelOrderNO;
    unsigned char       ucChnlStatus;
    unsigned char       ucChnlLock;
    unsigned char       ucAttribute;

    signed char			scVolumeOffset;			/*��������ֵ*/
    unsigned char		ucSoundChannel;			/*Ƶ�������趨ֵ*/

	unsigned short      usAuthority;            /*��Ŀ��Ȩ*/
	unsigned short      usPmtPid;               /*Pmtpid*/
	unsigned short      usNetChnlIndex;         /*����Ƶ����*/

}STBDATAServiceInfoTemp_t;

typedef struct STBDATANVODUnit_Info
{
    unsigned short  	usNVOD_TSID;
    unsigned short  	usNVOD_OriginalNetWorkID;
    unsigned short  	usNVOD_ServiceID;
    unsigned char   	ucStatus; 
    signed short     	ssServiceNO;    		/* ��Ƶ�� ��Ӧ��service���*/
}STBDATANVODUnit_t;

typedef struct STBDATANVODChannelInfoTemp
{
    STBDATANVODUnit_t 	NVODInfo[STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER];
    signed short		ssChannelNo;          	/*Ƶ����*/
    signed short    	ssServiceNO;    		/* ��Ƶ�� ��Ӧ��service���*/
    unsigned char       ucStatus;   			/* Ƶ��״̬ */
} STBDATANVODChannelInfoTemp_t;


/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : bIsAUTChanged
  Description     : �ж�AUT�Ƿ�ı��ʶ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL bIsAUTChanged(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : GetChannelOrderChangedTag
  Description     : ��ȡƵ����ű����ʶ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char GetChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : SetChannelOrderChangedTag
  Description     : ����Ƶ����ű����ʶ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetChannelOrderChangedTag(unsigned char ucTag);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : NVM_WriteChannelOrderChangedTag
  Description     : д��Ƶ����ű����ʶ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void NVM_WriteChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : NVM_ReadChannelOrderChangedTag
  Description     : ��ȡƵ����ű����ʶ
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void NVM_ReadChannelOrderChangedTag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : StartReceiveAppData
  Description     : ��ʼ����Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void StartReceiveAppData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : FinishReceiveAppData
  Description     : ��������Ƶ������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void FinishReceiveAppData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : GetAppDataReceiveStatus
  Description     : ��ȡ����Ƶ������״̬
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL GetAppDataReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : STBDATA_RequestApplicationData
  Description     : �������Ƶ����������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void STBDATA_RequestApplicationData(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :    llh
  Function Name   : UpdateApplicationData_New
  Description     : ����Ӧ��(Ƶ��)����(�����µ�Ƶ����������ݽṹ)
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void UpdateApplicationData_New(unsigned short StartRequestStatus);


#endif


