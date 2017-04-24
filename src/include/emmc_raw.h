/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*    Create by lidongxiang. 2011-09-21
*
******************************************************************************/
#ifndef __EMMC_RAW_H__
#define __EMMC_RAW_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define EMMC_EXT_PART_ID       5
#define EMMC_SECTOR_TAIL      (0xAA55)
#define EMMC_SECTOR_SIZE      (512)

typedef enum eMMCPartType_E
{
    EMMC_PART_TYPE_RAW,
    EMMC_PART_TYPE_LOGIC,

    EMMC_PART_TYPE_BUTT
}EMMC_PART_TYPE_E;

typedef struct emmc_flast_s
{
    HI_U64 u64RawAreaSize;
    HI_U32 u32EraseSize;
}EMMC_FLASH_S;

typedef struct emmc_cb_s
{
    HI_S32 fd;
    HI_U64 u64Address;
	HI_U64 u64PartSize;
	HI_U32 u32EraseSize;
    EMMC_PART_TYPE_E enPartType;

} EMMC_CB_S;

///////////////////////////////////////////////////
HI_S32 emmc_raw_init(void);
EMMC_CB_S *emmc_raw_open(HI_U64 u64Addr,
                         HI_U64 u64Length);
EMMC_CB_S *emmc_node_open(HI_U8 *pu8Node);

HI_S32 emmc_block_read(HI_S32 fd,
                       HI_U32 u32Start,
 		               HI_U32 u32Len,
 		               void *buff);

HI_S32 emmc_block_write(HI_S32 fd,
                        HI_U32 u32Start,
   				        HI_U32 u32Len,
   				        void   *buff);

HI_S32 emmc_raw_read(EMMC_CB_S *pstEmmcCB,
                     HI_U64    u64Offset,
                     HI_U32    u32Length,
                     HI_U8     *buf);

HI_S32 emmc_raw_write(EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,
                      HI_U32    u32Length,
                      HI_U8     *buf);

HI_S32 emmc_raw_close(EMMC_CB_S *pstEmmcCB);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

