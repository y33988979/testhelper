/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/03/25
  Description   :
  History       :
  1.Date        : 2011/03/25
    Author      :
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief Describes the information about the video phone (VP) engine. CNcomment: �ṩVP��Ƶ����������Ϣ
 */
#ifndef __HI_UNF_VP_H__
#define __HI_UNF_VP_H__
#include "hi_common.h"
#include "hi_unf_common.h"
#include "hi_unf_vi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2_13 */
/** @{ */  /** <!-- ====[VP data structure] */

/** */
typedef struct hi_VP_VENC_CHN_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E     enEncType;	       /**<Encoder type*/ /**<CNcomment: ���������� */
    HI_UNF_VI_E              enViPort;		   /**<Input port*/ /**<CNcomment: ����˿� */
    HI_UNF_VI_INPUT_MODE_E   enInputMode;      /**<Video input mode. Only the universal serial bus (USB) camera is supported.*/ /**<CNcomment: ��Ƶ����ģʽ��Ŀǰֻ֧��USB����ͷ*/
    HI_U32                   u32Width;
    HI_U32                   u32Height;
    HI_U32                   u32TargetBitRate; /**<Rate controller (RC) parameter. It can be set dynamically.*/ /**<CNcomment: RC����,�ɶ�̬���� */
    HI_U32                   u32TargetFrmRate; /**<Target frame rate. It can be set dynamically.*/ /**<CNcomment: Ŀ��֡��,�ɶ�̬���� */ 
    HI_U32                   u32InputFrmRate;  /**<Input frame rate. It can be set dynamically. The value of u32TargetFrmRate is less than or equal to the value of u32InputFrmRate.*/
                                               /**<CNcomment: ����֡��,�ɶ�̬����,u32TargetFrmRate <= u32InputFrmRate */ 
    HI_U32                   u32Gop;		   /**<Group of picture (GOP) size. It can be set dynamically.*/ /**<CNcomment: GOP��С,�ɶ�̬���� */ 
	HI_UNF_VP_NET_PARA_S     stEncNetPara;	   /**<Local network parameter*/ /**<CNcomment: �����������*/	
}HI_VP_VENC_CHN_ATTR_S; 

typedef struct hi_VP_VDEC_CHN_ATTR_S
{
    HI_UNF_VCODEC_TYPE_E     enDecType;	       /**<Decoder type*/ /**<CNcomment: ���������� */
	HI_UNF_VP_NET_PARA_S     stDecNetPara;	   /**<Local network parameter*/ /**<CNcomment: �����������*/
	
}HI_VP_VDEC_CHN_ATTR_S;          

/**Video engine channel attribute*/
/*CNcomment: ��Ƶ����ͨ������ */
typedef struct hiUNF_VP_CHN_ATTR_S
{
    HI_HANDLE hLocalWin;                       /**<Handle of a local video output (VO) window*/ /**<CNcomment: ����VO���ھ��*/  
    HI_HANDLE hRemoteWin;                      /**<Handle of a remote VO window*/ /**<CNcomment: Զ��VO���ھ��*/
    HI_VP_VENC_CHN_ATTR_S stEncAttr;           /**<Encoder parameter*/ /**<CNcomment: ����������*/
	HI_VP_VDEC_CHN_ATTR_S stDecAttr;           /**<Decoder parameter*/ /**<CNcomment: ����������*/
}HI_UNF_VP_CHN_ATTR_S;

/**RTCP configure of video engine channel*/
/*CNcomment: ��Ƶ����ͨ��RTCP������Ϣ */
typedef struct hiUNF_VP_RTCP_CFG_S
{
    HI_U32 		u32EnableFlag;                 /**<Enable RTCP function: 0--disable, 1--enable*/ /**<CNcomment: RTCPͳ��ʹ��: 0--��ʹ��, 1--ʹ��*/
    HI_U32 		u32SendInterval;               /**<Send interval of RTCP report, [500, 65535]ms, must be integer of 500ms*/ /**<CNcomment: RTCP���淢�ͼ��, [500, 65535] ms , ����Ϊ500ms��������*/
}HI_UNF_VP_RTCP_CFG_S;

/**RTCP statistics information*/
/**CNcomment: RTCP ͳ����Ϣ*/
typedef struct hiUNF_VP_RTCP_STATIC_INFO_S
{
    HI_U32 u32TotalLostPacket;                 /**<Total lost packet*/ /**<CNcomment: ���ݶ���ͳ��*/
    HI_U32 u32LossFraction;                    /**<Lost rate(%)*/ /**<CNcomment: ������(%) */
    HI_U32 u32Jitter;                          /**<Jitter(ms)*/ /**<CNcomment: ƽ������(ms) */
    HI_U32 u32Delay;                           /**<Delay(ms)*/ /**<CNcomment: ƽ��ʱ��(ms) */
}HI_UNF_VP_RTCP_STATIC_S; 

/**Statistic information of network*/
/*CNcomment: ��Ƶ����ͨ������ͳ����Ϣ */
typedef struct hiUNF_VP_NET_STA_S
{
    HI_U32 u32SendPacketNum;                   /**<Total send packet number*/ /**<CNcomment: ���Ͱ�����*/
    HI_U32 u32SendBytes;                       /**<Total send bytes*/ /**<CNcomment: �����ֽ���*/
    HI_UNF_VP_RTCP_STATIC_S stSendInfo;        /**<Send information*/ /**<CNcomment: ����ͳ��*/
    HI_UNF_VP_RTCP_STATIC_S stRecvInfo;        /**<Reveive information*/ /**<CNcomment: ����ͳ��*/
}HI_UNF_VP_NET_STA_S;
/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_2_3 */
/** @{ */  /** <!-- ��VP�� */

/**
 \brief Initializes the VP module. CNcomment: ��ʼ��VPģ��
 \attention \n
Call this application programming interface (API) before creating a VP channel by calling ::HI_UNF_VP_Create. 
CNcomment: �ڵ���::HI_UNF_VP_Create�ӿڴ�������֮ǰ�����ñ��ӿ�
 \param N/A CNcomment: ��
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_DEV_NOT_EXIST The VP device does not exist. CNcomment: �豸������
 \retval ::HI_ERR_VP_DEV_OPEN_ERR The VP device fails to be started. CNcomment: �豸�򿪴���
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_Init(HI_VOID);

/**
 \brief Deinitializes the VP module. CNcomment: ȥ��ʼ��VPģ��
 \attention \n
Call this API after destroying all VP channels by calling ::HI_UNF_VP_DestroyChannel.
CNcomment: �ڵ���::HI_UNF_VP_DestroyChannel�ӿ��������е�VPͨ���󣬵��ñ��ӿ�
 \param N/A CNcomment: ��
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_NOT_INIT The VP is not initialized. CNcomment: VPδ��ʼ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_DeInit(HI_VOID);

/**
 \brief Obtains the default parameter values of a VP channel. CNcomment: ��ȡVPͨ��ȱʡ����
 \attention \n
Call this API before creating a VP channel by calling ::HI_UNF_VP_CreateChannel. This enables you to set parameters conveniently.
CNcomment: �ڵ���::HI_UNF_VP_CreateChannel�ӿڴ���VPͨ��֮ǰ�����ñ��ӿڣ�������д����
 \param[in] N/A CNcomment: ��
 \param[out] pstDefChAttr Pointer to the default VP channel attributes. For details, see :HI_UNF_VP_CHN_ATTR_S. CNcomment: ָ�����ͣ�VPͨ��Ĭ�����ԣ��μ�:HI_UNF_VP_CHN_ATTR_S
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_GetChDefaultAttr(HI_UNF_VP_CHN_ATTR_S *pstDefChAttr);

/**
 \brief Creates a VP channel. CNcomment: ����VPͨ��
 \attention \n
N/A CNcomment: ��
 \param[in]  pstChAttr Channel parameter CNcomment: ͨ������
 \param[out] phVp Pointer to the VP channel handle. CNcomment: ָ�����ͣ�VPͨ�����
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval ::HI_ERR_VP_NOT_SUPPORT The input parameter is invalid. CNcomment: ��������Ƿ�
 \retval ::HI_ERR_VP_NOT_SUPPORT The mode is not supported. CNcomment: ��֧��ģʽ
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_CreateChannel(HI_UNF_VP_CHN_ATTR_S *pstChAttr, HI_HANDLE *phVp);

/**
 \brief Destroys a VP channel. CNcomment: ����VP ͨ��
 \attention \n
N/A CNcomment: ��
 \param[in] hVp VP channel handle CNcomment: VPͨ�����
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_NOT_INIT The VP module is not initialized. CNcomment: VPδ��ʼ��
 \retval ::HI_ERR_VP_INVALID_HANDLE The VP handle is invalid. CNcomment: �Ƿ���VP���
 \retval Other values: A channel fails to be destroyed. CNcomment: ���� ����ͨ��ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_DestroyChannel(HI_HANDLE hVp);

/**
 \brief Obtains the configuration of a VP channel. CNcomment: ��ȡVP ͨ������
 \attention \n
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 \param[out] pstChAttr Pointer to VP channel attributes. For details, see :HI_UNF_VP_CHN_ATTR_S. CNcomment: ָ�����ͣ�VP ͨ�����ԣ��μ�:HI_UNF_VP_CHN_ATTR_S
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: Channel attributes fail to be obtained. CNcomment: ���� ��ȡͨ������ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_GetChAttr(HI_HANDLE hVp, HI_UNF_VP_CHN_ATTR_S *pstChAttr);

/**
 \brief Modifies the configuration of a VP channel. CNcomment: �޸�VPͨ������
 \attention \n
You are advised to obtain the current channel configuration by calling HI_UNF_VP_GetChAttr(), and modify parameter values as required. Set the corresponding change flags to 1, and set irrelevant change flags to 0.
CNcomment: �����ȵ���HI_UNF_VP_GetChAttr()��ȡ��ǰͨ�����ã��ٶԲ�������Ҫ�޸ģ���Ӧ�ĸ��ı����1���޹ظ��ı�־��0��
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 \param[in] pstChAttr Pointer to VP channel attributes. For details, see :HI_UNF_VP_CHN_ATTR_S. CNcomment: ָ�����ͣ�VP ͨ�����ԣ��μ�:HI_UNF_VP_CHN_ATTR_S
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: Channel attributes fail to be modified. CNcomment: ���� �޸�ͨ������ʧ��
 \see \n
N/A CNcomment:��
 */
HI_S32 HI_UNF_VP_SetChAttr(HI_HANDLE hVp, HI_UNF_VP_CHN_ATTR_S *pstChAttr);

/**
 \brief Starts a VP channel. CNcomment: VPͨ����ʼ����
 \attention \n
After setting parameters including the codec, video input (VI), and video output (VO) parameters, you can start the video engine by calling this API to transmit or receive data over the Internet and display the local or remote video output.
CNcomment: �ڲ���׼���ú󣬰������������VI/VO������ȷ�󣬵��ô˽ӿڿ���������Ƶ��������У�ʵ�������շ�����ʾ���ء�Զ����Ƶ���
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: A VP channel fails to be started. CNcomment: ���� VPͨ����ʼ����ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_Start(HI_HANDLE hVp);

/**
 \brief Stops a VP channel. CNcomment: VPͨ��ֹͣ����
 \attention \n
After a VP channel is started successfully, you can stop it by calling this API to freeze the screen or stop transmitting or receiving data over Internet. You can resume the VP channel by calling HI_UNF_VP_Start.
CNcomment: ��VPͨ�������������к󣬿���ͨ���˽ӿ���ͣ���У�ʹ���涳�ᡢ�����շ���ͣ�����Ե���HI_UNF_VP_Start�ָ�����
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: A VP channel fails to be started. CNcomment: ���� VPͨ����ʼ����ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_Stop(HI_HANDLE hVp);

/**
 \brief Previews the local video over a VP channel. CNcomment: VPͨ����ʼԤ��������Ƶ
 \attention \n
After the VI and hLocalWin parameters are set, you can preview the local video by calling this API.
CNcomment: ��VI��hLocalWin׼����֮�󣬿�ʼԤ��������Ƶ
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: A VP channel fails to be started. CNcomment: ���� VPͨ����ʼ����ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_StartPreView(HI_HANDLE hVp);

/**
 \brief Stops previewing the local video over a VP channel. CNcomment: VPͨ��Ԥ��������Ƶ
 \attention \n
When a local video is previewed over a VP channel, you can stop previewing the video by calling this API.
CNcomment: ��VPͨ��Ԥ��������Ƶ�󣬵���ֹͣԤ��
 \param[in] hVp VP channel handle CNcomment: VP ͨ�����
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other values: A VP channel fails to be started. CNcomment: ����  VPͨ����ʼ����ʧ��
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_StopPreView(HI_HANDLE hVp);

/**
 \brief Configs RTCP channel over a VP channel. CNcomment: VPͨ������RTCPͨ��
 \attention \n
When a VP channel has been created, you can config RTCP channel to enable and config RTCP sending frequency by calling this API.
CNcomment: �ڴ���VPͨ���󣬵��øýӿ�����RTCPͨ��������ʹ���Լ�����RTCP����Ƶ��
 \param[in] hVp VP channel handle CNcomment: VPͨ�����
 \param[out] pstRtcpCfg Pointer to config RTCP information, refer to HI_UNF_VP_RTCP_CFG_S. CNcomment: ָ�����ͣ�VP ͨ��RTCP������Ϣ���μ�:HI_UNF_VP_RTCP_CFG_S
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other value CNcomment: ����
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_ConfigRtcp(HI_HANDLE hVp,HI_UNF_VP_RTCP_CFG_S *pstRtcpCfg);

/**
 \brief Gets network statistics information over a VP channel. CNcomment: VPͨ����ȡ����ͳ����Ϣ
 \attention \n
When a VP channel has been started, you can get network statistics information to config parameter of video encoder by calling this API.
CNcomment: ������VPͨ���󣬵��øýӿڻ�ȡ����ͳ����Ϣ�Ե�������������
 \param[in] hVp VP channel handle CNcomment: VPͨ�����
 \param[out] pstNetStatics Pointer to network statistics information of VP channel, refer to HI_UNF_VP_NET_STA_S. CNcomment: ָ�����ͣ�VPͨ������ͳ����Ϣ���μ�:HI_UNF_VP_NET_STA_S
 \retval ::HI_SUCCESS Success CNcomment: �ɹ�
 \retval ::HI_ERR_VP_INVALID_PARA The input parameter is invalid. CNcomment: ��Ч���������
 \retval ::HI_ERR_VP_NULL_PTR The pointer is null. CNcomment: ��ָ��
 \retval Other value CNcomment: ���� 
 \see \n
N/A CNcomment: ��
 */
HI_S32 HI_UNF_VP_GetNetStatics(HI_HANDLE hVp,HI_UNF_VP_NET_STA_S *pstNetStatics);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif
