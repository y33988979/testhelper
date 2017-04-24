/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : osddrv.h

    Description :图形引擎驱动模块，主要实现对底层驱动的
    封装

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-12
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    最近更新:
        1. 2005-12-14 10:00 函数及头文件实现大部分重写，不再以stosd为基础.
        2. 修改ARGB_GET_A、ARGB_GET_R、ARGB_GET_G、ARGB_GET_B的宏定义，以避免出现警告
        3. 去除颜色相关宏.
    当前版本号:    
    version : O-a01-c-04.01 
    time    : 2005-12-20 


*******************************************************************************/

#ifndef _OSDDRV_H_
#define _OSDDRV_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include <stddefs.h>
#include "drv_typedef.h"
#include "drv_disp.h"
#include "hi_go_bliter.h"

/*----------------------------------------------------------------------------
 *	Macro
 *----------------------------------------------------------------------------*/
 
#define OSD_REGION_NAME_LENGTH  8

/*----------------------------------------------------------------------------
 *	Enum
 *----------------------------------------------------------------------------*/

enum
{
    DRV_OSD_ERROR_HANDLE = DRV_OSD_BASE + 1,
    DRV_OSD_ERROR_INIT,
    DRV_OSD_ERROR_OPEN_EVT,
    DRV_OSD_ERROR_SUBSCRIBE_EVT,
    DRV_OSD_ERROR_GET_BLOCKADDR,
    DRV_OSD_ERROR_GET_LAYER_PARAMS,
    DRV_OSD_ERROR_GET_BITMAP_PARAMS,
    DRV_OSD_ERROR_FREE_BLOCK,
    DRV_OSD_ERROR_VP_OPEN,
    DRV_OSD_ERROR_VP_GET_SOURCE,
    DRV_OSD_ERROR_VP_ENABLE,
    DRV_OSD_ERROR_VP_DISABLE,
    DRV_OSD_ERROR_VP_SET_ALPHA,
    DRV_OSD_ERROR_PALETTE_SET_COLOR,
    DRV_OSD_ERROR_BLIT_OPERATE,
    DRV_OSD_ERROR_SCHEDULE_FREE_BLOCK
};



typedef enum OSD_ColorType_e
{
    OSD_COLOR_TYPE_ARGB8888 = (int)0x80000000, //32  
    OSD_COLOR_TYPE_RGB888,     //24
    OSD_COLOR_TYPE_ARGB8565, //24
    OSD_COLOR_TYPE_RGB565,     //16
    OSD_COLOR_TYPE_ARGB1555, //16
    OSD_COLOR_TYPE_ARGB4444, //16

    OSD_COLOR_TYPE_CLUT8,
    OSD_COLOR_TYPE_CLUT4,
    
    OSD_COLOR_TYPE_YCBCR888_422,
    OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422
} OSD_ColorType_t;

typedef enum
{
  OSD_FILTER_TYPE_ANTI_FLUTTER = 1,
  OSD_FILTER_TYPE_ANTI_FLICKER = 2,
  OSD_FILTER_TYPE_NONE = 4
}OSD_FilterType_t;

typedef enum 
{
    OSD_DISPLAY_ASPECT_RATIO_16TO9  = 1,
    OSD_DISPLAY_ASPECT_RATIO_4TO3   = 2,
    OSD_DISPLAY_ASPECT_RATIO_221TO1 = 4,
    OSD_DISPLAY_ASPECT_RATIO_SQUARE = 8
} OSD_DisplayAspectRatio_t;


typedef enum 
{
    OSD_SCAN_TYPE_PROGRESSIVE = 1,
    OSD_SCAN_TYPE_INTERLACED  = 2
} OSD_ScanType_t;

/*每个region对应的是hisi中一个软surface的概念，不是用window*/
typedef enum 
{
    OSD_REGION_STILL = 0,
    OSD_REGION_OSD1,
    OSD_REGION_OSD2,
    OSD_REGION_OSD3,
    OSD_REGION_OSD4,
    OSD_REGION_OSD5,
    OSD_REGION_CURSOR1 = 12,
    OSD_REGION_CURSOR2,
    OSD_REGION_CURSOR3,
    OSD_REGION_SUPPORT = 15,
} OSD_RegionId_t;

/*----------------------------------------------------------------------------
 *  Struct
 *----------------------------------------------------------------------------*/



/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


typedef enum STOSD_BitmapType_e
{
    STOSD_BITMAP_TYPE_DIB            = 1,
    STOSD_BITMAP_TYPE_DIB_BYTE_ALIGN = 2
} STOSD_BitmapType_t;


typedef enum STOSD_ColorType_e
{
    STOSD_COLOR_TYPE_PALETTE  = 1,
    STOSD_COLOR_TYPE_YCRCB888 = 2,
    STOSD_COLOR_TYPE_ARGB4444,
    STOSD_COLOR_TYPE_ARGB1555,
    STOSD_COLOR_TYPE_RGB565,
    STOSD_COLOR_TYPE_YCBCR888_422,
    STOSD_COLOR_TYPE_ARGB8888
} STOSD_ColorType_t;


typedef struct STOSD_Bitmap_s
{
    STOSD_BitmapType_t BitmapType;
    STOSD_ColorType_t  ColorType;
    U8                 BitsPerPel;
    U32                Width;
    U32                Height;
    void*              Data_p;
} STOSD_Bitmap_t;                            //for  CLUT Bitmap convertion



typedef struct STOSD_ColorYCrCb888_s
{
    U8 Y;
    U8 Cr;
    U8 Cb;
} STOSD_ColorYCrCb888_t;

typedef struct
{

   STOSD_ColorType_t     Type;
   STOSD_ColorYCrCb888_t Value;
   U8                    MixWeight;
} CAST_OSD_Color_t;

typedef void* OSD_Palette_p;
typedef void* OSD_BitMap_p;//used in bgbmp.h

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

typedef struct 
{
    U8 Y;
    U8 Cr;
    U8 Cb;
} OSD_ColorYCrCb888_t;

typedef struct 
{
  U8 Alpha;
  U8 R;
  U8 G;
  U8 B;
} OSD_ColorARGB_t;

typedef struct 
{
  U8 R;
  U8 G;
  U8 B;
} OSD_ColorRGB_t;
typedef union OSD_ColorValue_u
{
    OSD_ColorARGB_t ARGB4444;
    OSD_ColorARGB_t ARGB1555;
    OSD_ColorARGB_t ARGB8888;
    OSD_ColorRGB_t   RGB565;
    OSD_ColorRGB_t   RGB888;
    OSD_ColorYCrCb888_t YCrCb888;
} OSD_ColorValue_t;


typedef struct OSD_Color_s
{
    OSD_ColorType_t  Type;
    OSD_ColorValue_t Value;
    U8                 MixWeight;
} OSD_Color_t;

typedef struct OSD_Palette_s
{
    OSD_ColorType_t  ColorType;
	U8               ColorDepth;
	void*            Data_p;
} OSD_Palette_t;

typedef struct 
{
    OSD_DisplayAspectRatio_t  Aspect;
    U16                         Width;
    U16                         Height;
    U8                          XOffset;
    U8                          YOffset;
    OSD_ScanType_t            ScanType;
    U8                          FrameRate;
} OSD_ScreenParams_t;


typedef U32 OSD_RegionHandle_t;
typedef U32 STOSD_RegionHandle_t;/*used in digitv.h*/


 typedef struct OSD_Bitmap_s
 {
     OSD_ColorType_t    ColorType;
     U8                 BitsPerPel;
     U32                Width;/*Bitmap的高度*/
     U32                Height;/*Bitmap的宽度*/
     U32                Pitch;/*上下两个像素间隔的像素数*/
     void*              Data_p;
	 HI_HANDLE          SurfaceHandle;
 } OSD_Bitmap_t;

 typedef struct OSD_Rect_s
 {
    S32 PositionX;
    S32 PositionY;
    S32 Width;
    S32 Height;    
 } OSD_Rect_t;

 
typedef struct OSD_Region_s OSD_Region_t;

//#define LAYERS_OSD 1
//#define LAYERS_STILL (1<<1)
//#define LAYERS_CURSOR (1<<2)
typedef enum 
{
OSD_SCREEN_MODE_720_576,
OSD_SCREEN_MODE_1280_720,
OSD_SCREEN_MODE_1920_1080
}DRV_OSDScreenMode_t;

typedef enum 
{
OSD_FONT_HEI,/*黑体*/
OSD_FONT_KAI,/*楷*/
OSD_FONT_SUN/*宋*/
}DRV_OSDFont_t;

extern unsigned char *gFontWidth;
extern unsigned char *gFonthzData;
extern unsigned char *gAsciiData;

DRV_ErrCode drv_OsdFillRectangle (OSD_Region_t* Region_p,
                                                                S32                  PositionX,
                                                                S32                  PositionY,
                                                                U32                  Width,
                                                                U32                  Height,
                                                                S32                  Color);
DRV_ErrCode DRV_GetRegionBitmap(OSD_Region_t* Region_p, OSD_Bitmap_t* bitmap);

DRV_ErrCode DRV_OSDShowLogo(STOSD_Bitmap_t *Bitmap_p,U8 *File);

DRV_ErrCode DRV_OSDInit(DRV_OSDScreenMode_t Mode);

OSD_Region_t *DRV_OSDRegionCreate(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 colortype);

DRV_ErrCode DRV_OSDRegionDestory(OSD_Region_t* Region_p);

DRV_ErrCode DRV_OSDGetRegionSurface(OSD_Region_t* Region_p,HI_HANDLE *pSurfaceHandle);

OSD_Region_t* DRV_OSDGetRegionHandle(char *name);

DRV_ErrCode DRV_OSDChangeRegionType(OSD_Region_t *Region_p,OSD_ColorType_t ColorType);
DRV_ErrCode DRV_OSDGetRegionSize(OSD_Region_t * Region_p,OSD_Rect_t *InputRect,OSD_Rect_t * OutputRect);

DRV_ErrCode DRV_OSDSetRegionSize(OSD_Region_t * Region_p,OSD_Rect_t *InputRect,OSD_Rect_t * OutputRect);
/*与上述两个接口逻辑层面是一样的，无非参数有差别*/
DRV_ErrCode DRV_OSDRegionSetScreen(OSD_Region_t *Region_p,DRV_RectWh *Rect);

DRV_ErrCode DRV_OSDRegionGetScreen(OSD_Region_t *Region_p,DRV_RectWh *Rect);

BOOL DRV_OSDRegionVisiableGet(OSD_Region_t *Region_p);

S32 DRV_OSDRegionBPPGet(OSD_Region_t *Region_p);

char* DRV_OSDRegionNameGet(OSD_Region_t *Region_p);

OSD_ColorType_t DRV_OSDRegionGetColorType(OSD_Region_t* Region_p);
S32 drv_OsdGetRegionTrueColor(const OSD_Region_t *Region_p, S32 Color,OSD_Color_t* GxobjColor);

/*返回原来的CK*/
S32 DRV_OSDRegionSetColorKey(OSD_Region_t *Region_p,S32 Color);

DRV_ErrCode DRV_OSDRegionGetColorKey(OSD_Region_t *Region_p,S32* ColorKey,U32* State);

DRV_ErrCode DRV_OSDRegionEnableColorKey(OSD_Region_t *Region_p);

DRV_ErrCode DRV_OSDRegionDisableColorKey(OSD_Region_t *Region_p);

DRV_ErrCode DRV_OSDEnableLayerColorKey(OSD_Region_t *Region_p);

DRV_ErrCode DRV_OSDDisableLayerColorKey(OSD_Region_t * Region_p);
S32 DRV_OSDSetLayerColorKey(OSD_Region_t * Region_p, S32 Color);

DRV_ErrCode DRV_OSDGetLayerColorKey(OSD_Region_t * Region_p, U32 *Color );

DRV_ErrCode DRV_OSDRegionFilterSet(OSD_Region_t* Region_p,OSD_FilterType_t filter);

OSD_Palette_p DRV_OSDRegionPaletteSet(OSD_Region_t* Region_p, OSD_Palette_p Palette);

DRV_ErrCode DRV_OSDRegionShow(OSD_Region_t *Region_p);

DRV_ErrCode DRV_OSDRegionHide(OSD_Region_t *Region_p);

DRV_ErrCode DRV_OSDRegionMixWeightGet(OSD_Region_t* Region_p,U8 *MixWeight);

DRV_ErrCode DRV_OSDRegionMixWeightSet(OSD_Region_t* Region_p,U8 MixWeight);
DRV_ErrCode DRV_OSDPixelDraw(OSD_Region_t* Region_p,
                                             S32 PositionX,
                                             S32 PositionY,
                                             S32 Color);

DRV_ErrCode DRV_OSDGetPixelColor(OSD_Region_t* Region_p,
                                             S32 PositionX,
                                             S32 PositionY,
                                             S32* Color);

DRV_ErrCode DRV_OSDHorizontalLineDraw(OSD_Region_t * Region_p, 
                                                               S32 PositionX, 
                                                               S32 XRange, 
                                                               S32 PositionY,
                                                               S32 Color);

DRV_ErrCode DRV_OSDVerticalLineDraw(OSD_Region_t* Region_p, 
                                                               S32 PositionX, 
                                                               S32 PositionY, 
                                                               S32 YRange,
                                                               S32 Color);

DRV_ErrCode DRV_OSDLineDraw(OSD_Region_t* Region_p, 
                                              S32 XStart, 
                                              S32 YStart, 
                                              S32 XEnd, 
                                              S32 YEnd,
                                              S32 Color);
/*画一个矩形框*/
DRV_ErrCode DRV_OSDRectDraw(OSD_Region_t* Region_p, S32 X0, S32 Y0, S32 X1, S32 Y1, S32 Color);

/*画一个矩形，与上述接口是一个是画框一个是画矩形*/
DRV_ErrCode DRV_OSDRectFillDraw(OSD_Region_t* Region_p, S32 X0, S32 Y0, S32 X1, S32 Y1, S32 Color);

DRV_ErrCode DRV_OSDCircleFillDraw(OSD_Region_t* Region_p, S32 x_centre, S32 y_centre, S32 x_semi_axis, S32 y_semi_axis, S32 Color);

DRV_ErrCode DRV_OSDGetRegionCreateSize(OSD_Region_t * Region_p,OSD_Rect_t * Rect);

DRV_ErrCode DRV_OSDRegionFill(OSD_Region_t*Region_p, S32 Color);

DRV_ErrCode DRV_OSDAreaGet(OSD_Region_t *Region_p, S32 x, S32 y, S32 width,S32 height,S32 *area);

DRV_ErrCode DRV_OSDAreaShow(OSD_Region_t *Region_p, S32 x, S32 y, S32 *area);

DRV_ErrCode DRV_OSDAreaClose(S32 *area);

DRV_ErrCode DRV_OSDAreaRestore(OSD_Region_t *Region_p, S32 x, S32 y, S32 *area);
/*由于历史原因，我们打图工具打出来的头的结构为STOSD_Bitmap_t，底层需要转换层OSD_Bitmap_t*/
DRV_ErrCode drv_OsdBitmapStToDrv(STOSD_Bitmap_t *StBitmap_p,OSD_Bitmap_t *DrvBitmap_p);

DRV_ErrCode DRV_OSDBitmapDraw(OSD_Region_t *Region_p, S32 PositionX, S32 PositionY, STOSD_Bitmap_t *Bitmap_p);
DRV_ErrCode DRV_OSDDrawBitmap(OSD_Region_t *Region_p, S32 PositionX, S32 PositionY, OSD_Bitmap_t *Bitmap_p);
DRV_ErrCode DRV_OSDDrawBitmapRectZoom(OSD_Region_t *Region_p, DRV_RectWh *RegRect_p, DRV_RectWh *BmpRect_p,OSD_Bitmap_t *Bitmap_p);
DRV_ErrCode DRV_OSDDrawBitmapZoom(OSD_Region_t *Region_p, DRV_RectWh *Rect_p, OSD_Bitmap_t *Bitmap_p);

DRV_ErrCode DRV_OSDCreateDDBitmap(OSD_Region_t* Region_p,const U32 width, const U32 height, OSD_Bitmap_t** bitmap);

DRV_ErrCode DRV_OSDFreeDDBitmap(OSD_Bitmap_t* bitmap);

DRV_ErrCode DRV_BitmapFillRectangle(OSD_Bitmap_t*pBitmap,DRV_RectWh* pDstRect,S32 color);

DRV_ErrCode drv_OsdBitmapSetPixelColor(OSD_Bitmap_t*pDstBitmap,S32 PositionX, S32 PositionY,S32 Color);

/*为了增加对tab键的支持,把原来的第三个参数拆分成两个参数*/
DRV_ErrCode DRV_OSDCharacterDraw(OSD_Region_t *Region_p, 
                                                char *Text,
                                                S32 x,              /*文字所在行的横向起始位置*/
                                                S32 Offset,     /*当前字符的横向偏移*/
                                                S32 y,
                                                S32 wbs,
                                                S32 hbs, 
                                                S32 Color,
                                                S32 *DrawWidth,
                                                S32 *DrawHigh);
DRV_ErrCode DRV_OSDTextLineDraw(OSD_Region_t *Region_p,char *hz,S32 Len, S32 x,S32 y,S32 wbs,S32 hbs, S32  FontSpace, S32 Color);


