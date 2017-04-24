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
	GET_EVENT_INVALID_MODE=0,	/*无效的Evnet获取模式*/
	GET_EVENT_ID_MODE=1,      	/*通过EventID获取模式*/
	GET_EVENT_TIME_MODE=2,      /*通过EventTime获取模式*/
	GET_EVENT_ID_TIME_MODE=3,	/*通过Event的ID和Time获取模式*/
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
  Description     : 创建并初始化 DvbSI_t 结构
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
  Description     :  创建并初始化 DvbNetwork_t 结构
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
  Description     : 创建并初始化 DvbTs_t 结构
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
  Description     : 创建并初始化 DvbService_t 结构
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
  Description     : 创建并初始化 DvbMosaic_t 结构
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
  Description     : 创建并初始化 DvbMosaicLogicalCell_t 结构
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
  Description     : 创建并初始化 DvbEvent_t 结构
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
  Description     : 创建并初始化 DvbContentNibble_t 结构
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
  Description     : 创建并初始化 DvbItemInfo_t 结构
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
  Description     : 创建并初始化 DvbExtendedEventInfo_t 结构
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
  Description     : 创建并初始化 DvbComponent_t 结构
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
  Description     : 创建并初始化 DvbSubtitle_t 结构
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
  Description     : 创建并初始化 DvbCa_t 结构
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
  Description     : 创建并初始化DvbNVODReference_t结构
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
  Description     : 创建并初始化 DvbBouquet_t 结构
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
  Description     : 创建并初始化 DvbBouquetService_t 结构
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
  Description     : 创建并初始化 DvbDataBroadcast_t 结构
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
  Description     : 创建并初始化 DvbDataCarousel_t 结构
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
  Description     : 创建并初始化 DvbEPGIndex_t 结构
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
  Description     : 创建并初始化 DvbEPGSubTable_t 结构
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
  Description     : 创建并初始化 DvbEPGSectionSegment_t 结构
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
  Description     : 删除指针pDvbSi所指的 DvbSI_t 结构
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
  Description     : 删除指针pNetwork所指的 DvbNetwork_t 结构
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
  Description     : 删除指针pTs所指的DvbTs_t 结构
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
  Description     : 删除指针pService所指的 DvbService 结构
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
  Description     : 删除指针pMosaic所指的 DvbMosaic_t 结构
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
  Description     : 删除指针pMosaicLogicalCell所指的 DvbMosaicLogicalCell_t 结构
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
  Description     : 删除指针pEvent所指向的 DvbEvent_t 结构
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
  Description     : 删除指针pItem所指向的 DvbItemInfo_t 结构
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
  Description     : 删除指针pItem所指向的 DvbItemInfo_t 结构
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
  Description     : 删除指针pItem所指向的 DvbExtendedEventInfo_t 结构
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
  Description     : 删除指针pComponent所指向的 DvbComponent_t 结构
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
  Description     : 删除指针pSubtitle所指向的DvbSubtitle_t结构
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
  Description     : 删除指针pNVODReference所指向的 DvbNVODReference_t 结构
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
  Description     : 删除指针pBouquet所指向的 DvbBouquet_t 结构
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
  Description     : 删除指针pBouquetService所指向的 DvbBouquetService_t 结构
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
  Description     : 删除指针pDataBroadcast所指向的 DvbDataBroadcast_t 结构
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
  Description     : 删除指针pEPGIndex所指向的 DvbEPGIndex_t 结构
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
  Description     : 删除指针pSubTable所指向的 DvbEPGSubTable_t 结构
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
  Description     : 删除指针pSegment所指向的 DvbEPGSectionSegment_t 结构
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
  Description     : 拷贝 DvbSI_t 结构
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
  Description     : 从NIT中拷贝 DvbSI_t 结构
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
  Description     : 从NIT_OTHER中拷贝 DvbSI_t 结构
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
  Description     : 从BAT中拷贝 DvbSI_t 结构
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
  Description     : 拷贝 DvbNetwork_t 结构
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
  Description     : 拷贝 DvbTs_t 结构
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
  Description     : 从SDT中拷贝 DvbTs_t 结构
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
  Description     : 从CAT中拷贝 DvbTs_t 结构
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
  Description     : 从PSI中拷贝 DvbTs_t 结构
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
  Description     : 从SI中拷贝 DvbTs_t 结构
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
  Description     : 拷贝 DvbService_t 结构
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
  Description     : 从SDT中拷贝 DvbService_t 结构
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
  Description     : 从SDT中拷贝DvbMosaic_t结构
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
  Description     : 从EITSD中拷贝 DvbService_t 结构
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
  Description     : 将EIT PF DvbService_t结构下的EVENT信息拷贝到EIT SCHEDULE_ACTUALL
                    DvbService_t结构下，常用于NVOD EVENT 信息的拷贝
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
  Description     : 将EIT PF中的EVENT信息拷贝到 DvbService_t下 
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
  Description     : 从PSI中拷贝 DvbService_t 结构
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
  Description     : 从SI中拷贝 DvbService_t 结构
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
  Description     : 拷贝 DvbEvent_t 结构
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
  Description     : 拷贝 DvbComponent_t 结构
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
  Description     : 从SDT中拷贝 DvbDataBroadcast_t 结构
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
  Description     : 拷贝 DvbBouquet_t 结构
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
  Description     : 拷贝 DvbBouquetService_t 结构
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
  Description	  : 拷贝Subtitle_t结构
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
  Description	  : 拷贝DvbContentNibble_t结构
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
  Description	  : 拷贝DvbExtendedEventInfo_t结构
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
  Description	  : 拷贝DvbItemInfo_t结构
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
  Description     : 根据NetID从pDvbSi中得到特定的DvbNetwork_t结构
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
  Description     : 从pDvbSi中得到第一个特定的DvbNetwork_t结构
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
  Description     : 从pDvbSi中得到下一个特定的DvbNetwork_t结构
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
  Description     : 根据NetID和TsID从pDvbSi得到特定的DvbTs_t结构
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
  Description     : 根据TsID从pNet中得到特定的DvbTs_t结构
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
  Description     : 根据Status和Mask从pNet中得到第一个特定的DvbTs_t结构
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
  Description     : 根据Status和Mask从pNet中得到下一个特定的DvbTs_t结构
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
  Description     : 根据NetID、TsID和ServiceID从pDvbSi得到特定的DvbService_t结构
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
  Description     : 根据ServiceID从pTs中得到特定的DvbService_t结构
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
  Description     : 根据Status和Mask从pTs中得到第一个DvbService_t结构
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
  Description     : 根据Status和Mask从pTs中得到下一个DvbService_t结构
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
  Description     : 根据Event从Service中得到特定的DvbEvent_t结构
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
  Description     : 根据Status和Mask从Service中得到第一个特定的DvbEvent_t结构
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
  Description     : 根据Status和Mask从Service中得到下一个特定的DvbEvent_t结构
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
  Description     : 根据EsTag从Service中得到特定的DvbComponent_t结构
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
  Description     : 根据EsTag从Service中得到特定的DvbComponent_t结构
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
  Description     : 根据DvbContentNibble_t从Event中得到特定的DvbContentNibble_t结构
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
  Description     : 根据No从Event中得到特定的DvbExtendedEventInfo_t结构
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
  Description     : 根据DvbItemInfo_t从Event中得到特定的DvbItemInfo_t结构
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
  Description     : 根据Status和Mask从Service中得到第一个特定的DvbComponent_t结构
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
  Description     : 根据Status和Mask从Service中得到下一个特定的DvbComponent_t结构
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
  Description     : 根据ComponentTag从Service DataBroadcastList中得到特定的DvbDataBroadcast_t结构
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
  Description     : 根据BouquetID从pDvbSi中得到特定的DvbBouquet_t结构
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
  Description     : 根据OriginalNetworkID、TsID和ServiceID从pBouquet中得到特定的DvbBouquetService_t结构
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
  Description     : 根据OriginalNetworkID、TsID和ServiceID从所有的Bouquet链表中得到
  					特定的DvbBouquetService_t结构
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
  Description     : 根据SubTableId从pEPGIndex中得到特定的DvbEPGSubTable_t结构
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
  Description     : 根据SegmentNo从pSubTable中得到特定的DvbEPGSectionSegment_t结构
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
  Description     : 按照时间先后顺序将EVENT插入到 pList链表中
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
  Description     : 设置标准表接收状态
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
  Description     : 得到标准表接收状态
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
  Description     : 设置下标为FilterNo的FILTER状态
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
  Description     : 初始化下标为FilterNo的FILTER状态
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
  Description     : 初始化所有FILTER状态
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
  Description     : 删除指针pSectionIndex所指的DvbSectionIndex_t结构
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
  Description     : 删除指针pSigleTableIndex所指的DvbSigleTableIndex_t结构
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
  Description     : 删除指针pBATSubTableIndex所指的DvbBATSubTableIndex_t结构
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
  Description     : 删除指针pSDTSubTableIndex所指的DvbSDTSubTableIndex_t结构
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
  Description     : 删除指针pMulTableIndex所指的DvbMulTableIndex_t 结构
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
  Description     : 创建并初始化DvbSectionIndex_t结构
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
  Description     : 创建并初始化DvbSigleTableIndex_t结构
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
  Description     : 创建并初始化DvbBATSubTableIndex_t结构
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
  Description     : 创建并初始化DvbSDTSubTableIndex_t结构
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
  Description     : 创建并初始化BAT的DvbMulTableIndex_t结构
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
  Description     : 创建并初始化SDT的DvbMulTableIndex_t结构
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
  Description     : 通过SectionNo获取DvbSectionIndex_t结构
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
  Description     : 通过BouquetID得到DvbBATSubTableIndex_t结构
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
  Description     : 通过NetworkID和TsID得到DvbSDTSubTableIndex_t结构
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DvbSDTSubTableIndex_t *GetDvbSDTSubTableIndexByID(DvbMulTableIndex_t *pMulTableIndex,unsigned short usNetworkID,unsigned short TsID);
  
#endif
