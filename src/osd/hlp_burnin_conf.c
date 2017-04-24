/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_burnin.h>
#include <hlp_log.h>
#include "hlp_burnin_conf.h"
#include "hlp_osd.h"

#define ERASE_PATION_SELECT 5
#define ERASE_E2P_SELECT 4
#define NO_BUTTON_SELECT 3
#define UPDATA_CANCEL 2
#define UPDATE_SELECT 1
#define EXIT_SELECT 0

#define MAX_MEMER_LIST 8

extern hlp_burnin_conf_t  g_burnin_conf;

typedef enum hi_eKeyPress  
{
    APP_KEY_NUM0 = 0,
    APP_KEY_NUM1,
    APP_KEY_NUM2,
    APP_KEY_NUM3,
    APP_KEY_NUM4,
    APP_KEY_NUM5,
    APP_KEY_NUM6,
    APP_KEY_NUM7,
    APP_KEY_NUM8,
    APP_KEY_NUM9,
    APP_KEY_UP,
    APP_KEY_DOWN,
    APP_KEY_LEFT,
    APP_KEY_RIGHT,
    APP_KEY_MENU,
    APP_KEY_OK,
    APP_KEY_EXIT,
    APP_KEY_UNKNOWN
}eKeyPress;

/*--------------------------------*
 *作用：转换遥控器按键（九州遥控器）
 *返回：定义的遥控器按键枚举
 *--------------------------------*/
eKeyPress JZ_get_irkey_press(HI_U64 RawKey)
{
	printf("-----------  0x%llx  -----------\r\n",RawKey);
    switch (RawKey)
    {
		case 0x303058595a47LL:
		case 0xff005da0://jiuzhou RMC-C102
		case 0x303058595a53://苏州
		case 0xef107748://浙江省网
		case 0xcf30f601:
		case 0xd144412e47LL://广东
		case 0x9164416e67LL://广东
		case 0xb6498d20://大连
		case 0xed12af58://吴江
		case 0xec13f801://南昌
		case 0xb24de608://大庆
			return APP_KEY_NUM0;
		
		case 0x313058595a47LL:
		case 0xfe015da0://jiuzhou RMC-C102
		case 0x313058595a53://苏州
		case 0xee117748://浙江省网
		case 0xce31f601:
		case 0xd044412f47LL://广东
		case 0x9064416f67LL://广东
		case 0xec138d20://大连
		case 0xf609af58://吴江
		case 0xff00f801://南昌
		case 0xbd42e608://大庆
			return APP_KEY_NUM1;
		
		case 0x323058595a47LL:
		case 0xfd025da0://jiuzhou RMC-C102
		case 0x323058595a53://苏州
		case 0xed127748://浙江省网
		case 0xcd32f601:
		case 0xcf44413047LL://广东
		case 0x8f64417067LL://广东
		case 0xef108d20://大连
		case 0xe21daf58://吴江
		case 0xf807f801://南昌
		case 0xbe41e608://大庆
			return APP_KEY_NUM2;
			
		case 0x333058595a47LL:
		case 0xfc035da0://jiuzhou RMC-C102
		case 0x333058595a53://苏州
		case 0xec137748://浙江省网
		case 0xcc33f601:
		case 0xce44413147LL://广东
		case 0x8e64417167LL://广东
		case 0xee118d20://大连
		case 0xe01faf58://吴江
		case 0xf906f801://南昌
		case 0xbf40e608://大庆
			return APP_KEY_NUM3;
		
		case 0x343058595a47LL:
		case 0xfb045da0://jiuzhou RMC-C102
		case 0x343058595a53://苏州
		case 0xeb147748://浙江省网
		case 0xcb34f601:
		case 0xcd44413247LL://广东
		case 0x8d64417267LL://广东
		case 0xf00f8d20://大连
		case 0xf20daf58://吴江
		case 0xfb04f801://南昌
		case 0xb946e608://大庆
			return APP_KEY_NUM4;
			
		case 0x353058595a47LL:
		case 0xfa055da0://jiuzhou RMC-C102
		case 0x353058595a53://苏州
		case 0xea157748://浙江省网
		case 0xca35f601:
		case 0xcc44413347LL://广东
		case 0x8c64417367LL://广东
		case 0xf30c8d20://大连
		case 0xe619af58://吴江
		case 0xf40bf801://南昌
		case 0xba45e608://大庆
			return APP_KEY_NUM5;
			
		case 0x363058595a47LL:
		case 0xf9065da0://jiuzhou RMC-C102
		case 0x363058595a53://苏州
		case 0xe9167748://浙江省网
		case 0xc936f601:
		case 0xcb44413447LL://广东
		case 0x8b64417467LL://广东
		case 0xf20d8d20://大连
		case 0xe41baf58://吴江
		case 0xf50af801://南昌
		case 0xbb44e608://大庆
			return APP_KEY_NUM6;
			
		case 0x373058595a47LL:
		case 0xf8075da0://jiuzhou RMC-C102
		case 0x373058595a53://苏州
		case 0xe8177748://浙江省网
		case 0xc837f601:
		case 0xca44413547LL://广东
		case 0x8a64417567LL://广东
		case 0xf40b8d20://大连
		case 0xee11af58://吴江
		case 0xf708f801://南昌
		case 0xb54ae608://大庆
			return APP_KEY_NUM7;
			
		case 0x383058595a47LL:
		case 0xf7085da0://jiuzhou RMC-C102
		case 0x383058595a53://苏州
		case 0xe7187748://浙江省网
		case 0xc738f601:
		case 0xc944413647LL://广东
		case 0x8964417667LL://广东
		case 0xf7088d20://大连
		case 0xea15af58://吴江 
		case 0xf00ff801://南昌
		case 0xb649e608://大庆
			return APP_KEY_NUM8;
			
		case 0x393058595a47LL:
		case 0xf6095da0://jiuzhou RMC-C102
		case 0x393058595a53://苏州
		case 0xe6197748://浙江省网
		case 0xc639f601:
		case 0xc844413747LL://广东
		case 0x8864417767LL://广东
		case 0xf6098d20://大连
		case 0xe817af58://吴江
		case 0xf10ef801://南昌
		case 0xb748e608://大庆
			return APP_KEY_NUM9;
			
		case 0x505558595a47LL:
		case 0xee115da0://jiuzhou RMC-C102
		case 0x505558595a53://苏州
		case 0xff007748://浙江省网
		case 0xbb44f601:
		case 0xe144411e47LL://广东
		case 0xa164415e67LL://广东
		case 0xbb448d20://大连
		case 0xfc03af58://吴江
		case 0xe817f801://南昌
		case 0xeb14e608://大庆
			return APP_KEY_UP;
			
		case 0x4f4458595a47LL:
		case 0xed125da0://jiuzhou RMC-C102
		case 0x4f4458595a53://苏州
		case 0xfe017748://浙江省网
		case 0xba45f601:
		case 0xe044411f47LL://广东
		case 0xa064415f67LL://广东
		case 0xe21d8d20://大连
		case 0xfd02af58://吴江
		case 0xe51af801://南昌
		case 0xe916e608://大庆
			return APP_KEY_DOWN;
			
		case 0x454c58595a47LL:
		case 0xec135da0://jiuzhou RMC-C102
		case 0x454c58595a53://苏州
		case 0xfc037748://浙江省网
		case 0xb946f601:
		case 0xdf44412047LL://广东
		case 0x9f64416067LL://广东
		case 0xe31c8d20://大连
		case 0xf10eaf58://吴江
		case 0xbd42f801://南昌
		case 0xe21de608://大庆
			return APP_KEY_LEFT;
			
		case 0x495258595a47LL:
		case 0xeb145da0://jiuzhou RMC-C102
		case 0x495258595a53://苏州
		case 0xfd027748://浙江省网
		case 0xb847f601:
		case 0xde44412147LL://广东
		case 0x9e64416167LL://广东
		case 0xb7488d20://大连
		case 0xe51aaf58://吴江
		case 0xbe41f801://南昌
		case 0xee11e608://大庆
			return APP_KEY_RIGHT;
			
		case 0x4a5a58595a47LL:
		case 0xe01f5da0://jiuzhou RMC-C102
		case 0x454d58595a53://苏州
		case 0xf8077748://浙江省网
		case 0xbf40f601:
		case 0xe744411847LL://广东
		case 0xa764415867LL://广东
		case 0xfc038d20://大连
		case 0xb748af58://吴江
		case 0xb14ef801://南昌		
		case 0xed12e608://大庆
			return APP_KEY_MENU;
			
		case 0x4e4558595a47LL:
		case 0xea155da0://jiuzhou RMC-C102
		case 0x4e4558595a53://苏州
		case 0xe01f7748://浙江省网
		case 0xbc43f601:
		case 0xdd44412247LL://广东
		case 0x9d64416267LL://广东
		case 0xa35c8d20://大连
		case 0xf807af58://吴江
		case 0xea15f801://南昌
		case 0xea15e608://大庆
			return APP_KEY_OK;
			
		case 0x415058595a47LL:
		case 0xbf405da0://jiuzhou RMC-C102
		case 0x584558595a53://苏州
		case 0xe21d7748://浙江省网
		case 0xe41bf601:
		case 0xe244411d47LL://广东
		case 0xa264415d67LL://广东
		case 0xf8078d20://大连
		case 0xa35caf58://吴江
		case 0xe916f801://南昌		
		case 0xa25de608://大庆
			return APP_KEY_EXIT;
		
	    default:
			return APP_KEY_UNKNOWN;
    }
}


/*--------------------------------*
*作用：转换遥控器按键（九州前面板）
*返回：定义的遥控器按键枚举
*--------------------------------*/
eKeyPress JZ_get_pankey_press(HI_U32 RawKey)
{
    switch (RawKey)
    {
    case 6:
        return APP_KEY_UP;
    case 3:
        return APP_KEY_DOWN;
    case 4:
        return APP_KEY_LEFT;
    case 2:
        return APP_KEY_RIGHT;
    case 7:
        return APP_KEY_OK;
    case 5:
        return APP_KEY_EXIT;
    default:
        return APP_KEY_UNKNOWN;
    }
}


/*
 *Update window pic according to window member list and focus
 */
static HI_S32 WIN_ProcessWindKey(eKeyPress KeyId, HI_U32 WindHandle, HI_U32 *pIfUpdate)
{
    sWindow *pWind = (sWindow *)WindHandle;
    sWindMember *pMembTmp = pWind->pMemberList;
    sWindMember *pMembPre = pWind->pMemberList;
    const HI_CHAR *QamString[5] = {"5s ", "10s", "20s", "30s", "60s"};
    const HI_CHAR *pTempString  = {UPGRD_L_CN_TRANS_EXIT};
    const HI_CHAR *pDownload[6] = { UPGRD_L_CN_TRANS_NO, UPGRD_L_CN_TRANS_YES};

    if ((0 == WindHandle) || (HI_NULL == pIfUpdate))
    {
        printf("input ptr invalid!\n");
        return HI_FAILURE;
    }

	pTempString = LoaderOSDGetText(LOADER_CONTENT_TRANS_EXIT);
	pDownload[0] = LoaderOSDGetText(LOADER_CONTENT_TRANS_NO);
	pDownload[1] = LoaderOSDGetText(LOADER_CONTENT_TRANS_YES);
    *pIfUpdate = NO_BUTTON_SELECT;

    /*get current focus member*/
    if (HI_NULL == pWind->pMemberList)
    {
        printf("No focus, cannot respond any key input!\n");
        return HI_SUCCESS;
    }

    /* pMembPre point to the last member */
    while(HI_NULL != pMembPre->pNext)
    {
        pMembPre = pMembPre->pNext;
    }

    while(HI_NULL != pMembTmp)
    {
        if(HI_TRUE == pMembTmp->IsOnFocus)  /* member focus found */
        {
            if(APP_WMEMB_BUTTOM == pMembTmp->MembType) /* buttom member */
            {
                /* respond to direction and OK */
                if((APP_KEY_RIGHT == KeyId)||(APP_KEY_DOWN == KeyId))
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;

                    if (pMembTmp->pNext == HI_NULL)
                    {
                        pWind->pMemberList->IsOnFocus = HI_TRUE;
                    }
                    else
                    {
                        pMembTmp->pNext->IsOnFocus = HI_TRUE;
                    }

                    return HI_SUCCESS;
                }
                if((APP_KEY_LEFT == KeyId)||(APP_KEY_UP == KeyId))
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;
                    pMembPre->IsOnFocus = HI_TRUE;
                    return HI_SUCCESS;
                }

                /* OK to save input string and give message to main thread */
                if(APP_KEY_OK == KeyId)
                {
                    printf("Your choice:%s!\n", pMembTmp->pDefString);
                    if (*(pMembTmp->pDefString + 1) == *(pTempString + 1))
                    {
                        *pIfUpdate = EXIT_SELECT;
                    }
                    else
                    {
                        *pIfUpdate = UPDATE_SELECT;
                    }

                    return HI_SUCCESS;
                }

                return HI_SUCCESS;       /* no respondence */

            }

            if( APP_WMEMB_INPUT == pMembTmp->MembType)   /**** input string ****/
            {
                if ( APP_KEY_DOWN == KeyId )
                {
                    /* Text box page down function is disabled while in edit status .*/
                    if (HI_FALSE == pMembTmp->IsOnEdit)
                    {
                        /* focus trans */
                        pMembTmp->IsOnFocus = HI_FALSE;
                        pMembTmp->IsOnEdit = HI_FALSE;

                        if (pMembTmp->pNext == HI_NULL)
                        {
                            pWind->pMemberList->IsOnFocus = HI_TRUE;
                        }
                        else
                        {
                            pMembTmp->pNext->IsOnFocus = HI_TRUE;
                        }
                    }

                    /*  Decrease digit in edit status.*/
                    else                    {
                        char tmpNum = *(pMembTmp->pDefString + pMembTmp->SubFocusIndex);
                        if (tmpNum <= '0')
                        {
                            tmpNum = '9';
                        }
                        else
                        {
                            tmpNum--;
                        }

                        *(pMembTmp->pDefString + pMembTmp->SubFocusIndex) = tmpNum;
                        return HI_SUCCESS;
                    }

                    return HI_SUCCESS;
                }

                if (APP_KEY_UP == KeyId)
                {

                    /* Text box page down function is disabled while in edit status.*/
                    if (HI_FALSE == pMembTmp->IsOnEdit)
                    {
                        /* focus trans */
                        pMembTmp->IsOnFocus = HI_FALSE;
                        pMembPre->IsOnFocus = HI_TRUE;
                        pMembTmp->IsOnEdit = HI_FALSE;
                    }
                    else
                    {
                        char tmpNum = *(pMembTmp->pDefString + pMembTmp->SubFocusIndex);
                        if (tmpNum >= '9')
                        {
                            tmpNum = '0';
                        }
                        else
                        {
                            tmpNum++;
                        }

                        *(pMembTmp->pDefString + pMembTmp->SubFocusIndex) = tmpNum;
                        return HI_SUCCESS;
                    }

                    return HI_SUCCESS;
                }

                if (APP_KEY_LEFT == KeyId)
                {
                    if (pMembTmp->SubFocusIndex == 0)
                    {
                        return HI_SUCCESS;
                    }

                    pMembTmp->SubFocusIndex--;
                    return HI_SUCCESS;
                }

                if (APP_KEY_RIGHT == KeyId)
                {
                    if(pMembTmp->SubFocusIndex == (pMembTmp->TotalLen-1))
                    {
                        return HI_SUCCESS;
                    }

                    pMembTmp->SubFocusIndex++;
                    return HI_SUCCESS;
                }

                if (APP_KEY_OK == KeyId)
                {
                    if (HI_FALSE == pMembTmp->IsOnEdit)
                    {
                        pMembTmp->SubFocusIndex = 0;
                        pMembTmp->IsOnEdit = HI_TRUE;
                    }
                    else
                    {
                        pMembTmp->SubFocusIndex = 0;
                        pMembTmp->IsOnEdit = HI_FALSE;
                    }

                    return HI_SUCCESS;
                }

                if ((KeyId <= APP_KEY_NUM9) && (KeyId >= APP_KEY_NUM0))
                {

                    /* Text box page down function is disabled while in edit status.*/
                    if (HI_TRUE == pMembTmp->IsOnEdit)
                    {
                        *(pMembTmp->pDefString + pMembTmp->SubFocusIndex) = (0x30 + KeyId); //ascii code
                        if (pMembTmp->SubFocusIndex == (pMembTmp->TotalLen - 1))
                        {
                            return HI_SUCCESS;
                        }

                        pMembTmp->SubFocusIndex++;
                    }

                    return HI_SUCCESS;
                }

                return HI_SUCCESS;       /* no respondence */
            }

            if(APP_WMEMB_TRANS == pMembTmp->MembType) /* Transfer QAM stype */
            {
                if (APP_KEY_DOWN == KeyId)
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;

                    if (pMembTmp->pNext == HI_NULL)
                    {
                        pWind->pMemberList->IsOnFocus = HI_TRUE;
                    }
                    else
                    {
                        pMembTmp->pNext->IsOnFocus = HI_TRUE;
                    }

                    return HI_SUCCESS;

                }

                if (APP_KEY_UP == KeyId)
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;
                    pMembPre->IsOnFocus = HI_TRUE;
                    return HI_SUCCESS;
                }
                if (APP_KEY_LEFT == KeyId)
                {

					if (IntervalTime_5s == pMembTmp->IntervalNumber)
                    {
                        pMembTmp->IntervalNumber = IntervalTime_60s;
                    }
                    else
                    {
                        pMembTmp->IntervalNumber--;
                    }

                    memcpy(pMembTmp->pDefString, QamString[(HI_U8)pMembTmp->IntervalNumber], 4);

                    return HI_SUCCESS;
                }
                if (APP_KEY_RIGHT == KeyId)
                {
					if (IntervalTime_60s == pMembTmp->IntervalNumber)
                    {
                        pMembTmp->IntervalNumber = IntervalTime_5s;
                    }
                    else
                    {
                        pMembTmp->IntervalNumber++;
                    }
                    memcpy(pMembTmp->pDefString, QamString[(HI_U8)pMembTmp->IntervalNumber], 4);
                    return HI_SUCCESS;
                }
            }

            if (APP_WMEMB_TRANS_2 == pMembTmp->MembType) /* Transfer QAM stype */
            {
                if (APP_KEY_DOWN == KeyId)
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;

                    if (pMembTmp->pNext == HI_NULL)
                    {
                        pWind->pMemberList->IsOnFocus = HI_TRUE;
                    }
                    else
                    {
                        pMembTmp->pNext->IsOnFocus = HI_TRUE;
                    }

                    return HI_SUCCESS;
                }

                if (APP_KEY_UP == KeyId)
                {
                    /* focus trans */
                    pMembTmp->IsOnFocus = HI_FALSE;
                    pMembPre->IsOnFocus = HI_TRUE;
                    return HI_SUCCESS;
                }

                if ((APP_KEY_LEFT == KeyId) || (APP_KEY_RIGHT == KeyId))
                {
                    if (!memcmp(pMembTmp->pDefString, pDownload[0], 6))
                    {
                        memcpy(pMembTmp->pDefString, pDownload[1], 6);
                        pMembTmp->bAuto = 1; /* Auto upgrade -- yes */
                    }
                    else
                    {
                        memcpy(pMembTmp->pDefString, pDownload[0], 6);
                        pMembTmp->bAuto = 0; /* Auto upgrade -- no */
                    }

                    return HI_SUCCESS;
                }
            }
        }

        pMembTmp = pMembTmp->pNext;  /* check next window member */
        /* pMembPre move to next */
        if(HI_NULL != pMembPre->pNext)
        {
            pMembPre = pMembPre->pNext;
        }
        else
        {
            pMembPre = pWind->pMemberList;
        }
    }
    return HI_FAILURE;
}

HI_S32 hlp_osd_save_burnin_conf(HI_U32 testWind)
{
	HI_U32 u32Temp = 0;
	sWindow *pWindTemp = HI_NULL;
	sWindMember *pWinMemTemp  = HI_NULL;
	hlp_burnin_conf_t       *conf = &g_burnin_conf;

	pWindTemp   = (sWindow *)testWind;
	pWinMemTemp = pWindTemp->pMemberList;
	u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
	conf->total_time = u32Temp;
	
	pWinMemTemp = pWinMemTemp->pNext;
	u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
	conf->total_count = u32Temp;

	pWinMemTemp = pWinMemTemp->pNext;
	u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
	conf->interval_time = u32Temp;

	pWinMemTemp = pWinMemTemp->pNext;
	u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
	conf->capture_log = u32Temp;

	return 0;
}

HI_S32 osd_test(HI_VOID)
{
	HI_U32 testWind = 0;
	HI_S32 ret = HI_FAILURE;
	HI_U64 PressStatus = 0, KeyId = 0;
	HI_U8 Title[40] = {0};
	sPositionXY Pos    = {0};
	sWindow *pWindTemp = HI_NULL;
	sWindMember *pWinMemTemp  = HI_NULL;
	sWindMember *pWinMemTemp2 = HI_NULL;

	HI_U32 i = 0;
	HI_U32 IfUpdate = NO_BUTTON_SELECT;
	HI_U32 u32Temp = 0;


	HI_U32 u32Freq = 0;
	HI_U16 u16symborate = 0;
	HI_U16 u16TsPid   = 0x0;
	HI_U8 u8TableId   = 0;

	HI_U8 u8Version   = 0;
	HI_CHAR u8TmpBuf[7] = {0};


	Pos.x = 110;
	Pos.y = 80;
	/*************
	Pos.x = 380;
	Pos.y = 140;
	**************/
	testWind = WIN_CreateWind(Pos, 500, 420, APP_WIND_NOTES);

	strcpy(Title,"Brun-in Test");
	strcat(Title,(char*)"v1.0");

	(HI_VOID)Drv_ShowTextEx(testWind, Title, 140, 10, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "拷机时间", 50, 50, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "h  ", 330, 50, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "拷机次数", 50, 110, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "次", 330, 110, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "间隔时间", 50, 170, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "capture", 50, 230, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "unknown", 50, 290, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "<|", 272, 170, APP_WIN_TXT_COLOR);
	(HI_VOID)Drv_ShowTextEx(testWind, "|>", 400, 170, APP_WIN_TXT_COLOR);
	
	Pos.x = 300;
	Pos.y = 50;
	memset(u8TmpBuf, 0, 7);
	sprintf(u8TmpBuf, "%03d", u32Freq);
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_INPUT, Pos, u8TmpBuf, 2);

	Pos.x = 300;
	Pos.y = 110;
	memset(u8TmpBuf, 0, 7);
	sprintf(u8TmpBuf, "%04d", u16symborate);
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_INPUT, Pos, u8TmpBuf, 2);

	Pos.x = 300;
	Pos.y = 170;
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_TRANS, Pos, "10s", 6);

	Pos.x = 300;
	Pos.y = 230;
	memset(u8TmpBuf, 0, 7);
	sprintf(u8TmpBuf, "%04d", u16TsPid);
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_INPUT, Pos, u8TmpBuf, 2);

	Pos.x = 300;
	Pos.y = 290;
	memset(u8TmpBuf, 0, 7);
	sprintf(u8TmpBuf, "%3d", u8TableId);
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_INPUT, Pos, u8TmpBuf, 2);

	Pos.x = 100;//200;
	Pos.y = 350;//350;
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_BUTTOM, Pos, "开始", 6);

	Pos.x = 300;
	Pos.y = 350;
	(HI_VOID)WIN_CreateWindMember(testWind, APP_WMEMB_BUTTOM, Pos, "退出", 6);


	WIN_BuildAndPasteWind(testWind);
	(HI_VOID)WIN_ShowWind(testWind);

	/* process key input */
	printf("Enter IR Input...\n");
	(HI_VOID)HI_IR_Enable();
	
	while (1)
	{
	    /* Remote control input */
	    ret = HI_IR_GetValue(&PressStatus, &KeyId);
	    if (ret == HI_SUCCESS)
	    {
            eKeyPress KeyPress = JZ_get_irkey_press(KeyId);
            
	        if (KeyPress != APP_KEY_UNKNOWN)
	        {
	        	IfUpdate = NO_BUTTON_SELECT;
	            (HI_VOID)WIN_ProcessWindKey(KeyPress, testWind, &IfUpdate);
	            if ((UPDATE_SELECT == IfUpdate) )
	            {
	                hlp_osd_save_burnin_conf(testWind);
					hlp_burnin_print_config();
	            }
				else if (EXIT_SELECT == IfUpdate)
				{
					printf("exit osd!\n");
				}

	            /* update window */
	            WIN_BuildAndPasteWind(testWind);
	            (HI_VOID)WIN_ShowWind(testWind);
	        }
	    }

	}
#if 0
	if (UPDATE_SELECT == IfUpdate)
	{
		pWindTemp   = (sWindow *)testWind;
		pWinMemTemp = pWindTemp->pMemberList;
		u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
		g_stForUpGrdInfo.u32Frequency = u32Temp*1000; /* MHz->10KHz */
		g_stLoaderInfo.stOtaInfo.stTunerPara.unConnPara.stCab.u32OtaFreq = g_stForUpGrdInfo.u32Frequency;
		printf(UPGRD_L_CN_TITLE_FREQ": %d\n", u32Temp);

		pWinMemTemp = pWinMemTemp->pNext;
		u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
		g_stForUpGrdInfo.u32SymbolRate = u32Temp;
		g_stLoaderInfo.stOtaInfo.stTunerPara.unConnPara.stCab.u32OtaSymbRate = g_stForUpGrdInfo.u32SymbolRate;
		printf(UPGRD_L_CN_TITLE_SYM_RATE": %d\n", u32Temp);

		pWinMemTemp = pWinMemTemp->pNext;
		g_stForUpGrdInfo.enModulation = pWinMemTemp->QamNumber;
		g_stLoaderInfo.stOtaInfo.stTunerPara.unConnPara.stCab.u32OtaModulation = g_stForUpGrdInfo.enModulation;
		printf(UPGRD_L_CN_TITLE_MODULATE": %d\n", (HI_U32)g_stForUpGrdInfo.enModulation);

		pWinMemTemp = pWinMemTemp->pNext;
		u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
		g_stForUpGrdInfo.u16TsPID = u32Temp;
		g_stLoaderInfo.stOtaInfo.stTunerPara.unConnPara.stCab.u32OtaPid = g_stForUpGrdInfo.u16TsPID;
		printf("PID : %d\n", u32Temp);

		pWinMemTemp = pWinMemTemp->pNext;
		u32Temp = strtoul((HI_CHAR*)pWinMemTemp->pDefString, NULL, 10);
		g_stForUpGrdInfo.u8TableID = u32Temp;
		g_stLoaderInfo.stOtaInfo.stTunerPara.unConnPara.stCab.u32TableID = g_stForUpGrdInfo.u8TableID;
		printf("TABLE ID: %d\n", u32Temp);

	}
#endif
	pWindTemp   = (sWindow *)testWind;
	pWinMemTemp = pWindTemp->pMemberList;
	for (i = 0; i < MAX_MEMER_LIST; i++)
	{
		printf("pWinMemTemp->pDefString :%s \n", pWinMemTemp->pDefString);
		pWinMemTemp = pWinMemTemp->pNext;
		if (NULL == pWinMemTemp)
		{
			break;
		}
	}

	pWindTemp   = (sWindow *)testWind;
	pWinMemTemp = pWindTemp->pMemberList;
	for (i = 0; i < MAX_MEMER_LIST; i++)
	{
		free(pWinMemTemp->pDefString);
		pWinMemTemp2 = pWinMemTemp;
		pWinMemTemp = pWinMemTemp->pNext;
		if (NULL == pWinMemTemp)
		{
			break;
		}
		free(pWinMemTemp2);
	}

	pWindTemp = (sWindow *)testWind;
	//free(pWindTemp->pWindBuf);
	free(pWindTemp);

	if (UPDATE_SELECT == IfUpdate)
	{
		hlp_osd_save_burnin_conf(testWind);
		hlp_burnin_print_config();
		return UPDATE_SELECT;
	}
	else
	{
		return EXIT_SELECT;
	}
}


int dd()
{
	HI_IR_Open();
	Drv_OSDInit();
	osd_test();
	//adjf;
}


