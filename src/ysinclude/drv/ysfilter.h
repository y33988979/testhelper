/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysfilter.h

    Description : 

    Others:      

    History:      1. llh creat 2004-3-20


*******************************************************************************/
#ifndef _YSFILTER_H_
#define _YSFILTER_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include "drv_os.h"
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/

#define  ONE_KILO_SECTION_MAX_LENGTH       1152
#define  SECT_BUFFER_DEPTH                 4/*8 */
#define  DATA_BUFFER_SIZE                  ONE_KILO_SECTION_MAX_LENGTH
#define  SECTION_CIRCULAR_BUFFER_SIZE    4096 //  ( ONE_KILO_SECTION_MAX_LENGTH * SECT_BUFFER_DEPTH )
#define  FILTER_DEPTH_SIZE                 16
#define  SECTION_VERSION_MASK              0x3E000000;   /* 5 bits mask */
#define	 SECTION_PROG_NO_SERVICE_ID_MASK   0x00FFFF00;	 /* 16bits mask */

#define  NO_OF_SIMULTANEOUS_FILTERS       96


/* defintions related to both hardware and software filter slots*/
#define SMALL_SECTION_SIZE                 1024
#define LARGE_SECTION_SIZE                 4096

#define NO_OF_1K_FILTER_SLOTS              28
#define NO_OF_4K_FILTER_SLOTS              ( NO_OF_SIMULTANEOUS_FILTERS - NO_OF_1K_FILTER_SLOTS )


/* slot positions for the hardware and software filter data tables*/
#define SLOT_START_OFFSET                 0
#define SLOT_END_OFFSET                   ( NO_OF_SIMULTANEOUS_FILTERS - 1 )
#define SLOT_OF_1K_START_OFFSET           SLOT_START_OFFSET
#define SLOT_OF_1K_END_OFFSET             ( NO_OF_1K_FILTER_SLOTS - 1 )
#define SLOT_OF_4K_START_OFFSET           ( SLOT_OF_1K_END_OFFSET + 1 )
#define SLOT_OF_4K_END_OFFSET             SLOT_END_OFFSET
#define PACKET_LENGTH                     188


#define	SECTION_ERROR_UNABLE_TO_GRANT				   	-1
#define	SECTION_ERROR_TOO_MANY_FILTER_REQ				-2
#define	SECTION_ERROR_INVALID_SLOT_ID				   	-3
#define	SECTION_ERROR_CANT_REENABLE_FREE_FILTER	       	-4
#define	SECTION_ERROR_TWO_FILTER_REQ_FOR_SAME_PID		-5
#define SECTION_ERROR_ST_ERROR      					-6
#define SECTION_ERROR_OTHERS          					-7
/*
 * maximum bytes per section. note as per standard it will be 1024 for
 * ISO13818-1 defined tables and for private section it shall be 4096
 */
#define	SECTION_DATA_LENGTH_MAX		MAX_SECTION_LENGTH








#define SECTION_WAIT_TIMEOUT_STEP			 15 //ms
#define SECTION_WAIT_TIMEOUT_NONE			SECTION_WAIT_TIMEOUT_STEP
#define SECTION_WAIT_TIMEOUT_INFINITY		( g_DRV_OS_TickPerSecond * 256 )
#define SECTION_WAIT_TIMEOUT_DEAFULT		( g_DRV_OS_TickPerSecond / 4 )

/*define StandardTable Timeout*/
#define SECTION_WAIT_TIMEOUT_STANDARD_TABLE        3000


#define SECTION_WAIT_TIMEOUT_NIT_ACTUAL_MS		12000
#define SECTION_WAIT_TIMEOUT_NIT_OTHER_MS		12000

#define SECTION_WAIT_TIMEOUT_PAT_MS			2500
#define SECTION_WAIT_TIMEOUT_PMT_MS			4000//2000
#define SECTION_WAIT_TIMEOUT_CAT_MS			1000 

#define SECTION_WAIT_TIMEOUT_SDT_ACTUAL_MS		10000	
#define SECTION_WAIT_TIMEOUT_SDT_OTHER_MS		22000

#define SECTION_WAIT_TIMEOUT_EIT_SCHED_ACTUAL_MS		62000 
#define SECTION_WAIT_TIMEOUT_EIT_SCHED_OTHER_MS		62000

#define SECTION_WAIT_TIMEOUT_EIT_PF_ACUTAL_MS			12000
#define SECTION_WAIT_TIMEOUT_EIT_PF_OHTER_MS			12000

#define SECTION_WAIT_TIMEOUT_TDT_MS			32000
#define SECTION_WAIT_TIMEOUT_BAT_MS	              12000
#define SECTION_WAIT_TIMEOUT_PRIVATE_MS		10000



#define	SECTION_ERROR_UNABLE_TO_GRANT						-1
#define	SECTION_ERROR_TOO_MANY_FILTER_REQ					-2
#define	SECTION_ERROR_INVALID_SLOT_ID						-3
#define	SECTION_ERROR_CANT_REENABLE_FREE_FILTER				-4
#define	SECTION_ERROR_TWO_FILTER_REQ_FOR_SAME_PID			-5




#if 1

 /* defined as int for pti/link, but U16 for spti */
//typedef unsigned int pid_t;

typedef	enum
{
   START_DATA_BASE_BUILDING,
	STOP_DATA_BASE_BUILDING,
	DELETE_DATA_BASE_BASED_ON_TRANSPONDER_SLOT
} usif_db_cmd_state_t;
typedef	enum
{
   	SECTION_SLOT_FREE,					/* no section filter req for this slot */
	SECTION_SLOT_IN_USE,					/* filtering is	in progress */
	SECTION_SLOT_FILTER_DONE,			/* filtering is	completed and it is ready to restart or	get killed */
	SECTION_SLOT_CRC_ERROR_DETECTED,	/* crc error detetcted while filtering */
	SECTION_SLOT_TIMEOUT,				/*Timeout occur for the slot*/
	SECTION_SLOT_STOP	                      /*USED FOR CA*/
} sf_slot_status_t;

/* --- */
typedef	enum
{
   ONE_KILO_SECTION = 1,	/* 1KB long section */
   FOUR_KILO_SECTION = 4,	/* 4KB long section */
   EIGHT_KILO_SECTION = 8,   /*8Kb long section*/
   SIXTEEN_KILO_SECTION = 16   /*16Kb long section*/
} sf_filter_mode_t;


typedef void 	(*FiltCallBack)(int FilterNo, BOOL_YS isTimeOut, unsigned char *paucSectionData, unsigned short length);

typedef	struct tagSectionFilterStruct
{
      
   unsigned char	FilterData [ FILTER_DEPTH_SIZE ];   
   unsigned char	FilterMask [ FILTER_DEPTH_SIZE ];
   /*unsigned char	*ProducerPtr;      
   unsigned char	*ConsumerPtr;      */
   unsigned char	aucSectionData [1152/*ONE_KILO_SECTION_MAX_LENGTH * 4*/];
   unsigned char	*SectionData;
   unsigned char	ucTableId;
   sf_slot_status_t	status;
   sf_filter_mode_t	Mode;      
   STPTI_FilterType_t	FilterType;
   STPTI_FilterData_t 	FilterValues;
   STPTI_Pid_t		pidDataPid;			
   STPTI_Slot_t		SlotHandle;		
   STPTI_Filter_t	FilterHandle;
   STPTI_Buffer_t	BufferHandle;
   STPTI_Signal_t	SignalHandle;
   S8_YS VersionNo;
   S16_YS sSectionNo;
   S16_YS		sSectionLength;
   S32_YS			StreamId;
   DRVOS_MsgQueue_t	*pstQueueId;   
   /*unsigned char	*SectionBuffer;*/
   
   int						nTimeOutCount;	/*The number of timeout counts for the access*/
   int						nOrgTimeOutCount;
   int                     			TsId;
   int                    			OriginalNetworkId;
   FiltCallBack			Callback; 
   int 				MaxSectionLen;   

	char						IsAVITslot;
	char 					ChanID;

   
}ST_FILTER_STRUCT;
#endif

typedef enum {
    PTI_I_filter = 0xa5
} filter_type_t;

typedef struct {
    filter_type_t	 tag;
    unsigned char 	*filter_bytes;
    unsigned char  	*filter_masks;
    BOOL_YS		 discard_on_crc_error;
    BOOL_YS		 initial_state_enabled;
} section_filter_t;

typedef enum STPTI_KeyParity_s
{
    STPTI_KEY_PARITY_EVEN_PARITY,
    STPTI_KEY_PARITY_ODD_PARITY,
    STPTI_KEY_PARITY_GENERIC_KEY
} STPTI_KeyParity_t;


typedef enum STPTI_KeyUsage_s
{
    STPTI_KEY_USAGE_INVALID,
    STPTI_KEY_USAGE_VALID_FOR_PES,
    STPTI_KEY_USAGE_VALID_FOR_TRANSPORT,
    STPTI_KEY_USAGE_VALID_FOR_ALL
} STPTI_KeyUsage_t;

typedef union STPTI_SlotOrPid_s
{
    STPTI_Slot_t Slot;
    STPTI_Pid_t Pid;
} STPTI_SlotOrPid_t;

typedef enum DescrambleStatus
{
	DESCRAMBLE_FREE,
	DESCRAMBLE_IN_USE
}DescrambleStatus_e;

typedef enum DescrambleType
{
	INVALID_DESCRAMBLE_TYPE,
	PLAY_VIDEO_DESCRAMBLE_TYPE,
	PLAY_AUDIO_DESCRAMBLE_TYPE,
	REC_VIDEO_DESCRAMBLE_TYPE,
	REC_AUDIO_DESCRAMBLE_TYPE,
	DESCRAMBLE_TYPE_COUNT
}DescrambleType_e;
                     
extern ST_FILTER_STRUCT FilterDescript[];
/*-------------------------------------------------------------------------
 * Function     : DRV_SectionFilterInit
 * Description  : 这里会创建一个任务，来处理demux报上来的回调及
 调用我们自己的回调上报数据。这个应该是我们自己来实现。
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
DRV_ErrCode DRV_SectionFilterInit(void);
/*-------------------------------------------------------------------------
 * Function     : DRV_FILTReqFilterForStandard
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int	DRV_FILTReqFilterForStandard (DRVOS_MsgQueue_t*	ReturnQueueId,
						FiltCallBack		pCallbackFunc,
						sf_filter_mode_t	FilterMode,
						STPTI_Pid_t		ReqPid,
						int					ReqTableId,
						int					ReqProgNo,
						int					ReqSectionNo,
						int					ReqVersionNo,
						int					nTimeOutCount,
						int                 iTsId,
						int                 iOriginalNetworkId);
/*-------------------------------------------------------------------------
 * Function     : DRV_FILTReqFilterFree_CRCFlag
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int	DRV_FILTReqFilterFree_CRCFlag(			FiltCallBack		pCallbackFunc,
							sf_filter_mode_t	FilterMode,
							unsigned char				*paucFilterData,
							unsigned char				*paucFilterMask,
							STPTI_Pid_t /*pid_t*/				ReqPid,
							int					nTimeOutCount,
							BOOL_YS CRCFlag
							);
/*-------------------------------------------------------------------------
 * Function     : DRV_FILTReqFilterFree
 * Description  : 申请Filter并且开始获取数据
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int	DRV_FILTReqFilterFree  (			FiltCallBack		pCallbackFunc,
							sf_filter_mode_t	FilterMode,
							unsigned char				*paucFilterData,
							unsigned char				*paucFilterMask,
							STPTI_Pid_t /*pid_t*/				ReqPid,
							int					nTimeOutCount
							);