//单个字符，无法处理控制字符
S32 DRV_BitmapDrawChar(OSD_Bitmap_t*pBitmap,
                                                char *Text,
                                                S32 x,/*文字所在行的横向起始位置*/
                                                S32 y,
                                                S32 wbs,
                                                S32 hbs, 
                                                S32 fontSpase,
                                                S32 Color,
                                                S32 *DrawWidth,
                                                S32 *DrawHigh);



U32 DRV_BitmapTextLineDraw(OSD_Bitmap_t *pBitmap,
                                         const char   *Text,
                                         const U32 Length,
                                         const S32 PositionX,
                                         const S32 PositionY,
                                         const U32 Wbs,
                                         const U32 Hbs,
                                         const U32 FontSpace,
                                         const U32 Color);
U32 DRV_OSDGetTextWidth(const char   *Text,const U32 space,const U32 Wbs, U32* pWidth);
DRV_ErrCode DRV_STILLDrawIFrame(U8 * pData, S32 uiDataLength);


DRV_ErrCode DRV_STILLRegionShow();

DRV_ErrCode DRV_STILLRegionHide();

DRV_ErrCode DRV_OSDEnable(void);

DRV_ErrCode DRV_OSDDisable(void);

DRV_ErrCode DRV_OSDSetMode(DRV_OSDMode mode);

DRV_ErrCode DRV_OSDSetVtgTimingMode(DRV_VtgTimingMode_t mode);

DRV_ErrCode DRV_STILLEnable(void);

DRV_ErrCode DRV_STILLDisable(void);

DRV_ErrCode DRV_CURSOREnable(void);

DRV_ErrCode DRV_CURSORDisable(void);

DRV_ErrCode DRV_OSDUpdate(void);

DRV_ErrCode DRV_RegionDisplayInfo(void);

OSD_Region_t* DRV_RegionGetHandle(S32 index);

DRV_ErrCode DRV_OSDSetScreenModeFake(DRV_OSDScreenMode_t Mode);
DRV_ErrCode DRV_OSDSetScreenMode(DRV_OSDScreenMode_t Mode);

DRV_ErrCode DRV_OSDGetScreenMode(DRV_OSDScreenMode_t *Mode);
DRV_ErrCode DRV_OSDSetFont(DRV_OSDFont_t Font,U32 Size);
DRV_ErrCode DRV_OSDGetFont(DRV_OSDFont_t *Font,U32 *Size);
DRV_ErrCode DRV_OSDBitmapBlt(OSD_Bitmap_t *Dest_Bitmap_p, OSD_Bitmap_t *Src_Bitmap_p,DRV_RectWh *Dest_Rect_p,DRV_RectWh *Src_Rect_p,U8 Alpha);
DRV_ErrCode DRV_OSDBitmapFill(OSD_Bitmap_t *Bitmap_p, DRV_RectWh *Rect_p,U32 Color);
DRV_ErrCode DRV_OSDALU(OSD_Bitmap_t *Dest_Bitmap_p, OSD_Bitmap_t *fg_Bitmap_p,OSD_Bitmap_t *bg_Bitmap_p,DRV_RectWh *Dest_Rect_p,DRV_RectWh *fg_Rect_p,DRV_RectWh *bg_Rect_p,HIGO_COMPOPT_E Mode);
DRV_ErrCode DRV_OSDFillRectangle(OSD_Bitmap_t *Dest_Bitmap_p, DRV_RectWh *Dest_Rect_p,HI_COLOR Color,U8 Alpha);
DRV_ErrCode DRV_OSDFillRectangleEx(OSD_Bitmap_t *Dest_Bitmap_p, DRV_RectWh *Dest_Rect_p,HI_COLOR Color,HIGO_COMPOPT_E Mode);
DRV_ErrCode DRV_OSDBitmapScale(OSD_Bitmap_t *Bitmap_p, OSD_Bitmap_t *Dst_Bitmap_p);
/*******************************************************************************
函数名称:	GetDrvOsdVersion
功    能:	得到OSD模块的版本号,由串口输出
参    数:	无
返 回 值:	版本号时间，如:0x08042916表示08年4月29日16点
*******************************************************************************/
U32 GetDrvOsdVersion();

/*******************************************************************************
函数名称:	DRV_OSDClearScreen
功    能:	清屏
参    数:	无
返 回 值:	
*******************************************************************************/
DRV_ErrCode DRV_OSDClearScreen(void);

DRV_ErrCode YS_BlitClaim();
DRV_ErrCode YS_BlitRelease();

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif


