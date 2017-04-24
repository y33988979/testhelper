/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbtablemgr.h

    Description : 

    Others:      

    History:    1. llh creat 2004-3-29
				2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSDVBTABLEMGR_
#define _YSDVBTABLEMGR_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h"
#include "dvb.h"
#include "driver.h"
#include "dvbtabledata.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
typedef enum GetEventMode
{
	GET_EVENT_INVALID_MODE=0,	/*��Ч��Evnet��ȡģʽ*/
	GET_EVENT_ID_MODE=1,      	/*ͨ��EventID��ȡģʽ*/
	GET_EVENT_TIME_MODE=2,      /*ͨ��EventTime��ȡģʽ*/
	GET_EVENT_ID_TIME_MODE=3,	/*ͨ��Event��ID��Time��ȡģʽ*/
	GET_EVENT_MODE_COUNT
}GetEventMode_e;

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

struct DvbService ;

typedef struct DvbEPGSectionSegment
{
	signed char			scSegmentNo;
	signed short		ssSegmentLastSectionNo;
	signed char         SectionReceived[8];
} DvbEPGSectionSegment_t;


typedef struct DvbEPGSubTable
{
	unsigned char       ucSubTableId;
	signed short		ssLastSectionNo;
	list_t				EPGSectionSegmentList;
} DvbEPGSubTable_t;


typedef struct DvbEPGIndex
{
	signed short       	scTimeOut;
	signed int          siReceivingStartTime;
	signed short		ssLastTableId;
	list_t				EPGSubTableList;
} DvbEPGIndex_t;


typedef struct DvbBouquet
{
	unsigned char		szBouquetName[DVB_INFO_MAX_BOUQUET_NAME_LENGTH];  
	unsigned char       ucBATVersionNo;
	unsigned short      uiBouquetID; 
	list_t              BouquetServiceIndexList;	
} DvbBouquet_t;


typedef struct DvbBouquetService
{
	unsigned short		uiOriginalNetworkID;//original network ID
	unsigned short		uiTsID;				//TS ID
	unsigned short		uiServiceID; 
	unsigned short		uiServiceType; 
	signed short        uiChnlIndexNo;
	unsigned char		ucSoundChannel;
	signed char			scVolOffset;
}DvbBouquetService_t ;


typedef struct DvbDataBroadcast
{
	unsigned short		usDataBroadcastID; 
	unsigned char		ucComponentTag;
	void        		*pPData;
    unsigned char       Text[DVB_INFO_MAX_DATA_BROADCAST_TEXT_LENGTH];
} DvbDataBroadcast_t ;


typedef struct DvbDataCarousel
{
	unsigned char		ucCarouselTypeID; 
	unsigned int		uiTransactionID;
	unsigned int       	uiDSITimeOut;
    unsigned int        uiDIITimeOut;
	unsigned int        uiLeakRate;
} DvbDataCarousel_t ;


typedef struct DvbNVODReference
{	
	unsigned short		uiOriginalNetworkID;//original network ID
	unsigned short		uiTsID;				//TS ID
	unsigned short		uiServiceID; 
}DvbNVODReference_t ;


typedef struct MosaicLinkage
{
	unsigned short		usOriginalNetworkID;
	unsigned short		usTsID;				
	unsigned short		usServiceID; 
	unsigned short 		usBouquetID;
	unsigned short 		usEventID;
}MosaicLinkage_t;


typedef struct DvbMosaic
{
	list_t				MosaicLogicalCellList;
	unsigned char 		ucStatus;
	BOOL 				bEntryPoint;
	unsigned char		ucHorizontalCellNo;
	unsigned char		ucVerticalCellNo;
	unsigned short		usScreenLeft;
	unsigned short		usScreenTop;
	unsigned short		usScreenRight;
	unsigned short		usScreenBottom;
	MosaicLinkage_t 	*pPrevMosaic;
	MosaicLinkage_t 	*pNextMosaic;
}DvbMosaic_t;


typedef struct DvbMosaicLogicalCell
{
	unsigned char 		ucLogicalCellID;
	unsigned short 		usAudioPID;
	unsigned char 		ucAudioType;
	unsigned short 		usAudioEcmPID;
	unsigned char 		ucPresentationInfo;
	unsigned char 		ucElemCellNo;
	unsigned char 		*pElementaryCellID;
	unsigned char 		ucLinkageInfo;
	MosaicLinkage_t 	*pCellLinkage;
}DvbMosaicLogicalCell_t;


typedef struct DvbCable
{
	unsigned int		uiFrequency;
	unsigned short		uiFECOuter;
	unsigned short		uiFECInner;
	unsigned short		uiModulation;
	unsigned int		uiSymbolRate;
} DvbCable_t;

//////////////////////////////////////////////////////////////////////
// Satellite  Struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbSatellite
{
	unsigned int		uiFrequency;
	unsigned short		uiOrbitalPosition;
	BOOL				bWestEastFlag; //"0" : western position; "1": eastern position
	unsigned short		uiPolarization;//specifying the polarisation of the transmitted signal.
	unsigned short		uiModulation;
	unsigned int		uiSymbolRate;
	unsigned short		uiFECInner;
} DvbSatellite_t;

//////////////////////////////////////////////////////////////////////
// Terrestrial  Struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbTerrestrial
{
	unsigned int		uiCentreFrequency;
	unsigned short		uiBandWidth; //the value is "8" or "7"
	unsigned short		uiConstellation;//specifies the constellation pattern used on a terrestrial delivery system
	unsigned short		uiHierarchyInfo;//specifies whether the transmission is 
	unsigned short		uiCodeRateHPStream;
	unsigned short		uiCodeRateLPStream;
	unsigned short		uiGuardInterval;
	unsigned short		uiTransmissionMode;                  
	BOOL				bOtherFrequencyFlag;
} DvbTerrestrial_t;

typedef struct DvbDelivery
{
	unsigned short		uiDeliveryType;			//delivery type
	union {
		DvbCable_t		stCable;
		DvbSatellite_t	stSate;
		DvbTerrestrial_t stTerr;
	} Content;
} DvbDelivery_t;


//////////////////////////////////////////////////////////////////////
// CCa  Struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbCa 
{
	unsigned short		uiPID;				//ECM PID
	unsigned short		uiCaSID;			//CAS_ID
	char				szCaProviderName[DVB_INFO_MAX_CAPROVIDER_NAME_LENGTH];	//the ca's name
} DvbCa_t;

//////////////////////////////////////////////////////////////////////
// Network Class
//////////////////////////////////////////////////////////////////////
typedef struct DvbNetwork
{
	unsigned short		uiNetworkID;	//network id
	char				szNetworkName[DVB_INFO_MAX_NETWORK_NAME_LENGTH];
	unsigned char		uchVersion;
	list_t				TsList;
	unsigned short		nStatus;
} DvbNetwork_t;

//////////////////////////////////////////////////////////////////////
// Ts  Struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbTs
{
	unsigned short		uiTsID;				//TS ID
	unsigned short		uiOriginalNetworkID;//original network ID
	unsigned short		uiNetworkID;		//network ID
	DvbDelivery_t		Delivery;
	list_t				ServiceList;
	unsigned char       ucSDTVersionNo;
	unsigned char       ucPATVersionNo;

	DvbCa_t				EmmInfo;

	unsigned short		nStatus;
	BOOL             	bSDTReceived;
	BOOL             	bPATReceived;
	BOOL				bCATReceived;
	DvbNetwork_t*		pDvbNetwork;		
} DvbTs_t;

//////////////////////////////////////////////////////////////////////
// Component  Struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbComponent
{
	unsigned short		uiStreamType;		
	unsigned short      uiStreamContent;	//Stream content got from component descriptor
	unsigned short		uiEsPID;			// ES PID
	unsigned short      uiEsTag;			
	char				szLanguageCode[3];	//the 3-charactors language code
	char				szDescription[DVB_INFO_MAX_ES_DESCRIPTION_LENGTH];	//ES descriptor
	BOOL				bScrambled; // 1: scrambled 
	DvbCa_t				EcmInfo;
	unsigned short		nStatus;
	unsigned char       ucLanguageCodeNumber;

	list_t				SubtitleList;
} DvbComponent_t;


typedef struct DvbSubtitle
{
    unsigned char       ucSubtitlingType;
    unsigned short		usCompositionPageId;
    unsigned short      usAncillaryPageId;
    char			    szLanguageCode[3];	//the 3-charactors language code
}DvbSubtitle_t;


typedef struct DvbContentNibble
{
	unsigned char		ucNibbleLevel1;
	unsigned char   	ucNibbleLevel2;
	unsigned char		ucUserNibble1;
	unsigned char		ucUserNibble2;
}DvbContentNibble_t;


