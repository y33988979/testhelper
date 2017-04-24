/**
 \file
 \brief common head file or PVR
 \copyright Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version draft
 \author z40717
 \date 2010-6-12
 */

#ifndef __DRV_VO_H__
#define __DRV_VO_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_unf_ecs.h"
#include "hi_unf_vo.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DRV_ScreenRatioConversion_e
{
    DRV_AR_CONVERSION_PAN_SCAN   = 1,
    DRV_AR_CONVERSION_LETTER_BOX = 2,
    DRV_AR_CONVERSION_COMBINED   = 4,
    DRV_AR_CONVERSION_IGNORE     = 8
} DRV_ScreenRatioConversion;

typedef enum DRV_ScreenRatio_e
{
    DRV_SCREEN_RATE_4_3   = 0,
    DRV_SCREEN_RATE_16_9   = 1,
    DRV_SCREEN_RATE_AUTO   = 2
} DRV_ScreenRatio;

extern HI_HANDLE mAvPlayWinHandle;


DRV_ErrCode VO_Setup(void);

#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */
