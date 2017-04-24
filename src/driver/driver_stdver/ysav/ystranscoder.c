/******************************************************************************
  File Name     : ystranscoder.c
  Version       : 
  Author        : wrwei
  Created       : 2012/09/11
  Description   : 实现多屏互动、转码功能
  History       :
  1.Date        : 2012/09/11
    Author      : wrwei
    Modification: Created file

******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "stddefs.h"
#include "hi_transcoder.h"
#include "hi_protocol.h"
#include "hi_muxer.h"
#include "ystranscoder.h"
#include "drv_avplay.h"
#include "driver.h"

S32 g_TranOuts32X;                    /*转码的起始位置 X 坐标*/
S32 g_TranOuts32Y;                    /*转码的起始位置 Y 坐标*/
S32 g_TranOuts32Width = 720;                /*视频分辨率--高*/
S32 g_TranOuts32Height = 576;               /*视频分辨率--宽*/
S32 g_TranOuts32VideoBitRate = 1024 * 1024;             /*视频比特率*/
S32 g_TranOuts32VideoFrameRate = 25;           /*视频帧率*/
S32 g_TranOuts32AudioSampleRate = 32000;          /*音频采样率*/
S32 g_TranOuts32AudioBandWidth = 16;           /*音频采样宽度*/
S32 g_TranOuts32AudioChannels = 1;            /*音频采样的声道数量*/
U32 g_Progm = 0;
Transcoder_ProtocolType g_TranProtocolType = TRANSCODER_PROTOCOL_RTSP;

HI_HANDLE               hTranscoderHandle;
HI_HANDLE               hProtocolHandle;
HI_HANDLE               hMuxerHandle;
HI_HANDLE               phMbufReadHandle;
HI_HANDLE               hVenc;
HI_HANDLE               mTransWinHandle;

extern DRVOS_Partition_p SystemPartition;

static pthread_t g_threadSampleVenc;
static HI_BOOL g_bSampleVencStop  = HI_FALSE;

#define AENC_IN_PACKET_SIZE (1024 * 3)
#define AENC_SampleRate    HI_UNF_SAMPLE_RATE_48K /* samplerate:16000|22050|24000|32000|44100|48000 */
#define TRANSCODE_VO_WIDTH 640
#define TRANSCODE_VO_HEIGHT 480

typedef struct hiVENC_THREAD_ARG_S
{
    HI_HANDLE   hVenc;
    HI_HANDLE   hAvplayForVdec;
    FILE       *pFileToSave;
} VENC_THREAD_ARG_S;

typedef struct
{
    pthread_t pthreadSendES;
    HI_BOOL   bStopSendStream;

    //HI_CHAR * infileName;
    HI_CHAR * outfileName;

    HI_HANDLE          hAenc;
    HI_U32             u32AencCodecID;
    HI_UNF_AENC_ATTR_S stAencAttr;
} AudAenc_S;

static AudAenc_S g_stAudAenc;
static HI_U32    nFrame;

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_Init
  Description     : Transcoder初始化.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Init(void)
{
    DRV_ErrCode ErrCode; 
    
    ErrCode = HI_Transcoder_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Transcoder_Init failed.\n");
        return HI_FAILURE;
    }
    ErrCode = HI_Protocol_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Protocol_Init failed.\n");
        HI_Transcoder_DeInit();
        return HI_FAILURE;
    }
    ErrCode = HI_Muxer_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Muxer_Init failed.\n");
        HI_Protocol_DeInit();
        HI_Transcoder_DeInit();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_DeInit
  Description     : Transcoder去初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_DeInit(void)
{
    DRV_ErrCode ErrCode; 
    
    ErrCode = HI_Muxer_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Muxer_DeInit error\n");
        return ErrCode;
    }
    
    ErrCode = HI_Protocol_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_DeInit error\n");
        return ErrCode;
    }
    
    ErrCode = HI_Transcoder_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Transcoder_DeInit error\n");
        return ErrCode;
    }

    return HI_SUCCESS;
}
#if 1
/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_MuxerTask
  Description     : Transcoder句柄创建,转码后保存存为文件
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_MuxerTask(void)
{
    DRV_ErrCode             ErrCode; 
    Transcoder_HandleAttr   stTranHandleAttr;    
    ProtocolAttr            protoAttr;
    Transcoder_MuxerAttr    muxerAttr;
    HI_U32   muxerType;
    HI_U32   u32Playtime;
    HI_CHAR* pTAGFrame;
    FILE* TransData_fp;
    U32_YS WriteNum; 
    HI_S32 ps32Len = 30*1024*1024;
    TRANSCODER_STRAM pstTranFrame;

    pTAGFrame = DRV_OSMalloc(SystemPartition,ps32Len);  

    memset(&stTranHandleAttr,0,sizeof(Transcoder_HandleAttr));
    memset(&muxerAttr,0,sizeof(Transcoder_MuxerAttr));
    memset(&pstTranFrame,0,sizeof(TRANSCODER_STRAM));

    HI_SYS_GetPlayTime(& u32Playtime);
    printf("u32Playtime = %d\n",u32Playtime);

    muxerAttr.audioCodec = TRANSCODER_CODEC_ID_AACLC;
    muxerAttr.videoCodec = TRANSCODER_CODEC_ID_H264;
    muxerAttr.s32Width = g_TranOuts32Width;
    muxerAttr.s32Height = g_TranOuts32Height; 
    muxerAttr.s32VideoBitRate = g_TranOuts32VideoBitRate ;
    muxerAttr.s32VideoFrameRate =  g_TranOuts32VideoFrameRate; 
    muxerAttr.s32AudioSampleRate = g_TranOuts32AudioSampleRate; 
    muxerAttr.s32AudioBandWidth = g_TranOuts32AudioBandWidth;    
    muxerAttr.s32AudioChannels = g_TranOuts32AudioChannels;
    ErrCode = HI_Muxer_Create(&hMuxerHandle,TRANSCODER_MUTEX_HIES,&muxerAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Muxer_Create failed.\n");
        return ErrCode;
    }

    ErrCode = HI_Muxer_GetHeader(hMuxerHandle,pTAGFrame,&ps32Len);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Muxer_GetHeader failed.\n");
        return ErrCode;
    }
    
    ErrCode = HI_Transcoder_RegisterHandle(hTranscoderHandle,&phMbufReadHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Transcoder_RegisterHandle failed.\n");
        return ErrCode;
    }

    while(1)
    {
        printf("Transcoder Muxer Task while ....\n");
        ErrCode = HI_Transcoder_ReadStream(phMbufReadHandle,&pstTranFrame);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_Transcoder_ReadStream failed.\n");
            return ErrCode;
        } 
        
        ErrCode = HI_Muxer_DoMuxer(hMuxerHandle,pTAGFrame,&ps32Len,&pstTranFrame);
        if (ErrCode != HI_SUCCESS)
        {
            printf("call HI_Muxer_DoMuxer failed.\n");
            return ErrCode;
        }
    }

    return HI_SUCCESS;
}
#endif

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_Open
  Description     : Transcoder句柄创建
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Open(void)
{
    DRV_ErrCode             ErrCode; 
    Transcoder_HandleAttr   stTranHandleAttr;    
    ProtocolAttr            protoAttr;
    HI_U32  u32Playtime;
    DRVOS_Task_t *pTransTaskid;

    memset(&stTranHandleAttr,0,sizeof(Transcoder_HandleAttr));

    HI_SYS_GetPlayTime(& u32Playtime);
    printf("u32Playtime = %d\n",u32Playtime);
    
    Transcoder_getDefaultAttr(&stTranHandleAttr);
    stTranHandleAttr.s32Width = g_TranOuts32Width ;               
    stTranHandleAttr.s32Height = g_TranOuts32Height ;               
    stTranHandleAttr.s32VideoBitRate = g_TranOuts32VideoBitRate ;       
    stTranHandleAttr.s32VideoFrameRate =  g_TranOuts32VideoFrameRate;           
    stTranHandleAttr.s32AudioSampleRate = g_TranOuts32AudioSampleRate;          
    stTranHandleAttr.s32AudioBandWidth = g_TranOuts32AudioBandWidth;           
    stTranHandleAttr.s32AudioChannels = g_TranOuts32AudioChannels;
    stTranHandleAttr.bCycle = HI_FALSE;//HI_TRUE;
    stTranHandleAttr.hSrc = mAvPlayHandle;
    printf("~!@#$~!@~!@~! s32VideoBitRate : %d\n",stTranHandleAttr.s32VideoBitRate);
    ErrCode = HI_Transcoder_Create(&hTranscoderHandle,&stTranHandleAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Transcoder_Create failed.\n");
        return ErrCode;
    }
    
    protoAttr.s32Port = 4098;
    protoAttr.s32UserNum = 8;
    protoAttr.SourceHandle = mAvPlayHandle;
    protoAttr.u32ProtocolType = g_TranProtocolType;
    if(g_TranProtocolType == TRANSCODER_PROTOCOL_SAMPLE)
    {
        ErrCode = HI_Protocol_RegistProtocol("libprotocol_local.so");
        if(HI_SUCCESS != ErrCode)
        {
           printf("HI_Protocol_RegistProtocol error!\n");
        }
    }
    
    ErrCode = HI_Protocol_Create(&hProtocolHandle,&protoAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Protocol_Create failed.\n");
        HI_Transcoder_Destroy(hTranscoderHandle);
        return ErrCode;
    }

    ErrCode = HI_Protocol_RegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_RegistHandle error\n");
        HI_Transcoder_Destroy(hTranscoderHandle);
        HI_Protocol_destory(hProtocolHandle);
        return ErrCode;
    }

    ErrCode = HI_Transcoder_Start(hTranscoderHandle,0);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Transcoder_Start error\n");
        return ErrCode;
    }
#if 0
    /*create a thread for led display*/
	pTransTaskid = DRV_OS_TaskCreate("DRV_Transcoder_MuxerTask",DRV_Transcoder_MuxerTask,0,8096,NULL,0);
    if (pTransTaskid == NULL)
    {
        printf("%s: %d DRV_OS_TaskCreate ErrorCode!!!\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_Close
  Description     : 销毁transcoder句柄.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Close(void)
{
    DRV_ErrCode             ErrCode; 
    
    ErrCode = HI_Protocol_UnRegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_UnRegistHandle error\n");
        return ErrCode;
    }
    
    ErrCode = HI_Protocol_destory(hProtocolHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_destory error\n");
        return ErrCode;
    }

    ErrCode = HI_Transcoder_Destroy(hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Transcoder_Destroy error\n");
        return ErrCode;
    }
    
    return HI_SUCCESS;
}
/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_Start
  Description     : Transcoder开始工作
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Start(void)
{
    S32                  ErrCode = HI_SUCCESS;
    HI_U32                  u32Playtime;
    Transcoder_HandleAttr   stTranHandleAttr;    
    ProtocolAttr            protoAttr;
    
    memset(&stTranHandleAttr,0,sizeof(Transcoder_HandleAttr));

    ErrCode = HI_Transcoder_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Transcoder_Init failed.\n");
        return ErrCode;
    }
    ErrCode = HI_Protocol_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Protocol_Init failed.\n");
        HI_Transcoder_DeInit();
        return ErrCode;
    }
    ErrCode = HI_Muxer_Init();
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_Muxer_Init failed.\n");
        HI_Protocol_DeInit();
        HI_Transcoder_DeInit();
        return ErrCode;
    }

    HI_SYS_GetPlayTime(& u32Playtime);
    printf("u32Playtime = %d\n",u32Playtime);

    Transcoder_getDefaultAttr(&stTranHandleAttr);
    stTranHandleAttr.s32Width = g_TranOuts32Width ;               
    stTranHandleAttr.s32Height = g_TranOuts32Height ;               
    stTranHandleAttr.s32VideoBitRate = g_TranOuts32VideoBitRate ;       
    stTranHandleAttr.s32VideoFrameRate =  g_TranOuts32VideoFrameRate;           
    stTranHandleAttr.s32AudioSampleRate = g_TranOuts32AudioSampleRate;          
    stTranHandleAttr.s32AudioBandWidth = g_TranOuts32AudioBandWidth;           
    stTranHandleAttr.s32AudioChannels = g_TranOuts32AudioChannels;
    stTranHandleAttr.bCycle = HI_TRUE;
    stTranHandleAttr.hSrc = mAvPlayHandle;
    printf("~!@#$~!@~!@~! s32VideoBitRate : %d\n",stTranHandleAttr.s32VideoBitRate);
    ErrCode = HI_Transcoder_Create(&hTranscoderHandle,&stTranHandleAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Transcoder_Create failed.\n");
        return ErrCode;
    }
    
    protoAttr.s32Port = 4098;
    protoAttr.s32UserNum = 8;
    protoAttr.SourceHandle = mAvPlayHandle;
    protoAttr.u32ProtocolType = g_TranProtocolType;
    if(g_TranProtocolType == TRANSCODER_PROTOCOL_SAMPLE)
    {
        ErrCode = HI_Protocol_RegistProtocol("libprotocol_local.so");
        if(HI_SUCCESS != ErrCode)
        {
           printf("HI_Protocol_RegistProtocol error!\n");
        }
    }
    
    ErrCode = HI_Protocol_Create(&hProtocolHandle,&protoAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_Protocol_Create failed.\n");
        HI_Transcoder_Destroy(hTranscoderHandle);
        return ErrCode;
    }

    ErrCode = HI_Protocol_RegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_RegistHandle error\n");
        HI_Transcoder_Destroy(hTranscoderHandle);
        HI_Protocol_destory(hProtocolHandle);
        return ErrCode;
    }

    return ErrCode;
}

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_Stop
  Description     : Transcoder停止工作
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Stop(void)
{
    DRV_ErrCode             ErrCode; 
    Transcoder_HandleAttr   stTranHandleAttr;    
    ProtocolAttr            protoAttr;
    HI_U32  u32Playtime;
    
    ErrCode = HI_Protocol_UnRegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_UnRegistHandle error\n");
        return ErrCode;
    }
    
    ErrCode = HI_Protocol_destory(hProtocolHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_destory error\n");
        return ErrCode;
    }

    ErrCode = HI_Transcoder_Destroy(hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Transcoder_Destroy error\n");
        return ErrCode;
    }
    ErrCode = HI_Muxer_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Muxer_DeInit error\n");
        return ErrCode;
    }
    ErrCode = HI_Protocol_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Protocol_DeInit error\n");
        return ErrCode;
    }
    ErrCode = HI_Transcoder_DeInit();
    if(HI_SUCCESS != ErrCode)
    {
        printf("HI_Transcoder_DeInit error\n");
        return ErrCode;
    }

    return ErrCode; 
}

/*****************************************************************************
  Date&Time       : 2012-09-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_UnRegist
  Description     : Transcoder 重新开始工作
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 此函数需在Transcoder_create后才能调用,当用户的输入源发生较大变化，需要先调用本函数
	                例如：播放本地文件时，本地文件播放完成，又从头播放的情况，或者dvb出现换台的情况。
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_Reset(void)
{
    DRV_ErrCode             ErrCode; 

    printf("***************************DRV_Transcoder_Reset************\n");
    ErrCode = HI_Protocol_WFD_SetClientCommand(hProtocolHandle, WFD_SOURCE_METHOD_RESET,HI_FALSE);
    if(HI_SUCCESS != ErrCode)
    {
		printf("call HI_Protocol_UnRegistHandle failed!\n");
    }
#if 0
    ErrCode = HI_Protocol_UnRegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
		printf("call HI_Protocol_UnRegistHandle failed!\n");
    }

    ErrCode = HI_Protocol_RegistHandle(hProtocolHandle,hTranscoderHandle);
    if(HI_SUCCESS != ErrCode)
    {
		printf("call HI_Protocol_RegistHandle failed!\n");
    }
#endif    
    return ErrCode;
}

static void* drv_Transcoder_SampleVencRoutine(void* args)
{
    HI_S32 ret = HI_FAILURE;
    VENC_THREAD_ARG_S *pArgs = (VENC_THREAD_ARG_S*)args;
    HI_UNF_VENC_STREAM_S vencStream;
    HI_HANDLE hVideoEnc = pArgs->hVenc;    
    HI_CHAR* VENC_Out_FileName = "./venc.264";
    
    FILE *pFile = fopen(VENC_Out_FileName, "wb");
    
    while (!g_bSampleVencStop)
    {
        ret = HI_UNF_VENC_AcquireStream(hVenc, &vencStream, 0);
        if (HI_SUCCESS != ret)
        {
            //printf("====>>>>>>>can NOT AcquireStream from VENC, ret=%#x.\n", ret);
            usleep(10 * 1000);
            continue;
        }

        //printf("===========>>>>>>>>>>>AcquireStream from VENC\n");
        if (pFile) /* save to file */
        {
            if (vencStream.u32SlcLen[0] > 0)
            {
                ret = fwrite(vencStream.pu8Addr[0], 1, vencStream.u32SlcLen[0], pFile);
                if (ret != vencStream.u32SlcLen[0])
                {
                    printf("fwrite err0, write size=%d, slclen=%d\n", ret, vencStream.u32SlcLen[0]);
                }

                if (vencStream.u32SlcLen[1])
                {
                    ret = fwrite(vencStream.pu8Addr[1], 1, vencStream.u32SlcLen[1], pFile);
                    if (ret != vencStream.u32SlcLen[1])
                    {
                        printf("fwrite err1, write size=%d, slclen=%d\n", ret, vencStream.u32SlcLen[1]);
                    }
                }
            }
        }

        ret = HI_UNF_VENC_ReleaseStream(hVenc, &vencStream);
        if (HI_SUCCESS != ret)
        {
            printf("HI_UNF_VENC_ReleaseStream failed, u32Ret=%x\n", ret);
        }
    }

    return NULL;
}
/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_VideoEncInit
  Description     : 初始化视频转码模块，此模块把正在播放的节目视频转码成指定格式的视频ES数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_VideoEncInit(void)
{
    DRV_ErrCode   ErrCode; 
    HI_UNF_VENC_CHN_ATTR_S stVeAttr;
    HI_UNF_WINDOW_ATTR_S   stWinAttrVoVirtual;
    VENC_THREAD_ARG_S      g_stSampleVencArgs;

#if 1  //创建一个虚拟的window，并绑定播放器
    stWinAttrVoVirtual.enVo = HI_UNF_VO_HD0;
    /*virtual window*/
    stWinAttrVoVirtual.bVirtual = HI_TRUE;
    stWinAttrVoVirtual.enVideoFormat = HI_UNF_FORMAT_YUV_PACKAGE_YUYV;
    stWinAttrVoVirtual.enAspectRatio = HI_UNF_ASPECT_RATIO_4TO3;
    stWinAttrVoVirtual.enAspectCvrs = HI_UNF_ASPECT_CVRS_IGNORE;
    stWinAttrVoVirtual.stInputRect.s32X = 0;
    stWinAttrVoVirtual.stInputRect.s32Y = 0;
    stWinAttrVoVirtual.stInputRect.s32Width  = 1920;
    stWinAttrVoVirtual.stInputRect.s32Height = 1080;
    stWinAttrVoVirtual.stOutputRect.s32X = 0;
    stWinAttrVoVirtual.stOutputRect.s32Y = 0;
    stWinAttrVoVirtual.stOutputRect.s32Width  = TRANSCODE_VO_WIDTH;
    stWinAttrVoVirtual.stOutputRect.s32Height = TRANSCODE_VO_HEIGHT;
    ErrCode = HI_UNF_VO_CreateWindow(&stWinAttrVoVirtual, &mTransWinHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HIADP_VO_Init failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_VO_AttachWindow(mTransWinHandle, mAvPlayHandle);
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_UNF_VO_AttachWindow failed.\n");
        return ErrCode;
    }

    ErrCode = HI_UNF_VO_SetWindowEnable(mTransWinHandle, HI_TRUE);
    if (HI_SUCCESS != ErrCode)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        return ErrCode;
    }   
#endif
  
    ErrCode = HI_UNF_VENC_Init( );
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_Init failed.\n");
        return ErrCode;
    }

    ErrCode = HI_UNF_VENC_GetDefaultAttr(&stVeAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_GetDefaultAttr failed.\n");
        return ErrCode;
    }

    stVeAttr.enVencType = HI_UNF_VCODEC_TYPE_H264;
    stVeAttr.u32Height = TRANSCODE_VO_HEIGHT;
    stVeAttr.u32Width = TRANSCODE_VO_WIDTH;
    stVeAttr.bSlcSplitEn  = HI_FALSE;
    stVeAttr.u32SplitSize = 1024;
    stVeAttr.u32Gop = 300;
    stVeAttr.u32Qlevel = 0;
    stVeAttr.u32TargetBitRate = 1768 * 1024;
    stVeAttr.enYuvStoreType   = HI_UNF_VENC_STORE_PACKAGE;
    stVeAttr.enYuvSampleType  = HI_UNF_VENC_YUV_422;
    stVeAttr.u32InputFrmRate  = 30;
    stVeAttr.u32TargetFrmRate = 30;
    stVeAttr.bSendToNet = HI_FALSE;

    ErrCode = HI_UNF_VENC_CreateChn(&hVenc, &stVeAttr);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_CreateChn failed.\n");
        return ErrCode;
    }

    ErrCode = HI_UNF_VENC_AttachInput(hVenc, mTransWinHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_AttachInput failed.\n");
        return ErrCode;
    }

    //g_stSampleVencArgs.hAvplayForVdec = mAvPlayHandle;
    //g_stSampleVencArgs.hVenc = hVenc;

    //pthread_create(&g_threadSampleVenc, HI_NULL, drv_Transcoder_SampleVencRoutine, (void*)(&g_stSampleVencArgs));
    //ErrCode = HI_UNF_VENC_Start(hVenc);
    //if (ErrCode != HI_SUCCESS)
    //{
    //    printf("call HI_UNF_VENC_Start failed.\n");
    //    return ErrCode;
    //}

    return ErrCode;
}

/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_VideoEncDeInit
  Description     : 去初始化视频转码模块
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_VideoEncDeInit(void)
{
    DRV_ErrCode   ErrCode; 
    HI_UNF_VENC_CHN_ATTR_S stVeAttr;
    HI_UNF_WINDOW_ATTR_S   stWinAttrVoVirtual;
    VENC_THREAD_ARG_S      g_stSampleVencArgs;

    ErrCode = HI_UNF_VO_DetachWindow(mTransWinHandle, mAvPlayHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VO_DetachWindow failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_VO_DestroyWindow(mTransWinHandle);
    {
        printf("call HI_UNF_VENC_DeInit failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_VENC_DeInit();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_DeInit failed.\n");
        return ErrCode;
    }

    return ErrCode;
}
/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_VideoEncStart
  Description     : 把正在播放的节目视频转码成指定格式(如H264等)的视频ES数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_VideoEncStart(void)
{
    DRV_ErrCode   ErrCode = HI_SUCCESS; 
    VENC_THREAD_ARG_S      g_stSampleVencArgs;

    ErrCode = HI_UNF_VENC_Start(hVenc);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_Start failed.\n");
        return ErrCode;
    }

    g_stSampleVencArgs.hAvplayForVdec = mAvPlayHandle;
    g_stSampleVencArgs.hVenc = hVenc;
    pthread_create(&g_threadSampleVenc, HI_NULL, drv_Transcoder_SampleVencRoutine, (void*)(&g_stSampleVencArgs));

    return ErrCode;
}

/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_VideoEncStop
  Description     : 停止视频转码模块
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_VideoEncStop(void)
{
    DRV_ErrCode   ErrCode; 
  
    pthread_cancel(g_threadSampleVenc);
    ErrCode = HI_UNF_VENC_Stop(hVenc);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_VENC_Start failed.\n");
        return ErrCode;
    }
    
    return ErrCode;
}

static HI_S32 drv_Transcoder_AencWriteFile(FILE* outfile)
{
    HI_UNF_ES_BUF_S sAencFrame;
    HI_UNF_ES_BUF_S *pstAencFrame = &sAencFrame;
    HI_S32 ErrCode;

    ErrCode = HI_UNF_AENC_AcquireStream(g_stAudAenc.hAenc, pstAencFrame);
    if (HI_SUCCESS == ErrCode)
    {
        if (outfile)
        {
            fwrite(pstAencFrame->pu8Buf, 1, pstAencFrame->u32BufLen, outfile);
        }

        nFrame++;
        if ((nFrame % 256 == 0))
        {
            //printf("%s Sent Stream times =%d \n", __FUNCTION__, nFrame);
        }

        ErrCode = HI_UNF_AENC_ReleaseStream(g_stAudAenc.hAenc, pstAencFrame);
    }

    return ErrCode;
}

HI_S32 drv_Transcoder_AencThread(AudAenc_S* pAud)
{
    FILE *outfile;
    HI_S32 ErrCode = 0;

    g_stAudAenc.bStopSendStream = HI_FALSE;

    /* open output file */
    outfile = fopen(pAud->outfileName, "wb");
    if (!outfile)
    {
        printf(" *** Error opening output file %s ***\n", pAud->outfileName);
        return -1;
    }

    while (!(pAud->bStopSendStream))
    {
        if ((nFrame >= 256 * 100))
        {
            break; //8M
        }

        ErrCode = drv_Transcoder_AencWriteFile(outfile);
        usleep(10000); //<20ms
    }

    if (outfile)
    {
        fclose(outfile);
    }

    printf(" *** Aenc Send Stream Stop ***\n\n");
    
    return ErrCode;
}

/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_AudioEncInit
  Description     : 初始化音频转码模块，此模块把正在播放的节目音频转码成指定格式的音频ES数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_AudioEncInit(void)
{
    DRV_ErrCode   ErrCode; 
    HI_CHAR* AENC_Out_FileName = "./aenc.aac";
    HI_CHAR* g_HA_EncoderDllName = {"libHA.AUDIO.AAC.encode.so"};
    
    ErrCode = HI_UNF_AENC_Init();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_Init failed.\n");
        return ErrCode;
    }

    ErrCode = HI_UNF_AENC_RegisterEncoder(g_HA_EncoderDllName);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_RegisterEncoder failed.\n");
        return ErrCode;
    }

    g_stAudAenc.outfileName = AENC_Out_FileName;
    g_stAudAenc.stAencAttr.enAencType = HA_AUDIO_ID_AAC;
    g_stAudAenc.stAencAttr.sOpenParam.u32DesiredSampleRate = AENC_SampleRate;
    
    AAC_ENC_CONFIG stPrivateConfig;
    AAC_GetDefaultConfig(&stPrivateConfig);
    stPrivateConfig.sampleRate = AENC_SampleRate; // only support 2chn

    ErrCode = HIADP_AENC_GetAttr(&(g_stAudAenc.stAencAttr), (HI_VOID *)&stPrivateConfig);
    ErrCode |= HI_UNF_AENC_CreateChn(&(g_stAudAenc.hAenc), &(g_stAudAenc.stAencAttr));
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_CreateChn failed.\n");
        return ErrCode;
    }

    ErrCode = HI_UNF_AENC_Attach(g_stAudAenc.hAenc, mAvPlayHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_Attach failed.\n");
        return ErrCode;
    }

    pthread_create(&(g_stAudAenc.pthreadSendES), HI_NULL, (HI_VOID *)drv_Transcoder_AencThread, (HI_VOID *)&(g_stAudAenc));
    //ErrCode = HI_UNF_AENC_Start(g_stAudAenc.hAenc);
    //if (ErrCode != HI_SUCCESS)
    //{
    //    g_stAudAenc.bStopSendStream = HI_TRUE;
    //    printf("call HI_UNF_AENC_Start failed.\n");
    //    return ErrCode;
    //}
    return ErrCode;
}
/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_AudioEncDeInit
  Description     : 去初始化音频转码模块
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_AudioEncDeInit(void)
{
    DRV_ErrCode   ErrCode; 
    HI_CHAR* AENC_Out_FileName = "./aenc.aac";
    HI_CHAR* g_HA_EncoderDllName = {"libHA.AUDIO.AAC.encode.so"};

    pthread_kill(g_stAudAenc.pthreadSendES,0);

    ErrCode = HI_UNF_AENC_Detach(g_stAudAenc.hAenc, mAvPlayHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_Detach failed.\n");
        return ErrCode;
    }
    ErrCode = HI_UNF_AENC_DeInit();
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_DeInit failed.\n");
        return ErrCode;
    }
    
    return ErrCode;
}
/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_AudioEncStart
  Description     : 把正在播放的节目音频转码成指定格式(如H264等)的音频ES数据
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_AudioEncStart(void)
{
    DRV_ErrCode   ErrCode; 

    ErrCode = HI_UNF_AENC_Start(g_stAudAenc.hAenc);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_Start failed!\n");
        return ErrCode;
    }

    return ErrCode;
}

/*****************************************************************************
  Date&Time       : 2012-10-11
  Author          : wrwei create
  Function Name   : DRV_Transcoder_AudioEncStop
  Description     : 停止音频转码模块
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_Transcoder_AudioEncStop(void)
{
    DRV_ErrCode   ErrCode; 

    ErrCode = HI_UNF_AENC_Stop(g_stAudAenc.hAenc);
    if (ErrCode != HI_SUCCESS)
    {
        printf("call HI_UNF_AENC_Start failed!\n");
        return ErrCode;
    }
    
    return ErrCode;
}
    
