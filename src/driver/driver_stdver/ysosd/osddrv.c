/*******************************************************************************
Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

File name : osddrv.c

Description : 图形引擎驱动模块，主要实现对底层驱动的封装

Others:      

History:     
======================================================================
时    间：2004-03-12
作    者：Cao Songtao
说    明：1. 创建
======================================================================
最近更新:
    1. 2009-05-20 cran Area操作使用SystemPartition的内存不再使用AVMEM,使画图完全基于映射内存模式
    2. 2009-05-26 cran 加相应接口完成bitmap的绘字功能
    3. 2009-09-09 cran 将不再支持8位region,改为4444
    4. 2009-09-10 cran 改字放大时在没有背景的情况下画不全,用cpu,blit进行双重画
    5. 2009-09-11 cran 改换字库后bimap的字画不出问题
    6. 2010-04-16 cran 注掉接口Bitmap_CopyRectangle
    7. 2010-04-16 cran 改变画图时DRV_DCacheFlush的调用位置,画外后再进行同步
    8. 2010-04-16 cran cpu画点支持7101和7167两个平台
    9. 2010-07-26 cran 增加可以得到region的几个参数的接口(ColorKey,Size,Mode)
    10. 2010-07-26 cran 改DRV_GetRegionBitmap的bitmap不能直接写入显存的bug
当前版本号:    
    version : 
    time    : 2010-07-26
目前存在问题:
    1. 把各层接口严格区分

    
    2. 画图函数加上自动裁减功能

*******************************************************************************/

/*----------------------------------------------------------------------------
*	Include
*----------------------------------------------------------------------------*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/fb.h> 
#include <sys/mman.h>
#include <fcntl.h>
#include <hi_go_comm.h>
#include <hi_go_bliter.h>
#include <hi_go_surface.h>
#include <hi_go_gdev.h>
#include <hi_go_decoder.h>
#include <hi_go_text.h>
#include <hi_struct.h>

#include "drv_disp.h"
#include "drv_vo.h"
#include "drv_os.h" 
#include "drv_typedef.h" 
#include "osddrv.h"
#include "ys_os.h" 
#include "drv_basic.h"
#include "drv_sys.h"
#include "stbtype.h"
extern unsigned char ascii_hei_40_width_data[];
extern const unsigned char fonthz_hei_40_data[];
extern unsigned char ascii_hei_40_data[];
extern unsigned char ascii_hei_20_width_data[];
extern const unsigned char fonthz_hei_20_data[];
extern unsigned char ascii_hei_20_data[];

///#define DRV_OSD_UPDATE
/*----------------------------------------------------------------------------
*	Debug
*----------------------------------------------------------------------------*/
/*错误输出*/
#ifndef NULL
#define NULL ((void *)0)
#endif

#define CURRENT_MODULE DRV_MODULE_OSD

#define WAIT_BLIT_COMPLETE
#ifdef WAIT_BLIT_COMPLETE
#define WAIT_OPERATION_COMPLETE     {\
         if(DRV_OS_SemWait(mOsdDevice_p->SyncSemaphore, DRVOS_WAIT_FINITY, 600)!=0)\
         {\
             OSD_ERROR(("TIMEOUT: Re open BLIT"));\
         }\
     }
#endif

#define IS_NTSC ((mOsdDevice_p->Mode == OSD_NTSC)||(mOsdDevice_p->Mode == OSD_NTSC_443))
#define IS_720P ((mOsdDevice_p->VtgTimingMode == DRV_DISPLAY_TIMING_MODE_720P_50)\
	||(mOsdDevice_p->VtgTimingMode == DRV_DISPLAY_TIMING_MODE_720P_60))
#define ARGB_GET_A(x) (U8)(((U32)(x)&0XFF000000L)>>24)
#define ARGB_GET_R(x) (U8)(((U32)(x)&0XFF0000L)>>16)
#define ARGB_GET_G(x) (U8)(((U32)(x)&0XFF00L)>>8)
#define ARGB_GET_B(x) (U8)(((U32)(x))&0XFFL)

#define A1_(x) ((x)&0X01L)
#define A4_(x) ((x)&0X0FL)
#define A5_(x) ((x)&0X1FL)
#define A6_(x) ((x)&0X3FL)
#define A8_(x) (x)

#define BYTE_U32(x,n)  ( (U32)(x)<<(n) )


#define ARGB8888_Get_A(x)    ARGB_GET_A(x)
#define ARGB8888_Get_R(x)    ARGB_GET_R(x)
#define ARGB8888_Get_G(x)    ARGB_GET_G(x)
#define ARGB8888_Get_B(x)    ARGB_GET_B(x)
#define ARGB1555_Get_A(x)    (ARGB_GET_A(x)>0?1:0)
#define ARGB1555_Get_R(x)     (ARGB_GET_R(x)>>3)
#define ARGB1555_Get_G(x)    (ARGB_GET_G(x)>>3)
#define ARGB1555_Get_B(x)    (ARGB_GET_B(x)>>3)
#define ARGB4444_Get_A(x)    (ARGB_GET_A(x)>>4)
#define ARGB4444_Get_R(x)     (ARGB_GET_R(x)>>4)
#define ARGB4444_Get_G(x)     (ARGB_GET_G(x)>>4)
#define ARGB4444_Get_B(x)      (ARGB_GET_B(x)>>4)
#define RGB565_Get_R(x)       (ARGB_GET_R(x)>>3)
#define RGB565_Get_G(x)       (ARGB_GET_G(x)>>2)
#define RGB565_Get_B(x)         (ARGB_GET_B(x)>>3)

#define ARGB8888_2_1555(x)    (BYTE_U32(ARGB1555_Get_A(x),15) | BYTE_U32(ARGB1555_Get_R(x),10) |BYTE_U32(ARGB1555_Get_G(x),5) |BYTE_U32(ARGB1555_Get_B(x),0))
#define ARGB8888_2_4444(x)    (BYTE_U32(ARGB4444_Get_A(x),12) | BYTE_U32(ARGB4444_Get_R(x),8) |BYTE_U32(ARGB4444_Get_G(x),4) |BYTE_U32(ARGB4444_Get_B(x),0))
#define ARGB8888_2_565(x)    (BYTE_U32(RGB565_Get_R(x),11) |BYTE_U32(RGB565_Get_G(x),5) |BYTE_U32(RGB565_Get_B(x),0))


#define OSD_NULL_REGION_HANDLE   ((OSD_Region_t*)NULL)
#define REGION_IS_NULL ((OSD_Region_t*)NULL == Region_p)
#define getbit( byte, yy) (byte>>yy)&0x01


#define OSD_SIN(a)     ( a == 512 ? 0x8000 : (S32)OSD_SINE[a]     )
#define OSD_COS(a)		( a == 0   ? 0x8000 : (S32)OSD_SINE[512-a] )

#define YS_BLIT_PROTECT
#ifdef YS_BLIT_PROTECT
#define BLIT_USE_SEM_SYNC
#endif
#define CLEAN_STILL_IFRAME

/*----------------------------------------------------------------------------
我们约定，一个region就是对应一个window，并且不再有层的概念
*----------------------------------------------------------------------------*/

typedef struct
{
    DRVOS_Partition_p   CPUPartition_p;                   /*用于普通数据的内存分配*/
    U32                 MaxRegion;
    OSD_Region_t*       FirstCreatedRegion;    /* Handle of the first created region in list. Wrap_NO_Region_HANDLE if none*/
    OSD_Region_t*       LastCreatedRegion;     /* Handle of the last created region in list. Wrap_NO_Region_HANDLE if none*/
    U32                 NbCreatedRegions;
    U32                 NbCreatedOsdRegions;      /* 这个只针对OSD的那几个 */
    U32                 NbCreatedCursorRegions;      /* 这个只针对cursor的那几个 */
    BOOL                ShowLayerVisible;
    HI_HANDLE           LayerHandle;               /* Handle of the opened layer */
    HI_HANDLE           LayerSurfaceHandle;               /* Handle of the opened layer */
	HI_HANDLE           TextFont;
    U8*                 SyncBitmapData_p;
    DRVOS_Sem_t*        SyncSemaphore;
    DRV_OSDFont_t       Font;
    U32                 FontSize;
    DRV_OSDMode         Mode;
    DRV_VtgTimingMode_t VtgTimingMode;
    DRV_OSDScreenMode_t ScreenMode;
    DRV_RectWh          ScreenSize;
} OSD_Device_t;


struct OSD_Region_s
{
	OSD_Device_t*           Device_p; 
	U32                     LayerID;
	HI_HANDLE               LayerHandle;
	HI_HANDLE               SurfaceHandle;
    BOOL                    ShowSurfaceVisible;    /* Show Visible Regions */
	char                    Name[OSD_REGION_NAME_LENGTH+1];
	U32                     CreatedWidth;
	U32                     CreatedHeight;
	DRV_RectWh              RectIn;
	DRV_RectWh              RectOut;
	U32                     Pitch;
	BOOL                    PaletteValid;
	OSD_Palette_t           Palette;
	OSD_Palette_p           PaletteColorArrary;//设置调色板时保存传入的调色板的指针
	OSD_ColorType_t         ColorType;
	U8                      NbBitsPerPel; 
	U8                      MixWeight;// transparent factor
	BOOL                    ActivatedFilter;        /* If the filter is activated */
	U32                     ColorKey;
	BOOL                    EnableCK;
	U32                     LayerColorKey;
	BOOL                    EnableLayerColorKey;
	OSD_Region_t*           NextRegion;             /* Handle of the next created Region. Wrap_NO_Region_HANDLE if none*/
	OSD_Region_t*           PreviousRegion;         /* Handle of the previous open viewport. LAYCOMPO_NO_VIEWPORT_HANDLE if none*/
	void*                   FrameBufferaddr;
	U32                     FrameBufferSize;
	OSD_Palette_t           TextPalette;
};

#define MAX_SIZE_PALETTE  256 * 32 * 2

/*----------------------------------------------------------------------------
*	External variable
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
*	Private Constant & Enum & Macro
*----------------------------------------------------------------------------*/
static U32 gCHI_FONT_WIDTH;
static U32 gCHI_FONT_HEIGHT;
static U32 gCHI_FONT_PITCH;
static U32 gCHI_FONT_SIZE;

#ifdef DRV_SLZK_SUPPORT
static HI_HANDLE gSi_hFont = HIGO_INVALID_HANDLE;
#endif
U8 *gFontWidth;
U8 *gFonthzData;
U8 *gAsciiData;

//static char *mHzBuffer_Np;//用户态地址.可以用户态直接操作

/*----------------------------------------------------------------------------
*	Global variable
*----------------------------------------------------------------------------*/
static OSD_Device_t  mOsdDevice;
OSD_Device_t*        mOsdDevice_p;



#ifdef BLIT_USE_SEM_SYNC
static DRVOS_Sem_t*  m_YSBlitProtectSem_p;
#endif

/*For sync Blit operation. */
static U8*                   SyncBitmapData_p;

S32 OSD_SINE[512] = 
{
	0,   100,   201,   301,   402,   502,   603,   703,   804,   904,  1005,  1105,  1206,  1306,  1407,  1507,
	1607,  1708,  1808,  1909,  2009,  2109,  2210,  2310,  2410,  2510,  2611,  2711,  2811,  2911,  3011,  3111,
	3211,  3311,  3411,  3511,  3611,  3711,  3811,  3911,  4011,  4110,  4210,  4310,  4409,  4509,  4609,  4708,
	4808,  4907,  5006,  5106,  5205,  5304,  5403,  5503,  5602,  5701,  5800,  5898,  5997,  6096,  6195,  6294,
	6392,  6491,  6589,  6688,  6786,  6884,  6983,  7081,  7179,  7277,  7375,  7473,  7571,  7669,  7766,  7864,
	7961,  8059,  8156,  8254,  8351,  8448,  8545,  8642,  8739,  8836,  8933,  9029,  9126,  9223,  9319,  9415,
	9512,  9608,  9704,  9800,  9896,  9991, 10087, 10183, 10278, 10374, 10469, 10564, 10659, 10754, 10849, 10944,
	11039, 11133, 11228, 11322, 11416, 11511, 11605, 11699, 11793, 11886, 11980, 12073, 12167, 12260, 12353, 12446,
	12539, 12632, 12725, 12817, 12910, 13002, 13094, 13186, 13278, 13370, 13462, 13554, 13645, 13736, 13828, 13919,
	14010, 14100, 14191, 14282, 14372, 14462, 14552, 14642, 14732, 14822, 14912, 15001, 15090, 15180, 15269, 15357,
	15446, 15535, 15623, 15712, 15800, 15888, 15976, 16063, 16151, 16238, 16325, 16413, 16499, 16586, 16673, 16759,
	16846, 16932, 17018, 17104, 17189, 17275, 17360, 17445, 17530, 17615, 17700, 17784, 17869, 17953, 18037, 18121,
	18204, 18288, 18371, 18454, 18537, 18620, 18703, 18785, 18868, 18950, 19032, 19113, 19195, 19276, 19358, 19439,
	19519, 19600, 19681, 19761, 19841, 19921, 20001, 20080, 20159, 20239, 20318, 20396, 20475, 20553, 20631, 20709,
	20787, 20865, 20942, 21020, 21097, 21173, 21250, 21326, 21403, 21479, 21555, 21630, 21706, 21781, 21856, 21931,
	22005, 22080, 22154, 22228, 22301, 22375, 22448, 22521, 22594, 22667, 22740, 22812, 22884, 22956, 23027, 23099,
	23170, 23241, 23312, 23382, 23453, 23523, 23593, 23662, 23732, 23801, 23870, 23939, 24007, 24075, 24144, 24211,
	24279, 24346, 24414, 24480, 24547, 24614, 24680, 24746, 24812, 24877, 24943, 25008, 25073, 25137, 25201, 25266,
	25330, 25393, 25457, 25520, 25583, 25645, 25708, 25770, 25832, 25894, 25955, 26016, 26077, 26138, 26199, 26259,
	26319, 26379, 26438, 26498, 26557, 26615, 26674, 26732, 26790, 26848, 26905, 26963, 27020, 27076, 27133, 27189,
	27245, 27301, 27356, 27411, 27466, 27521, 27576, 27630, 27684, 27737, 27791, 27844, 27897, 27949, 28002, 28054,
	28106, 28157, 28208, 28259, 28310, 28361, 28411, 28461, 28511, 28560, 28609, 28658, 28707, 28755, 28803, 28851,
	28898, 28946, 28993, 29039, 29086, 29132, 29178, 29223, 29269, 29314, 29359, 29403, 29447, 29491, 29535, 29578,
	29621, 29664, 29707, 29749, 29791, 29833, 29874, 29915, 29956, 29997, 30037, 30077, 30117, 30156, 30196, 30235,
	30273, 30312, 30350, 30387, 30425, 30462, 30499, 30535, 30572, 30608, 30644, 30679, 30714, 30749, 30784, 30818,
	30852, 30886, 30919, 30952, 30985, 31018, 31050, 31082, 31114, 31145, 31176, 31207, 31237, 31268, 31298, 31327,
	31357, 31386, 31414, 31443, 31471, 31499, 31526, 31554, 31581, 31607, 31634, 31660, 31685, 31711, 31736, 31761,
	31785, 31810, 31834, 31857, 31881, 31904, 31927, 31949, 31971, 31993, 32015, 32036, 32057, 32078, 32098, 32118,
	32138, 32157, 32176, 32195, 32214, 32232, 32250, 32268, 32285, 32302, 32319, 32335, 32351, 32367, 32383, 32398,
	32413, 32427, 32442, 32456, 32469, 32483, 32496, 32509, 32521, 32533, 32545, 32557, 32568, 32579, 32589, 32600,
	32610, 32619, 32629, 32638, 32647, 32655, 32663, 32671, 32679, 32686, 32693, 32700, 32706, 32712, 32718, 32723,
	32728, 32733, 32737, 32741, 32745, 32749, 32752, 32755, 32758, 32760, 32762, 32764, 32765, 32766, 32767, 32767 
};


/*----------------------------------------------------------------------------
*	Private Module Variable
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
*	Private Function Prototype
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
*	Function
*----------------------------------------------------------------------------*/
DRV_ErrCode YS_BlitClaim()
{
#ifdef BLIT_USE_SEM_SYNC    
    DRV_OS_SemWait(m_YSBlitProtectSem_p,DRVOS_WAIT_INFINITY,0);
#endif
    return 0;
}
DRV_ErrCode YS_BlitRelease()
{
#ifdef BLIT_USE_SEM_SYNC    
    DRV_OSSemSignal(m_YSBlitProtectSem_p);
#endif
    return 0;
}

/*
对于上层应用来说，颜色值只有两种，调色板的index值，以及32位的ARGB8888值
需要驱动来转换给往下传递
*/
/*ARGB8888的颜色值转成指定格式的颜色值*/
static S32 drv_OsdArgbToTrueColor(OSD_ColorType_t ColorType, S32 Color)
{
    switch(ColorType)
    {
        case OSD_COLOR_TYPE_ARGB4444:
            return ARGB8888_2_4444(Color);        
            break;
        case OSD_COLOR_TYPE_ARGB1555:
            return ARGB8888_2_1555(Color);        
            break;
        case OSD_COLOR_TYPE_RGB565:
            return ARGB8888_2_565(Color); 
            break;
        case OSD_COLOR_TYPE_CLUT8:
            return ARGB8888_2_4444(Color);
        case OSD_COLOR_TYPE_ARGB8888:
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
            return Color;        
            break;
		default:
			STTBX_Print(("Warning,unkown colortype %d\n",ColorType));
	     return Color;
            break;
    }
}

/*通过调色板及其index得到对应的ARGB8888的颜色结构，并返回ARGB8888的S32形式*/
S32 drv_OsdGetPaletteColor(OSD_Palette_t* Palette_p,U32 PaletteIndex,OSD_Color_t* Color_p)
{
    U32 Size;
    S16 Value16;
    S32 Value32;
    S32 ArgbColor;
    U8* Dest_p;
    void  *PaletteData_p;
	PaletteData_p = Palette_p->Data_p;
    if(Palette_p->ColorType != OSD_COLOR_TYPE_ARGB8888)
    {
		STTBX_Print(("drv_OsdGetPaletteColor,unkown Palette_p->ColorType %d\n",Palette_p->ColorType));
		return 0;
    }

    switch (Palette_p->ColorType)
    {
        case OSD_COLOR_TYPE_ARGB8888:
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
            Size = sizeof(U32);
            Color_p->Type = Palette_p->ColorType;
            Dest_p = (U8*)((U32)PaletteData_p + (PaletteIndex * Size));
            Value32 = *((U32*)Dest_p);
            ArgbColor = Value32;
            Color_p->Value.ARGB8888.Alpha = (Value32 & 0xff000000) >> 24;
            Color_p->Value.ARGB8888.R     = (Value32 & 0x00ff0000) >> 16;
            Color_p->Value.ARGB8888.G     = (Value32 & 0x0000ff00) >>  8;
            Color_p->Value.ARGB8888.B     = (Value32 & 0x000000ff)      ;
            break;

        case OSD_COLOR_TYPE_ARGB4444:
            Size = sizeof(U16);
            Color_p->Type = Palette_p->ColorType;
            Dest_p = (U8*)((U32)PaletteData_p + (PaletteIndex * Size));
            Value16 = *((U16*)Dest_p);
            ArgbColor = Value16;
           Color_p->Value.ARGB4444.Alpha = (Value16 & 0xf000) >> 12;
            Color_p->Value.ARGB4444.R     = (Value16 & 0x0f00) >>  8;
            Color_p->Value.ARGB4444.G     = (Value16 & 0x00f0) >>  4;
            Color_p->Value.ARGB4444.B     = (Value16 & 0x000f)      ;
            break;

        case OSD_COLOR_TYPE_ARGB1555:
            Size = sizeof(U16);
            Color_p->Type = Palette_p->ColorType;
            Dest_p = (U8*)((U32)PaletteData_p + (PaletteIndex * Size));
            Value16 = *((U16*)Dest_p);
            ArgbColor = Value16;
            Color_p->Value.ARGB1555.Alpha = (Value16 & 0x8000) >> 15;
            Color_p->Value.ARGB1555.R     = (Value16 & 0x7c00) >> 10;
            Color_p->Value.ARGB1555.G     = (Value16 & 0x03e0) >>  5;
            Color_p->Value.ARGB1555.B     = (Value16 & 0x001f)      ;
            break;

        case OSD_COLOR_TYPE_RGB565:
            Size = sizeof(U16);
            Color_p->Type = Palette_p->ColorType;
            Dest_p = (U8*)((U32)PaletteData_p + (PaletteIndex * Size));
            Value16 = *((U16*)Dest_p);
            ArgbColor = Value16;
            Color_p->Value.RGB565.R     = (Value16 & 0xf800) >>  11;
            Color_p->Value.RGB565.G     = (Value16 & 0x07e0) >>  5;
            Color_p->Value.RGB565.B     = (Value16 & 0x001f)      ;
            break;
        default:
            STTBX_Print(("Warning,unkown colortype %d\n",PaletteIndex));
    }

    return ArgbColor;
}

/*这个argb已经不是应用传下来的argb了，而是根据truecolor转换以后的U32形式而已，
  使用higo接口都用这个，但是直接操作显存则不用
  与drv_OsdGetTrueColor，drv_OsdGetRegionTrueColor配合使用传入higo接口中
*/
typedef struct 
{
  U8 B;
  U8 G;
  U8 R;
  U8 Alpha;
} HISI_ColorARGB_t;

static S32 drv_OsdTrueColorToHisiArgb(OSD_ColorType_t ColorType, S32 TrueColor)
{
    S32 HisiArgbColor = 0;
    HISI_ColorARGB_t ARGB8888;
    switch(ColorType)
    {
        case OSD_COLOR_TYPE_ARGB4444:
            ARGB8888.Alpha = (TrueColor & 0x0000F000) >>  8;
            ARGB8888.R     = (TrueColor & 0x00000F00) >>  4;
            ARGB8888.G     = (TrueColor & 0x000000F0)      ;
            ARGB8888.B     = (TrueColor & 0x0000000F) <<  4;
            //ARGB8888.Alpha &= 0xF0000000;
            //ARGB8888.R     &= 0x00F00000;
            //ARGB8888.G     &= 0x0000F000;
            //ARGB8888.B     &= 0x000000F0;
            break;
        case OSD_COLOR_TYPE_ARGB1555:
            ARGB8888.Alpha = (TrueColor & 0x00008000) >>  8;
            ARGB8888.R     = (TrueColor & 0x00007C00) >>  7;
            ARGB8888.G     = (TrueColor & 0x000003E0) >>  2;
            ARGB8888.B     = (TrueColor & 0x0000001F) <<  3;
            //ARGB8888.Alpha &= 0x80000000;
            //ARGB8888.R     &= 0x00F80000;
            //ARGB8888.G     &= 0x0000F800;
            //ARGB8888.B     &= 0x000000F8;
            break;
        case OSD_COLOR_TYPE_RGB565:
            ARGB8888.Alpha = 0xFF;
            ARGB8888.R     = (TrueColor & 0x0000F800) >>  6;
            ARGB8888.G     = (TrueColor & 0x000007E0) >>  1;
            ARGB8888.B     = (TrueColor & 0x0000001F) <<  3;
            //ARGB8888.Alpha &= 0xFF000000;
            //ARGB8888.R     &= 0x00F80000;
            //ARGB8888.G     &= 0x0000FC00;
            //ARGB8888.B     &= 0x000000F8;
            break;
        case OSD_COLOR_TYPE_ARGB8888:
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
            ARGB8888.Alpha = (TrueColor & 0xFF000000) >> 24;
            ARGB8888.R     = (TrueColor & 0x00FF0000) >> 16;
            ARGB8888.G     = (TrueColor & 0x0000FF00) >>  8;
            ARGB8888.B     = (TrueColor & 0x000000FF)      ;
            break;
		default:
			STTBX_Print(("Warning,unkown colortype %d\n",ColorType));
	     return TrueColor;
            break;
    }
	memcpy((U8*)&HisiArgbColor,(U8*)&ARGB8888,4);
    return HisiArgbColor;
}

/*ARGB8888的颜色值转成指定格式的真实颜色结构并范围S32的颜色值*/
static S32 drv_OsdGetTrueColor(OSD_ColorType_t ColorType, S32 Color,OSD_Color_t* GxobjColor)
{
	S32 TrueColor = 0;
    if(GxobjColor != NULL)
    {
        switch(ColorType)
        {
            case OSD_COLOR_TYPE_CLUT8:
            case OSD_COLOR_TYPE_ARGB4444:
                GxobjColor->Type = OSD_COLOR_TYPE_ARGB4444;
                GxobjColor->Value.ARGB4444.Alpha = ARGB4444_Get_A(Color);
                GxobjColor->Value.ARGB4444.R = ARGB4444_Get_R(Color);
                GxobjColor->Value.ARGB4444.G = ARGB4444_Get_G(Color);
                GxobjColor->Value.ARGB4444.B = ARGB4444_Get_B(Color);
                break;
            case OSD_COLOR_TYPE_ARGB1555:
                GxobjColor->Type = OSD_COLOR_TYPE_ARGB1555;
                GxobjColor->Value.ARGB1555.Alpha = ARGB1555_Get_A(Color);
                GxobjColor->Value.ARGB1555.R = ARGB1555_Get_R(Color);
                GxobjColor->Value.ARGB1555.G = ARGB1555_Get_G(Color);
                GxobjColor->Value.ARGB1555.B = ARGB1555_Get_B(Color);
                break;
            case OSD_COLOR_TYPE_RGB565:
                GxobjColor->Type = OSD_COLOR_TYPE_RGB565;
                GxobjColor->Value.RGB565.R = RGB565_Get_R(Color);
                GxobjColor->Value.RGB565.G = RGB565_Get_G(Color);
                GxobjColor->Value.RGB565.B = RGB565_Get_B(Color);
                break;
            case OSD_COLOR_TYPE_ARGB8888:
                GxobjColor->Type = OSD_COLOR_TYPE_ARGB8888;
                GxobjColor->Value.ARGB8888.Alpha= ARGB8888_Get_A(Color);
                GxobjColor->Value.ARGB8888.R = ARGB8888_Get_R(Color);
                GxobjColor->Value.ARGB8888.G = ARGB8888_Get_G(Color);
                GxobjColor->Value.ARGB8888.B = ARGB8888_Get_B(Color);
                break;
        }    
    }
    TrueColor = drv_OsdArgbToTrueColor(ColorType,Color);
    return TrueColor;
}

/*通过region及传入的ARGB8888颜色值或者调色板的索引值得到真实颜色的结构，并返回真实颜色的S32形式*/
S32 drv_OsdGetRegionTrueColor(const OSD_Region_t *Region_p, S32 Color,OSD_Color_t* GxobjColor)
{
    DRV_ErrCode errCode;
    OSD_ColorType_t ColorType;
    OSD_Color_t GxobjColorTemp;
    S32 TrueColor = 0;
    S32 ArgbColor = 0;
    S32 HisiArgbColor = 0;
    ColorType = Region_p->ColorType;
    //if((Region_p->PaletteValid == TRUE)&&((Color&0xFF000000) == 0))
    if(Region_p->PaletteValid == TRUE)
    {
        ArgbColor = drv_OsdGetPaletteColor((OSD_Palette_t*)&(Region_p->Palette),Color,&GxobjColorTemp);
    }
    else
    {
    	ArgbColor = Color;
    }
	TrueColor = drv_OsdGetTrueColor(ColorType, ArgbColor, GxobjColor);
#if 1//add by jyshan for hisi
	HisiArgbColor = drv_OsdTrueColorToHisiArgb(ColorType,TrueColor);
    return HisiArgbColor;
#endif
    return TrueColor;
}

static void drv_OsdYcrcb2Argb(const CAST_OSD_Color_t *Ycrcb_p,
                                OSD_ColorARGB_t *Argb_p)
{
    S32 A_YCBCR,Y,Cb,Cr;
    S32 A_RGB,R,G,B;

    A_YCBCR = Ycrcb_p->MixWeight;
    Y       = Ycrcb_p->Value.Y;
    Cb      = Ycrcb_p->Value.Cb;
    Cr      = Ycrcb_p->Value.Cr;

    if(A_YCBCR > 0x3f)
        A_YCBCR = 0x3f;

    if(Y == 0 && Cr == 0 && Cb == 0)
    {
        A_RGB = A_YCBCR <<2;
        R     = 0;
        G     = 0;
        B     = 0;
    }
    else
    {
        if(Y < 16)
             Y = 16;
        else if(Y > 235)
             Y = 235;
        if(Cb < 16)
             Cb = 16;
        else if(Cb > 240)
             Cb = 240;
        if(Cr < 16)
             Cr = 16;
        else if(Cr > 240)
             Cr = 240;

        A_RGB = A_YCBCR << 2;//aplpha分量只有64级,扩展成256级
        R     = (298*(Y - 16) + 409*(Cr - 128))/256;
        G     = (298*(Y - 16) - 208*(Cr - 128) - 100*(Cb - 128))/256;
        B     = (298*(Y - 16) + 517*(Cb - 128))/256;

        if(R < 0)
             R = 0;
         else if(R > 255)
             R = 255;
        if(G < 0)
             G = 0;
        else if(G > 255)
             G = 255;
        if(B < 0)
             B = 0;
        else if(B > 255)
             B = 255;
    }

    Argb_p->Alpha = (U8)A_RGB;
    Argb_p->R     = (U8)R;
    Argb_p->G     = (U8)G;
    Argb_p->B     = (U8)B;

    return;
}


static void drv_OsdArgb2Ycrcb( S32 Argb_p, CAST_OSD_Color_t *Ycrcb_p)
{
    S32 Y,Cb,Cr;
    S32 R,G,B;
    
    R = (ARGB8888_Get_R(Argb_p)*219/255 + 16)&0xff;
    G = (ARGB8888_Get_G(Argb_p)*219/255 + 16)&0xff;
    B = (ARGB8888_Get_B(Argb_p)*219/255 + 16)&0xff;

    Y=(U8)(((77*R + 150*G + 29*B)/256));
    Cb=(U8)((-44*R - 87*G + 131*B)/256 + 128 ) ;
    Cr=(U8)((131*R - 110*G - 21*B)/256 +  128 ) ;

    Ycrcb_p->MixWeight = ARGB8888_Get_A(Argb_p)>>2;
    Ycrcb_p->Type = STOSD_COLOR_TYPE_YCRCB888;
    Ycrcb_p->Value.Y = Y;
    Ycrcb_p->Value.Cr = Cr;
    Ycrcb_p->Value.Cb = Cb;
}

static void drv_OSDAddRegionToList(OSD_Region_t* Region_p)
{
    OSD_Device_t*         Device_p    = (OSD_Device_t*)(Region_p->Device_p);

    /* Update Region descriptor */
    Region_p->NextRegion        = OSD_NULL_REGION_HANDLE;
    Region_p->PreviousRegion    = Device_p->LastCreatedRegion;

    /* Update Wrap_unit_t */
    if (Device_p->NbCreatedRegions == 0)
    {
        Device_p->FirstCreatedRegion = Region_p;
    }
    else /* LastCreatedRegion != WRAP_NO_REGION_HANDLE */
    {
        ((OSD_Region_t*)(Device_p->LastCreatedRegion))->NextRegion = Region_p;
    }
    Device_p->NbCreatedRegions++;
    Device_p->LastCreatedRegion = Region_p;

}

static void drv_OSdRemoveRegionFromList( OSD_Region_t* Region_p)
{
    OSD_Device_t *Device_p = (OSD_Device_t*)(Region_p->Device_p);

    /* Update Region descriptor. */
    if(Device_p->FirstCreatedRegion == Region_p)
    {
        Device_p->FirstCreatedRegion = Region_p->NextRegion;
    }
    if(Device_p->LastCreatedRegion == Region_p)
    {
        Device_p->LastCreatedRegion = Region_p->PreviousRegion;
    }
    if(Region_p->NextRegion != OSD_NULL_REGION_HANDLE)
    {
        Region_p->NextRegion->PreviousRegion = Region_p->PreviousRegion;
    }
    if(Region_p->PreviousRegion != OSD_NULL_REGION_HANDLE)
    {
        Region_p->PreviousRegion->NextRegion = Region_p->NextRegion;
    }
    Device_p->NbCreatedRegions--;
}

DRV_ErrCode drv_OsdFillRectangle (OSD_Region_t* Region_p,
                                                                S32                  PositionX,
                                                                S32                  PositionY,
                                                                U32                  Width,
                                                                U32                  Height,
                                                                S32                  Color)
{
    DRV_ErrCode             ErrCode;
    HI_RECT pRect;
    S32 TrueColor;
    pRect.x = PositionX;
    pRect.y = PositionY;
    pRect.w = Width;
    pRect.h = Height;
    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    YS_BlitClaim();
    ErrCode = HI_GO_FillRect(Region_p->SurfaceHandle,&pRect,TrueColor,HIGO_COMPOPT_NONE);
	YS_BlitRelease();    
    if (HI_SUCCESS != ErrCode) 
    {
        printf("drv_OsdFillRectangle HI_GO_FillRect ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
   
    return(ST_NO_ERROR);
}

DRV_ErrCode DRV_GetRegionBitmap(OSD_Region_t* Region_p, OSD_Bitmap_t* bitmap)
{
   	DRV_ErrCode errCode = 0;
   	HI_S32 Width;
   	HI_S32 Height;
   	HIGO_PF_E PixelFormat;

	if(Region_p == NULL)
	{
		Err_Print("[DRV_GetRegionBitmap] Region_p == NULL\n");
		return -1;
	}
	
	if(bitmap == NULL)
	{
		Err_Print("[DRV_GetRegionBitmap] bitmap == NULL\n");
		return -1;
	}
	
	
    YS_BlitClaim();
   	errCode = HI_GO_GetSurfaceSize(Region_p->SurfaceHandle, &Width, &Height);
   	errCode |= HI_GO_GetSurfacePixelFormat(Region_p->SurfaceHandle, &PixelFormat);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_GetRegionBitmap HI_GO_GetSurfacePixelFormat ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }

   	switch(PixelFormat)
   	{
   		case HIGO_PF_CLUT8:
		   	bitmap->ColorType     = OSD_COLOR_TYPE_CLUT8;
		   	bitmap->BitsPerPel    = 8;
	   		break;
   		case HIGO_PF_1555:
		   	bitmap->ColorType     = OSD_COLOR_TYPE_ARGB1555;
		   	bitmap->BitsPerPel    = 16;
	   		break;
   		case HIGO_PF_8888:
		   	bitmap->ColorType     = OSD_COLOR_TYPE_ARGB8888;
		   	bitmap->BitsPerPel    = 32;
	   		break;
   		case HIGO_PF_4444:
		   	bitmap->ColorType     = OSD_COLOR_TYPE_ARGB4444;
		   	bitmap->BitsPerPel    = 16;
	   		break;
   		case HIGO_PF_565:
		   	bitmap->ColorType     = OSD_COLOR_TYPE_RGB565;
		   	bitmap->BitsPerPel    = 16;
	   		break;
   	}
   	bitmap->Width         = Width;
   	bitmap->Height        = Height;
   	bitmap->Pitch         = bitmap->Width*bitmap->BitsPerPel/8;
   	bitmap->SurfaceHandle = Region_p->SurfaceHandle;
	return DRV_NO_ERROR;
}


/*************************************************
* Function:       DRV_OSDInit
* Description:    
* Input:          
* Output:        
* Return:         TRUE if error, FALSE if OK
* Others:        
*************************************************/
HI_HANDLE OsdLayerHandle;
HI_HANDLE OsdLayerSurfaceHandle;
HI_S32 DRV_OSDFileDec(U8 *pszFileName, HI_HANDLE SurfaceHandle)
{
    HI_S32 ret;
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;

    /**create decode */
    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;
    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /** decode and put to Surface */
   // ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    ret = HI_GO_DecImgToSurface(hDecoder, 0, SurfaceHandle);
    ret |= HI_GO_DestroyDecoder(hDecoder);

    return ret;
}

DRV_ErrCode DRV_OSDShowLogo(STOSD_Bitmap_t *Bitmap_p,U8 *File)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	OSD_Bitmap_t DrvBitmap;
	HI_HANDLE Surface,MemSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_SURINFO_S    stSurInfo;
    DRV_OSDScreenMode_t Mode;
	OSD_Region_t *mStillRegion;
    memset(&stSurInfo,0,sizeof(stSurInfo));
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
	if(Bitmap_p != NULL)
	{
	    ErrCode = drv_OsdBitmapStToDrv(Bitmap_p,&DrvBitmap);

	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = DrvBitmap.Height;
	    SurInfo.Width       = DrvBitmap.Width;
	    SurInfo.Pitch[0]    = DrvBitmap.Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(DrvBitmap.Data_p);
	    switch(DrvBitmap.ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapDraw HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("[DRV_OSDShowLogo] HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapDraw HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	    DrvBitmap.SurfaceHandle = Surface;
		SrcRect.x = 0;
		SrcRect.y = 0;
		SrcRect.w = Bitmap_p->Width;
		SrcRect.h = Bitmap_p->Height;
	   
		DstRect.x = 0;
		DstRect.y = 0;
		DstRect.w = 1280;
		DstRect.h = 720;
		
	    memset(&BlitOpt,0,sizeof(HIGO_BLTOPT_S));
	    BlitOpt.EnableScale    = TRUE;
	    ErrCode = HI_GO_Blit(Surface,&SrcRect,mStillRegion->SurfaceHandle,&DstRect,&BlitOpt);
	    if (HI_SUCCESS!= ErrCode)
	    {
		    printf("DRV_OSDShowLogo HI_GO_Blit failed.ErrCode 0x%08x\n",ErrCode);
	    	return ErrCode;
	    }
		ErrCode = DRV_OSDUpdate();
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		ErrCode |= HI_GO_FreeSurface(Surface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapDraw HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
			return HI_FAILURE; 
		}
	}
	else
	{
		DRV_STILLEnable();
		mStillRegion = DRV_OSDGetRegionHandle("STILL");
		ErrCode = DRV_OSDFileDec(File,mStillRegion->SurfaceHandle);
		ErrCode |= DRV_OSDUpdate();
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDUpdate HI_GO_RefreshLayer ErrCode 0x%08x\n",ErrCode);
	        return ErrCode;
	    }
	}
	return ErrCode;
}

DRV_ErrCode DRV_OSDInit(DRV_OSDScreenMode_t Mode)
{
    DRV_ErrCode ErrCode;
    HIGO_LAYER_INFO_S stLayerInfoHD;
	HI_HANDLE TextFont;
   	HIGO_PF_E PixelFormat;
    HIGO_TEXT_INFO_S info;
    HIGO_LAYER_KEY_S LayerColorKey;
	info.pMbcFontFile = "./simhei.ttf";
	info.pSbcFontFile = "./simhei.ttf";
	memset(&mOsdDevice,0,sizeof(mOsdDevice));
#ifdef BLIT_USE_SEM_SYNC
    m_YSBlitProtectSem_p = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);
#endif
    
    /** create graphic layer  */    
    ErrCode = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfoHD);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_GetLayerDefaultParam ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }

    switch(Mode)
    {
    	case OSD_SCREEN_MODE_720_576:
		    stLayerInfoHD.CanvasWidth = 720;
		    stLayerInfoHD.CanvasHeight = 576;
		    stLayerInfoHD.DisplayWidth = 720;
		    stLayerInfoHD.DisplayHeight = 576;
		    stLayerInfoHD.ScreenWidth = 720;
		    stLayerInfoHD.ScreenHeight = 576;
    		break;
    	case OSD_SCREEN_MODE_1280_720:
		    stLayerInfoHD.CanvasWidth = 1280;
		    stLayerInfoHD.CanvasHeight = 720;
		    stLayerInfoHD.DisplayWidth = 1280;
		    stLayerInfoHD.DisplayHeight = 720;
		    stLayerInfoHD.ScreenWidth = 1280;
		    stLayerInfoHD.ScreenHeight = 720;
     		break;
   		case OSD_SCREEN_MODE_1920_1080:
			stLayerInfoHD.CanvasWidth = 1920;
		    stLayerInfoHD.CanvasHeight = 1080;
		    stLayerInfoHD.DisplayWidth = 1920;
		    stLayerInfoHD.DisplayHeight = 1080;
		    stLayerInfoHD.ScreenWidth = 1920;
		    stLayerInfoHD.ScreenHeight = 1080;
	   		break;
    }
    stLayerInfoHD.PixelFormat = HIGO_PF_8888;
    stLayerInfoHD.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
    stLayerInfoHD.AntiLevel = HIGO_LAYER_DEFLICKER_AUTO;
    stLayerInfoHD.LayerID = HIGO_LAYER_HD_0;
	stLayerInfoHD.CanvasWidth = 1920;
    stLayerInfoHD.CanvasHeight = 1080;
    stLayerInfoHD.DisplayWidth = 1920;
    stLayerInfoHD.DisplayHeight = 1080;
    stLayerInfoHD.ScreenWidth = 1920;
    stLayerInfoHD.ScreenHeight = 1080;
    ErrCode = HI_GO_CreateLayer(&stLayerInfoHD,&OsdLayerHandle);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_CreateLayer ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
    
    /** set the background color of graphic layer*/    
    //ErrCode = HI_GO_SetLayerBGColor(OsdLayerHandle,0x00000000);
    //if (HI_SUCCESS != ErrCode)
    //{
    //    printf("DRV_OSDInit HI_GO_SetLayerBGColor ErrCode 0x%08x\n",ErrCode); 
    //    return ErrCode;       
    //}
    //ErrCode = HI_GO_RefreshLayer(OsdLayerHandle, NULL);
    //if (HI_SUCCESS != ErrCode)
    //{
    //    printf("DRV_OSDInit HI_GO_RefreshLayer ErrCode 0x%08x\n",ErrCode); 
    //    return ErrCode;       
    //}
    //ErrCode = HI_GO_SetLayerAlpha(OsdLayerHandle,0xFF);
    //if (HI_SUCCESS != ErrCode)
    //{
    //    printf("DRV_OSDInit HI_GO_SetLayerAlpha ErrCode 0x%08x\n",ErrCode); 
    //    return ErrCode;       
    //}
    /** get the graphic layer Surface */
    ErrCode = HI_GO_GetLayerSurface(OsdLayerHandle,&OsdLayerSurfaceHandle); 
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_GetLayerSurface ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
    LayerColorKey.bEnableCK = TRUE;
    LayerColorKey.ColorKey  = 0x00000000;
	//ErrCode = HI_GO_SetSurfaceColorKey(OsdLayerSurfaceHandle, 0x00000000);       
    //ErrCode = HI_GO_SetLayerColorkey(OsdLayerHandle,&LayerColorKey);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDSetLayerColorKey HI_GO_SetLayerColorkey ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
    
    mOsdDevice_p = &mOsdDevice;
    /*初始化默认设置一下，初始化之后可以再动态设置*/
    mOsdDevice.Mode            = OSD_PAL;
	mOsdDevice.MaxRegion       = 8;
	mOsdDevice.LayerHandle     = OsdLayerHandle;
	mOsdDevice.LayerSurfaceHandle = OsdLayerSurfaceHandle;
    mOsdDevice.VtgTimingMode   = DRV_DISPLAY_TIMING_MODE_1080i_50;
    mOsdDevice.ScreenSize.x    = 0;
    mOsdDevice.ScreenSize.y    = 0;
    mOsdDevice.ScreenSize.nWidth  = stLayerInfoHD.ScreenWidth;
    mOsdDevice.ScreenSize.nHeight = stLayerInfoHD.ScreenHeight;
	mOsdDevice_p->CPUPartition_p  = SystemPartition;
	DRV_OSDSetFont(OSD_FONT_HEI,40);
	DRV_OSDSetScreenMode(Mode); 

	info.u32Size = mOsdDevice_p->FontSize;
    //ErrCode = HI_GO_CreateTextEx(&info,&TextFont);
    //ErrCode = HI_GO_CreateText(NULL, "./higo_gb2312.bin",&TextFont);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_CreateText failed to create the font:./simhei.ttf ErrCode 0x08x!\n",ErrCode); 
        return ErrCode;       
    }
	//mOsdDevice.TextFont = TextFont;
    //ErrCode = HI_GO_SetTextColor(TextFont, 0xFF000000);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_CreateText failed to create the font:./simhei.ttf ErrCode 0x08x!\n",ErrCode); 
        return ErrCode;       
    }
    //ErrCode = HI_GO_SetTextStyle(TextFont, HIGO_TEXT_STYLE_NORMAL);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDInit HI_GO_CreateText failed to create the font:./simhei.ttf ErrCode 0x08x!\n",ErrCode); 
        return ErrCode;       
    }

#ifdef WAIT_BLIT_COMPLETE
    mOsdDevice.SyncSemaphore = DRV_OS_SemCreate(NULL,0,DRVOS_INIT_FIFO_TIMEOUT);
#else
    mOsdDevice.SyncSemaphore = DRV_OS_SemCreate(NULL,0,DRVOS_INIT_FIFO_WAIT);
#endif
	//mHzBuffer_Np = DRV_OSMalloc(mOsdDevice_p->CPUPartition_p,10*1024);
    return ErrCode;
}

BOOL ISopenHDMI = FALSE;
BOOL OSD_getHDMIopen(void)
{
	return ISopenHDMI;
}
void OSD_setHDMIopen(BOOL status)
{
	ISopenHDMI = status;
}
OSD_update_open_hdmi(HIGO_STEREO_MODE_E StereoMode,HIGO_ENCPICFRM_E EncPicFrm)
{
    HI_GO_SetStereoMode(OsdLayerHandle,   StereoMode);
    HI_GO_SetEncPicFrm(OsdLayerHandle, EncPicFrm);
    OSD_setHDMIopen(TRUE);
    HI_GO_RefreshLayer(OsdLayerHandle, NULL);
    printf("use OSD_update_hdmi to open 3D\n");
}
OSD_update_close_hdmi()
{
    HI_S32 ret;
    HIGO_ENCPICFRM_E EncPicFrm = HIGO_ENCPICFRM_MONO;
    EncPicFrm = HIGO_ENCPICFRM_MONO;
    HI_GO_SetEncPicFrm(OsdLayerHandle, EncPicFrm);
    OSD_setHDMIopen(FALSE);
    HI_GO_RefreshLayer(OsdLayerHandle, NULL);
    printf("use OSD_update_hdmi to close 3D\n");
}

/*************************************************
* Function:       DRV_OSDCreateRegion
* Description:    Creates a drawable region
* Input:  colortype   为4  region颜色类型为CLUT8(底层不支持CLUT4,只能由CLUT8模拟)
                        8                  CLUT8
                        16                 ARGB4444
                        4444               ARGB4444
                        1555               ARGB1555
                        565                ARGB565
* Output:        
* Return:
* Others:        
*************************************************/
OSD_Region_t *DRV_OSDRegionCreate(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 colortype)
{
    DRV_ErrCode ErrCode  = DRV_NO_ERROR;
    OSD_Region_t* Region_p = NULL;
    S32 xsize;
    S32 ysize;
    OSD_Palette_t               Palette;
    S32 layerID;
    HI_HANDLE hSurface;
   	HIGO_PF_E PixelFormat;
	HI_PIXELDATA pData; 
	
   	Region_p = (OSD_Region_t *)DRV_OSMalloc(mOsdDevice_p->CPUPartition_p,sizeof(OSD_Region_t));
   	Region_p->Device_p = mOsdDevice_p;
    memset(&Palette,0,sizeof(OSD_Palette_t));
    if(name == NULL || (*name == 0) || (strlen(name) > OSD_REGION_NAME_LENGTH))
    {
    	printf("DRV_OSDRegionCreate bad param!\n");
    	DRV_OSFree(mOsdDevice_p->CPUPartition_p,(void*)Region_p);
        return NULL;
    }    
    if(strncmp(name,"STILL",5) == 0)
        layerID = OSD_REGION_STILL;
    else if(strncmp(name,"CURSOR",6) == 0)
    {
    	layerID = Region_p->Device_p->NbCreatedCursorRegions + OSD_REGION_CURSOR1;
    	Region_p->Device_p->NbCreatedCursorRegions += 1;
    }    
    else
    {
    	layerID = Region_p->Device_p->NbCreatedOsdRegions + OSD_REGION_OSD1;
    	Region_p->Device_p->NbCreatedOsdRegions += 1;
    }    
    
    Region_p->LayerID  = layerID;
    strncpy(Region_p->Name,name,OSD_REGION_NAME_LENGTH);
    xsize = xend - xstart;
    ysize = yend - ystart;
    if((xsize > mOsdDevice.ScreenSize.nWidth)||(ysize > mOsdDevice.ScreenSize.nHeight))
    {
     	printf("DRV_OSDRegionCreate bad param!\n");
   		return NULL;
    }	
	/*input 创建的时候RectIn.x，RectIn.y都是为0*/
    Region_p->CreatedWidth        = xsize;
    Region_p->CreatedHeight       = ysize;
    Region_p->RectIn.x            = 0;
    Region_p->RectIn.y            = 0;
    Region_p->RectIn.nWidth       = Region_p->CreatedWidth;
    Region_p->RectIn.nHeight      = Region_p->CreatedHeight;
    /*OutPut只有在surface uptdate 到layer上时有用,默认满屏输出*/
#if 1 
    Region_p->RectOut.x           = xstart;
    Region_p->RectOut.nWidth      = Region_p->CreatedWidth;
	Region_p->RectOut.y           = ystart;
    Region_p->RectOut.nHeight     = Region_p->CreatedHeight;
#else
	if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080)
	{
	    Region_p->RectOut.x           = xstart*1280/1920;
	    Region_p->RectOut.nWidth      = Region_p->CreatedWidth*1280/1920;
		Region_p->RectOut.y           = ystart*720/1080;
	    Region_p->RectOut.nHeight     = Region_p->CreatedHeight*720/1080;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720)
	{
	    Region_p->RectOut.x           = xstart;
	    Region_p->RectOut.nWidth      = Region_p->CreatedWidth;
		Region_p->RectOut.y           = ystart;
	    Region_p->RectOut.nHeight     = Region_p->CreatedHeight;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576)
	{
	    Region_p->RectOut.x           = xstart*1280/720;
	    Region_p->RectOut.nWidth      = Region_p->CreatedWidth*1280/720;
		Region_p->RectOut.y           = ystart*720/576;
	    Region_p->RectOut.nHeight     = Region_p->CreatedHeight*720/576;
	}
	else
	{
     	printf("DRV_OSDRegionCreate bad param!\n");
		return NULL;
	}
#endif	
    Region_p->ShowSurfaceVisible  = FALSE;
    /* Creation Palette */
    switch(colortype)
    {
        case 8:
            Region_p->ColorType = OSD_COLOR_TYPE_CLUT8;
		    PixelFormat = HIGO_PF_CLUT8;
		    Region_p->NbBitsPerPel = 8;
            break;
        case 4444:
        case OSD_COLOR_TYPE_ARGB4444:
            Region_p->ColorType = OSD_COLOR_TYPE_ARGB4444;
		    PixelFormat = HIGO_PF_4444;
		    Region_p->NbBitsPerPel = 16;
            break;
        case 565:
        case OSD_COLOR_TYPE_RGB565:
            Region_p->ColorType = OSD_COLOR_TYPE_RGB565;
		    PixelFormat = HIGO_PF_565;
 		    Region_p->NbBitsPerPel = 16;
           break;
        case 16:
        case 1555:
        case OSD_COLOR_TYPE_ARGB1555:
            Region_p->ColorType = OSD_COLOR_TYPE_ARGB1555;
		    PixelFormat = HIGO_PF_1555;
 		    Region_p->NbBitsPerPel = 16;
           break;
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
            Region_p->ColorType = OSD_COLOR_TYPE_YCBCR888_422;
		    PixelFormat = HIGO_PF_8888;
		    Region_p->NbBitsPerPel = 32;
            break;
        case 32:
        case 8888:
        case OSD_COLOR_TYPE_ARGB8888:
            Region_p->ColorType = OSD_COLOR_TYPE_ARGB8888;
		    PixelFormat = HIGO_PF_8888;
		    Region_p->NbBitsPerPel = 32;
            break;
        /*24 32 bpp not supported*/
        default:
            OSD_ERROR((" Error region type in DRV_OSDRegionCreate!"));
            DRV_OSFree(mOsdDevice_p->CPUPartition_p,Region_p);
            return NULL;

    }
    Region_p->Palette.ColorType = OSD_COLOR_TYPE_ARGB8888;
    Region_p->Palette.ColorDepth = 32;
    Region_p->Palette.Data_p = DRV_OSMalloc(SystemPartition,MAX_SIZE_PALETTE);
    YS_BlitClaim();    
	ErrCode = HI_GO_CreateSurface(xsize, ysize, PixelFormat,&hSurface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return NULL; 
    }
    Region_p->SurfaceHandle = hSurface;
    /** draw a rectangle on the surface */
    ErrCode = HI_GO_FillRect(hSurface,NULL,0x00000000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return NULL; 
    }
   	ErrCode = HI_GO_LockSurface(hSurface, pData, HI_TRUE);//修正相关参数
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_LockSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return NULL; 
    }
    Region_p->Pitch = pData[0].Pitch;
   	ErrCode = HI_GO_UnlockSurface(hSurface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_UnlockSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return NULL; 
    }
    YS_BlitRelease();
    /* here make the region color to transparent or black */
    DRV_OSDRegionMixWeightSet(Region_p, 63);
    //DRV_OSDRegionSetColorKey(Region_p,0xFF000000);

    if(Region_p->NbBitsPerPel > 8)
    {
        DRV_OSDRegionFill(Region_p, 0x0);
    }
    drv_OSDAddRegionToList(Region_p);
    return Region_p;      
}

DRV_ErrCode DRV_OSDRegionDestory(OSD_Region_t* Region_p)
{
	DRV_ErrCode ErrCode;
    if(REGION_IS_NULL)
        return -1;
    YS_BlitClaim();
    ErrCode = HI_GO_FreeSurface(Region_p->SurfaceHandle);
    YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDRegionDestory HI_GO_FreeSurface ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
    Region_p->SurfaceHandle = 0;
    drv_OSdRemoveRegionFromList(Region_p);
    return 0;
}

DRV_ErrCode DRV_OSDGetRegionSurface(OSD_Region_t* Region_p,HI_HANDLE *pSurfaceHandle)
{
    if(REGION_IS_NULL)
        return -1;
    *pSurfaceHandle = Region_p->SurfaceHandle;
    return 0;
}


OSD_Region_t* DRV_OSDGetRegionHandle(char *name)
{
    OSD_Region_t *Region_p = NULL;
    Region_p = mOsdDevice_p->FirstCreatedRegion;
    while(Region_p != NULL)
    {
        if(strncmp(Region_p->Name,name,OSD_REGION_NAME_LENGTH) == 0)
            break;
        Region_p = Region_p->NextRegion;
    }
    return Region_p;
}

DRV_ErrCode DRV_OSDChangeRegionType(OSD_Region_t *Region_p,OSD_ColorType_t ColorType)
{
	DRV_ErrCode ErrCode;
    HI_HANDLE hSurface;
    S32 pitch;
   	HIGO_PF_E PixelFormat;
	HI_PIXELDATA pData; 
    BOOL PaletteValid;
    if(REGION_IS_NULL)
        return -1;
    if(ColorType == Region_p->ColorType)
        return 0;
    YS_BlitClaim();
    ErrCode = HI_GO_FreeSurface(Region_p->SurfaceHandle);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDRegionDestory DRV_OSDChangeRegionType ErrCode 0x%08x\n",ErrCode); 
    	YS_BlitRelease();
        return ErrCode;       
    }
    DRV_OSFree(mOsdDevice_p->CPUPartition_p,Region_p->Palette.Data_p);
    Region_p->SurfaceHandle = 0;
    Region_p->ColorType = ColorType;
    /* Creation Palette */
    switch(ColorType)
    {
        case 8:
		    PixelFormat = HIGO_PF_CLUT8;
		    Region_p->NbBitsPerPel = 8;
            break;
        case 4444:
        case OSD_COLOR_TYPE_ARGB4444:
		    PixelFormat = HIGO_PF_4444;
		    Region_p->NbBitsPerPel = 16;
            break;
        case 565:
        case OSD_COLOR_TYPE_RGB565:
		    PixelFormat = HIGO_PF_565;
		    Region_p->NbBitsPerPel = 16;
            break;
        case 16:
        case 1555:
        case OSD_COLOR_TYPE_ARGB1555:
		    PixelFormat = HIGO_PF_1555;
		    Region_p->NbBitsPerPel = 16;
            break;
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
		    PixelFormat = HIGO_PF_8888;
		    Region_p->NbBitsPerPel = 32;
            break;
        case 32:
        case 8888:
        case OSD_COLOR_TYPE_ARGB8888:
		    PixelFormat = HIGO_PF_8888;
		    Region_p->NbBitsPerPel = 32;
            break;
        /*24 32 bpp not supported*/
        default:
            OSD_ERROR((" Error region type in DRV_OSDRegionCreate!"));
            DRV_OSFree(mOsdDevice_p->CPUPartition_p,Region_p);
	    	YS_BlitRelease();
            return -1;

    }
    Region_p->Palette.ColorType = OSD_COLOR_TYPE_ARGB8888;
    Region_p->Palette.ColorDepth = 32;
    Region_p->Palette.Data_p = DRV_OSMalloc(SystemPartition,MAX_SIZE_PALETTE);
    
	ErrCode = HI_GO_CreateSurface(Region_p->CreatedWidth, Region_p->CreatedHeight, PixelFormat,&hSurface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRVOSDChangeRegionType HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
        return HI_FAILURE;
    }
    /** draw a rectangle on the surface */
    ErrCode = HI_GO_FillRect(hSurface,NULL,0x00000000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return HI_FAILURE; 
    }
   	ErrCode = HI_GO_LockSurface(hSurface, pData, HI_TRUE);//修正相关参数
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_LockSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return HI_FAILURE; 
    }
    Region_p->Pitch = pData[0].Pitch;
   	ErrCode = HI_GO_UnlockSurface(hSurface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_UnlockSurface errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return HI_FAILURE; 
    }
    YS_BlitRelease();
    Region_p->SurfaceHandle = hSurface;        
    /* here make the region color to transparent or black */
    DRV_OSDRegionMixWeightSet(Region_p, 63);
    PaletteValid = Region_p->PaletteValid;
    Region_p->PaletteValid = FALSE; 
    //DRV_OSDRegionSetColorKey(Region_p,0xFF000000);
    Region_p->PaletteValid = PaletteValid;
        
    return ErrCode;
}
DRV_ErrCode DRV_OSDGetRegionSize(OSD_Region_t * Region_p,OSD_Rect_t *InputRect,OSD_Rect_t * OutputRect)
{

	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HI_S32 Width,Height;
	if(REGION_IS_NULL)
		return -1;
	//ErrCode = HI_GO_GetSurfaceSize(Region_p->SurfaceHandle,&Width,&Height);
	//InputRect->PositionX = 0;
	//InputRect->PositionY = 0;
	//InputRect->Width     = Width;
	//InputRect->Height    = Height;
	InputRect->PositionX = Region_p->RectIn.x;
	InputRect->PositionY = Region_p->RectIn.y;
	InputRect->Width     = Region_p->RectIn.nWidth;
	InputRect->Height    = Region_p->RectIn.nHeight;
#if 1
		OutputRect->PositionX  = Region_p->RectOut.x;
		OutputRect->PositionY  = Region_p->RectOut.y;
		OutputRect->Width      = Region_p->RectOut.nWidth;
		OutputRect->Height     = Region_p->RectOut.nHeight;
#else
    /*OutPut只有在surface uptdate 到layer上时有用,默认满屏输出*/
	if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080)
	{
		OutputRect->PositionX  = Region_p->RectOut.x*1280/1920;
		OutputRect->PositionY  = Region_p->RectOut.y*720/1080;
		OutputRect->Width      = Region_p->RectOut.nWidth*1280/1920;
		OutputRect->Height     = Region_p->RectOut.nHeight*720/1080;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720)
	{
	OutputRect->PositionX = Region_p->RectOut.x;
	OutputRect->PositionY = Region_p->RectOut.y;
	OutputRect->Width     = Region_p->RectOut.nWidth;
	OutputRect->Height    = Region_p->RectOut.nHeight;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576)
	{
		OutputRect->PositionX  = Region_p->RectOut.x*720/1280;
		OutputRect->PositionY  = Region_p->RectOut.y*720/576;
		OutputRect->Width      = Region_p->RectOut.nWidth*720/1280;
		OutputRect->Height     = Region_p->RectOut.nHeight*720/576;
	}
	else
	{
     	printf("DRV_OSDRegionCreate bad param!\n");
		return -1;
	}
#endif	
	return ErrCode;
}

DRV_ErrCode DRV_OSDSetRegionSize(OSD_Region_t * Region_p,OSD_Rect_t *InputRect,OSD_Rect_t * OutputRect)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;
	HIGO_PF_E PixelFormat;
    HI_HANDLE hSurface;
	if(REGION_IS_NULL)
		return -1;
	if((InputRect->PositionX + InputRect->Width > Region_p->CreatedWidth)||(InputRect->PositionY+ InputRect->Height > Region_p->CreatedHeight))
		return -1;
	if((OutputRect->PositionX + OutputRect->Width > Region_p->Device_p->ScreenSize.nWidth)||(OutputRect->PositionY + OutputRect->Height > Region_p->Device_p->ScreenSize.nHeight))
		return -1;
	Region_p->RectIn.x       = InputRect->PositionX;
	Region_p->RectIn.y       = InputRect->PositionY;
	Region_p->RectIn.nWidth  = InputRect->Width;
	Region_p->RectIn.nHeight = InputRect->Height;
#if 1
		Region_p->RectOut.x       = OutputRect->PositionX;
		Region_p->RectOut.y       = OutputRect->PositionY;
		Region_p->RectOut.nWidth  = OutputRect->Width;
		Region_p->RectOut.nHeight = OutputRect->Height;
#else
    /*OutPut只有在surface uptdate 到layer上时有用,默认满屏输出*/
	if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080)
	{
		Region_p->RectOut.x       = OutputRect->PositionX*1280/1920;
		Region_p->RectOut.y       = OutputRect->PositionY*720/1080;
		Region_p->RectOut.nWidth  = OutputRect->Width*1280/1920;
		Region_p->RectOut.nHeight = OutputRect->Height*720/1080;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720)
	{
		Region_p->RectOut.x       = OutputRect->PositionX;
		Region_p->RectOut.y       = OutputRect->PositionY;
		Region_p->RectOut.nWidth  = OutputRect->Width;
		Region_p->RectOut.nHeight = OutputRect->Height;
	}
	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576)
	{
		Region_p->RectOut.x       = OutputRect->PositionX*1280/720;
		Region_p->RectOut.y       = OutputRect->PositionY*720/576;
		Region_p->RectOut.nWidth  = OutputRect->Width*1280/720;
		Region_p->RectOut.nHeight = OutputRect->Height*720/576;
	}
	else
	{
     	printf("DRV_OSDRegionCreate bad param!\n");
		return -1;
	}
#endif	
	return ErrCode;
}
/*与上述两个接口逻辑层面是一样的，无非参数有差别*/
DRV_ErrCode DRV_OSDRegionSetScreen(OSD_Region_t *Region_p,DRV_RectWh *Rect)
{
    DRV_ErrCode error = ST_NO_ERROR;
    S32 xsize,ysize,xend,yend;
	OSD_Rect_t  InputRect,  OutputRect;
    
    if(REGION_IS_NULL || Rect == NULL)
        return -1;
    if(Rect->nWidth > Region_p->CreatedWidth || Rect->nHeight > Region_p->CreatedHeight)
        return -1;

	InputRect.PositionX = Region_p->RectIn.x;
	InputRect.PositionY = Region_p->RectIn.y;
	InputRect.Width     = Region_p->RectIn.nWidth;
	InputRect.Height    = Region_p->RectIn.nHeight;

	OutputRect.PositionX = Rect->x;//这种坐标还是特定坐标系下的坐标
	OutputRect.PositionY = Rect->y;
	OutputRect.Width     = Rect->nWidth;
	OutputRect.Height    = Rect->nHeight;

	error = DRV_OSDSetRegionSize(Region_p, &InputRect, &OutputRect);
    return error;
}

DRV_ErrCode DRV_OSDRegionGetScreen(OSD_Region_t *Region_p,DRV_RectWh *Rect)
{
    if(REGION_IS_NULL)
        return -1;
    *Rect =  Region_p->RectOut;
    return 0;
}

BOOL DRV_OSDRegionVisiableGet(OSD_Region_t *Region_p)
{
    if(REGION_IS_NULL)
        return -1;
    return Region_p->ShowSurfaceVisible;
}

S32 DRV_OSDRegionBPPGet(OSD_Region_t *Region_p)
{
    if(REGION_IS_NULL)
        return -1;
    switch(Region_p->ColorType)
    {
        case OSD_COLOR_TYPE_ARGB1555:
        case OSD_COLOR_TYPE_ARGB4444:
        case OSD_COLOR_TYPE_RGB565:    
        case OSD_COLOR_TYPE_CLUT8:
            return 16;
		case OSD_COLOR_TYPE_ARGB8888:
			return 32;
    }
    return -1;
}

char* DRV_OSDRegionNameGet(OSD_Region_t *Region_p)
{
    if(REGION_IS_NULL)
        return NULL;
    else
        return Region_p->Name;
}

OSD_ColorType_t DRV_OSDRegionGetColorType(OSD_Region_t* Region_p)
{
    if(REGION_IS_NULL)
        return -1;    
    
    return Region_p->ColorType;
}

/*返回原来的CK*/
S32 DRV_OSDRegionSetColorKey(OSD_Region_t *Region_p,S32 Color)
{
    DRV_ErrCode errCode;
    HI_COLOR ColorKeyNew;
    HI_COLOR ColorKeyOrg;
    HI_BOOL PaletteValid;
    if(REGION_IS_NULL)
        return -1;
    ColorKeyOrg = Region_p->ColorKey;//ARGB8888的值，而非真实值
    Region_p->ColorKey = Color;//ARGB8888的值，而非真实值
    PaletteValid = Region_p->PaletteValid;
    Region_p->PaletteValid = FALSE;
    ColorKeyNew = drv_OsdGetRegionTrueColor(Region_p, Color,NULL);
    Region_p->PaletteValid = PaletteValid;
    YS_BlitClaim();
	errCode = HI_GO_SetSurfaceColorKey(Region_p->SurfaceHandle, ColorKeyNew);       
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDRegionSetColorKey HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
    return ColorKeyOrg;
}

DRV_ErrCode DRV_OSDRegionGetColorKey(OSD_Region_t *Region_p,S32* ColorKey,U32* State)
{
    HI_COLOR ColorKeyNow;
    DRV_ErrCode errCode;
    if((REGION_IS_NULL)||(ColorKey == NULL)||(State == NULL))
        return -1;
	
    YS_BlitClaim();
	//errCode = HI_GO_GetSurfaceColorKey(Region_p->SurfaceHandle, &ColorKeyNow);      
	YS_BlitRelease();
	*ColorKey = Region_p->ColorKey;    
    *State = (U32)(Region_p->EnableCK);
    return 0;
}
DRV_ErrCode DRV_OSDRegionEnableColorKey(OSD_Region_t *Region_p)
{
    DRV_ErrCode errCode;
    if(REGION_IS_NULL)
        return -1;
    YS_BlitClaim();
	errCode = HI_GO_EnableSurfaceColorKey(Region_p->SurfaceHandle, TRUE);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDRegionEnableColorKey HI_GO_EnableSurfaceColorKey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
    Region_p->EnableCK = TRUE;
    return 0;
}

DRV_ErrCode DRV_OSDRegionDisableColorKey(OSD_Region_t *Region_p)
{
    DRV_ErrCode errCode;
    if(REGION_IS_NULL)
        return -1;
    YS_BlitClaim();
	errCode = HI_GO_EnableSurfaceColorKey(Region_p->SurfaceHandle, FALSE);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDRegionDisableColorKey HI_GO_EnableSurfaceColorKey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
    Region_p->EnableCK = FALSE;
    return 0;
}

DRV_ErrCode DRV_OSDEnableLayerColorKey(OSD_Region_t *Region_p)
{
    DRV_ErrCode errCode;
    HI_COLOR ColorKeyNow;
    HIGO_LAYER_KEY_S LayerColorKey;
    HI_BOOL PaletteValid;
    if(Region_p == NULL)
    {
        return DRV_OSD_ERROR_HANDLE;
    }
    LayerColorKey.bEnableCK = TRUE;
    PaletteValid = Region_p->PaletteValid;
    Region_p->PaletteValid = FALSE;
    LayerColorKey.ColorKey  = drv_OsdGetRegionTrueColor(Region_p, Region_p->LayerColorKey,NULL);
    Region_p->PaletteValid = PaletteValid;
    YS_BlitClaim();
    errCode = HI_GO_SetLayerColorkey(Region_p->LayerHandle,&LayerColorKey);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDEnableLayerColorKey HI_GO_SetLayerColorkey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
	Region_p->EnableLayerColorKey = TRUE;
    return (errCode);
}

DRV_ErrCode DRV_OSDDisableLayerColorKey(OSD_Region_t * Region_p)
{
    DRV_ErrCode errCode;
    HIGO_LAYER_KEY_S LayerColorKey;
    LayerColorKey.bEnableCK = FALSE;
    if(Region_p == NULL)
    {
        return DRV_OSD_ERROR_HANDLE;
    }
    YS_BlitClaim();
    errCode = HI_GO_SetLayerColorkey(Region_p->LayerHandle,&LayerColorKey);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDDisableLayerColorKey HI_GO_SetLayerColorkey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
    Region_p->EnableLayerColorKey = FALSE;
    return (errCode);
}
S32 DRV_OSDSetLayerColorKey(OSD_Region_t * Region_p, S32 Color)
{
    DRV_ErrCode errCode;
    HI_COLOR ColorKeyNew;
    HI_COLOR ColorKeyOrg;
    HIGO_LAYER_KEY_S LayerColorKey;
    HI_BOOL PaletteValid;
    if(Region_p == NULL)
    {
        return DRV_OSD_ERROR_HANDLE;
    }
    LayerColorKey.bEnableCK = TRUE;
    PaletteValid = Region_p->PaletteValid;
    Region_p->PaletteValid = FALSE;
    LayerColorKey.ColorKey  = drv_OsdGetRegionTrueColor(Region_p, Color,NULL);
    Region_p->PaletteValid = PaletteValid;
    YS_BlitClaim();
    errCode = HI_GO_SetLayerColorkey(Region_p->LayerHandle,&LayerColorKey);
	YS_BlitRelease();
    if (HI_SUCCESS != errCode) 
    {
        printf("DRV_OSDSetLayerColorKey HI_GO_SetLayerColorkey ErrCode 0x%08x\n",errCode); 
        return errCode;       
    }
    Region_p->LayerColorKey = Color;
	Region_p->EnableLayerColorKey = TRUE;
    return (errCode);
}

DRV_ErrCode DRV_OSDGetLayerColorKey(OSD_Region_t * Region_p, U32 *Color )
{
    DRV_ErrCode errCode;
    HIGO_LAYER_KEY_S LayerColorKey;
    if(REGION_IS_NULL)
        return -1;
    YS_BlitClaim();
    //HI_GO_GetLayerColorkey(Region_p->LayerHandle, &LayerColorKey);
	YS_BlitRelease();
    *Color = Region_p->LayerColorKey;
   return 0;
}

DRV_ErrCode DRV_OSDRegionFilterSet(OSD_Region_t* Region_p,OSD_FilterType_t filter)
{
    DRV_ErrCode errCode = DRV_NO_ERROR;
    
    if(errCode != ST_NO_ERROR)
    {
        OSD_INFO(("DRV_OSDRegionFilterSet errCode[0x%x]\n",errCode));
    }
    return DRV_NO_ERROR;
}

OSD_Palette_p DRV_OSDRegionPaletteSet(OSD_Region_t* Region_p, OSD_Palette_p Palette)
{
	/*OSD_Palette_p区别于调色板，通常Palette.Data_p才是指调色板*/
    OSD_Palette_p oldPalette = (OSD_Palette_p)NULL; 
    OSD_ColorARGB_t  argbArray;
    CAST_OSD_Color_t *PaletteData_p;
    S32 index, paletteLength;
    
    /* Check Handle validity */
    if (REGION_IS_NULL)
    {
        return oldPalette;
    }
    if(Palette == NULL)
    {
	    Region_p->PaletteValid = FALSE;
	    oldPalette = Region_p->PaletteColorArrary;
	    Region_p->PaletteColorArrary = Palette;
    }
    else
    {
	    PaletteData_p = (CAST_OSD_Color_t*)Palette;

	    for(index=0; index<256; index++)
	    {
	        if(PaletteData_p[index].Type == STOSD_COLOR_TYPE_YCRCB888)
	        {
	            drv_OsdYcrcb2Argb(&PaletteData_p[index], &argbArray);
	        }
	        else if(PaletteData_p[index].Type == STOSD_COLOR_TYPE_ARGB8888)
	        {
	            argbArray.Alpha = PaletteData_p[index].MixWeight;
	            argbArray.R = PaletteData_p[index].Value.Y;
	            argbArray.G = PaletteData_p[index].Value.Cr;
	            argbArray.B = PaletteData_p[index].Value.Cb;
	        }
	        else
	        {
	            argbArray.Alpha = 0x00;
	            argbArray.R = 0x00;
	            argbArray.G = 0x00;
	            argbArray.B = 0x00;
	        }
			*((U8 *)(Region_p->Palette.Data_p)+4*index+0) = argbArray.B;
			*((U8 *)(Region_p->Palette.Data_p)+4*index+1) = argbArray.G;
			*((U8 *)(Region_p->Palette.Data_p)+4*index+2) = argbArray.R;
			*((U8 *)(Region_p->Palette.Data_p)+4*index+3) = argbArray.Alpha;
	    }
	    
	    //YS_BlitClaim();
	 	//HI_GO_SetSurfacePalette(Region_p->SurfaceHandle, Palette);   
		//YS_BlitRelease();
	    Region_p->PaletteValid = TRUE;
	    oldPalette = Region_p->PaletteColorArrary;
	    Region_p->PaletteColorArrary = Palette;
    }
    return oldPalette;
}


DRV_ErrCode DRV_OSDRegionShow(OSD_Region_t *Region_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	
	/* Check Handle validity */
	if (REGION_IS_NULL)
	{
	return(DRV_ERROR_DRVLAYER);
	}

	if (Region_p->ShowSurfaceVisible == FALSE)
	{
		//HI_GO_SetWindowOpacity(Region_p->WindowHandle[Region_p->LayerID],0xFF);
		if ( ErrCode != ST_NO_ERROR)
		{
			OSD_ERROR (("Start DRV_OSDRegionShow \n"));        
			return (DRV_ERROR_DRVLAYER);
		}
	}

	Region_p->ShowSurfaceVisible = TRUE;

	return (DRV_NO_ERROR);
}


DRV_ErrCode DRV_OSDRegionHide(OSD_Region_t *Region_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	
	/* Check Handle validity */
	if (REGION_IS_NULL)
	{
		return(DRV_ERROR_DRVLAYER);
	}

	if (Region_p->ShowSurfaceVisible == TRUE)
	{
		//HI_GO_SetWindowOpacity (Region_p->WindowHandle[Region_p->LayerID],0x0);
		if ( ErrCode != ST_NO_ERROR)
		{
			OSD_ERROR (("Start DRV_OSDRegionHide \n"));        
			return (DRV_ERROR_DRVLAYER);
		}
	}

	Region_p->ShowSurfaceVisible = FALSE;

	return (DRV_NO_ERROR);
}

DRV_ErrCode DRV_OSDRegionMixWeightGet(OSD_Region_t* Region_p,U8 *MixWeight)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    HI_U8 Alpha;
    
    /* Check Handle validity */
    if (REGION_IS_NULL)
    {
        return(DRV_ERROR_DRVLAYER);
    }
   
    YS_BlitClaim();
	ErrCode = HI_GO_GetSurfaceAlpha(Region_p->SurfaceHandle,&Alpha);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDRegionMixWeightGet HI_GO_GetSurfaceAlpha ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
	*MixWeight=(Alpha)/4;
   
    return (DRV_NO_ERROR);
}

DRV_ErrCode DRV_OSDRegionMixWeightSet(OSD_Region_t* Region_p,U8 MixWeight)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    HI_U8 Alpha;
    /* Check Handle validity */
    if (REGION_IS_NULL)
    {
        return(DRV_ERROR_DRVLAYER);
    }
    if(MixWeight*4 > 255)
        Alpha = 255;
    else    
    	Alpha = MixWeight*4;
    YS_BlitClaim();
	ErrCode = HI_GO_SetSurfaceAlpha(Region_p->SurfaceHandle,Alpha);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDRegionMixWeightSet HI_GO_SetSurfaceAlpha ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }

    Region_p->MixWeight = MixWeight;

    return (DRV_NO_ERROR);
}
/*******************************************************************************
Name        : DRV_OSDPixelDraw()
Description :  
Parameters  : 
Assumptions : 
Limitations :
Returns     : 
*******************************************************************************/
DRV_ErrCode DRV_OSDPixelDraw(OSD_Region_t* Region_p,
                                             S32 PositionX,
                                             S32 PositionY,
                                             S32 Color)
{
    DRV_ErrCode             ErrCode = ST_NO_ERROR;
    HI_COLOR TrueColor;
    if(REGION_IS_NULL)
        return -1;

    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    YS_BlitClaim();
	ErrCode = HI_GO_DrawLine(Region_p->SurfaceHandle, PositionX, PositionY, PositionX, PositionY,TrueColor);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDPixelDraw HI_GO_DrawLine ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}

DRV_ErrCode DRV_OSDGetPixelColor(OSD_Region_t* Region_p,
                                             S32 PositionX,
                                             S32 PositionY,
                                             S32* Color)
{
    DRV_ErrCode             ErrCode = ST_NO_ERROR;
    return ErrCode;
}

DRV_ErrCode DRV_OSDHorizontalLineDraw(OSD_Region_t * Region_p, 
                                                               S32 PositionX, 
                                                               S32 XRange, 
                                                               S32 PositionY,
                                                               S32 Color)
{
    DRV_ErrCode             ErrCode = ST_NO_ERROR;
    HI_COLOR TrueColor;
    HI_RECT Rect;
    if(REGION_IS_NULL)
        return -1;
    if(XRange == 0)
    	return -1;
    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
	YS_BlitClaim();
	ErrCode = HI_GO_DrawLine(Region_p->SurfaceHandle,PositionX,PositionY, PositionX + XRange, PositionY, TrueColor);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDHorizontalLineDraw HI_GO_DrawLine ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}

DRV_ErrCode DRV_OSDVerticalLineDraw(OSD_Region_t* Region_p, 
                                                               S32 PositionX, 
                                                               S32 PositionY, 
                                                               S32 YRange,
                                                               S32 Color)
{
    DRV_ErrCode             ErrCode = ST_NO_ERROR;
    HI_COLOR TrueColor;
    HI_RECT Rect;
    
    if(REGION_IS_NULL)
        return -1;
    if(YRange == 0)
    	return -1;
    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
	YS_BlitClaim();
	ErrCode = HI_GO_DrawLine(Region_p->SurfaceHandle,PositionX,PositionY,PositionX, PositionY + YRange, TrueColor);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDVerticalLineDraw HI_GO_DrawLine ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}

/*************************************************
* Function:       
* Description:    draw a line
* Input:          
* Output:        
* Return:         
* Others:        
*************************************************/
DRV_ErrCode DRV_OSDLineDraw(OSD_Region_t* Region_p, 
                                              S32 XStart, 
                                              S32 YStart, 
                                              S32 XEnd, 
                                              S32 YEnd,
                                              S32 Color)
{
    DRV_ErrCode             ErrCode = ST_NO_ERROR;
    HI_COLOR TrueColor;
    
    if(REGION_IS_NULL)
        return -1;
    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    YS_BlitClaim();
	ErrCode = HI_GO_DrawLine(Region_p->SurfaceHandle,XStart,YStart,XEnd,YEnd, TrueColor);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDLineDraw HI_GO_DrawLine ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}
/*画一个矩形框*/
DRV_ErrCode DRV_OSDRectDraw(OSD_Region_t* Region_p, S32 X0, S32 Y0, S32 X1, S32 Y1, S32 Color)
{
	DRV_ErrCode                  ErrCode = ST_NO_ERROR;
	HI_COLOR TrueColor;
	HI_RECT Rect;
	if(REGION_IS_NULL)
	return -1;
	if( (X0 == X1) && (Y0 == Y1) )
	{
		ErrCode = DRV_OSDPixelDraw(Region_p, X0, Y0, Color);
	}
	else
	{
		Rect.x = X0;
		Rect.y = Y0;
		Rect.w = X1 - X0;
		Rect.h = Y1 - Y0;
    	TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    	YS_BlitClaim();
		ErrCode = HI_GO_DrawRect(Region_p->SurfaceHandle, &Rect, TrueColor);
		YS_BlitRelease();
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDRectDraw HI_GO_DrawRect ErrCode 0x%08x\n",ErrCode); 
	        return ErrCode;       
	    }
	}
	return ErrCode;
}

/*画一个矩形，与上述接口是一个是画框一个是画矩形*/
DRV_ErrCode DRV_OSDRectFillDraw(OSD_Region_t* Region_p, S32 X0, S32 Y0, S32 X1, S32 Y1, S32 Color)
{
    DRV_ErrCode                  ErrCode = ST_NO_ERROR;	
    HI_RECT Rect;
    HI_COLOR TrueColor;
    if(REGION_IS_NULL)
        return;
    if((X0 == X1) || (Y0 == Y1))//画点或者画线
    {
	    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    	YS_BlitClaim();
		ErrCode = HI_GO_DrawLine(Region_p->SurfaceHandle, X0, Y0, X1, Y1,TrueColor);
		YS_BlitRelease();
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDRectFillDraw HI_GO_DrawLine ErrCode 0x%08x\n",ErrCode); 
	        return ErrCode;       
	    }
    }
    else
    {
    	if(X1 > X0)
    	{
	 	    Rect.x = X0;
		    Rect.w = X1 - X0;
    	}
    	else
    	{
	 	    Rect.x = X1;
		    Rect.w = X0 - X1;
    	}
    	if(Y1 > Y0)
    	{
		    Rect.y = Y0;
		    Rect.h = Y1 - Y0;
    	}
    	else
    	{
		    Rect.y = Y1;
		    Rect.h = Y0 - Y1;
    	}
    	TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
    	YS_BlitClaim();
	    ErrCode = HI_GO_FillRect(Region_p->SurfaceHandle,&Rect,TrueColor,HIGO_COMPOPT_NONE);
		YS_BlitRelease();
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDRectFillDraw HI_GO_FillRect ErrCode 0x%08x\n",ErrCode); 
	        return ErrCode;       
	    }
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}

DRV_ErrCode DRV_OSDCircleFillDraw(OSD_Region_t* Region_p, S32 x_centre, S32 y_centre, S32 x_semi_axis, S32 y_semi_axis, S32 Color)
#if 1
{    
  DRV_ErrCode ErrCode = DRV_NO_ERROR;
  S32  max_points_per_quadrant;
  S32  angular_increment;
  S32  angle;
  S32  normalised_angle;
  S32  x;
  S32  y;
  S32  last_y = 9999;
   
  max_points_per_quadrant = 2 * OS_MAX(x_semi_axis,y_semi_axis);    /* Reduce number of times round loop for small axes */
  angular_increment = 0x10000 / max_points_per_quadrant;

  for( angle=0; angle<0x10000; angle+=angular_increment )
{
    normalised_angle = angle >> 7;              /* Normalised parametric equations */
    x = (x_semi_axis * OSD_COS(normalised_angle)) >> 15;
    y = (y_semi_axis * OSD_SIN(  normalised_angle)) >> 15;

    if( y != last_y )
    {
      DRV_OSDHorizontalLineDraw(Region_p, x_centre - x,  2*x, y_centre - y,Color); /* Fill above and below x axis */
      DRV_OSDHorizontalLineDraw(Region_p, x_centre - x, 2*x, y_centre + y, Color);
    
      last_y = y;                       /* Don't fill these again */
    }
  }
}
#else
{    
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    S32 TrueColor;
    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);

    YS_BlitClaim();
    if(x_semi_axis == y_semi_axis)
    {
		ErrCode = HI_GO_DrawCircle(Region_p->SurfaceHandle, x_centre, y_centre, x_semi_axis, Color); 
	}	
	else
	{
		ErrCode = HI_GO_DrawEllipse(Region_p->SurfaceHandle, x_centre, y_centre, x_semi_axis, y_centre, Color);
	}	
	YS_BlitRelease();

    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDCircleFillDraw HI_GO_DrawCircle ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }

#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}
#endif
DRV_ErrCode DRV_OSDGetRegionCreateSize(OSD_Region_t * Region_p,OSD_Rect_t * Rect)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	
    if(REGION_IS_NULL)
        return DRV_ERROR_BAD_PARAMETER;

	Rect->PositionX = 0;
	Rect->PositionY = 0;
	Rect->Width=Region_p->CreatedWidth;
	Rect->Height = Region_p->CreatedHeight;
	return DRV_NO_ERROR;
}

DRV_ErrCode DRV_OSDRegionFill(OSD_Region_t*Region_p, S32 Color)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	
    if (REGION_IS_NULL)
        return -1;

    ErrCode = drv_OsdFillRectangle(Region_p, 
                               0,
                               0,
                               Region_p->CreatedWidth, 
                               Region_p->CreatedHeight, 
                               Color);
	return ErrCode;                               
}

DRV_ErrCode DRV_OSDAreaGet(OSD_Region_t *Region_p, S32 x, S32 y, S32 width,S32 height,S32 *area)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    OSD_Bitmap_t *bitmap = NULL;
    HI_RECT SrcRect;
    HI_RECT DstRect;
    HI_HANDLE Surface;
	HI_PIXELDATA pData; 
	HIGO_PF_E PixelFormat;
    HIGO_BLTOPT_S BltOpt = {0};
    if ((area == NULL)||REGION_IS_NULL)
    {
        return DRV_ERROR_BAD_PARAMETER;
    }
    *area = (S32)0;

    bitmap = (OSD_Bitmap_t*)DRV_OSMalloc(mOsdDevice.CPUPartition_p, sizeof(OSD_Bitmap_t));
    if (NULL == bitmap) 
    {
    	printf("DRV_OSDAreaGet DRV_OSMalloc err!\n");
        return(DRV_ERROR_DRVLAYER);
    }
    
    switch(Region_p->ColorType)
    {
        case 8:
		    PixelFormat = HIGO_PF_CLUT8;
            break;
        case OSD_COLOR_TYPE_ARGB4444:
		    PixelFormat = HIGO_PF_4444;
            break;
        case OSD_COLOR_TYPE_RGB565:
		    PixelFormat = HIGO_PF_565;
            break;
        case OSD_COLOR_TYPE_ARGB1555:
		    PixelFormat = HIGO_PF_1555;
            break;
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
		    PixelFormat = HIGO_PF_8888;
            break;
        case OSD_COLOR_TYPE_ARGB8888:
		    PixelFormat = HIGO_PF_8888;
            break;
    }
    
    YS_BlitClaim();
	ErrCode = HI_GO_CreateSurface(width,height,PixelFormat,&Surface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDAreaGet HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
    }
   	ErrCode = HI_GO_LockSurface(Surface, pData, HI_TRUE);//修正相关参数
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDAreaGet HI_GO_LockSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
    }
   	bitmap->Pitch  = pData[0].Pitch;
	bitmap->Data_p = pData[0].pData;
   	ErrCode = HI_GO_UnlockSurface(Surface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDAreaGet HI_GO_UnlockSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
    }	
	bitmap->SurfaceHandle = Surface;
	bitmap->ColorType     = Region_p->ColorType;
	bitmap->BitsPerPel    = Region_p->NbBitsPerPel;
	bitmap->Width         = width;
	bitmap->Height        = height;
	SrcRect.x = x;
	SrcRect.y = y;
	SrcRect.w = width;
	SrcRect.h = height;
	DstRect.x = 0;
	DstRect.y = 0;
	DstRect.w = width;
	DstRect.h = height;
	ErrCode = HI_GO_Blit(Region_p->SurfaceHandle, &SrcRect, Surface,&DstRect, &BltOpt);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDAreaGet HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		return HI_FAILURE; 
    }
    *area = (S32)bitmap;
   return ErrCode;    
}

DRV_ErrCode DRV_OSDAreaShow(OSD_Region_t *Region_p, S32 x, S32 y, S32 *area)
{
    DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Bitmap_t *bitmap = NULL;
	HI_RECT SrcRect,DstRect;
	HIGO_BLTOPT_S BlitOpt = {0};
	
    if (REGION_IS_NULL)
        return DRV_ERROR_DRVLAYER;
	
    if (NULL == area)
    {
		Err_Print("[DRV_OSDAreaShow] area is NULL!\n");
        return DRV_ERROR_DRVLAYER;
    }

    bitmap = (OSD_Bitmap_t*)(*area);
    
	if(bitmap == NULL)
	{
		Err_Print("[ERROR]DRV_OSDAreaShow bitmap == NULL\n\n");
		return DRV_ERROR_DRVLAYER;
	}
	
    if (bitmap->SurfaceHandle != 0)
    {
		SrcRect.x = 0;
		SrcRect.y = 0;
		SrcRect.w = bitmap->Width;
		SrcRect.h = bitmap->Height;
       
		DstRect.x = x;
		DstRect.y = y;
		DstRect.w = bitmap->Width;
		DstRect.h = bitmap->Height;
    	YS_BlitClaim();
		ErrCode = HI_GO_Blit(bitmap->SurfaceHandle,&SrcRect,Region_p->SurfaceHandle,&DstRect, &BlitOpt);
		YS_BlitRelease();
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDAreaShow HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			return HI_FAILURE; 
	    }
	}
	else
	{
		Err_Print("[DRV_OSDAreaShow] bitmap->SurfaceHandle is NULL!\n");
	}
    
 #ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
   return ErrCode;
}

DRV_ErrCode DRV_OSDAreaClose(S32 *area)
{
    DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Bitmap_t *bitmap = NULL;
	
    if (NULL == area)
        return DRV_ERROR_DRVLAYER;
    
    bitmap = (OSD_Bitmap_t*)(*area);
	if(bitmap == NULL)
	{
		Err_Print("[DRV_OSDAreaClose] bitmap == NULL\n");
		return DRV_ERROR_DRVLAYER;
	}
    YS_BlitClaim();
	ErrCode = HI_GO_FreeSurface(bitmap->SurfaceHandle);      
	YS_BlitRelease();
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDAreaClose HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		return HI_FAILURE; 
	}
    bitmap->SurfaceHandle == 0;
    *area = NULL;   
    DRV_OSFree(mOsdDevice.CPUPartition_p, bitmap);

    return ErrCode;
}

DRV_ErrCode DRV_OSDAreaRestore(OSD_Region_t *Region_p, S32 x, S32 y, S32 *area)
{
    DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	
  if (REGION_IS_NULL)
	{
		Err_Print("[DRV_OSDAreaRestore] region is NULL!\n");
    return ErrCode;
	}
  
    ErrCode = DRV_OSDAreaShow(Region_p, x, y, area);
    ErrCode = DRV_OSDAreaClose(area);
    
    return ErrCode;
}
/*由于历史原因，我们打图工具打出来的头的结构为STOSD_Bitmap_t，底层需要转换层OSD_Bitmap_t*/
DRV_ErrCode drv_OsdBitmapStToDrv(STOSD_Bitmap_t *StBitmap_p,OSD_Bitmap_t *DrvBitmap_p)
{
    DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	DrvBitmap_p->ColorType  = StBitmap_p->ColorType;
	switch(StBitmap_p->ColorType)
	{
		case STOSD_COLOR_TYPE_PALETTE:
        	DrvBitmap_p->ColorType = OSD_COLOR_TYPE_CLUT8;
        	break;
        case STOSD_COLOR_TYPE_ARGB4444:
        	DrvBitmap_p->ColorType = OSD_COLOR_TYPE_ARGB4444;
        	break;
        case STOSD_COLOR_TYPE_ARGB1555:
        	DrvBitmap_p->ColorType = OSD_COLOR_TYPE_ARGB1555;
        	break;
        case STOSD_COLOR_TYPE_RGB565:
        	DrvBitmap_p->ColorType = OSD_COLOR_TYPE_RGB565;
        	break;
        case STOSD_COLOR_TYPE_ARGB8888:
        	DrvBitmap_p->ColorType = OSD_COLOR_TYPE_ARGB8888;
        	break;
    }
	DrvBitmap_p->BitsPerPel = StBitmap_p->BitsPerPel;
	DrvBitmap_p->Width      = StBitmap_p->Width;
	DrvBitmap_p->Height     = StBitmap_p->Height;
	DrvBitmap_p->Data_p     = StBitmap_p->Data_p;
	DrvBitmap_p->Pitch      = DrvBitmap_p->BitsPerPel*DrvBitmap_p->Width/8;
	return ErrCode;
}


/*实现颜色混合填充*/
DRV_ErrCode DRV_OSDALU(OSD_Bitmap_t *Dest_Bitmap_p, OSD_Bitmap_t *fg_Bitmap_p,OSD_Bitmap_t *bg_Bitmap_p,DRV_RectWh *Dest_Rect_p,DRV_RectWh *fg_Rect_p,DRV_RectWh *bg_Rect_p,HIGO_COMPOPT_E Mode)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT bgRect,DstRect,fgRect;
	HI_HANDLE DestMemSurface,fgMemSurface,bgMemSurface,DestSurface,fgSurface,bgSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HIGO_BLTOPT_S Dest_BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Dest_Bitmap_p)||(NULL == fg_Bitmap_p)||(NULL == bg_Bitmap_p)||(NULL == fg_Rect_p)||(NULL == Dest_Rect_p)||(NULL == bg_Rect_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Dest_Bitmap_p->Height;
	    SurInfo.Width       = Dest_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Dest_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Dest_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Dest_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &DestMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&DestSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDALU HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(DestMemSurface,NULL,DestSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

    {
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = fg_Bitmap_p->Height;
	    SurInfo.Width       = fg_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = fg_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(fg_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(fg_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &fgMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&fgSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDALU HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode |= HI_GO_FreeSurface(fgMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(fgMemSurface,NULL,fgSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode |= HI_GO_FreeSurface(fgSurface);
			ErrCode |= HI_GO_FreeSurface(fgMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}

		ErrCode = HI_GO_FreeSurface(fgMemSurface);
	}

    {
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = bg_Bitmap_p->Height;
	    SurInfo.Width       = bg_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = bg_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(bg_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(bg_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &bgMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&bgSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDALU HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode |= HI_GO_FreeSurface(fgSurface);
			ErrCode |= HI_GO_FreeSurface(bgMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(bgMemSurface,NULL,bgSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode |= HI_GO_FreeSurface(fgSurface);
			ErrCode |= HI_GO_FreeSurface(bgSurface);
			ErrCode |= HI_GO_FreeSurface(bgMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}

		ErrCode = HI_GO_FreeSurface(bgMemSurface);
	}

	
	
	fgRect.x = fg_Rect_p->x;
	fgRect.y = fg_Rect_p->y;
	fgRect.w = fg_Rect_p->nWidth;
	fgRect.h = fg_Rect_p->nHeight;

	bgRect.x = bg_Rect_p->x;
	bgRect.y = bg_Rect_p->y;
	bgRect.w = bg_Rect_p->nWidth;
	bgRect.h = bg_Rect_p->nHeight;
   
	DstRect.x = Dest_Rect_p->x;
	DstRect.y = Dest_Rect_p->y;
	DstRect.w = Dest_Rect_p->nWidth;
	DstRect.h = Dest_Rect_p->nHeight;

	BlitOpt.EnableScale = TRUE;
	BlitOpt.EnableGlobalAlpha = TRUE;
	BlitOpt.PixelAlphaComp = Mode;
	ErrCode = HI_GO_Blit(fgSurface,&fgRect,bgSurface,&bgRect,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DestSurface);
		ErrCode |= HI_GO_FreeSurface(DestMemSurface);
		ErrCode |= HI_GO_FreeSurface(fgSurface);
		ErrCode |= HI_GO_FreeSurface(bgSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}

	ErrCode = HI_GO_Blit(bgSurface,&bgRect,DestSurface,&DstRect,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DestSurface);
		ErrCode |= HI_GO_FreeSurface(DestMemSurface);
		ErrCode |= HI_GO_FreeSurface(fgSurface);
		ErrCode |= HI_GO_FreeSurface(bgSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	
	ErrCode = HI_GO_Blit(DestSurface,NULL,DestMemSurface,NULL,&Dest_BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDALU HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DestSurface);
		ErrCode |= HI_GO_FreeSurface(DestMemSurface);
		ErrCode |= HI_GO_FreeSurface(fgSurface);
		ErrCode |= HI_GO_FreeSurface(bgSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(DestSurface);
	ErrCode |= HI_GO_FreeSurface(DestMemSurface);
	ErrCode |= HI_GO_FreeSurface(fgSurface);
	ErrCode |= HI_GO_FreeSurface(bgSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDALU HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


/*实现把目标颜色Alpha混合到特定区域*/
DRV_ErrCode DRV_OSDFillRectangle(OSD_Bitmap_t *Dest_Bitmap_p, DRV_RectWh *Dest_Rect_p,HI_COLOR Color,U8 Alpha)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE DestMemSurface,DestSurface,SrcSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HIGO_BLTOPT_S Dest_BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Dest_Bitmap_p)||(NULL == Dest_Rect_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Dest_Bitmap_p->Height;
	    SurInfo.Width       = Dest_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Dest_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Dest_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Dest_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &DestMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDFillRectangle HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&DestSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangle HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(DestMemSurface,NULL,DestSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDFillRectangle HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

    {
		SrcRect.x  = Dest_Rect_p->x;
		SrcRect.y  = Dest_Rect_p->y;
		SrcRect.w  = Dest_Rect_p->nWidth;
		SrcRect.h  = Dest_Rect_p->nHeight;
		ErrCode = HI_GO_CreateSurface(SrcRect.w, SrcRect.h, SurInfo.PixelFormat,&SrcSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangle HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
	    ErrCode = HI_GO_FillRect(SrcSurface, &SrcRect, Color, HIGO_COMPOPT_NONE);
	    ErrCode |= HI_GO_SetSurfaceAlpha(SrcSurface,Alpha);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangle HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			HI_GO_FreeSurface(SrcSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
	    
    	BlitOpt.EnableScale = TRUE;
    	BlitOpt.EnableGlobalAlpha = TRUE;
    	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_AKD;
    	ErrCode = HI_GO_Blit(SrcSurface,NULL,DestSurface,&SrcRect,&BlitOpt);
    	if (HI_SUCCESS != ErrCode)
    	{
    		printf("DRV_OSDFillRectangle HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			HI_GO_FreeSurface(SrcSurface);
    		YS_BlitRelease();
    		return HI_FAILURE; 
    	}
	}
	
	ErrCode = HI_GO_Blit(DestSurface,NULL,DestMemSurface,NULL,&Dest_BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDFillRectangle HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		HI_GO_FreeSurface(DestSurface);
		HI_GO_FreeSurface(DestMemSurface);
		HI_GO_FreeSurface(SrcSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(DestSurface);
	ErrCode |= HI_GO_FreeSurface(DestMemSurface);
	ErrCode |= HI_GO_FreeSurface(SrcSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDFillRectangle HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}



/*实现把目标颜色混合到特定区域*/
DRV_ErrCode DRV_OSDFillRectangleEx(OSD_Bitmap_t *Dest_Bitmap_p, DRV_RectWh *Dest_Rect_p,HI_COLOR Color,HIGO_COMPOPT_E Mode)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE DestMemSurface,DestSurface,SrcSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HIGO_BLTOPT_S Dest_BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Dest_Bitmap_p)||(NULL == Dest_Rect_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Dest_Bitmap_p->Height;
	    SurInfo.Width       = Dest_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Dest_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Dest_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Dest_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &DestMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDFillRectangleEx HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&DestSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangleEx HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(DestMemSurface,NULL,DestSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDFillRectangleEx HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

    {
		SrcRect.x  = Dest_Rect_p->x;
		SrcRect.y  = Dest_Rect_p->y;
		SrcRect.w  = Dest_Rect_p->nWidth;
		SrcRect.h  = Dest_Rect_p->nHeight;
		ErrCode = HI_GO_CreateSurface(SrcRect.w, SrcRect.h, SurInfo.PixelFormat,&SrcSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangleEx HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
	    ErrCode = HI_GO_FillRect(SrcSurface, &SrcRect, Color, HIGO_COMPOPT_NONE);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDFillRectangleEx HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			HI_GO_FreeSurface(SrcSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
	    
    	BlitOpt.EnableScale = TRUE;
    	BlitOpt.EnableGlobalAlpha = TRUE;
    	BlitOpt.PixelAlphaComp = Mode;
    	ErrCode = HI_GO_Blit(SrcSurface,NULL,DestSurface,&SrcRect,&BlitOpt);
    	if (HI_SUCCESS != ErrCode)
    	{
    		printf("DRV_OSDFillRectangleEx HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(DestSurface);
			HI_GO_FreeSurface(DestMemSurface);
			HI_GO_FreeSurface(SrcSurface);
    		YS_BlitRelease();
    		return HI_FAILURE; 
    	}
	}
	
	ErrCode = HI_GO_Blit(DestSurface,NULL,DestMemSurface,NULL,&Dest_BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDFillRectangleEx HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		HI_GO_FreeSurface(DestSurface);
		HI_GO_FreeSurface(DestMemSurface);
		HI_GO_FreeSurface(SrcSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(DestSurface);
	ErrCode |= HI_GO_FreeSurface(DestMemSurface);
	ErrCode |= HI_GO_FreeSurface(SrcSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDFillRectangleEx HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


/*实现bitmap数据块的放大*/
DRV_ErrCode DRV_OSDBitmapScale(OSD_Bitmap_t *Bitmap_p, OSD_Bitmap_t *Dst_Bitmap_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT Rect,DstRect;
	HI_HANDLE MemSurface,Surface,DstMemSurface,DstSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Bitmap_p)||(NULL == Dst_Bitmap_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Bitmap_p->Height;
	    SurInfo.Width       = Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapScale HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
 		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDBitmapScale HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
	    	ErrCode = HI_GO_FreeSurface(MemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }       
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,NULL,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapScale HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

    {
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Dst_Bitmap_p->Height;
	    SurInfo.Width       = Dst_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Dst_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Dst_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Dst_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &DstMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapScale HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&DstSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDBitmapScale HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			ErrCode |= HI_GO_FreeSurface(DstMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }

		ErrCode = HI_GO_Blit(DstMemSurface,NULL,DstSurface,NULL,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapScale HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			ErrCode |= HI_GO_FreeSurface(DstSurface);
			ErrCode |= HI_GO_FreeSurface(DstMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	
	BlitOpt.EnableScale = TRUE;
	ErrCode = HI_GO_Blit(Surface,NULL,DstSurface,NULL,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapScale HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(Surface);
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		ErrCode |= HI_GO_FreeSurface(DstSurface);
		ErrCode |= HI_GO_FreeSurface(DstMemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}

	ErrCode = HI_GO_Blit(DstSurface,NULL,DstMemSurface,NULL,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapScale HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(Surface);
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		ErrCode |= HI_GO_FreeSurface(DstSurface);
		ErrCode |= HI_GO_FreeSurface(DstMemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}

	
    ErrCode = HI_GO_FreeSurface(Surface);
	ErrCode |= HI_GO_FreeSurface(MemSurface);
	ErrCode |= HI_GO_FreeSurface(DstSurface);
	ErrCode |= HI_GO_FreeSurface(DstMemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapScale HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


/*实现bitmap数据块的拷贝*/
DRV_ErrCode DRV_OSDBitmapBlt(OSD_Bitmap_t *Dest_Bitmap_p, OSD_Bitmap_t *Src_Bitmap_p,DRV_RectWh *Dest_Rect_p,DRV_RectWh *Src_Rect_p,U8 Alpha)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE DestMemSurface,SrcMemSurface,DestSurface,SrcSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HIGO_BLTOPT_S Dest_BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Dest_Bitmap_p)||(NULL == Src_Bitmap_p)||(NULL == Dest_Rect_p)||(NULL == Src_Rect_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Dest_Bitmap_p->Height;
	    SurInfo.Width       = Dest_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Dest_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Dest_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Dest_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &DestMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapBlt HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&DestSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDBitmapBlt HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(DestMemSurface,NULL,DestSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapBlt HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

    {
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Src_Bitmap_p->Height;
	    SurInfo.Width       = Src_Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Src_Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Src_Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Src_Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &SrcMemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapBlt HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&SrcSurface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDBitmapBlt HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode |= HI_GO_FreeSurface(SrcMemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(SrcMemSurface,NULL,SrcSurface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapBlt HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(DestSurface);
			ErrCode |= HI_GO_FreeSurface(DestMemSurface);
			ErrCode = HI_GO_FreeSurface(SrcSurface);
			ErrCode |= HI_GO_FreeSurface(SrcMemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}

	
	
	SrcRect.x = Src_Rect_p->x;
	SrcRect.y = Src_Rect_p->y;
	SrcRect.w = Src_Rect_p->nWidth;
	SrcRect.h = Src_Rect_p->nHeight;
   
	DstRect.x = Dest_Rect_p->x;
	DstRect.y = Dest_Rect_p->y;
	DstRect.w = Dest_Rect_p->nWidth;
	DstRect.h = Dest_Rect_p->nHeight;

	ErrCode = HI_GO_SetSurfaceAlpha(SrcSurface,Alpha);       
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDBitmapBlt HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",ErrCode); 
    }

	BlitOpt.EnableScale = TRUE;
	BlitOpt.EnableGlobalAlpha = TRUE;
	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_SRC;
	ErrCode = HI_GO_Blit(SrcSurface,&SrcRect,DestSurface,&DstRect,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapBlt HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DestSurface);
		ErrCode |= HI_GO_FreeSurface(DestMemSurface);
		ErrCode = HI_GO_FreeSurface(SrcSurface);
		ErrCode |= HI_GO_FreeSurface(SrcMemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}

	ErrCode = HI_GO_Blit(DestSurface,NULL,DestMemSurface,NULL,&Dest_BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapBlt HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DestSurface);
		ErrCode |= HI_GO_FreeSurface(DestMemSurface);
		ErrCode = HI_GO_FreeSurface(SrcSurface);
		ErrCode |= HI_GO_FreeSurface(SrcMemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(DestSurface);
	ErrCode |= HI_GO_FreeSurface(DestMemSurface);
	ErrCode = HI_GO_FreeSurface(SrcSurface);
	ErrCode |= HI_GO_FreeSurface(SrcMemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapBlt HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


/*实现bitmap数据块的拷贝*/
DRV_ErrCode DRV_OSDBitmapFill(OSD_Bitmap_t *Bitmap_p, DRV_RectWh *Rect_p,U32 Color)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT Rect;
	HI_HANDLE MemSurface,Surface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HIGO_BLTOPT_S Dest_BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if((NULL == Bitmap_p)||(NULL == Rect_p))
        return -1;
	YS_BlitClaim();

	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Bitmap_p->Height;
	    SurInfo.Width       = Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapFill HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
 		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDBitmapFill HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
	    	ErrCode = HI_GO_FreeSurface(MemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }       
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,NULL,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapFill HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
    	Rect.x = Rect_p->x;
    	Rect.y = Rect_p->y;
    	Rect.w = Rect_p->nWidth;
    	Rect.h = Rect_p->nHeight;
        ErrCode = HI_GO_FillRect(Surface,&Rect,Color,HIGO_COMPOPT_NONE);   
        if (HI_SUCCESS != ErrCode) 
        {
            printf("DRV_OSDBitmapFill HI_GO_FillRect ErrCode 0x%08x\n",ErrCode); 
            ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
            return ErrCode;       
        }
		ErrCode = HI_GO_Blit(Surface,NULL,MemSurface,NULL,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDBitmapFill HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
	}
	}
    ErrCode = HI_GO_FreeSurface(Surface);
	ErrCode |= HI_GO_FreeSurface(MemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmap HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


DRV_ErrCode DRV_OSDBitmapDraw(OSD_Region_t *Region_p, S32 PositionX, S32 PositionY, STOSD_Bitmap_t *Bitmap_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	OSD_Bitmap_t DrvBitmap;
	HI_HANDLE Surface,MemSurface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
    HI_HANDLE hDecoder;
    HIGO_DEC_ATTR_S stSrcDesc;
	
   if(REGION_IS_NULL||NULL == Bitmap_p)
        return -1;

    ErrCode = drv_OsdBitmapStToDrv(Bitmap_p,&DrvBitmap);
    if(Bitmap_p->ColorType == STOSD_COLOR_TYPE_PALETTE)
    	DrvBitmap.ColorType = Region_p->ColorType;

    SurInfo.MemType     = HIGO_MEMTYPE_OS;
    SurInfo.Height      = DrvBitmap.Height;
    SurInfo.Width       = DrvBitmap.Width;
    SurInfo.Pitch[0]    = DrvBitmap.Pitch;
    SurInfo.pVirAddr[0] = (HI_CHAR*)(DrvBitmap.Data_p);
    switch(DrvBitmap.ColorType)
    {
        case 8:
		    SurInfo.PixelFormat = HIGO_PF_CLUT8;
            break;
        case 4444:
        case OSD_COLOR_TYPE_ARGB4444:
		    SurInfo.PixelFormat = HIGO_PF_4444;
            break;
        case OSD_COLOR_TYPE_RGB565:
		    SurInfo.PixelFormat = HIGO_PF_565;
            break;
        case OSD_COLOR_TYPE_ARGB1555:
		    SurInfo.PixelFormat = HIGO_PF_1555;
            break;
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
		    SurInfo.PixelFormat = HIGO_PF_8888;
            break;
        case OSD_COLOR_TYPE_ARGB8888:
		    SurInfo.PixelFormat = HIGO_PF_8888;
            break;
    }
    YS_BlitClaim();
    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapDraw HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("[DRV_OSDBitmapDraw] HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
    	ErrCode = HI_GO_FreeSurface(MemSurface);
    	YS_BlitRelease();
		return HI_FAILURE; 
    }
	DstRect.x       = 0;
	DstRect.y       = 0;
	DstRect.w  = SurInfo.Width;
	DstRect.h  = SurInfo.Height;
	ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,&DstRect,&BlitOpt);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapDraw HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(Surface);
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
    DrvBitmap.SurfaceHandle = Surface;
	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = Bitmap_p->Width;
	SrcRect.h = Bitmap_p->Height;
   
	DstRect.x = PositionX;
	DstRect.y = PositionY;
	DstRect.w = Bitmap_p->Width;
	DstRect.h = Bitmap_p->Height;
	
    if((Region_p->EnableCK == TRUE)&&(Region_p->ColorKey != 0))
    {
	    HI_COLOR ColorKeyNew;
	    HI_BOOL PaletteValid;
	    PaletteValid = Region_p->PaletteValid;
	    Region_p->PaletteValid = FALSE;
	    ColorKeyNew = drv_OsdGetRegionTrueColor(Region_p, Region_p->ColorKey,NULL);
	    Region_p->PaletteValid = PaletteValid;
		ErrCode = HI_GO_SetSurfaceColorKey(DrvBitmap.SurfaceHandle, ColorKeyNew);       
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDBitmapDraw HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",ErrCode); 
	    }
	    BlitOpt.ColorKeyFrom   = HIGO_CKEY_SRC;
    }	
	ErrCode = HI_GO_Blit(DrvBitmap.SurfaceHandle,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	if(ErrCode == HIGO_ERR_NOCOLORKEY)
	{
		BlitOpt.ColorKeyFrom = HIGO_CKEY_NONE;
		ErrCode = HI_GO_Blit(DrvBitmap.SurfaceHandle,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	}
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapDraw HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(DrvBitmap.SurfaceHandle);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(DrvBitmap.SurfaceHandle);
	ErrCode |= HI_GO_FreeSurface(MemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDBitmapDraw HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
	return ErrCode;
}

DRV_ErrCode DRV_OSDDrawBitmap(OSD_Region_t *Region_p, S32 PositionX, S32 PositionY, OSD_Bitmap_t *Bitmap_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE MemSurface,Surface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
	
    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if(REGION_IS_NULL||NULL == Bitmap_p)
        return -1;
	YS_BlitClaim();
	if(Bitmap_p->SurfaceHandle == 0)
	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Bitmap_p->Height;
	    SurInfo.Width       = Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmap HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDDrawBitmap HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
	    	ErrCode = HI_GO_FreeSurface(MemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmap HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	else
	{
		Surface = Bitmap_p->SurfaceHandle;
	}	
	if(PositionX >= 0)
	{
		SrcRect.x= 0;
		SrcRect.y= 0;
		if(Bitmap_p->Width > Region_p->RectIn.nWidth)
		{
			SrcRect.w = Region_p->RectIn.nWidth;
		}
		else
		{
			SrcRect.w = Bitmap_p->Width;
		}

		SrcRect.h = Bitmap_p->Height;
		DstRect.x = PositionX;
		DstRect.y = PositionY;
		DstRect.w = SrcRect.w;
		DstRect.h = Bitmap_p->Height;
	}
	else
	{
		SrcRect.x = 0 - PositionX;
		if(SrcRect.x >= Bitmap_p->Width)
			SrcRect.x = Bitmap_p->Width -1;
		SrcRect.y = 0;
		if(Bitmap_p->Width > Region_p->RectIn.nWidth)
		{
			SrcRect.w = Region_p->RectIn.nWidth;
		}
		else
		{
			SrcRect.w = Bitmap_p->Width;
		}

		SrcRect.h = Bitmap_p->Height;
		DstRect.x = 0;
		DstRect.y = PositionY;
		DstRect.w = SrcRect.w;
		DstRect.h = Bitmap_p->Height;
	}
	
    if((Region_p->EnableCK == TRUE)&&(Region_p->ColorKey != 0))
    {
	    HI_COLOR ColorKeyNew;
	    HI_BOOL PaletteValid;
	    PaletteValid = Region_p->PaletteValid;
	    Region_p->PaletteValid = FALSE;
	    ColorKeyNew = drv_OsdGetRegionTrueColor(Region_p, Region_p->ColorKey,NULL);
	    Region_p->PaletteValid = PaletteValid;
		ErrCode = HI_GO_SetSurfaceColorKey(Surface, ColorKeyNew);       
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDDrawBitmap HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",ErrCode); 
	    }
	    BlitOpt.ColorKeyFrom   = HIGO_CKEY_SRC;
    }	
	//BlitOpt.EnableScale = TRUE; 打开的话CA滚动字会有问题
	ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect, &BlitOpt);
	if(ErrCode == HIGO_ERR_NOCOLORKEY)
	{
		BlitOpt.ColorKeyFrom = HIGO_CKEY_NONE;
		ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect, &BlitOpt);
	}
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmap HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		if(Bitmap_p->SurfaceHandle == 0)
		{
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
		}		
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	if(Bitmap_p->SurfaceHandle == 0)
	{
		ErrCode = HI_GO_FreeSurface(Surface);
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmap HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}
DRV_ErrCode DRV_OSDDrawBitmapRectZoom(OSD_Region_t *Region_p, DRV_RectWh *RegRect_p, DRV_RectWh *BmpRect_p,OSD_Bitmap_t *Bitmap_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE MemSurface,Surface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};

    /*此处是否应该判断Bitmap_p的类型与Region_p的类型是否一致*/
    if(REGION_IS_NULL||NULL == Bitmap_p)
        return -1;
	YS_BlitClaim();
	//if(Bitmap_p->SurfaceHandle == 0)
	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Bitmap_p->Height;
	    SurInfo.Width       = Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmapRectZoom HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDDrawBitmapRectZoom HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(MemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmapRectZoom HI_GO_Blit errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(Surface);
			ErrCode |= HI_GO_FreeSurface(MemSurface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	
	SrcRect.x       = BmpRect_p->x;
	SrcRect.y       = BmpRect_p->y;
	SrcRect.w  = BmpRect_p->nWidth;
	SrcRect.h  = BmpRect_p->nHeight;
   
	DstRect.x       = RegRect_p->x;
	DstRect.y       = RegRect_p->y;
	DstRect.w  = RegRect_p->nWidth;
	DstRect.h = RegRect_p->nHeight;
    if((Region_p->EnableCK == TRUE)&&(Region_p->ColorKey != 0))
    {
	    HI_COLOR ColorKeyNew;
	    HI_BOOL PaletteValid;
	    PaletteValid = Region_p->PaletteValid;
	    Region_p->PaletteValid = FALSE;
	    ColorKeyNew = drv_OsdGetRegionTrueColor(Region_p, Region_p->ColorKey,NULL);
	    Region_p->PaletteValid = PaletteValid;
		ErrCode = HI_GO_SetSurfaceColorKey(Surface, ColorKeyNew);       
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDDrawBitmapRectZoom HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",ErrCode); 
	    }
	    BlitOpt.ColorKeyFrom   = HIGO_CKEY_SRC;
    }	
	BlitOpt.EnableScale = TRUE;
	ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	if(ErrCode == HIGO_ERR_NOCOLORKEY)
	{
		BlitOpt.ColorKeyFrom = HIGO_CKEY_NONE;
		ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	}
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmapRectZoom HI_GO_Blit errCode 0x%08x!\n",ErrCode);
		ErrCode = HI_GO_FreeSurface(Surface);
		ErrCode |= HI_GO_FreeSurface(MemSurface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(Surface);
	ErrCode |= HI_GO_FreeSurface(MemSurface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmapRectZoom HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}

DRV_ErrCode DRV_OSDDrawBitmapZoom(OSD_Region_t *Region_p, DRV_RectWh *Rect_p, OSD_Bitmap_t *Bitmap_p)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_RECT SrcRect,DstRect;
	HI_HANDLE MemSurface,Surface;
	HIGO_SURINFO_S SurInfo;
   	HIGO_PF_E PixelFormat;
   	BOOL IsCreateSurface = FALSE;
	HIGO_BLTOPT_S BlitOpt = {0};
    if(REGION_IS_NULL||NULL == Bitmap_p)
        return -1;
	YS_BlitClaim();
	//if(Bitmap_p->SurfaceHandle == 0)
	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = Bitmap_p->Height;
	    SurInfo.Width       = Bitmap_p->Width;
	    SurInfo.Pitch[0]    = Bitmap_p->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
    	//SurInfo.pPhyAddr[0] = (HI_CHAR*)(Bitmap_p->Data_p);
	    switch(Bitmap_p->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmapZoom HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_CreateSurface(SurInfo.Width, SurInfo.Height, SurInfo.PixelFormat,&Surface);
	    if (HI_SUCCESS != ErrCode)
	    {
	    	printf("DRV_OSDDrawBitmapZoom HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			ErrCode = HI_GO_FreeSurface(MemSurface);
	    	YS_BlitRelease();
			return HI_FAILURE; 
	    }
		DstRect.x       = 0;
		DstRect.y       = 0;
		DstRect.w  = SurInfo.Width;
		DstRect.h  = SurInfo.Height;
		ErrCode = HI_GO_Blit(MemSurface,NULL,Surface,&DstRect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmapZoom HI_GO_Blit errCode 0x%08x! line[%d]\n",ErrCode, __LINE__);
			ErrCode = HI_GO_FreeSurface(MemSurface);
			ErrCode |= HI_GO_FreeSurface(Surface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = Bitmap_p->Width;
	SrcRect.h = Bitmap_p->Height;
   
	DstRect.x       = Rect_p->x;
	DstRect.y       = Rect_p->y;
	DstRect.w  = Rect_p->nWidth;
	DstRect.h  = Rect_p->nHeight;
	/*暂时不支持缩小*/
	//if(SrcRect.w > DstRect.w)
	//	SrcRect.w = DstRect.w;
	//if(SrcRect.h > DstRect.h)
	//	SrcRect.h = DstRect.h;
    if((Region_p->EnableCK == TRUE)&&(Region_p->ColorKey != 0))
    {
	    HI_COLOR ColorKeyNew;
	    HI_BOOL PaletteValid;
	    PaletteValid = Region_p->PaletteValid;
	    Region_p->PaletteValid = FALSE;
	    ColorKeyNew = drv_OsdGetRegionTrueColor(Region_p, Region_p->ColorKey,NULL);
	    Region_p->PaletteValid = PaletteValid;
		ErrCode = HI_GO_SetSurfaceColorKey(Surface, ColorKeyNew);       
	    if (HI_SUCCESS != ErrCode) 
	    {
	        printf("DRV_OSDDrawBitmapZoom HI_GO_SetSurfaceColorKey ErrCode 0x%08x\n",ErrCode); 
	    }
	    BlitOpt.ColorKeyFrom   = HIGO_CKEY_SRC;
    }	
	BlitOpt.EnableScale = TRUE;
	ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	if(ErrCode == HIGO_ERR_NOCOLORKEY)
	{
		BlitOpt.ColorKeyFrom = HIGO_CKEY_NONE;
		ErrCode = HI_GO_Blit(Surface,&SrcRect,Region_p->SurfaceHandle,&DstRect,&BlitOpt);
	}
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmapZoom HI_GO_Blit errCode 0x%08x! line[%d]\n",ErrCode, __LINE__);
		ErrCode = HI_GO_FreeSurface(MemSurface);
		ErrCode |= HI_GO_FreeSurface(Surface);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	ErrCode = HI_GO_FreeSurface(MemSurface);
	ErrCode |= HI_GO_FreeSurface(Surface);
	if (HI_SUCCESS != ErrCode)
	{
		printf("DRV_OSDDrawBitmapZoom HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
	return ErrCode;
}


DRV_ErrCode DRV_OSDCreateDDBitmap(OSD_Region_t* Region_p,const U32 width, const U32 height, OSD_Bitmap_t** bitmap)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    HI_HANDLE Surface;
	HIGO_PF_E PixelFormat;    
	HI_PIXELDATA pData; 
	if(Region_p == NULL || width <1 || height < 1)
        return -1;
	*bitmap = (OSD_Bitmap_t*)DRV_OSMalloc(SystemPartition,sizeof(**bitmap));       
    if(*bitmap == NULL)
        return -1;
    switch(Region_p->ColorType)
    {
        case 8:
		    PixelFormat = HIGO_PF_CLUT8;
            break;
        case OSD_COLOR_TYPE_ARGB4444:
		    PixelFormat = HIGO_PF_4444;
            break;
        case OSD_COLOR_TYPE_RGB565:
		    PixelFormat = HIGO_PF_565;
            break;
        case OSD_COLOR_TYPE_ARGB1555:
		    PixelFormat = HIGO_PF_1555;
            break;
        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
		    PixelFormat = HIGO_PF_8888;
            break;
        case OSD_COLOR_TYPE_ARGB8888:
		    PixelFormat = HIGO_PF_8888;
            break;
    }
    YS_BlitClaim();
	ErrCode = HI_GO_CreateSurface(width, height, PixelFormat, &Surface);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDCreateDDBitmap HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
    }
   	ErrCode = HI_GO_LockSurface(Surface, pData, HI_TRUE);//修正相关参数
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDCreateDDBitmap HI_GO_LockSurface errCode 0x%08x!\n",ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
    }
    (*bitmap)->Pitch = pData[0].Pitch;
    (*bitmap)->Data_p = pData[0].pData;
   	ErrCode = HI_GO_UnlockSurface(Surface);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDCreateDDBitmap HI_GO_UnlockSurface errCode 0x%08x!\n",ErrCode);
		return HI_FAILURE; 
    }	
	(*bitmap)->BitsPerPel    = Region_p->NbBitsPerPel;
    (*bitmap)->Width         = width;
    (*bitmap)->Height        = height;
    (*bitmap)->SurfaceHandle = Surface;
    (*bitmap)->ColorType     = Region_p->ColorType;
	return ErrCode;
}

DRV_ErrCode DRV_OSDFreeDDBitmap(OSD_Bitmap_t* bitmap)
{/*此处应加参数检查*/
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
   if(bitmap == NULL)
        return -1;
    YS_BlitClaim();
    ErrCode = HI_GO_FreeSurface(bitmap->SurfaceHandle);
	YS_BlitRelease();
	DRV_OSFree(SystemPartition,bitmap);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDCreateDDBitmap HI_GO_FreeSurface errCode 0x%08x!\n",ErrCode);
		return HI_FAILURE; 
    }
    return ErrCode;
}

DRV_ErrCode DRV_BitmapFillRectangle(OSD_Bitmap_t*pBitmap,DRV_RectWh* pDstRect,S32 color)
{
	DRV_ErrCode             ErrCode;
	HI_RECT Rect;
  	HI_COLOR TrueColor;
  	HI_COLOR HisiArgbColor;
	HI_HANDLE MemSurface;
	HIGO_SURINFO_S SurInfo;
	HI_MMZ_BUF_S MmzBuf = {0};
	HIGO_BLTOPT_S BlitOpt = {0};
	
	Rect.x = pDstRect->x;
	Rect.y = pDstRect->y;
	Rect.w = pDstRect->nWidth;
	Rect.h = pDstRect->nHeight;
	
	TrueColor = drv_OsdGetTrueColor(pBitmap->ColorType, color, NULL);
	HisiArgbColor = drv_OsdTrueColorToHisiArgb(pBitmap->ColorType,TrueColor);
	
    YS_BlitClaim();
	
	if(pBitmap->SurfaceHandle == 0)
	{
	    SurInfo.MemType     = HIGO_MEMTYPE_OS;
	    SurInfo.Height      = pBitmap->Height;
	    SurInfo.Width       = pBitmap->Width;
	    SurInfo.Pitch[0]    = pBitmap->Pitch;
	    SurInfo.pVirAddr[0] = (HI_CHAR*)(pBitmap->Data_p);
    	//SurInfo.pPhyAddr[0] =(HI_CHAR *)MmzBuf.phyaddr;
    	
	    switch(pBitmap->ColorType)
	    {
	        case 8:
			    SurInfo.PixelFormat = HIGO_PF_CLUT8;
	            break;
	        case 4444:
	        case OSD_COLOR_TYPE_ARGB4444:
			    SurInfo.PixelFormat = HIGO_PF_4444;
	            break;
	        case OSD_COLOR_TYPE_RGB565:
			    SurInfo.PixelFormat = HIGO_PF_565;
	            break;
	        case OSD_COLOR_TYPE_ARGB1555:
			    SurInfo.PixelFormat = HIGO_PF_1555;
	            break;
	        case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	        case OSD_COLOR_TYPE_ARGB8888:
			    SurInfo.PixelFormat = HIGO_PF_8888;
	            break;
	    }
		
	    ErrCode = HI_GO_CreateSurfaceFromMem(&SurInfo, &MemSurface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_BitmapFillRectangle HI_GO_CreateSurfaceFromMem errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
	}
	else
	{
		MemSurface = pBitmap->SurfaceHandle;
	}

	{	
		S32 Width = Rect.w;
		S32 FillWidth = 0;

		while((Width - FillWidth )> 0)
		{
			Rect.x = FillWidth;
			if(Width - FillWidth > 1920)
				Rect.w = 1920;
			else
				Rect.w = 1920;
			
			printf("\n%s,%s,x = %d,y = %d,w = %d,h = %d\n",__FILE__,__FUNCTION__,Rect.x,Rect.y,Rect.w,Rect.h);
			ErrCode = HI_GO_FillRect(MemSurface, &Rect, HisiArgbColor, HIGO_COMPOPT_NONE);

			if (HI_SUCCESS != ErrCode)
				break;
			
			FillWidth += 1920;
		}

	}
	
	//ErrCode = HI_GO_FillRect(MemSurface, &Rect,HisiArgbColor, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_BitmapFillRectangle HI_GO_FillRect errCode 0x%08x!\n",ErrCode);

		YS_BlitRelease();
		return HI_FAILURE; 
    }
	
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
   return(ErrCode);
} /* STOSD_FillRectangle */

DRV_ErrCode drv_OsdBitmapSetPixelColor(OSD_Bitmap_t*pDstBitmap,S32 PositionX, S32 PositionY,S32 Color)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
  	HI_COLOR TrueColor;
  	HI_COLOR HisiArgbColor;
    if(PositionX > pDstBitmap->Width || PositionY> pDstBitmap->Height ||PositionX<0 || PositionY <0)
        return -1;

    TrueColor = drv_OsdGetTrueColor(pDstBitmap->ColorType,Color,NULL);
	HisiArgbColor = drv_OsdTrueColorToHisiArgb(pDstBitmap->ColorType,TrueColor);
    YS_BlitClaim();
    ErrCode = HI_GO_DrawLine(pDstBitmap->SurfaceHandle,PositionX,PositionY, PositionX, PositionY, HisiArgbColor);
	YS_BlitRelease();
    if (HI_SUCCESS != ErrCode)
    {
    	printf("drv_OsdBitmapSetPixelColor HI_GO_DrawLine errCode 0x%08x!\n",ErrCode);
		return HI_FAILURE; 
    }
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
    return ErrCode;
}
static void DrawCharacter16(OSD_Bitmap_t *Bitmap_p,
                                      char *Character,
                                      S32 Font,
                                      S32 PositionX,
                                      S32 PositionY,
                                      S32 wbs,
                                      S32 hbs,
                                      U16 Color)/*color为0时不能显示*/
{
	void  *Data_p;
	U16*    Dest_p;
    S32 Offset;
    S32 i, m, n,j;
	HI_PIXELDATA pData; 
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    YS_BlitClaim();
   	ErrCode = HI_GO_LockSurface(Bitmap_p->SurfaceHandle, pData, HI_TRUE);//get mem from surface
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DrawCharacter16 HI_GO_LockSurface ErrCode 0x%08x\n",ErrCode); 
    	YS_BlitRelease();
        return;       
    }
	Bitmap_p->Data_p = pData[0].pData;
	Bitmap_p->Pitch  = pData[0].Pitch;
	Bitmap_p->BitsPerPel = pData[0].Bpp*8;
	Data_p = Bitmap_p->Data_p;
    /*不放大字符时单独处理*/
    if ((1 == wbs) && (1 == hbs))
    {
        for (i = 0; i < Font; ++i)
        {
			Offset = (PositionY+i)*Bitmap_p->Pitch+2*PositionX;
			Dest_p = (U16*)((U32)Data_p + Offset);//行地址

            for (m = 0; m < gCHI_FONT_PITCH; m++)
            {   
				if(Character[i*gCHI_FONT_PITCH+m])
                {
                    for(n=0;n<8;n++)
                    {
						if(getbit(Character[i*gCHI_FONT_PITCH+m],7-n))
						{
                            *(Dest_p+n) = Color;
                    }
                }
				}
                Dest_p += 8;
            }
        }
    }
    else
    {
        /*字符放大*/
        S32 xs;
        for (i = 0; i < Font; ++i)
        {
            for(j =0;j<hbs;++j)
            {
				Offset = (PositionY+i*hbs+j)*Bitmap_p->Pitch+2*PositionX;
				Dest_p = (U16*)((U32)Data_p + Offset);//行地址

                for (m = 0; m < gCHI_FONT_PITCH; m++)
                {   
					if(Character[i*gCHI_FONT_PITCH+m])
                    {
                        for(n=0;n<8;n++)
                        {
							if(getbit(Character[i*gCHI_FONT_PITCH+m],7-n))
                            {
                                for(xs=0;xs<wbs;xs++)
								{
                                *(Dest_p+n*wbs+xs) = Color;
                            }
                        }
                    }
                    }
                    Dest_p += 8*wbs;
                }
            }
        }
    }
   	ErrCode = HI_GO_UnlockSurface(Bitmap_p->SurfaceHandle);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DrawCharacter16 HI_GO_UnlockSurface ErrCode 0x%08x\n",ErrCode); 
    	YS_BlitRelease();
        return;       
    }
    YS_BlitRelease();
}

static void DrawCharacter32(OSD_Bitmap_t *Bitmap_p,
                                      char *Character,
                                      S32 Font,
                                      S32 PositionX,
                                      S32 PositionY,
                                      S32 wbs,
                                      S32 hbs,
                                      U32 Color)/*color为0时不能显示*/
{
    void  *Data_p;
	U32*    Dest_p;
    S32 Offset;
    S32 i, m, n,j;
	HI_PIXELDATA pData; 
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    YS_BlitClaim();
   	ErrCode = HI_GO_LockSurface(Bitmap_p->SurfaceHandle, pData, HI_TRUE);//get mem from surface
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DrawCharacter32 HI_GO_LockSurface ErrCode 0x%08x\n",ErrCode); 
    	YS_BlitRelease();
        return;       
    }
	Bitmap_p->Data_p = pData[0].pData;
	Bitmap_p->Pitch  = pData[0].Pitch;
	Bitmap_p->BitsPerPel = pData[0].Bpp*8;
	Data_p = Bitmap_p->Data_p;
    /*不放大字符时单独处理*/
    if ((1 == wbs) && (1 == hbs))
    {
        for (i = 0; i < Font; ++i)
        {
			Offset = (PositionY+i)*Bitmap_p->Pitch+4*PositionX;
			Dest_p = (U32*)((U32)Data_p + Offset);//行地址

            for (m = 0; m < gCHI_FONT_PITCH; m++)
            {   
				if(Character[i*gCHI_FONT_PITCH+m])
                {
                    for(n=0;n<8;n++)
                    {
						if(getbit(Character[i*gCHI_FONT_PITCH+m],7-n))
							{
                        *(Dest_p+n) = Color;
							}
                    }
                }
                Dest_p += 8;
            }
        }
    }
    else
    {
        /*字符放大*/
        S32 xs;
        for (i = 0; i < Font; ++i)
        {
            for(j =0;j<hbs;++j)
            {
				Offset = (PositionY+i*hbs+j)*Bitmap_p->Pitch+4*PositionX;
				Dest_p = (U32*)((U32)Data_p + Offset);//行地址

                for (m = 0; m < gCHI_FONT_PITCH; m++)
                {   
					if(Character[i*gCHI_FONT_PITCH+m])
                    {
                        for(n=0;n<8;n++)
                        {
							if(getbit(Character[i*gCHI_FONT_PITCH+m],7-n))
                            {
                                for(xs=0;xs<wbs;xs++)
								{
                                  *(Dest_p+n*wbs+xs) = Color;
                            }
                        }
                    }
					}
                    Dest_p += 8*wbs;
                }
            }
        }
    }
   	ErrCode = HI_GO_UnlockSurface(Bitmap_p->SurfaceHandle);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DrawCharacter32 HI_GO_UnlockSurface ErrCode 0x%08x\n",ErrCode); 
    	YS_BlitRelease();
        return;       
    }
    YS_BlitRelease();
}

#ifndef	DRV_SLZK_SUPPORT
/*为了增加对tab键的支持,把原来的第三个参数拆分成两个参数*/
S32 DRV_OSDCharacterDraw(OSD_Region_t *Region_p, 
                                                char *Text,
                                                S32 x,              /*文字所在行的横向起始位置*/
                                                S32 Offset,     /*当前字符的横向偏移*/
                                                S32 y,
                                                S32 wbs,
                                                S32 hbs, 
                                                S32 Color,
                                                S32 *DrawWidth,
                                                S32 *DrawHigh)
{/*只支持单个字符*/
    S32 i3;       
    //U8 *pchar = (char*)mHzBuffer_Np;
    U8 AscOdd, AscEven;
    S32 qm, wm, rec;
    U8 *asc = NULL;
    S32 Width = 0;
    S32 High = 0;
    S32 Speed = 3;
    S32 TrueColor = Color;
	OSD_Color_t GxobjColor;    
    if (REGION_IS_NULL || (NULL == Text))
    {
        return Width;
    }

    /*以下代码取字模*/
    AscOdd = *Text;
    if(AscOdd & 0x80)
    { 
        AscEven = *(Text + 1);
        if(AscEven & 0x80)
        {
            rec=0;
	    if(AscEven >0x00a0 && AscOdd > 0x00a0)
            {
                qm=(AscOdd - 0x00a1) & 0x7f;
                wm=(AscEven - 0x00a1) & 0x7f;
                rec = (qm*94 + wm) ;
                if(rec <8178)//GB2312只有8178个字符
                    rec=rec* gCHI_FONT_SIZE;
                else //超出的用空格代替
                    rec = 0;
            }
            asc = (U8*)gFonthzData + rec;
            Width = gCHI_FONT_WIDTH* wbs;
            High = gCHI_FONT_HEIGHT* hbs;
        }
        else
        {
            asc = NULL;
            if (AscEven == /*中文空格*/0xa0)
            {
                Width = gCHI_FONT_WIDTH* wbs;
                High = gCHI_FONT_HEIGHT* hbs;
            }
        }
    }
    else
    {
        if(AscOdd > 0x20)/*可见字符 0X20 asc空格*/
        {
            rec = AscOdd;
            rec = rec*gCHI_FONT_SIZE;
            asc = (U8 *)(gAsciiData+rec);
            Width =  gFontWidth[AscOdd] * wbs;
            High = gCHI_FONT_HEIGHT * hbs;
        }
        else if(0X9 == AscOdd)/*tab键处理*/
        {
            asc = NULL;            
            Width = gCHI_FONT_WIDTH-Offset%gCHI_FONT_WIDTH;
            High = gCHI_FONT_HEIGHT * hbs;  
        }
        else
        {
            AscOdd = ' ';
            asc = NULL;
        
            if (AscOdd == /*asc空格*/0x20)
            {
                Width = gFontWidth[AscOdd] * wbs;
                High = gCHI_FONT_HEIGHT* hbs;
            }
        }
    }

    /*以下代码调整字模*/
    if(NULL != asc)
    {
        //memset(pchar,0,gCHI_FONT_SIZE);
        //i3 = 0;
        //while( i3 < gCHI_FONT_SIZE)
        //{
        //    *pchar = asc[i3];
        //    pchar ++;
        //    i3 ++;
        //}

        if(Region_p->NbBitsPerPel == 16)
        {
            OSD_Bitmap_t  RegBitmap_p;
            DRV_GetRegionBitmap(Region_p, &RegBitmap_p);
		    TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
		    TrueColor = drv_OsdArgbToTrueColor(Region_p->ColorType,TrueColor);
            DrawCharacter16(&RegBitmap_p, asc, gCHI_FONT_HEIGHT, x+Offset, y, wbs, hbs, TrueColor);
        }
        else  if(Region_p->NbBitsPerPel == 32)
        {
            OSD_Bitmap_t  RegBitmap_p;
            DRV_GetRegionBitmap(Region_p, &RegBitmap_p);
            TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
		    TrueColor = drv_OsdArgbToTrueColor(Region_p->ColorType,TrueColor);
            DrawCharacter32(&RegBitmap_p, asc, gCHI_FONT_HEIGHT, x+Offset, y, wbs, hbs, TrueColor);
        }
          
    }

    if (NULL != DrawWidth)
    {
        *DrawWidth = Width;
    }

    if (NULL != DrawHigh)
    {
        *DrawHigh = High;
    }

    return Width;

}
DRV_ErrCode DRV_OSDTextLineDraw(OSD_Region_t *Region_p,char *hz,S32 Len, S32 x,S32 y,S32 wbs,S32 hbs, S32  FontSpace, S32 Color)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	S32 i=0;
	S32 iDistance = 0;
	char asc[3] = {0, 0, 0};

	if (REGION_IS_NULL|| (NULL == hz)||(0 == Len))
	{
		return -1;
	}

	for (i = 0; (i < Len) && ('\0' != hz[i]); i++)
	{
		if(hz[i] & 0x80)
		{
			memcpy(asc, &(hz[i]), 2);
			i++;
		}
		else
		{
			asc[0] = hz[i];
			asc[1] = '\0';
		}

		iDistance +=  FontSpace + DRV_OSDCharacterDraw(Region_p, 
		                                            asc, 
		                                            x ,
		                                            iDistance, 
		                                            y, 
		                                            wbs, 
		                                            hbs, 
		                                            Color,
		                                            NULL,
		                                            NULL);    
	}
}


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
                                                S32 *DrawHigh)
{
	/*只支持单个字符*/

	S32 i3;       
	//char *pchar = (char*)mHzBuffer_Np;     
	U8 AscOdd, AscEven;
	U32 qm, wm, rec;
	U8 *asc = NULL;
	S32 Width = 0;
	S32 High = 0;
	S32 Speed = 3;
	S32 TrueColor = Color;
	
	if ((pBitmap == NULL)|| (NULL == Text))
	{
		return Width;
	}

	/*以下代码取字模*/
	AscOdd = *Text;
	if(AscOdd & 0x80)
	{ 
		AscEven = *(Text + 1);
		if(AscEven & 0x80)
		{
			rec=0;
			if(AscEven >0x00a0 && AscOdd > 0x00a0)
			{
				qm=(AscOdd - 0x00a1) & 0x7f;
				wm=(AscEven - 0x00a1) & 0x7f;
				rec = (qm*94 + wm) ;
				if(rec <8178)//GB2312只有8178个字符
					rec=rec* gCHI_FONT_SIZE;
				else //超出的用空格代替
					rec = 0;
			}
			asc = (U8*)gFonthzData + rec;
			Width = gCHI_FONT_WIDTH* wbs;
			High = gCHI_FONT_HEIGHT* hbs;
		}
		else
		{
			asc = NULL;
			if (AscEven == /*中文空格*/0xa0)
			{
				Width = gCHI_FONT_WIDTH* wbs;
				High = gCHI_FONT_HEIGHT* hbs;
			}
		}
	}
	else
	{
		if(AscOdd > 0x20)/*可见字符 0X20 asc空格*/
		{
			rec = AscOdd;
			rec = rec*gCHI_FONT_SIZE;
			asc = (U8 *)(gAsciiData+rec);
			Width =  gFontWidth[AscOdd] * wbs;
			High = gCHI_FONT_HEIGHT * hbs;
		}
		else if(0X9 == AscOdd)/*tab键处理*/
		{
			asc = NULL;            
			Width = 40* wbs;
			High = gCHI_FONT_HEIGHT * hbs;  
		}
		else
		{
			AscOdd = ' ';
			asc = NULL;

			if (AscOdd == /*asc空格*/0x20)
			{
				Width = gFontWidth[AscOdd] * wbs;
				High = gCHI_FONT_HEIGHT* hbs;
			}
		}
	}

	/*以下代码调整字模*/
	if(NULL != asc)
	{
		//memset(pchar,0,gCHI_FONT_SIZE);
		//i3 = 0;
		//while( i3 < gCHI_FONT_SIZE)
		//{
		//	*pchar = asc[i3];
		//	pchar ++;
		//	i3 ++;
		//}
		TrueColor = drv_OsdGetTrueColor(pBitmap->ColorType,Color,NULL);
		if(16 == pBitmap->BitsPerPel)
		DrawCharacter16(pBitmap, asc, gCHI_FONT_HEIGHT, x+fontSpase, y, wbs, hbs, (U16)TrueColor);
		else if(32 == pBitmap->BitsPerPel)
		DrawCharacter32(pBitmap, asc, gCHI_FONT_HEIGHT, x+fontSpase, y, wbs, hbs, (U32)TrueColor);
	}

	if (NULL != DrawWidth)
	{
		*DrawWidth = Width;
	}

	if (NULL != DrawHigh)
	{
		*DrawHigh = High;
	}

	return Width;

}



U32 DRV_BitmapTextLineDraw(OSD_Bitmap_t *pBitmap,
                                         const char   *Text,
                                         const U32 Length,
                                         const S32 PositionX,
                                         const S32 PositionY,
                                         const U32 Wbs,
                                         const U32 Hbs,
                                         const U32 FontSpace,
                                         const U32 Color)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
  S32 i=0;
  S32 iDistance = 0;
  char asc[3] = {0, 0, 0};
  if ((NULL == pBitmap) || (NULL == Text))
  {
    return -1;
  }

  for (i = 0; (i < Length) && ('\0' != Text[i]); i++)
  {
    if(Text[i] & 0x80)
    {
      memcpy(asc, &(Text[i]), 2);
      i++;
    }
    else
    {
      asc[0] = Text[i];
      asc[1] = '\0';
    }
    
    iDistance +=  FontSpace + DRV_BitmapDrawChar(pBitmap, 
                                                        asc, 
                                                        PositionX+iDistance,                                                        
                                                        PositionY, 
                                                        Wbs, 
                                                        Hbs, 
                                                        FontSpace,
                                                        Color,
                                                        NULL,
                                                        NULL);    
  }
}
U32 DRV_OSDGetTextWidth(const char   *Text,const U32 space,const U32 Wbs, U32* pWidth)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    S32 i = 0;
    S32 XOffset = 0;
    if(Text == NULL || pWidth == NULL)
        return -1;

    while(i<strlen(Text))
    {
        if((Text[i] & 0x80) != 0)    /* 中文字体. */
        {
            XOffset += Wbs * gCHI_FONT_WIDTH;
            i += 2;
        }
        else
        {
            XOffset   += Wbs * gFontWidth[Text[i]];
            i += 1;
        }
        XOffset += space;
    }
    XOffset -= space;
    *pWidth = XOffset;
    return 0;
}

#else	
S32 DRV_OSDCharacterDraw(OSD_Region_t *Region_p, 
                                                char *Text,
                                                S32 x,              /*文字所在行的横向起始位置*/
                                                S32 Offset,     /*当前字符的横向偏移*/
                                                S32 y,
                                                S32 wbs,
                                                S32 hbs, 
                                                S32 Color,
                                                S32 *DrawWidth,
                                                S32 *DrawHigh)
{/*只支持单个字符*/
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	S32 Width = 0;
	S32 High = 0;
	//S32 Len = 0;
	S32 TrueColor = Color;
	HI_RECT src_rect = {0};
	HI_RECT dst_rect = {0};
	U8 AscOdd, AscEven;
	HIGO_BLTOPT_S BlitOpt = {0};
	HI_HANDLE Surface;
	HIGO_PF_E PixelFormat;
	//unsigned int start_time,end_time;


	//start_time=DRV_OS_TimeNow();
	//printf("\n[%s] start-- [%d ms]\n",__FUNCTION__,start_time);
	if (REGION_IS_NULL || (NULL == Text))
	{
		return Width;
	}
	AscOdd = *Text;
	if(AscOdd & 0x80)
	{
		AscEven = *(Text + 1);
		if(AscEven >0x00a0 && AscOdd > 0x00a0)	
			Width = (gCHI_FONT_WIDTH );
		else 
		{
			if (AscEven == /*中文空格*/0xa0)
			{
				Width = gCHI_FONT_WIDTH;
				return Width;
			}
		}
	}
	else
	{
		if(AscOdd > 0x20)/*可见字符 0X20 asc空格*/
		{
			Width = (gCHI_FONT_WIDTH/2);
		}
		else if(0X9 == AscOdd)/*tab键处理*/
		{        
		    Width = ((gCHI_FONT_WIDTH/2)-Offset)%(gCHI_FONT_WIDTH/2);
			return Width;
		}
		else
		{
		    if (AscOdd == /*asc空格*/0x20)
		    {
		        Width = (gCHI_FONT_WIDTH/2);
			return Width;
		    }
		}
	}
	High = (gCHI_FONT_HEIGHT);
	src_rect.x=(x+Offset);
	src_rect.y=y;
	src_rect.w=Width;
	src_rect.h=High;
	TrueColor = drv_OsdGetRegionTrueColor(Region_p,Color,NULL);
	TrueColor = drv_OsdArgbToTrueColor(Region_p->ColorType,TrueColor);

	YS_BlitClaim();
	ErrCode =HI_GO_SetTextColor(gSi_hFont, TrueColor);
	if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]HI_GO_SetTextColor failed ErrCode:%x\n",__FUNCTION__,ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	 ErrCode =HI_GO_SetCharExtra(gSi_hFont, wbs);
	 if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]HI_GO_SetCharExtra failed ErrCode:%x\n",__FUNCTION__,ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	 ErrCode =HI_GO_SetLineExtra(gSi_hFont, hbs);
	 if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]HI_GO_SetLineExtra failed ErrCode:%x\n",__FUNCTION__,ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}

	ErrCode = HI_GO_TextOut(gSi_hFont, Region_p->SurfaceHandle, Text, &src_rect);
	if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]failed to text out char!\n",__FUNCTION__);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	if((wbs>1)||((hbs>1)))//字体放大调整
	{
		Width=Width*wbs;
		High=High*hbs;
		dst_rect.x=(x+Offset);
		dst_rect.y=y;
		dst_rect.w=Width;
		dst_rect.h=High;

		switch(Region_p->ColorType)
		{
		    case 8:
			    PixelFormat = HIGO_PF_CLUT8;
		        break;
		    case OSD_COLOR_TYPE_ARGB4444:
			    PixelFormat = HIGO_PF_4444;
		        break;
		    case OSD_COLOR_TYPE_RGB565:
			    PixelFormat = HIGO_PF_565;
		        break;
		    case OSD_COLOR_TYPE_ARGB1555:
			    PixelFormat = HIGO_PF_1555;
		        break;
		    case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
			    PixelFormat = HIGO_PF_8888;
		        break;
		    case OSD_COLOR_TYPE_ARGB8888:
			    PixelFormat = HIGO_PF_8888;
		        break;
		}
		ErrCode = HI_GO_CreateSurface(Width/wbs, High/hbs, PixelFormat,&Surface);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDDrawBitmapZoom HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_FillRect(Surface,NULL,0xff000000,HIGO_COMPOPT_NONE);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(Surface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		memset(&BlitOpt,0,sizeof(HIGO_BLTOPT_S));
		BlitOpt.EnablePixelAlpha = HI_TRUE;
		BlitOpt.EnableGlobalAlpha = HI_TRUE;
		/** should set the pixel alpha mix mode when enable global alpha*/	
		BlitOpt.PixelAlphaComp = HIGO_COMPOPT_NONE;
		BlitOpt.EnableScale    = TRUE;
		ErrCode = HI_GO_Blit(Region_p->SurfaceHandle,&src_rect,Surface,NULL,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("[%s] HI_GO_Blit HI_GO_Blit errCode 0x%08x! line[%d]\n",__FUNCTION__,ErrCode, __LINE__);
			HI_GO_FreeSurface(Surface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_FillRect(Region_p->SurfaceHandle,&src_rect,0xff000000,HIGO_COMPOPT_NONE);
		if (HI_SUCCESS != ErrCode)
		{
			printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
			HI_GO_FreeSurface(Surface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		ErrCode = HI_GO_Blit(Surface,NULL,Region_p->SurfaceHandle,&dst_rect,&BlitOpt);
		if (HI_SUCCESS != ErrCode)
		{
			printf("[%s] HI_GO_Blit HI_GO_Blit errCode 0x%08x! line[%d]\n",__FUNCTION__,ErrCode, __LINE__);
			HI_GO_FreeSurface(Surface);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		HI_GO_FreeSurface(Surface);
	}
	YS_BlitRelease();
	if (NULL != DrawWidth)
	{
	    *DrawWidth = Width;
	}

	if (NULL != DrawHigh)
	{
	    *DrawHigh = High;
	}

	//end_time=DRV_OS_TimeNow();
	//printf("[%s] used time[%d ms] ,wbs[%d],hbs[%d],Width[%d],High[%d]!\n",__FUNCTION__,end_time-start_time,wbs,hbs,Width,High);
	//printf("[%s] end ---- [%d ms]\n",__FUNCTION__,DRV_OS_TimeNow());
	return Width;
}

DRV_ErrCode DRV_OSDTextLineDraw(OSD_Region_t *Region_p,char *hz,S32 Len, S32 x,S32 y,S32 wbs,S32 hbs, S32  FontSpace, S32 Color)
{
	S32 i=0;
	S32 iDistance = 0;
	char asc[3] = {0, 0, 0};
	//unsigned int start_time,end_time,zf_len;
	 //float average_time;

	if (REGION_IS_NULL|| (NULL == hz)||(0 == Len))
	{
		return -1;
	}
	//start_time=DRV_OS_TimeNow();
	//zf_len=0;
	for (i = 0; (i < Len) && ('\0' != hz[i]); i++)
	{
		if(hz[i] & 0x80)
		{
			memcpy(asc, &(hz[i]), 2);
			i++;
			//zf_len+=2;
		}
		else
		{
			asc[0] = hz[i];
			asc[1] = '\0';
			//zf_len++;
		}

		iDistance +=  FontSpace + DRV_OSDCharacterDraw(Region_p, 
		                                            asc, 
		                                            x ,
		                                            iDistance, 
		                                            y, 
		                                            wbs, 
		                                            hbs, 
		                                            Color,
		                                            NULL,
		                                            NULL);    
	}
	//end_time=DRV_OS_TimeNow();
	//average_time=((float)(end_time-start_time))/((float)zf_len);
	//printf("[%s] used title_time[%d ms],average_time[%.2f] ,len[%d],wbs[%d],hbs[%d],Width[%d],High[%d]!\n",__FUNCTION__,end_time-start_time,average_time,zf_len,wbs,hbs,gCHI_FONT_WIDTH,gCHI_FONT_HEIGHT);
}

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
                                                S32 *DrawHigh)
{
	/*只支持单个字符*/

	DRV_ErrCode  ErrCode = DRV_NO_ERROR;  
	S32 Width = 0;
	S32 High = 0;
	S32 Speed = 3;
	S32 TrueColor = Color;
	S32 Len = 0;
	U8 AscOdd;
	HI_RECT src_rect = {0};
	HI_RECT dst_rect = {0};
	HI_HANDLE Surface;
	HIGO_PF_E PixelFormat;
	HIGO_BLTOPT_S BlitOpt = {0};
	
	if ((pBitmap == NULL)|| (NULL == Text))
	{
		return Width;
	}

	AscOdd=*Text;
	/*以下代码调整字模*/
	if(NULL != Text)
	{
		if(AscOdd & 0x80)
		{
			Width = (gCHI_FONT_WIDTH );
		}
		else
		{
			Width = (gCHI_FONT_WIDTH/2);
		}
		High = gCHI_FONT_HEIGHT* hbs;
		if(pBitmap->Width<(x+Width))
			Width=pBitmap->Width-x;
		if(pBitmap->Height<(y+High))
			High=pBitmap->Height-y;
		if((Width<=0)||(High<=0))
		{
			printf("[%s]para failed Width :%x,High :%x\n",__FUNCTION__,Width,High);
			return HI_FAILURE; 
		}
		src_rect.x=x;
		src_rect.y=y;
		src_rect.w=Width;
		src_rect.h=High;
		TrueColor = drv_OsdGetTrueColor(pBitmap->ColorType,Color,NULL);
		
		YS_BlitClaim();
		ErrCode =HI_GO_SetTextColor(gSi_hFont, TrueColor);
		if (HI_SUCCESS != ErrCode)
		{
			printf("[%s]HI_GO_SetTextColor failed ErrCode:%x\n",__FUNCTION__,ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		 ErrCode =HI_GO_SetCharExtra(gSi_hFont, wbs);
		 if (HI_SUCCESS != ErrCode)
		{
			printf("[%s]HI_GO_SetCharExtra failed ErrCode:%x\n",__FUNCTION__,ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		 ErrCode =HI_GO_SetLineExtra(gSi_hFont, hbs);
		 if (HI_SUCCESS != ErrCode)
		{
			printf("[%s]HI_GO_SetLineExtra failed ErrCode:%x\n",__FUNCTION__,ErrCode);
			YS_BlitRelease();
			return HI_FAILURE; 
		}
		 if(pBitmap->SurfaceHandle!=NULL)
	 	{
			ErrCode = HI_GO_TextOut(gSi_hFont, pBitmap->SurfaceHandle, Text, &src_rect);
			if (HI_SUCCESS != ErrCode)
			{
				printf("[%s]failed to text out char!\n",__FUNCTION__);
				YS_BlitRelease();
				return HI_FAILURE; 
			}
			if((wbs>1)||((hbs>1)))
			{

				Width=Width*wbs;
				High=High*hbs;
				dst_rect.x=x;
				dst_rect.y=y;
				dst_rect.w=Width;
				dst_rect.h=High;
				if(pBitmap->Width<(x+dst_rect.w))
					dst_rect.w=pBitmap->Width-x;
				if(pBitmap->Height<(y+dst_rect.h))
					dst_rect.h=pBitmap->Height-y;
				if((dst_rect.w<=0)||(dst_rect.h<=0))
				{
					printf("[%s]para failed dst_rect.w :%x,dst_rect.h :%x\n",__FUNCTION__,dst_rect.w,dst_rect.h);
					YS_BlitRelease();
					return HI_FAILURE; 
				}

				switch(pBitmap->ColorType)
				{
				    case 8:
					    PixelFormat = HIGO_PF_CLUT8;
				        break;
				    case OSD_COLOR_TYPE_ARGB4444:
					    PixelFormat = HIGO_PF_4444;
				        break;
				    case OSD_COLOR_TYPE_RGB565:
					    PixelFormat = HIGO_PF_565;
				        break;
				    case OSD_COLOR_TYPE_ARGB1555:
					    PixelFormat = HIGO_PF_1555;
				        break;
				    case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
					    PixelFormat = HIGO_PF_8888;
				        break;
				    case OSD_COLOR_TYPE_ARGB8888:
					    PixelFormat = HIGO_PF_8888;
				        break;
				}
				ErrCode = HI_GO_CreateSurface(Width/wbs, High/hbs, PixelFormat,&Surface);
				if (HI_SUCCESS != ErrCode)
				{
					printf("DRV_OSDDrawBitmapZoom HI_GO_CreateSurface errCode 0x%08x!\n",ErrCode);
					YS_BlitRelease();
					return HI_FAILURE; 
				}
				ErrCode = HI_GO_FillRect(Surface,NULL,0xff000000,HIGO_COMPOPT_NONE);
				if (HI_SUCCESS != ErrCode)
				{
					printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
					HI_GO_FreeSurface(Surface);
					YS_BlitRelease();
					return HI_FAILURE; 
				}
				memset(&BlitOpt,0,sizeof(HIGO_BLTOPT_S));
				BlitOpt.EnablePixelAlpha = HI_TRUE;
				BlitOpt.EnableGlobalAlpha = HI_TRUE;
				/** should set the pixel alpha mix mode when enable global alpha*/	
				BlitOpt.PixelAlphaComp = HIGO_COMPOPT_NONE;
				BlitOpt.EnableScale    = TRUE;
				ErrCode = HI_GO_Blit(pBitmap->SurfaceHandle,&src_rect,Surface,NULL,&BlitOpt);
				if (HI_SUCCESS != ErrCode)
				{
					printf("[%s] HI_GO_Blit HI_GO_Blit errCode 0x%08x! line[%d]\n",__FUNCTION__,ErrCode, __LINE__);
					HI_GO_FreeSurface(Surface);
					YS_BlitRelease();
					return HI_FAILURE; 
				}
				ErrCode = HI_GO_FillRect(pBitmap->SurfaceHandle,&src_rect,0xff000000,HIGO_COMPOPT_NONE);
				if (HI_SUCCESS != ErrCode)
				{
					printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
					HI_GO_FreeSurface(Surface);
					YS_BlitRelease();
					return HI_FAILURE; 
				}
				ErrCode = HI_GO_Blit(Surface,NULL,pBitmap->SurfaceHandle,&dst_rect,&BlitOpt);
				if (HI_SUCCESS != ErrCode)
				{
					printf("[%s] HI_GO_Blit HI_GO_Blit errCode 0x%08x! line[%d]\n",__FUNCTION__,ErrCode, __LINE__);
					HI_GO_FreeSurface(Surface);
					YS_BlitRelease();
					return HI_FAILURE; 
				}
				HI_GO_FreeSurface(Surface);
			}
	 	}
		 else
	 	{
			printf("[%s]ERRO pBitmap->SurfaceHandle=NULL!\n",__FUNCTION__);
	 	}
		 YS_BlitRelease();
	}

	if (NULL != DrawWidth)
	{
		*DrawWidth = Width;
	}

	if (NULL != DrawHigh)
	{
		*DrawHigh = High;
	}

	return Width;

}

U32 DRV_BitmapTextLineDraw(OSD_Bitmap_t *pBitmap,
                                         const char   *Text,
                                         const U32 Length,
                                         const S32 PositionX,
                                         const S32 PositionY,
                                         const U32 Wbs,
                                         const U32 Hbs,
                                         const U32 FontSpace,
                                         const U32 Color)
{
	S32 i=0;
	S32 iDistance = 0;
	char asc[3] = {0, 0, 0};
	
	if ((NULL == pBitmap) || (NULL == Text))
	{
	return -1;
	}
	for (i = 0; (i < Length) && ('\0' != Text[i]); i++)
	{
		if(Text[i] & 0x80)
		{
			memcpy(asc, &(Text[i]), 2);
			i++;
		}
		else
		{
			asc[0] = Text[i];
			asc[1] = '\0';
		}

		iDistance +=  FontSpace + DRV_BitmapDrawChar(pBitmap, 
	                                                    Text, 
	                                                    PositionX+iDistance,                                                        
	                                                    PositionY, 
	                                                    Wbs, 
	                                                    Hbs, 
	                                                    FontSpace,
	                                                    Color,
	                                                    NULL,
	                                                    NULL);    
	}
}

U32 DRV_OSDGetTextWidth(const char   *Text,const U32 space,const U32 Wbs, U32* pWidth)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    S32 i = 0;
    S32 XOffset = 0;
    if(Text == NULL || pWidth == NULL)
        return -1;

    while(i<strlen(Text))
    {
        if((Text[i] & 0x80) != 0)    
        {
        	if(Text[0]  >0x00a0 && Text[1] > 0x00a0)/* 中文字体. */	
			XOffset=gCHI_FONT_WIDTH*Wbs;
		else
		{
			if (Text[1] == /*中文空格*/0xa0)
			{
				XOffset = gCHI_FONT_WIDTH;
			}
		}
		i += 2;
        }
        else
        {
		XOffset=gCHI_FONT_WIDTH*Wbs/2;
		 i += 1;
        }
        XOffset += space;
    }
    XOffset -= space;
    *pWidth = XOffset;
    return 0;
}


#endif
DRV_ErrCode DRV_STILLDrawIFrame(U8 * pData, S32 uiDataLength)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
	HI_UNF_CAPTURE_PICTURE_S pCapPic;
    HIGO_SURINFO_S    stSurInfo;
    HIGO_BLTOPT2_S    stBltOpt;
    HI_HANDLE hOsdSurface,hMemSurface;
    U32 OsdLayerId = OSD_REGION_STILL;
    return 0;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
   ErrCode = drv_InjectIframe(pData,uiDataLength,&pCapPic);
#if 0
    printf("=======IFrame Picture Info=============\n"
            "u32TopYStartAddr:%x\n"
            "u32TopCStartAddr:%x\n"
            "u32BottomYStartAddr:%x\n"
            "u32BottomCStartAddr:%x\n"
            "u32Width:%d\n"
            "u32Height:%d\n"
            "u32Stride:%d\n"
            "enVideoFormat:%d\n"
            "enFieldMode:%d\n"
            "enSampleType:%d\n",
            pCapPic->u32TopYStartAddr,
            pCapPic->u32TopCStartAddr,
            pCapPic->u32BottomYStartAddr,
            pCapPic->u32BottomCStartAddr,
            pCapPic->u32Width,
            pCapPic->u32Height,
            pCapPic->u32Stride,
            pCapPic->enVideoFormat,
            pCapPic->enFieldMode,
            pCapPic->enSampleType);
#endif
    memset(&stSurInfo,0,sizeof(stSurInfo));
    stSurInfo.Width = pCapPic.u32Width;
    stSurInfo.Height = pCapPic.u32Height;
    stSurInfo.PixelFormat = HIGO_PF_YUV420;
    stSurInfo.pPhyAddr[0] = (HI_CHAR*)pCapPic.u32TopYStartAddr;
    stSurInfo.pPhyAddr[1] = (HI_CHAR*)pCapPic.u32TopCStartAddr;
    stSurInfo.Pitch[0] = pCapPic.u32Stride;
    stSurInfo.Pitch[1] = pCapPic.u32Stride;
    stSurInfo.pVirAddr[0] = (HI_CHAR*)pCapPic.u32TopYStartAddr;
    stSurInfo.pVirAddr[1] = (HI_CHAR*)pCapPic.u32TopCStartAddr;
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if(Region_p->LayerID == OsdLayerId) 
		{
			hOsdSurface = Region_p->SurfaceHandle;
	    	break;
	    }	    
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}

    YS_BlitClaim();
    ErrCode = HI_GO_CreateSurfaceFromMem(&stSurInfo, &hMemSurface);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_STILLDrawIFrame HI_GO_CreateSurfaceFromMem ErrCode 0x%08x\n",ErrCode); 
		YS_BlitRelease();
        return ErrCode;       
    }
    memset(&stBltOpt,0,sizeof(HIGO_BLTOPT2_S));
    ErrCode = HI_GO_StretchBlit(hMemSurface,HI_NULL,hOsdSurface,HI_NULL,&stBltOpt);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_STILLDrawIFrame HI_GO_StretchBlit ErrCode 0x%08x\n",ErrCode); 
		YS_BlitRelease();
        return ErrCode;       
    }
	YS_BlitRelease();
#ifdef DRV_OSD_UPDATE
	DRV_OSDUpdate();
#endif	
	return ErrCode;
}


DRV_ErrCode DRV_STILLRegionShow()
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID != OSD_REGION_STILL)||(Region_p->ShowSurfaceVisible == TRUE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0xff);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = TRUE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_STILLRegionHide()
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID != OSD_REGION_STILL)||(Region_p->ShowSurfaceVisible == FALSE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0x00);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = FALSE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_OSDEnable(void)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID == OSD_REGION_STILL)||(Region_p->LayerID >= OSD_REGION_CURSOR1)||(Region_p->ShowSurfaceVisible == TRUE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0xff);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = TRUE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_OSDDisable(void)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID == OSD_REGION_STILL)||(Region_p->LayerID >= OSD_REGION_CURSOR1)||(Region_p->ShowSurfaceVisible == FALSE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0x0);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = FALSE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}


DRV_ErrCode DRV_STILLEnable(void)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID != OSD_REGION_STILL)||(Region_p->ShowSurfaceVisible == TRUE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0xff);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = TRUE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_STILLDisable(void)
{
	DRV_ErrCode  ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID != OSD_REGION_STILL)||(Region_p->ShowSurfaceVisible == FALSE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0x00);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = FALSE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_CURSOREnable(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID < OSD_REGION_CURSOR1)||(Region_p->ShowSurfaceVisible == TRUE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0xff);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = TRUE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}

DRV_ErrCode DRV_CURSORDisable(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    
    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if((Region_p->LayerID < OSD_REGION_CURSOR1)||(Region_p->ShowSurfaceVisible == FALSE))     
		{
	    	;
	    }
	    else
	    {
	    	//ErrCode = HI_GO_SetWindowOpacity(Region_p.WindowHandle[Region_p->LayerID],0x00);
	        if ( ErrCode != ST_NO_ERROR)
	        {
	            OSD_ERROR (("DRV_OSDEnable ErrCode %d\n",ErrCode));
	            return ErrCode;
	        }
	        Region_p->ShowSurfaceVisible = FALSE;
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    return ErrCode;
}
DRV_ErrCode DRV_OSDUpdate(void)
{
    HIGO_BLTOPT_S stBlitOpt = {0};
    HI_RECT stRect;
    HI_RECT SrcRect;
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    OSD_Region_t* Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion);
    YS_BlitClaim();
    /** draw a rectangle on the surface */
    ErrCode = HI_GO_FillRect(mOsdDevice_p->LayerSurfaceHandle,NULL,0x00000000,HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDRegionCreate HI_GO_FillRect errCode 0x%08x!\n",ErrCode);
    	YS_BlitRelease();
		return HI_FAILURE; 
    }

    while ( Region_p != (OSD_Region_t*)NULL)
    {
		if(Region_p->ShowSurfaceVisible == FALSE)     
		{
	    	;
	    }
	    else
	    {
		    /** move it to graphic layer Surface */
		    SrcRect.x = Region_p->RectIn.x;
		    SrcRect.y = Region_p->RectIn.y;
		    SrcRect.w = Region_p->RectIn.nWidth;
		    SrcRect.h = Region_p->RectIn.nHeight;
		    stRect.x = Region_p->RectOut.x;
		    stRect.y = Region_p->RectOut.y;
		    stRect.w = Region_p->RectOut.nWidth;
		    stRect.h = Region_p->RectOut.nHeight;
            /*OutPut只有在surface uptdate 到layer上时有用,默认满屏输出*/
        	if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080)
        	{
          		stRect.x  = Region_p->RectOut.x;
        		stRect.y  = Region_p->RectOut.y;
        		stRect.w  = Region_p->RectOut.nWidth;
        		stRect.h  = Region_p->RectOut.nHeight;
      	    }
        	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720)
        	{
        		stRect.x  = Region_p->RectOut.x*1920/1280;
        		stRect.y  = Region_p->RectOut.y*1080/720;
        		stRect.w  = Region_p->RectOut.nWidth*1920/1280;
        		stRect.h  = Region_p->RectOut.nHeight*1080/720;
      	    }
        	else if(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576)
        	{
        		stRect.x  = Region_p->RectOut.x*1920/720;
        		stRect.y  = Region_p->RectOut.y*1080/576;
        		stRect.w  = Region_p->RectOut.nWidth*1920/720;
        		stRect.h  = Region_p->RectOut.nHeight*1080/576;
        	}
		    
		    memset(&stBlitOpt,0,sizeof(HIGO_BLTOPT_S));
		    stBlitOpt.EnablePixelAlpha = HI_TRUE;
		    stBlitOpt.EnableGlobalAlpha = HI_TRUE;
		    /** should set the pixel alpha mix mode when enable global alpha*/	
		    stBlitOpt.PixelAlphaComp = HIGO_COMPOPT_NONE;
		    stBlitOpt.EnableScale    = TRUE;
		    //if((Region_p->EnableCK == TRUE)&&(Region_p->ColorKey != 0))
		    //	stBlitOpt.ColorKeyFrom   = HIGO_CKEY_SRC;
		    ErrCode = HI_GO_Blit(Region_p->SurfaceHandle,&SrcRect,mOsdDevice_p->LayerSurfaceHandle,&stRect,&stBlitOpt);
			if(ErrCode == HIGO_ERR_NOCOLORKEY)
			{
				stBlitOpt.ColorKeyFrom = HIGO_CKEY_NONE;
				ErrCode = HI_GO_Blit(Region_p->SurfaceHandle,&SrcRect,mOsdDevice_p->LayerSurfaceHandle,&stRect,&stBlitOpt);
			}
		    if (HI_SUCCESS != ErrCode)
		    {
		    	printf("DRV_OSDUpdate HI_GO_Blit ErrCode 0x%08x\n",ErrCode);
				//YS_BlitRelease();
		        //return ErrCode;
		    }
        }
        Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
    /**fresh the graphic layer */ 
    ErrCode = HI_GO_RefreshLayer(mOsdDevice_p->LayerHandle, NULL);
    if (HI_SUCCESS != ErrCode)
    {
    	printf("DRV_OSDUpdate HI_GO_RefreshLayer ErrCode 0x%08x\n",ErrCode);
		YS_BlitRelease();
        return ErrCode;
    }
	YS_BlitRelease();
    return ErrCode;
}

DRV_ErrCode DRV_RegionDisplayInfo(void)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    OSD_Region_t *Region_p = NULL;
    S32 i = 0;
    
    Region_p = mOsdDevice_p->FirstCreatedRegion;
    while(Region_p != NULL)
    {
        printf("[%d][0x%x]name[%s]",i,Region_p,Region_p->Name);
        printf("layer[");
        switch(Region_p->LayerID)
        {
            case OSD_REGION_CURSOR1:
                printf("CURSOR]");
                break;
            case OSD_REGION_OSD1:
                printf("OSD]");
                break;
            case OSD_REGION_STILL:
                printf("STILL]");
                break;     
        }
        printf("RectIn[%d,%d,%d,%d]",Region_p->RectIn.x,Region_p->RectIn.y,Region_p->RectIn.nWidth,Region_p->RectIn.nHeight);
        printf("RectOut[%d,%d,%d,%d]",Region_p->RectOut.x,Region_p->RectOut.y,Region_p->RectOut.nWidth,Region_p->RectOut.nHeight);
        printf("MixWeight[%d]",Region_p->MixWeight);
        printf("visiable[%d]",Region_p->ShowSurfaceVisible);
        printf("colortype[");
        switch(Region_p->ColorType)
        {
            case OSD_COLOR_TYPE_RGB565:
                printf("RGB565]");
                break;
            case OSD_COLOR_TYPE_ARGB1555:
                printf("ARGB1555]");
                break;
            case OSD_COLOR_TYPE_ARGB4444:
                printf("4444]");
                break;
            case OSD_COLOR_TYPE_CLUT8:
                printf("CLUT8]");
                break;     
            case OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422:
                printf("YCRCR422]");
                break;   
            case OSD_COLOR_TYPE_ARGB8888:
                printf("ARGB8888]");
                break;
        }
		printf("PaletteValid[%d]",Region_p->PaletteValid);
        Region_p = (OSD_Region_t*)Region_p->NextRegion;
        i++;
        printf("\n");
    }
    return 0;
}

OSD_Region_t* DRV_RegionGetHandle(S32 index)
{
    OSD_Region_t *Region_p = NULL;
    S32 i = 0;
    
    Region_p = mOsdDevice_p->FirstCreatedRegion;
    while(Region_p != NULL)
    {
        if(index == i)
            return Region_p;
        i++;
        Region_p = (OSD_Region_t*)Region_p->NextRegion;
    }
    return NULL;
}

DRV_ErrCode DRV_OSDSetScreenModeFake(DRV_OSDScreenMode_t Mode)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;
    if(mOsdDevice_p->ScreenMode == Mode)
	{
		return -1;
	}

	mOsdDevice_p->ScreenMode = Mode;
	return errCode;
}
DRV_ErrCode DRV_OSDSetScreenMode(DRV_OSDScreenMode_t Mode)
{
	OSD_Region_t* Region_p;
	DRV_ErrCode errCode = DRV_NO_ERROR;

	if(mOsdDevice_p->ScreenMode == Mode)
	{
		return -1;
	}
	Region_p  = (OSD_Region_t*) (mOsdDevice_p->FirstCreatedRegion) ;
	while ( Region_p != (OSD_Region_t*)NULL)
	{
		if((Mode == OSD_SCREEN_MODE_720_576)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*720/1920;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*576/1080;
		}
		else if((Mode == OSD_SCREEN_MODE_720_576)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*720/1280;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*576/720;
		}
		else if((Mode == OSD_SCREEN_MODE_1280_720)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*1280/720;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*720/576;
		}
		else if((Mode == OSD_SCREEN_MODE_1280_720)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1920_1080))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*1280/1920;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*720/1080;
		}
		else if((Mode == OSD_SCREEN_MODE_1920_1080)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_720_576))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*1920/720;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*1080/576;
		}
		else if((Mode == OSD_SCREEN_MODE_1920_1080)&&(mOsdDevice_p->ScreenMode == OSD_SCREEN_MODE_1280_720))
		{
			Region_p->RectIn.x = 0;
			Region_p->RectIn.y = 0;
			Region_p->RectIn.nWidth = Region_p->RectIn.nWidth*1920/1280;
			Region_p->RectIn.nHeight = Region_p->RectIn.nHeight*1080/720;
		}
		else
		{
			return -1;
		}
				
		Region_p = (OSD_Region_t*)Region_p->NextRegion;    
	}
	mOsdDevice_p->ScreenMode = Mode;
	OSD_INFO(("DRV_OSDSetScreenMode set Mode %d!\n",Mode));
	return errCode;
}

DRV_ErrCode DRV_OSDGetScreenMode(DRV_OSDScreenMode_t *Mode)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;
	*Mode = mOsdDevice_p->ScreenMode;
	return errCode;
}
#ifndef  DRV_SLZK_SUPPORT
DRV_ErrCode DRV_OSDSetFont(DRV_OSDFont_t Font,U32 Size)
{
	if(Size == 40)
	{
		gFontWidth=ascii_hei_40_width_data;
		gFonthzData=fonthz_hei_40_data;
		gAsciiData=ascii_hei_40_data;
		gCHI_FONT_WIDTH  =         40;
		gCHI_FONT_HEIGHT         = 40;
		gCHI_FONT_PITCH         =5;
		gCHI_FONT_SIZE           =200;
	}
	else if(Size == 20)
	{
		gFontWidth=ascii_hei_20_width_data;
		gFonthzData=fonthz_hei_20_data;
		gAsciiData=ascii_hei_20_data;
		gCHI_FONT_WIDTH  =         20;
		gCHI_FONT_HEIGHT         = 20;
		gCHI_FONT_PITCH         =3;
		gCHI_FONT_SIZE           =60;
	}
	else
	{
		OSD_INFO(("DRV_OSDSetFont set Size %d,but not supported!\n",Size));
		return ST_ERROR_BAD_PARAMETER;
	}
	mOsdDevice_p->Font = Font;
	mOsdDevice_p->FontSize = Size;
	OSD_INFO(("DRV_OSDSetFont set Font %d,Size %d!\n",Font,Size));
	return ST_NO_ERROR;
}
#else
DRV_ErrCode DRV_OSDSetFont(DRV_OSDFont_t Font,U32 Size)
{
	DRV_ErrCode          ErrCode = ST_NO_ERROR;
	HIGO_TEXT_INFO_S pInfo;
	if(Size == 40)//为了兼容其他模块临时添加
	{
		gFontWidth=ascii_hei_40_width_data;
		gFonthzData=fonthz_hei_40_data;
		gAsciiData=ascii_hei_40_data;
		gCHI_FONT_PITCH         =5;
		gCHI_FONT_SIZE           =200;
	}
	else if(Size == 20)
	{
		gFontWidth=ascii_hei_20_width_data;
		gFonthzData=fonthz_hei_20_data;
		gAsciiData=ascii_hei_20_data;
		gCHI_FONT_PITCH         =3;
		gCHI_FONT_SIZE           =60;
	}
	if(Size <= 100)
	{
		gCHI_FONT_WIDTH  =         Size;
		gCHI_FONT_HEIGHT         = Size;
	}
	else
	{
		printf("DRV_OSDSetFont set Size %d,but not supported!\n",Size);
		return ST_ERROR_BAD_PARAMETER;
	}
	mOsdDevice_p->Font = Font;
	mOsdDevice_p->FontSize = Size;
	pInfo.pSbcFontFile = "./simhei_ms.ttf";
	pInfo.pMbcFontFile = NULL;
	pInfo.u32Size = Size;
	printf("\n[%s]--Start-------->gSi_hFont=%x\n",__FUNCTION__,gSi_hFont);
	YS_BlitClaim();
	ErrCode = HI_GO_CreateTextEx(&pInfo, &gSi_hFont);
	if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]failed to create the font ErrCode:0x%x\n",__FUNCTION__,ErrCode);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	printf("\n[%s]--end-------->gSi_hFont=%x\n",__FUNCTION__,gSi_hFont);
	ErrCode = HI_GO_SetTextStyle(gSi_hFont, HIGO_TEXT_STYLE_BOLD);
	if (HI_SUCCESS != ErrCode)
	{
		printf("[%s]failed to SetTextStyle ErrCode:0x%x,gSi_hFont[%x]\n",__FUNCTION__,ErrCode,gSi_hFont);
		YS_BlitRelease();
		return HI_FAILURE; 
	}
	YS_BlitRelease();
	printf("\nDRV_OSDSetFont set Font %d,Size %d!\n",Font,Size);
	return ST_NO_ERROR;
}
#endif
DRV_ErrCode DRV_OSDGetFont(DRV_OSDFont_t *Font,U32 *Size)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;
	*Font = mOsdDevice_p->Font;
	*Size = mOsdDevice_p->FontSize;
	return errCode;
}

DRV_ErrCode DRV_OSDSetMode(DRV_OSDMode mode)
{
	DRV_ErrCode          ErrCode = ST_NO_ERROR;
	HI_UNF_ENC_FMT_E enEncodingFormat;
	if((mode != OSD_NTSC)&&(mode != OSD_NTSC_443)\
	&&(mode != OSD_PAL))
	{
		OSD_ERROR(( "\nMode Error \n"));
		return 0;  
	}
	if(mOsdDevice_p->Mode == mode)
	{
		return 0;
	}
	switch(mode)
	{
		case OSD_NTSC:
			enEncodingFormat = HI_UNF_ENC_FMT_NTSC;
			break;
		case OSD_NTSC_443:
			enEncodingFormat = HI_UNF_ENC_FMT_NTSC_J;
			break;
		case OSD_PAL:
			enEncodingFormat = HI_UNF_ENC_FMT_PAL;
			break;	
		default:
			OSD_ERROR(("Not surport Denc Mode\n"));
			return -1;    
	}   
	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_SD0, enEncodingFormat);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDSetMode HI_UNF_DISP_SetFormat ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
	mOsdDevice_p->Mode = mode;
	return ErrCode;       
}


DRV_ErrCode DRV_OSDSetVtgTimingMode(DRV_VtgTimingMode_t mode)
{
	DRV_ErrCode          ErrCode = ST_NO_ERROR;
	HI_UNF_ENC_FMT_E enEncodingFormat;
	S32 videovisable;
    S32 Layerenabled[3];
    DRV_RectWh VideoWin;
	
	if((mode != DRV_DISPLAY_TIMING_MODE_720P_50)&&(mode != DRV_DISPLAY_TIMING_MODE_720P_60)\
	&&(mode != DRV_DISPLAY_TIMING_MODE_1080i_50)&&(mode != DRV_DISPLAY_TIMING_MODE_1080P_50)&&(mode != DRV_DISPLAY_TIMING_MODE_1080P_60))
	{
		OSD_ERROR(( "\nMode Error \n"));
		return 0;  
	}
	if(mOsdDevice_p->VtgTimingMode == mode)
	{
		return 0;
	}
	switch(mode)
	{
		case DRV_DISPLAY_TIMING_MODE_720P_50:
			enEncodingFormat = HI_UNF_ENC_FMT_720P_50;
			break;
		case DRV_DISPLAY_TIMING_MODE_720P_60:
			enEncodingFormat = HI_UNF_ENC_FMT_720P_60;
			break;
		case DRV_DISPLAY_TIMING_MODE_1080i_50:
			enEncodingFormat = HI_UNF_ENC_FMT_1080i_50;
			break;
		case DRV_DISPLAY_TIMING_MODE_1080P_60:
			enEncodingFormat = HI_UNF_ENC_FMT_1080P_60;
			break;
		case DRV_DISPLAY_TIMING_MODE_1080P_50:
			enEncodingFormat = HI_UNF_ENC_FMT_1080P_50;
			break;
		default:
			enEncodingFormat = HI_UNF_ENC_FMT_1080i_50;
			break;
	}
	ErrCode = HI_UNF_DISP_SetFormat(HI_UNF_DISP_HD0, enEncodingFormat);
    if (HI_SUCCESS != ErrCode) 
    {
        printf("DRV_OSDSetVtgTimingMode HI_UNF_DISP_SetFormat ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
    mOsdDevice_p->VtgTimingMode = mode; 
	return ErrCode;       
}

DRV_ErrCode DRV_OSDGetMode(DRV_OSDMode *SdMode,DRV_VtgTimingMode_t* HdMode)
{
	if((mOsdDevice_p == NULL)||(SdMode == NULL)||(HdMode == NULL))
		return -1;
	
	*SdMode = mOsdDevice_p->Mode;
	*HdMode = mOsdDevice_p->VtgTimingMode;
	return 0;
}



/*******************************************************************************
函数名称:	GetDrvOsdVersion
功    能:	得到OSD模块的版本号,由串口输出
参    数:	无
返 回 值:	版本号时间，如:0x08042916表示08年4月29日16点
*******************************************************************************/
U32 GetDrvOsdVersion()//其它模块会调用到来本接口做为对照
{
	U32 ver = 0x11062818;
	OSD_INFO((("DRV_OSD Version: 7101/7167 20%02x-%02x-%02x-%02x"),((ver>>6) & 0xff),((ver>>4) & 0xff),((ver>>2) & 0xff),(ver & 0xff)));
	return ver;
}

/*******************************************************************************
函数名称:	DRV_OSDClearScreen
功    能:	清屏
参    数:	无
返 回 值:	
*******************************************************************************/
DRV_ErrCode DRV_OSDClearScreen(void)
{
	 DRV_ErrCode  ErrCode = HI_SUCCESS;

    if (OsdLayerSurfaceHandle <= 0)
        return -1;

    YS_BlitClaim();
    ErrCode = HI_GO_FillRect(OsdLayerSurfaceHandle, NULL, 0x00000000, HIGO_COMPOPT_NONE);
	if(HI_SUCCESS == ErrCode)
	{
		ErrCode = HI_GO_RefreshLayer(mOsdDevice_p->LayerHandle, NULL);
	}
    YS_BlitRelease();
	
    return ErrCode;
}

