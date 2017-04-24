/*****************************************************************************
*
* Description: 
*    ï¿½ï¿½ï¿½ï¿½Ê¼ï¿½ï¿½Ä£ï¿½ï¿½ï¿½ï¿½Õ±ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½Ä¼ï¿½
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-03    Qiujordan      ï¿½ï¿½ï¿½ï¿½
*
*****************************************************************************/

#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#ifndef STTBX_REPORT
#define STTBX_REPORT
#endif
#include <sttbx.h>
#include "drv_basic.h"
//#include "drv_dmx.h"
#include "ysav.h"
/* Private Types ------------------------------------------------------ */

#if defined BOARD_TYPE_fpga || defined ATBM886
#define DEFAULT_DVB_PORT 1
#else
#define DEFAULT_DVB_PORT 0
#endif

/* Private Constants -------------------------------------------------- */

/* Private Variables -------------------------------------------------- */


/* Private Macros ----------------------------------------------------- */


/* Private Function prototypes ---------------------------------------- */

/* Global Variables -------------------------------------------------------- */



/* Functions --------------------------------------------------------------- */

/* Semaphore to ensure correct multi thread                                   */
static BOOL        pti_SemInit = FALSE;
static DRVOS_Sem_t *pti_SemLock;

DRV_ErrCode DRV_PtiInit(void)
#ifndef ATBM886
{
    DRV_ErrCode ErrCode;
    ErrCode = HI_UNF_DMX_Init();
    ErrCode |= HI_UNF_DMX_AttachTSPort(0,DEFAULT_DVB_PORT);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("DRV_PtiInit HIADP_Demux_Init failed.ErrCode 0x%08x!\n",ErrCode);
        return ErrCode;
    }
    //drv_InitSlotSetup();
    return (DRV_NO_ERROR);
}
#else
{
    DRV_ErrCode ErrCode;
	HI_UNF_DMX_PORT_ATTR_S pstAttr;
	HI_U32  au32GpioRegValue;
	
    ErrCode = HI_UNF_DMX_Init();
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("[DRV]DRV_PtiInit HI_UNF_DMX_Init failed.ErrCode 0x%08x!\n",ErrCode);
        return ErrCode;
    }
	ErrCode = HI_UNF_DMX_GetTSPortAttr(DEFAULT_DVB_PORT,&pstAttr);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("[DRV]DRV_PtiInit HI_UNF_DMX_SetTSPortAttr failed.ErrCode 0x%08x!\n",ErrCode);
        return ErrCode;
    }	
	//pstAttr.enPortMod = HI_UNF_DMX_PORT_MODE_EXTERNAL;
	pstAttr.enPortType = HI_UNF_DMX_PORT_TYPE_SERIAL;
	pstAttr.u32TunerInClk = 1;
	pstAttr.u32SerialBitSelector = 1; //±ØÐëÉèÖÃ£¬Ä¬ÈÏÎªcdata¡¾7¡¿Êä³ö
	pstAttr.u32TunerErrMod = 0;
	ErrCode = HI_UNF_DMX_SetTSPortAttr(DEFAULT_DVB_PORT,&pstAttr);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("[DRV]DRV_PtiInit HI_UNF_DMX_SetTSPortAttr failed.ErrCode 0x%08x!\n",ErrCode);
        return ErrCode;
    } 

	ErrCode = HI_UNF_DMX_AttachTSPort(0,DEFAULT_DVB_PORT);
    if (ErrCode != DRV_NO_ERROR)
    {
        printf("[DRV]DRV_PtiInit HI_UNF_DMX_AttachTSPort failed.ErrCode 0x%08x!\n",ErrCode);
        return ErrCode;
    }	
    return (DRV_NO_ERROR);
}
#endif
DRV_ErrCode drv_PtiProtect(void)
{
    if (pti_SemInit == FALSE)
    {
         pti_SemLock = DRV_OS_SemCreate(NULL,0,DRVOS_INIT_FIFO_WAIT);
        pti_SemInit = TRUE;
    }
    else
    {
	DRV_OS_SemWait(pti_SemLock,DRVOS_WAIT_INFINITY,0);
    }

    return ST_NO_ERROR;
}

DRV_ErrCode drv_PtiUnProtect(void)
{
    DRV_OSSemSignal(pti_SemLock);
    return ST_NO_ERROR;
}

DRV_ErrCode DRV_PtiGetPacketCount(U16 *PacketCount)
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
    return ErrCode;
}

DRV_ErrCode drv_PtiAudioStart( PTI_HandleId_t HandleId, STPTI_Pid_t AudioPid) 
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
	if(AudioPid == STPTI_InvalidPid())
	{
		STTBX_Print(("drv_PtiAudioStart Invalid VidPid %d\n",AudioPid));
		return ST_ERROR_BAD_PARAMETER;
	}
	drv_PtiProtect();
	m_curAudPid = AudioPid;
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&m_curAudPid);
    if(ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiAudioStart : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
    	drv_PtiUnProtect();
        return ErrCode;
    }
    drv_PtiUnProtect();
    return ErrCode;
}
/* -----------------------------------------------------------------------------
 * Name        : drv_PtiAudioStop
 * Description : Stop PTI transferring Audio
 * Parameters  :
 *     PTI_DeviceId_t    DeviceId    "Target"
 *     PTI_HandleId_t    HandleId    For multi-instance management
 * Assumptions :
 * Limitations :
 * Returns     :
 *     ST_NO_ERROR                     Success.
 * -------------------------------------------------------------------------- */
DRV_ErrCode drv_PtiAudioStop(PTI_HandleId_t HandleId)
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
    drv_PtiProtect();
 	m_curAudPid = STPTI_NullPid();
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&m_curAudPid);
    if ( ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiAudioStop : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
     	drv_PtiUnProtect();
		return ErrCode;
    }
    drv_PtiUnProtect();
	return ErrCode;
}

DRV_ErrCode drv_PtiVideoStart(PTI_HandleId_t HandleId, STPTI_Pid_t VideoPid)
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
	if(VideoPid == STPTI_InvalidPid())
	{
		STTBX_Print(("drv_PtiVideoStart Invalid VidPid %d\n",VideoPid));
		return ST_ERROR_BAD_PARAMETER;
	}
	drv_PtiProtect();
	m_curVidPid = VideoPid;
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&m_curVidPid);
    if(ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiVideoStart : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
    	drv_PtiUnProtect();
        return ErrCode;
    }
    drv_PtiUnProtect();
    return ErrCode;
}

DRV_ErrCode drv_PtiVideoStop(PTI_HandleId_t HandleId) 
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
	drv_PtiProtect();
	m_curVidPid = STPTI_NullPid();
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&m_curVidPid);
    if(ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiVideoStop : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
    	drv_PtiUnProtect();
        return ErrCode;
    }
    drv_PtiUnProtect();
    return ErrCode;
}

DRV_ErrCode drv_PtiPcrStart( PTI_HandleId_t HandleId ,STPTI_Pid_t PCRPid )
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
	if(PCRPid == STPTI_InvalidPid())
	{
		STTBX_Print(("drv_PtiPcrStart Invalid VidPid %d\n",PCRPid));
		return ST_ERROR_BAD_PARAMETER;
	}
	drv_PtiProtect();
	m_curPcrPid = PCRPid;
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_PCR_PID,&m_curPcrPid);
    if(ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiPcrStart : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
    	drv_PtiUnProtect();
        return ErrCode;
    }
    drv_PtiUnProtect();
    return ErrCode;
}

DRV_ErrCode drv_PtiPcrStop( PTI_HandleId_t HandleId) 
{
    DRV_ErrCode      ErrCode = ST_NO_ERROR;
	drv_PtiProtect();
	m_curPcrPid = STPTI_NullPid();
	ErrCode = HI_UNF_AVPLAY_SetAttr(mAvPlayHandle, HI_UNF_AVPLAY_ATTR_ID_PCR_PID,&m_curPcrPid);
    if(ErrCode != ST_NO_ERROR)
    {
        STTBX_Print(("drv_PtiPcrStop : HI_UNF_AVPLAY_SetAttr ErrCode 0x%08x\n",ErrCode));
    	drv_PtiUnProtect();
        return ErrCode;
    }
    drv_PtiUnProtect();
    return ErrCode;
}

DRV_ErrCode pti_allocate_dynamic_key(U32 DmxID, U32 *key)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	
	ErrCode = HI_UNF_DMX_CreateDescrambler(DmxID,key);
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_allocate_dynamic_key Failed,ErrCode is 0x%08x\n",ErrCode));
	}
	return ErrCode;
}

DRV_ErrCode pti_deallocate_dynamic_key(U32 key)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
	
	ErrCode = HI_UNF_DMX_DestroyDescrambler(key);
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_deallocate_dynamic_key Failed,ErrCode is 0x%08x\n",ErrCode));
	}
	return ErrCode;
}

DRV_ErrCode pti_associate_descramble_key(U32 DmxID,U32 slot_or_pid,U32 key)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_U32 u32Pid = 0;//¶ÔÓÚhisi£¬Ä¬ÈÏ´«ÈëµÄÊÇPID
    HI_HANDLE hChannel = 0;

    u32Pid = slot_or_pid;
	ErrCode = HI_UNF_DMX_GetChannelHandle(DmxID,u32Pid,&hChannel); 
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_associate_descramble_key Failed Because GetChannelHandle Failed,ErrCode is 0x%08x\n",ErrCode));
		return ErrCode;
	}
	
	ErrCode = HI_UNF_DMX_AttachDescrambler(key,hChannel);
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_associate_descramble_key Failed Because Attach Descrambler Failed,ErrCode is 0x%08x\n",ErrCode));
	}
	return ErrCode;
}

DRV_ErrCode pti_disassociate_descramble_key(U32 DmxID,U32 slot_or_pid,U32 key)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_U32 u32Pid = 0;//¶ÔÓÚhisi£¬Ä¬ÈÏ´«ÈëµÄÊÇPID
    HI_HANDLE hChannel = 0;
	
    u32Pid = slot_or_pid;
	ErrCode = HI_UNF_DMX_GetChannelHandle(DmxID,u32Pid,&hChannel); 
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_disassociate_descramble_key Failed Because GetChannelHandle Failed,ErrCode is 0x%08x\n",ErrCode));
		return ErrCode;
	}
	ErrCode = HI_UNF_DMX_DetachDescrambler(key, hChannel);
	if(ErrCode != DRV_NO_ERROR)
	{
		STTBX_Print(("pti_disassociate_descramble_key Failed Because Attach Descrambler Failed,ErrCode is 0x%08x\n",ErrCode));
	}
	return ErrCode;
}

DRV_ErrCode pti_set_descramble_key(U32 key,STPTI_KeyParity_t parity,STPTI_KeyUsage_t validity,U8 *key_data)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;

    if(parity == STPTI_KEY_PARITY_EVEN_PARITY)
    {
 		ErrCode = HI_UNF_DMX_SetDescramblerEvenKey(key,key_data);
		if(ErrCode != DRV_NO_ERROR)
		{
			STTBX_Print(("pti_set_descramble_key HI_UNF_DMX_SetDescramblerEvenKey,ErrCode is 0x%08x\n",__func__,ErrCode));
			return ErrCode;
		}
	}
    else if(parity == STPTI_KEY_PARITY_ODD_PARITY)
    {
		ErrCode = HI_UNF_DMX_SetDescramblerOddKey(key,key_data);
		if(ErrCode != DRV_NO_ERROR)
		{
			STTBX_Print(("pti_set_descramble_key HI_UNF_DMX_SetDescramblerOddKey,ErrCode is 0x%08x\n",__func__,ErrCode));
			return ErrCode;
		}
    }
    else
    {
		STTBX_Print(("pti_set_descramble_key bad param,parity is %d\n",parity));
    }

 	return ErrCode;
}

/* EOF --------------------------------------------------------------------- */


