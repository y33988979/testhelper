/**
 \file
 \brief common head file or PVR
 \copyright Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version draft
 \author z40717
 \date 2010-6-12
 */

#ifndef __DRV_DISP_H__
#define __DRV_DISP_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_unf_ecs.h"
#include <hi_unf_disp.h>
#include <drv_typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern HI_UNF_ENC_FMT_E mEncFormat;

typedef enum
{
    OSD_NTSC,
    OSD_PAL,
    OSD_NONE,                  /*auto mode*/
    OSD_NTSC_443    
}DRV_OSDMode;

typedef enum DRV_VtgTimingMode_e
{
    DRV_DISPLAY_TIMING_MODE_NTSC,
    DRV_DISPLAY_TIMING_MODE_PAL,
    DRV_DISPLAY_TIMING_MODE_NTSC_443,
    DRV_DISPLAY_TIMING_MODE_1080P_50,
    DRV_DISPLAY_TIMING_MODE_1080i_50,
    DRV_DISPLAY_TIMING_MODE_1080i_60,
    DRV_DISPLAY_TIMING_MODE_720P_50,
    DRV_DISPLAY_TIMING_MODE_720P_60,
    DRV_DISPLAY_TIMING_MODE_1080P_60,
    DRV_DISPLAY_TIMING_MODE_NONE
} DRV_VtgTimingMode_t;



DRV_ErrCode DISPLAY_Setup(void);

#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */
