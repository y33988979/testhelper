/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosd.h

    Description : ͼ������ģ�飬��Ҫ�ṩͼ�λ��ƹ��ܣ�
    ʵ���ߡ����Ρ�Բ������䡢�ı�����ɫ��ͼ��
    �ȵĻ��ƣ��������豸����ģ���ṩ�ķ���ӿڣ�

    Others:      

    History:     
    ======================================================================
    ʱ    �䣺2004-03-12
    ��    �ߣ�Cao Songtao
    ˵    ����1. ����
    ======================================================================
    ʱ    �䣺2004-03-01
    ��    �ߣ�XXX
    ˵    ����1. �������
              2. �޸����ݼ���
              3. ����

*******************************************************************************/

#ifndef _DRVIER_H_
#define _DRVIER_H_
/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "drv_snd.h"
#include "drv_basic.h"
#include "drv_avplay.h"
#include "drv_cas.h"
#include "drv_os.h"
#include "drv_sys.h"
#include "ys_os.h"
#include "drv_typedef.h"
#include "ysav.h"
#include "ysfdata.h"
#include "ysfilter.h"
#include "yshdmi.h"
#include "ysnvm.h"
#include "ysosd.h"
#include "ysosdecompress.h"
#include "ysoslist.h"
#include "ysostime.h"
#include "ystuner.h"

#include "ysvfs.h"
#include "ysusb.h"
#include "yspanel.h"
#include "ysdisk.h"
#include "ip_cfg.h"
#include "drv_memory.h"
#include "drv_debug.h"
#include "ysreset.h"

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum {
    even_parity	= 0,
    odd_parity  = 1
} key_parity_t;
/*************************************************
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // �������˵��������ÿ����������
                  // �á�ȡֵ˵�����������ϵ��
* Output:         // �����������˵����
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
*************************************************/
//��ȡdriver.lib�İ汾��
unsigned int GetDrvVersion(void);
//��ӡdriver.lib�İ汾�ż�������Ϣ��
void ShowDrvVersion(void );
U32 DRV_GetLoaderSWVer(void);
/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif

#endif //_DRVIER_H__




