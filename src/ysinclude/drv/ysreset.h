#ifndef _YSRESET_H_
#define _YSRESET_H_

#include "drv_typedef.h"

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

/*���ƴ�������ʾģʽ��0:����ʾ��1:��ʾʱ�� ; 2:��ʾ��ĿƵ��(��ʱû��ʵ��)*/
void DRV_Standby_DISP(U8 Flag);
DRV_ErrCode RebootSTB(void);
DRV_ErrCode  DRV_RealStandBy();


#ifdef __cplusplus
}
#endif
#endif
