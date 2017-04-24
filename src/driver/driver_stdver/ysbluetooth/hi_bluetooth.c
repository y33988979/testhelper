#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "bluetooth_api.h"
#include "hi_bluetooth.h"

#define CHECK_NULL_PTR(ptr)\
do{\
    if(NULL == ptr )\
    {\
        printf("null pointer.\n");\
        return HI_ERR_BLUETOOTH_NULL_PTR;\
    }\
}while(0)

#define CHECK_ADAPTER_NAME(szName)\
do{\
    if(strncmp(szName,"hci",3)!=0)\
        return HI_ERR_BLUETOOTH_INVALID_PARA;\
    if(szName[3]<'0'||(szName[3])>'9')\
        return HI_ERR_BLUETOOTH_INVALID_PARA;\
}while(0)

#define CHECK_BLUETOOTH_ADDRESS(add)\
do{\
    if(bachk(add)<0)\
    {\
        printf("Invalid bluetooth address, %s\n",add);\
        return HI_ERR_BLUETOOTH_INVALID_PARA;\
    }\
}while(0)

static pthread_mutex_t  tBluetoothMutex = PTHREAD_MUTEX_INITIALIZER;

#define BLUETOOTH_MUTEX_LOCK()\
do{\
    (void)pthread_mutex_lock(&tBluetoothMutex);\
}while(0)

#define BLUETOOTH_MUTEX_TRYLOCK()\
do{\
    if(pthread_mutex_trylock(&tBluetoothMutex)!=0)\
    {\
        return HI_ERR_BLUETOOTH_PTHREAD_BUSY;\
    }\
}while(0)

#define BLUETOOTH_MUTEX_UNLOCK()\
do{\
    (void)pthread_mutex_unlock(&tBluetoothMutex);\
}while(0)

    
/**
 \brief Get the default bluetooth adapter
 \attention \n
None
 \param[out] pstBTDefaultAdp  The pointer of default adapter
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetDefaultAdapter(HI_BLUETOOTH_ADP_ATTR_S *pstBTDefaultAdp)
{
    int ret = HI_SUCCESS;
    bdaddr_t stAdpAddr;
    int s32Status;
        
    CHECK_NULL_PTR(pstBTDefaultAdp);
    BLUETOOTH_MUTEX_LOCK();
    
    ret = get_adapter_updown_status("hci0", &s32Status);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get adapter status failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_ADP_FAILURE;
    }

    if(s32Status == HI_BLUETOOTH_ADP_STATUS_DOWN)  //power down status
    {
        ret = enable_adapter("hci0", 1);
        if(HI_SUCCESS != ret)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            printf("power up adapter failure, ret is %d\n",ret);
            return HI_ERR_BLUETOOTH_ADP_FAILURE;
        }
    }
    
    ret = get_adapter_bdaddr("hci0", &stAdpAddr);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get bluetooth adapter 'hci0' address failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_ADP_FAILURE;
    }
    strcpy(pstBTDefaultAdp->szName,"hci0");
    ba2str(&stAdpAddr, pstBTDefaultAdp->szAdpAddr);
    pstBTDefaultAdp->u32Status = HI_BLUETOOTH_ADP_STATUS_UP;

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}

/**
 \brief Get the bluetooth adapter list
 \attention \n
None
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTAdpList  The pointer of adapter list
 \param[out] pu32Num  The returned number of adapters
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetAdapterList(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdpList,unsigned int u32MaxNum,unsigned int *pu32Num)
{
    int ret = HI_SUCCESS;
    bdaddr_t stAdpAddr;
    int s32Status;
        
    CHECK_NULL_PTR(pstBTAdpList);
    CHECK_NULL_PTR(pu32Num);

    if((u32MaxNum == 0)||(u32MaxNum > HI_BLUETOOTH_DEV_LIST_MAX))
    {
        printf("invalid para u32MaxNum=%d\n",u32MaxNum);
        return HI_ERR_BLUETOOTH_INVALID_PARA;
    }

    BLUETOOTH_MUTEX_LOCK();
    
    ret = get_adapter_updown_status("hci0", &s32Status);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get adapter status failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_ADP_FAILURE;
    }

    if(s32Status == HI_BLUETOOTH_ADP_STATUS_DOWN)  //power down status
    {
        ret = enable_adapter("hci0", 1);
        if(HI_SUCCESS != ret)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            printf("power up adapter failure, ret is %d\n",ret);
            return HI_ERR_BLUETOOTH_ADP_FAILURE;
        }
    }
    
    ret = get_adapter_bdaddr("hci0", &stAdpAddr);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get bluetooth adapter 'hci0' address failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_ADP_FAILURE;
    }
    *pu32Num = 1;
    strcpy(pstBTAdpList[0].szName,"hci0");
    ba2str(&stAdpAddr, pstBTAdpList[0].szAdpAddr);
    pstBTAdpList[0].u32Status = HI_BLUETOOTH_ADP_STATUS_UP;

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}

/**
 \brief Control the bluetooth adapter
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] enCmd  The control command
 \param[in] pstPara  The parameter of control command (reserved)
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_AdapterCtrl(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_ADAPTER_CTRL_E enCmd, void *pstPara)
{
    int ret = HI_SUCCESS;
    int s32Status;
    
    CHECK_NULL_PTR(pstBTAdp);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    if(enCmd >= HI_BLUETOOTH_ADP_CTRL_BUTT)
    {
        printf("invalid para enCmd [%d].\n",enCmd);
        return HI_ERR_BLUETOOTH_INVALID_PARA;
    }

    BLUETOOTH_MUTEX_LOCK();
    
    ret = get_adapter_updown_status(pstBTAdp->szName, &s32Status);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get adapter status failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_ADP_FAILURE;
    }

    if(enCmd < HI_BLUETOOTH_ADP_CTRL_RESET)
    {
    
        if(s32Status == enCmd)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            return HI_SUCCESS;
        }
        
        ret = enable_adapter(pstBTAdp->szName, enCmd);
        if(HI_SUCCESS != ret)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            printf("power %s adapter failure, ret is %d\n",enCmd?"up":"down",ret);
            return HI_ERR_BLUETOOTH_ADP_FAILURE;
        }
    }
    else if(enCmd == HI_BLUETOOTH_ADP_CTRL_RESET)
    {
        ret = enable_adapter(pstBTAdp->szName, HI_BLUETOOTH_ADP_CTRL_DOWN);
        if(HI_SUCCESS != ret)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            printf("power %s adapter failure, ret is %d\n",enCmd?"up":"down",ret);
            return HI_ERR_BLUETOOTH_ADP_FAILURE;
        }
        ret = enable_adapter(pstBTAdp->szName, HI_BLUETOOTH_ADP_CTRL_UP);
        if(HI_SUCCESS != ret)
        {
            BLUETOOTH_MUTEX_UNLOCK();
            printf("power %s adapter failure, ret is %d\n",enCmd?"up":"down",ret);
            return HI_ERR_BLUETOOTH_ADP_FAILURE;
        }
    }
    else
    {
        printf("invalid para enCmd [%d].\n",enCmd);
        ret = HI_ERR_BLUETOOTH_INVALID_PARA;
    }

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}

/**
 \brief Get the bluetooth adapter status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[out] penStatus  The status of bluetooth adapter
 \retval ::0 SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetAdapterStatus(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_ADAPTER_STATUS_E *penStatus)
{
    int ret = HI_SUCCESS;
    
    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(penStatus);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);

    BLUETOOTH_MUTEX_TRYLOCK();
    
    ret = get_adapter_updown_status(pstBTAdp->szName, (int*)penStatus);
    if(HI_SUCCESS != ret)
    {
        printf("get adapter status failure, ret is %d\n",ret);
        *penStatus = HI_BLUETOOTH_ADP_STATUS_NOTEXIST;
        ret = HI_ERR_BLUETOOTH_ADP_FAILURE;
    }

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}



/**
 \brief Scan the bluetooth device, and return the pairing status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTDevList  The pointer of scaned bluetooth devices list
 \param[out] pu32Num  The return number of scaned bluetooth devices
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_ScanDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDevList,unsigned int u32MaxNum,unsigned int *pu32Num)
{
    int ret = HI_SUCCESS;
    int scaned_num = 0;
    struct bt_device *pstBTDevListTmp = NULL;
    int paired_num = 0;
    struct bt_device *pstBTPairedDevList = NULL;
    unsigned int i,j;
        
    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDevList);
    CHECK_NULL_PTR(pu32Num);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);

    if((u32MaxNum == 0)||(u32MaxNum > HI_BLUETOOTH_DEV_LIST_MAX))
    {
        printf("invalid para u32MaxNum=%d\n",u32MaxNum);
        return HI_ERR_BLUETOOTH_INVALID_PARA;
    }

    pstBTPairedDevList = malloc(HI_BLUETOOTH_DEV_LIST_MAX*sizeof(struct bt_device));
    if(NULL == pstBTPairedDevList)
    {
        printf("Malloc bluetooth paired device list failure.\n");
        return HI_ERR_BLUETOOTH_MALLOC_MEM_FAILURE;
    }
    memset(pstBTPairedDevList,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(struct bt_device));  

    BLUETOOTH_MUTEX_LOCK();
    
    paired_num = get_paired_devices(pstBTAdp->szName, pstBTPairedDevList, HI_BLUETOOTH_DEV_LIST_MAX);
    if(paired_num < 0)
    {
        printf("get paired bluetooth device failure, ret is %d.\n",paired_num);
    }
    else if(paired_num > HI_BLUETOOTH_DEV_LIST_MAX)
    {
       paired_num = HI_BLUETOOTH_DEV_LIST_MAX;
    }

    pstBTDevListTmp = malloc(u32MaxNum*sizeof(struct bt_device));
    if(NULL == pstBTDevListTmp)
    {
        printf("Malloc bluetooth device list failure.\n");
        ret = HI_ERR_BLUETOOTH_MALLOC_MEM_FAILURE;
        goto finished;
    }
    memset(pstBTDevListTmp,0,u32MaxNum*sizeof(struct bt_device));  
    
    scaned_num = adapter_scan_devices(pstBTAdp->szName, pstBTDevListTmp, u32MaxNum);
    if(scaned_num < 0)
    {
        printf("scan bluetooth device failure, ret is %d.\n",scaned_num);
        ret = HI_ERR_BLUETOOTH_SCAN_FAILURE;
        goto finished;
    }
    else if(scaned_num > u32MaxNum)
    {
       scaned_num = u32MaxNum;
    }

    for(i=0;i<scaned_num;i++)
    {
        memset(&pstBTDevList[i],0,sizeof(struct bt_device));
        strcpy(pstBTDevList[i].szName,pstBTDevListTmp[i].dev_name);
        ba2str(&pstBTDevListTmp[i].bdaddr, pstBTDevList[i].szDevAddr);
        for(j=0;j<paired_num;j++)
        {
            if(0==bacmp(&pstBTDevListTmp[i].bdaddr, &pstBTPairedDevList[j].bdaddr))
            {
                pstBTDevList[i].s32Paired = 1;
            }
        }
    }

finished:
    *pu32Num = scaned_num;
    if(NULL != pstBTDevListTmp)
    {
        free(pstBTDevListTmp);
        pstBTDevListTmp = NULL;
    }
    if(NULL != pstBTPairedDevList)
    {
        free(pstBTPairedDevList);
        pstBTPairedDevList = NULL;
    }
    
    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}


/**
 \brief Pair the bluetooth device with adapter
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The pointer of bluetooth device
 \param[in] szPassword  The password of bluetooth device
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_Pairing(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, char *szPassword)
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;
    int paired_num = 0;
    struct bt_device *pstBTPairedDevList = NULL;
    int j;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_NULL_PTR(szPassword);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);
    
    str2ba(pstBTDev->szDevAddr, &stBTDevTmp.bdaddr);
    
    pstBTPairedDevList = malloc(HI_BLUETOOTH_DEV_LIST_MAX*sizeof(struct bt_device));
    if(NULL == pstBTPairedDevList)
    {
        printf("Malloc bluetooth paired device list failure.\n");
        return HI_ERR_BLUETOOTH_MALLOC_MEM_FAILURE;
    }
    memset(pstBTPairedDevList,0,HI_BLUETOOTH_DEV_LIST_MAX*sizeof(struct bt_device));  

    BLUETOOTH_MUTEX_LOCK();
    
    paired_num = get_paired_devices(pstBTAdp->szName, pstBTPairedDevList, HI_BLUETOOTH_DEV_LIST_MAX);
    if(paired_num < 0)
    {
        printf("get paired bluetooth device failure, ret is %d.\n",paired_num);
    }
    else if(paired_num > HI_BLUETOOTH_DEV_LIST_MAX)
    {
       paired_num = HI_BLUETOOTH_DEV_LIST_MAX;
    }

    for(j=0;j<paired_num;j++)
    {
        if(0==bacmp(&stBTDevTmp.bdaddr, &pstBTPairedDevList[j].bdaddr))
        {
            printf("The bluetooth device %s has been paired.\n",pstBTDev->szDevAddr);
            goto finished;
        }
    }
    ret = pair_with_device(pstBTAdp->szName, &stBTDevTmp, szPassword);
    if(HI_SUCCESS != ret)
    {
        printf("pare bluetooth device failure, ret is %d.\n",ret);
        ret = HI_ERR_BLUETOOTH_PAIR_FAILURE;
        goto finished;
    }
    
finished:
    if(NULL != pstBTPairedDevList)
    {
        free(pstBTPairedDevList);
        pstBTPairedDevList = NULL;
    }

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}


/**
 \brief Get the paired bluetooth devices
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] u32MaxNum  The max elements number of list
 \param[out] pstBTDevList  The list of paired bluetooth devices
 \param[out] pu32Num  The number of paired bluetooth devices
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetPairedDevList(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDevList ,unsigned int u32MaxNum, unsigned int *pu32Num)
{
    int ret = HI_SUCCESS;
    int num;
    struct bt_device *pstBTDevListTmp = NULL;
    unsigned int i;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDevList);
    CHECK_NULL_PTR(pu32Num);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    if((u32MaxNum == 0)||(u32MaxNum > HI_BLUETOOTH_DEV_LIST_MAX))
    {
        printf("invalid para u32MaxNum=%d\n",u32MaxNum);
        return HI_ERR_BLUETOOTH_INVALID_PARA;
    }

    pstBTDevListTmp = malloc(u32MaxNum*sizeof(struct bt_device));
    if(NULL == pstBTDevListTmp)
    {
        printf("Malloc bluetooth device list failure.\n");
        return HI_ERR_BLUETOOTH_MALLOC_MEM_FAILURE;
    }
    
    BLUETOOTH_MUTEX_LOCK();
    
    num = get_paired_devices(pstBTAdp->szName, pstBTDevListTmp, u32MaxNum);
    if(num < 0)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get paired bluetooth device failure, ret is %d.\n",num);
        free(pstBTDevListTmp);
        return HI_ERR_BLUETOOTH_GET_PAIRED_DEV_FAILURE;
    }

    if(num > u32MaxNum)
    {
       num = u32MaxNum;
    }

    for(i=0;i<num;i++)
    {
        strcpy(pstBTDevList[i].szName,pstBTDevListTmp[i].dev_name);
        ba2str(&pstBTDevListTmp[i].bdaddr, pstBTDevList[i].szDevAddr);
    }
    *pu32Num = num;
    
    free(pstBTDevListTmp);

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}



/**
 \brief Remove the paired bluetooth device
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The paired bluetooth device which should be removed
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_RemovePairedDev(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev)
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;
        
    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_LOCK();

    str2ba(pstBTDev->szDevAddr, &stBTDevTmp.bdaddr);
    ret =  remove_paried_device(pstBTAdp->szName,&stBTDevTmp);
    if(HI_SUCCESS != ret)
    {
        printf("remove paired bluetooth device failure, ret is %d.\n",ret);
        ret = HI_ERR_BLUETOOTH_REMOVE_PAIRED_DEV_FAILURE;
    }
    pstBTDev->s32Paired = 0;

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}


/**
 \brief Get the bluetooth device status
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The bluetooth device
 \param[out] penStatus  The status of bluetooth device
 \retval ::0 SUCCESS
 \retval ::HI_ERR_BLUETOOTH_PTHREAD_BUSY
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_GetDevStatus(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, HI_BLUETOOTH_DEVICE_STATUS_E *penStatus )
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_NULL_PTR(penStatus);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_TRYLOCK();

    str2ba(pstBTDev->szDevAddr, &stBTDevTmp.bdaddr);
#if 0    
    ret = get_device_conn_status(pstBTAdp->szName, &stBTDevTmp, (unsigned int*)penStatus);
    if(HI_SUCCESS != ret)
    {
        printf("get bluetooth device status failure, ret is %d\n",ret);
        ret = HI_ERR_BLUETOOTH_GET_DEV_STATUS_FAILURE;
    }
#else    
    ret = get_headset_conn_status(pstBTAdp->szName, &stBTDevTmp, (unsigned int*)penStatus);
    if(HI_SUCCESS != ret)
    {
        printf("get bluetooth headset connect status failure, ret is %d\n",ret);
        ret = HI_ERR_BLUETOOTH_GET_DEV_STATUS_FAILURE;
    }
#endif

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}

/**
 \brief Connect the bluetooth device
 \attention \n
None
 \param[in] pstBTAdp  The pointer of bluetooth adapter
 \param[in] pstBTDev  The bluetooth device
 \param[in] bConnect  HI_TRUE--Connect HI_FALSE--Disconnect
 \retval ::0 SUCCESS
 \retval ::others FAILURE
 \see \n
None
 */
int HI_BLUETOOTH_Connect(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, char bConnect)
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_LOCK();

    str2ba(pstBTDev->szDevAddr, &stBTDevTmp.bdaddr);
    ret = connect_with_device(pstBTAdp->szName, &stBTDevTmp, (unsigned char)bConnect);
    if(HI_SUCCESS != ret)
    {
        printf("%s the bluetooth device failure, ret is %d\n",bConnect?"Connect":"Disconnect",ret);
        ret = HI_ERR_BLUETOOTH_CONNECT_DEV_FAILURE;
    }

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}

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

int HI_BLUETOOTH_GetConnectedHeadset(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev)
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);

    BLUETOOTH_MUTEX_TRYLOCK();

    ret = get_connected_headset(pstBTAdp->szName,&stBTDevTmp);
    if(HI_SUCCESS == ret)
    {
        strcpy(pstBTDev->szName,stBTDevTmp.dev_name);
        ba2str(&stBTDevTmp.bdaddr, pstBTDev->szDevAddr);
    }
    else
    {
        ret = HI_ERR_BLUETOOTH_NOT_CONNECTED;
    }
    BLUETOOTH_MUTEX_UNLOCK();

    return ret;
}

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

int HI_BLUETOOTH_Headset_Ring(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp, HI_BLUETOOTH_DEV_ATTR_S *pstBTDev, char bRing)
{
    int ret = HI_SUCCESS;
    struct bt_device stBTDevTmp;
    unsigned int u32Status=0;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_LOCK();

    str2ba(pstBTDev->szDevAddr, &stBTDevTmp.bdaddr);

    ret = get_headset_conn_status(pstBTAdp->szName, &stBTDevTmp, &u32Status);
    if(HI_SUCCESS != ret)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("get bluetooth headset connect status failure, ret is %d\n",ret);
        return HI_ERR_BLUETOOTH_GET_DEV_STATUS_FAILURE;
    }
    else if(HI_BLUETOOTH_DEV_STATUS_CONNECTED != u32Status)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("bluetooth headset not connected.\n");
        return HI_ERR_BLUETOOTH_NOT_CONNECTED;
    }

    ret = headset_ring(pstBTAdp->szName, &stBTDevTmp, (unsigned char)bRing);
    if(HI_SUCCESS != ret)
    {
        printf("Headset ring failure, ret is %d\n",ret);
        ret = HI_ERR_BLUETOOTH_HEADSET_RING_FAILURE;
    }

    BLUETOOTH_MUTEX_UNLOCK();
    return ret;
}


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

int HI_BLUETOOTH_SaveActiveDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp,HI_BLUETOOTH_DEV_ATTR_S *pstBTDev)
{
    FILE *fp = NULL;
//    char cEnvStr[64];

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);
    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_LOCK();
    
    fp = fopen(BLUETOOTH_ACTIVE_DEVICE_FILE, "w");
    if (NULL == fp)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("open file %s failure!\n",BLUETOOTH_ACTIVE_DEVICE_FILE);
        return HI_ERR_BLUETOOTH_OPEN_FILE_FAILURE;
    }

    fprintf(fp,"%s %s\n",pstBTAdp->szAdpAddr,pstBTAdp->szName); 
    fprintf(fp,"%s %s\n",pstBTDev->szDevAddr,pstBTDev->szName); 
    fclose(fp);

    BLUETOOTH_MUTEX_UNLOCK();
    return HI_SUCCESS;
}

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

int HI_BLUETOOTH_GetActiveDevice(HI_BLUETOOTH_ADP_ATTR_S *pstBTAdp,HI_BLUETOOTH_DEV_ATTR_S *pstBTDev)
{
    FILE *fp = NULL;

    CHECK_NULL_PTR(pstBTAdp);
    CHECK_NULL_PTR(pstBTDev);

    BLUETOOTH_MUTEX_TRYLOCK();
    
    fp = fopen(BLUETOOTH_ACTIVE_DEVICE_FILE, "r");
    if (NULL == fp)
    {
        BLUETOOTH_MUTEX_UNLOCK();
        printf("open file %s failure!\n",BLUETOOTH_ACTIVE_DEVICE_FILE);
        return HI_ERR_BLUETOOTH_OPEN_FILE_FAILURE;
    }
    
    fscanf(fp,"%s %s\n",pstBTAdp->szAdpAddr,pstBTAdp->szName); 
    fscanf(fp,"%s %s\n",pstBTDev->szDevAddr,pstBTDev->szName); 
    fclose(fp);

    CHECK_ADAPTER_NAME(pstBTAdp->szName);
    CHECK_BLUETOOTH_ADDRESS(pstBTDev->szDevAddr);

    BLUETOOTH_MUTEX_UNLOCK();
    return HI_SUCCESS;
}

#endif