/*-------------------------------------------------------------------------
 * Function     : DRV_FILTFreeFilterReq
 * Description  : 释放所申请的Filter
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int	 DRV_FILTFreeFilterReq (  int SlotIndex);
S32	DRV_FILTReenableFilter ();


/*-------------------------------------------------------------------------
 * Function     : DRV_FILTReqFilterForNextSection
 * Description  : 改变搜索条件后重新开始获取数据
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
S32 DRV_FILTReqFilterForNextSection(S32 FilterNo, S32 iNextSectionNo, S32 nNextTableID);
#if 0
/*-------------------------------------------------------------------------
 * Function     : DRV_FLT_SetFilter
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_FLT_SetFilter(U32_YS Channel, 
								U32_YS Buffer,
								S32_YS FilterNo,
								U8_YS *Data,
								U8_YS *Mask);

/*-------------------------------------------------------------------------
 * Function     : DRV_FLT_FreeFilter
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_FLT_FreeFilter(U32_YS Channel, U32_YS Buffer, S32_YS FilterNo);

/*-------------------------------------------------------------------------
 * Function     : DRV_FLT_AllocateFilter
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_FLT_AllocateFilter(U32_YS Channel,
								U32_YS Buffer, 
								OUT_YS S32_YS *FilterNo);

/*-------------------------------------------------------------------------
 * Function     : DRV_DMX_ControlChannel
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_DMX_ControlChannel(U32_YS Channel, 
									U32_YS Buffer,
									U32_YS Action);

/*-------------------------------------------------------------------------
 * Function     : DRV_DMX_SetChannelPid
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_DMX_SetChannelPid(U32_YS Channel, U32_YS Buffer, U16_YS Pid);

/*-------------------------------------------------------------------------
 * Function     : DRV_DMX_FreeSectionChannel
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_DMX_FreeSectionChannel(U32_YS Channel, U32_YS Buffer);

/*-------------------------------------------------------------------------
 * Function     : DRV_DMX_AllocateSectionChannel
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
BOOL_YS DRV_DMX_AllocateSectionChannel(U32_YS BufferSize, OUT_YS U32_YS *Channel, OUT_YS U32_YS *Buffer);

/*-------------------------------------------------------------------------
 * Function     : pti_allocate_dynamic_slot
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int  pti_allocate_dynamic_slot ( int *slotnum , int *buff,  int  nBufferSize);


/*-------------------------------------------------------------------------
 * Function     : pti_deallocate_dynamic_slot
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int pti_deallocate_dynamic_slot(STPTI_Slot_t SlotHandle, STPTI_Buffer_t BufferHandle);

/*-------------------------------------------------------------------------
 * Function     : pti_allocate_dynamic_filter
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
int pti_allocate_dynamic_filter(int *filter);

/*-------------------------------------------------------------------------
 * Function     : pti_deallocate_dynamic_filter
 * Description  : 
 * Input        : 
 * Output       :
 * Return       : 
 * ----------------------------------------------------------------------*/
void pti_deallocate_dynamic_filter(int filter);
#endif


/*dmx model*/
DRV_ErrCode DRV_PtiInit(void);
DRV_ErrCode drv_PtiProtect(void);
DRV_ErrCode drv_PtiUnProtect(void);
DRV_ErrCode DRV_PtiGetPacketCount(U16 *PacketCount);
DRV_ErrCode drv_PtiAudioStart( PTI_HandleId_t HandleId, STPTI_Pid_t AudioPid); 
DRV_ErrCode drv_PtiAudioStop(PTI_HandleId_t HandleId);
DRV_ErrCode drv_PtiVideoStart(PTI_HandleId_t HandleId, STPTI_Pid_t VideoPid);
DRV_ErrCode drv_PtiVideoStop(PTI_HandleId_t HandleId);
DRV_ErrCode drv_PtiPcrStart( PTI_HandleId_t HandleId ,STPTI_Pid_t PCRPid );
DRV_ErrCode drv_PtiPcrStop( PTI_HandleId_t HandleId); 
DRV_ErrCode pti_allocate_dynamic_key(U32 DmxID, U32 *key);
DRV_ErrCode pti_deallocate_dynamic_key(U32 key);
DRV_ErrCode pti_associate_descramble_key(U32 DmxID,U32 slot_or_pid,U32 key);
DRV_ErrCode pti_disassociate_descramble_key(U32 DmxID,U32 slot_or_pid,U32 key);
DRV_ErrCode pti_set_descramble_key(U32 key,STPTI_KeyParity_t parity,STPTI_KeyUsage_t validity,U8 *key_data);


/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

