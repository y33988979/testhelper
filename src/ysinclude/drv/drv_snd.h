/**
 \file
 \brief common head file or PVR
 \copyright Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version draft
 \author z40717
 \date 2010-6-12
 */

#ifndef __DRV_SND_H__
#define __DRV_SND_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_unf_ecs.h"
#include <hi_unf_sound.h>
#include "drv_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
/*************************** Structure Definition ****************************/
/********************** Global Variable declaration **************************/
/******************************* API declaration *****************************/
DRV_ErrCode SND_Setup(void);


HI_S32 AudioSIOPinSharedEnable();

HI_VOID AudioSlicTlv320RST();

HI_VOID AudioSPIPinSharedEnable();


#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */
