/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_DRV_H__
#define __HLP_DRV_H__


#include <hlp_config.h>
#include <hlp_core.h>

#define callback  int
#define DRV_ErrCode int

void DRV_LED_UnDisplayTime();
void DRV_LED_DisplayTime(U32 hour, U32 min);
void DRV_LED_DisplayChar(U8 d1,U8 d2,U8 d3,U8 d4);
int DRV_UsbInit(void);
DRV_ErrCode DRV_InitBasic(void);
DRV_ErrCode DRV_PanelInit(int PanelType);
DRV_ErrCode DRV_PanelEnableCallback(int Callback_f);
void register_app_mountok_callback(callback cb);
void register_app_plugout_callback(callback cb);
void unregister_app_mountfailed_callback();
U8 drv_SystemIsOK(pid_t status);



#endif /* __HLP_DRV_H__ */

