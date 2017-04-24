/******************************************************************************
  Copyright (C), 2004-2050, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_unf_vo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 
  Last Modified :
  Description   : header file for audio and video output control
  Function List :
  History       :
  1.Date        :
  Author        : 
  Modification  : Created file
******************************************************************************/

/** 
 * \file
 * \brief describle the information about AENC. CNcomment:�ṩAENC�������Ϣ
 */
 
#ifndef  __HI_UNF_AENC_H__
#define  __HI_UNF_AENC_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


/*************************** Structure Definition ****************************/

/**
\brief Initializes audio encoder CNcomment:��ʼ����Ƶ������
\attention \n
Call the interface before calling HI_UNF_AENC_Open. CNcomment:�ӿ�HI_UNF_AENC_Openǰ����
\param N/A
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_Init(HI_VOID);

/**
\brief Deinitializes audio encoder CNcomment:ȥ��ʼ����Ƶ������
\attention \n
N/A
\param N/A
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  Invalid operation CNcomment:HI_ERR_AENC_CH_NOT_SUPPORT ��Ч�Ĳ���
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_DeInit(HI_VOID);

/**
\brief Create audio encoder CNcomment:������Ƶ������
\attention \n
Before calling the interface, you must configure the encoder. Now AENC only supports 16-bit and 2 channels, and support 16K, 22K, 24K, 32K, 44K, or 48Ksample rate.   
CNcomment:���øýӿ�ǰ���������ñ�������ĿǰAENCֻ֧��16λ2�������Ҳ�����Ϊ��16K 22K 24K 32K 44K��48K
\param[in] phAenc                    Obtain the handle of the audio encoder. CNcomment:�õ���Ƶ���������
\param[in] pstAencAttr               Attribute structure of the audio encoder. CNcomment:��Ƶ���������Խṹ
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\retval ::HI_ERR_AENC_NULL_PTR        The pointer is null. CNcomment:ָ��Ϊ��
\retval ::HI_ERR_AENC_CREATECH_FAIL   The encoder fails to be created. CNcomment:����������ʧ��
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_CreateChn(HI_HANDLE *phAenc, const HI_UNF_AENC_ATTR_S *pstAencAttr);

/**
\brief Destroy the audio encoder CNcomment:������Ƶ������
\attention \n
\param[in] phAenc the handle of the audio encoder CNcomment:��Ƶ���������
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  The operation is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_INVALID_PARA    The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_CH_NOT_OPEN     The audio encoder is not opened. CNcomment:��Ƶ������û�д�
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_DestroyChn(HI_HANDLE hAenc);

/** 
\brief Copy data to the encoder for encoding CNcomment:����������������Թ�����
\attention \n
\param[in] phAenc     The audio encoder handle CNcomment:��Ƶ���������
\param[in] pstAOFrame Send a frame of data to the encoder. CNcomment:����Ƶ��������һ֡����
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_IN_BUF_FULL      The input buffer of the encoder is full. CNcomment:���������뻺����
\retval ::HI_ERR_AENC_INVALID_PARA     The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT   The operation is invalid. CNcomment:��Ч�Ĳ���
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_Pull(HI_HANDLE hAenc, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

/**
\brief Obtain data from a output buffer of the encoder and write the data into the file. CNcomment:�ӱ�������һ�����������ȡ�������Թ�д���ļ�
\attention \n
\param[in] phAenc     The audio encoder handle CNcomment:��Ƶ���������
\param[in] pstStream  The data in a output buffer of the audio encoder CNcomment:��Ƶ������һ����������е�����
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA    The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_OUT_BUF_EMPTY   The output buffer of the encoder is empty. CNcomment:�������������Ϊ��
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT  The operation is invalid. CNcomment:��Ч�Ĳ���
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_AcquireStream(HI_HANDLE hAenc, HI_UNF_ES_BUF_S *pstStream);

/**
\brief Destroy the data in a output buffer of the encoder CNcomment:���ٱ�����һ����������е�����
\attention \n
Call the interface after calling HI_UNF_AENC_ReceiveStream. CNcomment:��HI_UNF_AENC_ReceiveStream֮�����
\param[in] phAenc     The handle of audio encoder CNcomment:��Ƶ���������
\param[in] pstStream  The data in a output buffer of the audio encoder CNcomment:��Ƶ������һ����������е�����
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA   The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_OUT_BUF_BAD    The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_IN_BUF_UNEMPTY The input buffer is empty. CNcomment:���뻺��ǿ�
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT The operation is invalid. CNcomment:��Ч�Ĳ���
\see \n
N/A
*/
HI_S32   HI_UNF_AENC_ReleaseStream(HI_HANDLE hAenc, const HI_UNF_ES_BUF_S *pstStream);


/**
\brief Register dynamic audio encoding libraries. CNcomment:ע����Ƶ��̬�����
\attention \n
\param[in] pEncoderDllName The file name of audio encoding  libraries CNcomment:pEncoderDllName ��Ƶ������ļ���
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Failure CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_RegisterEncoder(const HI_CHAR *pEncoderDllName);

/**
\brief Attach audio encoder with AVPLAY player CNcomment:����Ƶ��������AVPLAY������
\attention \n
Before obtaining the streams and encoding, you must call the interface. CNcomment:��ʼ����ͻ�ȡ����֮ǰ��Ҫ���ȵ��øýӿ�
\param[in] hSource   The handle of AVPLAY player CNcomment:AVPLAY���������
\param[in] hAenc     The handle of audio encoder CNcomment:��Ƶ���������
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT        The operation is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Attach(HI_HANDLE hAenc, HI_HANDLE hSource);

/**
\brief Detach audio encoder with AVPLAY player CNcomment:�����Ƶ��������AVPLAY��������
\attention \n
Before calling the interface, stop the encoding. CNcomment:���øýӿ���Ҫ����ֹͣ����
\param[in] hSource   The handle of AVPLAY player CNcomment:AVPLAY���������
\param[in] hAenc     The handle of audio encoder CNcomment:��Ƶ���������
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_CH_NOT_SUPPORT        The operation is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Detach(HI_HANDLE hAenc, HI_HANDLE hSource);

/**
\brief Start to encode CNcomment:��ʼ����
\attention \n
Before calling the interface, you need to initialize the encoder, create an encoding channel and attach the player.
CNcomment:���øýӿ���Ҫ���ȳ�ʼ������������������ͨ�����󶨲�����
\param[in] hAenc The handle of audio encoder CNcomment:��Ƶ���������
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Start(HI_HANDLE hAenc);


/**
\brief Stop to encode CNcomment:ֹͣ����
\attention \n
Before calling the interface, you need to initialize the encoder, create an encoding channel, attach the player, and start the encoding.
CNcomment:���øýӿ���Ҫ���ȳ�ʼ������������������ͨ�����󶨲��������Ѿ���ʼ����
\param[in] hAenc   The handle of audio encoder CNcomment:��Ƶ���������
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_AENC_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ���
\retval ::HI_ERR_AENC_NULL_PTR	            The pointer is null. CNcomment:ָ��Ϊ��
\retval ::HI_ERR_AVPLAY_INVALID_OPT	        The operation is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_AENC_Stop(HI_HANDLE hAenc);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_AENC_H__*/

