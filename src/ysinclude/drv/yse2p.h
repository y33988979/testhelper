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
/*����stbinfo������д��stbinfo����ֻ����һ�β�������ڽ�����һ�β���*/
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
