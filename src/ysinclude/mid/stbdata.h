/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdata.h

    Description : 频道数据管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	_YSSTBDATA_
#define	_YSSTBDATA_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "dvbtablemgr.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
 
/*定义九州私有数据开机接收掩码*/
#define START_REQUEST_STATUS_AUT 					0x0001
#define START_REQUEST_STATUS_PDT 					0x0002
#define START_REQUEST_STATUS_XDT 					0x0004
#define START_REQUEST_STATUS_ADT 					0x0008
#define START_REQUEST_STATUS_PIC 					0x0010
#define START_REQUEST_STATUS_PRG 					0x0020
#define START_REQUEST_STATUS_NIT 					0x0040
#define START_REQUEST_STATUS_EIT 					0x0080
#define START_REQUEST_ALL_STATUS  					0xffff

/*定义单个Service伴音个数*/
#define STBDATA_MAX_AUDIO_NUMBER                    10 

/*定义频道简介字符个数*/
#define SERVICE_INFORMATION_BYTES       			260

/*定义频道数据保存个数*/
#define STBDATA_MAX_TV_CHANNEL_NUMBER		        1000
#define STBDATA_MAX_RADIO_CHANNEL_NUMBER		    1000
#define STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER		10
#define STBDATA_MAX_MOSAIC_LOGICAL_CELL_NUMBER		20
#define STBDATA_MAX_MOSAIC_ELEM_CELL_NUMBER	        16
#define STBDATA_MAX_BAT_SERVICE_NUMBER              200

#ifdef USE_DATA_BLOCK_64K_MODE
/*定义保存的频道数据信息个数*/
#define STBDATA_MAX_TS_NUMBER				        100
#define STBDATA_MAX_SERVICE_NUMBER			        700
#define STBDATA_MAX_NVOD_CHANNEL_NUMBER		        20
#define STBDATA_MAX_MOSAIC_NUMBER		            5
#define STBDATA_MAX_DATABROADCAST_NUMBER	        30
#define STBDATA_MAX_BAT_NUMBER                      30

#define STBDATA_NVM_MAX_TV_CHANNEL_NUMBER           500
#define STBDATA_NVM_MAX_RADIO_CHANNEL_NUMBER        200

/*定义保存的扩展频道数据信息个数*/
#define STBDATA_NVM_MAX_AUDIO_INFO_NUMBER           200
#define STBDATA_NVM_MAX_SERVICE_INFORMATION_NUMBER  150
#define STBDATA_NVM_MAX_SUBTITLE_INFO_NUMBER  		90
#else
/*定义保存的频道数据信息个数*/
#define STBDATA_MAX_TS_NUMBER				        150
#define STBDATA_MAX_SERVICE_NUMBER			        1000
#define STBDATA_MAX_NVOD_CHANNEL_NUMBER		        40
#define STBDATA_MAX_MOSAIC_NUMBER		            10
#define STBDATA_MAX_DATABROADCAST_NUMBER	        40
#define STBDATA_MAX_BAT_NUMBER                      40

#define STBDATA_NVM_MAX_TV_CHANNEL_NUMBER           800
#define STBDATA_NVM_MAX_RADIO_CHANNEL_NUMBER        200

/*定义保存的扩展频道数据信息个数*/
#define STBDATA_NVM_MAX_AUDIO_INFO_NUMBER           300
#define STBDATA_NVM_MAX_SERVICE_INFORMATION_NUMBER  250
#define STBDATA_NVM_MAX_SUBTITLE_INFO_NUMBER  		150
#endif

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*使用到的扩展service 数据结构*/
typedef DvbService_t STBDATAExtendedService_t;

/*使用到的EVENT 数据结构*/
typedef DvbEvent_t STBDATAEvent_t;

typedef struct STBDATADeliveryInfo
{
    unsigned int                uiFrequency;
    unsigned int			    uiSymbolRate;
    unsigned short		        usModulation;
    unsigned short		        usDeliveryType;
}STBDATADeliveryInfo_t;


/*TS信息结构(size 20)*/ 
typedef struct STBDATATsInfo    
{	
	unsigned short              usNetID;		/*org net id*/
	unsigned short		        usTSID;			/*ts id*/
	
	unsigned int                uiFrequency;	/*频率值*/
	unsigned int			    uiSymbolRate;	/*符号率*/

	unsigned char               ucStatus;			
	unsigned char               ucInversion;    /*信号反转标志*/  
	unsigned char	            ucModulation;	/*调制方式*/
	signed char 		        scTsNO;			/*数组下标*/

    unsigned char               ucReserve1;     /*预留1*/
    signed char               	scReserve2;     /*预留2*/
    unsigned short              usReserve3;     /*预留3*/
} STBDATATsInfo_t;


/*Service 信息结构*/ 
typedef struct STBDATAServiceInfo
{
	signed char                 scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];/*service名称*/
	
	unsigned short		        usStatus;		/*Service的状态*/
	signed short			    ssServiceNO;	/*数组下标*/
	unsigned short		        usServiceID;	/*Service ID*/

    unsigned short              usPmtPID;
	unsigned short		        usVideoPID;		/*视频Pid*/
	unsigned short		        usAudio1PID;	/*音频Pid*/
	unsigned short		        usPcrPID;		/*PCR Pid*/

	signed char                 scTsNO;			/*该Service对应的TS序号*/
	unsigned char               ucServiceType;                    
	unsigned char               ucVideoType;	/*视频类型*/
	unsigned char               ucAudio1Type;	/*音频1类型*/
	
	unsigned short		        usVideoEcmPID;	/*Vedio ecm pid*/
	unsigned short		        usAudio1EcmPID;	/*Audio ecm pid*/
    unsigned short              usDdataEcmPID;  //某些数据广播在一个service内分别对音视频及数据加密
	unsigned short		        usEmmPID;		/*EMM Pid*/
	unsigned short              usCaSID;		/*CA标识符 */
	STBDATAExtendedService_t    *pExtendedService;/*用于扩展信息*/

	bool                        IsAuthority;   /*是否授权标识*/
	signed char			        scVolumeOffset; /*音量补偿值*/
	unsigned char			    ucSoundChannel;	/*频道声道设定值*/
	unsigned int                netChannelindex;/*网上接收到的频道序号*/
}STBDATAServiceInfo_t;


/*存储用Service信息结构(size 60)*/
typedef struct STBDATANVMServiceInfo  
{
	signed char 			    scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];/*Service名称*/
	
	unsigned short		        usStatus;		/*Service的状态*/
	signed short			    ssServiceNO;	/*数组下标*/
	unsigned short		        usServiceID;	/*Service ID*/

    unsigned short              usPmtPID;
	unsigned short		        usVideoPID;		/*视频Pid*/
	unsigned short		        usAudio1PID;	/*音频Pid*/
	unsigned short		        usPcrPID;		/*PCR Pid*/

	signed char                 scTsNO;			/*该Service对应的TS序号*/
	unsigned char               ucServiceType;                    
	unsigned char               ucVideoType;	/*视频类型*/
	unsigned char               ucAudio1Type;	/*音频类型*/
	
	unsigned short		        usVideoEcmPID;	/*Vedio Ecm Pid*/
	unsigned short		        usAudio1EcmPID;	/*Audio Ecm Pid */
    unsigned short              usDdataEcmPID;  //某些数据广播在一个service内分别对音视频及数据加密
	unsigned short		        usEmmPID;		/*EMM Pid*/
	unsigned short              usCaSID;		/*CA标识符*/

	bool                        IsAuthority;    /*是否授权标识*/
	unsigned char			    scVolumeOffset;	/*音量补偿值*/
	unsigned char			    ucSoundChannel;	/*频道声道设定值*/
    unsigned int                channelIndex;   //网上接收到的频道序号

    unsigned char               ucReserve1;     /*预留1*/
    signed char               	scReserve2;     /*预留2*/
    unsigned short              usReserve3;     /*预留3*/
}STBDATANVMServiceInfo_t;


/*存储节目描述信息结构(size 268)*/
typedef  struct  STBDATANVMServiceInformation  
{
	unsigned char               ucStatus;
	unsigned short		        usNetID;		/*org net id*/
	unsigned short		        usTSID;			/*ts id*/
	unsigned short		        usServiceID;	/*Service ID*/
	unsigned char               ServiceInfo[SERVICE_INFORMATION_BYTES];
}STBDATANVMServiceInformation_t;


/*频道结构(size 12)*/ 
typedef struct STBDATAChannelInfo  
{
    unsigned char        	    ucStatus;   	/*频道状态 */
    unsigned char        	    ucLocked;  
    unsigned char 		        ucAttribute;	/*频道属性*/
    signed short		        ssChannelNo;   	/*频道号*/
    signed short    		    ssServiceNO;   	/*该频道 对应的service序号*/

    unsigned char               ucReserve1;     /*预留1*/
    signed char               	scReserve2;     /*预留2*/
    unsigned short              usReserve3;     /*预留3*/
} STBDATAChannelInfo_t;


/*NVOD时移频道结构(size 4)*/
typedef struct STBDATANVODOffsetChannelInfo 
{
    unsigned char        	    ucStatus; 		/*频道状态*/
    signed short    		    ssServiceNO;	/*该频道对应的service序号*/
} STBDATANVODOffsetChannelInfo_t;


/*NVOD频道结构(size 46)*/
typedef struct STBDATANVODChannelInfo   
{
	unsigned char        	        ucStatus;   	/*频道状态 */
	signed short		            ssChannelNo;	/*频道号*/
	signed short    		        ssServiceNO;    /*该频道 对应的service序号*/
    STBDATANVODOffsetChannelInfo_t  OffsetChannel[STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER];
} STBDATANVODChannelInfo_t;


/*Mosaic小窗口数据结构(size 26)*/
typedef struct STBDATAMosaicLogicalCellInfo 
{
	unsigned char                   ucLogicalCellID;
	unsigned char                   ucAudioType;				

	unsigned char		            ucPresentationInfo;
	unsigned char 	                ucElemNo;
	unsigned char		            ucElementaryCellID[STBDATA_MAX_MOSAIC_ELEM_CELL_NUMBER];	
	unsigned short	                usAudioPID;
	unsigned short	                usAudioEcmPID;	
	signed short		            ssServiceNo;
}STBDATAMosaicLogicalCellInfo_t;


/*Mosaic数据结构(size 542)*/
typedef struct STBDATAMosaicInfo	
{
    unsigned char		            ucStatus;				
    signed short		            ssServiceNo;			
    signed short		            ssChannelNo;

    STBDATAMosaicLogicalCellInfo_t	LogicalCell[STBDATA_MAX_MOSAIC_LOGICAL_CELL_NUMBER];

    signed short		            ssPrevServiceNo;		
    signed short		            ssNextServiceNo;

    bool				            bEntryPoint;
    unsigned char		            ucHorizontalCellNo;	
    unsigned char		            ucVerticalCellNo;	

    unsigned short	                usScreenLeft;
    unsigned short	                usScreenTop;
    unsigned short	                usScreenRight;
    unsigned short	                usScreenBottom;		
}STBDATAMosaicInfo_t;


/*Mosaic小窗口临时数据结构*/
typedef struct STBDATATempMosaicCellInfo
{
	unsigned char					ucLogicalCellID;
	unsigned short					usAudioPID;
	unsigned char        			ucAudioType;		
	unsigned short					usAudioEcmPID;			

	unsigned short					ucLogicalCellStatus;
	
	unsigned char					ucPresentationInfo;
	unsigned char 					ucElemNo;
	unsigned char					ucElementaryCellID[STBDATA_MAX_MOSAIC_ELEM_CELL_NUMBER];		

	signed short					ssServiceNo;
	unsigned char					ucLinkageInfo;
	unsigned short					usOriginalNetworkID;
	unsigned short					usTsID;				
	unsigned short					usServiceID; 
	unsigned short 					usBouquetID;
	unsigned short 					usEventID;
}STBDATATempMosaicCellInfo_t;


