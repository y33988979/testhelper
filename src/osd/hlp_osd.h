/******************************************************************************
                              Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : test_loader.h
Version       : Initial Draft
Author        : Hisilicon multimedia Hardware group l50549
Created       : 2007/08/20
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __UPDATE_OSD_H__
#define __UPDATE_OSD_H__

#if 0
#include "upgrd_internal.h"
#include "upgrd_identify.h"
#endif
//#include "hlp_core.h"
//#include "hlp_config.h"
#include "hi_type.h"
#include "upgrd_lang.h"


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4 */
/** @{ */  /** <!-- [UI display data structure] */


/** Micros Modifiable, not used in general cases */
#define     APPTUN_PORT            1
/** Maximum frequency */
#define     APPTUN_MAX_FREQ        858000
/** Minimum frequency */
#define     APPTUN_MIN_FREQ        45000
/** Default frequency */
#define     APPTUN_DEFAULT_FREQ    403000
/** Maximum symbol rate */
#define     APPTUN_MAX_SYMB        7200000
/** Minimum symbol rate */
#define     APPTUN_MIN_SYMB        2600000
/** Default symbol rate */
#define     APPTUN_DEFAULT_SYMB    6875000
/** 4: 256Qam */
#define     APPTUN_MAX_QAM         4
/** 0: 16Qam */
#define     APPTUN_MIN_QAM         0
/** 2: 64Qam */
#define     APPTUN_DEFAULT_QAM     2
/** 0: not converse */
#define     APPTUN_DEFAULT_SI      0
/** Maximum width */
#define     APPOSD_MAX_HEIGHT_PAL 576
/** Maximum length */
#define        APPOSD_MAX_WIDTH_PAL 704

#define    GRPH_COLOR_MODE RGB5551
/**24 x 24 for Asian characters and 16 x 24 for numerical characters and letters */
#define GRPH_FONT_SIZE 26//24
/** Total number of Asian characters */
#define CHN_CHR_NUM 128 + 4 + 11 + 9

//#define NO_BUTTON_SELECT 3
//#define UPDATA_CANCEL 2
//#define UPDATE_SELECT 1
//#define EXIT_SELECT 0

//#define GET_COLOR(r, g, b) ((0x1 << 15) + ((r) << 10) + ((g) << 5) + (b))    /* ARGB16 */
#define GET_COLOR(r, g, b) (0xff000000 | (((r) << 16) & 0x00ff0000) | (((g) << 8) & 0x0000ff00) | (b & 0xff))
//#define GET_COLOR(b, g, r) ((0xff << 24) + ((r) << 19) + ((g) << 11) + (b<<3))
#if 0
#define APP_WIND_BGCOLOR GET_COLOR(9, 20, 31) //(77,120,200)
#define APP_WIND_LCOLOR GET_COLOR(24, 30, 30)
#define APP_BG_COLOR GET_COLOR(0, 0, 0) //(199,200,203)
#define APP_WIND_BWIDTH 2
#define APP_TXT_FONTCOLOR_F GET_COLOR(0, 0, 8)
#define APP_TXT_BGCOLOR_F GET_COLOR(31, 16, 0)
#define APP_TXT_FONTCOLOR GET_COLOR(31, 31, 31)
#define APP_TXT_BGCOLOR GET_COLOR(0, 0, 31)
#define APP_WIN_TXT_BGCOLOR GET_COLOR(15, 15, 31)
#define APP_WIN_TXT_COLOR GET_COLOR(0, 0, 31)
#define APP_PROGREE_BGCOLOR GET_COLOR(24, 24, 24)
#define APP_WIN_TXT_RED GET_COLOR(0, 0, 31)

#else
#define APP_WIND_BGCOLOR GET_COLOR(72, 160, 248) //(77,120,200)
#define APP_WIND_LCOLOR GET_COLOR(192, 240, 240)
#define APP_BG_COLOR GET_COLOR(0, 0, 0) //(199,200,203)
#define APP_WIND_BWIDTH 2
#define APP_TXT_FONTCOLOR GET_COLOR(0, 6, 0)
#define APP_TXT_BGCOLOR 0x80FAFA00 //GET_COLOR(248, 128, 0)
#define APP_TXT_FONTCOLOR_F GET_COLOR(255, 255, 255)
#define APP_TXT_BGCOLOR_F  0x80FAFA00
#define APP_WIN_TXT_BGCOLOR GET_COLOR(120, 120, 248)
#define APP_WIN_TXT_COLOR GET_COLOR(248, 248, 248)
#define APP_PROGREE_BGCOLOR GET_COLOR(192, 192, 192)
#define APP_WIN_TXT_RED GET_COLOR(0, 0, 248)
#define BACK_COLOR 0xFF000000
#define BLUE_COLOR 0xFF0000FA
#define YELLOW_COLOR 0x80FAFA00

#endif

typedef struct hi_sPositionXY
{
    HI_S32 x;    /**< X axis*/
    HI_S32 y;    /**< Y axis*/
}sPositionXY;

typedef enum hi_eWindType
{
    APP_WIND_NOTES,     /**< Prompt window. For Digital Video Networks, a exclamation mark is added on the left.*/
    APP_WIND_INPUT      /**< A common window with a text box.*/
}eWindType;

typedef enum hi_eWMembType
{
    APP_WMEMB_INPUT,     /**< Text box*/
    APP_WMEMB_BUTTOM,    /**< Button*/
    APP_WMEMB_TRANS,     /**< Translation box*/
    APP_WMEMB_TRANS_2,
    APP_WMEMB_PASSWD,    /**< Password prompt*/
} eWMembType;

typedef enum IntervalTime
{
	IntervalTime_5s,
	IntervalTime_10s,
	IntervalTime_20s,
	IntervalTime_30s,
	IntervalTime_60s,
}IntervalTime_e;

typedef struct hi_sWindMember
{
    HI_U32      hMembHandle;   /**< To store the structure pointer as a window ID*/
    eWMembType  MembType;      /**< Window member type: message input or button*/
    HI_BOOL     IsOnFocus;     /**< Focus or not*/
    HI_BOOL     IsOnEdit;      /**<  Input status or not */
    sPositionXY MembPos;       /**< Relative position in the window*/
    HI_CHAR *     pDefString;   /**< Input character, not modifiable for a non-text box*/
    HI_U8       TotalLen;      /**< Maximum characters length allowed, available for text boxes*/
    HI_U8       SubFocusIndex; /**< Current focus index (relative position of the input character string of the current member)*/
    //CABLE_MOD_E QamNumber;     /**<Quadrature amplitude modulation (QAM) mode*/
    IntervalTime_e      IntervalNumber; //add by ychen
    HI_U32      u32Polar;
    HI_BOOL     bAuto;         /**<  Automatic download */
    struct hi_sWindMember *pNext; /**< Pointed to the next member*/
} sWindMember;

typedef struct hi_sWindow
{
    HI_U32       hWindHandle;   /**< To store the structure pointer as a window ID*/
    eWindType    WindType;      /**< Window type*/
    sPositionXY  WindPos;       /**< Position of a window in the region*/
    HI_S32       WindLength;    /**<Window width*/
    HI_S32       WindWidth;     /**<Window height*/
    HI_HANDLE    hMemSurface;   /**< Window picture buffer*/
    sPositionXY  FocusPos;      /**< Focus position of the current member (WindPos for members of inputting types)*/
    sWindMember *pMemberList;   /**< Member link table*/
} sWindow;

#if 0
typedef enum hi_eKeyPress   /* Keypress Type whose infrared responsed. */
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
    APP_KEY_UP ,
    APP_KEY_LEFT,
    APP_KEY_OK,
    APP_KEY_RIGHT,
    APP_KEY_MENU,
    APP_KEY_DOWN,
    APP_KEY_EXIT,
    APP_KEY_UNKNOWN
} eKeyPress;
#endif

typedef struct hi_sCode2Index
{
    unsigned short ChCode;       /**<  GB2312 code of a Chinese character */
    unsigned short Index;        /**<  GB2312 code library query index */
}sCode2Index;

typedef struct vo_osd_region_s
{
    unsigned int 		RegionLinkPhy;       /**< Character address */
    unsigned int		pu32ClutPhy;         /**< Bitmap address */
    unsigned int		pu32BitmapPhy;       /**< Bitmap address */
    unsigned char       u8Clutb;         /**< Not used */
    unsigned char       u8Rmix;          /**< Not used */
    unsigned short      BitmapMode:4;    /**< Not used */
    unsigned short      Reserved0:1;     /**< Not used */
    unsigned short      ClutReload:1;    /**< Not used */
    unsigned short      Reserved1:10;    /**< Not used */

    unsigned short      u16Width;   /**< Width */
    unsigned short      u16StX;     /**< Initial point of X axis */

    unsigned short      u16Height;  /**< Length */
    unsigned short      Reserved2;  /**< Not used */

    unsigned short      u16Pitch;   /**< Character length */
    unsigned short      u16StY;     /**< Initial point of Y axis */

    unsigned short	    u16StYP2N;  /**< Not used */
    unsigned short      Reserved3;  /**< Not used */
} VO_OSD_REGION_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */



/******************************* API declaration *****************************/
/** \addtogroup      H_1_5 */
/** @{ */  /** <!- [UI display function API]*/

//HI_S32 force_upgrade_cable_check(HI_VOID);
/**
\brief To create an upgrade progress UI
\attention \n
Background of two progress bars displayed during data download. The progress bars are not drawn.
\param N/A
\retval ::HI_SUCCESS  Success
\retval ::HI_FAILURE  Failure
\see ::sWindow\n
For details, see the definition in sWindow.
*/
HI_U32 Drv_CreatGuageWin(HI_VOID);

/**
\brief Displaying the two progress bars during upgrade
\attention \n
This API is used to show the data download progress
\param[in] Progress percentages of two items
\retval ::HI_SUCCESS  Success
\retval ::HI_FAILURE  Failure
\see \n
N/A
*/

HI_S32 Drv_ShowDoubleGuage(HI_U32 u32Percent1, HI_U32 u32Percent2);

HI_S32 Drv_FailUpdateDisplay(HI_VOID);

/**
\brief Upgrading failure display UI
\attention \n
All the contradictions during the upgrade are displayed, such as the hardware and software contradiction, vendor ID contradiction, frequency unlocking, or IP contradiction.
\param[in] String.
\retval ::HI_SUCCESS  Prompt display UI success
\retval ::HI_FAILURE  Prompt display UI failure
\see \n
N/A
*/
//HI_S32 Drv_DownLoadFail(LOADER_Content_E enContent);

/**
\brief Initializes UI display
\attention \n
This API is used to enable the OSD interface
\param N/A
\retval ::HI_SUCCESS  Initialization success
\retval ::HI_FAILURE  Initialization failure
\see ::VO_OSD_REGION_S \n
For details, see the definition in VO_OSD_REGION_S.
*/
HI_S32 Drv_OSDInit(HI_VOID);

//HI_S32 Drv_CheckPara(VO_OSD_REGION_S *pRegion);
//HI_S32 Drv_WinDialogBox(HI_U8 *CharBuffer1, HI_U8 *CharBuffer2);


/**
\brief UI clear function
\attention \n
This API is used to clear the UI when no UI needs to be displayed.
\param N/A
\retval none
\see  \n
N/A
*/

HI_VOID Drv_OSDClear(HI_VOID);


/**
\brief UI display deinitialization function
\attention \n
This API is used to disable the OSD interface and release the memory.
\param N/A
\retval none
\see ::VO_OSD_REGION_S \n
For details, see the definition in VO_OSD_REGION_S.
*/

HI_VOID Drv_OSDDeInit(HI_VOID);

/**
\brief UI prompt function for download
\attention \n
This API is used to display a message indicating the device is searching data,
downloading data, or updating data when performing a data download task.
\param[in] String
\retval none
\see  \n
N/A
*/

//HI_VOID Drv_UpdateIndictorText(LOADER_Content_E enContent);
//UPGRD_MODE_E force_serial_upgrade_check(HI_VOID);

const HI_CHAR * LoaderOSDGetText(LOADER_Content_E enContent);

/** @} */  /** <!-- ==== API declaration end ==== */

//add by ychen
/*Define OSD UI language.*/
typedef enum tagUILang
{
    HI_UI_LANG_CN,
    HI_UI_LANG_EN,

    HI_UI_LANG_BUTT
}HI_UI_LANG_E;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__UPDATE_OSD_H__*/


