/*****************************************************************************

  File name: layer.c
Description: LAYER functions

COPYRIGHT (C) STMicroelectronics 2004.

*****************************************************************************/

/* Includes --------------------------------------------------------------- */
#include <stddefs.h>
#include <hi_unf_common.h>
#include <hi_unf_disp.h>
#include <hi_unf_hdmi.h>
#include <hi_go_gdev.h>
#include <hi_go_bliter.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"

HI_UNF_ENC_FMT_E mEncFormat;
/* Global Variables ------------------------------------------------------- */

/* Functions -------------------------------------------------------------- */


/*-------------------------------------------------------------------------
 * Function : LAYER_Setup
 * Input    : Instance
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
DRV_ErrCode DISPLAY_Setup(void)
{
    DRV_ErrCode                 ErrCode;
    HI_UNF_DISP_BG_COLOR_S      BgColor;
    HI_UNF_DISP_INTERFACE_S     DacMode;
    mEncFormat = HI_UNF_ENC_FMT_1080i_50;
    ErrCode = HI_UNF_DISP_Init();
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Init failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    BgColor.u8Red = 0;
    BgColor.u8Green = 0;
    BgColor.u8Blue = 0;
	ErrCode = HI_UNF_DISP_SetBgColor(HI_UNF_DISP_SD0, &BgColor);
	if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_SetBgColor HI_UNF_DISP_SD0 failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DISP_SetBgColor(HI_UNF_DISP_HD0, &BgColor);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_SetBgColor HI_UNF_DISP_HD0 failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DISP_SetIntfType(HI_UNF_DISP_HD0, HI_UNF_DISP_INTF_TYPE_TV);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_SetIntfType failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, mEncFormat);  
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Init HI_UNF_DISP_SetFormat.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    if ((HI_UNF_ENC_FMT_1080P_60 == mEncFormat)
        ||(HI_UNF_ENC_FMT_1080i_60 == mEncFormat)
        ||(HI_UNF_ENC_FMT_720P_60 == mEncFormat)
        ||(HI_UNF_ENC_FMT_480P_60 == mEncFormat))
    {
        ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_NTSC);
        if (DRV_NO_ERROR != ErrCode)
        {
            printf("HI_UNF_DISP_SetFormat HI_UNF_DISP_SetFormat failed.ErrCode 0x%08x\n",ErrCode);
            HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
            HI_UNF_DISP_DeInit();
            return ErrCode;
        }
    }

    if ((HI_UNF_ENC_FMT_1080P_50 == mEncFormat)
        ||(HI_UNF_ENC_FMT_1080i_50 == mEncFormat)
        ||(HI_UNF_ENC_FMT_720P_50 == mEncFormat)
        ||(HI_UNF_ENC_FMT_576P_50 == mEncFormat))
    {
        ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_PAL);
        if (DRV_NO_ERROR != ErrCode)
        {
            printf("HI_UNF_DISP_SetFormat HI_UNF_DISP_SetFormat failed.ErrCode 0x%08x\n",ErrCode);
            HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
            HI_UNF_DISP_DeInit();
            return ErrCode;
        }
    }

    DacMode.bScartEnable = HI_FALSE;
    DacMode.bBt1120Enable = HI_FALSE;
    DacMode.bBt656Enable = HI_FALSE;
    DacMode.enDacMode[0] = DACMODE0;
    DacMode.enDacMode[1] = DACMODE1;
    DacMode.enDacMode[2] = DACMODE2;
    DacMode.enDacMode[3] = DACMODE3;
    DacMode.enDacMode[4] = DACMODE4;
    DacMode.enDacMode[5] = DACMODE5;
    ErrCode = HI_UNF_DISP_SetDacMode(&DacMode);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_SetDacMode failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_DISP_DeInit();
        return ErrCode;
    }

    ErrCode = HI_UNF_DISP_Attach(HI_UNF_DISP_SD0, HI_UNF_DISP_HD0);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Attach failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_DISP_DeInit();
        return ErrCode;
    }
    
    ErrCode = HI_UNF_DISP_Open(HI_UNF_DISP_HD0);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Open failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_DISP_DeInit();
        return ErrCode;
    }
	
    ErrCode = HI_UNF_DISP_Open(HI_UNF_DISP_SD0);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Open SD failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
        HI_UNF_DISP_DeInit();
        return ErrCode;
    }

    ErrCode = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0,mEncFormat);
    if (DRV_NO_ERROR != ErrCode)
    {
        printf("DISPLAY_Setup HI_UNF_DISP_Open SD failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_DISP_Close(HI_UNF_DISP_SD0);
        HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
        HI_UNF_DISP_DeInit();
        return ErrCode;
    }
#if 0
{
    HI_S32 ret;
    HIGO_LAYER_INFO_S stLayerInfoHD;
    HI_HANDLE hLayerHD,hLayerSurfaceHD;
    /** create graphic layer  */    
    ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfoHD);
    stLayerInfoHD.CanvasWidth = 1920;
    stLayerInfoHD.CanvasHeight = 1080;
    stLayerInfoHD.DisplayWidth = 1920;
    stLayerInfoHD.DisplayHeight = 1080;
    stLayerInfoHD.ScreenWidth = 1920;
    stLayerInfoHD.ScreenHeight = 1080;
    stLayerInfoHD.PixelFormat = HIGO_PF_8888;
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    stLayerInfoHD.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;
    stLayerInfoHD.LayerID = HIGO_LAYER_HD_0;
    
    ret = HI_GO_CreateLayer(&stLayerInfoHD,&hLayerHD);
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }
       
    /** get the graphic layer Surface */
    ret = HI_GO_GetLayerSurface (hLayerHD,&hLayerSurfaceHD); 
    if (HI_SUCCESS != ret)  
    {
       HI_GO_DestroyLayer(hLayerHD);
       HI_GO_Deinit();
       return HI_FAILURE;
    }
    
    /** draw a rectangle on the Surface*/
    ret = HI_GO_FillRect(hLayerSurfaceHD,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
       HI_GO_DestroyLayer(hLayerHD);
       HI_GO_Deinit();
       return HI_FAILURE; 
    }

    ret = HI_GO_RefreshLayer(hLayerHD, NULL);
    if (HI_SUCCESS != ret)
    {
       HI_GO_DestroyLayer(hLayerHD);
        HI_GO_Deinit();
        return HI_FAILURE;
    }
    printf("HD, SD both show red at the same time, there is no black screen, no flicker, please input anykey and stop it\n");	
	while(1)
	{}
}
#endif
    
    /* don't let the video be covered by the start-up logo, usually, you should close osd layer in HiGO or HiFB */
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_HD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVETOP);
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_SD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVETOP);

    return ErrCode;
}


/* EOF --------------------------------------------------------------------- */

