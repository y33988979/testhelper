#ifndef __YS_HDMI_H__
#define __YS_HDMI_H__
#include <hi_unf_hdmi.h>
#include <hi_type.h>
#include "drv_typedef.h"
#include <hi_unf_ecs.h>
#include "drv_os.h"

typedef void (*User_HDMI_CallBack)(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData);
typedef enum DRV_UsbWifiState_e
{
	DRV_UsbWIFI_STATE_RESERVED=0,
	DRV_HDMI_Switch_IN,
	DRV_HDMI_Switch_REMOVE,
} DRV_HDMI_SwitchState;
typedef void (*DRV_HDMI_SwitchCallback)(DRV_HDMI_SwitchState SwitchState);

HI_UNF_ENC_FMT_E stringToUnfFmt(HI_CHAR *pszFmt);
HI_S32 HIADP_HDMI_Init(HI_UNF_HDMI_ID_E enHDMIId, HI_UNF_ENC_FMT_E enWantFmt);
HI_S32 HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_E enHDMIId);
HI_S32 HIADP_HDMI_SetAdecAttr(HI_UNF_SND_INTERFACE_E enInterface, HI_UNF_SAMPLE_RATE_E enRate);
HI_VOID HDMI_PrintSinkCap(HI_UNF_HDMI_SINK_CAPABILITY_S *pCapbility);
DRV_ErrCode DRV_HdmiInit(void);
DRV_ErrCode DRV_HdmiTerm(void);
DRV_ErrCode DRV_HdmiEdidSurpportDD(void);

#ifdef DRV_HdmiSwitch
void register_hdmi_switch_callback(DRV_HDMI_SwitchCallback cb);
void unregister_hdmi_switch_callback(void);
DRV_ErrCode DRV_HdmiSwitchInit(void);
DRV_ErrCode DRV_HdmiSwitchSet_STB(void);
DRV_ErrCode DRV_HdmiSwitchSet_UnSTB(void);
DRV_ErrCode DRV_HdmiSwitchSet_Un2STB(void);
#endif

#endif /* #if pub_HDMI_H_ */

