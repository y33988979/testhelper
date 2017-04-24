/*****************************************************************************
*
* Description: 
*    inject TS
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2010-06-28    Jyshan       创建
*
*****************************************************************************/
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include <string.h>

#include "yspti.h"
#include "drv_snd.h"
#include "drv_avplay.h"
#include "drv_os.h"
#include "ysav.h"
#include "drv_os.h"
#include "ysvfs.h"
#include "drv_basic.h"

#define TS_INJECTION_BUF_SIZE       (5 * 1024 * 1024)
#define TS_BUF_THRESHOLD            (3)
#define TS_INJECTION_SIZE			(188*50)
#define TS_RECEIVE_SIZE			    (4*1024)
typedef struct DRV_TsInjectBuffer_s
{
	U32                  TsBufferSize;
	U32					 TsBufferBase;
	U32					 TsBufferProducer;
	U32					 TsBufferConsumer;
	BOOL				 TsBufferSwap;/* buffer翻转,即write指针转了一圈又追上了read指针*/
	DRV_MutexHandle 	 *Mutex_lock;
} DRV_TsInjectBuffer;

DRVOS_Task_t           *mTsReceiveTask;
DRVOS_Task_t           *mTsInjectTask;
static HI_HANDLE       mTsInjectBufferHandle;
static HI_BOOL         mTsReceiveTaskStatus = HI_FALSE;
static HI_BOOL         mTsInjectTaskStatus  = HI_FALSE;
static HI_BOOL         mTsReceiveTaskExit   = HI_FALSE;
static HI_BOOL         mTsInjectTaskExit    = HI_FALSE;
HI_HANDLE mTsInjectAvplay = 0;
DRV_TsInjectBuffer TsInjectBuffer;
void  *TsFileHandle;
void drv_TsInjectTask(void *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            ErrCode;

    while (!mTsInjectTaskStatus)
    {
    	ErrCode = drv_TsInjectDatafromBuf();
    	if(ErrCode == DRV_NO_ERROR)
    	{
    		continue;
    	}
		DRV_OS_TaskDelay_MS(500);
    }
    mTsInjectTaskExit = TRUE;
    return;
}

void drv_TsReceiveTask(void *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            ErrCode;
    HI_U32            FreeLen;
    HI_U8             *TemBuffer;
	TemBuffer = DRV_OSMalloc(SystemPartition,TS_RECEIVE_SIZE);

    while (!mTsReceiveTaskStatus)
    {
    	drv_TsInjectBufGetFreeLength(&FreeLen);
    	if(FreeLen <= TS_INJECTION_BUF_SIZE/TS_BUF_THRESHOLD)
    	{
    		printf("drv_TsReceiveTask FreeLen is %d,InjectBuffer full,so slow!\n",FreeLen);
    		DRV_OS_TaskDelay_MS(500);
    		continue;
    	}
		DRV_VfsReadFile(TsFileHandle,TemBuffer,TS_RECEIVE_SIZE);
		//DRV_OS_TaskDelay_MS(100);
		drv_TsInjectCopyDatatoBuf(TemBuffer,TS_RECEIVE_SIZE);
	}
	ErrCode = HI_UNF_DMX_ResetTSBuffer(mTsInjectBufferHandle);
	if (ErrCode != HI_SUCCESS )
	{
		printf("call drv_TsReceiveTask HI_UNF_DMX_ResetTSBuffer failed ErrCode 0x%08x \n", ErrCode);
	}

	DRV_OSFree(SystemPartition,TemBuffer);
	mTsReceiveTaskExit = TRUE;
    return;
}

/* Init injection buffer */
DRV_ErrCode DRV_TsInjectSetup(void)
{
	DRV_ErrCode ErrCode;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;
	ErrCode = DRV_AVAPI_AVstop(0);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_TsInjectSetup DRV_AVAPI_AVstop ErrCode 0x%08x \n", ErrCode);
    }
	ErrCode = DRV_AvplayerTerm(mAvPlayHandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_TsInjectSetup DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call DRV_TsInjectSetup HI_UNF_DMX_DetachTSPort ErrCode 0x%08x \n", ErrCode);
        HI_UNF_DMX_DeInit();
        return ErrCode;
    }
    ErrCode |= HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE,PVR_DMX_PORT_ID_IP);
    if (HI_SUCCESS != ErrCode)
    {
        printf("call DRV_TsInjectSetup HI_UNF_DMX_AttachTSPort ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DMX_CreateTSBuffer(PVR_DMX_PORT_ID_IP, 0x200000, &mTsInjectBufferHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call DRV_TsInjectSetup HI_UNF_DMX_CreateTSBuffer ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mTsInjectAvplay);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call DRV_TsInjectSetup DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    TsInjectBuffer.TsBufferBase = DRV_OSMalloc(SystemPartition,TS_INJECTION_BUF_SIZE);
    TsInjectBuffer.TsBufferSize = TS_INJECTION_BUF_SIZE;
    TsInjectBuffer.TsBufferSwap = FALSE;
    TsInjectBuffer.TsBufferConsumer = TsInjectBuffer.TsBufferProducer = TsInjectBuffer.TsBufferBase;
	DRV_OS_MutexCreate(NULL, &(TsInjectBuffer.Mutex_lock));

    DRV_OS_MutexLock(TsInjectBuffer.Mutex_lock);
    HI_UNF_DMX_ResetTSBuffer(mTsInjectBufferHandle);
    DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);
    
	return DRV_NO_ERROR;
}

/*Deallocate injection buffer */
DRV_ErrCode DRV_TsInjectTerm(void)
{
	DRV_ErrCode ErrCode;
    ErrCode = HI_UNF_DMX_DestroyTSBuffer(mTsInjectBufferHandle);
    ErrCode |= DRV_AvplayerTerm(mTsInjectAvplay,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_TsInjectTerm DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call DRV_TsInjectTerm HI_UNF_DMX_DetachTSPort ErrCode 0x%08x \n", ErrCode);
        HI_UNF_DMX_DeInit();
        return ErrCode;
    }
    ErrCode |= HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE,PVR_DMX_PORT_ID_DVB);
    if (HI_SUCCESS != ErrCode)
    {
        printf("call DRV_TsInjectTerm HI_UNF_DMX_AttachTSPort ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
   
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_TsInjectTerm DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }


    return DRV_NO_ERROR;
}
/* Get ethernet injection buffer available size */
DRV_ErrCode drv_TsInjectBufGetUsedLength(U32* len_p)
{
	DRV_ErrCode ErrCode;
	U32 FreeSize;
	drv_TsInjectBufGetFreeLength(&FreeSize);
	*len_p = TsInjectBuffer.TsBufferSize - FreeSize;
    return DRV_NO_ERROR;
}

/* Get ethernet injection buffer free size */
DRV_ErrCode drv_TsInjectBufGetFreeLength(U32* len_p)
{
	S32	datsize = 0;
	DRV_OS_MutexLock(TsInjectBuffer.Mutex_lock);
	datsize = TsInjectBuffer.TsBufferConsumer - TsInjectBuffer.TsBufferProducer;
	if( datsize < 0 )
	{
		datsize += TsInjectBuffer.TsBufferSize;
	}
    else if( datsize == 0 )
    {
    	if(TsInjectBuffer.TsBufferSwap == FALSE)
    		datsize = TsInjectBuffer.TsBufferSize;
    }
	DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);
    *len_p = datsize;
    //printf("------drv_TsInjectBufGetFreeLength Free Len is %d\n",datsize);
	return DRV_NO_ERROR;
}

/* copy payload to ethernet injection buffer */
DRV_ErrCode drv_TsInjectCopyDatatoBuf(U8* src_p, U32 len)
{
	S32	datsize = 0;
	S32	lenTemp = 0;
	DRV_OS_MutexLock(TsInjectBuffer.Mutex_lock);
#if 1
	datsize = TsInjectBuffer.TsBufferProducer - TsInjectBuffer.TsBufferConsumer;
	if( datsize < 0 )
		datsize += TsInjectBuffer.TsBufferSize;

	if(	(datsize >= TsInjectBuffer.TsBufferSize * 2 / 3) ||
		(TsInjectBuffer.TsBufferSize <= datsize + len) )
	{
		STTBX_Print(("drv_TsInjectCopyDatatoBuf Inject buffer full.datsize = 0x%x,BufferProducer = 0x%x, TsInjectBuffer.BufferConsumer = 0x%x,BufferSize = %d\n",datsize, 
			TsInjectBuffer.TsBufferProducer, TsInjectBuffer.TsBufferConsumer,TsInjectBuffer.TsBufferSize));
		DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);
		return 1;
	}
#endif	
	if( TsInjectBuffer.TsBufferProducer + len <= TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize )
	{
		memcpy((U8 *)TsInjectBuffer.TsBufferProducer, src_p, len);
		TsInjectBuffer.TsBufferProducer += len;
		if(TsInjectBuffer.TsBufferProducer == TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize)
		{
			TsInjectBuffer.TsBufferProducer = TsInjectBuffer.TsBufferBase;
		}
    }
	else
	{
		lenTemp = TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize - TsInjectBuffer.TsBufferProducer;
		memcpy((U8 *)TsInjectBuffer.TsBufferProducer, src_p, lenTemp);
		memcpy((U8 *)TsInjectBuffer.TsBufferBase, src_p + lenTemp, len - lenTemp );
		TsInjectBuffer.TsBufferProducer = TsInjectBuffer.TsBufferBase + len - lenTemp;
	}
	if(TsInjectBuffer.TsBufferProducer == TsInjectBuffer.TsBufferConsumer)
		TsInjectBuffer.TsBufferSwap = TRUE;
	else	
		TsInjectBuffer.TsBufferSwap = FALSE;
	DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);
	return DRV_NO_ERROR;
}

/* Inject data from Buffer to SWTS */
DRV_ErrCode drv_TsInjectDatafromBuf()
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_S32            ErrCode;
    HI_U32            DataLen;
	drv_TsInjectBufGetUsedLength(&DataLen);
	if(DataLen < 188*50)
	{
		printf("call drv_TsInjectDatafromBuf drv_TsInjectBufGetUsedLength DataLen.%d,InjectBuffer empty\n",DataLen);
		return 1;
	}
	
	DRV_OS_MutexLock(TsInjectBuffer.Mutex_lock);
	ErrCode = HI_UNF_DMX_GetTSBuffer(mTsInjectBufferHandle, 188*50, &StreamBuf, 1000);
	if (ErrCode != HI_SUCCESS )
	{
		DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);
		printf("call drv_TsInjectDatafromBuf HI_UNF_DMX_GetTSBuffer ErrCode.0x%08x TSBuffer full\n",ErrCode);
		return 2;
	}
	if(TsInjectBuffer.TsBufferConsumer + 188*50 <= TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize)
	{
		memcpy(StreamBuf.pu8Data,TsInjectBuffer.TsBufferConsumer,188*50);
		if(TsInjectBuffer.TsBufferConsumer + 188*50 == TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize)
		{
			TsInjectBuffer.TsBufferConsumer = TsInjectBuffer.TsBufferBase;
		}
		else
		{
			TsInjectBuffer.TsBufferConsumer = TsInjectBuffer.TsBufferConsumer + 188*50;
		}
	}	
	else
	{	
		DataLen = TsInjectBuffer.TsBufferBase + TsInjectBuffer.TsBufferSize - TsInjectBuffer.TsBufferConsumer;
		memcpy(StreamBuf.pu8Data,TsInjectBuffer.TsBufferConsumer,DataLen);
		memcpy(StreamBuf.pu8Data+DataLen,TsInjectBuffer.TsBufferBase,188*50 - DataLen);
		TsInjectBuffer.TsBufferConsumer = TsInjectBuffer.TsBufferBase + 188*50 - DataLen;
	}
	ErrCode = HI_UNF_DMX_PutTSBuffer(mTsInjectBufferHandle, 188*50);
	if (ErrCode != HI_SUCCESS )
	{
		printf("call drv_TsInjectDatafromBuf HI_UNF_DMX_PutTSBuffer ErrCode 0x%08x \n", ErrCode);
	}
	DRV_OS_MutexRelease(TsInjectBuffer.Mutex_lock);


	return ErrCode;
}

DRV_ErrCode DRV_TsInjectStart(DRV_InjectSrc InjectSrc)
{
    DRV_ErrCode ErrCode;
	U32  TsFileSize;
	U8   *TsFileBuffer;
    char filename[128];
	DRVOS_Task_t  *TsInjectTask;
	switch(InjectSrc)
	{
		case DRV_INJECT_FROM_BUFFER:
			DRV_AVAPI_AVstart_HD(CURRENT_DECODE_CHANNEL,VIDEO_INJECT_PTI_INSTANCE,\
			    0x11,0x12,8191,0xe000,0xe000,DRV_AUDIO_TYPE_MPEG2,DRV_VIDEO_TYPE_MP2V);
			DRV_AV_UnMuteAudio();
			break;
		case DRV_INJECT_FROM_FILE:
		    //strcpy(filename,DRV_PathChange("c:/pvt.ts"));
			TsFileHandle=DRV_VfsOpenFile((char *)"c:/test.ts","rb");
			if (TsFileHandle==NULL)
			{
				STTBX_Print(("DRV_VfsOpenFile:**ERROR** !!! File doesn't exists !!!\n"));
				return(ST_ERROR_BAD_PARAMETER);
			}
		    mTsReceiveTaskStatus = HI_FALSE;
		    mTsReceiveTaskExit   = HI_FALSE;
			mTsReceiveTask = DRV_OS_TaskCreate("drv_TsReceiveTask",drv_TsReceiveTask,0,8*1024,HI_NULL,HI_NULL);
			DRV_AVAPI_AVstart_HD(CURRENT_DECODE_CHANNEL,VIDEO_INJECT_PTI_INSTANCE,\
			    0x121,0x124,8191,0xe000,0xe000,DRV_AUDIO_TYPE_MPEG2,DRV_VIDEO_TYPE_MP2V);
			DRV_AV_UnMuteAudio();
		    mTsInjectTaskStatus = HI_FALSE;
		    mTsInjectTaskExit   = HI_FALSE;
			mTsInjectTask = DRV_OS_TaskCreate("drv_TsInjectTask",drv_TsInjectTask,0,8*1024,HI_NULL,HI_NULL);
			break;
		default:
			return DRV_ERROR_BAD_PARAMETER;
			break;
	}
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_TsInjectStop(DRV_InjectSrc InjectSrc)
{
    DRV_ErrCode ErrCode;
	DRV_AVAPI_AVstop_HD(CURRENT_DECODE_CHANNEL,VIDEO_INJECT_PTI_INSTANCE);
	mTsReceiveTaskStatus = HI_TRUE;
	mTsInjectTaskStatus  = HI_TRUE;
	while(!mTsReceiveTaskExit)
	{
		DRV_OS_TaskDelay_MS(100);
	}
	while(!mTsInjectTaskExit)
	{
		DRV_OS_TaskDelay_MS(100);
	}
	printf("------task exit ok!\n");
	while(1)
	{
		ErrCode = DRV_OS_TaskWait(mTsReceiveTask, DRVOS_WAIT_FINITY, 500); 
		if(ErrCode == 0)
			break;
	    DRV_OS_TaskKill(mTsReceiveTask, 0, 0);
		DRV_OS_TaskDelay_MS(100);
	}
    DRV_OSTaskDelete(mTsReceiveTask);
	while(1)
	{
		ErrCode = DRV_OS_TaskWait(mTsInjectTask, DRVOS_WAIT_FINITY, 500); 
		if(ErrCode == 0)
			break;
	    DRV_OS_TaskKill(mTsInjectTask, 0, 0);
		DRV_OS_TaskDelay_MS(100);
	}
    DRV_OSTaskDelete(mTsInjectTask);
	return DRV_NO_ERROR;
}


