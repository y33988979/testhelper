/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/16
  Description   :
  History       :
  1.Date        : 2009/12/16
    Author      : w58735
    Modification: Created file

*******************************************************************************/
/** 
 * \file
 * \brief  supply info of vo module. CNcomment:�ṩVO�������Ϣ
 */
 
#ifndef  __HI_UNF_VO_H__
#define  __HI_UNF_VO_H__

#include "hi_unf_common.h"
#include "hi_unf_vi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      VO */
/** @{ */  /** <!-- [VO] */

/**defines the vo enumeration of video layer.*/
/**CNcomment:����VOö��*/
typedef enum hiUNF_VO_E
{
    HI_UNF_VO_SD0   = 0x0,  /**< standard definition video layer*//**<CNcomment:������Ƶ��0 */
    HI_UNF_VO_HD0,          /**< high definition video layer*//**<CNcomment:������Ƶ��0 */
    HI_UNF_VO_BUTT
}HI_UNF_VO_E;

/**define the attribution of window.*/
/**CNcomment:����WINDOW����*/
typedef struct hiUNF_WINDOW_ATTR_S
{
    HI_UNF_VO_E               enVo;          /**<the layer where window locate in.*//**<CNcomment: �������ڵ���Ƶ�� */
    HI_BOOL                   bVirtual;      /**< whether is virtual window or not.*//**<CNcomment: �Ƿ�Ϊ���ⴰ�� */
    HI_UNF_VIDEO_FORMAT_E     enVideoFormat; /**<the data format of virtual window*//**<CNcomment: ���ⴰ�����ݸ�ʽ */
    HI_UNF_ASPECT_RATIO_E     enAspectRatio; /**<the aspect ratio of window.*//**<CNcomment: ���ڵĿ��ת������ */
    HI_UNF_ASPECT_CVRS_E      enAspectCvrs;  /**<the method to the aspect ratio transfer.*//**<CNcomment: ���ڵĿ��ת����ʽ */
    HI_RECT_S                 stInputRect;   /**<the crop area .*//**<CNcomment: ���ڵĲü����� */
    HI_RECT_S                 stOutputRect;  /**<the window display area on the layer.*//**<CNcomment: ��������Ƶ���ϵ���ʾ���� */
    HI_RECT_S                 stReviseOutputRect; /*Revise win size*/
} HI_UNF_WINDOW_ATTR_S;

typedef struct hiUNF_BUFFER_ATTR_S
{
    HI_S32              s32Stride;
    HI_U32              u32PhyAddr[16];
    HI_U32              u32BuffIndex[16];
    HI_S32              s32BuffNum;
}HI_UNF_BUFFER_ATTR_S;

/** the running mode of vo*/
/**CNcomment:VO�豸������ģʽ */
typedef enum hiUNF_VO_DEV_MODE_E
{
    HI_UNF_VO_DEV_MODE_NORMAL,  /**<normal mode: no more than 2 windows to be created.*//**<CNcomment: ����ģʽ��ֻ�ܴ���2��WINDOW */
    HI_UNF_VO_DEV_MODE_MOSAIC,  /**<mosaic mode: can create more than 2 windows.*//**<CNcomment: ������ģʽ�����Դ������WINDOW */
    HI_UNF_VO_DEV_MODE_STANDALONE,  /*seperate mode,only 1 window can be created,and  the aspect ratio set seperately.*/
    HI_UNF_VO_DEV_MODE_BUTT
} HI_UNF_VO_DEV_MODE_E;

/**define the freeze mode enumeration.*/
/**CNcomment:����Freezeģʽö��*/
typedef enum hiUNF_WINDOW_FREEZE_MODE_E
{
    HI_UNF_WINDOW_FREEZE_MODE_LAST = 0,   /**<display the last frame when freeze.*//**<CNcomment: freezeʱ��ʾ���һ֡*/
    HI_UNF_WINDOW_FREEZE_MODE_BLACK = 1,  /**<display a black frame when freeze.*//**<CNcomment: freezeʱ��ʾ�����*/
    HI_UNF_WINDOW_FREEZE_MODE_BUTT
} HI_UNF_WINDOW_FREEZE_MODE_E;

/**define the algs enumeration.*/
/**CNcomment:�����㷨����ö��*/
typedef  enum hiUNF_ALG_TYPE_E
{
    HI_ALG_TYPE_DEI = 0,		/**<deinteralce alg.*//**<CNcomment: ��Ƶȥ�����㷨*/
    HI_ALG_TYPE_ACC,			/**<clolor enhanced alg. *//**<CNcomment: ��Ƶ��ɫ��ǿ�㷨*/
    HI_ALG_TYPE_ACM,			/**<color management alg.*//**<CNcomment: ��Ƶ��ɫ�����㷨*/
    HI_ALG_TYPE_SHARPNESS,		/**<the color sharpen alg.*//**<CNcomment: ��Ƶ��ɫ���㷨*/
    HI_ALG_TYPE_BUTT
} HI_UNF_ALG_TYPE_E;

/**define the struct of alg contoller func.*/
/**CNcomment:�����㷨���ƽṹ��  */
typedef struct hiUNFALG_CONTROL_S
{
    HI_UNF_ALG_TYPE_E	enAlgType;		/**<the alg enum.*//**<CNcomment: �㷨��������*/
    HI_BOOL		bEnable;		/**<whether to open the alg or not.*//**<CNcomment: �㷨ʹ��/�رտ���*/
    HI_VOID    *alg_info;		/** define the alg information.*/
	                                        /** since the alg_info structure is not the same for different alg types, so it's a ptr of void **/
} HI_UNF_ALG_CONTROL_S;

/*define the sharpen alg info struct.*/
typedef struct hiUNF_VO_SHARPEN_INFO_S
{ 
    HI_S16 s16LTICTIStrengthRatio;		/**define the sharpen strength.*/
}HI_UNF_VO_SHARPEN_INFO_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      VO */
/** @{ */  /** <!-- [VO] */

/**
\brief  init vo module. CNcomment:��ʼ��VOģ��
\attention \n
call this func before any other vo funcs .
CNcomment:����VOģ��Ҫ�����ȵ��ñ��ӿ�
\param[in]   enDevMode  the vo running mode.  CNcomment:VOģ�������ģʽ
\retval ::HI_SUCCESS successs.CNcomment:�ɹ�
\retval ::HI_ERR_VO_DEV_NOT_EXIST  vo device not exist. CNcomment:VO�豸������
\retval ::HI_ERR_VO_NOT_DEV_FILE   not vo device. CNcomment:VO���豸
\retval ::HI_ERR_VO_DEV_OPEN_ERR   open vo failed. CNcomment:VO��ʧ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal param. CNcomment:��������Ƿ�
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_Init(HI_UNF_VO_DEV_MODE_E enDevMode);

/**
\brief  deinit vo module.CNcomment:ȥ��ʼ��VOģ��
\attention \n
before calling this func ,you should call HI_UNF_VO_Close to close all the windows first.
CNcomment:�ڵ���::HI_UNF_VO_Close�ӿڹر����д򿪵�VO����ñ��ӿ�
\param CNcomment:��
\retval ::HI_SUCCESS  success.CNcomment:�ɹ�
\retval ::HI_ERR_VO_DEV_CLOSE_ERR  vo deinited failed. CNcomment:VO�ر�ʧ��
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_DeInit(HI_VOID);

/**
\brief open the given vo layer.CNcomment:��ָ��VO
\attention \n
CNcomment:��
\param[in] enVo    vo layer.CNcomment:VO���
\retval ::HI_SUCCESS openned success.CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT  display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_CREATE_ERR    opend failed.CNcomment:����ʧ��
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_Open(HI_UNF_VO_E enVo);

/**
\brief close the given vo layer.CNcomment:�ر�ָ��VO
\attention \n
CNcomment:��
\param[in] enVo  vo layer.CNcomment:VO���
\retval ::HI_SUCCESS  close success.CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params. CNcomment:��������Ƿ�
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_Close(HI_UNF_VO_E enVo);




/**
\brief set the alpha value of given vo layer.CNcomment:����ָ��VO��ALPHA
\attention \n
CNcomment:��
\param[in] enVo       vo layer num.  CNcomment:VO���
\param[in] u32Alpha   the alpha value determines transparence of image,range:0~255.0:transparent;255: not transparent. 
 CNcomment:��Ƶͼ����ʾ͸����ֵ��ȡֵ��ΧΪ0��255��ȡֵΪ0ʱ��ȫ͸����ȡֵΪ255ʱ����͸��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA illegal params. CNcomment:��������Ƿ�
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetAlpha(HI_UNF_VO_E enVo, HI_U32 u32Alpha);

/**
\brief  get the alpha value of given vo layer. CNcomment:��ȡָ��vo��ALPHA
\attention \n
CNcomment:��
\param[in]  enVo    vo layer number. CNcomment:VO���
\param[out] pu32Alpha   ptr value,determines the alpha value.CNcomment:ָ�����ͣ���Ƶͼ����ʾ͸����ֵ
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA illegal params. CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR      null pointerCNcomment:����ָ��Ϊ��
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_GetAlpha(HI_UNF_VO_E enVo, HI_U32 *pu32Alpha);

/**
\brief set the rect of given vo.CNcomment:����ָ��VO��rect
\attention \n
this func can be called only at the conditions of mosaic mode of vo.
CNcomment:ֻ����VO������MOSAICģʽ���ܵ��ñ��ӿ�
\param[in] enVo       vo layer number. CNcomment:VO���
\param[in] pstRect   a pointer typeof HI_RECT_S.CNcomment:rect�Ľṹָ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR      null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT   illegal operation.CNcomment:�����Ƿ�
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetRect(HI_UNF_VO_E enVo, const HI_RECT_S *pstRect);


/**
\brief  get the rect of given vo.CNcomment:��ȡָ��VO��rect
\attention \n
only under mosaic mode can this func be called.
CNcomment:ֻ����VO������MOSAICģʽ���ܵ��ñ��ӿ�
\param[in] enVo       vo layer number. CNcomment:VO���
\param[in] pstRect   a pointer typeof HI_RECT_S.CNcomment:rect�Ľṹָ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR      null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT   illegal operation. CNcomment:�����Ƿ�
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_GetRect(HI_UNF_VO_E enVo, HI_RECT_S *pstRect);

/**
\brief create the window.CNcomment:����WINDOW
\attention \n
only ater vo openning can windows be created.
CNcomment:ֻ�д�VO�󣬲�������֮�ϴ���WINDOW
\param[in] pWinAttr   a pointer typeof HI_UNF_WINDOW_ATTR_S.CNcomment:WINDO���Խṹָ��
\param[in] phWindow   the window handler to be created.CNcomment:������WINDOW���
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_CREATE_ERR     CNcomment:����ʧ��
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_CreateWindow(const HI_UNF_WINDOW_ATTR_S *pWinAttr, HI_HANDLE *phWindow);

/**
\brief  destroy the given window.CNcomment:����WINDOW
\attention \n
CNcomment:��
\param[in] hWindow    the input window handler. CNcomment:WINDOW���
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_DestroyWindow(HI_HANDLE hWindow);


/**
\brief to enable or disable the given window.CNcomment:ʹ��/��ֹWINDOW
\attention \n
Window can only be disabled after the corresponding avplay is stopped, else data will overflow.
CNcomment:ֻ���Ƚ�AVPLAY��VIDͨ��stop�󣬲��ܽ�ֹwindow�������������ݵ����
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] bEnable     the flag  of enable or disable window. CNcomment:ʹ��/��ֹ��־
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowEnable(HI_HANDLE hWindow, HI_BOOL bEnable);

/**
\brief get the status of the given window: enable or not.CNcomment:��ȡWINDOW��ʹ��״̬
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] pbEnable    the status of window:enable or disable.CNcomment:ʹ��/��ֹ��־.
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_GetWindowEnable(HI_HANDLE hWindow, HI_BOOL *pbEnable);

/**
\brief set the attribution of the given window. CNcomment:����WINDOW����
\attention \n
the two members of pWinAttr:envo and bVirtual are fixed when window created, and they can't be modified here.
CNcomment:enVo��bVirtual����һ��������Ͳ����ٽ����޸ġ�
\param[in] hWindow    the input window handler.CNcomment:WINDOW���
\param[in] pWinAttr   the pointer of windows attr.CNcomment:WINDO���Խṹָ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowAttr(HI_HANDLE hWindow, const HI_UNF_WINDOW_ATTR_S *pWinAttr);

/**
\brief get the attribution of the given window.CNcomment:��ȡWINDOW����
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] pWinAttr    the pointer of windows attr. CNcomment:WINDO���Խṹָ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_GetWindowAttr(HI_HANDLE hWindow, HI_UNF_WINDOW_ATTR_S *pWinAttr);

/**
\brief get the frame from given window ,used for 3d graphics testing.CNcomment:��ȡWINDOW��Frame��Ϣ��ר������3Dͼ�β���
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[out] pstFrameinfo  frame information. CNcomment:Frame��Ϣ
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited.CNcomment: DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_AcquireFrame(HI_HANDLE hWindow, HI_UNF_VO_FRAMEINFO_S *pstFrameinfo);

/**
\brief return the frame back to the given window, used for 3d graphics testing.CNcomment:�ͷ��û���ȡWINDOW��Frame��ר������3Dͼ�β���
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_ReleaseFrame(HI_HANDLE hWindow);

/**
\brief set the zorder of the given window.CNcomment:����WINDOW��Z��
\attention \n
CNcomment:��
\param[in] hWindow    the input window handler.CNcomment:WINDOW���
\param[in] enZFlag    how to adjust zorder. CNcomment:Z����ڷ�ʽ
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited. CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowZorder(HI_HANDLE hWindow, HI_LAYER_ZORDER_E enZFlag);

/**
\brief get the zorder of the given window.CNcomment:��ȡWINDOW��Z��
\attention \n
CNcomment:��
\param[in] hWindow       the input window handler.CNcomment:WINDOW���
\param[in] pu32Zorder    value of zoder, bigger value means higher prioty. CNcomment:Z����ֵ����ֵԽ�����ȼ�Խ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_GetWindowZorder(HI_HANDLE hWindow, HI_U32 *pu32Zorder);

/**
\brief attach the window with video source such as vi/avplay. CNcomment:��WINDOW��ָ����AVPLAY/VI��
\attention \n
it can attach the handler of avplay or vi to the window.
CNcomment:���Խ�AVPLAY��VI�ľ���󶨵�WINDOW�ϡ�
\param[in] hWindow     the input window handler. CNcomment:WINDOW���
\param[in] hSrc        the handler of video source. CNcomment:����Դ���
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params. CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_AttachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc);


/**
\brief detach the window from the video source such as avplay or vi. CNcomment:��WINDOW��ָ����AVPLAY/VI���
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] hSrc        the handler of video source.CNcomment:����Դ���
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR      null pointer.CNcomment: ����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_DetachWindow(HI_HANDLE hWindow, HI_HANDLE hSrc);

/**
\brief set the playing ratio of the given window.CNcomment:����Winsow�Ĳ�������
\attention \n
this function can change the playing ratio when playing.
CNcomment:�˹��������ڲ��Ź����иı䲥������
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] u32WinRatio      play rate of window, range:[1,256*256], 256 means normal rate, 512 means 2X, 128 means 1/2X.CNcomment:ȡֵ��ΧΪ [1, 256*256]��256���������ٶȲ���
\retval ::HI_SUCCESS CNcomment: �ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment: ��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST windows not exist.CNcomment: ���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowRatio(HI_HANDLE hWindow, HI_U32 u32WinRatio);


/**
\brief  to freeze the video content of given window.CNcomment:����WINDOW����ʾ����
\attention \n
the display will be freezed after calling this func, but backgrounddly,the data flow is normal from video decoder.
so when freezing is cacelled, the newest video content can be display at once.
CNcomment:�˹��������ڲ��Ź����ж�����Ļ��ʾ���ݣ���ʱWINDOW�е��������������У�\n
���ȡ������󣬻�������ʾ���µ����ݡ�
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] bEnable           shows whether to freeze window.CNcomment:ʹ��/��ֹ��־
\param[in] enWinFreezeMode   the mode of freeze.CNcomment:����ķ�ʽ
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR      null pointer.CNcomment: ����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_FreezeWindow(HI_HANDLE hWindow, HI_BOOL bEnable, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode);


/**
\brief reset the window.CNcomment:��λWINDOW
\attention \n
this function can reset buffers of the given window, and black frame or the last frame will be displayed according configuration.
CNcomment:�˹������window�еĻ��壬���ݲ�����ʾ��֡�����һ֡��
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] enWinFreezeMode   method of freeze the window.CNcomment:��λ�ķ�ʽ
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_ResetWindow(HI_HANDLE hWindow, HI_UNF_WINDOW_FREEZE_MODE_E enWinFreezeMode);

/**
\brief pass the external allocated buffer to this function to test 3d graphics.CNcomment:��WINDOW�����û��ⲿ�����buffer��ר������3Dͼ�β���
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] pstBufAttr  external buffer.CNcomment:�ⲿ��buffer
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_AttachExternBuffer(HI_HANDLE hWindow,HI_UNF_BUFFER_ATTR_S* pstBufAttr);

/**
\brief to set the work mode of window: quick output or not. Deinterlace alg will be disabled when quick output is enabled.
CNcomment:����WINDOW�Ƿ����ڿ������ģʽ���������ģʽǿ�ƹر�ȥ�����㷨
\attention \n
CNcomment:��
\param[in] hWindow     the input window handler.CNcomment:WINDOW���
\param[in] bQuickOutputEnable  whether determine the output mode of window. CNcomment:�������ģʽʹ��/��ֹ��־
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment: ��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetQuickOutputEnable(HI_HANDLE hWindow, HI_BOOL bQuickOutputEnable);

/**
\brief set the output rect attr when window is under browser mode.CNcomment:���������ģʽ�£������������
\attention \n
Only the member:stOutputRect of struct HI_UNF_WINDOW_ATTR_S is effective when calling this func.And then, the setting of HI_UNF_VO_SetWindowAttr
will be useless once calling this function. Negative value is allowed in rect setting.
CNcomment:ֻ��stOutputRect��Ա������Ч�����ô˽ӿں�HI_UNF_VO_SetWindowAttr\n
���õĴ������Խ���Ч���ýӿ�����������ָ�����
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] pWinAttr          the window attr only effective to outrect.CNcomment:WINDOW���ڣ�ֻ��stOutputRect��Ч��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT   display not inited.CNcomment: DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowOutRect(HI_HANDLE hWindow, const HI_UNF_WINDOW_ATTR_S *pWinAttr);


/**
\brief  to set the filed mode of given window. CNcomment:���ô��ڵĳ�����ģʽ
\attention \n
when fast-backward playing, there will be no abnormity through calling this func. But calling of this
func is forbidden when normal playing. It should be called after HI_UNF_VO_AttachWindow.
CNcomment:ͨ�����ô˽ӿ�ʹ�ܣ���֤���˲�����ֳ�ͼ����˵���������������ʱ��\n
������Ϊ��ֹ��Ĭ��Ϊ��ֹ��HI_UNF_VO_AttachWindow����֮��ʹ����Ч��
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] bEnable           wether to enable field play.CNcomment:ʹ��/��ֹ��־
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_SetWindowFieldMode(HI_HANDLE hWindow, HI_BOOL bEnable);

/**
\brief  to switch the given alg on or off.CNcomment:��Ƶ���㷨����
\attention \n
to switch the ACC/ACM alg on or off using this func, it also works when playing and should be called after vo init.
CNcomment:ͨ���˽ӿڿ�����Ƶ��ACC\ACM���㷨�Ŀ����رգ���������ʱҲ�����л���\n
����ҪVO INIT֮����ܵ���.
\param[in] pAlgControl         the control struct of alg. CNcomment:�㷨���ƽṹ��
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA   illegal params.CNcomment:��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT    illegal operation.CNcomment:�����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_AlgControl(const HI_UNF_ALG_CONTROL_S *pAlgControl);

/**
\brief capture the current frame of given window.CNcomment:����WINDOW�ĵ�ǰ֡
\attention \n
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] pstCapPicture
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment: ��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT   illegal operation.CNcomment: �����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_CapturePicture(HI_HANDLE hWindow, HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);


/**
\brief if you capture picture is finished, release the memory .CNcomment:������ϣ��ͷŲ�����ڴ�����
\attention \n
\param[in] hWindow           the input window handler.CNcomment:WINDOW���
\param[in] pstCapPicture
\retval ::HI_SUCCESS CNcomment:�ɹ�
\retval ::HI_ERR_VO_NO_INIT    display not inited.CNcomment:DISPδ��ʼ��
\retval ::HI_ERR_VO_INVALID_PARA  illegal params.CNcomment: ��������Ƿ�
\retval ::HI_ERR_VO_NULL_PTR       null pointer.CNcomment:����ָ��Ϊ��
\retval ::HI_ERR_VO_INVALID_OPT   illegal operation.CNcomment: �����Ƿ�
\retval ::HI_ERR_VO_WIN_NOT_EXIST  windows not exist.CNcomment:���ڲ�����
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_VO_CapturePictureRelease(HI_HANDLE hWindow, HI_UNF_CAPTURE_PICTURE_S *pstCapPicture);



/** @} */  /** <!-- ==== API declaration end ==== */


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
