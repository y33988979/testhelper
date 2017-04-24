/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosd.h

    Description : 图形引擎模块，主要提供图形绘制功能，
    实现线、矩形、圆、域填充、文本、颜色、图形
    等的绘制；它调用设备驱动模块提供的服务接口；

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-12
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2004-03-01
    作    者：XXX
    说    明：1. 问题简述
              2. 修改内容简述
              3. 其他

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
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 输入参数说明，包括每个参数的作
                  // 用、取值说明及参数间关系。
* Output:         // 对输出参数的说明。
* Return:         // 函数返回值的说明
* Others:         // 其它说明
*************************************************/
//获取driver.lib的版本号
unsigned int GetDrvVersion(void);
//打印driver.lib的版本号及编译信息。
void ShowDrvVersion(void );
U32 DRV_GetLoaderSWVer(void);
/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif

#endif //_DRVIER_H__




