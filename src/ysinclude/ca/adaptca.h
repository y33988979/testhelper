/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : adaptca.h

    Version   : 1.0

    Description : ca 模块头文件

    Others:       // 其它内容的说明
 
*******************************************************************************/

#ifndef _ADAPTERCA_H__
#define _ADAPTERCA_H__
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#ifdef CDCAS30
#include "CDCASa.h"
#include "ca_adapter.h"
#include "ca_osd.h"
#include "ca_smartcard.h"
#endif

#ifdef SMSX51
#include "ca_adapter.h"
#include "ca_osd.h"
#include "ca_smartcard.h"
#include "DVTCAS_STBDataPublic.h"
#include "DVTCAS_STBInf.h"
#include "DVTSTB_CASInf.h"
#endif

#endif