/*Mosaic临时数据结构*/
typedef struct STBDATATempMosaicInfo
{
	unsigned char					ucStatus;				
	signed short					ssServiceNo;			
	signed short					ssChannelNo;
	
	STBDATATempMosaicCellInfo_t		LogicalCell[STBDATA_MAX_MOSAIC_LOGICAL_CELL_NUMBER];

	unsigned short					usPrevNetID;
	unsigned short					usPrevTsID;				
	unsigned short					usPrevServiceID; 
	unsigned short					usNextNetID;
	unsigned short					usNextTsID;				
	unsigned short					usNextServiceID; 

	signed short					ssPrevServiceNo;		
	signed short					ssNextServiceNo;

	bool							bEntryPoint;
	unsigned char					ucHorizontalCellNo;	
	unsigned char					ucVerticalCellNo;	
	
	unsigned short					usScreenLeft;
	unsigned short					usScreenTop;
	unsigned short					usScreenRight;
	unsigned short					usScreenBottom;		
}STBDATATempMosaicInfo_t;


/*数据广播频道结构(size 6)*/
typedef struct STBDATABroadcastInfo 
{
	unsigned char 	                ucStatus;
	signed short		            ssChannelNo;
	signed short	                ssServiceINo;
}STBDATABroadcastInfo_t;


/*BAT分类信息结构(size 432)*/
typedef struct STBDATABATInfo  
{
	signed char			            szBouquetName[DVB_INFO_MAX_BOUQUET_NAME_LENGTH];
	unsigned char 		            ucStatus;
	unsigned short		            ssBATNO;	
	unsigned short 		            usBATid;
	unsigned char			        ucBATServiceNum;		
	unsigned short 		            ServiceNoData[STBDATA_MAX_BAT_SERVICE_NUMBER];	//单个BAT可关联100个频道

    unsigned char                   ucReserve1;     /*预留1*/
    signed char                   	scReserve2;     /*预留2*/
    unsigned short                  usReserve3;		/*预留3*/
}STBDATABATInfo_t;


/*节目分类节点结构*/
typedef struct TVProgClassNode
{
	void 				            *pClassInfoData;
	void 				            *pPageFirstNode;
	unsigned int			        uiFocusRow;		
}TVProgClassNode_t;


/*搜索前临时存储用户设置的频道属性 */                 
typedef struct STBDATAStorageChannelInfoTemp
{
	unsigned char			        ucStatus;					
    unsigned short		            usNetworkID;	/*Network ID*/
    unsigned short		            usServiceID;	/*Service ID*/
    unsigned short		            usTsID;			/*Ts ID*/

    unsigned char        	        ucLocked;  
    unsigned char 		            ucAttribute;	/*频道属性*/

    unsigned char			        scVolumeOffset;	/*音量补偿值*/
	unsigned char			        ucSoundChannel;	/*频道声道设定值*/
} STBDATAStorageChannelInfoTemp_t;


/*伴音存储数据结构(size 8)*/
typedef struct STBAudioInfo     
{
	unsigned char                   ucAudioType;
	char			        		scLanguageCode[3];
    unsigned short		            usAudioPID;
    unsigned short		            usAudioEcmPID;	 	
}STBAudioInfo_t;


/*多伴音存储数据结构(size 84)*/
typedef struct STBDATAServiceAudioInfo     
{  
    unsigned char                   ucStatus;
	unsigned char                   ucAudioInfoNum;
	signed short       				ssAudioNO;		/*对应ServiceNO*/
    STBAudioInfo_t                  AudioInfo[STBDATA_MAX_AUDIO_NUMBER];
}STBDATAServiceAudioInfo_t;


/*Subtitle存储数据结构(size 12)*/
typedef struct STBSubtitleInfo    
{
    unsigned char                   ucSubtitleType;
    char			        		scLanguageCode[3];	
    unsigned short		            usSubtitlePID;
    unsigned short		            usSubtitleEcmPID;	
	unsigned short					usCompositionPageId;
	unsigned short 					usAncillaryPageId;
}STBSubtitleInfo_t;


/*多Subtitle存储数据结构(size 126)*/
typedef struct STBDATAServiceSubtitleInfo    
{  
    unsigned char                   ucStatus;
    unsigned char                   ucSubtitleNum;
	unsigned char					ucCurrentSubtitleNo;	/*当前使用的Subtitle*/
	signed short       				ssSubtitleNO;			/*对应ServiceNO*/
    STBSubtitleInfo_t               SubtitleInfo[STBDATA_MAX_AUDIO_NUMBER];
}STBDATAServiceSubtitleInfo_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/

#endif

