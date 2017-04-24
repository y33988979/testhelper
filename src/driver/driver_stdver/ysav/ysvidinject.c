/**************************************************************************************

dtvinject.c(20051122)

DTV Video Inject for 5105

Caution: 1. Inject Will Unlink VID from PTI and Link it to User buffer,  The live TV decode
                 can't work before Inject finish(Link VID to PTI).
              2. The MinInjectTime is 17ms and the MaxInjectTime is 22ms, we use 22ms.
                  Wait 17ms for VSYNC,not large than 22ms to avoid bit buffer overflow.
                  After Inject wait one frame time about 40ms.
              3. The MaxInjectSize is 90K. We inject 4K per ms,Max inject 88K in 22ms .

**************************************************************************************/
#include <string.h>
#include <stdio.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "drv_avplay.h"
#include "ysav.h"
#include "osddrv.h"
#include <hi_go_surface.h>
#include <hi_go_bliter.h>
#include <hi_go_encoder.h>
#include <hi_unf_common.h>
#include <hi_type.h>
#include <hi_unf_mce.h>
   

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : drv_InjectIframe
  Description     :灌入I 帧
  Input           :当pstCapPicture为空指针时，解完的I帧将在window上显示，
                    如果非空，则不会显示而是将I帧信息上报。
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_InjectIframe(U8 * pData,U32 uiDataLength,HI_UNF_CAPTURE_PICTURE_S *pstCapPicture)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_UNF_AVPLAY_I_FRAME_S stIframe;
    HI_UNF_AVPLAY_STOP_OPT_S stStopOpt;
    HI_UNF_SYNC_ATTR_S   SyncAttr;
    if(pData == NULL) 
    {
        return HI_FAILURE; 
    }
 
	drv_videoInjectClaim();
	ErrCode = DRV_AV_StopVideo(0, FALSE);
    ErrCode = HI_UNF_AVPLAY_GetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
    ErrCode |= HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (ErrCode != DRV_NO_ERROR)
    {
		printf("drv_InjectIframe HI_UNF_AVPLAY_SetAttr enSyncRef failed.ErrCode 0x%08x\n",ErrCode);
    }
    stIframe.enType     = HI_UNF_VCODEC_TYPE_MPEG2;
    stIframe.pu8Addr    = pData;
    stIframe.u32BufSize = uiDataLength;
	ErrCode = HI_UNF_AVPLAY_DecodeIFrame(mAvPlayHandle,&stIframe,pstCapPicture);
	if (DRV_NO_ERROR != ErrCode)
	{
	    printf("drv_InjectIframe HI_UNF_AVPLAY_DecodeIFrame failed.ErrCode 0x%08x\n",ErrCode);
    	drv_videoInjectRelease();
	    return HI_FAILURE;
	}
    drv_videoInjectRelease();
    DRV_OS_TaskDelay_MS(100);
    return ErrCode;
}

static OSD_Region_t *mStillRegion;
   

 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : drv_ProcessIframe
  Description     :对I帧数据进行处理，保存成图片，位块搬移等
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
HI_S32  drv_ProcessIframe(HI_HANDLE DstSurfaceHandle,HI_UNF_CAPTURE_PICTURE_S *pCapPic,HI_BOOL bSave)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_HANDLE hLayerSurface,hMemSurface;
    HIGO_SURINFO_S    stSurInfo;
    HIGO_BLTOPT2_S    stBltOpt;
    memset(&stSurInfo,0,sizeof(stSurInfo));
    stSurInfo.Width = pCapPic->u32Width;
    stSurInfo.Height = pCapPic->u32Height;
    stSurInfo.PixelFormat = HIGO_PF_YUV420;
    stSurInfo.pPhyAddr[0] = (HI_CHAR*)pCapPic->u32TopYStartAddr;
    stSurInfo.pPhyAddr[1] = (HI_CHAR*)pCapPic->u32TopCStartAddr;
    stSurInfo.Pitch[0] = pCapPic->u32Stride;
    stSurInfo.Pitch[1] = pCapPic->u32Stride;
    stSurInfo.pVirAddr[0] = (HI_CHAR*)pCapPic->u32TopYStartAddr;
    stSurInfo.pVirAddr[1] = (HI_CHAR*)pCapPic->u32TopCStartAddr;

    ErrCode= HI_GO_CreateSurfaceFromMem(&stSurInfo, &hMemSurface);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("drv_ProcessIframe HI_GO_CreateSurfaceFromMem failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }

    if(HI_TRUE == bSave)  /*将最后一帧保存为jpeg图片*/
    {
        HIGO_ENC_ATTR_S stAttr;
        #define CAPTURE_PIC_PATH    "./capture.jpg"

        stAttr.ExpectType = HIGO_IMGTYPE_JPEG;
        stAttr.QualityLevel = 99;

        ErrCode= HI_GO_InitEncoder();
        if (DRV_NO_ERROR!= ErrCode)
        {
            printf("\n drv_ProcessIframe HI_GO_InitEncoder failed\n");
        }
        ErrCode= HI_GO_EncodeToFile(hMemSurface , CAPTURE_PIC_PATH, &stAttr);
        if (DRV_NO_ERROR!= ErrCode)
        {
            printf("\n drv_ProcessIframe HI_GO_EncodeToFile failed ErrCode=0x%x\n",ErrCode);
        }
        ErrCode= HI_GO_DeinitEncoder();
        if (DRV_NO_ERROR!= ErrCode)
        {
            printf("\n drv_ProcessIframe HI_GO_DeinitEncoder failed\n");
        }
        printf("\n drv_ProcessIframe save to capture.jpeg end\n");
    }
    
    memset(&stBltOpt,0,sizeof(HIGO_BLTOPT2_S));
    ErrCode = HI_GO_StretchBlit(hMemSurface,HI_NULL,DstSurfaceHandle,HI_NULL,&stBltOpt);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("drv_ProcessIframe HI_GO_StretchBlit failed.ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
	DRV_OSDUpdate();
    ErrCode = HI_GO_FreeSurface(hMemSurface);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("drv_ProcessIframe HI_GO_FreeSurface failed.ErrCode 0x%08x\n",ErrCode);
    }

    return ErrCode;
}
    
 
  /*****************************************************************************
   Date&Time       : 2012-12-03
   Author          : whyu modify
   Function Name   : DRV_CreatStillRegion
   Description     :创建Still层Region
   Input           :
   Outut           :  
   Calls           : 
   Called by       :
   Global Variables: 
   Others          : 
 *****************************************************************************/
DRV_ErrCode DRV_CreatStillRegion(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	DRV_OSDScreenMode_t Mode;
	
	DRV_OSDGetScreenMode(&Mode);
	if(Mode == OSD_SCREEN_MODE_1920_1080)
	{
		mStillRegion = DRV_OSDRegionCreate("STILL",0,0,1920,1080,OSD_COLOR_TYPE_ARGB1555);
	}
	else if(Mode == OSD_SCREEN_MODE_1280_720)
	{
		mStillRegion = DRV_OSDRegionCreate("STILL",0,0,1280,720,OSD_COLOR_TYPE_ARGB1555);
	}
	else if(Mode == OSD_SCREEN_MODE_720_576)
	{
    	mStillRegion = DRV_OSDRegionCreate("STILL",0,0,720,576,OSD_COLOR_TYPE_ARGB1555);
	}

	return ErrCode;
}


 /*****************************************************************************
  Date&Time       : 2012-12-03
  Author          : whyu modify
  Function Name   : InjectMpeg
  Description     :注入视频数据，对视频数据解码并显示等
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode InjectMpeg(U8 * pData,U32 uiDataLength)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    DRV_OSDScreenMode_t Mode;
    HI_UNF_CAPTURE_PICTURE_S CapPicture;
    HI_HANDLE SurfaceHandle;
    mStillRegion = DRV_OSDGetRegionHandle("STILL");
    DRV_OSDGetScreenMode(&Mode);
    if(mStillRegion == NULL)
	{
		if(Mode == OSD_SCREEN_MODE_1920_1080)
		{
			mStillRegion = DRV_OSDRegionCreate("STILL",0,0,1920,1080,OSD_COLOR_TYPE_ARGB1555);
		}
		else if(Mode == OSD_SCREEN_MODE_1280_720)
		{
			mStillRegion = DRV_OSDRegionCreate("STILL",0,0,1280,720,OSD_COLOR_TYPE_ARGB1555);
		}
		else if(Mode == OSD_SCREEN_MODE_720_576)
		{
        	mStillRegion = DRV_OSDRegionCreate("STILL",0,0,720,576,OSD_COLOR_TYPE_ARGB1555);
		}
	}
	DRV_STILLEnable();
    ErrCode = drv_InjectIframe(pData,uiDataLength,&CapPicture);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("InjectMpeg drv_InjectIframe failed.ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
    ErrCode = DRV_OSDGetRegionSurface(mStillRegion,&SurfaceHandle);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("InjectMpeg DRV_OSDGetRegionSurface failed.ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
    ErrCode = drv_ProcessIframe(SurfaceHandle,&CapPicture,HI_FALSE);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("InjectMpeg drv_ProcessIframe failed.ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
    return ErrCode;
}

#define LOGO_MAX_SIZE 1*1024*1024
static U8 tmpbuff[LOGO_MAX_SIZE];
static int tmpbuffsize = 0;
static int drv_MceUpdateLogo(unsigned char *buff, int size)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    ErrCode = HI_UNF_MCE_Init(HI_UNF_PARTION_TYPE_LOGO);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_MCE_Init failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }
    
    HI_UNF_MCE_LOGO_PARAM_S LogoParam;
    ErrCode = HI_UNF_MCE_GetLogoParam(&LogoParam);
    LogoParam.contentLen = size;
    /*updata logo parameter  */
    ErrCode = HI_UNF_MCE_UpdateLogoParam(&LogoParam);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_MCE_UpdateLogoParam failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }

    ErrCode = HI_UNF_MCE_UpdateLogoContent(buff, size);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_MCE_UpdateLogoContent failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }
    
    HI_UNF_MCE_deInit();
    return 0;
}

static int drv_YUV2JEPG(HI_UNF_CAPTURE_PICTURE_S *pCapPic)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_HANDLE hMemSurface;
    HIGO_SURINFO_S    stSurInfo;
    memset(&stSurInfo,0,sizeof(stSurInfo));
    stSurInfo.Width = pCapPic->u32Width;
    stSurInfo.Height = pCapPic->u32Height;
    stSurInfo.PixelFormat = HIGO_PF_YUV420;
    stSurInfo.pPhyAddr[0] = (HI_CHAR*)pCapPic->u32TopYStartAddr;
    stSurInfo.pPhyAddr[1] = (HI_CHAR*)pCapPic->u32TopCStartAddr;
    stSurInfo.Pitch[0] = pCapPic->u32Stride;
    stSurInfo.Pitch[1] = pCapPic->u32Stride;
    stSurInfo.pVirAddr[0] = (HI_CHAR*)pCapPic->u32TopYStartAddr;
    stSurInfo.pVirAddr[1] = (HI_CHAR*)pCapPic->u32TopCStartAddr;

    ErrCode= HI_GO_CreateSurfaceFromMem(&stSurInfo, &hMemSurface);
    if (HI_SUCCESS!= ErrCode)
    {
	    printf("drv_ProcessIframe HI_GO_CreateSurfaceFromMem failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }

    HIGO_ENC_ATTR_S stAttr;
    stAttr.ExpectType = HIGO_IMGTYPE_JPEG;
    stAttr.QualityLevel = 99;
    ErrCode= HI_GO_InitEncoder();
    if (HI_SUCCESS != ErrCode)
    {
        printf("\n drv_ProcessIframe HI_GO_InitEncoder failed\n");
        return -1;
    }
    ErrCode = HI_GO_EncodeToMem(hMemSurface ,tmpbuff,  LOGO_MAX_SIZE, &tmpbuffsize, &stAttr);
    if (HI_SUCCESS != ErrCode)
    {
        printf("\n drv_ProcessIframe HI_GO_EncodeToFile failed ErrCode=0x%x\n",ErrCode);        
        return -1;
    }
    ErrCode= HI_GO_DeinitEncoder();
    if (HI_SUCCESS!= ErrCode)
    {
        printf("\n drv_ProcessIframe HI_GO_DeinitEncoder failed\n");        
        return -1;
    }

    ErrCode = HI_GO_FreeSurface(hMemSurface);
    if (HI_SUCCESS != ErrCode)
    {
        printf("drv_ProcessIframe HI_GO_FreeSurface failed.ErrCode 0x%08x\n",ErrCode);        
        return -1;
    }

    return ErrCode;
}

DRV_ErrCode DRV_LogoUpdate(U8 * pData,U32 uiDataLength)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_CAPTURE_PICTURE_S CapPicture;

    /* 解码I帧*/
    ErrCode = drv_InjectIframe(pData,uiDataLength,&CapPicture);
    if (HI_SUCCESS!= ErrCode)
    {
        printf("InjectMpeg drv_InjectIframe failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = drv_YUV2JEPG(&CapPicture);
    if (HI_SUCCESS!= ErrCode)
    {
        printf("InjectMpeg drv_YUV2JEPG failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    ErrCode = drv_MceUpdateLogo(tmpbuff, tmpbuffsize);
    if (HI_SUCCESS!= ErrCode)
    {
        printf("InjectMpeg drv_MceUpdateLogo failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    
    return ErrCode;
}

DRV_ErrCode DRV_LogoDisplay(bool disp)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    ErrCode = HI_UNF_MCE_Init(HI_UNF_PARTION_TYPE_LOGO);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_MCE_Init failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }
    ErrCode = HI_UNF_MCE_SetLogoFlag(disp);
    if (HI_SUCCESS != ErrCode)
    {
        printf("HI_UNF_MCE_SetLogoFlag failed.ErrCode 0x%08x\n",ErrCode);
        return HI_FAILURE;
    }
    HI_UNF_MCE_deInit();

    return 0;
}

/***************************************************
  Date&Time       : 2012-07-02
  Author          :   wrwei
  Description     : 清除I帧在window的显示
****************************************************/
DRV_ErrCode  drv_CleanIframe(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    
    ErrCode = HI_UNF_VO_ResetWindow(mAvPlayWinHandle, HI_UNF_WINDOW_FREEZE_MODE_BLACK);
    if (DRV_NO_ERROR!= ErrCode)
    {
	    printf("drv_CleanIframe HI_UNF_VO_ResetWindow failed.ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }

    return ErrCode;
}
#if 0
/*以下是灌到视频层的实现*/
DRV_ErrCode InjectMpeg(U8 * pData,U32 uiDataLength)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    if(uiDataLength > 0x80000)//最大512K，实际无限制，只是对错误参数的一个保护
    {
        STTBX_Print(("DRV Inject I frame is too big\n"));
        return TRUE;
    }
    DRV_VideoEnable();
    ErrCode = drv_InjectIframe(pData,uiDataLength,NULL);
    return ErrCode;
}
#endif

