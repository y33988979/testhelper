#ifndef _YSRESET_H_
#define _YSRESET_H_

#include "drv_typedef.h"

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

/*控制待机后显示模式；0:不显示；1:显示时间 ; 2:显示节目频道(暂时没有实现)*/
void DRV_Standby_DISP(U8 Flag);
DRV_ErrCode RebootSTB(void);
DRV_ErrCode  DRV_RealStandBy();


#ifdef __cplusplus
}
#endif
#endif
