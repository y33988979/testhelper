/******************************************************************************
*
*  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
*
*******************************************************************************
*  File Name     : hi_bluetooth.h
*  Version       : Initial Draft
*  Author        : f00172091
*  Created       : 2011/4/21
*  Last Modified :
*  Description   : Bluetooth define
*  Function List :
*
*
*  History:
* 
*       1.  Date         : 2011/4/21
*           Author       : f00172091
*           Modification : Created file
*
******************************************************************************/
#ifndef __HI_BLUETOOTH_H__
#define __HI_BLUETOOTH_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/

#include "hi_bluetooth_err.h"

/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/

#define HI_BT_ERR(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_USR, fmt)

#define HI_BT_WARN(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_WARNING, HI_DEBUG_ID_USR, fmt)

#define HI_BT_INFO(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_USR, fmt)

#define HI_BLUETOOTH_ADP_NAME_LEN_MAX   8
#define HI_BLUETOOTH_DEV_NAME_LEN_MAX   48
#define HI_BLUETOOTH_ADDRESS_LEN_MAX    20

#if 1   //add by ganwei
#define HI_SUCCESS 0


#endif

#define BLUETOOTH_ACTIVE_DEVICE_FILE "/usr/share/bluetooth/var/lib/bluetooth/ActiveDev"

#ifndef HI_BLUETOOTH_DEV_LIST_MAX
#define HI_BLUETOOTH_DEV_LIST_MAX    10
#endif

/**Define the bluetooth adapter control command*/
typedef enum hi_BLUETOOTH_ADAPTER_CTRL_E
{
    HI_BLUETOOTH_ADP_CTRL_DOWN = 0x00,
    HI_BLUETOOTH_ADP_CTRL_UP,
    HI_BLUETOOTH_ADP_CTRL_RESET,
    HI_BLUETOOTH_ADP_CTRL_BUTT
} HI_BLUETOOTH_ADAPTER_CTRL_E;

/**Define the bluetooth adapter status*/
typedef enum hi_BLUETOOTH_ADAPTER_STATUS_E
{
    HI_BLUETOOTH_ADP_STATUS_DOWN = 0x00,
    HI_BLUETOOTH_ADP_STATUS_UP,
    HI_BLUETOOTH_ADP_STATUS_NOTEXIST,
    HI_BLUETOOTH_ADP_STATUS_BUTT
} HI_BLUETOOTH_ADAPTER_STATUS_E;

/**Define the bluetooth device status*/
typedef enum hi_BLUETOOTH_DEVICE_STATUS_E
{
    HI_BLUETOOTH_DEV_STATUS_DISCONNECTED = 0x00,
    HI_BLUETOOTH_DEV_STATUS_CONNECTED,
    HI_BLUETOOTH_DEV_STATUS_BUTT
} HI_BLUETOOTH_DEVICE_STATUS_E;

/**The bluetooth adapter attribute struct*/
typedef struct hi_BLUETOOTH_ADP_ATTR_S
{   
    char szName[HI_BLUETOOTH_ADP_NAME_LEN_MAX];
    char szAdpAddr[HI_BLUETOOTH_ADDRESS_LEN_MAX];
    HI_BLUETOOTH_ADAPTER_STATUS_E u32Status;
}HI_BLUETOOTH_ADP_ATTR_S;

/**The bluetooth device attribute struct*/
typedef struct hi_BLUETOOTH_DEV_ATTR_S
{   
    char szName[HI_BLUETOOTH_DEV_NAME_LEN_MAX];
    char szDevAddr[HI_BLUETOOTH_ADDRESS_LEN_MAX];
    HI_BLUETOOTH_DEVICE_STATUS_E u32Status;
    int s32Paired;
}HI_BLUETOOTH_DEV_ATTR_S;


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

/**
 \brief Get the default bluetooth adapter
 \attention \n
None
 \param[out] pstBTDefaultAdp  The pointer of default adapter
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

int HI_BLUETOOTH_GetDefaultAdapter(HI_BLUETOOTH_ADP_ATTR_S *pstBTDefaultAdp);


/**
 \brief Get the bluetooth adapter list
 \attention \n
None
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTAdpList  The pointer of adapter list
 \param[out] pu32Num  The returned number of adapters
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetAdapterList(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdpList,unsigned int u32MaxNum,unsigned int *pu32Num);

/**
 \brief Control the bluetooth adapter
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] enCmd  The control command
 \param[in] pstPara  The parameter of control command (reserved)
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_AdapterCtrl(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_ADAPTER_CTRL_E enCmd, void *pstPara);

/**
 \brief Get the bluetooth adapter status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[out] penStatus  The status of bluetooth adapter
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetAdapterStatus(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_ADAPTER_STATUS_E *penStatus);


/**
 \brief Scan the bluetooth device, and return the pairing status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTDevList  The pointer of scaned bluetooth devices list
 \param[out] pu32Num  The return number of scaned bluetooth devices
 \retval ::0  SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_ScanDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDevList,unsigned int u32MaxNum,unsigned int *pu32Num);


/**
 \brief Pair the bluetooth device with adapter
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The pointer of bluetooth device
 \param[in] szPassword  The password of bluetooth device
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_Pairing(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, char *szPassword);

/**
 \brief Get the paired bluetooth devices
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTDevList  The list of paired bluetooth devices
 \param[out] pu32Num  The number of paired bluetooth devices
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetPairedDevList(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDevList ,unsigned int u32MaxNum, unsigned int *pu32Num);


/**
 \brief Remove the paired bluetooth device
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The paired bluetooth device which should be removed
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_RemovePairedDev(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev);


/**
 \brief Get the bluetooth device status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The bluetooth device
 \param[out] penStatus  The status of bluetooth device
 \retval ::0  SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetDevStatus(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, HI_BLUETOOTH_DEVICE_STATUS_E *penStatus );

/**
 \brief Connect the bluetooth device
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The bluetooth device
 \param[in] bConnect  HI_TRUE--Connect HI_FALSE--Disconnect
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_Connect(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, char bConnect);

/**
 \brief Get connected headset
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[out] pstBTDev  The connected bluetooth device
 \retval ::0 SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */

int HI_BLUETOOTH_GetConnectedHeadset(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev);

#if 0

/**
 \brief ring the bluetooth headset
 \attention the bluetooth headset must be connected
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The pointer of bluetooth device
 \param[in] bRing  HI_TRUE--Start ring HI_FALSE--Stop ring
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

int HI_BLUETOOTH_Headset_Ring(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, HI_BOOL bRing);

/**
 \brief Save the active bluetooth device for other application (such as VOIP) use 
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The pointer of bluetooth device
 \retval ::0  SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */

int HI_BLUETOOTH_SaveActiveDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp,HI_BLUETOOTH_DEV_ATTR_S *pstBTDev);

/**
 \brief Get the active bluetooth device for other application (such as VOIP) use 
 \attention \n
None
 \param[out] pstBTAdp  The pointer of bluetooth adapter
 \param[out] pstBTDev  The pointer of bluetooth device
 \retval ::0  SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */

int HI_BLUETOOTH_GetActiveDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp,HI_BLUETOOTH_DEV_ATTR_S *pstBTDev);
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif

