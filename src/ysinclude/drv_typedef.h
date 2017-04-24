/*****************************************************************************
*
* Description: 
*    驱动层类型定义头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-12    Qiujordan      创建
*
*****************************************************************************/
#ifndef JZ_DRIVER_LAYER_TYPEDEF_H
#define JZ_DRIVER_LAYER_TYPEDEF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MODULE
#include <string.h>
#endif
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include <sttbx.h>
#include <hi_type.h>
/*基本类型定义*/
#include "ys_typedef.h"

#define VALID_FLAG_8  0x93// Do here use const signed char
#define VALID_FLAG_16  0xa50f// Do here use const signed short
#define VALID_FLAG_32  0xaa5500ff// Do here use const signed int
#if 0
extern const signed char VALID_FLAG_8;
extern const signed short VALID_FLAG_16;
extern const signed int VALID_FLAG_32;
#endif
#define DRV_NO_ERROR HI_SUCCESS

#define DRV_CHECK(Fun)  { DRV_ErrCode err = Fun; \
                          if(err != DRV_NO_ERROR) \
                          { \
							  printf("ERROR"); \
                              return err; \
                          } }

#define DRV_CHECKGOTO(Fun)        \
    {                             \
        DRV_ErrCode err = Fun;    \
        if(err != DRV_NO_ERROR)   \
        {                         \
            goto ERROR;           \
        }                         \
    }                             
enum
{
	DRV_DRVLAYER_BASE  = 0,
	DRV_OS_BASE        = 1000,
	DRV_AUDIO_BASE     = 2000,
	DRV_CAS_BASE       = 3000,
	DRV_FILTER_BASE    = 4000,
	DRV_DISPLAY_BASE   = 5000,
	DRV_INIT_BASE      = 6000,
	DRV_PANEL_BASE     = 7000,
	DRV_NVM_BASE       = 8000,
	DRV_OSD_BASE       = 9000,
	DRV_TUNER_BASE     = 10000,
	DRV_VIDEO_BASE     = 11000
};

enum
{
    DRV_ERROR_DRVLAYER = DRV_DRVLAYER_BASE +1 ,
    DRV_ERROR_BAD_PARAMETER,            /* Bad parameter passed       */
    DRV_ERROR_NO_MEMORY,                /* Memory allocation failed   */
    DRV_ERROR_UNKNOWN_DEVICE,           /* Unknown device name        */
    DRV_ERROR_ALREADY_INITIALIZED,      /* Device already initialized */
    DRV_ERROR_NO_FREE_HANDLES,          /* Cannot open device again   */
    DRV_ERROR_OPEN_HANDLE,              /* At least one open handle   */
    DRV_ERROR_INVALID_HANDLE,           /* Handle is not valid        */
    DRV_ERROR_FEATURE_NOT_SUPPORTED,    /* Feature unavailable        */
    DRV_ERROR_TIMEOUT,                  /* Timeout occured            */
    DRV_ERROR_DEVICE_BUSY,               /* Device is currently busy   */
    DRV_ERROR_DEVICEFILE_OPEN,            /* Bad parameter passed       */
    DRV_ERROR_DEVICEFILE_PATH,
    DRV_ERROR_IOCTL,
    DRV_ERROR_NO_INIT,                  /* driver api called before driver init */
    DRV_ERROR_REGISTER_COMMAND,          /* failed register testtool command */
    DRV_ERROR_CREATE_TMPFILE,             /* create temp file failed */
    DRV_ERROR_WRITE_FILE,
    DRV_ERROR_OPEN_FILE
};


typedef const char *DRV_Revision;

typedef S32_YS DRV_ErrCode;
typedef S32_YS DRV_Handle;

typedef struct DRV_Point_s
{
    S32_YS PositionX;
    S32_YS PositionY;
} DRV_Point;

typedef struct DRV_Rect_s
{
    S32_YS StartX;
    S32_YS StartY;
    S32_YS EndX;
    S32_YS EndY;
} DRV_Rect;

typedef struct DRV_RectWh_s
{
    S32_YS x;
    S32_YS y;
    U32_YS nWidth;
    U32_YS nHeight;
} DRV_RectWh;

/* 定义驱动模块类型, 用6位表示, 最大为63. */
typedef enum DRV_Module_e
{
    DRV_MODULE_APP    ,/* 0 */
    DRV_MODULE_BASIC  ,/* 1 */
    DRV_MODULE_OS     ,/* 2 */
    DRV_MODULE_AUDIO  ,/* 3 */
    DRV_MODULE_CAS    ,/* 4 */
    DRV_MODULE_DEBUG  ,/* 5 */
    DRV_MODULE_DISPLAY,/* 6 */
    DRV_MODULE_FILTER ,/* 7 */
    DRV_MODULE_INIT   ,/* 8 */
    DRV_MODULE_NVM    ,/* 9 */
    DRV_MODULE_OSD    ,/* 10 */
    DRV_MODULE_PANEL  ,/* 11 */
    DRV_MODULE_TUNER  ,/* 12 */
    DRV_MODULE_VIDEO  ,/* 13 */
    DRV_MODULE_NET    ,/* 14 */
    DRV_MODULE_DVM    ,/* 15 */
    DRV_MODULE_PLAYER ,/* 16 */
    DRV_MODULE_MISC ,/* 17 */    
    DRV_MODULE_DISK ,/* 18 */    
    DRV_MODULE_MEDIAPLAYER ,/* 19 */
    DRV_MODULE_PLAYREC ,/* 20 */    
    DRV_MODULE_BUFFER ,/* 21 */    
    DRV_MODULE_MAX_NB
} DRV_Module;

typedef enum DRV_DefinitionMode_e
{
    DRV_DM_HD   = 1,
    DRV_DM_SD   = 2,
    DRV_DM_BOTH = DRV_DM_HD | DRV_DM_SD,
    DRV_DM_MAX
} DRV_DefinitionMode;

#ifndef ROM_TAG_SIZE
#define ROM_TAG_SIZE 1024
#endif

typedef struct 
{
    CHAR_YS  DesStringReserved[16];    CHAR_YS  ReservedByte[(ROM_TAG_SIZE-16-14*16)];	
    CHAR_YS  DesStringLDBuilder[16];   CHAR_YS  LDBuilderStr[48];   // 14*16 Byre 
    CHAR_YS  DesStringLDBuildDate[16]; CHAR_YS  LDBuildDateStr[16]; // 10*16Byre
    CHAR_YS  DesStringLDBuildTime[16]; CHAR_YS  LDBuildTimeStr[16]; // 8*16Byre
    CHAR_YS  DesStringLDSWVerStr[16];  CHAR_YS  LDSWVerStr[16];     // 6*16Byre
    CHAR_YS  DesStringLDSWVer[12];     U32_YS   LDSWVerU32;         // 4*16Byre
    CHAR_YS  DesStringLDHWVer[12];     U32_YS   LDHWVerU32;         // 3*16Byre
    CHAR_YS  DesStringLDTypeID[12];    U32_YS   LDTypeIDU32;        // 2*16Byre
    CHAR_YS  DesStringCheckSum[12];    U32_YS   CheckSumU32;        // 1*16Byre
} YS_RomTag_t;//总长度为1K (1024) Byte


#ifdef __cplusplus
}
#endif

#endif

