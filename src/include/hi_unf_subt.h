#ifndef __HI_UNF_SUBT_H__
#define __HI_UNF_SUBT_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus*/

/***************************** Macro Definition ******************************/
/** Invalid handle in subtitle */
/**CNcomment: ��Ч����Ļ��� */
#define SUBT_INVALID_HANDLE   (0x0)
/** the max item(language) in Subtitling descriptor */
/**CNcomment: ������Ļ��(����)���� */
#define SUBT_ITEM_MAX_NUM     (32)
/** the max subtitle instance */
/**CNcomment: ������Ļģ��ʵ�� */
#define SUBT_INSTANCE_MAX_NUM (8)

/** @} */  /** <!-- ==== Macro Definition end ==== */

/*************************** Structure Definition ****************************/
/** Defines the type used to code the subtitle */
/**CNcomment: ������Ļ��������ö�� */
typedef enum hiUNF_SUBT_TYPE_E
{
    HI_UNF_SUBT_TYPE_BITMAP = 0, /**< coding of bitmap */ /**<CNcomment: λͼ���� */
    HI_UNF_SUBT_TYPE_TEXT,       /**< coded as a string of characters */ /**<CNcomment: �ı����� */
    HI_UNF_SUBT_TYPE_BUTT
}HI_UNF_SUBT_TYPE_E;

/** Defines the status of the subtitling page */
/**CNcomment: ������Ļҳ״̬ö�� */
typedef enum hiUNF_SUBT_PAGE_STATE_E
{
	HI_UNF_SUBT_PAGE_NORMAL_CASE		= 0,	/**< page update, use previous page instance to display */ /**<CNcomment: �ֲ����� */
	HI_UNF_SUBT_PAGE_ACQUISITION_POINT,		    /**< page refresh, use next page instance to display */ /**<CNcomment: ȫ��ˢ�� */
	HI_UNF_SUBT_PAGE_MODE_CHANGE,				/**< new page, needed to display the new page */ /**<CNcomment: ˢ��������Ļҳ */
	HI_UNF_SUBT_PAGE_BUTT
}HI_UNF_SUBT_PAGE_STATE_E;

/** Defines the data of the subtitle output */
/**CNcomment: ������Ļ������ݽṹ�� */
typedef struct hiUNF_SUBT_DATA_S
{
    HI_UNF_SUBT_TYPE_E enDataType;        /**< the type used to code the subtitle */ /**<CNcomment: ��Ļ�������� */
    HI_UNF_SUBT_PAGE_STATE_E enPageState; /**< the status of the subtitling page */ /**<CNcomment: ��Ļҳ״̬ */
    HI_U32             u32x;              /**< the horizontal address of subtitling page */ /**<CNcomment: x���� */
    HI_U32             u32y;              /**< the vertical address of subtitling page */ /**<CNcomment: y���� */
    HI_U32             u32w;              /**< the horizontal length of subtitling page */ /**<CNcomment: ��� */
    HI_U32             u32h;              /**< the vertical length of subtitling page */ /**<CNcomment: �߶� */
    HI_U32             u32BitWidth;       /**< bits in pixel-code */ /**<CNcomment: λ�� */
    HI_U32             u32PTS;            /**< presentation time stamp */ /**<CNcomment: ʱ��� */
    HI_U32             u32Duration;       /**< the period, expressed in ms, after which a page instance is no longer valid */ /**<CNcomment: ����ʱ�䣬��λ��ms */
    HI_U32             u32PaletteItem;    /**< pixels of palette */ /**<CNcomment: ��ɫ�峤�� */
    HI_VOID*           pvPalette;         /**< palette data */ /**<CNcomment: ��ɫ������ */
    HI_U32             u32DataLen;        /**< subtitling page data length */ /**<CNcomment: ��Ļ���ݳ��� */
    HI_U8*             pu8SubtData;       /**< subtitling page data */ /**<CNcomment: ��Ļ���� */
}HI_UNF_SUBT_DATA_S;

/** Defines the item of the subtitling content */
/**CNcomment: ������Ļ������ṹ�� */
typedef struct hiUNF_SUBT_ITEM_S
{
    HI_U32 u32SubtPID;      /**< the pid for playing subtitle */ /**<CNcomment: ��Ļpid */
    HI_U16 u16PageID;       /**< the Subtitle page id */ /**<CNcomment: ��Ļҳid */
    HI_U16 u16AncillaryID;  /**< the Subtitle ancillary id */ /**<CNcomment: ��Ļ����ҳid */
}HI_UNF_SUBT_ITEM_S;

typedef HI_S32 (*HI_UNF_SUBT_CALLBACK_FN)(HI_U32 u32UserData, HI_UNF_SUBT_DATA_S *pstData);

/** Defines the parameter of subtitle instance */
/**CNcomment: ������Ļģ������ṹ�� */
typedef struct hiUNF_SUBT_PARAM_S
{
    HI_UNF_SUBT_ITEM_S astItems[SUBT_ITEM_MAX_NUM]; /**< the item of the subtitling content */ /**<CNcomment: ��Ļ���� */
    HI_U8  u8SubtItemNum;                           /**< amount of subtitling item */ /**<CNcomment: ��Ļ���ݸ��� */
    HI_UNF_SUBT_CALLBACK_FN pfnCallback;            /**< callback function in which output subtitling page data */ /**<CNcomment: �ص����������������������Ļ���� */
    HI_U32 u32UserData;                             /**< user data used in callback function */ /**<CNcomment: �û����ݣ�ֻ���ڻص��������� */
}HI_UNF_SUBT_PARAM_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/**
\brief Initialize subtitle module. CNcomment: ��ʼ����Ļģ��
\attention \n
none. CNcomment: ��
\retval ::HI_SUCCESS initialize success. CNcomment: ��ʼ���ɹ�
\retval ::HI_FAILURE initialize failure. CNcomment: ��ʼ��ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_Init(HI_VOID);

/**
\brief DeInitialize subtitle module. CNcomment: ȥ��ʼ����Ļģ��
\attention \n
none. CNcomment: ��
\retval ::HI_SUCCESS deinitialize success. CNcomment: ȥ��ʼ���ɹ�
\retval ::HI_FAILURE deinitialize failure. CNcomment: ȥ��ʼ��ʧ��
\see \n
none. CNcomment: ��
*/
 HI_S32 HI_UNF_SUBT_DeInit(HI_VOID);

/**
\brief Create subtitle module. CNcomment: ������Ļģ��
\attention \n
none. CNcomment: ��
\param[in]  pstSubtParam  parameters used in created subtitle. CNcomment: ����ֵ
\param[out] phSubt        subtitle handle. CNcomment: ��Ļ���
\retval ::HI_SUCCESS create success. CNcomment: �����ɹ�
\retval ::HI_FAILURE create failure. CNcomment: ����ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_Create(HI_UNF_SUBT_PARAM_S *pstSubtParam, HI_HANDLE *phSubt);

/**
\brief Destroy subtitle module. CNcomment: ������Ļģ��
\attention \n
none. CNcomment: ��
\param[in]  hSubt         subtitle handle. CNcomment: ��Ļ���
\retval ::HI_SUCCESS destroy success. CNcomment: ���ٳɹ�
\retval ::HI_FAILURE destroy failure. CNcomment: ����ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_Destroy(HI_HANDLE hSubt);

/**
\brief Select one subtitle content to output. CNcomment: �л���Ļ����
\attention \n
none. CNcomment: ��
\param[in]  hSubt         subtitle handle. CNcomment: ��Ļ���
\param[in]  pstSubtItem   subtitle item. CNcomment: ��Ļ������
\retval ::HI_SUCCESS switching success. CNcomment: �л��ɹ�
\retval ::HI_FAILURE switching failure. CNcomment: �л�ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_SwitchContent(HI_HANDLE hSubt, HI_UNF_SUBT_ITEM_S *pstSubtItem);

/**
\brief Reset subtitle module. CNcomment: ��λ��Ļģ��
\attention \n
none. CNcomment: ��
\param[in]  hSubt         subtitle handle. CNcomment: ��Ļ���
\retval ::HI_SUCCESS reset success. CNcomment: ��λ�ɹ�
\retval ::HI_FAILURE reset failure. CNcomment: ��λʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_Reset(HI_HANDLE hSubt);

/**
\brief Update subtitle module. CNcomment: ������Ļģ��
\attention \n
none. CNcomment: ��
\param[in]  hSubt         subtitle handle. CNcomment: ��Ļ���
\param[in]  pstSubtParam  the new subtitle content. CNcomment: �µ���Ļ������Ϣ
\retval ::HI_SUCCESS update success. CNcomment: ���³ɹ�
\retval ::HI_FAILURE update failure. CNcomment: ����ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_Update(HI_HANDLE hSubt, HI_UNF_SUBT_PARAM_S *pstSubtParam);

/**
\brief Inject DVB subtitle stream to decoder. CNcomment: ע����Ļ���ݵ�������
\attention \n
Used the PES packet syntax for carriage of DVB subtitles. CNcomment:DVB��Ļ����ʹ��PES��ʽ 
\param[in]  hSubt         subtitle handle. CNcomment: ��Ļ���
\param[in]  u32SubtPID    the pid of subtitle stream. CNcomment: ��Ļ��pid
\param[in]  pu8Data       subtitle stream data. CNcomment: ��Ļ����
\param[in]  u32DataSize   the size of subtitle stream data. CNcomment: ��Ļ���ݳ���
\retval ::HI_SUCCESS inject success. CNcomment: ע��ɹ�
\retval ::HI_FAILURE inject failure. CNcomment: ע��ʧ��
\see \n
none. CNcomment: ��
*/
HI_S32 HI_UNF_SUBT_InjectData(HI_HANDLE hSubt, HI_U32 u32SubtPID, HI_U8* pu8Data, HI_U32 u32DataSize);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus*/

#endif
