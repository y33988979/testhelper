/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: test_loader.c
 * Description:
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <hi_go_bliter.h>
//#include "upgrd_identify.h"
//#include "upgrd_interface.h"
#include "hi_type.h"
#include "hlp_osd.h"
#include <hlp_log.h>
//#include "upgrd_common.h"
//#include "upgrd_upgrdstub.h"
//#include "hi_loader_info.h"
#include "hi_go_surface.h"
#include "hi_go_text.h"
#include "hi_go_gdev.h"
#include "hi_unf_ecs.h"
#include "hi_go_encoder.h"
//#include "hi_flash.h"
//#include "hi_flash_inter.h"
#include "upgrd_lang.h"
//#include "upgrd_info.h"
#include "hi_unf_common.h"


#define BOARD_TYPE_hi3716mdmo3avera

#include "hi_unf_disp.h"
#include "drv_basic.h"
//#include "hi_adp_mpi.h"
#include "hi_adp_hdmi.h"

/*******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define LOADER_ASSERT  printf
#define LOADER_PROMPT  printf

//#define WINDOW_START_X 380
//#define WINDOW_START_Y 140

#define WINDOW_START_X 150
#define WINDOW_START_Y 150
//#define WINDOW_WIDTH 500
//#define WINDOW_HEIGHT 420

static const HI_CHAR *pCHN_Str[] =
{
	//0-4
	 UPGRD_L_CN_MAN_UPGRD,
	 UPGRD_L_CN_PROMPT_UPGRD_PWD1,
	 UPGRD_L_CN_PROMPT_UPGRD_PWD2,
	 UPGRD_L_CN_INPUT_PWD,
	 UPGRD_L_CN_PROMPT_NOT_OP1,

//5-9
	 UPGRD_L_CN_PROMPT_NOT_OP2,
	 UPGRD_L_CN_PROMPT_NOT_OP3,
	 UPGRD_L_CN_BUTTON_CONFIRM,
	 UPGRD_L_CN_BUTTON_CANCLE,
	 UPGRD_L_CN_PROMPT_OPRTN,

//10-14
	 UPGRD_L_CN_TITLE_FREQ,
	 UPGRD_L_CN_TITLE_SYM_RATE,
	 UPGRD_L_CN_TITLE_MODULATE,
	 UPGRD_L_CN_TITLE_PID,
	 UPGRD_L_CN_TITLE_TALEID,

//15-19
	 UPGRD_L_CN_TITLE_AUTO,
	 UPGRD_L_CN_BUTTON_START,
	 UPGRD_L_CN_TRANS_YES,
	 UPGRD_L_CN_TRANS_NO,
	 UPGRD_L_CN_TRANS_EXIT,

//20-24
	 UPGRD_L_CN_PROMPT_POWER1,
	 UPGRD_L_TRANS_E2P,
	 UPGRD_L_TRANS_PATITION,
	 UPGRD_L_TRANS_UPDATE,
	 UPGRD_L_TRANS_ERASE,

//25-29
	 UPGRD_L_CN_PROMPT_POWER2,
	 UPGRD_L_CN_PROMPT_SEARCH,
	 UPGRD_L_CN_NT_CUR_PRGRSS,
	 UPGRD_L_CN_NT_TTL_PRGRSS,
	 UPGRD_L_CN_PT_USB_PRGRSS,

//30-34
	 UPGRD_L_CN_USB_UPGRD,
	 UPGRD_L_CN_TITLE_CONTENT,
	 UPGRD_L_CN_TITLE_FILE,
	 UPGRD_L_CN_TITLE_ERASE,
	 UPGRD_L_CN_PT_USB_CHECK,

//35-39
	 UPGRD_L_CN_PT_USB_NO_CONTENT,
	 UPGRD_L_CN_PT_USB_NO_FILE,
	 UPGRD_L_CN_ERR_USB_READ_FILE,
	 UPGRD_L_CN_ERR_USB_MEM_GET,
	 UPGRD_L_CN_SUCCESS_USB_ERASE,

//40-44
	 UPGRD_L_CN_FAIL_USB_ERASE,
	 UPGRD_L_CN_PT_USB_ERASEING,
	 UPGRD_L_CN_FAIL_SIG_EXPT,
	 UPGRD_L_CN_FAIL_FACTORY,
	 UPGRD_L_CN_FAIL_HARD_VER,

//45-49
	 UPGRD_L_CN_FAIL_SOFT_VER,
	 UPGRD_L_CN_FAIL_FILE_RNG,
	 UPGRD_L_CN_FAIL_CRC,
	 UPGRD_L_CN_FAIL_STREAM,
	 UPGRD_L_CN_FAIL_DOWNLOAD,

//50-54
	 UPGRD_L_CN_FAIL_DATA,
	 UPGRD_L_CN_FAIL_FREQUENCY,
	 UPGRD_L_CN_FAIL_FLASH,
	 UPGRD_L_CN_FAIL_UPDATE_TAG,
	 UPGRD_L_CN_FAIL_MEM_ALLOC,

//55-59
	 UPGRD_L_CN_FAIL_SYMBOL,
	 UPGRD_L_CN_FAIL_PID,
	 UPGRD_L_CN_FAIL_MODUL,
	 UPGRD_L_CN_PT_DOWNLOADING,
	 UPGRD_L_CN_PT_WRITING,

//60-64
	 UPGRD_L_CN_IP_UPGRD,
	 UPGRD_L_CN_PT_SUCC,
	 UPGRD_L_CN_WARN_DISCONN,
	 UPGRD_L_CN_SET_IP_ERR,
	 UPGRD_L_CN_FAIL_NET_EXPT,

//65-69
	 UPGRD_L_CN_FAIL_NET_UNRCH,
	 UPGRD_L_CN_IP_PARSE_XML_ERR,
	 UPGRD_L_CN_FAIL_NO_RESP,
	 UPGRD_L_CN_FAIL_FILE_LEN,
	 UPGRD_L_CN_DWNLD_WTING,

//70-74
	 UPGRD_L_CN_FAIL_UPGRD,
	 UPGRD_L_CN_IP_SERVER_ADDR,
	 UPGRD_L_CN_IP_SERVER_PORT,
	 UPGRD_L_CN_IP_HOST_SET,
	 UPGRD_L_CN_IP_OUR_DHCP,	

//75-79
	 UPGRD_L_CN_IP_OUR_HAND,
	 UPGRD_L_CN_IP_OUR_ADDR,
	 UPGRD_L_CN_IP_OUR_MASK,
	 UPGRD_L_CN_IP_OUR_GATEWAY,
	 UPGRD_L_CN_IP_CFG_NET,

//80-84
	 UPGRD_L_CN_IP_PARSE_XML,
	 UPGRD_L_CN_FILE_PARAM_ERROR,
};

static const HI_CHAR*pENG_Str[] =
{
    //0-4
	UPGRD_L_EN_MAN_UPGRD,
	UPGRD_L_EN_PROMPT_UPGRD_PWD1,
	UPGRD_L_EN_PROMPT_UPGRD_PWD2,
	UPGRD_L_EN_INPUT_PWD,
	UPGRD_L_EN_PROMPT_NOT_OP1,
	//5-9
	UPGRD_L_EN_PROMPT_NOT_OP2,
	UPGRD_L_EN_PROMPT_NOT_OP3,
	UPGRD_L_EN_BUTTON_CONFIRM,
	UPGRD_L_EN_BUTTON_CANCLE,
	UPGRD_L_EN_PROMPT_OPRTN,
	//10-14
	UPGRD_L_EN_TITLE_FREQ,
	UPGRD_L_EN_TITLE_SYM_RATE,
	UPGRD_L_EN_TITLE_MODULATE,
	UPGRD_L_EN_TITLE_PID,
	UPGRD_L_EN_TITLE_AUTO,
	//15-19
	UPGRD_L_EN_BUTTON_START,
	UPGRD_L_EN_TRANS_YES,
	UPGRD_L_EN_TRANS_NO,
	UPGRD_L_EN_TRANS_EXIT,
	UPGRD_L_EN_PROMPT_POWER1,
	//20-24
	UPGRD_L_EN_PROMPT_POWER2,
	UPGRD_L_EN_PROMPT_SEARCH,
	UPGRD_L_EN_NT_CUR_PRGRSS,
	UPGRD_L_EN_NT_TTL_PRGRSS,
	UPGRD_L_EN_PT_USB_PRGRSS,
	//25-29
	UPGRD_L_EN_FAIL_SIG_EXPT,
	UPGRD_L_EN_FAIL_FACTORY,
	UPGRD_L_EN_FAIL_HARD_VER,
	UPGRD_L_EN_FAIL_SOFT_VER,
	UPGRD_L_EN_FAIL_FILE_RNG,
	//30-34
	UPGRD_L_EN_FAIL_CRC,
	UPGRD_L_EN_FAIL_STREAM,
	UPGRD_L_EN_FAIL_DOWNLOAD,
	UPGRD_L_EN_FAIL_DATA,
	UPGRD_L_EN_FAIL_FREQUENCY,
	//35-39
	UPGRD_L_EN_FAIL_SYMBOL,
	UPGRD_L_EN_FAIL_PID,
	UPGRD_L_EN_FAIL_MODUL,
	UPGRD_L_EN_PT_DOWNLOADING,
	UPGRD_L_EN_PT_WRITING,
	//40-44
	UPGRD_L_EN_PT_SUCC,
	UPGRD_L_EN_WARN_DISCONN,
	UPGRD_L_EN_SET_IP_ERR,
	UPGRD_L_EN_FAIL_NET_EXPT,
	UPGRD_L_EN_FAIL_NET_UNRCH,
	//45-49
	UPGRD_L_EN_FAIL_NO_RESP,
	UPGRD_L_EN_FAIL_FILE_LEN,
	UPGRD_L_EN_DWNLD_WTING,
	UPGRD_L_EN_FAIL_UPGRD
};

HI_S32 g_loader_osd_init = 0;

static HI_HANDLE hLayer, hLayerSurface,hFont=0;
static HI_HANDLE hMemSurface;
static HIGO_PF_E szPixelFormat = HIGO_PF_8888;

typedef struct UpgrdBaseParamInfo_S
{
	HI_U32 checkflag;
	HI_U32 hdIntf;
	HI_U32 sdIntf;
	HI_U32 hdFmt;
	HI_U32 sdFmt;
	HI_U32 scart;
	HI_U32 Bt1120;
	HI_U32 Bt656;
	HI_U32 dac[6];
	HI_U32 layerformat;
	HI_U32 inrectwidth;
	HI_U32 inrectheight;
	HI_U32 outrectwidth;
	HI_U32 outrectheight;
	HI_U32 usewbc;
	HI_U32 resv1;
	HI_U32 resv2;
	HI_U32 logoMtdSize;
	HI_U32 playMtdSize;
}UPGRD_BASEPARAM_INFO_S;

static HI_S32 Upgrd_ReadBaseParamInfo(UPGRD_BASEPARAM_INFO_S *pstParamInfo);
static HI_S32 Upgrd_GetLanguage(HI_U8 *pu8Value);

//////////////////////////////////////
HI_S32 Drv_ShowText(HI_U32 WindHandle, LOADER_Content_E enContent, HI_S32 StartX, HI_S32 StartY, HI_U32 FontColor);

HI_S32 Drv_ShowTextEx(HI_U32 WindHandle, const HI_CHAR *pCharBuffer, HI_S32 StartX, HI_S32 StartY, HI_U32 FontColor);

/*--------------------------------*
*Role: to create a window
*input:WindPos -- The relative position of the window in the Region
        Length -- window length
        Width -- window width
        WindType -- window type
*output: N/A
*return: window handle or fail(-1)
*--------------------------------*/
HI_U32        WIN_CreateWind(sPositionXY WindPos, HI_S32 Length, HI_S32 Width, eWindType WindType);

/*--------------------------------*
*Role: Paste the window in the Region
*input:WindHandle -- window handle
        pRegion -- Region pointer
*output: N/A
*return: success(0) or fail(-1)
*--------------------------------*/
static HI_S32        WIN_PasteWind(HI_U32 WindHandle, HI_HANDLE hSurface);

/*--------------------------------*
*Role: to create a window member (input box or button)
*input:WindHandle -- window handle
        MembType -- members type
        MembPos -- The relative position of members in the window
        DefString -- Enter the default of a member of the character (for the input box, which only supports digital input)
        MaxLength -- The maximum length of characters (a character that is occupied by two characters)
*output: N/A
*return: success(0) or fail(-1)
*--------------------------------*/
HI_U32        WIN_CreateWindMember(HI_U32 WindHandle, eWMembType MembType, sPositionXY MembPos,
                                   const HI_CHAR *DefString, HI_U8 MaxLength);

/*--------------------------------*
*Role: reconstruction of the current window picture window owned by the members of the type and focus of state
*Input: MembHandle - members of the handle
*output: N/A
*return: success(0) or fail(-1)
*--------------------------------*/
static HI_S32        WIN_BuildWindPic(HI_U32 WindHandle);

/*--------------------------------*
*Role: the display window
*Input: MembHandle - members of the handle
*output: N/A
*return: success(0) or fail(-1)
*--------------------------------*/
HI_S32        WIN_ShowWind(HI_U32 WindHandle);

HI_S32        serial_upgade_check(HI_VOID);

/* function */

HI_U32 WIN_CreateWind(sPositionXY WindPos, HI_S32 Length, HI_S32 Width, eWindType WindType)
{
    sWindow *pWind = HI_NULL;
    HI_S32 ret;

    ret = HI_GO_CreateSurface(Length,Width,szPixelFormat,&hMemSurface);
    if (HI_SUCCESS != ret)
    {
       LOADER_ASSERT("%s->%d\n", __FUNCTION__, __LINE__);
       return HI_INVALID_HANDLE;
    }

    /* mem alloc for window struct and window pic */
    pWind = (sWindow *)malloc(sizeof(sWindow));
    if (HI_NULL == pWind)
    {
        LOADER_ASSERT("wind create fail!\n");
        return HI_INVALID_HANDLE;
    }
    memset((HI_U8 *)pWind, 0, sizeof(sWindow));


    /* init window struct */
    pWind->hWindHandle = (HI_U32)pWind;
    pWind->WindType   = WindType;
    pWind->WindPos.x  = WindPos.x;
    pWind->WindPos.y  = WindPos.y;
    pWind->WindLength = Length;
    pWind->WindWidth  = Width;
    pWind->hMemSurface = hMemSurface;
    pWind->FocusPos.x  = 0;
    pWind->FocusPos.y  = 0;
    pWind->pMemberList = HI_NULL;

    /** Draw a rectangle in the memory Surface */
    ret = HI_GO_FillRect(hMemSurface,NULL,BLUE_COLOR,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
       HI_GO_FreeSurface(hMemSurface);
       return HI_INVALID_HANDLE;
    }

    LOADER_PROMPT("%s->%d\n", __FUNCTION__, __LINE__);
    return (pWind->hWindHandle);
}

static HI_S32 WIN_PasteWind(HI_U32 WindHandle, HI_HANDLE hSurface)
{
    sWindow *pWind  = HI_NULL;
    HI_S32 ret ;
    HI_RECT rc={0};
    HIGO_BLTOPT_S stBltOpt ={0};

    if (0 == WindHandle)
    {
        LOADER_ASSERT("window handle or pRegion invalid!\n");
        return HI_FAILURE;
    }

    pWind   = (sWindow *)WindHandle;

    rc.x = pWind->WindPos.x;
    rc.y = pWind->WindPos.y;
    rc.w = pWind->WindLength;
    rc.h = pWind->WindWidth;

    ret = HI_GO_Blit(pWind->hMemSurface, NULL, hSurface, &rc, &stBltOpt);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    rc.x = pWind->WindPos.x - APP_WIND_BWIDTH;
    rc.y = pWind->WindPos.y - APP_WIND_BWIDTH;
    rc.w = pWind->WindLength + APP_WIND_BWIDTH * 2;
    rc.h = APP_WIND_BWIDTH;
    ret = HI_GO_FillRect(hSurface, &rc, APP_WIND_LCOLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    rc.x = pWind->WindPos.x - APP_WIND_BWIDTH;
    rc.y = pWind->WindPos.y + pWind->WindWidth;
    rc.w = pWind->WindLength + APP_WIND_BWIDTH * 2;
    rc.h = APP_WIND_BWIDTH;
    ret = HI_GO_FillRect(hSurface, &rc, APP_WIND_LCOLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    rc.x = pWind->WindPos.x - APP_WIND_BWIDTH;
    rc.y = pWind->WindPos.y - APP_WIND_BWIDTH;
    rc.w = APP_WIND_BWIDTH;
    rc.h = pWind->WindWidth;
    ret = HI_GO_FillRect(hSurface, &rc, APP_WIND_LCOLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    rc.x = pWind->WindPos.x + pWind->WindLength;
    rc.y = pWind->WindPos.y - APP_WIND_BWIDTH;
    rc.w = APP_WIND_BWIDTH;
    rc.h = pWind->WindWidth;

    ret = HI_GO_FillRect(hSurface, &rc, APP_WIND_LCOLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    return HI_SUCCESS;
}

HI_U32 WIN_CreateWindMember(HI_U32 WindHandle, eWMembType MembType,
                            sPositionXY MembPos, const HI_CHAR *pDefString, HI_U8 MaxLength)
{
    sWindMember *pMemb = HI_NULL;
    sWindMember *pMembTmp = HI_NULL;
    sWindow *pWind = (sWindow *)WindHandle;
    HI_CHAR *pStr = HI_NULL;

    /*params check*/
    if ((0 == WindHandle) || (HI_NULL == pDefString) || (0 == MaxLength))
    {
        LOADER_ASSERT("input params invalid!\n");
        return (HI_U32)HI_NULL;
    }

    /* member position check */
    if ((MembPos.x > pWind->WindLength) || (MembPos.y > pWind->WindWidth))
    {
        LOADER_ASSERT("input position params invalid!\n");
        return (HI_U32)HI_NULL;
    }

    /* alloc struct and input string mem */
    pMemb = (sWindMember *)malloc(sizeof(sWindMember));
    if (HI_NULL == pMemb)
    {
        LOADER_ASSERT("mem alloc fail!\n");
        return (HI_U32)HI_NULL;
    }

    pStr = (HI_CHAR *)malloc(sizeof(HI_U8) * (MaxLength + 1));  // +1 for \0
    if (HI_NULL == pStr)
    {
        free(pMemb);
        LOADER_ASSERT("mem alloc fail!\n");
        return (HI_U32)HI_NULL;
    }

    memset((HI_U8 *)pMemb, 0, sizeof(sWindMember));
    memset((HI_U8 *)pStr, 0, sizeof(HI_U8) * (MaxLength + 1));

    /* init struct according to member type */
    pMemb->hMembHandle = (HI_U32)pMemb;
    pMemb->MembType = MembType;
    if (HI_NULL == pWind->pMemberList) /* The first control */
    {
        pMemb->IsOnFocus = HI_TRUE;

        /* set window focus to this member */
        pWind->FocusPos.x = MembPos.x;
        pWind->FocusPos.y = MembPos.y;
    }
    else
    {
        pMemb->IsOnFocus = HI_FALSE;
    }

    pMemb->MembPos.x = MembPos.x;
    pMemb->MembPos.y = MembPos.y;
    pMemb->pNext = HI_NULL;

    memcpy(pStr, pDefString, MaxLength);
    pMemb->pDefString = pStr;
    if (APP_WMEMB_BUTTOM == MembType)
    {
        pMemb->TotalLen = MaxLength;
        pMemb->SubFocusIndex = 0;
    }
    else if(APP_WMEMB_PASSWD == MembType)
    {
        pMemb->TotalLen = MaxLength;
        pMemb->SubFocusIndex = 0;
        pMemb->IntervalNumber= IntervalTime_5s;
        pMemb->bAuto = 0;
        pMemb->IsOnEdit = HI_TRUE;
    }
    else
    {
        pMemb->TotalLen = MaxLength;
        pMemb->SubFocusIndex = (MaxLength - 1);
        pMemb->IntervalNumber= IntervalTime_5s;
        pMemb->bAuto = 0;
    }

    /* add to window member list */
    pMembTmp = pWind->pMemberList;
    if (HI_NULL == pMembTmp)  /* the first window member */
    {
        pWind->pMemberList = pMemb;
    }
    else
    {
        while (1)
        {
            if (HI_NULL == pMembTmp->pNext)
            {
                pMembTmp->pNext = pMemb;

                break;
            }

            pMembTmp = pMembTmp->pNext;
        }
    }

    return (pMemb->hMembHandle);
}




static HI_S32 WIN_BuildWindPic(HI_U32 WindHandle)
{
    sWindow *pWind = (sWindow *)WindHandle;
    sWindMember *pMembTmp = pWind->pMemberList;
    HI_S32 ret;
    HI_RECT rc={0};

    if (0 == WindHandle)
    {
        LOADER_ASSERT("invalid window handle!\n");
        return HI_FAILURE;
    }

    while (HI_NULL != pMembTmp)
    {
        if ((APP_WMEMB_BUTTOM == pMembTmp->MembType)
            || (APP_WMEMB_TRANS == pMembTmp->MembType)
            || (APP_WMEMB_TRANS_2 == pMembTmp->MembType))
        {

            if (HI_TRUE == pMembTmp->IsOnFocus)
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR);
            }
            rc.x = pMembTmp->MembPos.x;
            rc.y = pMembTmp->MembPos.y;
#if 1

	        rc.w = 14 * pMembTmp->TotalLen;
            rc.h = GRPH_FONT_SIZE;

            ret = HI_GO_FillRect(pWind->hMemSurface,&rc,YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }
#endif
            ret = HI_GO_GetTextExtent(hFont, pMembTmp->pDefString, &rc.w, &rc.h);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }

            ret = HI_GO_TextOut(hFont, pWind->hMemSurface, pMembTmp->pDefString, &rc);
            if (HI_SUCCESS != ret)
            {
                 printf("failed to HI_GO_TextOut,string [%s]\n",pMembTmp->pDefString);
                 return HI_FAILURE;
            }
        }

        if (APP_WMEMB_INPUT == pMembTmp->MembType)
        {

            if (HI_TRUE == pMembTmp->IsOnFocus)
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR);
            }

            rc.x = pMembTmp->MembPos.x;
            rc.y = pMembTmp->MembPos.y;
#if 1
            rc.w = 16 * pMembTmp->TotalLen;
            rc.h = GRPH_FONT_SIZE;

            ret = HI_GO_FillRect(pWind->hMemSurface,&rc,YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }
#endif
            ret = HI_GO_GetTextExtent(hFont, pMembTmp->pDefString, &rc.w, &rc.h);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }

            ret = HI_GO_TextOut(hFont, pWind->hMemSurface, pMembTmp->pDefString, &rc);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_TextOut\n");
                 return HI_FAILURE;
            }
#if 1
            if (HI_TRUE == pMembTmp->IsOnEdit)
            {
                /* add cursor */
                rc.x = pMembTmp->MembPos.x + pMembTmp->SubFocusIndex * 13;//11;
                rc.y = pMembTmp->MembPos.y + 21;//22;
                rc.w = 12;//10;
                rc.h = 3;// 2;
                ret = HI_GO_FillRect(pWind->hMemSurface,&rc,APP_TXT_FONTCOLOR_F/*0xFF888888*/, HIGO_COMPOPT_NONE);
                if (HI_SUCCESS != ret)
                {
                     LOADER_ASSERT("failed to HI_GO_FillRect\n");
                     return HI_FAILURE;
                }
            }
#endif
        }

        if (APP_WMEMB_PASSWD == pMembTmp->MembType)
        {

            if (HI_TRUE == pMembTmp->IsOnFocus)
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR_F);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR_F);
            }
            else
            {
                HI_GO_SetTextColor(hFont, APP_TXT_FONTCOLOR);
                HI_GO_SetTextBGColor(hFont, APP_TXT_BGCOLOR);
            }

            rc.x = pMembTmp->MembPos.x;
            rc.y = pMembTmp->MembPos.y;
#if 1
            rc.w = 16 * pMembTmp->TotalLen;
            rc.h = GRPH_FONT_SIZE;

            ret = HI_GO_FillRect(pWind->hMemSurface,&rc,YELLOW_COLOR, HIGO_COMPOPT_NONE);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_FillRect\n");
                 return HI_FAILURE;
            }
#endif
            ret = HI_GO_GetTextExtent(hFont, pMembTmp->pDefString, &rc.w, &rc.h);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_GetTextExtent\n");
                 return HI_FAILURE;
            }
            ret = HI_GO_TextOut(hFont, pWind->hMemSurface, pMembTmp->pDefString, &rc);
            if (HI_SUCCESS != ret)
            {
                 LOADER_ASSERT("failed to HI_GO_TextOut\n");
                 return HI_FAILURE;
            }
#if 1
            if (HI_TRUE == pMembTmp->IsOnEdit)
            {
                /* add cursor */
                rc.x = pMembTmp->MembPos.x + pMembTmp->SubFocusIndex * 11 + pMembTmp->SubFocusIndex/3;
                rc.y = pMembTmp->MembPos.y + 22;
                rc.w = 10;
                rc.h = 2;
                ret = HI_GO_FillRect(pWind->hMemSurface,&rc,0xFF888888, HIGO_COMPOPT_NONE);
                if (HI_SUCCESS != ret)
                {
                     LOADER_ASSERT("failed to HI_GO_FillRect\n");
                     return HI_FAILURE;
                }
            }
#endif
        }

        pMembTmp = pMembTmp->pNext;
    }

    return HI_SUCCESS;
}

HI_S32 WIN_BuildAndPasteWind(HI_U32 WindHandle)
{
    HI_S32 ret = 0;

    ret = WIN_BuildWindPic(WindHandle);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_BuildWindPic fail!\n");
        return HI_FAILURE;
    }

    ret = WIN_PasteWind(WindHandle, hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_PasteWind fail!\n");
        return HI_FAILURE;
    }

	return ret;
}

HI_S32 WIN_ShowWind(HI_U32 WindHandle)
{
    HI_S32 ret = 0;

#if 0
    sWindow *pWind = WindHandle;
    HI_RECT stRect;

    stRect.x = pWind->WindPos.x;
    stRect.y = pWind->WindPos.y;
    stRect.w = pWind->WindLength;
    stRect.h = pWind->WindWidth;

    HIGO_BLTOPT_S stBlitOpt;
    /* call osd drv to show window */

    memset(&stBlitOpt,0,sizeof(HIGO_BLTOPT_S));
    //stBlitOpt.EnablePixelAlpha = HI_TRUE;
    stBlitOpt.EnableGlobalAlpha = HI_TRUE;
    /** The use globalalpha be sure to specify the pixel alpha mixed*/
    stBlitOpt.PixelAlphaComp = HIGO_COMPOPT_SRCOVER;


    ret = HI_GO_Blit(pWind->hMemSurface,NULL,hLayerSurface,&stRect,&stBlitOpt);
    if (HI_SUCCESS != ret)
    {
       //HI_GO_FreeSurface(hMemSurface);
       //HI_GO_DestroyLayer(hLayer);
       //HI_GO_Deinit();
       return HI_FAILURE;
    }
#endif

    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("HI_GO_RefreshLayer\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HIADP_Disp_Init(HI_UNF_ENC_FMT_E enFormat)
{
    HI_S32                  Ret;
    HI_UNF_DISP_BG_COLOR_S      BgColor;
    HI_UNF_DISP_INTERFACE_S     DacMode;

    Ret = HI_UNF_DISP_Init();
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Init failed.\n");
        return Ret;
    }

    BgColor.u8Red = 0;
    BgColor.u8Green = 0;
    BgColor.u8Blue = 0;
    HI_UNF_DISP_SetBgColor(HI_UNF_DISP_HD0, &BgColor);
    HI_UNF_DISP_SetIntfType(HI_UNF_DISP_HD0, HI_UNF_DISP_INTF_TYPE_TV);
    HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, enFormat);

    if ((HI_UNF_ENC_FMT_1080P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_60 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_60 == enFormat)
        ||(HI_UNF_ENC_FMT_480P_60 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_NTSC);
        if (HI_SUCCESS != Ret)
        {
            LOADER_ASSERT("call HI_UNF_DISP_SetFormat failed.\n");
            HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
            HI_UNF_DISP_DeInit();
            return Ret;
        }
    }

    if ((HI_UNF_ENC_FMT_1080P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_1080i_50 == enFormat)
        ||(HI_UNF_ENC_FMT_720P_50 == enFormat)
        ||(HI_UNF_ENC_FMT_576P_50 == enFormat))
    {
        Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, HI_UNF_ENC_FMT_PAL);
        if (HI_SUCCESS != Ret)
        {
            LOADER_ASSERT("call HI_UNF_DISP_SetFormat failed.\n");
            HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
            HI_UNF_DISP_DeInit();
            return Ret;
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
    HI_UNF_DISP_SetDacMode(&DacMode);

    Ret = HI_UNF_DISP_Attach(HI_UNF_DISP_SD0, HI_UNF_DISP_HD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Attach failed.\n");
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISP_HD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Open failed.\n");
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISP_SD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Open SD failed.\n");
        HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0,enFormat);
    if (HI_SUCCESS != Ret)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Open SD failed.\n");
        HI_UNF_DISP_Close(HI_UNF_DISP_SD0);
        HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    /* don't let the video be covered by the start-up logo, usually, you should close osd layer in HiGO or HiFB */
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_HD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVETOP);
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_SD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVETOP);

    return Ret;
}

HI_S32 HIADP_Disp_DeInit(HI_VOID)
{
    HI_S32                      Ret;

    /* don't let the OSD be covered by the video, when we exit */
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_HD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVEBOTTOM);
    //HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_SD0, HI_UNF_DISP_LAYER_VIDEO_0, HI_LAYER_ZORDER_MOVEBOTTOM);


    Ret = HI_UNF_DISP_Close(HI_UNF_DISP_SD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Close failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DISP_Close(HI_UNF_DISP_HD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Close failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DISP_Detach(HI_UNF_DISP_SD0, HI_UNF_DISP_HD0);
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_Detach failed.\n");
        return Ret;
    }

    Ret = HI_UNF_DISP_DeInit();
    if (Ret != HI_SUCCESS)
    {
        LOADER_ASSERT("call HI_UNF_DISP_DeInit failed.\n");
        return Ret;
    }

    HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_0);

    return Ret;
}


HI_S32 Drv_OSDInit(HI_VOID)
{
    HI_S32 ret = HI_FAILURE;
    HIGO_LAYER_INFO_S stLayerInfo;
    UPGRD_BASEPARAM_INFO_S stParamInfo;
    HI_UNF_ENC_FMT_E enFmt = HI_UNF_ENC_FMT_720P_50;

    LOADER_PROMPT("Drv_OSDInit!\n");

    ret = HI_GO_Init();
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to init higo! ret = 0x%x !\n", ret);
        return ret;
    }

    HI_SYS_Init();
#if 0
    if( HI_SUCCESS != Upgrd_ReadBaseParamInfo(&stParamInfo))
    {
        LOADER_ASSERT("Failed to read baseparam, use default setting.\n");
    }
    else
    {
        enFmt = (HI_UNF_ENC_FMT_E)stParamInfo.hdFmt;
    }
#endif
enFmt = (HI_UNF_ENC_FMT_E)stParamInfo.hdFmt;
    /* Set Video show mode (N/P, Dacmode) */
    ret = HIADP_Disp_Init(enFmt);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to init HI_ADP_Disp_Init! ret = 0x%x !\n", ret);
        return ret;
    }
    else
    {
        LOADER_PROMPT("success to init disp\n");
    }

    ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        return HI_FAILURE;
    }
#if 1

    stLayerInfo.CanvasWidth = 720;
    stLayerInfo.CanvasHeight = 576;
    stLayerInfo.DisplayWidth = 720;
    stLayerInfo.DisplayHeight = 576;

    switch( enFmt )
    {
        case HI_UNF_ENC_FMT_1080i_50:
        case HI_UNF_ENC_FMT_1080i_60:
        case HI_UNF_ENC_FMT_1080P_24:
        case HI_UNF_ENC_FMT_1080P_25:
        case HI_UNF_ENC_FMT_1080P_30:
        case HI_UNF_ENC_FMT_1080P_50:
        case HI_UNF_ENC_FMT_1080P_60:
            stLayerInfo.ScreenWidth = 1920;
            stLayerInfo.ScreenHeight = 1080;
            break;

        case HI_UNF_ENC_FMT_720P_50:
        case HI_UNF_ENC_FMT_720P_60:
            stLayerInfo.ScreenWidth = 1280;
            stLayerInfo.ScreenHeight = 720;
            break;

        case HI_UNF_ENC_FMT_480P_60:
        case HI_UNF_ENC_FMT_NTSC:
        case HI_UNF_ENC_FMT_NTSC_J:
        case HI_UNF_ENC_FMT_NTSC_PAL_M:
            stLayerInfo.ScreenWidth = 720;
            stLayerInfo.ScreenHeight = 480;
            break;

        default:
            stLayerInfo.ScreenWidth = 1280;
            stLayerInfo.ScreenHeight = 720;
            break;
    }

    /*
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.AntiLevel = HIGO_LAYER_DEFLICKER_HIGH;
    stLayerInfo.PixelFormat = szPixelFormat;
    stLayerInfo.LayerID = HIGO_LAYER_SD_0;
    */

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;//HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;//HIGO_LAYER_DEFLICKER_HIGH;
    stLayerInfo.PixelFormat = szPixelFormat;
    stLayerInfo.LayerID = HIGO_LAYER_HD_0;

#else
    stLayerInfo.ScreenWidth = 1280;
    stLayerInfo.ScreenHeight = 720;
    stLayerInfo.CanvasWidth = 1280;
    stLayerInfo.CanvasHeight = 720;
    stLayerInfo.DisplayWidth = 1280;
    stLayerInfo.DisplayHeight = 720;

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;//HIGO_LAYER_FLUSH_NORMAL;
    stLayerInfo.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;//HIGO_LAYER_DEFLICKER_HIGH;
    stLayerInfo.PixelFormat = szPixelFormat;
    stLayerInfo.LayerID = HIGO_LAYER_HD_0;
#endif
    ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != ret)
    {
        HI_GO_Deinit();
        LOADER_ASSERT("failed to create the layer sd 0, ret = 0x%x !\n", ret);
        return HI_FAILURE;
    }

    ret =  HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if  (HI_SUCCESS != ret)
    {
        HI_GO_DestroyLayer(hLayer);
        HI_GO_Deinit();
        LOADER_ASSERT("failed to get layer surface! s32Ret = 0x%x \n", ret);
        return HI_FAILURE;
    }
    else
    {
        LOADER_PROMPT("GetLayerSurface success! hLayerSurface=%d\n", hLayerSurface);
    }

    ret = HI_GO_FillRect(hLayerSurface,NULL,BACK_COLOR,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
       LOADER_ASSERT("HI_GO_FillRect failed!\n");
       HI_GO_DestroyLayer(hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }

    ret = HI_GO_RefreshLayer(hLayer,NULL);
    if (HI_SUCCESS != ret)
    {
       LOADER_ASSERT("++++ HI_GO_RefreshLayer failed! hLayerSurface %d ret 0x%x\n",hLayerSurface,ret);
       HI_GO_DestroyLayer(hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }

    ret = HI_GO_CreateText("./res/CourierNew-24-ascii.vbf", "./res/SongTi-22-gb2312.vbf", &hFont);

    if (HI_SUCCESS != ret)
    {
       LOADER_ASSERT("HI_GO_CreateText failed! ret = 0x%x \n", ret);
       HI_GO_DestroyLayer(hLayer);
       HI_GO_Deinit();
       HIADP_Disp_DeInit();
       return HI_FAILURE;
    }


    /* OSD driver init */
    g_loader_osd_init = 1;

    return HI_SUCCESS;
}

static HI_S32 Drv_OSDFillRect(HI_S32 StartX, HI_S32 StartY,
                       HI_S32 height, HI_S32 width, HI_U32 color)
{
    HI_S32 ret;
    HI_RECT rect ={0};

    if ((StartX > APPOSD_MAX_WIDTH_PAL)
        || (StartY > APPOSD_MAX_HEIGHT_PAL)
        || ((StartY + height) > APPOSD_MAX_HEIGHT_PAL)
        || ((StartX + width) > APPOSD_MAX_WIDTH_PAL))
    {
        LOADER_ASSERT("[DRV]OSD input Rect params invalid!\n");
        return HI_FAILURE;
    }
    rect.x = StartX;
    rect.y = StartY;
    rect.h = height;
    rect.w = width;
    ret = HI_GO_FillRect(hLayerSurface,&rect,color,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("Drv_OSDFillRect:HI_GO_FillRect failed! rect (%d %d %d %d)\n",rect.x,rect.y,rect.w,rect.h);
        return ret;
    }

    return HI_SUCCESS;
}

HI_VOID Drv_RectShowText(const HI_CHAR *CharBuffer, HI_S32 StartX, HI_S32 StartY)
{
    HI_S32 ret = 0;
    HI_U32 u32len = 0;
    HI_RECT rc={0};
    rc.x = StartX;
    rc.y = StartY;

    ret = HI_GO_GetTextExtent(hFont, CharBuffer, &rc.w, &rc.h);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_GetTextExtent line %d \n",__LINE__);
        return;
    }
#if 1
    u32len = strlen(CharBuffer);
    if(u32len < 5)
        rc.w = 16 * 5;
    else
        rc.w = 16 * 15;
    //rc.h = GRPH_FONT_SIZE;

    ret = HI_GO_FillRect(hLayerSurface,&rc,BLUE_COLOR, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ret)
    {
         LOADER_ASSERT("failed to HI_GO_FillRect\n");
         return;
    }
#endif

    ret = HI_GO_TextOut(hFont, hLayerSurface, CharBuffer, &rc);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_TextOut line %d  ret=%x\n",__LINE__, ret);
        return;
    }

    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("HI_GO_RefreshLayer\n");
        return ;
    }

    return;
}

HI_VOID Drv_UpdateIndictorText(LOADER_Content_E enContent)
{
	const HI_CHAR *pCharBuffer = NULL;

#ifdef CONFIG_SUPPORT_CA_RELEASE
    if( LOADER_CONTENT_PT_SUCC != enContent )
    {
        return;
    }
#endif
	pCharBuffer = LoaderOSDGetText(enContent);

	if (HI_NULL != pCharBuffer)
	{
    	Drv_RectShowText(pCharBuffer,
                     WINDOW_START_X + 120,
                     WINDOW_START_Y + 100 );
	}

	return;
}

HI_U32 Drv_CreatGuageWin(HI_VOID)
{
    HI_U32 testWind = 0;
    HI_S32 ret = HI_FAILURE;
    sPositionXY Pos   = {0};

    Pos.x = WINDOW_START_X;//150;//
    Pos.y = WINDOW_START_Y;//150;//

    testWind   = WIN_CreateWind(Pos, 400, 300, APP_WIND_NOTES);
	if( HI_NULL == testWind)
	{
        LOADER_ASSERT("[%s-->%d] fail!\n",__FUNCTION__,__LINE__);
		return HI_INVALID_HANDLE;
	}
    ret = Drv_ShowText(testWind, LOADER_CONTENT_PROMPT_POWER1, 10, 5, APP_WIN_TXT_COLOR);
    ret = Drv_ShowText(testWind, LOADER_CONTENT_PROMPT_POWER2, 10, 30, APP_WIN_TXT_COLOR);

#ifndef CONFIG_SUPPORT_CA_RELEASE
    ret += Drv_ShowText(testWind, LOADER_CONTENT_NT_CUR_PRGRSS, 20, 180, APP_WIN_TXT_COLOR);
    ret += Drv_ShowText(testWind, LOADER_CONTENT_NT_TTL_PRGRSS, 20, 260, APP_WIN_TXT_COLOR);
#endif

    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("Win fill rect fail!\n");
		goto returnfail;
    }

    ret = WIN_BuildWindPic(testWind);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_BuildWindPic fail!\n");
		goto returnfail;
    }

    ret = WIN_PasteWind(testWind, hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_PasteWind fail! ret = %d\n", ret);
		goto returnfail;
    }

    ret = WIN_ShowWind(testWind);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_ShowWind fail!\n");
		goto returnfail;
    }
	
	return testWind;

returnfail:
	if(testWind)
	{
		if(((sWindow *)testWind)->hMemSurface )				
			HI_GO_FreeSurface(((sWindow *)testWind)->hMemSurface );
		free((sWindow *)testWind);
	}
	
	return HI_INVALID_HANDLE;
 
}

HI_S32 Drv_ShowDoubleGuage(HI_U32 u32Percent1, HI_U32 u32Percent2)
{
#ifndef CONFIG_SUPPORT_CA_RELEASE
    HI_S32 ret = HI_FAILURE;
    HI_CHAR CharBuffer1[10] = {0};
    HI_CHAR CharBuffer2[10] = {0};

    if ((u32Percent1 > 100) || (u32Percent2 > 100))
    {
        return HI_FAILURE;
    }

    if (0 == u32Percent1)
    {
        ret = Drv_OSDFillRect(WINDOW_START_X + 10,
                              WINDOW_START_Y + 160,
                              20,
                              300,
                              APP_PROGREE_BGCOLOR);
        if (HI_SUCCESS != ret)
        {
            LOADER_ASSERT("u32Percent1 : Drv_OSDFillRect fail!\n");
            return HI_FAILURE;
        }
    }

    if (0 == u32Percent2)
    {
        ret = Drv_OSDFillRect(WINDOW_START_X + 10,
                              WINDOW_START_Y + 240,
                              20,
                              300,
                              APP_PROGREE_BGCOLOR);
        if (HI_SUCCESS != ret)
        {
            LOADER_ASSERT("u32Percent2 : Drv_OSDFillRect fail!\n");
            return HI_FAILURE;
        }
    }
    
    if(u32Percent1 != 0)
    {
        ret  = Drv_OSDFillRect(WINDOW_START_X + 10,
                               WINDOW_START_Y + 160,
                               20,
                               u32Percent1 * 3,
                               0xFFFFFF00);
        if (HI_SUCCESS != ret)
        {
            LOADER_ASSERT("u32Percent1*3 : Drv_WinFillRect fail! ret 0x%x\n",ret);
            return HI_FAILURE;
        }
    }

    if(u32Percent2 != 0)
    {
        ret = Drv_OSDFillRect(WINDOW_START_X + 10,
                              WINDOW_START_Y + 240,
                              20,
                              u32Percent2 * 3,
                              0xFFFFFF00);
        if (HI_SUCCESS != ret)
        {
            LOADER_ASSERT("u32Percent2*3 : Drv_WinFillRect fail!\n");
            return HI_FAILURE;
        }
    }

    sprintf(CharBuffer1, "%3d%%", u32Percent1);
    sprintf(CharBuffer2, "%3d%%", u32Percent2);

    Drv_RectShowText(CharBuffer1, 460, WINDOW_START_Y + 160);
    Drv_RectShowText(CharBuffer2, 460, WINDOW_START_Y + 240);

    ret = HI_GO_RefreshLayer(hLayer, NULL);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("HI_GO_RefreshLayer\n");
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

HI_VOID Drv_OSDClear(HI_VOID)
{
    HI_GO_FillRect(hLayerSurface,NULL,BACK_COLOR,HIGO_COMPOPT_NONE);
}
//HI_S32 Drv_ShowText(HI_U32 WindHandle, int enContent, HI_S32 StartX, HI_S32 StartY, HI_U32 FontColor)
//{}
#if 1
HI_S32 Drv_ShowText(HI_U32 WindHandle, LOADER_Content_E enContent, HI_S32 StartX, HI_S32 StartY, HI_U32 FontColor)
{
    HI_S32 ret = HI_FAILURE;
    HI_RECT rc = {0};
    sWindow *pWind = (sWindow *)WindHandle;
	const HI_CHAR *pCharBuffer = HI_NULL;

	pCharBuffer = LoaderOSDGetText(enContent);

    if ((0 == WindHandle) || (HI_NULL == pCharBuffer))
    {
        LOADER_ASSERT("input parameter invalid!\n");
        return HI_FAILURE;
    }

    ret =HI_GO_SetTextColor(hFont, FontColor);

    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_SetTextColor\n");
        return HI_FAILURE;
    }

    rc.x = StartX;
    rc.y = StartY;
    ret = HI_GO_GetTextExtent(hFont, pCharBuffer, &rc.w, &rc.h);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_GetTextExtent\n");
        return HI_FAILURE;
    }

    //HI_GO_SetTextStyle(hFont, HIGO_TEXT_STYLE_ITALIC|HIGO_TEXT_STYLE_BOLD);
    ret = HI_GO_TextOut(hFont, pWind->hMemSurface, pCharBuffer, &rc);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_TextOut\n");
        return HI_FAILURE;
    }

//    ret = HI_GO_RefreshLayer(hLayer, NULL);
//    if (HI_SUCCESS != ret)
//    {
//        printf("HI_GO_RefreshLayer\n");
//        return HI_FAILURE;
//    }
    return HI_SUCCESS;
}
#endif
HI_S32 Drv_ShowTextEx(HI_U32 WindHandle, const HI_CHAR *pCharBuffer, HI_S32 StartX, HI_S32 StartY, HI_U32 FontColor)
{
    HI_S32 ret = HI_FAILURE;
    HI_RECT rc = {0};
    sWindow *pWind = (sWindow *)WindHandle;

    if ((0 == WindHandle) || (HI_NULL == pCharBuffer))
    {
        LOADER_ASSERT("input parameter invalid!\n");
        return HI_FAILURE;
    }

    ret =HI_GO_SetTextColor(hFont, FontColor);

    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_SetTextColor\n");
        return HI_FAILURE;
    }

    rc.x = StartX;
    rc.y = StartY;
    ret = HI_GO_GetTextExtent(hFont, pCharBuffer, &rc.w, &rc.h);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_GetTextExtent\n");
        return HI_FAILURE;
    }

    //HI_GO_SetTextStyle(hFont, HIGO_TEXT_STYLE_ITALIC|HIGO_TEXT_STYLE_BOLD);
    ret = HI_GO_TextOut(hFont, pWind->hMemSurface, pCharBuffer, &rc);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("failed to HI_GO_TextOut\n");
        return HI_FAILURE;
    }

//    ret = HI_GO_RefreshLayer(hLayer, NULL);
//    if (HI_SUCCESS != ret)
//    {
//        printf("HI_GO_RefreshLayer\n");
//        return HI_FAILURE;
//    }
    return HI_SUCCESS;
}

HI_S32 Drv_DownLoadFail(LOADER_Content_E enContent)
{
    HI_U32 testWind = 0;
    HI_S32 ret = HI_FAILURE;
    sPositionXY Pos = {0};

    Pos.x = WINDOW_START_X;//150;
    Pos.y = WINDOW_START_Y;

    Drv_OSDClear();
    testWind = WIN_CreateWind(Pos, 420, 300, APP_WIND_NOTES);

#ifdef CONFIG_SUPPORT_CA_RELEASE
    switch(enContent)
    {
       case LOADER_CONTENT_DWNLD_WTING:
       case LOADER_CONTENT_PT_USB_PRGRSS:
       case LOADER_CONTENT_FAIL_UPGRD:
                break;

       default:
            enContent = LOADER_CONTENT_FAIL_UPGRD;
            break;
    }
#endif

    ret = Drv_ShowText(testWind, enContent, 5, 110, APP_WIN_TXT_COLOR);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("Win fill rect fail!\n");
		goto returnFail;
    }

    ret = WIN_BuildWindPic(testWind);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_BuildWindPic fail!\n");
		goto returnFail;
    }

    ret = WIN_PasteWind(testWind, hLayerSurface);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_PasteWind fail!\n");
		goto returnFail;
    }

    ret = WIN_ShowWind(testWind);
    if (HI_SUCCESS != ret)
    {
        LOADER_ASSERT("WIN_ShowWind fail!\n");
		goto returnFail;
    }
	
	if( HI_INVALID_HANDLE != testWind )
	{
		if(((sWindow *)testWind)->hMemSurface )				
			HI_GO_FreeSurface(((sWindow *)testWind)->hMemSurface );
		free((sWindow *)testWind);
	}

    return HI_SUCCESS;
	
returnFail:
	
	if( HI_INVALID_HANDLE != testWind )
	{
		if(((sWindow *)testWind)->hMemSurface )				
			HI_GO_FreeSurface(((sWindow *)testWind)->hMemSurface );
		free((sWindow *)testWind);
	}
	
	return HI_FAILURE;
}
#if 1
const HI_CHAR * LoaderOSDGetText(LOADER_Content_E enContent)
{
	if ((enContent < LOADER_CONTENT_MAN_UPGRD) || (enContent >= LOADER_CONTENT_MAX))
	{
        printf("\n[LoaderOSDGetText] Error enContent[%d]!!", enContent);
		return HI_NULL;
	}
	else
	{
		HI_U8  u8Value = 0;
		HI_S32 u32Ret = Upgrd_GetLanguage(&u8Value);

		if ((HI_SUCCESS == u32Ret) && (HI_UI_LANG_EN == u8Value))
		{
			return pENG_Str[enContent];
		}
		else
		{
			return pCHN_Str[enContent];
		}
	}
}

static HI_S32 Upgrd_GetLanguage(HI_U8 *pu8Value)
{
	//HI_LOADER_OTA_INFO_S stOTAInfo;

	if(HI_NULL == pu8Value)
	{
		return HI_FAILURE;
	}
#if 0
	if (HI_SUCCESS == HI_LOADER_ReadOTAInfo(&stOTAInfo))
	{
	    *pu8Value = stOTAInfo.enUILang;
    }
    else
    {
        *pu8Value = HI_UI_LANG_EN;
    }
#else
	*pu8Value = HI_UI_LANG_CN;
#endif
	return HI_SUCCESS;
}
static HI_S32 Upgrd_ReadBaseParamInfo(UPGRD_BASEPARAM_INFO_S *pstParamInfo)
{}
#if 0
static HI_S32 Upgrd_ReadBaseParamInfo(UPGRD_BASEPARAM_INFO_S *pstParamInfo)
{
    HI_U8 *pucBuf;
    UPGRD_BASEPARAM_INFO_S *pstTempInfo = NULL;
    HI_U32 u32FlashHandle = HI_INVALID_HANDLE;
    HI_U32 u32FlashBlockSize = 0;

    if (NULL == pstParamInfo )
    {
        LOADER_ASSERT("Invaled params.\n");
        return HI_FAILURE;
    }

    u32FlashHandle = HI_FLASH_Open(UpgrdPro_GetFlashTypeOfPatition(LOADER_BASEPARAM), LOADER_BASEPARAM, (HI_U64)0, (HI_U64)0);
    if( HI_INVALID_HANDLE == u32FlashHandle)
    {
        LOADER_ASSERT("Failed to open partition '%s' \n", LOADER_BASEPARAM);
        return HI_FAILURE;
    }

    if(0 > HI_FLASH_Ioctl(u32FlashHandle, HI_FLASH_CMD_GET_ERASE_SIZE, &u32FlashBlockSize)
        ||u32FlashBlockSize > 0x80000)
    {
        HI_FLASH_Close(u32FlashHandle);
        return HI_FAILURE;
    }

    HI_FLASH_Close(u32FlashHandle);
    pucBuf = malloc(u32FlashBlockSize);
    if(NULL == pucBuf)
    {
        SAVE_FREE(pucBuf);
        return HI_FAILURE;
    }

    if (0 >= HI_FLASH_Read_Block(LOADER_BASEPARAM,  pucBuf, u32FlashBlockSize))
    {
        LOADER_ASSERT("Failed to read flash block '%s'. \n", LOADER_BASEPARAM);
        SAVE_FREE(pucBuf);
        return HI_FAILURE;
    }

    LOADER_PROMPT("HI_LOADER_ReadBaseParamInfo  OK\r\n");

    pstTempInfo = (UPGRD_BASEPARAM_INFO_S *)pucBuf;
    if( DEF_CHECK_FLAG != pstTempInfo->checkflag)
    {
        LOADER_ASSERT("Invalid basepara flag.\n");
        SAVE_FREE(pucBuf);
        return HI_FAILURE;
    }

    memcpy(pstParamInfo, pstTempInfo, sizeof(*pstParamInfo));
    SAVE_FREE(pucBuf);
    return HI_SUCCESS;
}


void DRV_ShowUpgradeTypeAndVersion(HI_CHAR *UpdateType)
{
	HI_U8 Title[40] = {0};

	strcpy(Title,UpdateType);
	strcat(Title,(char*)GetVersionString());
	
    Drv_RectShowText(Title,
                 WINDOW_START_X + 120,
                 WINDOW_START_Y + 10 );
}
#endif
#endif
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

