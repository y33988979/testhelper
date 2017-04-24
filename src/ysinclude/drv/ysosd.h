/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosd.h

    Description : 图形引擎模块，主要提供图形绘制功能，
    实现线、矩形、圆、域填充、文本、颜色、图形
    等的绘制；它调用设备驱动模块提供的服务接口；

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-12
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2004-03-01
    作    者：XXX
    说    明：1. 问题简述
              2. 修改内容简述
              3. 其他

*******************************************************************************/

#ifndef _YSOSD_H_
#define _YSOSD_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "osddrv.h"
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct Rect{
	S32 x0;
	S32 y0;
	S32 x1;
	S32 y1;
} Rect_t;

typedef struct MidTextStyle
{
	S32			crFore;		//the foreground color of text
	S32			crBack;		//the background color of text
	S32			nFontSize;  //the size of the text
	S32			nFontStyle;	//the style of th text
	S32				nStyle;		//the alignment of the text
}MidTextStyle_t;

typedef enum
{
  OSD_UP_SHADOW = 0,
  OSD_DOWN_SHADOW
}OSD_ShadowType_e;

typedef enum
{
  OSD_UP_TRIANGLE = 0,
  OSD_LEFT_TRIANGLE,
  OSD_RIGHT_TRIANGLE,
  OSD_DOWN_TRIANGLE
}OSD_TriangleType_e;

typedef enum
{
  TEXT_UP_LEFT,
  TEXT_UP_MID,
  TEXT_UP_RIGHT,
  TEXT_MID_LEFT,
  TEXT_MID_MID,
  TEXT_MID_RIGHT,
  TEXT_DOWN_LEFT,
  TEXT_DOWN_MID,
  TEXT_DOWN_RIGHT,
  TEXT_LINE_FULL,
  TEXT_TEXTER_FLAG = 0x8000
}TEXT_TextStyle_e;

typedef struct
{
  S32 LineSpace;
  S32 FontSpace;
  S32 LeftSpace;
  S32 RightSpace;
  S32 TopSpace;
  S32 BottomSpace;
}TEXT_TexterStyle_t;

/*----------------------------------------------------------------------------
 *	Enum & Macro
 *----------------------------------------------------------------------------*/
#define	SOLID					0

#define ALIGNMENT_MID			0
#define ALIGNMENT_LEFT			1
#define ALIGNMENT_RIGHT			2

#define BMP_TRANSPARENCE_COLOR  0

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
OSD_Region_t *MidCreateRegion(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 bits_per_pixel);

/* ________________________________________________________________________
 * FunctionName: DRV_OSDDrawRegionSet
 * Description : specify region to draw to
 * Parameters  : OSD_Region_t *reg
 * ________________________________________________________________________*/
void MidDrawRegionSet(OSD_Region_t *region );

void MidDrawRegionNameSet(char *name);

void gprim_setbcol(S32 color);
void gprim_setfcol(S32 color);

void MidDrawDot(S32 x, S32 y, S32 color);

void MidDrawLine(S32 x0,S32 y0,S32 x1,S32 y1,S32 style,S32 width,S32 color);

void MidDrawRect(Rect_t rect, S32 color,S32 IsFill,S32 width,S32 style);

void MidDrawCircle(Rect_t rect,S32 color,S32 IsFill,S32 width,S32 style);

void MidDrawShadow(Rect_t rect, S32 FillColor, S32 ShadowColor, S32 style,S32 width);

void MidDrawTriangle(Rect_t rect,S32 color,S32 IsFill,S32 style,S32 width);

void MidDrawHz(char *pHz,S32 x,S32 y,S32 nFontSize,S32 nFontStyle);

void MidDrawText(char *TextInput,S32 nFontSize,S32 nFontStyle,Rect_t rect,S32 style,S32 crFore,S32 crBack);

void MidDrawTextStyle(char* pText,Rect_t rect,MidTextStyle_t *pTextStyle);

void MidDrawCls (S32 color);

S32 MidDrawSaveArea(S32 x0, S32 y0, S32 x1, S32 y1, OSD_Bitmap_t *area);

S32 MidDrawRestoreArea(S32 x0, S32 y0, OSD_Bitmap_t *area);

S32 MidDrawShowArea(S32 x0, S32 y0, OSD_Bitmap_t *area);

S32 MidDrawCloseArea(OSD_Bitmap_t *area);

void bmpdraw(void *graphic, Rect_t *rect);

S32 TextLineDraw(char *TextInput,
                                  S32 nFontSize,
                                  S32 nFontStyle, 
                                  S32 wbs, 
                                  S32 hbs, 
                                  Rect_t *rect,
                                  TEXT_TextStyle_e TextStyle,
                                  S32 FontSpace,
                                  S32 crFore,
                                  S32 crBack);

S32 TexterDraw(char *TextInput,
                            S32 nFontSize,
                            S32 nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle,
                            S32 crFore,
                            S32 crBack);

OSD_Region_t *MidCreateRegion16(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 bits_per_pixel);

void MidDrawRegionSet16(OSD_Region_t *region );

OSD_Region_t * MidDrawRegionGet(void);

S32 DRV_TextLineDraw(OSD_Region_t *region,char *TextInput,
                                  S32 nFontSize,
                                  S32 nFontStyle, 
                                  S32 wbs, 
                                  S32 hbs, 
                                  Rect_t *rect,
                                  TEXT_TextStyle_e TextStyle,
                                  S32 FontSpace,
                                  S32 crFore,
                                  S32 crBack);

S32 DRV_OSDTexterDraw(OSD_Region_t *region,char *TextInput,
                            S32 nFontSize,
                            S32 nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle,
                            S32 crFore,
                            S32 crBack);

int MidGetTextWidth(char *TextInput,
                            int nFontSize,
                            int nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif

