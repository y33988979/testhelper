/*****************************************************************************
*
* Description: 
*    驱动层TUNER模块实现文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-25    Qiujordan      创建
* 2. 2005-11-28    Qiujordan      取消对原有驱动的支持，现在只支持STTUNER驱动
* 3. 2013-09-15    yjzhou         添加TDA18250B Turner支持
*
*****************************************************************************/
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include <sttbx.h>
#include <hi_unf_ecs.h>
#include "ystuner.h"
#include "drv_basic.h"
#include "drv_os.h"
#include <stdio.h>

/*以前驱动内部来判断的，但是5197内部没有判断，
符号率过小会导致FE_5197_Search中FE_5197_Algo次数太多而无法跳出
*/
/* TUNER */
#define TUNER_USE    0
//#define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_MT2081
//#define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_TDA18250
//#define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_TDA18250
#define DEMOD_TYPE   HI_UNF_DEMOD_DEV_TYPE_3130I
#define I2C_CHANNEL  HI_UNF_I2C_CHANNEL_QAM

#define CAB5197_SYMBOLMIN 870000
#define CAB5197_SYMBOLMAX 11700000
DRV_TunerCallback mTunerCallback = NULL;
DRV_TunerCallback mTunerCallback_user = NULL;
unsigned char cQamSize ;
int 			iSymbolRate;
int 			iTransponderFreq;
BOOL g_TunerLock;
U32 D0297E_Signal_Agc = 0;
U32 D0297E_Signal_BerCnt = 0;
U32 D0297E_Signal_CNdB = 0;

HI_U32 TunerType = HI_UNF_TUNER_DEV_TYPE_TDA18250;

static DRVOS_Sem_t  *m_TunerSetParmProtel = NULL;

static void *drv_TunerCallback(BOOL_YS Locked)
{
    if(mTunerCallback_user != NULL)
        mTunerCallback_user(Locked);
    STTBX_Print(("Callback:Locked[%d]!\n",Locked));
}
#define NUMBER_LOST_BEFORE_UNLOCK     12
#define NUMBER_LOST_BEFORE_LOCK       1
static U8 LastLockStatus;
static U8 NowLockStatus;
static U32 LockStatusCount;
static U32 UnLockStatusCount;

/*******************************************************************************
* Function:     mAUD_SetVolume
* Author:       wrwei 2012.0703
* Description:  自适应选择tuner
* Input:        void
* Output:   
* Return:  
                DRV_NO_ERROR：成功
                -1：失败
* Others:          
*******************************************************************************/
DRV_ErrCode drv_TunerTypeSelect(void)
#if 0
{
    DRV_ErrCode                  errCode;
    HI_UNF_TUNER_ATTR_S          TunerAttr;
    HI_UNF_TUNER_CONNECT_PARA_S  ConnectPara;
	HI_U32 u32I2cNum;
	HI_U8 u8DevAddress;
	HI_U32 u32RegAddr;
	HI_U32 u32RegAddrCount;
	HI_U8 *pu8Buf;
	HI_U32 u32Length;		
	HI_U8 *reg_value;
	

    /* 获取默认属性,以便接下来设置属性 */
    errCode = HI_UNF_TUNER_GetDeftAttr(TUNER_USE, &TunerAttr);
    if(DRV_NO_ERROR != errCode)
    {
        printf("call HI_UNF_TUNER_GetDeftAttr failed.errCode 0x%08x\n",errCode);
        HI_UNF_TUNER_Close(TUNER_USE);
        HI_UNF_TUNER_DeInit();
        return errCode;
    }

	/* 通过I2C读取Turner数据，自适应选择是R820C、TDA18250、TDA18250B tuner */
	u32I2cNum = I2C_CHANNEL;
	u32RegAddrCount = 1;
	pu8Buf = malloc(3);
	reg_value = malloc(1);
	u32Length = 1;	
	
	errCode = HI_UNF_I2C_Open();
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Open() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	*reg_value = 0;
	u8DevAddress = 0xA1;
	u32RegAddr = 0x46;
	errCode = HI_UNF_I2C_Read( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,reg_value,u32Length);
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Read() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	*reg_value = *reg_value|0x80;
 
	u8DevAddress = 0xA0;
	u32RegAddr = 0x46;
	errCode = HI_UNF_I2C_Write( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,reg_value,u32Length);
	if(HI_SUCCESS != errCode)
	{
    	printf("%s: %d HI_UNF_I2C_Write() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	*pu8Buf = 0;
	u8DevAddress = 0xC1;
	u32RegAddr = 0;
	errCode = HI_UNF_I2C_Read( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,pu8Buf,u32Length);
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Read() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}

	u8DevAddress = 0xA0;
	u32RegAddr = 0x46;
	errCode = HI_UNF_I2C_Write( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,reg_value,u32Length);
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Write() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	*(pu8Buf + 1) = 0;
	u8DevAddress = 0xC1;
	u32RegAddr = 1;
	errCode = HI_UNF_I2C_Read( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,(pu8Buf+1),u32Length);
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Read() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	u8DevAddress = 0xA0;
	u32RegAddr = 0x46;
	errCode = HI_UNF_I2C_Write( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,reg_value,u32Length);
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Write() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	*(pu8Buf + 2) = 0;
	u8DevAddress = 0xC1;
	u32RegAddr = 2;
	errCode = HI_UNF_I2C_Read( u32I2cNum,u8DevAddress,u32RegAddr,u32RegAddrCount,(pu8Buf+2),u32Length);
	if(HI_SUCCESS != errCode)
	{
    	printf("%s: %d HI_UNF_I2C_Read() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}	
/*	
	errCode=*pu8Buf;
	printf("@@errCode=0x%X\n",errCode);
	
	errCode=(((HI_U32)*pu8Buf&0x7f)<<8 |(HI_U32)*(pu8Buf+1));
	printf("@@errCode=%d\n",errCode);		
*/	
	errCode = HI_UNF_I2C_Close();
	if(HI_SUCCESS != errCode)
	{
        printf("%s: %d HI_UNF_I2C_Close() ErrorCode = 0x%08x\n", __FILE__, __LINE__, errCode);
		return errCode;
	}
	
	if(*pu8Buf == 0xD5)
	{
		TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250;
		TunerAttr.enDemodDevType = DEMOD_TYPE;
		TunerAttr.enI2cChannel   = I2C_CHANNEL;
		errCode = HI_UNF_TUNER_SetAttr(TUNER_USE, &TunerAttr);
		if(HI_SUCCESS != errCode)
		{
	        printf("%s: %d HI_UNF_TUNER_SetAttr() ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
			return errCode;
		}
		
		ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
		ConnectPara.unConnectPara.stCab.bReverse = 0;
		ConnectPara.unConnectPara.stCab.u32Freq = 323000;
		ConnectPara.unConnectPara.stCab.u32SymbolRate = 6875 * 1000;
		ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
		errCode = HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 500);
		if((HI_SUCCESS == errCode) || (HI_ERR_TUNER_FAILED_CONNECT == errCode))
		{
		    printf("drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_TDA18250 succes.\n");
		    return HI_SUCCESS;
		}  
	}
	else if((((HI_U32)*pu8Buf&0x7f)<<8 |(HI_U32)*(pu8Buf+1))==18250)
	{
		TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250B;
		TunerAttr.enDemodDevType = DEMOD_TYPE;
		TunerAttr.enI2cChannel   = I2C_CHANNEL;
		errCode = HI_UNF_TUNER_SetAttr(TUNER_USE, &TunerAttr);
		if (HI_SUCCESS != errCode)
		{
		    printf("%s: %d HI_UNF_TUNER_SetAttr() ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
			return errCode;
		}
		
		ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
		ConnectPara.unConnectPara.stCab.bReverse = 0;
		ConnectPara.unConnectPara.stCab.u32Freq = 323000;
		ConnectPara.unConnectPara.stCab.u32SymbolRate = 6875 * 1000;
		ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
		errCode = HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 2000);
		if((HI_SUCCESS == errCode) || (HI_ERR_TUNER_FAILED_CONNECT == errCode))
		{
			printf("drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_TDA18250B succes.\n");
			return HI_SUCCESS;
		} 
	}
	else
	{
		TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_R820C;
		TunerAttr.enDemodDevType = DEMOD_TYPE;
		TunerAttr.enI2cChannel   = I2C_CHANNEL;
		errCode = HI_UNF_TUNER_SetAttr(TUNER_USE, &TunerAttr);
		if (HI_SUCCESS != errCode)
		{
		    printf("%s: %d HI_UNF_TUNER_SetAttr() ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
			return errCode;
		}
		
		ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
		ConnectPara.unConnectPara.stCab.bReverse = 0;
		ConnectPara.unConnectPara.stCab.u32Freq = 323000;
		ConnectPara.unConnectPara.stCab.u32SymbolRate = 6875 * 1000;
		ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
		errCode = HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 500);
		if((HI_SUCCESS == errCode) || (HI_ERR_TUNER_FAILED_CONNECT == errCode))
		{
			printf("drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_R820C succes.\n");
			return HI_SUCCESS;
		} 
	}
	
    printf("drv_TunerTypeSelect failed!!!\n");
    return -1;
}
#else
{
    DRV_ErrCode                  errCode;
    HI_UNF_TUNER_ATTR_S          TunerAttr;
    HI_UNF_TUNER_CONNECT_PARA_S  ConnectPara;

    printf("drv_TunerTypeSelect in!!!\n");

    /* 获取默认属性,以便接下来设置属性 */
    errCode = HI_UNF_TUNER_GetDeftAttr(TUNER_USE, &TunerAttr);
    if (HI_SUCCESS != errCode)
    {
        printf("call HI_UNF_TUNER_GetDeftAttr failed.errCode 0x%08x\n",errCode);
        HI_UNF_TUNER_Close(TUNER_USE);
        HI_UNF_TUNER_DeInit();
        return errCode;
    }

    //自适应选择是R820C tuner还是TDA18250 tuner
    {
    #if (HWVER == 0x693)
 	 TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250B;
    #else
        TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_TDA18250;
    #endif
        TunerAttr.enDemodDevType = DEMOD_TYPE;
        TunerAttr.enI2cChannel   = I2C_CHANNEL;
        errCode = HI_UNF_TUNER_SetAttr(TUNER_USE, &TunerAttr);
        if (HI_SUCCESS == errCode)
        {
#if 0       /*lrlong-优化开机时间去掉*/
            ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
            ConnectPara.unConnectPara.stCab.bReverse = 0;
            ConnectPara.unConnectPara.stCab.u32Freq = 323000;
            ConnectPara.unConnectPara.stCab.u32SymbolRate = 6875 * 1000;
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            errCode = HI_UNF_TUNER_Connect(HISI_TUNER_ID, &ConnectPara, 0);
            if ((HI_SUCCESS == errCode) || (HI_ERR_TUNER_FAILED_CONNECT == errCode))
            {
                HISI_INFO(MODULE_PRINT_TUNER, "drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_TDA18250 succes.");
                return HI_SUCCESS;
            }
#else
			printf("drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_TDA18250 succes!!!\n");
            return HI_SUCCESS;
#endif
        }
        else
        {
        	printf("drv_TunerTypeSelect not TDA18250!\n");
        }

        TunerAttr.enTunerDevType = HI_UNF_TUNER_DEV_TYPE_R820C;
        TunerAttr.enDemodDevType = DEMOD_TYPE;
        TunerAttr.enI2cChannel   = I2C_CHANNEL;
        errCode = HI_UNF_TUNER_SetAttr(TUNER_USE, &TunerAttr);
        if (HI_SUCCESS == errCode)
        {
#if 0       /*lrlong-优化开机时间去掉*/
            ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
            ConnectPara.unConnectPara.stCab.bReverse = 0;
            ConnectPara.unConnectPara.stCab.u32Freq = 323000;
            ConnectPara.unConnectPara.stCab.u32SymbolRate = 6875 * 1000;
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            errCode = HI_UNF_TUNER_Connect(HISI_TUNER_ID, &ConnectPara, 0);
            if ((HI_SUCCESS == errCode) || (HI_ERR_TUNER_FAILED_CONNECT == errCode))
            {
                HISI_INFO(MODULE_PRINT_TUNER, "drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_R820C succes.");
                return HI_SUCCESS;
            }
#else
			printf("drv_TunerTypeSelect HI_UNF_TUNER_DEV_TYPE_R820C succes!!!\n");
            return HI_SUCCESS;
#endif
        }
    }

    printf("drv_TunerTypeSelect failed!!!\n");
    return -1;
}
#endif

static void drv_TunerTask(void *pvParam)
{
    DRV_ErrCode errCode;
    static HI_UNF_TUNER_STATUS_S  pstTunerStatus;
	
	while(1)
	{       

		DRV_OS_TaskDelay_MS(50);
		errCode = HI_UNF_TUNER_GetStatus(TUNER_USE,&pstTunerStatus); 
	    if(DRV_NO_ERROR != errCode)
	    {
	        printf("drv_TunerTask HI_UNF_TUNER_GetStatus failed.errCode 0x%08x\n",errCode);
	        continue;
	    }
	    NowLockStatus = pstTunerStatus.enLockStatus;
	    if((NowLockStatus != HI_UNF_TUNER_SIGNAL_DROPPED)&&(NowLockStatus != HI_UNF_TUNER_SIGNAL_LOCKED))
		{
	        printf("drv_TunerTask bad param failed.NowLockStatus 0x%08x\n",NowLockStatus);
	        continue;
		}
		if(NowLockStatus != LastLockStatus)//锁定状态有变化
		{
			UnLockStatusCount = 0;
			LockStatusCount = 0;
		}
		else
		{
			if(NowLockStatus == HI_UNF_TUNER_SIGNAL_LOCKED)
			{
				UnLockStatusCount = 0;
				if(LockStatusCount < 0xFFFF)
					LockStatusCount ++;
			}
			else
			{
				if(UnLockStatusCount < 0xFFFF)
					UnLockStatusCount ++;
				LockStatusCount = 0;
			}
		}
		LastLockStatus = NowLockStatus;
		if(LockStatusCount == NUMBER_LOST_BEFORE_LOCK)
		{
			drv_TunerCallback(TRUE);
		}	
		if(UnLockStatusCount == NUMBER_LOST_BEFORE_UNLOCK)
		{
			drv_TunerCallback(FALSE);
		}	
	}
}

DRV_ErrCode DVBTunerInit( void)
{
    DRV_ErrCode errCode;
    U32 index;
    DRVOS_Task_t *pTunerTaskid;

    m_TunerSetParmProtel = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);

    /* 初始化Tuner*/
    errCode = HI_UNF_TUNER_Init();
    if(DRV_NO_ERROR != errCode)
    {
        printf("call HI_UNF_TUNER_Init failed.errCode 0x%08x\n",errCode);
        return errCode;
    }

    /* 打开Tuner*/
    errCode = HI_UNF_TUNER_Open(TUNER_USE);
    if(DRV_NO_ERROR != errCode)
    {
        printf("call HI_UNF_TUNER_Open failed.errCode 0x%08x\n",errCode);
        HI_UNF_TUNER_DeInit();
        return errCode;
    }

    errCode = drv_TunerTypeSelect();
    if(DRV_NO_ERROR != errCode)
    {
        printf("call drv_TunerTypeSelect failed.errCode 0x%08x\n",errCode);
        HI_UNF_TUNER_Close(TUNER_USE);
        HI_UNF_TUNER_DeInit();
        return errCode;
    }
    
    /*create a thread for led display*/
	pTunerTaskid = DRV_OS_TaskCreate("drv_TunerTask",drv_TunerTask,3,8096,NULL,0);
    if (pTunerTaskid == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    mTunerCallback = (DRV_TunerCallback)drv_TunerCallback;
    return DRV_NO_ERROR;
}

U32 drv_TunerLockCallBack(BOOL IsLock)
{
   if(mTunerCallback != NULL)
       mTunerCallback(IsLock);			
   return 0;    
}
DRV_ErrCode DRV_TunerEnableCallback(DRV_TunerCallback Callback_f)
{
    mTunerCallback_user = Callback_f;
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_TunerDisableCallback(void)
{
    mTunerCallback_user = NULL;
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_SetNewTS( U32_YS Fre, U32_YS Sym, U8_YS Qam )
{
    DRV_ErrCode errCode;
    HI_UNF_TUNER_CONNECT_PARA_S  ConnectPara;
	if((g_TunerLock==TRUE) &&	   
	(cQamSize == Qam) &&
	(iSymbolRate == Sym) &&
	(iTransponderFreq == Fre ))
	{
        return DRV_NO_ERROR;
	}
        /* 停止收标准表 */
    DVB_StopStandardTableReceiveByTuner();
  
	DRV_OS_SemWait(m_TunerSetParmProtel,DRVOS_WAIT_INFINITY,0);  
    /*内部不检查了，所以这里必须检查*/	
    if((Sym*1000<CAB5197_SYMBOLMIN)||(Sym*1000>CAB5197_SYMBOLMAX))
    {
        STTBX_Print(("Reques Sym[%d] is over the range!\n",Sym));    
	 	DRV_OSSemSignal(m_TunerSetParmProtel);    
    	return DRV_ERROR_BAD_PARAMETER;
    }
    
	cQamSize = Qam; 	 
	iSymbolRate = Sym ;     
	iTransponderFreq = Fre;   //kHz
	
    DRV_Lock_Display(0);
    g_TunerLock = FALSE;
    STTBX_Print(("Reques Freq[%d],Sym[%d],Qam[%d]!\n",Fre,Sym,Qam));
    ConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
    ConnectPara.unConnectPara.stCab.bReverse = 0;
    ConnectPara.unConnectPara.stCab.u32Freq = Fre;
    ConnectPara.unConnectPara.stCab.u32SymbolRate = Sym * 1000;
    switch (Qam)
    {
        case 1 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 2 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_32;
            break;
        case 3 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        case 4 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_128;
            break;
        case 5 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
        case 6 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_512;
            break;            
        default:
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64; 
	}
    
    /*SetNewTS函数只做设置工作，不判断状态，以提高切台时间 wrwei 20130228*/
    //errCode = HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 500);
    errCode = HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 0);
    if(errCode == HI_SUCCESS)
    {
        LastLockStatus = 0;
        NowLockStatus = 0;
        LockStatusCount = 0;
        UnLockStatusCount = 0;
    }
    else  if(errCode == HI_ERR_TUNER_FAILED_CONNECT)
    {
        LastLockStatus = 0;
        NowLockStatus = 0;
        LockStatusCount = 0;
        UnLockStatusCount = 0;
        STTBX_Print(("------DRV_SetNewTS Cable not Connect\n")); 
    }
    else if (HI_SUCCESS != errCode)
    {
        STTBX_Print(("HI_UNF_TUNER_Connect errCode 0x%08x\n",errCode)); 
        DRV_OSSemSignal(m_TunerSetParmProtel);    
        return DRV_ERROR_BAD_PARAMETER;
    }

    DRV_OSSemSignal(m_TunerSetParmProtel);    
    return DRV_NO_ERROR;
}
    
DRV_ErrCode GetSignalQuality( U32_YS * pCN_Ratio,U32_YS  * pBitErrRate, U32_YS* pCN_dB100)
{
    DRV_ErrCode errCode = HI_SUCCESS;
    U32_YS Signal_BerCnt[3] = {0};

    if(pCN_Ratio == NULL || pBitErrRate == NULL || pCN_dB100 == NULL )
        return DRV_ERROR_BAD_PARAMETER;


    if(g_TunerLock)
    {
		errCode = HI_UNF_TUNER_GetSignalStrength(TUNER_USE,&D0297E_Signal_Agc);
		if(errCode == HI_FAILURE)
		{
            D0297E_Signal_Agc = 20;
		}
		errCode = HI_UNF_TUNER_GetBER(TUNER_USE,Signal_BerCnt);
		if(errCode == HI_FAILURE)
		{
            D0297E_Signal_BerCnt = 5000;
		}
		else
		{
            if(Signal_BerCnt[2] > 4)
            {
                D0297E_Signal_BerCnt = 0;
            }
            else if(Signal_BerCnt[2] = 4)
            {
                D0297E_Signal_BerCnt = Signal_BerCnt[0];
            }
            else if(Signal_BerCnt[2] = 3)
            {
                D0297E_Signal_BerCnt = Signal_BerCnt[0]*10 + Signal_BerCnt[1]/100;
            }
            else if(Signal_BerCnt[2] = 2)
            {
                D0297E_Signal_BerCnt = Signal_BerCnt[0]*100 + Signal_BerCnt[1]/10;
            }
            else if(Signal_BerCnt[2] = 1)
            {
                D0297E_Signal_BerCnt = Signal_BerCnt[0]*1000 + Signal_BerCnt[1];
            }
            else
            {
                D0297E_Signal_BerCnt = 1;
            }   
		}
		errCode = HI_UNF_TUNER_GetSNR(TUNER_USE,&D0297E_Signal_CNdB);
		if(errCode == HI_FAILURE)
		{
		    D0297E_Signal_CNdB = 22;
		}
		
		*pBitErrRate = D0297E_Signal_BerCnt;
		*pCN_dB100 = D0297E_Signal_Agc;
		*pCN_Ratio = D0297E_Signal_CNdB;
		
    }
    else
    {
        *pBitErrRate = 1;
        *pCN_dB100 = 0;
        *pCN_Ratio = 0;        
    }
    //STTBX_Print(("pBitErrRate[%d]---pCN_Ratio[%d]---pCN_dB100[%d]\n",*pBitErrRate,*pCN_Ratio,*pCN_dB100));

    return DRV_NO_ERROR;
}

BOOL_YS DRV_TunerGetLockStatus(void)
{
    DRV_ErrCode errCode;
    HI_UNF_TUNER_STATUS_S  pstTunerStatus;
    errCode = HI_UNF_TUNER_GetStatus(TUNER_USE,&pstTunerStatus); 
    if(pstTunerStatus.enLockStatus == HI_UNF_TUNER_SIGNAL_LOCKED)
    {
		STTBX_Print(("Frequency (Khz) : %d\n"    , pstTunerStatus.stConnectPara.unConnectPara.stCab.u32Freq));
		STTBX_Print(("Symbol Rate     : %d\n"    , pstTunerStatus.stConnectPara.unConnectPara.stCab.u32SymbolRate));
		STTBX_Print(("Modulation      : %d\n"    , pstTunerStatus.stConnectPara.unConnectPara.stCab.enModType));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void DRV_CurrentTuner(U32_YS  *pFre, U32_YS *pSym,  U8_YS *pQam)
{
	*pFre =  iTransponderFreq;
	*pSym = iSymbolRate;
	*pQam = cQamSize;
}
void SignalDiagnoseStop()
{}
void SignalDiagnose()
{}

