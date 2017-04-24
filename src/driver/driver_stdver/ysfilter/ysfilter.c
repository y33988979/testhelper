/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysfilter.c

    Description : 

    Others:      

    History:      1. llh creat 2004-3-20


*******************************************************************************/


#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include <hi_unf_demux.h>
#include "drv_basic.h"
#include "ysfilter.h"
#include "ysostime.h"
#include "drv_os.h"
#include "stbtype.h"
#include "ystuner.h"

//#define SECTION_DEBUG
/*---------------------------definition-----------------------------------*/
#define INDEFINITE_WAIT         0
#define CURRENT_MODULE DRV_MODULE_FILTER

#define STACK_SIZE_SAFE	   1024
#define SFILTER_MONITOR_PRIORITY  (255 - 16)
const S32   SFILTER_MONITOR_WORKSPACE		    =  128*1024;   // (	2048 + STACK_SIZE_SAFE );

/*---------------------External Functions----------------------------------------*/
static HI_U8 g_match[FILTER_DEPTH_SIZE];
static HI_U8 g_mask[FILTER_DEPTH_SIZE];
static HI_U8 g_Negate[FILTER_DEPTH_SIZE];

/*-------------------------Local Functions----------------------------------------*/
static S32 IsSlotShared(S32 SlotIndex);
static void SectionFilterMonitor(void *pvParam);
BOOL SendMessage(S32 SectionSlot, sf_slot_status_t RetStatus);

/*------------------------Global Variable-----------------------------------------*/

DRVOS_Task_t *SectionTaskId;
ST_FILTER_STRUCT FilterDescript[NO_OF_SIMULTANEOUS_FILTERS];

unsigned char 	   *g_Filter_puiSectData;
#define  MAX_SECTION_DATA_BURRER_SIZE 0x100000
static unsigned char *m_pSectionDataBuffer;
DRVOS_Partition_t* 			g_pPtnSectionDataBuffer;


/*-------------------------Local Variable-----------------------------------------*/
DRVOS_Sem_t *SectionTableAccess = NULL;
static DRVOS_Sem_t *CancelWait = NULL;
S32 NoOfFreeSlots = NO_OF_SIMULTANEOUS_FILTERS;


/*-------------------------External Variables-----------------------------------------*/


/***************************************************************************/
DRV_ErrCode SRSTF_SectionFilterInit(void)
{
    S32 index;
        
    DRV_ErrCode ErrCode;
    m_pSectionDataBuffer = (U8 *)DRV_OSMalloc(SystemPartition,MAX_SECTION_DATA_BURRER_SIZE);
    g_pPtnSectionDataBuffer = DRV_OSPartCreate(NULL,(void *)m_pSectionDataBuffer,MAX_SECTION_DATA_BURRER_SIZE);

    /* initialise all access & wait semaphores */
    SectionTableAccess    = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
    CancelWait           = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);

    /* initialise the section filter tables to the default value*/
    for (index = 0; index < NO_OF_SIMULTANEOUS_FILTERS; index++)
    {
	    FilterDescript[index].status      = SECTION_SLOT_FREE;
	    FilterDescript[index].Mode            = ONE_KILO_SECTION;
		FilterDescript[index].pstQueueId = NULL;
		FilterDescript[index].Callback  	= NULL;
		FilterDescript[index].IsAVITslot = FALSE;
    }
    /* Create section filter monitor process */
    FILTER_INFO(( "Creating SFILTER MONITOR Process\n"));
    SectionTaskId = DRV_OS_TaskCreate("sfilter_monitor",SectionFilterMonitor,SFILTER_MONITOR_PRIORITY, 
                                                                     SFILTER_MONITOR_WORKSPACE,NULL,0) ;                               
    
    if (SectionTaskId == NULL)
    {
        FILTER_ERROR(( "SFILTER_INIT=> Unable to create monitor process\n"));
		return TRUE;
    }
    
    FILTER_INFO(( "NO_OF_SIMULTANEOUS_FILTERS equal to %d\n", NO_OF_SIMULTANEOUS_FILTERS));
    FILTER_INFO(( "Succesfully Installed SFILTER module\n"));
    return FALSE;
}

DRV_ErrCode DRV_SectionFilterInit(void)
{
	DRV_ErrCode errCode; 
	errCode = SRSTF_SectionFilterInit();
	return errCode;
}

//找出那些slot channel使用的filter索引值!
#define MAX_SHARE_CHANNEL_NUM DMX_TOTALCHAN_CNT
static HI_HANDLE mShareChannelFilter[MAX_SHARE_CHANNEL_NUM];
static S32 FindChannelIndex(HI_HANDLE Channel,HI_U32 *ShareFilterNum_p)
{
    S32 index = 0;
	HI_U32 num = 0;
	S32 ret = -1;

    for(index = 0; index < NO_OF_SIMULTANEOUS_FILTERS; index++)
    {
        if((FilterDescript[index].status != SECTION_SLOT_FREE) && (FilterDescript[index].SlotHandle == Channel))
        {
            ret = index;
            mShareChannelFilter[num] = index;
            num ++;
        }
    }
    
    if(ShareFilterNum_p != NULL)
        *ShareFilterNum_p = num;
    	
    return ret;    
}

