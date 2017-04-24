/**
 \file
 \brief common head file or uart
 \author wrwei
 \date 2012.05.22
 */

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


#ifdef __cplusplus
extern "C" {
#endif

int DRV_MouseOpen(void);

int DRV_MouseOpen(void);

int DRV_MouseRead(U32 handle, S32 *type, S32 *xOff, S32 *yOff );

#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */

