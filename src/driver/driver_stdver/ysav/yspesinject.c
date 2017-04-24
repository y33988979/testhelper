#include <string.h>
#include <stdio.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "drv_avplay.h"
#include "drv_snd.h"
#include "ysav.h"
#include "ysvfs.h"
#include "drv_os.h"
#include "hi_unf_pes.h"
#include "HA.AUDIO.G711.codec.h"
#include "HA.AUDIO.MP3.decode.h"
#include "HA.AUDIO.MP2.decode.h"
#include "HA.AUDIO.AAC.decode.h"
#include "HA.AUDIO.DRA.decode.h"
#include "HA.AUDIO.PCM.decode.h"
#include "HA.AUDIO.WMA9STD.decode.h"
#include "HA.AUDIO.AMRNB.codec.h"
#include "HA.AUDIO.AMRWB.codec.h"
#include "HA.AUDIO.TRUEHDPASSTHROUGH.decode.h"
#include "HA.AUDIO.DTSHD.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.FFMPEG_DECODE.decode.h"
#include "HA.AUDIO.AAC.encode.h"
typedef struct DRV_VideoPush_s
{
	U32 			 	 device_id;				/* index of vdec						*/
	STVID_StartParams_t  VID_StartParams;

	U32                  BufferDevice;      	/* Buffer device						*/

	U32                  BufferSize;            /* Buffer size					        */
	U32					 Buffer;				/* Video pes Buffer 			        */
	U32					 BufferProducer;
	U32					 BufferConsumer;
	BOOL				 BufferSwap;/* buffer翻转,即write指针转了一圈又追上了read指针*/
	U32					 BitBufferSize;
	DRVOS_Sem_t			 *sem_lock;

} DRV_VideoPush;

typedef struct DRV_AudioPush_e
{
	S32 			  device_id;			 /* index of adec						 */
	STAUD_StartParams_t   AUD_StartParams;       /* Audio start parameters               */
	U32                   BufferSize;       	 /* Audio pes buffer size                */
	U32                   Buffer;           	 /* Audio pes buffer                     */
	U32                   BufferProducer;   	 /* Producer pointer in the audio buffer */
	U32                   BufferConsumer;   	 /* Consumer pointer in the audio buffer */
	BOOL                  BufferSwap;
	U32                   BitBufferSize;    	 /* Audio bit buffer empty size          */

	U32                   BufferDevice;     	 /* Audio pes buffer physical address    */
	
	DRVOS_Sem_t			  *sem_lock;

}DRV_AudioPush;

static DRV_VideoPush mVideoPush[DRV_VIDEO_TYPE_MAX];
static DRV_AudioPush mAudioPush;

static HI_HANDLE    mPesInjectVideoplayer;
static HI_HANDLE    mPesInjectAudioplayer;

static HI_HANDLE   m_PES_VIDEO_AVhandle = 0;
static HI_HANDLE   m_PES_AUDIO_AVhandle = 0;
 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_PesInjectSetup
  Description     :从直播切换到pes注入，关闭直播和去初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_PesInjectSetup(void)/*从直播切换到pes注入*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	ErrCode = DRV_AVAPI_AVstop(0);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_PesInjectSetup DRV_AVAPI_AVstop ErrCode 0x%08x \n", ErrCode);
    }
    
	ErrCode = DRV_AvplayerTerm(mAvPlayHandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_PesInjectSetup DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    
    return DRV_NO_ERROR;
}
   
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_PesInjectTerm
   Description     :从pes注入切换到直播，打开音视频通道等
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_PesInjectTerm(void)/*从pes注入切换到直播*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_TS,HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID,&mAvPlayHandle);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_PesInjectTerm DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
        return ErrCode;
    }
    
    return DRV_NO_ERROR;
}
    

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectStart
  Description     :开启PES播放器
  Input           : VidStartParams无效参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectStart(STVID_StartParams_t VidStartParams,DRV_VideoType VidStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_SYNC_ATTR_S          AvSyncAttr;
	HI_UNF_VCODEC_TYPE_E        enType;    
    HI_UNF_PES_OPEN_S           stOpenParam;
    HI_HANDLE                   PEShandle = 0;
    HI_HANDLE                   PESAVhandle = 0;
    HI_U32                      u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_UNKNOW;
    HI_U32                      u32PES_AudioType= HI_UNF_PES_STREAM_TYPE_AUDIO_UNKNOW;

	
	if(mVideoPush[VidStreamContent].sem_lock == NULL)
	{
		mVideoPush[VidStreamContent].sem_lock = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
	}	
    if(mPesInjectVideoplayer == 0)
    {	
		ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_ES,HI_UNF_AVPLAY_MEDIA_CHAN_VID,&PESAVhandle);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_VideoPesInjectStart DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
    }
	switch(VidStreamContent)
	{
		case DRV_VIDEO_TYPE_MP2V:
			enType = HI_UNF_VCODEC_TYPE_MPEG2;
			u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_MPEG2;
			break;
		case DRV_VIDEO_TYPE_MPEG4P2:
			enType = HI_UNF_VCODEC_TYPE_MPEG4;
            u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_MPEG4;
			break;
		case DRV_VIDEO_TYPE_H264:
			enType = HI_UNF_VCODEC_TYPE_H264;
            u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_H264;
			break;	
		//case DRV_VIDEO_TYPE_AVS:
			//enType = HI_UNF_VCODEC_TYPE_AVS;
            //u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_AVS;
			//break;
		default:
			printf("Stream type is not supported.\n");
			return -1;
	}
	ErrCode = HIADP_AVPlay_SetVdecAttr(PESAVhandle,enType,HI_UNF_VCODEC_MODE_NORMAL); 	
	if (ErrCode != DRV_NO_ERROR)
	{
        STTBX_Print(("DRV_VideoPesInjectStart HIADP_AVPlay_SetVdecAttr ErrCode 0x%08x\n",ErrCode));
		return ErrCode;
	}

	//if (m_VidNewEvtCallBackInit[m_curVidSDecodeChannel])
	//{
	//	ErrCode = HI_UNF_AVPLAY_UnRegisterEvent(mPesInjectVideoplayer,HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
	//	m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = FALSE;
	//}
    //ErrCode = HI_UNF_AVPLAY_RegisterEvent(mPesInjectVideoplayer, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, DRV_NewVideoFrameEvent);
    //if(ErrCode != HI_SUCCESS)
    //{
    //    STTBX_Print(("DRV_AV_StartVideo : HI_UNF_AVPLAY_RegisterEvent ErrCode 0x%08x\n",ErrCode));
    //    return ErrCode;
    //}
	//m_VidNewEvtCallBackInit[m_curVidSDecodeChannel] = TRUE;
	//DBG_VidEvtCounter=0;
	//m_VidNewEvtCounter=0;
    
    ErrCode = HI_UNF_AVPLAY_GetAttr(PESAVhandle, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    AvSyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    ErrCode |= HI_UNF_AVPLAY_SetAttr(PESAVhandle, HI_UNF_AVPLAY_ATTR_ID_SYNC,&AvSyncAttr);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DRV_VideoPesInjectStart HI_UNF_AVPLAY_SetAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    	    
    ErrCode = HI_UNF_AVPLAY_Start(PESAVhandle, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_VideoPesInjectStart HI_UNF_AVPLAY_Start failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    STTBX_Print(("DRV_VideoPesInjectStart(VidStreamContent=%d) OK!\n",VidStreamContent));

        /* Open PES handle */
    stOpenParam.hAVPlay           = PESAVhandle;
    stOpenParam.u32VdecType       = u32PESVideoType;
    stOpenParam.u32AudioType      = u32PES_AudioType;
    stOpenParam.u32PESBufferSize  = 2 * 1024 * 1024; // Allocate 2MBytes
    ErrCode = HI_UNF_PES_Open(&stOpenParam, &PEShandle);
    printf("PEShandle:0x%x\n", PEShandle);

    m_PES_VIDEO_AVhandle = PESAVhandle;
    mPesInjectVideoplayer = PEShandle;
    
	return DRV_NO_ERROR;
}
     

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectStop
  Description     :停止PES播放器
  Input           : DisableLastFrame，VidStreamContent无效参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectStop(DRV_VideoType VidStreamContent, BOOL DisableLastFrame)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
    if(mPesInjectVideoplayer != 0)
    {	
		ErrCode = HI_UNF_PES_Close(mPesInjectVideoplayer);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_VideoPesInjectStop DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
        
            ErrCode = DRV_AvplayerTerm(m_PES_VIDEO_AVhandle,HI_UNF_AVPLAY_MEDIA_CHAN_VID);
            if (DRV_NO_ERROR != ErrCode)
            {
                printf("DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
                return ErrCode;
            }
    }
    
	mPesInjectVideoplayer = 0;
    
	return DRV_NO_ERROR;
}
      
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_VideoPesFlushBuffer
   Description     :pes buffer flush,should restart video
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_VideoPesFlushBuffer(DRV_VideoType VidStreamContent)/*pes buffer flush,should restart video*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Reset(mPesInjectVideoplayer, NULL);
	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_VideoPesFlushBuffer HI_UNF_AVPLAY_Reset Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}
DRV_ErrCode DRV_VideoFlushBitBuffer(DRV_VideoType VidStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Reset(mPesInjectVideoplayer, NULL);
	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_VideoFlushBitBuffer HI_UNF_AVPLAY_Reset Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectPause
  Description     :暂停AV播放，进入PAUSE状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectPause(DRV_VideoType VidStreamContent)
{			
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Pause(mPesInjectVideoplayer,NULL);
	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_VideoPesInjectPause HI_UNF_AVPLAY_Pause Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectResume
  Description     :恢复AV播放，进入PLAY状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectResume(DRV_VideoType VidStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Resume(mPesInjectVideoplayer,NULL);
	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
	if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_VideoPesInjectResume HI_UNF_AVPLAY_Resume Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	} 
    
	return DRV_NO_ERROR;
}
 

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectCopy
  Description     :COPY要播放的PES数据
  Input           : consumer数据buffer，SizeToCopy数据长度
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectCopy(U8 *consumer,U32 SizeToCopy,DRV_VideoType VidStreamContent)/*上层保证成功，下层如果保护限制则返回错误并不拷贝数据*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_STREAM_BUF_S   StreamBuf;
    
 	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_UNF_PES_GetBuffer(mPesInjectVideoplayer, SizeToCopy, &StreamBuf);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("call DRV_VideoPesInjectCopy HI_UNF_AVPLAY_GetBuf ErrCode.0x%08x\n",ErrCode);
    	DRV_OS_TaskDelay_MS(100);/* wait for buffer */
    	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
    	return -1;
    }
    
    memcpy(StreamBuf.pu8Data, consumer, SizeToCopy);

    ErrCode = HI_UNF_PES_PutBuffer(mPesInjectVideoplayer, SizeToCopy);
    if (ErrCode != DRV_NO_ERROR )
    {
        printf("call DRV_VideoPesInjectCopy HI_UNF_AVPLAY_PutBuf ErrCode.0x%08x\n",ErrCode);
        return -1;
    }
    DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
    
	return DRV_NO_ERROR;
}
  

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesGetPesBufferFreeSize
  Description     :获取缓冲区剩余的数据大小
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    
 	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_AVPLAY_GetStatusInfo(mPesInjectVideoplayer,&stStatusInfo);
    if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_VideoPesGetPesBufferFreeSize HI_UNF_AVPLAY_GetStatusInfo Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	}
    *FreeSize = stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize - stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize;
    DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesGetPesBufferSize
  Description     :获取缓冲区的数据大小
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesGetPesBufferSize(U32 *BufferSize,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    
 	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_AVPLAY_GetStatusInfo(mPesInjectVideoplayer,&stStatusInfo);
    if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_VideoPesGetPesBufferSize HI_UNF_AVPLAY_GetStatusInfo Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	}
    *BufferSize = stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize;
    DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
    
	return DRV_NO_ERROR;
}
   

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesGetPesBufferFreeSize
  Description     :获取缓冲区剩余的数据大小
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_VideoPesInjectGetFreeSize(U32 *FreeSize,DRV_VideoType VidStreamContent)/*pes && bit buffer free size*/
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    
 	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_AVPLAY_GetStatusInfo(mPesInjectVideoplayer,&stStatusInfo);
    if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_VideoPesInjectGetFreeSize HI_UNF_AVPLAY_GetStatusInfo Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	}
    *FreeSize = stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32BufSize - stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize;
    DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
    
	return DRV_NO_ERROR;
}
  

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesInjectBufferRate
  Description     :获取码流的码率
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode	DRV_VideoPesInjectBufferRate(U32 *Rate_p,DRV_VideoType VidStreamContent)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_STREAM_INFO_S stStreamInfo;

	DRV_OS_SemWait_MS(mVideoPush[VidStreamContent].sem_lock,DRVOS_WAIT_INFINITY,0);   
    ErrCode = HI_UNF_AVPLAY_GetStreamInfo(mPesInjectVideoplayer,&stStreamInfo);
    if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_VideoPesInjectBufferRate HI_UNF_AVPLAY_GetStreamInfo Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
	DRV_OSSemSignal(mVideoPush[VidStreamContent].sem_lock);
	*Rate_p = stStreamInfo.stVidStreamInfo.u32bps;
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesInjectStart
  Description     :开启PES播放器
  Input           : AudPCMInputParams,AudStartParams无效参数
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioPesInjectStart(STAUD_StartParams_t AudStartParams,STAUD_PCMInputParams_t AudPCMInputParams,DRV_AudioType AudStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_U32 enADecType;    

    HI_UNF_PES_OPEN_S           stOpenParam;
    HI_HANDLE                   PEShandle = 0;
    HI_HANDLE                   PESAVhandle;

    HI_U32                      u32PESVideoType = HI_UNF_PES_STREAM_TYPE_VIDEO_UNKNOW;
    HI_U32                      u32PES_AudioType= HI_UNF_PES_STREAM_TYPE_AUDIO_UNKNOW;

    if(mAudioPush.sem_lock == NULL)
    {
    	mAudioPush.sem_lock = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
    }	
	            
    if(mPesInjectAudioplayer == 0)
    {	
		ErrCode = DRV_AvplayerSetup(HI_UNF_AVPLAY_STREAM_TYPE_ES,HI_UNF_AVPLAY_MEDIA_CHAN_AUD,&PESAVhandle);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioPesInjectStart DRV_AvplayerSetup ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
    }
	switch(AudStreamContent)
	{
		case DRV_AUDIO_TYPE_AC3:
			enADecType = HA_AUDIO_ID_AC3PASSTHROUGH;
			u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_AC3;
			break;
		case DRV_AUDIO_TYPE_PCM:
			enADecType = HA_AUDIO_ID_PCM;
			u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_PCM;
			break;
		case DRV_AUDIO_TYPE_MPEG1:
		case DRV_AUDIO_TYPE_MPEG2:
			enADecType = HA_AUDIO_ID_MP2;
			u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_UNKNOW;
			break;
		case DRV_AUDIO_TYPE_MP3:
			enADecType = HA_AUDIO_ID_MP3;
			u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_MP3;
			break;
		case DRV_AUDIO_TYPE_MPEG_AAC:
			enADecType = HA_AUDIO_ID_AAC;
            u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_AAC;
			break;
		case DRV_AUDIO_TYPE_MPEG_HE_AAC:
			enADecType = HA_AUDIO_ID_AAC;
			u32PES_AudioType = HI_UNF_PES_STREAM_TYPE_AUDIO_AAC;
			break;
		default:
			return -1;
	}
    ErrCode = HIADP_AVPlay_SetAdecAttr(PESAVhandle,enADecType,HD_DEC_MODE_RAWPCM,1);
     if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_AudioPesInjectStart HIADP_AVPlay_SetAdecAttr failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    	    
    ErrCode = HI_UNF_AVPLAY_Start(PESAVhandle, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_AudioPesInjectStart HI_UNF_AVPLAY_Start failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    DRV_AV_UnMuteAudio();
    STTBX_Print(("DRV_AudioPesInjectStart(AudStreamContent=%d) OK!\n",AudStreamContent));

    /* Open PES handle */
    stOpenParam.hAVPlay           = PESAVhandle;
    stOpenParam.u32VdecType       = u32PESVideoType;
    stOpenParam.u32AudioType      = u32PES_AudioType;
    stOpenParam.u32PESBufferSize  = 2 * 1024 * 1024; // Allocate 2MBytes
    ErrCode = HI_UNF_PES_Open(&stOpenParam, &PEShandle);
    printf("PEShandle:0x%x\n", PEShandle);

    m_PES_AUDIO_AVhandle = PESAVhandle;
    mPesInjectAudioplayer = PEShandle;

    return DRV_NO_ERROR;
}
      

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesInjectStop
  Description     :停止PES播放器
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioPesInjectStop(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
    if(mPesInjectAudioplayer != 0)
    {	
		ErrCode = HI_UNF_PES_Close(mPesInjectAudioplayer);
	    if (DRV_NO_ERROR != ErrCode)
	    {
	        printf("DRV_AudioPesInjectStop DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
	        return ErrCode;
	    }
        
        ErrCode = DRV_AvplayerTerm(m_PES_AUDIO_AVhandle,HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        if (DRV_NO_ERROR != ErrCode)
        {
            printf(" DRV_AvplayerTerm ErrCode 0x%08x \n", ErrCode);
            return ErrCode;
        }
    }
    
	mPesInjectAudioplayer = 0;
    
	return DRV_NO_ERROR;
}
      
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AudioPesFlushBuffer
   Description     :pes buffer flush,should restart video
   Input           : 
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AudioPesFlushBuffer(void)/*pes buffer flush,should restart Audio*/
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Reset(mPesInjectAudioplayer, NULL);
	DRV_OSSemSignal(mAudioPush.sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AudioPesFlushBuffer HI_UNF_AVPLAY_Reset Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}
DRV_ErrCode DRV_AudioFlushBitBuffer(STAUD_StartParams_t AudStartParams,DRV_AudioType AudStreamContent)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Reset(mPesInjectAudioplayer, NULL);
	DRV_OSSemSignal(mAudioPush.sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AudioFlushBitBuffer HI_UNF_AVPLAY_Reset Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesInjectPause
  Description     :暂停AV播放，进入PAUSE状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioPesInjectPause(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Pause(mPesInjectAudioplayer,NULL);
	DRV_OSSemSignal(mAudioPush.sem_lock);
	if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AudioPesInjectPause HI_UNF_AVPLAY_Pause Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
    
	return DRV_NO_ERROR;
}
 

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesInjectResume
  Description     :恢复AV播放，进入PLAY状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioPesInjectResume(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
	ErrCode = HI_UNF_AVPLAY_Resume(mPesInjectAudioplayer,NULL);
	DRV_OSSemSignal(mAudioPush.sem_lock);
	if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_AudioPesInjectResume HI_UNF_AVPLAY_Resume Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	} 
    
	return DRV_NO_ERROR;
}
  
 
  /*****************************************************************************
   Date&Time       : 2012-12-07
   Author          : whyu modify
   Function Name   : DRV_AudioPesInjectCopy
   Description     :COPY要播放的PES数据
   Input           : consumer数据buffer，SizeToCopy数据长度
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_AudioPesInjectCopy(U8 *consumer,U32 SizeToCopy)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_STREAM_BUF_S   StreamBuf;

	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_UNF_PES_GetBuffer(mPesInjectAudioplayer, SizeToCopy, &StreamBuf);
    if (DRV_NO_ERROR != ErrCode)
    {
    	DRV_OS_TaskDelay_MS(100);/* wait for buffer */
    	DRV_OSSemSignal(mAudioPush.sem_lock);
         printf("call DRV_AudioPesInjectCopy HI_UNF_AVPLAY_GetBuf ErrCode.0x%08x\n",ErrCode);
   		return -1;
    }
    
    memcpy(StreamBuf.pu8Data, consumer, SizeToCopy);

    ErrCode = HI_UNF_PES_PutBuffer(mPesInjectAudioplayer, SizeToCopy);
    if (ErrCode != DRV_NO_ERROR )
    {
        printf("call DRV_AudioPesInjectCopy HI_UNF_AVPLAY_PutBuf failed.\n");
    }
	DRV_OSSemSignal(mAudioPush.sem_lock);
    
	return DRV_NO_ERROR;
}
    
  
   /*****************************************************************************
    Date&Time       : 2012-12-07
    Author          : whyu modify
    Function Name   : DRV_AudioPesInjectBufferRate
    Description     :获取码流的码率
    Input           : 
    Outut           :  
    Calls           : 
    Called by       :
    Global Variables: 
    Others          : 
  *****************************************************************************/
DRV_ErrCode	DRV_AudioPesInjectBufferRate(U32 *Rate_p)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_AVPLAY_STREAM_INFO_S stStreamInfo;

	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
    ErrCode = HI_UNF_AVPLAY_GetStreamInfo(mPesInjectAudioplayer,&stStreamInfo);
    if (ErrCode != DRV_NO_ERROR)
    {
        STTBX_Print(("DRV_AudioPesInjectBufferRate HI_UNF_AVPLAY_GetStreamInfo Failed! ErrCode=0x%x", ErrCode));
        return ErrCode;
    }
	DRV_OSSemSignal(mAudioPush.sem_lock);

	*Rate_p = stStreamInfo.stVidStreamInfo.u32bps;
    
	return DRV_NO_ERROR;
}
  

 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesGetPesBufferFreeSize，DRV_AudioPesInjectGetFreeSize
  Description     :获取缓冲区剩余的数据大小
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_AudioPesGetPesBufferFreeSize(U32 *FreeSize)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_AVPLAY_GetStatusInfo(mPesInjectAudioplayer,&stStatusInfo);
    if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_AudioPesGetPesBufferFreeSize HI_UNF_AVPLAY_GetStatusInfo Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	}
    *FreeSize = stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufSize - stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize;
	DRV_OSSemSignal(mAudioPush.sem_lock);
    
	return DRV_NO_ERROR;
}
DRV_ErrCode DRV_AudioPesInjectGetFreeSize(U32 *FreeSize)/*pes && bit buffer free size*/
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_STATUS_INFO_S stStatusInfo;
    
	DRV_OS_SemWait_MS(mAudioPush.sem_lock,DRVOS_WAIT_INFINITY,0);
 	ErrCode = HI_UNF_AVPLAY_GetStatusInfo(mPesInjectAudioplayer,&stStatusInfo);
    if (ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_AudioPesInjectGetFreeSize HI_UNF_AVPLAY_GetStatusInfo Failed! ErrCode=0x%x", ErrCode));
		return ErrCode;
	}
    *FreeSize = stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_AUD].u32BufSize - stStatusInfo.stBufStatus[HI_UNF_AVPLAY_BUF_ID_ES_VID].u32UsedSize;
	DRV_OSSemSignal(mAudioPush.sem_lock);
    
	return DRV_NO_ERROR;
}


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesSetSpeed，DRV_AudioPesSetSpeed
  Description     :无效接口
*****************************************************************************/
DRV_ErrCode DRV_VideoPesSetSpeed(int speed)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;

    ErrCode = HI_UNF_AVPLAY_Tplay(mPesInjectVideoplayer, speed);
	if(ErrCode!= DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_VideoPesSetSpeed(%x):**ErrCode** !!! Unable to set the video speed !!!\n",ErrCode));
		return ErrCode;
	}

	return DRV_NO_ERROR;
}
DRV_ErrCode DRV_AudioPesSetSpeed(int speed)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;

    ErrCode = HI_UNF_AVPLAY_Tplay(mPesInjectAudioplayer, speed);
	if(ErrCode!= DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_AudioPesSetSpeed(%x):**ErrCode** !!! Unable to set the video speed !!!\n",ErrCode));
		return ErrCode;
	}
    
	return DRV_NO_ERROR;
}


static DRV_InjectData PesVideoInjectData;
static U32 PesCurrtentOffset;
static U32 PesCurrtentTestFileSize;
static BOOL PesCurrtentTestTaskStatus = TRUE;
static BOOL PesTaskExit = FALSE;
static U8 * PesCurrtentTestFileBuffer;
static DRVOS_Task_t  *PesVideoInjectTestTask;


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_VideoPesTestInjectCopy，PesVideoInjectTestProcess
                            DRV_PesVideoTestInjectStart，DRV_PesVideoTestInjectStop
  Description     :视频PES注入测试接口
*****************************************************************************/
DRV_ErrCode DRV_VideoPesTestInjectCopy(DRV_InjectData *InjectData)
{
	DRV_ErrCode ErrCode;
	do{
    	ErrCode = DRV_VideoPesInjectCopy(InjectData->Buffer,InjectData->Lenth,DRV_VIDEO_TYPE_MP2V);
    }while(ErrCode);
    InjectData->BufferStatus = FALSE;
    return DRV_NO_ERROR;
}
void PesVideoInjectTestProcess(void* pvParam)
{
	U32 uDataLenth;
	U32 FreeSize;
	uDataLenth = 1024*16;
	PesCurrtentOffset = 0;
	memset(&PesVideoInjectData,0,sizeof(DRV_InjectData));
	while (PesCurrtentTestTaskStatus)
	{
		if(PesVideoInjectData.BufferStatus == FALSE)
		{
			PesVideoInjectData.BufferStatus = TRUE;
			PesVideoInjectData.Buffer = PesCurrtentTestFileBuffer+PesCurrtentOffset;
			PesVideoInjectData.Lenth = uDataLenth;
			DRV_VideoPesTestInjectCopy(&PesVideoInjectData);
			PesCurrtentOffset += uDataLenth;
			if(PesCurrtentOffset > PesCurrtentTestFileSize)
				PesCurrtentOffset = 0;

			//DRV_VideoPesInjectGetFreeSize(&FreeSize,DRV_VIDEO_TYPE_MP2V);
			//STTBX_Print(("------Get Buffer Free Size is %d!\n",FreeSize));
		}
		DRV_OS_TaskDelay_MS(10);
	}	
	PesTaskExit = TRUE;
}

DRV_ErrCode DRV_PesVideoTestInjectStart(DRV_InjectSrc InjectSrc)
{
    DRV_ErrCode ErrCode;
	void  *AudFileHandle;
    STVID_StartParams_t VidStartParams;

	switch(InjectSrc)
	{
		case DRV_INJECT_FROM_BUFFER:
			break;
		case DRV_INJECT_FROM_FILE:
			AudFileHandle=DRV_VfsOpenFile((char *)"c:/h264.pes","rb");
			if (AudFileHandle==NULL)
			{
				STTBX_Print(("OSPLUS_FOpen:**ErrCode** !!! File doesn't exists !!!\n"));
				return DRV_ERROR_BAD_PARAMETER;
			}
			PesCurrtentTestFileSize = DRV_VfsTellFile(AudFileHandle);
			DRV_VfsCloseFile(AudFileHandle);
			if(PesCurrtentTestFileSize > 0x1e00000)
			{
			    PesCurrtentTestFileSize = 0x1e00000;
			}
		    PesCurrtentTestFileBuffer = DRV_OSMalloc(SystemPartition,PesCurrtentTestFileSize);
		    if(PesCurrtentTestFileBuffer != NULL)
		    {
				AudFileHandle=DRV_VfsOpenFile((char *)"c:/h264.pes","rb");
		    
				DRV_VfsReadFile(AudFileHandle,PesCurrtentTestFileBuffer,PesCurrtentTestFileSize);
				DRV_VfsCloseFile(AudFileHandle);
			}	

			DRV_VideoPesInjectStart(VidStartParams,DRV_VIDEO_TYPE_MP2V);
			PesVideoInjectTestTask = DRV_OS_TaskCreate("PesVideoInjectTestProcess",
                               PesVideoInjectTestProcess,
                               9,
                               4096*40,
                               NULL,
                               0);
			break;
		default:
			return DRV_ERROR_BAD_PARAMETER;
			break;
	}
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PesVideoTestInjectStop(DRV_InjectSrc InjectSrc)
{
	DRV_ErrCode ErrCode;
	PesCurrtentTestTaskStatus = FALSE;
	while(!PesTaskExit)
	{
		DRV_OS_TaskDelay_MS(100);
	}
	printf("------task exit ok!\n");
	PesTaskExit = FALSE;
	while(1)
	{
		ErrCode = DRV_OS_TaskWait(PesVideoInjectTestTask, DRVOS_WAIT_FINITY, 500); 
		if(ErrCode == 0)
			break;
	    DRV_OS_TaskKill(PesVideoInjectTestTask, 0, 0);
		DRV_OS_TaskDelay_MS(100);
	}
    DRV_OSTaskDelete(PesVideoInjectTestTask);
    if(PesCurrtentTestFileBuffer != NULL)
    {
    	DRV_OSFree(SystemPartition,PesCurrtentTestFileBuffer);
    	PesCurrtentTestFileBuffer = NULL;
    }	
	DRV_VideoPesInjectStop(DRV_VIDEO_TYPE_MP2V,TRUE);
	return DRV_NO_ERROR;
}

static DRV_InjectData PesAudioInjectData;
static DRVOS_Task_t  *PesAudioInjectTestTask;


 /*****************************************************************************
  Date&Time       : 2012-12-07
  Author          : whyu modify
  Function Name   : DRV_AudioPesTestInjectCopy，PesAudioInjectTestProcess
                            DRV_PesAudioTestInjectStart，DRV_PesAudioTestInjectStop
  Description     :音频PES注入测试接口
*****************************************************************************/
DRV_ErrCode DRV_AudioPesTestInjectCopy(DRV_InjectData *InjectData)
{
	DRV_ErrCode ErrCode;
	do{
    	ErrCode = DRV_AudioPesInjectCopy(InjectData->Buffer,InjectData->Lenth);
    }while(ErrCode);
    InjectData->BufferStatus = FALSE;
    return DRV_NO_ERROR;
}
void PesAudioInjectTestProcess(void* pvParam)
{
	U32 uDataLenth;
	U32 FreeSize;
	uDataLenth = 1024*16;
	PesCurrtentOffset = 0;
	memset(&PesAudioInjectData,0,sizeof(DRV_InjectData));
	while (PesCurrtentTestTaskStatus)
	{
		if(PesAudioInjectData.BufferStatus == FALSE)
		{
			PesAudioInjectData.BufferStatus = TRUE;
			PesAudioInjectData.Buffer = PesCurrtentTestFileBuffer+PesCurrtentOffset;
			PesAudioInjectData.Lenth = uDataLenth;
			DRV_AudioPesTestInjectCopy(&PesAudioInjectData);
			PesCurrtentOffset += uDataLenth;
			if(PesCurrtentOffset > PesCurrtentTestFileSize)
				PesCurrtentOffset = 0;
			//DRV_AudioPesInjectGetFreeSize(&FreeSize);
			//STTBX_Print(("------Get Buffer Free Size is %d!\n",FreeSize));
		}
		DRV_OS_TaskDelay_MS(10);
	}	
	PesTaskExit = TRUE;
}

DRV_ErrCode DRV_PesAudioTestInjectStart(DRV_InjectSrc InjectSrc)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	void  *AudFileHandle;
	STAUD_StartParams_t AudStartParams;
	STAUD_PCMInputParams_t AudPCMInputParams;

	switch(InjectSrc)
	{
		case DRV_INJECT_FROM_BUFFER:
			break;
		case DRV_INJECT_FROM_FILE:
			AudFileHandle=DRV_VfsOpenFile((char *)"c:/h264.pes","rb");
			if (AudFileHandle==NULL)
			{
				STTBX_Print(("OSPLUS_FOpen:**ErrCode** !!! File doesn't exists !!!\n"));
				return(ST_ERROR_BAD_PARAMETER);
			}
			PesCurrtentTestFileSize = DRV_VfsTellFile(AudFileHandle);
			DRV_VfsCloseFile(AudFileHandle);
			if(PesCurrtentTestFileBuffer > 0x500000)
			{
                PesCurrtentTestFileBuffer = 0x500000;
			}
		    PesCurrtentTestFileBuffer = DRV_OSMalloc(SystemPartition,PesCurrtentTestFileSize);
		    if(PesCurrtentTestFileBuffer != NULL)
		    {
				AudFileHandle=DRV_VfsOpenFile((char *)"c:/h264.pes","rb");
		    
				DRV_VfsReadFile(AudFileHandle,PesCurrtentTestFileBuffer,PesCurrtentTestFileSize);
				DRV_VfsCloseFile(AudFileHandle);
			}
            
			DRV_AudioPesInjectStart(AudStartParams,AudPCMInputParams,DRV_AUDIO_TYPE_MP3);
			PesAudioInjectTestTask = DRV_OS_TaskCreate("PesAudioInjectTestProcess",
                               PesAudioInjectTestProcess,
                               9,
                               4096*40,
                               NULL,
                               0);
			break;
		default:
			return DRV_ERROR_BAD_PARAMETER;
			break;
	}
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_PesAudioTestInjectStop(DRV_InjectSrc InjectSrc)
{
	DRV_ErrCode ErrCode;
	PesCurrtentTestTaskStatus = FALSE;
	while(!PesTaskExit)
	{
		DRV_OS_TaskDelay_MS(100);
	}
	printf("------task exit ok!\n");
	PesTaskExit = FALSE;
	while(1)
	{
		ErrCode = DRV_OS_TaskWait(PesAudioInjectTestTask, DRVOS_WAIT_FINITY, 500); 
		if(ErrCode == 0)
			break;
	    DRV_OS_TaskKill(PesAudioInjectTestTask, 0, 0);
		DRV_OS_TaskDelay_MS(100);
	}
    DRV_OSTaskDelete(PesAudioInjectTestTask);
    if(PesCurrtentTestFileBuffer != NULL)
    {
    	DRV_OSFree(SystemPartition,PesCurrtentTestFileBuffer);
    	PesCurrtentTestFileBuffer = NULL;
    }	
	DRV_AudioPesInjectStop();
	return DRV_NO_ERROR;
}