/*******************************************************************************/
BOOL mFilterTaskRun = TRUE;
static void SectionFilterMonitor(void *pvParam)
{
    DRV_ErrCode ErrCode;
    U8 *Destination0;
    U32 SectLength;
	HI_U32 ChannelIndex,ChannelGotIndex,AcquiredIndex,h;
	HI_U32 u32AcquireNum = 1;
	HI_U32 pu32AcquiredNum;
	HI_U32 ShareFilterNum;
	HI_U32 ValidMatchFilter;
	HI_UNF_DMX_DATA_S sSection[10];
	static HI_HANDLE Channel[MAX_SHARE_CHANNEL_NUM];
	static HI_HANDLE CurrentChannelId;
	HI_U32   ChNum = MAX_SHARE_CHANNEL_NUM;
    static U32 timeTempUs, timeLastUs;
    
    g_Filter_puiSectData = (unsigned char *)DRV_OSMalloc(g_pPtnSectionDataBuffer,4096+16);
    timeLastUs =  DRV_OS_TimeNow_US();
	Destination0 = g_Filter_puiSectData;

    while (TRUE)
    {
    	if(mFilterTaskRun == FALSE)
    	{
    		DRV_OS_TaskDelay_MS(100);
    		continue;
    	}
    		
    	ChNum = MAX_SHARE_CHANNEL_NUM;
		ErrCode = HI_UNF_DMX_GetDataHandle(Channel,&ChNum,SECTION_WAIT_TIMEOUT_STEP);
		timeTempUs = DRV_OS_TimeNow_US();
		if (ErrCode != HI_SUCCESS )
		{
			if (ErrCode == HI_ERR_DMX_TIMEOUT)
			{
				for (ChannelIndex = 0; ChannelIndex < NO_OF_SIMULTANEOUS_FILTERS; ChannelIndex++)
				{
					if (FilterDescript[ChannelIndex].status == SECTION_SLOT_IN_USE)
					{
						if ( FilterDescript[ChannelIndex].nTimeOutCount <= 0 )
						{
							/*Disable it from processing further incoming data*/
							FilterDescript[ChannelIndex].status = SECTION_SLOT_TIMEOUT;
							if (FilterDescript[ChannelIndex].Callback != NULL)
							{
								FilterDescript[ChannelIndex].Callback(ChannelIndex, TRUE, NULL, 0);
							}
						}
						FilterDescript[ChannelIndex].nTimeOutCount -= SECTION_WAIT_TIMEOUT_STEP;
					}
				}
				timeLastUs = timeTempUs;
			}
			else if(ErrCode == HI_ERR_DMX_NOAVAILABLE_DATA)
			{
				DRV_OS_TaskDelay_MS(10);
			}
			else
			{
			    FILTER_ERROR(("SectionFilterMonitor HI_UNF_DMX_GetDataHandle wait Failed!ErrCode[0x%08x]\n",ErrCode));
			}
			continue;
		}        	
    
		DRV_OS_SemWait(CancelWait,DRVOS_WAIT_INFINITY,0);   	
		for(ChannelGotIndex = 0; ChannelGotIndex < ChNum; ChannelGotIndex++) 
		{
			CurrentChannelId = FindChannelIndex(Channel[ChannelGotIndex],&ShareFilterNum);//确定是否绑定了filter，并且有过滤条件
			if(CurrentChannelId < 0)
			{
		        FILTER_ERROR(("SectionFilterMonitor FindChannelIndex faild,Channel[0x%08x] 0x%08x\n", ChannelGotIndex,Channel[ChannelGotIndex]));
        	    DRV_OSSemSignal(CancelWait);
		        continue;	
			}

			pu32AcquiredNum = 0;
			u32AcquireNum = 1;
			ErrCode = HI_UNF_DMX_AcquireBuf(Channel[ChannelGotIndex], u32AcquireNum, &pu32AcquiredNum, sSection, SECTION_WAIT_TIMEOUT_STEP);
			if(pu32AcquiredNum != 1)
			{
		        FILTER_ERROR(("SectionFilterMonitor HI_UNF_DMX_AcquireBuf Faild,Channel[0x%08x] 0x%08x\n", ChannelGotIndex,Channel[ChannelGotIndex]));
        	    DRV_OSSemSignal(CancelWait);
		        continue;
			}

	        if ((ErrCode == HI_SUCCESS) && (pu32AcquiredNum > 0))//目前只支持pu32AcquiredNum = 1
	        {
	        	Destination0 = g_Filter_puiSectData; 
	            for (AcquiredIndex = 0; AcquiredIndex < pu32AcquiredNum; AcquiredIndex++)
	            {
	                if (sSection[AcquiredIndex].enDataType == HI_UNF_DMX_DATA_TYPE_WHOLE)
	                {
	                	memcpy(Destination0,sSection[AcquiredIndex].pu8Data,sSection[AcquiredIndex].u32Size);
	                	SectLength = sSection[AcquiredIndex].u32Size;
	                	
	                }
	                else
	                {
			        	STTBX_Print(("SectionFilterMonitor HI_UNF_DMX_AcquireBuf enDataType 0x%08x\n",sSection[AcquiredIndex].enDataType));
	                }
	            }
        		ErrCode = HI_UNF_DMX_ReleaseBuf(Channel[ChannelGotIndex], pu32AcquiredNum, sSection);
				if (ErrCode != HI_SUCCESS)
				{
			        STTBX_Print(("SectionFilterMonitor HI_UNF_DMX_ReleaseBuf ErrCode 0x%08x\n",ErrCode));
				}
				
	            DRV_OSSemSignal(CancelWait);
	            ValidMatchFilter = 0;
	            for(ChannelIndex = 0; ChannelIndex < ShareFilterNum; ChannelIndex++)
	            {
				  	if(FilterDescript[mShareChannelFilter[ChannelIndex]].status == SECTION_SLOT_IN_USE)
				  	{
				  		h = 0;
			  			if((FilterDescript[mShareChannelFilter[ChannelIndex]].FilterMask[0] & Destination0[0])\
			  				== (FilterDescript[mShareChannelFilter[ChannelIndex]].FilterMask[0] & FilterDescript[mShareChannelFilter[ChannelIndex]].FilterData[0]))
		  				{
						  	for(h=1;h<FILTER_DEPTH_SIZE-1;h++)
						  	{
						  		if(FilterDescript[mShareChannelFilter[ChannelIndex]].FilterMask[h] != 0 )
					  			{
								  	if((FilterDescript[mShareChannelFilter[ChannelIndex]].FilterMask[h] & Destination0[h+2])
					  					!= (FilterDescript[mShareChannelFilter[ChannelIndex]].FilterMask[h] & FilterDescript[mShareChannelFilter[ChannelIndex]].FilterData[h]))
					  				break;	
						  		}
						  	}
		  				}
		  		
					  	if(h == FILTER_DEPTH_SIZE-1)
					  	{
					  		ValidMatchFilter ++;
	        		  		if ((FilterDescript[mShareChannelFilter[ChannelIndex]].Callback != NULL)&&(g_TunerLock))
	        		  			FilterDescript[mShareChannelFilter[ChannelIndex]].Callback(mShareChannelFilter[ChannelIndex], FALSE, Destination0, SectLength);
					  	}
				  	}
				  	else	
				  	{
						;//FILTER_ERROR((" MatchFilter not in use!FitlerNo[%d]  status[%d]\n", CurrentChannelId,FilterDescript[mShareChannelFilter[ChannelIndex]].status));
				  	}
			  	}
			  	if(ValidMatchFilter == 0)
			  	{
					U32 errCount;
					#ifdef	SECTION_DEBUG
					FILTER_ERROR(("SectionFilterMonitor can not find MatchFilter ShareFilterNum %d\n",ShareFilterNum));
					for(errCount = 0;errCount < ShareFilterNum;errCount++)
					{
			        	STTBX_Print(("MatchFilter index[%d] FilterMask ",mShareChannelFilter[errCount]));
			        	for(h=0;h<FILTER_DEPTH_SIZE;h++)
			        	{
			        		STTBX_Print(("0x%02x ",FilterDescript[mShareChannelFilter[errCount]].FilterMask[h]));
			        	}
			        	STTBX_Print(("\n"));
			        	STTBX_Print(("MatchFilter index[%d] FilterData ",mShareChannelFilter[errCount]));
			        	for(h=0;h<FILTER_DEPTH_SIZE;h++)
			        	{
			        		STTBX_Print(("0x%02x ",FilterDescript[mShareChannelFilter[errCount]].FilterData[h]));
			        	}
			        	STTBX_Print(("\n"));
					}
		        	STTBX_Print(("SectionFilterMonitor Destinati0 "));
		        	for(h=0;h<FILTER_DEPTH_SIZE;h++)
		        	{
		        		STTBX_Print(("0x%02x ",Destination0[h]));
		        	}
					
		        	STTBX_Print(("\n"));
		        	#endif
				}
		    }
	        else
	        {
        		ErrCode = HI_UNF_DMX_ReleaseBuf(Channel[ChannelGotIndex], pu32AcquiredNum, sSection);
				if (ErrCode != HI_SUCCESS)
				{
			        STTBX_Print(("SectionFilterMonitor HI_UNF_DMX_ReleaseBuf ErrCode 0x%08x\n",ErrCode));
				}	        
				
	            printf("SectionFilterMonitor HI_UNF_DMX_AcquireBuf time out\n");
		       	if((timeTempUs -timeLastUs) > (SECTION_WAIT_TIMEOUT_STEP* 1000))
		    	{
					timeLastUs = timeTempUs;
					for (ChannelIndex = 0; ChannelIndex < NO_OF_SIMULTANEOUS_FILTERS; ChannelIndex++)
					{
						if (FilterDescript[ChannelIndex].status == SECTION_SLOT_IN_USE)
						{
							if ( FilterDescript[ChannelIndex].nTimeOutCount <= 0 )
							{
								/*Disable it from processing further incoming data*/
								FilterDescript[ChannelIndex].status = SECTION_SLOT_TIMEOUT;

					            if (FilterDescript[ChannelIndex].Callback != NULL)
									FilterDescript[ChannelIndex].Callback(ChannelIndex, TRUE, NULL, 0);
							}
							FilterDescript[ChannelIndex].nTimeOutCount -= SECTION_WAIT_TIMEOUT_STEP;
						}
					}
		    	}   
		    	DRV_OSSemSignal(CancelWait);
		    }
	    }	
		
		if((timeTempUs -timeLastUs) > (SECTION_WAIT_TIMEOUT_STEP*1000))
		{
			timeLastUs = timeTempUs;
			for (ChannelIndex = 0; ChannelIndex < NO_OF_SIMULTANEOUS_FILTERS; ChannelIndex++)
			{
				if (FilterDescript[ChannelIndex].status == SECTION_SLOT_IN_USE)
				{
					if ( FilterDescript[ChannelIndex].nTimeOutCount <= 0 )
					{
						/*Disable it from processing further incoming data*/
						FilterDescript[ChannelIndex].status = SECTION_SLOT_TIMEOUT;

			            if (FilterDescript[ChannelIndex].Callback != NULL)
						FilterDescript[ChannelIndex].Callback(ChannelIndex, TRUE, NULL, 0);
					}
					FilterDescript[ChannelIndex].nTimeOutCount -= SECTION_WAIT_TIMEOUT_STEP;
				}
			}
		}
    }
}


/**********************************************************************
判断channel是否使用了多个filter?
***********************************************************************/
static S32 IsSlotShared(S32 SlotIndex)
{
    S32 index = 0;
    while (index < NO_OF_SIMULTANEOUS_FILTERS)
    {
	if (index != SlotIndex && \
	        FilterDescript[index].status != SECTION_SLOT_FREE && \
	        FilterDescript[index].pidDataPid == FilterDescript[SlotIndex].pidDataPid)
        {   
           return (index);
	}
        index++;
    }
    return(-1);
}


/************************************************************************************************/

DRV_ErrCode DRV_filterStatus(void)
{
    S32 TempSlot;

    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    
    /*  if all the filters are occupied then quit this routine*/
    if (NoOfFreeSlots >= NO_OF_SIMULTANEOUS_FILTERS)
    {
        DRV_OSSemSignal(SectionTableAccess);
	    FILTER_ERROR(( "Section Engine is Idle ...\n\n"));
        return TRUE;
    }

    FILTER_ERROR(( "No of Slots Free : %d\n", NoOfFreeSlots));

    /* find the free hardware filter by checking in the array of section data
     * structure */
    TempSlot= 0;
    while (TempSlot < NO_OF_SIMULTANEOUS_FILTERS)
    {
	    if (FilterDescript[TempSlot].status != SECTION_SLOT_FREE)
	    {

	        FILTER_ERROR(( "FilterSlot [%d] Status [%d] Pid [0x%04X] currenttimeout [%d] orgtimeout [%d] callback 0x%8.8x Mask_MS:LS [%X:%X] Data_MS:LS [%X:%X]\n",
		                   TempSlot, FilterDescript[TempSlot].status,
		                   FilterDescript[TempSlot].pidDataPid,
		                   FilterDescript[TempSlot].nTimeOutCount,
		                   FilterDescript[TempSlot].nOrgTimeOutCount,
		                   (UINT)FilterDescript[TempSlot].Callback,
		                   *((UINT *)&FilterDescript[TempSlot].FilterMask[0]),
		                   *((UINT *)&FilterDescript[TempSlot].FilterMask[4]),
		                   *((UINT *)&FilterDescript[TempSlot].FilterData[0]),
		                   *((UINT *)&FilterDescript[TempSlot].FilterData[4])));
	    }

	    ++TempSlot;
    }
    DRV_OSSemSignal(SectionTableAccess);
    return FALSE;
}

void drv_FilterSwapMask(HI_U8 *Src,HI_U8 *Dst)
{
	HI_U8 DataBuffer[16];
	HI_U8 DataSwap;
	HI_U8 Index;
	memcpy(DataBuffer,Src,16);
	for(Index =0;Index < 16;Index++)
	{
		DataSwap = ~DataBuffer[Index];
		DataBuffer[Index] = DataSwap;
	}
	memcpy(Dst,DataBuffer,16);
}
S32	DRV_FILTReqFilterForStandard (DRVOS_MsgQueue_t*	ReturnQueueId,
						FiltCallBack		pCallbackFunc,
						sf_filter_mode_t	FilterMode,
						STPTI_Pid_t		    ReqPid,
						S32					ReqTableId,
						S32					ReqProgNo,
						S32					ReqSectionNo,
						S32					ReqVersionNo,
						S32					nTimeOutCount,
						S32                 iTsId,
						S32                 iOriginalNetworkId)
{
	DRV_ErrCode	ErrCode;
	S32		    ActIndx;
	S32			SlotId = 0;
	S32			index = 0;
	WORD2BYTE   *DataMask;
	HI_HANDLE   Channel = 0;
	HI_HANDLE   NewChannel = 0;
	HI_HANDLE   NewFilter = 0;
	BOOL		SlotShared = FALSE;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
	memset(&stFilterAttr,0,sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    /* lock the section storage table  */
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    if (NoOfFreeSlots <= 0)
    {
    	DRV_OSSemSignal(SectionTableAccess);
    	#ifdef	SECTION_DEBUG
	    FILTER_ERROR(( "No Free Filter => PID [%d] TableId [%d] SectionNo [%d]  VersionNo [%d]\n",
				ReqPid, (UINT)ReqTableId, ReqSectionNo, ReqVersionNo));
		#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }

    /* find the free filter slot by checking in the array of section data structure */
    while (SlotId < NO_OF_SIMULTANEOUS_FILTERS)
    {
		if (FilterDescript[SlotId].status == SECTION_SLOT_FREE)
            break;
		SlotId++;
    }
    
    /*No free filter */
    if (SlotId >= NO_OF_SIMULTANEOUS_FILTERS)
    {
		DRV_OSSemSignal(SectionTableAccess);
		#ifdef	SECTION_DEBUG
		FILTER_ERROR(( "No Free Filter => PID [%d]  TableId [%d]  SectionNo [%d]  VersionNo [%d]\n",
			               ReqPid, (UINT)ReqTableId, ReqSectionNo, ReqVersionNo));
		#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }
           
    /*check for any other on-going section filter reqs to the same PID */
    ErrCode = HI_UNF_DMX_GetChannelHandle(0,ReqPid,&Channel);
	if ((ErrCode != HI_SUCCESS)&&(ErrCode != HI_ERR_DMX_UNMATCH_CHAN))
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("[1]DRV_FILTReqFilterForStandard HI_UNF_DMX_GetChannelHandle ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
    if (Channel != 0)
    {
		/* check if the PID is received on desired slot */
		ActIndx = FindChannelIndex(Channel,NULL);
		if(ActIndx < 0)
		{
	        DRV_OSSemSignal(SectionTableAccess);
	        FILTER_ERROR(("[standard] slot exist but not found in FilterDescript! Channel[%d]  ReqPid[%d]\n", Channel,ReqPid));
	        return (SECTION_ERROR_OTHERS);            
		}
		else
		{
		    SlotShared = TRUE;
		}
    }
    
    if(!SlotShared) //没有配置这个PID的channel，所以需要create channel
    {
		#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[standard] no slot is collecting pid[%d], so allocate a new one\n",ReqPid));
		#endif
	 	ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("[2]DRV_FILTReqFilterForStandard HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
		stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
		stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
		stChnAttr.u32BufSize = 64*1024;
		ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&NewChannel);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("[3]DRV_FILTReqFilterForStandard HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
    }
    else 
    {
		NewChannel = Channel;
		//ErrCode = HI_UNF_DMX_DetachFilter(FilterDescript[ActIndx].FilterHandle,FilterDescript[ActIndx].SlotHandle);
		ErrCode = HI_UNF_DMX_CloseChannel(FilterDescript[ActIndx].SlotHandle);
	    if (ErrCode != ST_NO_ERROR)
	    {
	        FILTER_ERROR(("[standard] HI_UNF_DMX_CloseChannel Failed!  ErrCode[%d]\n",ErrCode));         
	    }            
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[standard] the slot is collecting pid[%d], so no allocate,SlotHandle is %d\n",ReqPid,NewChannel));
#endif
    }
     
    /* setup the filter DATA & MASK. */
    memset(FilterDescript[SlotId].FilterData,0,FILTER_DEPTH_SIZE);
    memset(FilterDescript[SlotId].FilterMask,0,FILTER_DEPTH_SIZE);
      
    /* incoming data is in the following format:
    *	BYTE		DESCRIPTION		BYTE_POSITION_IN_SF	EXAMPLE
    *	=============================================================
    *	0		TABLE_ID (8)		MS_WORD.BYTE0		PAT_TABLE_ID
    *	1,2		SECTION_LENGTH (12)	-			-
    *	3,4		TP_STREAM_ID (16)	MS_WORD.BYTE1,2		0x0000
    *	5.6-7		RESERVED (2)		MS_WORD.BYTE3.6-7	0x3
    *	5.5-1		VERSION_NUMBER (5)	MS_WORD.BYTE3.5-1	0
    *	5.0		CUR_NXT_INDICATOR (1)	MS_WORD.BYTE3.0		0
    *	6		SECTION_NUMBER (8)	LS_WORD.BYTE0		0
    *	7		LAST_SECTION_NUMBER (8)	LS_WORD.BYTE1		0
    *	8		-			LS_WORD.BYTE2		0
    *	9		-			LS_WORD.BYTE3		0
    */
 
	/** setup MS_data word ( DATA_BYTE 00 to 03 )	*/
	DataMask = ( WORD2BYTE * )( &(FilterDescript[SlotId].FilterData[0]));
	if( ReqTableId != 0x70 )
		DataMask -> uiWord32			= 0x01000000;	/* reserved bits */
	if( ReqTableId != -1 )
	{
		DataMask -> byte . ucByte0	= ReqTableId;
	}
	if( ReqVersionNo != -1 )
	{
		DataMask -> byte . ucByte3	|= ( ReqVersionNo << 1 );
	}
	if ( ReqProgNo != -1 )
	{
		if(ReqTableId!=0x42)
		{
			DataMask -> byte . ucByte1	= ( unsigned char ) ( ( ReqProgNo & 0xFF00 ) >> 8 );
			DataMask -> byte . ucByte2	= ( unsigned char ) ( ReqProgNo & 0xFF );
		}
	}
      
	/** setup MS_mask word ( MASK_BYTE 00 to 03 )	*/
	DataMask = ( WORD2BYTE * )( &(FilterDescript[SlotId].FilterMask[0]));

	if (ReqTableId != 0x70)
		DataMask -> uiWord32		= 0x01000000;	/* reserved bits & TableId */
	else
		DataMask -> uiWord32		= 0x00000000;	/* TableId only*/
	if (ReqTableId != -1)
	{
		DataMask->byte.ucByte0 = 0xFF;
	}

	if ( ReqVersionNo != -1 )
		DataMask -> uiWord32	|= SECTION_VERSION_MASK;
     if ( ReqProgNo != -1 )
	{
		if(ReqTableId!=0x42)
			DataMask -> uiWord32	|= SECTION_PROG_NO_SERVICE_ID_MASK;
      }


     /** setup LS_data word ( DATA_BYTE 04 to 07 )	*/
	DataMask = ( WORD2BYTE * ) (&(FilterDescript[SlotId].FilterData[4]));
	DataMask -> uiWord32	= 0;
	if ( ReqSectionNo != -1 )
	{
		DataMask -> byte . ucByte0	= ( unsigned char ) ReqSectionNo;
	}
	if( ( iTsId != -1 )&&((ReqTableId>=0x60)&&(ReqTableId<=0x6f)||(ReqTableId==0x4f)))
	{
		DataMask -> byte . ucByte2	= ( unsigned char ) ( ( iTsId & 0xFF00 ) >> 8 );
		DataMask -> byte . ucByte3	= ( unsigned char ) ( iTsId & 0xFF );
		
	}
       else
       {
       	if((iOriginalNetworkId!=-1)&&(ReqTableId==0x46))
       	{
       		DataMask -> byte . ucByte2	= ( unsigned char ) ( ( iOriginalNetworkId & 0xFF00 ) >> 8 );
       		DataMask -> byte . ucByte3	= ( unsigned char ) ( iOriginalNetworkId & 0xFF );
       	}
       }

	/** setup LS_mask word ( MASK_BYTE 04 to 07 )	*/
	DataMask = ( WORD2BYTE * )(&(FilterDescript[SlotId].FilterMask[4]));
	DataMask -> uiWord32				= 0;
	if( ReqSectionNo != -1 )
	{
		DataMask -> byte . ucByte0	= 0xFF;		
	}
	if( ( iTsId != -1 )&&((ReqTableId>=0x60)&&(ReqTableId<=0x6f)||(ReqTableId==0x4f)))
	{
		DataMask -> uiWord32	|= 0xffff0000;
	}
       else
       {
       	if((iOriginalNetworkId!=-1)&&(ReqTableId==0x46))
       	{
       		DataMask -> uiWord32	|= 0xffff0000;
       	}
       }
	/*
	* setup LS_data word ( DATA_BYTE 08 to 11 )
	*/
	DataMask = ( WORD2BYTE * ) (&(FilterDescript[SlotId].FilterData[8]));
	DataMask -> uiWord32	= 0;
	if( ( iTsId != -1 )&&((ReqTableId>=0x60)&&(ReqTableId<=0x6f)||(ReqTableId==0x4f)))
	{
		DataMask -> byte . ucByte0	= ( unsigned char ) ( ( iOriginalNetworkId & 0xFF00 ) >> 8 );
		DataMask -> byte . ucByte1	= ( unsigned char ) ( iOriginalNetworkId & 0xFF );
	}
       
	/*
	* setup LS_mask word ( MASK_BYTE 08 to 11 )
	*/
	DataMask = ( WORD2BYTE * )(&(FilterDescript[SlotId].FilterMask[8]));
	DataMask -> uiWord32	= 0;
	if( ( iTsId != -1 )&&((ReqTableId>=0x60)&&(ReqTableId<=0x6f)||(ReqTableId==0x4f)))
	{
		DataMask -> uiWord32	|= 0x0000ffff;
	}
    	
    //FilterDescript[SlotId].FilterType = STPTI_FILTER_TYPE_SECTION_FILTER_SHORT_MODE;

    /* set filter attr */
    memset(&g_mask,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_match,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_Negate,0,sizeof(FILTER_DEPTH_SIZE));
    stFilterAttr.u32FilterDepth = FILTER_DEPTH_SIZE;
	drv_FilterSwapMask(FilterDescript[SlotId].FilterMask,stFilterAttr.au8Mask);
	memcpy(stFilterAttr.au8Match,FilterDescript[SlotId].FilterData,FILTER_DEPTH_SIZE);
	memcpy(stFilterAttr.au8Negate,&g_Negate,FILTER_DEPTH_SIZE);
	ErrCode = HI_UNF_DMX_CreateFilter(0,&stFilterAttr,&NewFilter);
	if (ErrCode != HI_SUCCESS)
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("[4]DRV_FILTReqFilterForStandard HI_UNF_DMX_CreateFilter ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	ErrCode = HI_UNF_DMX_AttachFilter(NewFilter,NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
        HI_UNF_DMX_DestroyFilter(NewFilter);
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("[5]DRV_FILTReqFilterForStandard HI_UNF_DMX_AttachFilter ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	/* decrement the number of free slots */
	 --NoOfFreeSlots;
    /* initialise this section structure */
    FilterDescript[SlotId].pstQueueId   = ReturnQueueId;
    FilterDescript[SlotId].SlotHandle   = NewChannel;
    FilterDescript[SlotId].BufferHandle = NewChannel; 
    FilterDescript[SlotId].FilterHandle = NewFilter;
    FilterDescript[SlotId].pidDataPid   = ReqPid;
    FilterDescript[SlotId].ucTableId    = ReqTableId;
    FilterDescript[SlotId].VersionNo    = ReqVersionNo;
    FilterDescript[SlotId].sSectionNo   = ReqSectionNo;
    FilterDescript[SlotId].StreamId     = ReqProgNo;

    FilterDescript[SlotId].TsId				 = iTsId;
    FilterDescript[SlotId].OriginalNetworkId = iOriginalNetworkId;
    FilterDescript[SlotId].nTimeOutCount     = nTimeOutCount;	
    FilterDescript[SlotId].nOrgTimeOutCount  = nTimeOutCount;	    
    FilterDescript[SlotId].Callback		     = pCallbackFunc;
    FilterDescript[SlotId].status            = SECTION_SLOT_IN_USE;
    FilterDescript[SlotId].MaxSectionLen     = ONE_KILO_SECTION_MAX_LENGTH * FilterMode;

	ErrCode = HI_UNF_DMX_SetChannelPID(NewChannel,ReqPid);
	ErrCode |= HI_UNF_DMX_OpenChannel(NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
		++NoOfFreeSlots;
		FilterDescript[SlotId].status = SECTION_SLOT_FREE;
		FilterDescript[SlotId].pidDataPid = -1; /*  IsSlotShared */
		FilterDescript[SlotId].Callback = NULL; 
		FilterDescript[SlotId].BufferHandle = 0;
		FilterDescript[SlotId].SlotHandle = 0;
		HI_UNF_DMX_DetachFilter(NewFilter,NewChannel);
		HI_UNF_DMX_DestroyFilter(NewFilter);
        DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("[6]DRV_FILTReqFilterForStandard HI_UNF_DMX_SetChannelPID ErrCode 0x%08x\n",ErrCode));
        return (SECTION_ERROR_ST_ERROR);
	}
    
#ifdef SECTION_DEBUG
    FILTER_ERROR(( "[standard]: Pid = %d Slot= %d Mask_MS:LS [%X:%X] Data_MS:LS [%X:%X]\n",
	             ReqPid,SlotId, *((UINT *)&FilterDescript[SlotId].FilterMask[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterMask[4]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[4])));
#endif

    DRV_OSSemSignal(SectionTableAccess);
    return SlotId;

}


static void drv_FFILTResetFilterDiscDescript(int FilterNO)
{
    FilterDescript[FilterNO].status = SECTION_SLOT_FREE;
    FilterDescript[FilterNO].pidDataPid = 8191; /*  IsSlotShared */
    FilterDescript[FilterNO].Callback = NULL; /* FindFilter */
    FilterDescript[FilterNO].BufferHandle = 0xFFFFFFFF;
    FilterDescript[FilterNO].SlotHandle = 0xFFFFFFFF;
	FilterDescript[FilterNO].FilterHandle = 0xFFFFFFFF;
	FilterDescript[FilterNO].SlotHandle = 0xFFFFFFFF;
	FilterDescript[FilterNO].nTimeOutCount = 0;
	FilterDescript[FilterNO].nOrgTimeOutCount = 0;
    FilterDescript[FilterNO].IsAVITslot = FALSE;
}

S32	DRV_FILTReqFilterFree_CRCFlag(FiltCallBack pCallbackFunc,
							               sf_filter_mode_t	FilterMode,
											unsigned char *paucFilterData,
											unsigned char *paucFilterMask,
											STPTI_Pid_t ReqPid,
											S32 nTimeOutCount,
											BOOL_YS CRCFlag)
{
	DRV_ErrCode	ErrCode;
	S32		    ActIndx;
	S32			SlotId = 0;
	WORD2BYTE   *DataMask;
	HI_HANDLE   Channel = 0;
	HI_HANDLE   NewChannel = 0;
	HI_HANDLE   NewFilter = 0;
	BOOL		SlotShared = FALSE;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
	memset(&stFilterAttr,0,sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    /* lock the section storage table  */
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    if (NoOfFreeSlots <= 0)
    {
    	DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
	    FILTER_ERROR(("No Free Filter => PID [%d]\n", ReqPid));
#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }

    /* find the free filter slot by checking in the array of section data structure */
    while (SlotId < NO_OF_SIMULTANEOUS_FILTERS)
    {
		if (FilterDescript[SlotId].status == SECTION_SLOT_FREE)
            break;
		SlotId++;
    }
    
    /*No free filter */
    if (SlotId >= NO_OF_SIMULTANEOUS_FILTERS)
    {
		DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
	    FILTER_ERROR(("No Free Filter => PID [%d]\n", ReqPid));
#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }
           
    /*check for any other on-going section filter reqs to the same PID */
    ErrCode = HI_UNF_DMX_GetChannelHandle(0,ReqPid,&Channel);
	if ((ErrCode != HI_SUCCESS)&&(ErrCode != HI_ERR_DMX_UNMATCH_CHAN))
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_GetChannelHandle ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
    if (Channel != 0)
    {
		/* check if the PID is received on desired slot */
		ActIndx = FindChannelIndex(Channel,NULL);
		if(ActIndx < 0)
		{
	        DRV_OSSemSignal(SectionTableAccess);
	        FILTER_ERROR(("[CRCFlag] slot exist but not found in FilterDescript! Channel[%d]  ReqPid[%d]\n", Channel,ReqPid));
	        return (SECTION_ERROR_OTHERS);            
		}
		else
		{
		    SlotShared = TRUE;
		}
    }
    
    if(!SlotShared)  //没有配置这个PID的channel，所以需要create channel
    {
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[CRCFlag] no slot is collecting pid[%d], so allocate a new one\n",ReqPid));
#endif
 	 	ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
		stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
		stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
		stChnAttr.u32BufSize = 64*1024;
       	if (paucFilterData[0] == 0x3c)
			stChnAttr.u32BufSize = 128*1024;
		ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&NewChannel);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
    }
    else 
    {
		NewChannel = Channel;
		//ErrCode = HI_UNF_DMX_DetachFilter(FilterDescript[ActIndx].FilterHandle,FilterDescript[ActIndx].SlotHandle);
		ErrCode = HI_UNF_DMX_CloseChannel(FilterDescript[ActIndx].SlotHandle);
	    if (ErrCode != ST_NO_ERROR)
	    {
	        FILTER_ERROR(("[CRCFlag] HI_UNF_DMX_CloseChannel Failed!  ErrCode[%d]\n",ErrCode));         
	    }            
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[CRCFlag] the slot is collecting pid[%d], so no allocate,SlotHandle is %d\n",ReqPid,NewChannel));
#endif
    }
     
    memcpy(FilterDescript[SlotId].FilterData, paucFilterData, FILTER_DEPTH_SIZE);
    memcpy(FilterDescript[SlotId].FilterMask, paucFilterMask, FILTER_DEPTH_SIZE);

    //FilterDescript[SlotId].FilterType = STPTI_FILTER_TYPE_SECTION_FILTER_SHORT_MODE;
#if 0
    if(CRCFlag)
    {
		FilterDescript[SlotId].FilterValues.u.SectionFilter.DiscardOnCrcError = TRUE;
    }
    else
    {
		FilterDescript[SlotId].FilterValues.u.SectionFilter.DiscardOnCrcError = FALSE;
    }
#endif
    /* set filter attr */
    memset(&g_mask,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_match,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_Negate,0,sizeof(FILTER_DEPTH_SIZE));
    stFilterAttr.u32FilterDepth = FILTER_DEPTH_SIZE;
	drv_FilterSwapMask(FilterDescript[SlotId].FilterMask,stFilterAttr.au8Mask);
	memcpy(stFilterAttr.au8Match,FilterDescript[SlotId].FilterData,FILTER_DEPTH_SIZE);
	memcpy(stFilterAttr.au8Negate,&g_Negate,FILTER_DEPTH_SIZE);
	ErrCode = HI_UNF_DMX_CreateFilter(0,&stFilterAttr,&NewFilter);
	if (ErrCode != HI_SUCCESS)
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_CreateFilter ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	ErrCode = HI_UNF_DMX_AttachFilter(NewFilter,NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
        HI_UNF_DMX_DestroyFilter(NewFilter);
        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_AttachFilter ErrCode 0x%08x\n",ErrCode));
        DRV_OSSemSignal(SectionTableAccess);
		return (SECTION_ERROR_ST_ERROR);
	}
	/* decrement the number of free slots */
	 --NoOfFreeSlots;
    /* initialise this section structure */
    FilterDescript[SlotId].SlotHandle   = NewChannel;
    FilterDescript[SlotId].FilterHandle = NewFilter;
    FilterDescript[SlotId].BufferHandle = NewChannel;
    FilterDescript[SlotId].pidDataPid     = ReqPid;

    FilterDescript[SlotId].nTimeOutCount = nTimeOutCount;	
    FilterDescript[SlotId].nOrgTimeOutCount = nTimeOutCount;	    
    FilterDescript[SlotId].Callback		  = pCallbackFunc;
    FilterDescript[SlotId].MaxSectionLen      = ONE_KILO_SECTION_MAX_LENGTH*FilterMode;
    FilterDescript[SlotId].status   = SECTION_SLOT_IN_USE;

	ErrCode = HI_UNF_DMX_SetChannelPID(NewChannel,ReqPid);
	ErrCode |= HI_UNF_DMX_OpenChannel(NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
		++NoOfFreeSlots;
		FilterDescript[SlotId].status = SECTION_SLOT_FREE;
		FilterDescript[SlotId].pidDataPid = -1; /*  IsSlotShared */
		FilterDescript[SlotId].Callback = NULL; 
		FilterDescript[SlotId].BufferHandle = 0;
		FilterDescript[SlotId].SlotHandle = 0;
		HI_UNF_DMX_DetachFilter(NewFilter,NewChannel);
		HI_UNF_DMX_DestroyFilter(NewFilter);
        DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("DRV_FILTReqFilterFree_CRCFlag HI_UNF_DMX_SetChannelPID ErrCode 0x%08x\n",ErrCode));
        return (SECTION_ERROR_ST_ERROR);
	}
    
#ifdef SECTION_DEBUG
    FILTER_ERROR(( "[CRCFlag]: Pid = %d Slot= %d Mask_MS:LS [%X:%X] Data_MS:LS [%X:%X]\n",
	             ReqPid,SlotId, *((UINT *)&FilterDescript[SlotId].FilterMask[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterMask[4]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[4])));
#endif

    DRV_OSSemSignal(SectionTableAccess);
    return SlotId;

}

S32	DRV_FILTReqFilterFree  (			FiltCallBack		pCallbackFunc,
							sf_filter_mode_t	FilterMode,
							unsigned char				*paucFilterData,
							unsigned char				*paucFilterMask,
							STPTI_Pid_t /*pid_t*/				ReqPid,
							S32					nTimeOutCount
							)
{
	DRV_ErrCode	ErrCode;
	S32		    ActIndx;
	S32			SlotId = 0;
	WORD2BYTE   *DataMask;
	HI_HANDLE   Channel = 0;
	HI_HANDLE   NewChannel = 0;
	HI_HANDLE   NewFilter = 0;
	BOOL		SlotShared = FALSE;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
	memset(&stFilterAttr,0,sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    /* lock the section storage table  */
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    if (NoOfFreeSlots <= 0)
    {
    	DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
	    FILTER_ERROR(("No Free Filter => PID [%d]\n", ReqPid));
#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }

    /* find the free filter slot by checking in the array of section data structure */
    while (SlotId < NO_OF_SIMULTANEOUS_FILTERS)
    {
		if (FilterDescript[SlotId].status == SECTION_SLOT_FREE)
            break;
		SlotId++;
    }
    
    /*No free filter */
    if (SlotId >= NO_OF_SIMULTANEOUS_FILTERS)
    {
		DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
	    FILTER_ERROR(("No Free Filter => PID [%d]\n", ReqPid));
#endif
		return ((S32)SECTION_ERROR_TOO_MANY_FILTER_REQ);
    }
           
    /*check for any other on-going section filter reqs to the same PID */
    ErrCode = HI_UNF_DMX_GetChannelHandle(0,ReqPid,&Channel);
	if ((ErrCode != HI_SUCCESS)&&(ErrCode != HI_ERR_DMX_UNMATCH_CHAN))
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("[7]DRV_FILTReqFilterFree HI_UNF_DMX_GetChannelHandle ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
    if (Channel != 0)
    {
		/* check if the PID is received on desired slot */
		ActIndx = FindChannelIndex(Channel,NULL);
		if(ActIndx < 0)
		{
	        DRV_OSSemSignal(SectionTableAccess);
	        FILTER_ERROR(("[Req] slot exist but not found in FilterDescript! Channel[%d]  ReqPid[%d]\n", Channel,ReqPid));
	        return (SECTION_ERROR_OTHERS);            
		}
		else
		{
		    SlotShared = TRUE;
		}
    }
    
    if(!SlotShared)  //没有配置这个PID的channel，所以需要create channel
    {
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[Req] no slot is collecting pid[%d], so allocate a new one\n",ReqPid));
#endif
    	
	 	ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("DRV_FILTReqFilterFree HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
		stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
		stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
		stChnAttr.u32BufSize = 64*1024;
       if (paucFilterData[0] == 0x3c)
		stChnAttr.u32BufSize = 128*1024;
		ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&NewChannel);
		if (ErrCode != HI_SUCCESS)
		{
			DRV_OSSemSignal(SectionTableAccess);
	        STTBX_Print(("[8]DRV_FILTReqFilterFree HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
			return (SECTION_ERROR_ST_ERROR);
		}
    }
    else 
    {
		NewChannel = Channel;
		//ErrCode = HI_UNF_DMX_DetachFilter(FilterDescript[ActIndx].FilterHandle,FilterDescript[ActIndx].SlotHandle);
		ErrCode = HI_UNF_DMX_CloseChannel(FilterDescript[ActIndx].SlotHandle);
	    if (ErrCode != ST_NO_ERROR)
	    {
	        FILTER_ERROR(("[Req] HI_UNF_DMX_CloseChannel Failed!  ErrCode[%d]\n",ErrCode));         
	    }            
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[Req] the slot is collecting pid[%d], so no allocate,SlotHandle is %d\n",ReqPid,NewChannel));
#endif
    }
     
    memcpy(FilterDescript[SlotId].FilterData, paucFilterData, FILTER_DEPTH_SIZE);
    memcpy(FilterDescript[SlotId].FilterMask, paucFilterMask, FILTER_DEPTH_SIZE);
    
    //FilterDescript[SlotId].FilterType = STPTI_FILTER_TYPE_SECTION_FILTER_SHORT_MODE;

    /* set filter attr */
    memset(&g_mask,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_match,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_Negate,0,sizeof(FILTER_DEPTH_SIZE));
    stFilterAttr.u32FilterDepth = FILTER_DEPTH_SIZE;
	drv_FilterSwapMask(FilterDescript[SlotId].FilterMask,stFilterAttr.au8Mask);
	memcpy(stFilterAttr.au8Match,FilterDescript[SlotId].FilterData,FILTER_DEPTH_SIZE);
	memcpy(stFilterAttr.au8Negate,&g_Negate,FILTER_DEPTH_SIZE);
	ErrCode = HI_UNF_DMX_CreateFilter(0,&stFilterAttr,&NewFilter);
	if (ErrCode != HI_SUCCESS)
	{
		DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("DRV_FILTReqFilterFree HI_UNF_DMX_CreateFilter ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	ErrCode = HI_UNF_DMX_AttachFilter(NewFilter,NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
        STTBX_Print(("DRV_FILTReqFilterFree HI_UNF_DMX_AttachFilter ErrCode 0x%08x\n",ErrCode));
        HI_UNF_DMX_DestroyFilter(NewFilter);
        DRV_OSSemSignal(SectionTableAccess);
		return (SECTION_ERROR_ST_ERROR);
	}
	/* decrement the number of free slots */
	 --NoOfFreeSlots;
    /* initialise this section structure */
    FilterDescript[SlotId].SlotHandle   = NewChannel;
    FilterDescript[SlotId].FilterHandle = NewFilter;
    FilterDescript[SlotId].BufferHandle = NewChannel;
    FilterDescript[SlotId].pidDataPid     = ReqPid;

    FilterDescript[SlotId].nTimeOutCount = nTimeOutCount;	
    FilterDescript[SlotId].nOrgTimeOutCount = nTimeOutCount;	    
    FilterDescript[SlotId].Callback		  = pCallbackFunc;
    FilterDescript[SlotId].MaxSectionLen      = ONE_KILO_SECTION_MAX_LENGTH*FilterMode;
    FilterDescript[SlotId].status   = SECTION_SLOT_IN_USE;

	ErrCode = HI_UNF_DMX_SetChannelPID(NewChannel,ReqPid);
	ErrCode |= HI_UNF_DMX_OpenChannel(NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
		++NoOfFreeSlots;
		FilterDescript[SlotId].status = SECTION_SLOT_FREE;
		FilterDescript[SlotId].pidDataPid = -1; /*  IsSlotShared */
		FilterDescript[SlotId].Callback = NULL; 
		FilterDescript[SlotId].BufferHandle = 0;
		FilterDescript[SlotId].SlotHandle = 0;
		HI_UNF_DMX_DetachFilter(NewFilter,NewChannel);
		HI_UNF_DMX_DestroyFilter(NewFilter);
        DRV_OSSemSignal(SectionTableAccess);
        STTBX_Print(("DRV_FILTReqFilterFree HI_UNF_DMX_SetChannelPID ErrCode 0x%08x\n",ErrCode));
        return (SECTION_ERROR_ST_ERROR);
	}
    
#ifdef SECTION_DEBUG
    FILTER_ERROR(( "[Req]: Pid = %d Slot= %d Mask_MS:LS [%X:%X] Data_MS:LS [%X:%X]\n",
	             ReqPid,SlotId, *((UINT *)&FilterDescript[SlotId].FilterMask[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterMask[4]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[0]),
	             *((UINT *)&FilterDescript[SlotId].FilterData[4])));
#endif

    DRV_OSSemSignal(SectionTableAccess);
    return SlotId;

}

S32	DRV_FILTFreeFilterReq (S32 SlotIndex)
{	
    S32 ShareSlot;
    DRV_ErrCode ErrCode = ST_NO_ERROR;
	
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);

    /* Check for validity */
    if (SlotIndex < 0 || SlotIndex >= NO_OF_SIMULTANEOUS_FILTERS)
    {
        DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
        FILTER_ERROR(("[freefilter]Invalid FilterNo[%d]\n", SlotIndex));
#endif       
		return SECTION_ERROR_INVALID_SLOT_ID;
    }
    ShareSlot = IsSlotShared(SlotIndex); //判断这个channel上是否还是用了其他的filter

    if (FilterDescript[SlotIndex].status == SECTION_SLOT_FREE)
    {
        DRV_OSSemSignal(SectionTableAccess);
        FILTER_ERROR(("[freefilter]Warning : Requesting cancellation of free filter[%d]!", SlotIndex));
        return FALSE;
    }

    DRV_OS_SemWait(CancelWait,DRVOS_WAIT_INFINITY,0);    	

	ErrCode = HI_UNF_DMX_DetachFilter(FilterDescript[SlotIndex].FilterHandle,FilterDescript[SlotIndex].SlotHandle);
	ErrCode |= HI_UNF_DMX_DestroyFilter(FilterDescript[SlotIndex].FilterHandle);
    if (ErrCode != ST_NO_ERROR)
    {
        FILTER_ERROR(("[freefilter] HI_UNF_DMX_DestroyFilter Failed!  ErrCode[%d]\n",ErrCode));         
    }            
    NoOfFreeSlots++;
    if(ShareSlot < 0 )  //这个channel只使用了一个filter，所以destroy filter后，destroy channel
	{
		ErrCode = HI_UNF_DMX_CloseChannel(FilterDescript[SlotIndex].SlotHandle);
		ErrCode |= HI_UNF_DMX_DestroyChannel(FilterDescript[SlotIndex].SlotHandle);
		if (ErrCode != ST_NO_ERROR)
		{
			FILTER_ERROR(("[freefilter] HI_UNF_DMX_DestroyChannel Failed!  ErrCode 0x%08x\n",ErrCode));         
		}
#ifdef	SECTION_DEBUG
		FILTER_ERROR(("[freefilter]: FilterNo [%d] Pid [%d] NoOfFreeFilter [%d]\n",
		SlotIndex, FilterDescript[SlotIndex].pidDataPid,
		NoOfFreeSlots));
#endif
	}
    else
    {
#ifdef	SECTION_DEBUG
        FILTER_ERROR(("[freefilter]: FilterNo [%d] Pid [%d] NoOfFreeFilter [%d],Share Slot,Not Deallocate Slot\n",
                  SlotIndex, FilterDescript[SlotIndex].pidDataPid,
                  NoOfFreeSlots));
#endif    	
    }
    //FilterDescript[SlotIndex].status = SECTION_SLOT_FREE;
    //FilterDescript[SlotIndex].pidDataPid = -1; /*  IsSlotShared */
    //FilterDescript[SlotIndex].Callback = NULL;
    //FilterDescript[SlotIndex].BufferHandle = 0;
    //FilterDescript[SlotIndex].SlotHandle = 0;
    //FilterDescript[SlotIndex].IsAVITslot = FALSE;

	drv_FFILTResetFilterDiscDescript(SlotIndex);
	
    DRV_OSSemSignal(CancelWait);    
    DRV_OSSemSignal(SectionTableAccess);
    return FALSE;
}


S32 DRV_FILTReqFilterForNextSection(S32 FilterNo, S32 iNextSectionNo, S32 nNextTableID)
{
	ST_FILTER_STRUCT *pstSectionStruct;
	WORD2BYTE	*pstDataMask;
	DRV_ErrCode ErrCode;
	HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
	memset(&stFilterAttr,0,sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);

    /* Check for validity */
    if (FilterNo < 0 || FilterNo >= NO_OF_SIMULTANEOUS_FILTERS)
    {
        DRV_OSSemSignal(SectionTableAccess);
#ifdef	SECTION_DEBUG
 	 	FILTER_ERROR(( "[fornextsection]Invalid FilterNo [%d]\n", FilterNo));
#endif
        return SECTION_ERROR_INVALID_SLOT_ID;
    }

	pstSectionStruct = &(FilterDescript[FilterNo]);

	/* if the filter to be reenabled is already free then return with error */
	if ( (FilterDescript [ FilterNo ].status == SECTION_SLOT_FREE ) || (FilterDescript [ FilterNo ].status == SECTION_SLOT_IN_USE))
	{
	       DRV_OSSemSignal(SectionTableAccess);
		return ( SECTION_ERROR_CANT_REENABLE_FREE_FILTER );
	}
 	ErrCode = HI_UNF_DMX_DetachFilter(pstSectionStruct->FilterHandle,pstSectionStruct->SlotHandle);
	if(ErrCode != ST_NO_ERROR)
	{
		FILTER_ERROR(("[fornextsection] HI_UNF_DMX_DetachFilter Failed!  ErrCode 0x%08x\n",ErrCode));
	}

	/* setup Table ID byte */
	pstDataMask = ( WORD2BYTE * )( &(pstSectionStruct ->FilterData [ 0 ]));
	pstDataMask ->byte.ucByte0	= ( unsigned char ) nNextTableID;

	/*Adjust the contents of section_filter_t*/
	pstDataMask = ( WORD2BYTE * ) (&(pstSectionStruct->FilterData [ 4 ]));
	pstDataMask ->byte.ucByte0	= ( unsigned char ) iNextSectionNo;

	/* reinitialise this section structure */
	pstSectionStruct->status 		= SECTION_SLOT_IN_USE;
	pstSectionStruct->nTimeOutCount = pstSectionStruct->nOrgTimeOutCount;
	pstSectionStruct->ucTableId		= nNextTableID;
	pstSectionStruct->sSectionNo	= iNextSectionNo;
	ErrCode = HI_UNF_DMX_GetFilterAttr(pstSectionStruct->FilterHandle,&stFilterAttr);
	if(ErrCode != ST_NO_ERROR)
	{
		FILTER_ERROR(("[fornextsection] HI_UNF_DMX_GetFilterAttr Failed!  ErrCode 0x%08x\n",ErrCode));
	}
    /* set filter attr */
    memset(&g_mask,0,sizeof(FILTER_DEPTH_SIZE));
    stFilterAttr.u32FilterDepth = FILTER_DEPTH_SIZE;
	memcpy(stFilterAttr.au8Match,pstSectionStruct->FilterData,FILTER_DEPTH_SIZE);
	ErrCode |= HI_UNF_DMX_SetFilterAttr(pstSectionStruct->FilterHandle,&stFilterAttr);
	if(ErrCode != ST_NO_ERROR)
	{
		FILTER_ERROR(("[fornextsection] HI_UNF_DMX_SetFilterAttr Failed!  ErrCode 0x%08x\n",ErrCode));
	}
	ErrCode = HI_UNF_DMX_AttachFilter(pstSectionStruct->FilterHandle,pstSectionStruct->SlotHandle);
	if(ErrCode != ST_NO_ERROR)
	{
		FILTER_ERROR(("[fornextsection] HI_UNF_DMX_AttachFilter Failed!  ErrCode 0x%08x\n",ErrCode));
	}
	DRV_OSSemSignal(SectionTableAccess);
	return FilterNo;
}
#if 0
BOOL_YS DRV_FLT_SetFilter(U32_YS Channel, U32_YS Buffer, S32_YS FilterNo, U8_YS *Data, U8_YS *Mask)
{
	return 0;
}


BOOL_YS DRV_FLT_FreeFilter(U32_YS Channel, U32_YS Buffer, S32_YS FilterNo)
{
	return 0;
}

BOOL_YS DRV_FLT_AllocateFilter(U32_YS Channel, U32_YS Buffer, OUT_YS S32_YS *FilterNo)
{
	return 0;
}


typedef enum
{
  DRV_DMX_CHANNEL_DISABLE = 0,
  DRV_DMX_CHANNEL_ENABLE,
  DRV_DMX_CHANNEL_RESET
}DRV_DMX_ChannelAction_e;

BOOL_YS DRV_DMX_ControlChannel(U32_YS Channel, U32_YS Buffer, U32_YS Action)
{
	return 0;
}

BOOL_YS DRV_DMX_SetChannelPid(U32_YS Channel, U32_YS Buffer, U16_YS Pid)
{
	return 0;
}

BOOL_YS DRV_DMX_FreeSectionChannel(U32_YS Channel, U32_YS Buffer)
{
	return 0;
}

BOOL_YS DRV_DMX_AllocateSectionChannel(U32_YS BufferSize, OUT_YS U32_YS *Channel, OUT_YS U32_YS *Buffer)
{
	return 0;
}

#endif

S32	DRV_FILTReenableFilter ()
{
	return 0;
}

#if 0 //def _AVIT_DBC_
S32  pti_allocate_dynamic_slot ( S32 *slotnum , S32 *buff,  S32  nBufferSize)
{
	DRV_ErrCode	ErrCode;
	HI_HANDLE   NewChannel = 0;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
    
    /* lock the section storage table  */
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
	if (ErrCode != HI_SUCCESS)
	{
        STTBX_Print(("pti_allocate_dynamic_slot HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
	stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
	stChnAttr.u32BufSize = 64*1024;
	ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&NewChannel);
	if (ErrCode != HI_SUCCESS)
	{
        STTBX_Print(("pti_allocate_dynamic_slot HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}

	*slotnum = NewChannel;
	*buff = NewChannel;

    DRV_OSSemSignal(SectionTableAccess);
    return ErrCode;
}

S32 pti_deallocate_dynamic_slot(HI_HANDLE SlotHandle, STPTI_Buffer_t BufferHandle)
{	
    S32 ShareSlot;
    DRV_ErrCode ErrCode = ST_NO_ERROR;
	
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    DRV_OS_SemWait(CancelWait,DRVOS_WAIT_INFINITY,0);    	

	ErrCode = HI_UNF_DMX_CloseChannel(SlotHandle);
	ErrCode |= HI_UNF_DMX_DestroyChannel(SlotHandle);
	if (ErrCode != ST_NO_ERROR)
	{
		FILTER_ERROR(("pti_deallocate_dynamic_slot HI_UNF_DMX_DestroyChannel Failed!  ErrCode 0x%08x\n",ErrCode));         
	}
    DRV_OSSemSignal(CancelWait);    
    DRV_OSSemSignal(SectionTableAccess);
    return FALSE;
}

S32 pti_allocate_dynamic_filter(S32 *filter)
{
	DRV_ErrCode	ErrCode;
	HI_HANDLE   NewFilter = 0;
	BOOL		SlotShared = FALSE;
	HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
	memset(&stFilterAttr,0,sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    /* lock the section storage table  */
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    
    /* set filter attr */
    memset(&g_mask,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_match,0,sizeof(FILTER_DEPTH_SIZE));
    memset(&g_Negate,0,sizeof(FILTER_DEPTH_SIZE));
    stFilterAttr.u32FilterDepth = FILTER_DEPTH_SIZE;
	drv_FilterSwapMask(FilterDescript[SlotId].FilterMask,stFilterAttr.au8Mask);
	memcpy(stFilterAttr.au8Match,FilterDescript[SlotId].FilterData,FILTER_DEPTH_SIZE);
	memcpy(stFilterAttr.au8Negate,&g_Negate,FILTER_DEPTH_SIZE);
	ErrCode = HI_UNF_DMX_CreateFilter(0,&stFilterAttr,&NewFilter);
	if (ErrCode != HI_SUCCESS)
	{
        STTBX_Print(("DRV_FILTReqFilterForStandard HI_UNF_DMX_CreateFilter ErrCode 0x%08x\n",ErrCode));
		return (SECTION_ERROR_ST_ERROR);
	}
	ErrCode = HI_UNF_DMX_AttachFilter(NewFilter,NewChannel);
	if (HI_SUCCESS != ErrCode)
	{
        STTBX_Print(("DRV_FILTReqFilterForStandard HI_UNF_DMX_AttachFilter ErrCode 0x%08x\n",ErrCode));
        HI_UNF_DMX_DestroyFilter(NewFilter);
		return (SECTION_ERROR_ST_ERROR);
	}
    DRV_OSSemSignal(SectionTableAccess);
    return ErrCode;

}
void pti_deallocate_dynamic_filter(S32 filter)
#if 1
{
	return 0;
}
#else
{
	
    DRV_OS_SemWait(SectionTableAccess,DRVOS_WAIT_INFINITY,0);
    DRV_OS_SemWait(CancelWait,DRVOS_WAIT_INFINITY,0);   	

    STPTI_FilterDeallocate(FilterDescript[filter].FilterHandle);
    DRV_OS_TaskLock();
    FilterDescript[filter].FilterHandle = -1;
    FilterDescript[filter].status   = SECTION_SLOT_FREE;
    FilterDescript[filter].pidDataPid = -1;    
    FilterDescript[filter].IsAVITslot = false;    
    ++NoOfFreeSlots;
    DRV_OS_TaskLock();
    DRV_OSSemSignal(CancelWait);        
    DRV_OSSemSignal(SectionTableAccess);
    
}
#endif

#endif


