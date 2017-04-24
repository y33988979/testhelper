/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdata.h

    Description : Ƶ�����ݹ���

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
 
/*�������˽�����ݿ�����������*/
#define START_REQUEST_STATUS_AUT 					0x0001
#define START_REQUEST_STATUS_PDT 					0x0002
#define START_REQUEST_STATUS_XDT 					0x0004
#define START_REQUEST_STATUS_ADT 					0x0008
#define START_REQUEST_STATUS_PIC 					0x0010
#define START_REQUEST_STATUS_PRG 					0x0020
#define START_REQUEST_STATUS_NIT 					0x0040
#define START_REQUEST_STATUS_EIT 					0x0080
#define START_REQUEST_ALL_STATUS  					0xffff

/*���嵥��Service��������*/
#define STBDATA_MAX_AUDIO_NUMBER                    10 

/*����Ƶ������ַ�����*/
#define SERVICE_INFORMATION_BYTES       			260

/*����Ƶ�����ݱ������*/
#define STBDATA_MAX_TV_CHANNEL_NUMBER		        1000
#define STBDATA_MAX_RADIO_CHANNEL_NUMBER		    1000
#define STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER		10
#define STBDATA_MAX_MOSAIC_LOGICAL_CELL_NUMBER		20
#define STBDATA_MAX_MOSAIC_ELEM_CELL_NUMBER	        16
#define STBDATA_MAX_BAT_SERVICE_NUMBER              200

#ifdef USE_DATA_BLOCK_64K_MODE
/*���屣���Ƶ��������Ϣ����*/
#define STBDATA_MAX_TS_NUMBER				        100
#define STBDATA_MAX_SERVICE_NUMBER			        700
#define STBDATA_MAX_NVOD_CHANNEL_NUMBER		        20
#define STBDATA_MAX_MOSAIC_NUMBER		            5
#define STBDATA_MAX_DATABROADCAST_NUMBER	        30
#define STBDATA_MAX_BAT_NUMBER                      30

#define STBDATA_NVM_MAX_TV_CHANNEL_NUMBER           500
#define STBDATA_NVM_MAX_RADIO_CHANNEL_NUMBER        200

/*���屣�����չƵ��������Ϣ����*/
#define STBDATA_NVM_MAX_AUDIO_INFO_NUMBER           200
#define STBDATA_NVM_MAX_SERVICE_INFORMATION_NUMBER  150
#define STBDATA_NVM_MAX_SUBTITLE_INFO_NUMBER  		90
#else
/*���屣���Ƶ��������Ϣ����*/
#define STBDATA_MAX_TS_NUMBER				        150
#define STBDATA_MAX_SERVICE_NUMBER			        1000
#define STBDATA_MAX_NVOD_CHANNEL_NUMBER		        40
#define STBDATA_MAX_MOSAIC_NUMBER		            10
#define STBDATA_MAX_DATABROADCAST_NUMBER	        40
#define STBDATA_MAX_BAT_NUMBER                      40

#define STBDATA_NVM_MAX_TV_CHANNEL_NUMBER           800
#define STBDATA_NVM_MAX_RADIO_CHANNEL_NUMBER        200

/*���屣�����չƵ��������Ϣ����*/
#define STBDATA_NVM_MAX_AUDIO_INFO_NUMBER           300
#define STBDATA_NVM_MAX_SERVICE_INFORMATION_NUMBER  250
#define STBDATA_NVM_MAX_SUBTITLE_INFO_NUMBER  		150
#endif

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*ʹ�õ�����չservice ���ݽṹ*/
typedef DvbService_t STBDATAExtendedService_t;

/*ʹ�õ���EVENT ���ݽṹ*/
typedef DvbEvent_t STBDATAEvent_t;

typedef struct STBDATADeliveryInfo
{
    unsigned int                uiFrequency;
    unsigned int			    uiSymbolRate;
    unsigned short		        usModulation;
    unsigned short		        usDeliveryType;
}STBDATADeliveryInfo_t;


/*TS��Ϣ�ṹ(size 20)*/ 
typedef struct STBDATATsInfo    
{	
	unsigned short              usNetID;		/*org net id*/
	unsigned short		        usTSID;			/*ts id*/
	
	unsigned int                uiFrequency;	/*Ƶ��ֵ*/
	unsigned int			    uiSymbolRate;	/*������*/

	unsigned char               ucStatus;			
	unsigned char               ucInversion;    /*�źŷ�ת��־*/  
	unsigned char	            ucModulation;	/*���Ʒ�ʽ*/
	signed char 		        scTsNO;			/*�����±�*/

    unsigned char               ucReserve1;     /*Ԥ��1*/
    signed char               	scReserve2;     /*Ԥ��2*/
    unsigned short              usReserve3;     /*Ԥ��3*/
} STBDATATsInfo_t;


/*Service ��Ϣ�ṹ*/ 
typedef struct STBDATAServiceInfo
{
	signed char                 scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];/*service����*/
	
	unsigned short		        usStatus;		/*Service��״̬*/
	signed short			    ssServiceNO;	/*�����±�*/
	unsigned short		        usServiceID;	/*Service ID*/

    unsigned short              usPmtPID;
	unsigned short		        usVideoPID;		/*��ƵPid*/
	unsigned short		        usAudio1PID;	/*��ƵPid*/
	unsigned short		        usPcrPID;		/*PCR Pid*/

	signed char                 scTsNO;			/*��Service��Ӧ��TS���*/
	unsigned char               ucServiceType;                    
	unsigned char               ucVideoType;	/*��Ƶ����*/
	unsigned char               ucAudio1Type;	/*��Ƶ1����*/
	
	unsigned short		        usVideoEcmPID;	/*Vedio ecm pid*/
	unsigned short		        usAudio1EcmPID;	/*Audio ecm pid*/
    unsigned short              usDdataEcmPID;  //ĳЩ���ݹ㲥��һ��service�ڷֱ������Ƶ�����ݼ���
	unsigned short		        usEmmPID;		/*EMM Pid*/
	unsigned short              usCaSID;		/*CA��ʶ�� */
	STBDATAExtendedService_t    *pExtendedService;/*������չ��Ϣ*/

	bool                        IsAuthority;   /*�Ƿ���Ȩ��ʶ*/
	signed char			        scVolumeOffset; /*��������ֵ*/
	unsigned char			    ucSoundChannel;	/*Ƶ�������趨ֵ*/
	unsigned int                netChannelindex;/*���Ͻ��յ���Ƶ�����*/
}STBDATAServiceInfo_t;


/*�洢��Service��Ϣ�ṹ(size 60)*/
typedef struct STBDATANVMServiceInfo  
{
	signed char 			    scServiceName[DVB_INFO_MAX_SERVICE_NAME_LENGTH];/*Service����*/
	
	unsigned short		        usStatus;		/*Service��״̬*/
	signed short			    ssServiceNO;	/*�����±�*/
	unsigned short		        usServiceID;	/*Service ID*/

    unsigned short              usPmtPID;
	unsigned short		        usVideoPID;		/*��ƵPid*/
	unsigned short		        usAudio1PID;	/*��ƵPid*/
	unsigned short		        usPcrPID;		/*PCR Pid*/

	signed char                 scTsNO;			/*��Service��Ӧ��TS���*/
	unsigned char               ucServiceType;                    
	unsigned char               ucVideoType;	/*��Ƶ����*/
	unsigned char               ucAudio1Type;	/*��Ƶ����*/
	
	unsigned short		        usVideoEcmPID;	/*Vedio Ecm Pid*/
	unsigned short		        usAudio1EcmPID;	/*Audio Ecm Pid */
    unsigned short              usDdataEcmPID;  //ĳЩ���ݹ㲥��һ��service�ڷֱ������Ƶ�����ݼ���
	unsigned short		        usEmmPID;		/*EMM Pid*/
	unsigned short              usCaSID;		/*CA��ʶ��*/

	bool                        IsAuthority;    /*�Ƿ���Ȩ��ʶ*/
	unsigned char			    scVolumeOffset;	/*��������ֵ*/
	unsigned char			    ucSoundChannel;	/*Ƶ�������趨ֵ*/
    unsigned int                channelIndex;   //���Ͻ��յ���Ƶ�����

    unsigned char               ucReserve1;     /*Ԥ��1*/
    signed char               	scReserve2;     /*Ԥ��2*/
    unsigned short              usReserve3;     /*Ԥ��3*/
}STBDATANVMServiceInfo_t;


/*�洢��Ŀ������Ϣ�ṹ(size 268)*/
typedef  struct  STBDATANVMServiceInformation  
{
	unsigned char               ucStatus;
	unsigned short		        usNetID;		/*org net id*/
	unsigned short		        usTSID;			/*ts id*/
	unsigned short		        usServiceID;	/*Service ID*/
	unsigned char               ServiceInfo[SERVICE_INFORMATION_BYTES];
}STBDATANVMServiceInformation_t;


/*Ƶ���ṹ(size 12)*/ 
typedef struct STBDATAChannelInfo  
{
    unsigned char        	    ucStatus;   	/*Ƶ��״̬ */
    unsigned char        	    ucLocked;  
    unsigned char 		        ucAttribute;	/*Ƶ������*/
    signed short		        ssChannelNo;   	/*Ƶ����*/
    signed short    		    ssServiceNO;   	/*��Ƶ�� ��Ӧ��service���*/

    unsigned char               ucReserve1;     /*Ԥ��1*/
    signed char               	scReserve2;     /*Ԥ��2*/
    unsigned short              usReserve3;     /*Ԥ��3*/
} STBDATAChannelInfo_t;


/*NVODʱ��Ƶ���ṹ(size 4)*/
typedef struct STBDATANVODOffsetChannelInfo 
{
    unsigned char        	    ucStatus; 		/*Ƶ��״̬*/
    signed short    		    ssServiceNO;	/*��Ƶ����Ӧ��service���*/
} STBDATANVODOffsetChannelInfo_t;


/*NVODƵ���ṹ(size 46)*/
typedef struct STBDATANVODChannelInfo   
{
	unsigned char        	        ucStatus;   	/*Ƶ��״̬ */
	signed short		            ssChannelNo;	/*Ƶ����*/
	signed short    		        ssServiceNO;    /*��Ƶ�� ��Ӧ��service���*/
    STBDATANVODOffsetChannelInfo_t  OffsetChannel[STBDATA_MAX_NVOD_OFFSET_SERVICE_NUMBER];
} STBDATANVODChannelInfo_t;


/*MosaicС�������ݽṹ(size 26)*/
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


/*Mosaic���ݽṹ(size 542)*/
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


/*MosaicС������ʱ���ݽṹ*/
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


/*Mosaic��ʱ���ݽṹ*/
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


/*���ݹ㲥Ƶ���ṹ(size 6)*/
typedef struct STBDATABroadcastInfo 
{
	unsigned char 	                ucStatus;
	signed short		            ssChannelNo;
	signed short	                ssServiceINo;
}STBDATABroadcastInfo_t;


/*BAT������Ϣ�ṹ(size 432)*/
typedef struct STBDATABATInfo  
{
	signed char			            szBouquetName[DVB_INFO_MAX_BOUQUET_NAME_LENGTH];
	unsigned char 		            ucStatus;
	unsigned short		            ssBATNO;	
	unsigned short 		            usBATid;
	unsigned char			        ucBATServiceNum;		
	unsigned short 		            ServiceNoData[STBDATA_MAX_BAT_SERVICE_NUMBER];	//����BAT�ɹ���100��Ƶ��

    unsigned char                   ucReserve1;     /*Ԥ��1*/
    signed char                   	scReserve2;     /*Ԥ��2*/
    unsigned short                  usReserve3;		/*Ԥ��3*/
}STBDATABATInfo_t;


/*��Ŀ����ڵ�ṹ*/
typedef struct TVProgClassNode
{
	void 				            *pClassInfoData;
	void 				            *pPageFirstNode;
	unsigned int			        uiFocusRow;		
}TVProgClassNode_t;


/*����ǰ��ʱ�洢�û����õ�Ƶ������ */                 
typedef struct STBDATAStorageChannelInfoTemp
{
	unsigned char			        ucStatus;					
    unsigned short		            usNetworkID;	/*Network ID*/
    unsigned short		            usServiceID;	/*Service ID*/
    unsigned short		            usTsID;			/*Ts ID*/

    unsigned char        	        ucLocked;  
    unsigned char 		            ucAttribute;	/*Ƶ������*/

    unsigned char			        scVolumeOffset;	/*��������ֵ*/
	unsigned char			        ucSoundChannel;	/*Ƶ�������趨ֵ*/
} STBDATAStorageChannelInfoTemp_t;


/*�����洢���ݽṹ(size 8)*/
typedef struct STBAudioInfo     
{
	unsigned char                   ucAudioType;
	char			        		scLanguageCode[3];
    unsigned short		            usAudioPID;
    unsigned short		            usAudioEcmPID;	 	
}STBAudioInfo_t;


/*������洢���ݽṹ(size 84)*/
typedef struct STBDATAServiceAudioInfo     
{  
    unsigned char                   ucStatus;
	unsigned char                   ucAudioInfoNum;
	signed short       				ssAudioNO;		/*��ӦServiceNO*/
    STBAudioInfo_t                  AudioInfo[STBDATA_MAX_AUDIO_NUMBER];
}STBDATAServiceAudioInfo_t;


/*Subtitle�洢���ݽṹ(size 12)*/
typedef struct STBSubtitleInfo    
{
    unsigned char                   ucSubtitleType;
    char			        		scLanguageCode[3];	
    unsigned short		            usSubtitlePID;
    unsigned short		            usSubtitleEcmPID;	
	unsigned short					usCompositionPageId;
	unsigned short 					usAncillaryPageId;
}STBSubtitleInfo_t;


/*��Subtitle�洢���ݽṹ(size 126)*/
typedef struct STBDATAServiceSubtitleInfo    
{  
    unsigned char                   ucStatus;
    unsigned char                   ucSubtitleNum;
	unsigned char					ucCurrentSubtitleNo;	/*��ǰʹ�õ�Subtitle*/
	signed short       				ssSubtitleNO;			/*��ӦServiceNO*/
    STBSubtitleInfo_t               SubtitleInfo[STBDATA_MAX_AUDIO_NUMBER];
}STBDATAServiceSubtitleInfo_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/

#endif

