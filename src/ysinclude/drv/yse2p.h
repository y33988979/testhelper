/*****************************************************************************

File Name   : e2p.h

Description : EEPROM header

Copyright (C) 1999 STMicroelectronics

*****************************************************************************/
#ifndef _YSE2P_H_
#define _YSE2P_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "drv_os.h"

/*=========================================================================
    Global Variables
=========================================================================== */
/*保护stbinfo分区读写，stbinfo分区只能在一次操作完后在进行下一次操作*/
extern DRVOS_Sem_t  *m_StbInfoRWMutex;

/*=========================================================================
    Global function declarations
=========================================================================== */
DRV_ErrCode DRV_E2pSetup(void);

DRV_ErrCode DRV_E2pTerm(void);


#ifdef __cplusplus
}
#endif
#endif
