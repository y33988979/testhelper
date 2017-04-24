/*****************************************************************************

File name  : vout.c
Description: VOUT functions

COPYRIGHT (C) STMicroelectronics 2004.

*****************************************************************************/

/* Includes --------------------------------------------------------------- */

#include <string.h>
#include <hi_unf_vo.h>
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "drv_vo.h"
#include "drv_avplay.h"
/* Private Types ---------------------------------------------------------- */

/* Private Constants ------------------------------------------------------ */

/* Private Variables ------------------------------------------------------ */
/* Private Macros --------------------------------------------------------- */
HI_HANDLE mAvPlayWinHandle;
/* Private Function prototypes -------------------------------------------- */

/* Global Variables ------------------------------------------------------- */
/* Functions -------------------------------------------------------------- */


/*-------------------------------------------------------------------------
 * Function : VOUT_Setup
 * Input    : None
 * Output   :
 * Return   : Error Code
 * ----------------------------------------------------------------------*/
DRV_ErrCode VO_Setup(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
#ifdef DRV_TRANSCODER_SERVER
    HI_UNF_VO_DEV_MODE_E        enDevMode = HI_UNF_VO_DEV_MODE_MOSAIC;
#else
    HI_UNF_VO_DEV_MODE_E        enDevMode = HI_UNF_VO_DEV_MODE_NORMAL;
#endif
    HI_UNF_WINDOW_ATTR_S   WinAttr;
    ErrCode = HI_UNF_VO_Init(enDevMode);
    if (ErrCode != HI_SUCCESS)
    {
        printf("VO_Setup HI_UNF_VO_Init failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }

    ErrCode = HI_UNF_VO_Open(HI_UNF_VO_HD0);
    if (ErrCode != HI_SUCCESS)
    {
        printf("VO_Setup HI_UNF_VO_Open failed.ErrCode 0x%08x\n",ErrCode);
        HI_UNF_VO_DeInit();
        return ErrCode;
    }
    WinAttr.enVo = HI_UNF_VO_HD0;
    WinAttr.bVirtual = HI_FALSE;
    WinAttr.enAspectRatio = HI_UNF_ASPECT_RATIO_16TO9;
    WinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_IGNORE;
    WinAttr.stInputRect.s32X = 0;
    WinAttr.stInputRect.s32Y = 0;
    WinAttr.stInputRect.s32Width = 1920;
    WinAttr.stInputRect.s32Height = 1080;
    memcpy(&WinAttr.stOutputRect,&WinAttr.stInputRect,sizeof(HI_RECT_S));

    ErrCode = HI_UNF_VO_CreateWindow(&WinAttr, &mAvPlayWinHandle);
    if (ErrCode != HI_SUCCESS)
    {
        printf("VO_Setup HI_UNF_VO_CreateWindow failed.ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
    }
    //ErrCode = HI_UNF_VO_AttachWindow(mAvPlayWinHandle, mAvPlayHandle);
    //if (HI_SUCCESS != ErrCode)
    //{
    //    printf("VO_Setup HI_UNF_VO_AttachWindow failed.ErrCode 0x%08x\n",ErrCode);
    //    return ErrCode;
    //}	
    /* Return no errors */
	/* ================ */
	return ErrCode;
}
/*****************************************************************************
  Date&Time       : 2004-03-16
  Author          : yzb
  Function Name   : DRV_SetScreenRatio
  Description     :  
  Input           : 
  Outut           : none 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode  DRV_SetScreenRatio(DRV_ScreenRatio  mode) /* 1( 16:9)   0(4:3) 2(auto)*/
{
    DRV_ErrCode ErrCode;
	HI_UNF_WINDOW_ATTR_S stWinAttr;
	HI_UNF_ASPECT_RATIO_E enAspectRatio;
	switch(mode)
	{
		case DRV_SCREEN_RATE_4_3:
			enAspectRatio = HI_UNF_ASPECT_RATIO_4TO3; 
			break;
		case DRV_SCREEN_RATE_16_9:
			enAspectRatio = HI_UNF_ASPECT_RATIO_16TO9; 
			break;
		case DRV_SCREEN_RATE_AUTO:
            enAspectRatio = HI_UNF_ASPECT_RATIO_FULL; 
            break;
		default:
			enAspectRatio = HI_UNF_ASPECT_RATIO_FULL; 
			break;
	}
	ErrCode = HI_UNF_VO_GetWindowAttr(mAvPlayWinHandle, &stWinAttr);
	if (ErrCode != HI_SUCCESS)
	{
		printf("DRV_SetScreenRatio HI_UNF_VO_GetWindowAttr failed!ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
	}
	
	stWinAttr.enAspectRatio = enAspectRatio;
	if(mode == DRV_SCREEN_RATE_AUTO)
	{
        stWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_IGNORE;
	}
	else
	{
        stWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_LETTERBOX;
	}
 	ErrCode = HI_UNF_VO_SetWindowAttr(mAvPlayWinHandle, &stWinAttr);
	if (ErrCode != HI_SUCCESS)
	{
		printf("DRV_SetScreenRatio HI_UNF_VO_SetWindowAttr failed!ErrCode 0x%08x\n",ErrCode);
        return ErrCode;
	}
   return ErrCode;
}

DRV_ErrCode  DRV_SetScreenRatioConversion(DRV_ScreenRatioConversion  mode)
{
    DRV_ErrCode ErrCode;
	HI_UNF_WINDOW_ATTR_S pWinAttr;    
	ErrCode = HI_UNF_VO_GetWindowAttr(mAvPlayWinHandle, &pWinAttr);
    if(ErrCode != DRV_NO_ERROR)
    {
		printf("DRV_SetScreenRatioConversion HI_UNF_VO_SetWindowAttr failed!ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
	switch(mode)
	{
		case DRV_AR_CONVERSION_PAN_SCAN:
			pWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_PANANDSCAN;
			break;
		case DRV_AR_CONVERSION_COMBINED:
			pWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_COMBINED;
			break;
		case DRV_AR_CONVERSION_IGNORE:
			pWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_IGNORE;
			break;
		case DRV_AR_CONVERSION_LETTER_BOX:
		default:
			pWinAttr.enAspectCvrs = HI_UNF_ASPECT_CVRS_LETTERBOX;
			break;
	}
	ErrCode = HI_UNF_VO_SetWindowAttr(mAvPlayWinHandle,&pWinAttr);
    if(ErrCode != DRV_NO_ERROR)
    {
		printf("DRV_SetScreenRatioConversion HI_UNF_VO_SetWindowAttr failed!ErrCode 0x%08x\n",ErrCode);
    	return ErrCode;
    }
    return ErrCode;
}


/* EOF --------------------------------------------------------------------- */