typedef struct DvbItemInfo
{
	unsigned char		ucItemDescLength;
	unsigned char 		*ucItemDesc;
	unsigned char		ucItemLength;
	unsigned char		*ucItem;
}DvbItemInfo_t;

typedef struct DvbExtendedEventInfo
{
	unsigned char		ucExtendedEventInfoNo;
	unsigned char		ucLanguageCode[3];
	unsigned char 		ucTextLength;
	unsigned char 		*ucText;
	list_t				ItemList;
}DvbExtendedEventInfo_t;

//////////////////////////////////////////////////////////////////////
// DvbEvent  struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbEvent
{
	DWORD				dwStartTime;  
	DWORD				dwEndTime;	
	char				szEventName[DVB_INFO_MAX_EVENT_NAME_LENGTH];				//event name
	char				bPreOrder;			//TRUE: PreOrder
	char				bFreeCA;			//TRUE: scrambled
	unsigned short      uiEventID;
	unsigned short      uiReferenceEventID;
	signed short        ssDescriptionLength;
	char				*szEventDescription;//event descriptor

	unsigned short		uiEventType;		//event type
	unsigned short		nStatus;
	unsigned char 		ucLanguageIndex;

	list_t				ContentNibbleList;
	list_t				ExtendedEventList;
	
	//unsigned char		uiRunningStatus;
	//TFCAIPPVInfo_t    *pEventIPPV;
	//list_t			RatingList;
	//list_t			ComponentList;
	//BOOL              bInteractive;
	//BOOL              bPreRecord;
	
	struct DvbService	*pDvbService;		
}DvbEvent_t;


//////////////////////////////////////////////////////////////////////
// Service  struct
//////////////////////////////////////////////////////////////////////
typedef struct DvbService
{
	unsigned short		uiApplicationID;  //identifier of application
	unsigned short		uiServiceType; 
	unsigned short		uiServiceID; 
	unsigned short		uiReferenceServiceID; 
	BOOL				bMultilingual;	
	char				szServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];
	char				szProviderName[DVB_INFO_MAX_PROVIDER_NAME_LENGTH];
	
	
	BOOL				bEitS;					//eit_schedule_flag used in SDT 
	BOOL				bEitPF;					//eit_prsent_following_flag used in SDT
	unsigned char		ucRunStatus;			//running_status used in SDT
	unsigned short		uiPcrPid;
	unsigned short		PmtPID;

	unsigned char       ucPMTVersionNo;
	unsigned char       ucEITSDVersionNo;
	unsigned char       ucEITPFVersionNo;
	
	
	DvbCa_t				EcmInfo;			//the pid of Ecm this service being scrambled by the following CA provider

	DvbEvent_t          *pPresentEvent;
	DvbEvent_t          *pFollowingEvent;
	DvbMosaic_t			*pDvbMosaicInfo;

	DvbEPGIndex_t       *pEPGIndex;
	BOOL             	bEPGDataChanged;
	BOOL             	EventReceiveComplete;
	list_t				EventList;				//all events belonging to this service

	list_t				ComponentList;			//all component belonging to this service 
	list_t				NVODReferenceList;

	list_t				DataBroadcastList;
	BOOL				bFavorite;				//TRUE: favorite channel
	BOOL				bFreeCA;				//TRUE: Scrambled
	BOOL                bPMTReceived;             

	unsigned short		nStatus;				//the status of the table item, both for running and building
	DvbTs_t				*pDvbTs;	

	signed short        uiChnlIndexNo;

	unsigned char		ucSoundChannel;
	signed char			scVolOffset;

	BOOL 				bSubscribedFlag;

	unsigned short      ServiceInformationLength;               
	unsigned char       *ServiceInformation;   
}DvbService_t ;


//////////////////////////////////////////////////////////////////////
// DvbSI
//////////////////////////////////////////////////////////////////////
typedef struct DvbSI {
	list_t				NetworkList;	//the pointer to the list of network
	list_t              BouquetList;

	long				nIndexHead;		//the head index of the si information
	long				nIndexEnd;		//the end index of the si information
	unsigned short		nStatus;		//the status of the si information
	unsigned char       ucNITVersionNo;
	unsigned char       ucNITOtherVersionNo;
	BOOL             	bNITReceived;
} DvbSI_t;


/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
 
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSICreate
  Description     : ��������ʼ�� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSI_t *DvbSICreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbNetworkCreate
  Description     :  ��������ʼ�� DvbNetwork_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNetwork_t *DvbNetworkCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCreate
  Description     : ��������ʼ�� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbTs_t *DvbTsCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCreate
  Description     : ��������ʼ�� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbService_t *DvbServiceCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMosaicCreate
  Description     : ��������ʼ�� DvbMosaic_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbMosaic_t * DvbMosaicCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMosaicCellCreate
  Description     : ��������ʼ�� DvbMosaicLogicalCell_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbMosaicLogicalCell_t * DvbMosaicCellCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEventCreate
  Description     : ��������ʼ�� DvbEvent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEvent_t *DvbEventCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbContentNibbleCreate
  Description     : ��������ʼ�� DvbContentNibble_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbContentNibble_t *DvbContentNibbleCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbItemInfoCreate
  Description     : ��������ʼ�� DvbItemInfo_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbItemInfo_t *DvbItemInfoCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbExtendedEventInfoCreate
  Description     : ��������ʼ�� DvbExtendedEventInfo_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbExtendedEventInfo_t *DvbExtendedEventInfoCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbComponetCreate
  Description     : ��������ʼ�� DvbComponent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbComponent_t *DvbComponentCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSubtitleCreate
  Description     : ��������ʼ�� DvbSubtitle_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSubtitle_t *DvbSubtitleCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbCaCreat
  Description     : ��������ʼ�� DvbCa_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbCa_t  *DvbCaCreat(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbNVODReferenceCreate
  Description     : ��������ʼ��DvbNVODReference_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNVODReference_t *DvbNVODReferenceCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetCreate
  Description     : ��������ʼ�� DvbBouquet_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBouquet_t *DvbBouquetCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetServiceCreate
  Description     : ��������ʼ�� DvbBouquetService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBouquetService_t *DvbBouquetServiceCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbDataBroadcastCreate
  Description     : ��������ʼ�� DvbDataBroadcast_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbDataBroadcast_t *DvbDataBroadcastCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbDataCarouselCreate
  Description     : ��������ʼ�� DvbDataCarousel_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbDataCarousel_t *DvbDataCarouselCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGIndexCreate
  Description     : ��������ʼ�� DvbEPGIndex_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEPGIndex_t *DvbEPGIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGSubTableCreate
  Description     : ��������ʼ�� DvbEPGSubTable_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEPGSubTable_t *DvbEPGSubTableCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGSectionSegmentCreat
  Description     : ��������ʼ�� DvbEPGSectionSegment_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEPGSectionSegment_t  *DvbEPGSectionSegmentCreat(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSIDelete
  Description     : ɾ��ָ��pDvbSi��ָ�� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbSIDelete(void *pDvbSi, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbNetworkDelete
  Description     : ɾ��ָ��pNetwork��ָ�� DvbNetwork_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbNetworkDelete(void *pNetwork, DRVOS_Partition_t *pPtn); 

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsDelete
  Description     : ɾ��ָ��pTs��ָ��DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbTsDelete(void *pTs, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceDelete
  Description     : ɾ��ָ��pService��ָ�� DvbService �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbServiceDelete(void *pService, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMosaicDelete
  Description     : ɾ��ָ��pMosaic��ָ�� DvbMosaic_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DvbMosaicDelete(void *pMosaic, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMosaicCellDelete
  Description     : ɾ��ָ��pMosaicLogicalCell��ָ�� DvbMosaicLogicalCell_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DvbMosaicCellDelete(void *pMosaicLogicalCell, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEventDelete
  Description     : ɾ��ָ��pEvent��ָ��� DvbEvent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbEventDelete(void *pEvent, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbContentNibbleDelete
  Description     : ɾ��ָ��pItem��ָ��� DvbItemInfo_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DvbContentNibbleDelete(void *pContentNibble,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbItemInfoDelete
  Description     : ɾ��ָ��pItem��ָ��� DvbItemInfo_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbItemInfoDelete(void *pItem,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbExtendedEventInfoDelete
  Description     : ɾ��ָ��pItem��ָ��� DvbExtendedEventInfo_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbExtendedEventInfoDelete(void *pExtendedEventInfo,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbComponentDelete
  Description     : ɾ��ָ��pComponent��ָ��� DvbComponent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbComponentDelete(void *pComponent, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSubtitleDelete
  Description     : ɾ��ָ��pSubtitle��ָ���DvbSubtitle_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbSubtitleDelete(void *pSubtitle, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbNVODReferenceDelete
  Description     : ɾ��ָ��pNVODReference��ָ��� DvbNVODReference_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbNVODReferenceDelete(void *pNVODReference, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetDelete
  Description     : ɾ��ָ��pBouquet��ָ��� DvbBouquet_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbBouquetDelete(void *pBouquet, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetServiceDelete
  Description     : ɾ��ָ��pBouquetService��ָ��� DvbBouquetService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbBouquetServiceDelete(void *pBouquetService, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbDataBroadcastDelete
  Description     : ɾ��ָ��pDataBroadcast��ָ��� DvbDataBroadcast_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbDataBroadcastDelete(void *pData, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGIndexDelete
  Description     : ɾ��ָ��pEPGIndex��ָ��� DvbEPGIndex_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbEPGIndexDelete(void *pEPGIndex, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGSubTableDelete
  Description     : ɾ��ָ��pSubTable��ָ��� DvbEPGSubTable_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbEPGSubTableDelete(void *pSubTable, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEPGSectionSegmentDelete
  Description     : ɾ��ָ��pSegment��ָ��� DvbEPGSectionSegment_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbEPGSectionSegmentDelete(void *pSegment, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSiCopy
  Description     : ���� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbSiCopy(DvbSI_t *pDvbSiDestination,DvbSI_t *pDvbSiSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSiCopyForNIT
  Description     : ��NIT�п��� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbSiCopyForNIT(DvbSI_t *pDvbSiDestination,DvbSI_t *pDvbSiSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSiCopyForNITOther
  Description     : ��NIT_OTHER�п��� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbSiCopyForNITOther(DvbSI_t *pDvbSiDestination,DvbSI_t *pDvbSiSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSiCopyForBAT
  Description     : ��BAT�п��� DvbSI_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbSiCopyForBAT(DvbSI_t *pDvbSiDestination,DvbSI_t *pDvbSiSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbNetworkCopy
  Description     : ���� DvbNetwork_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/  
BOOL DvbNetworkCopy(DvbNetwork_t *pNetworkDestination,DvbNetwork_t *pNetworkSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCopy
  Description     : ���� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbTsCopy(	DvbTs_t *pTsDestination,DvbTs_t *pTsSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCopyForSDT
  Description     : ��SDT�п��� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbTsCopyForSDT(DvbTs_t *pTsDestination,DvbTs_t *pTsSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCopyForPSI
  Description     : ��CAT�п��� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbTsCopyForCAT(DvbTs_t *pTsDestination,DvbTs_t *pTsSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCopyForPSI
  Description     : ��PSI�п��� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbTsCopyForPSI(DvbTs_t *pTsDestination,DvbTs_t *pTsSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbTsCopyForSI
  Description     : ��SI�п��� DvbTs_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbTsCopyForSI(DvbTs_t *pTsDestination,DvbTs_t *pTsSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopy
  Description     : ���� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopy(DvbService_t *pServiceDestination,DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForSDT
  Description     : ��SDT�п��� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForSDT(DvbService_t *pServiceDestination, DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMosaicCopyForSDT
  Description     : ��SDT�п���DvbMosaic_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbMosaicCopyForSDT(DvbMosaic_t * pMosaicDestination, DvbMosaic_t * pMosaicSource, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForEITSD
  Description     : ��EITSD�п��� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForEITSD(DvbService_t *pServiceDestination,DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForEITPFASSD
  Description     : ��EIT PF DvbService_t�ṹ�µ�EVENT��Ϣ������EIT SCHEDULE_ACTUALL
                    DvbService_t�ṹ�£�������NVOD EVENT ��Ϣ�Ŀ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForEITPFASSD(DvbService_t *pServiceDestination,DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForEITPF
  Description     : ��EIT PF�е�EVENT��Ϣ������ DvbService_t�� 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForEITPF(DvbService_t *pServiceDestination,DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn,int SectionNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForPSI
  Description     : ��PSI�п��� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForPSI(DvbService_t *pServiceDestination,DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbServiceCopyForSI
  Description     : ��SI�п��� DvbService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbServiceCopyForSI(DvbService_t *pServiceDestination, DvbService_t *pServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbEventCopy
  Description     : ���� DvbEvent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbEventCopy(DvbEvent_t *pEventDestination, DvbEvent_t *pEventSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbComponentCopy
  Description     : ���� DvbComponent_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbComponentCopy(DvbComponent_t *pComponentDestination,DvbComponent_t *pComponentSource,DRVOS_Partition_t *pPtn);
 
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbDataBroadCastCopyForSDT
  Description     : ��SDT�п��� DvbDataBroadcast_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbDataBroadCastCopyForSDT(DvbDataBroadcast_t *pDataBroadcastDestination,DvbDataBroadcast_t *pDataBroadcastSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetCopy
  Description     : ���� DvbBouquet_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbBouquetCopy(DvbBouquet_t *pBouquetDestination,DvbBouquet_t *pBouquetSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBouquetServiceCopy
  Description     : ���� DvbBouquetService_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbBouquetServiceCopy(	DvbBouquetService_t *pBouquetServiceDestination,DvbBouquetService_t *pBouquetServiceSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSubtitleCopy
  Description	  : ����Subtitle_t�ṹ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL DvbSubtitleCopy(DvbSubtitle_t *pSubtitleDestination,DvbSubtitle_t  *pSubtitleSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author		  :   xyan
  Function Name   : DvbContentNibbleCopy
  Description	  : ����DvbContentNibble_t�ṹ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL DvbContentNibbleCopy(DvbContentNibble_t *pNibbleDestination,DvbContentNibble_t  *pNibbleSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author		  :   xyan
  Function Name   : DvbExtendedEventInfoCopy
  Description	  : ����DvbExtendedEventInfo_t�ṹ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL DvbExtendedEventInfoCopy(DvbExtendedEventInfo_t *pExtendedEventInfoDestination,DvbExtendedEventInfo_t *pExtendedEventInfoSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author		  :   xyan
  Function Name   : DvbItemInfoCopy
  Description	  : ����DvbItemInfo_t�ṹ
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL DvbItemInfoCopy(DvbItemInfo_t *pItemDestination,DvbItemInfo_t *pItemSource,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DVBGetNetworkOfID
  Description     : ����NetID��pDvbSi�еõ��ض���DvbNetwork_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNetwork_t* DVBGetNetworkOfID(DvbSI_t *pDvbSi,NET_ID sNetID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstNetworkFromSI
  Description     : ��pDvbSi�еõ���һ���ض���DvbNetwork_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNetwork_t* DvbGetFirstNetworkFromSI(DvbSI_t *pDvbSi);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstNetworkFromSI
  Description     : ��pDvbSi�еõ���һ���ض���DvbNetwork_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNetwork_t* DvbGetNextNetworkFromSI(DvbSI_t *pDvbSi);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetTsFromSIOfID
  Description     : ����NetID��TsID��pDvbSi�õ��ض���DvbTs_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbTs_t* DvbGetTsFromSIOfID(DvbSI_t *pDvbSi,NET_ID sNetID, TS_ID sTsID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetTsFromNetworkOfID
  Description     : ����TsID��pNet�еõ��ض���DvbTs_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbTs_t* DvbGetTsFromNetworkOfID(DvbNetwork_t* pNet, TS_ID sTsID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstTsFromNetwork
  Description     : ����Status��Mask��pNet�еõ���һ���ض���DvbTs_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbTs_t* DvbGetFirstTsFromNetwork(DvbNetwork_t *pNet,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetNextTsFromNetwork
  Description     : ����Status��Mask��pNet�еõ���һ���ض���DvbTs_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbTs_t* DvbGetNextTsFromNetwork(DvbNetwork_t *pNet,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetServiceFromSIOfID
  Description     : ����NetID��TsID��ServiceID��pDvbSi�õ��ض���DvbService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbService_t* DvbGetServiceFromSIOfID(DvbSI_t *pDvbSi,NET_ID sNetID, TS_ID sTsID, PRG_ID sPrgID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetServiceFromTsOfID
  Description     : ����ServiceID��pTs�еõ��ض���DvbService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbService_t* DvbGetServiceFromTsOfID(DvbTs_t *pTs, PRG_ID sPrgID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstServiceFromTs
  Description     : ����Status��Mask��pTs�еõ���һ��DvbService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbService_t* DvbGetFirstServiceFromTs(DvbTs_t *pTs,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetNextServiceFromTs
  Description     : ����Status��Mask��pTs�еõ���һ��DvbService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbService_t* DvbGetNextServiceFromTs(DvbTs_t *pTs,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetEventFromServiceOfEvent
  Description     : ����Event��Service�еõ��ض���DvbEvent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEvent_t* DvbGetEventFromServiceOfEvent(DvbService_t* pPrg,DvbEvent_t *pSourceEvent,unsigned int uiGetEventMode);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstEventFromService
  Description     : ����Status��Mask��Service�еõ���һ���ض���DvbEvent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEvent_t* DvbGetFirstEventFromService(DvbService_t *pPrg,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetNextEventFromService
  Description     : ����Status��Mask��Service�еõ���һ���ض���DvbEvent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEvent_t* DvbGetNextEventFromService(DvbService_t *pPrg,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetComponentFromServiceOfID
  Description     : ����EsTag��Service�еõ��ض���DvbComponent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbComponent_t* DvbGetComponentFromServiceOfID(DvbService_t* pPrg, COMP_ID sCmpID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetComponentFromServiceOfID
  Description     : ����EsTag��Service�еõ��ض���DvbComponent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSubtitle_t* DvbGetSubtitleFromComponentOfInfo(DvbComponent_t* pComponent,DvbSubtitle_t *pSubtitleInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetContentNibbleFromEventOfInfo
  Description     : ����DvbContentNibble_t��Event�еõ��ض���DvbContentNibble_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbContentNibble_t* DvbGetContentNibbleFromEventOfInfo(DvbEvent_t* pEvent,DvbContentNibble_t *pContentNibble);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetExtendedEventInfoFromEventOfNo
  Description     : ����No��Event�еõ��ض���DvbExtendedEventInfo_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbExtendedEventInfo_t *DvbGetExtendedEventInfoFromEventOfNo(DvbEvent_t *pEventDestination,unsigned char ucExtendedEventInfoNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetItemInfoFromExtendedEventOfInfo
  Description     : ����DvbItemInfo_t��Event�еõ��ض���DvbItemInfo_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbItemInfo_t *DvbGetItemInfoFromExtendedEventOfInfo(DvbExtendedEventInfo_t *pExtendedEventDestination,DvbItemInfo_t *pItemSource);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetFirstComponentFromService
  Description     : ����Status��Mask��Service�еõ���һ���ض���DvbComponent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbComponent_t* DvbGetFirstComponentFromService(DvbService_t *pPrg,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetNextComponentFromService
  Description     : ����Status��Mask��Service�еõ���һ���ض���DvbComponent_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbComponent_t* DvbGetNextComponentFromService(DvbService_t *pPrg,unsigned int nStatus,unsigned int nMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetDataBroadcastFromServiceByTag
  Description     : ����ComponentTag��Service DataBroadcastList�еõ��ض���DvbDataBroadcast_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbDataBroadcast_t *DvbGetDataBroadcastFromServiceByTag(DvbService_t  *pService, U8 ucComponentTag);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetBouquetFromSIByID
  Description     : ����BouquetID��pDvbSi�еõ��ض���DvbBouquet_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBouquet_t *DvbGetBouquetFromSIByID( DvbSI_t *pDvbSi, U16  usBouquetID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetBouquetServiceFromBouquetByID
  Description     : ����OriginalNetworkID��TsID��ServiceID��pBouquet�еõ��ض���DvbBouquetService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBouquetService_t *DvbGetBouquetServiceFromBouquetByID( DvbBouquet_t *pBouquet,U16  usOriginalNetworkID,U16  usTsID,U16  usServiceID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetBouquetServiceFromAllBouquetByID
  Description     : ����OriginalNetworkID��TsID��ServiceID�����е�Bouquet�����еõ�
  					�ض���DvbBouquetService_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBouquetService_t *DvbGetBouquetServiceFromAllBouquetByID( DvbSI_t *pDvbSi,U16  usOriginalNetworkID,U16  usTsID,U16  usServiceID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetEITSubTableFromIndexByID
  Description     : ����SubTableId��pEPGIndex�еõ��ض���DvbEPGSubTable_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEPGSubTable_t* DvbGetEITSubTableFromIndexByID(DvbEPGIndex_t* pEPGIndex, U8 ucSubTableId);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetEITSegmentFromSubtableByID
  Description     : ����SegmentNo��pSubTable�еõ��ض���DvbEPGSectionSegment_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbEPGSectionSegment_t* DvbGetEITSegmentFromSubtableByID(DvbEPGSubTable_t* pSubTable, S8 scSegmentNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbInsertEventBefore
  Description     : ����ʱ���Ⱥ�˳��EVENT���뵽 pList������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DvbInsertEventBefore(list_t *pList, DWORD iEventStartTime, void *pData );

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetNVODReferenceFromServiceOfID
  Description     : Get NVOD Reference From Service By ID
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbNVODReference_t *DvbGetNVODReferenceFromServiceOfID(DvbService_t *pService,unsigned short usNetworkId,unsigned short usTsId,unsigned short usServiceId);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbGetLogicalCellFromMosaicOfID
  Description     : Get Mosaic Logical Cell From Mosaic By ID
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbMosaicLogicalCell_t *DvbGetLogicalCellFromMosaicOfID(DvbMosaic_t *pMosaic,unsigned char ucLogicalCellID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : SetRecSucStatus
  Description     : ���ñ�׼�����״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetRecSucStatus(unsigned short *ucStatus,unsigned short usSucMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : GetRecStatus
  Description     : �õ���׼�����״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char GetRecStatus(unsigned short ucStatus,unsigned short usSucMask);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSetSimpleFilterStatus
  Description     : �����±�ΪFilterNo��FILTER״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void  DvbSetSimpleFilterStatus(signed char FilterNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbInitSimpleFilterStatus
  Description     : ��ʼ���±�ΪFilterNo��FILTER״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void  DvbInitSimpleFilterStatus(signed char FilterNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbInitAllFilterStatus
  Description     : ��ʼ������FILTER״̬
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void  DvbInitAllFilterStatus(void);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSectionIndexDelete
  Description     : ɾ��ָ��pSectionIndex��ָ��DvbSectionIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbSectionIndexDelete(void *pSectionIndex, DRVOS_Partition_t *pPtn); 

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSigleTableIndexDelete
  Description     : ɾ��ָ��pSigleTableIndex��ָ��DvbSigleTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbSigleTableIndexDelete(void *pSigleTableIndex, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBATSubTableIndexDelete
  Description     : ɾ��ָ��pBATSubTableIndex��ָ��DvbBATSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbBATSubTableIndexDelete(void *pBATSubTableIndex, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSDTSubTableIndexDelete
  Description     : ɾ��ָ��pSDTSubTableIndex��ָ��DvbSDTSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbSDTSubTableIndexDelete(void *pSDTSubTableIndex, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbMulTableIndexDelete
  Description     : ɾ��ָ��pMulTableIndex��ָ��DvbMulTableIndex_t �ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DvbMulTableIndexDelete(void *pMulTableIndex, DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSectionIndexCreate
  Description     : ��������ʼ��DvbSectionIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSectionIndex_t *DvbSectionIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSigleTableIndexCreate
  Description     : ��������ʼ��DvbSigleTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSigleTableIndex_t *DvbSigleTableIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBATSubTableIndexCreate
  Description     : ��������ʼ��DvbBATSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBATSubTableIndex_t *DvbBATSubTableIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSDTSubTableIndexCreate
  Description     : ��������ʼ��DvbSDTSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSDTSubTableIndex_t *DvbSDTSubTableIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbBATMulTableIndexCreate
  Description     : ��������ʼ��BAT��DvbMulTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbMulTableIndex_t *DvbBATMulTableIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DvbSDTMulTableIndexCreate
  Description     : ��������ʼ��SDT��DvbMulTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbMulTableIndex_t *DvbSDTMulTableIndexCreate(DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : GetDvbSectionIndexByNo
  Description     : ͨ��SectionNo��ȡDvbSectionIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSectionIndex_t *GetDvbSectionIndexByNo(list_t *pList,unsigned char ucSetionNo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : GetDvbBATSubTableIndexByID
  Description     : ͨ��BouquetID�õ�DvbBATSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbBATSubTableIndex_t *GetDvbBATSubTableIndexByID(DvbMulTableIndex_t *pMulTableIndex,unsigned short usBouquetID);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : GetDvbSDTSubTableIndexByID
  Description     : ͨ��NetworkID��TsID�õ�DvbSDTSubTableIndex_t�ṹ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSDTSubTableIndex_t *GetDvbSDTSubTableIndexByID(DvbMulTableIndex_t *pMulTableIndex,unsigned short usNetworkID,unsigned short TsID);
  
#endif
