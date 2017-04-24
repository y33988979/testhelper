/**
 \file
 \brief Subtitle output Module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author 
 \date 2010-03-10
 */

#ifndef __HI_UNF_SO_H__
#define __HI_UNF_SO_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/*************************** Structure Definition ****************************/
/** \addtogroup     SO */
/** @{ */  /** <!��[SO]*/  


#define HI_UNF_SO_PALETTE_ENTRY (256)    /**< Number of palette */ /**< CNComment: ��ɫ����ɫ���� */
#define HI_UNF_SO_NO_PTS        (-1)
#define HI_UNF_SO_MAX_NODE_NUM  (20)
#define HI_UNF_SO_MAX_BUFFER_SIZE (0x200000)  /* 2M */


/** ��Ļ���� */
typedef enum hiUNF_SO_SUBTITLE_TYPE_E
{
    HI_UNF_SUBTITLE_BITMAP = 0x0,  /**< Bmp subtitle *//**< CNComment:ͼ����Ļ */
    HI_UNF_SUBTITLE_TEXT,          /**< Text subtitle *//**< CNComment:string��Ļ */
    HI_UNF_SUBTITLE_ASS,           /**< Ass/ssa sbutitle *//**< CNComment:ass��Ļ */
    HI_UNF_SUBTITLE_BUTT
} HI_UNF_SO_SUBTITLE_TYPE_E;

/** Message type of subtitle */
/** CNComment:��ʾ��Ϣ������teltext������ʾ�Ͳ���������Ϣ */
typedef enum hiUNF_SO_DISP_MSG_TYPE_E
{
    HI_UNF_SO_DISP_MSG_NORM  = 0x0,     /**< Draw the subtitle *//**< CNComment:������ʾ���� */
    HI_UNF_SO_DISP_MSG_ERASE,           /**< Clear the subtitle *//**< CNComment:���� */
    HI_UNF_SO_DISP_MSG_END              /**< Message of drawing a page *//**< CNComment:��ҳ��ʾ���� */
} HI_UNF_SO_DISP_MSG_TYPE_E;

/** The parameter of clear region */
/** CNComment:�������� */
typedef struct hiUNF_SO_CLEAR_PARAM_S
{
    HI_U32 x, y, w, h;     /**< Postion of clearing *//**< CNComment:����λ�ú͸߿���Ϣ */
} HI_UNF_SO_CLEAR_PARAM_S;

/** Color components  */
/** CNComment:��ɫ�ṹ */
typedef struct hiUNF_SO_COLOR_S
{
    HI_U8 u8Red;      /**< Red component *//**< CNComment:R������ɫֵ */
    HI_U8 u8Green;    /**< Green component *//**< CNComment:G������ɫֵ */
    HI_U8 u8Blue;     /**< Blue component *//**< CNComment:B������ɫֵ */
    HI_U8 u8Alpha;    /**< Alpha component *//**< CNComment:͸���ȣ�ֵΪ0Ϊ͸����0xFFΪ��͸�� */
} HI_UNF_SO_COLOR_S;

/** Information of bitmap subtitle */
/** CNComment:ͼ����Ļ��Ϣ */
typedef struct hiUNF_SO_GFX_S
{
    HI_S64 s64Pts;         /**< Start a display time, unit is Millisecond *//**< CNComment:��ʾʱ�������λms */
    HI_U32 u32Duration;    /**< Duration of displaying, unit is Millisecond *//**< CNComment:��ʾʱ������λms */
    HI_U32 u32Len;         /**< Bytes of subtitle data *//**< CNComment:���ݳ��ȣ���λ�ֽ� */
    HI_U8  *pu8PixData;    /**< Data of subtitle *//**< CNComment: ͼ������ */

    HI_UNF_SO_DISP_MSG_TYPE_E enMsgType;                   /**< Type of display message *//**< CNComment:��ʾ��Ϣ���� */  
    HI_UNF_SO_COLOR_S stPalette[HI_UNF_SO_PALETTE_ENTRY];  /**< Palette *//**< CNComment:��ɫ�壬ARGB8888 */
    HI_S32 s32BitWidth;    /**< Bits of Pix *//**< CNComment:����λ�� , ����Ϊ 2,4,8λ*/
    HI_U32 x, y, w, h;     /**< Position of display subtitle *//**< CNComment:��ʾλ�ú͸߿���Ϣ */
} HI_UNF_SO_GFX_S;

/** Infomation of text subtitle */
/** CNComment:�ı���Ļ��Ϣ */
typedef struct hiUNF_SO_TEXT_S
{
    HI_S64 s64Pts;              /**< Start a display time, unit is Millisecond *//**< CNComment:��ʾʱ�������λms */
    HI_U32 u32Duration;         /**< Duration of displaying, unit is Millisecond *//**< CNComment:��ʾʱ������λms */
    HI_U32 u32Len;              /**< Bytes of subtitle data *//**< CNComment:��Ļ���ݳ��ȣ���λ�ֽ� */
    HI_U8 *pu8Data;             /**< Data of subtitle *//**< CNComment:��Ļ���� */

    HI_U32 x, y, w, h;          /**< Position of display subtitle *//**< CNComment:��ʾλ�ú͸߿���Ϣ */
} HI_UNF_SO_TEXT_S;

/** Infomation of ass/ssa subtitle */
/** CNComment:ass��Ļ��Ϣ */
typedef struct hiUNF_SO_ASS_S
{
    HI_S64 s64Pts;                /**< Start a display time, unit is Millisecond *//**< CNComment:��ʾʱ�������λms */
    HI_U32 u32Duration;           /**< Duration of displaying, unit is Millisecond *//**< CNComment:��ʾʱ������λms */
    HI_U32 u32FrameLen;           /**< Bytes of subtitle data *//**< CNComment:֡���ȣ���λ�ֽ� */
    HI_U8  *pu8EventData;         /**< Data of subtitle *//**< CNComment:֡���� */
    HI_U32 u32ExtradataSize;      /**< Length of extra data *//**< CNComment:��չ���ݳ��ȣ���λ�ֽ� */
    HI_U8  *pu8Extradata;         /**< Extra data *//**< CNComment:��չ���� */
} HI_UNF_SO_ASS_S;

/** Infomation of subtitle */
/** CNComment:��Ļ��Ϣ */
typedef struct hiUNF_SO_SUBTITLE_INFO_S
{
    HI_UNF_SO_SUBTITLE_TYPE_E eType; /**< type *//**< CNComment:��Ļ���� */

    union
    {
        HI_UNF_SO_GFX_S  stGfx;      /**< Gfx subtitle *//**< CNComment:ͼ����Ļ */
        HI_UNF_SO_TEXT_S stText;     /**< Text subtitle *//**< CNComment:�ı���Ļ */
        HI_UNF_SO_ASS_S  stAss;      /**< Ass subtitle *//**< CNComment:ASS��Ļ */
    } unSubtitleParam;
} HI_UNF_SO_SUBTITLE_INFO_S;

/** 
\brief Call back funtion of getting current play time. 
CNComment:��ȡ��ǰ����֡��ʱ����������ǰʱ����Ч��ps64CurrentPts��ֵΪHI_UNF_SO_NO_PTS
\attention \n
None
\param[in] u32UserData Userdata.CNComment:ע�ᴫ����û�����
\param[out] ps64CurrentPts Current play time. CNComment:��ǰ����֡ʱ�������λms 

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_GETPTS_FN)(HI_U32 u32UserData, HI_S64 *ps64CurrentPts);

/** 
\brief Callback funtion of drawing the subtitle. CNComment:��Ļ��ͼ�ص�����
\attention \n
None
\param[in] u32UserData Userdata. CNComment:ע�ᴫ����û�����
\param[in] pstInfo Information of subtitle. CNComment:��Ļ��Ϣ
\param[in] pArg User data. CNComment:��չ����

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_ONDRAW_FN)(HI_U32 u32UserData, const HI_UNF_SO_SUBTITLE_INFO_S *pstInfo, HI_VOID *pArg);

/** 
\brief Call back funtion of clearing the subtitle. CNComment:��Ļ��ʾ��ʱ�������
\attention \n
None
\param[in] u32UserData User data. CNComment:ע�ᴫ����û�����
\param[out] pArg Parameter of clearing the subtitle, Type is HI_UNF_SO_CLEAR_PARAM_S. CNComment:����������ָ��HI_UNF_SO_CLEAR_PARAM_S�ṹ

\retval ::HI_SUCCESS

\see \n
None
*/
typedef HI_S32 (*HI_UNF_SO_ONCLEAR_FN)(HI_U32 u32UserData, HI_VOID *pArg);

/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      SO */
/** @{ */  /** <!��[SO] */

/**
\brief  Init SO module. CNComment:subtitle output ģ���ʼ��
\attention \n
None
\param None

\retval ::HI_SUCCESS Init success.
\retval ::HI_FAILURE Operation fail.

\see \n
None
*/
HI_S32 HI_UNF_SO_Init(HI_VOID);

/**
\brief  Deinit SO module. CNComment:subtitle output ģ��ȥ��ʼ��
\attention \n
None
\param None

\retval ::HI_SUCCESS  Deinit success.
\retval ::HI_FAILURE Operation fail.

\see \n
None
*/
HI_S32 HI_UNF_SO_DeInit(HI_VOID);

/**
\brief  Creates an SO instance. CNComment:����һ��soʵ��
\attention \n
None
\param[out] phdl Handle of SO instance. CNComment:������ʵ��

\retval ::HI_SUCCESS  Create success. CNComment:�����ɹ��������Ч
\retval ::HI_FAILURE  Operation fail. CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_Create(HI_HANDLE *phdl);

/**
\brief  Destroy an SO instance. CNComment:����һ��soʵ��
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����

\retval ::HI_SUCCESS  Destroy success. CNComment:���ٳɹ�
\retval ::HI_FAILURE  Operation fail. CNComment:����ʧ�ܣ������Ƿ�

\see \n
None
*/
HI_S32 HI_UNF_SO_Destroy(HI_HANDLE handle);

/**
\brief  Setting offset time of the subtitle. CNComment:������Ļʱ��ƫ��
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[in] s64OffsetMs Offset of subtitle, unit is Millisecondes.CNComment:��Ļ��ʾʱ��ƫ��ֵ����λms

\retval ::HI_SUCCESS  Operation success.
\retval ::HI_FAILURE  Operation fail.

\see \n
None
*/
HI_S32 HI_UNF_SO_SetOffset(HI_HANDLE handle, HI_S64 s64OffsetMs);

/**
\brief  Register the callback function of getting current play time. CNComment:ע��ʱ�����ȡ�ص�������soͨ����ȡ��ǰ����ʱ���ͬ����Ļ
\attention \n
None
\param[in] handle Handle of SO instance.CNComment:soʵ�����
\param[in] pstCallback Callback function. CNComment:�ص���������
\param[in] u32UserData User data. CNComment:�û�����

\retval ::HI_SUCCESS  Register success. CNComment:ע��ɹ�
\retval ::HI_FAILURE  Parameter invalid. CNComment:ע��ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_RegGetPtsCb(HI_HANDLE handle, HI_UNF_SO_GETPTS_FN pstCallback, HI_U32 u32UserData);

/**
\brief  Register the callback function of drawing subtitle.
CNComment:ע����Ļ��������ص���������������˸ú�������soʹ�øú���ʵ����Ļ�������Ļ��ʾʱ�������so�����������֪ͨ���
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[in] pfnOnDraw Callback function of drawing subtitle. CNComment:���ƺ���
\param[in] pfnOnClear Callback function of clearing subtitle. CNComment:�������
\param[in] u32UserData User data. CNComment:�û�����

\retval ::HI_SUCCESS  Register success. CNComment:ע��ɹ�
\retval ::HI_FAILURE Parameter invalid. CNComment:ע��ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_RegOnDrawCb(HI_HANDLE handle, HI_UNF_SO_ONDRAW_FN pfnOnDraw,
                                          HI_UNF_SO_ONCLEAR_FN pfnOnClear, HI_U32 u32UserData);

/**
\brief  Set the surface of drawing subtitle, if you do not set the ondraw callback function, SO painting subtitles with the surface.
CNComment:������Ļ���ƻ�����������û������ondraw�ص���������soʹ�����õĻ�����������Ļ
\attention \n
This feature is not implemented. CNComment:�ù���δʵ��
\param[in] handle Handle of SO instance.CNComment:soʵ�����
\param[in] hSurfaceHandle Handle of surface. CNComment:�������

\retval ::HI_SUCCESS Success of setting surface. CNComment:���óɹ�
\retval ::HI_FAILURE  Operation invalid. CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_SetDrawSurface(HI_HANDLE handle, HI_HANDLE hSurfaceHandle);

/**
\brief  Set the font of drawing subtitle.
CNComment:������Ļ��ʾ���壬�����ý������û��������Ļ�ķ�ʽ������
\attention \n
This feature is not implemented. CNComment:�ù���δʵ��
\param[in] handle Handle of SO instance.CNComment:soʵ�����
\param[in] hFont Handle of font instance.CNComment:������������

\retval ::HI_SUCCESS  Success.CNComment:���óɹ�
\retval ::HI_FAILURE  Operation invalid. CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_SetFont(HI_HANDLE handle, HI_HANDLE hFont);

/**
\brief Set the color of drawing text subtitle.
CNComment:������Ļ��ʾ��ɫ�������ý������û��������Ļ�ķ�ʽ������
\attention \n
This feature is not implemented. CNCommnet:�ù���δʵ��
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[in] u32Color Value of the color. CNComment:��ɫֵ

\retval ::HI_SUCCESS  Success. CNComment:���óɹ�
\retval ::HI_FAILURE  Operation invalid. CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_SetColor(HI_HANDLE handle, HI_U32 u32Color);

/**
\brief Set the position of display subtitle.
CNComment:������Ļ��ʾ���꣬�����ý������û��������Ļ�ķ�ʽ������
\attention \n
This feature is not implemented. �ù���δʵ��
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[in] u32x Value of x coordinate.CNComment:x����
\param[in] u32y Value of y coordinate. CNComment:y����

\retval ::HI_SUCCESS  Success. CNComment:���óɹ�
\retval ::HI_FAILURE  Parameter invalid. CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_SetPos(HI_HANDLE handle, HI_U32 u32x, HI_U32 u32y);

/**
\brief  Getting the number in the butitle queue.
CNComment:��ȡ����buffer��δ��ʾ����Ļ���ݸ���
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[out] pu32SubNum Number of subtitles in queue. CNComment:������Ļ����

\retval ::HI_SUCCESS Success. CNComment:�ɹ�
\retval ::HI_FAILURE Operation fail. CNComment:ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_GetSubNumInBuff(HI_HANDLE handle, HI_U32 *pu32SubNum);

/**
\brief Clear the subtitles in queue.
CNComment:�����Ļ�����е����ݣ���Ļ�л�ʱ������øýӿ����so��Ļ����
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����

\retval ::HI_SUCCESS Success. CNComment:�ɹ�
\retval ::HI_FAILURE Operation invalid. CNComment:ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_ResetSubBuf(HI_HANDLE handle);

/**
\brief  Send subtitles to SO queue. If the data size and the number is larger than the value set by the user, then the transmission fails.
CNComment:������Ļ���ݸ�so���������size�͸����������û����õ�ֵ������ʧ��
\attention \n
None
\param[in] handle Handle of SO instance. CNComment:soʵ�����
\param[in] pstSubInfo Data of subtitle. CNComment:��Ļ��Ϣ
\param[in] u32TimeOut Timeout of sending subtitle. CNComment:���ͳ�ʱʱ�䣬��λms

\retval ::HI_SUCCESS  Success. CNComment:���ͳɹ�
\retval ::HI_FAILURE  Operation fail.CNComment:����ʧ��

\see \n
None
*/
HI_S32 HI_UNF_SO_SendData(HI_HANDLE handle, const HI_UNF_SO_SUBTITLE_INFO_S *pstSubInfo, HI_U32 u32TimeOutMs);

/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_SO_H__ */

