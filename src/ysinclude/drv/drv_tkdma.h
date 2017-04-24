/*****************************************************************************
*
* Description: 
*    ����оƬ����ģ��ͷ�ļ�
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-17    Qiujordan      ����
* 2. 2008-08-07    Lqjun            Modify.
*
*****************************************************************************/
#ifndef _DRV_TKDMA_H_
#define _DRV_TKDMA_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "drv_typedef.h"


  typedef enum DRV_TKDMA_DESChannel_e
  {
    DRVTKDMA_CHANNEL_A,
    DRVTKDMA_CHANNEL_B,
    DRVTKDMA_CHANNEL_C
  } DRV_TKDMA_DESChannel_t;

typedef enum DRV_TKDMA_TKConfigCommand_e
{
    DRVTKDMA_TKCFG_AES_NOT_TDES_FOR_CW = 0,
    DRVTKDMA_TKCFG_SECURE_CW_OVERRIDE = 1,
    DRVTKDMA_TKCFG_OTP_SCK_OVERRIDE = 2,
    DRVTKDMA_TKCFG_SCK_FOR_CW_OVERRIDE = 3,
    DRVTKDMA_TKCFG_SCK_ALT_FOR_CPCW_OVERRIDE = 4,
    DRVTKDMA_TKCFG_ALT_FORMAT = 5,
    DRVTKDMA_TKCFG_DMA_ALT_FORMAT = 6,
    DRVTKDMA_TKCFG_LEGACY_KEYS = 7,
    DRVTKDMA_TKCFG_DMA_FIPS_FORMAT = 8
} DRV_TKDMA_TKConfigCommand_t;

typedef enum DRV_TKDMA_Command_e
{
    DRVTKDMA_COMMAND_DECRYPT_FK = 0,
    DRVTKDMA_COMMAND_DECRYPT_CK = 1,
    DRVTKDMA_COMMAND_DECRYPT_CW = 2,
    DRVTKDMA_COMMAND_DECRYPT_CPCW = 3,
    DRVTKDMA_COMMAND_DECRYPT_HK = 10,
    DRVTKDMA_COMMAND_DECRYPT_PFA = 11,
    DRVTKDMA_COMMAND_DECRYPT_SFA = 12,
    DRVTKDMA_COMMAND_DECRYPT_KEY = 13,
    DRVTKDMA_COMMAND_DECRYPT_SW = 14,
    DRVTKDMA_COMMAND_DIRECT_CPCW = 20,
    DRVTKDMA_COMMAND_DIRECT_CW = 21
} DRV_TKDMA_Command_t;
  
typedef struct DRV_TKDMA_Key_s
{
    DRV_U8 key_data[16];
} DRV_TKDMA_Key_t;


/*****************************************************************************
* Function    : DRV_TKDMAGetRevision
* Description :
*                   ��ȡ�汾��Ϣ
* Note        :
******************************************************************************/
DRV_U8* DRV_TKDMAGetRevision(void);

/*****************************************************************************
* Function    : DRV_TKDMAInit
* Description :
*                   ��ʼ��
* Note        :
******************************************************************************/
DRV_ErrCode DRV_TKDMAInit(void);

/*****************************************************************************
* Function    : DRV_TKDMAReset
* Description :
*                   ��λTKDMA�����CK��FKֵ
* Note        : 
****************************************************************************/
DRV_ErrCode DRV_TKDMAReset (void);

/*****************************************************************************
* Function    : DRV_TKDMATerm
* Description :
*                   �رո�ģ�飬�ͷ�������Դ
* Note        :
******************************************************************************/
DRV_ErrCode DRV_TKDMATerm(void);

/*****************************************************************************
* Function    : DRV_TKDMAReadPublicID
* Description :
*                   ��ȡCPU ID��оƬΨһ���
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasTkdmaReadPublicID(DRV_TKDMA_Key_t  *key);

/*****************************************************************************
* Function    : DRV_TKDMAConfigureTK
* Description :
*                   ����TK ����
* Note        :
 ****************************************************************************/
DRV_ErrCode DRV_TKDMAConfigureTK (DRV_TKDMA_TKConfigCommand_t Command, DRV_BOOL on);


/****************************************************************************
* Function    : DRV_TKDMADecryptKey
* Description :
*                   ת������CW��оƬ�ڲ�������׼�����õ���������CW
* Note        :
 ***************************************************************************/
DRV_ErrCode DRV_TKDMADecryptKey (DRV_TKDMA_Command_t Command,
                                                DRV_TKDMA_Key_t * Key,
                                                DRV_U8 Index, 
                                                DRV_TKDMA_Key_t *Offset);

/****************************************************************************
* Function    : STTKDMA_StoreDESKey
* Description :
*                   ������Ե�DES Key ����Ӧ��PID
* Note        :
 ***************************************************************************/
DRV_ErrCode DRV_TKDMAStoreDESKey (DRV_TKDMA_DESChannel_t Channel,
				      DRV_U8 Index, 
				      DRV_TKDMA_Key_t * Key, 
				      DRV_U32 PID);


#ifdef __cplusplus
}
#endif
#endif

