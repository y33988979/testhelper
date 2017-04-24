/**
 \file
 \brief Describes the header file that contains the common contents of the HiGo.CNcomment:HiGo��������ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-16
 */

#ifndef __HI_GO_COMM_H__
#define __HI_GO_COMM_H__

/* add include here */
#include "hi_type.h"
#include "hi_go_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_3_3_2 */
/** @{ */  /**<!��[Common contents of the HiGo]*//**CNcomment: <!-- ��HiGo�������ݡ� */

/**Maximum number of colors in the palette*//**CNcomment: ��ɫ�������ɫ�� */
#define MAX_PALETTE_COLOR_SIZE 256

/**Color value*//**CNcomment: ��ɫֵ */
typedef HI_U32 HI_COLOR;

/**Palette*//**CNcomment: ��ɫ�� */
typedef HI_COLOR HI_PALETTE[MAX_PALETTE_COLOR_SIZE];

typedef enum 
{
    HIGO_IMGTYPE_JPEG = 0, /**<.jpeg picture*//**CNcomment:< JPEG��ʽͼƬ*/
    HIGO_IMGTYPE_GIF,      /**<.gif picture*//**CNcomment:< GIF��ʽͼƬ*/
    HIGO_IMGTYPE_BMP,       /**<.bmp picture*//**CNcomment:< BMP��ʽͼƬ */
    HIGO_IMGTYPE_PNG,      /**<.png picture*//**CNcomment:< PNG��ʽͼƬ */
    HIGO_IMGTPYE_BUTT
} HIGO_IMGTYPE_E;


/**Rectangle*//**CNcomment: ���� */
typedef struct
{
    HI_S32 x, y;

    HI_S32 w, h;
} HI_RECT;

/**Region*//**CNcomment: ���� */
typedef struct
{
    HI_S32 l;
    HI_S32 t;
    HI_S32 r;
    HI_S32 b;
} HI_REGION;


#define HIGO_INVALID_HANDLE 0x0

/**Stream position*//**CNcomment: ��ʽλ��*/


/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** \addtogroup      H_1_3_3_2 */
/** @{ */  /** <!��[Common contents of the HiGo]*//**CNcomment: <!-- ��HiGo�������ݡ� */


/** 
\brief Initializes the HiGo.CNcomment:HiGo��ʼ��
\attention \n
Before using the HiGo, you must call this application programming interface (API) to initialize the HiGo. The HiGo 
includes the graphic device (Gdev) module, decoder, Winc module, bit block transfer (Bliter) module, and surface 
module.
CNcomment:ʹ��HiGo���ܱ����ȵ��øýӿڣ����HiGo�ĳ�ʼ������Ҫ����gdev, decoder, winc, blit, surfaceģ��
\param N/A.CNcomment:��

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INITFAILED

\see \n
::HI_GO_Deinit
*/
HI_S32	HI_GO_Init(HI_VOID);

/**
\brief Deinitializes the HiGo.CNcomment: HiGoȥ��ʼ��
\attention \n
If the HiGo is not used, you need to call this API to release resources.CNcomment: ����ʹ��HiGo����ʱ��Ҫ���øýӿڣ��ͷ���Դ
\param N/A.CNcomment: ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_Init
*/
HI_S32	HI_GO_Deinit(HI_VOID);

/** 
\brief Initializes the extended library of the HiGo.CNcomment:HiGo ��չ���ʼ��
\attention \n
If the extended library of the HiGo is not used, you need to call this API to release resources, especially the 
resources of the cursor and text modules.
CNcomment:��Ҫ��CURSOR��TEXTģ�顣
\param N/A.CNcomment:��

\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_DeInitExt
*/
HI_S32 HI_GO_InitExt();

/**
\brief Deinitializes the extended library of the HiGo.CNcomment: HiGo��չ��ȥ��ʼ��
\attention \n
If the HiGo is not used, you need to call this API to release resources.CNcomment: ����ʹ��HiGo��չ�⹦��ʱ��Ҫ���øýӿڣ��ͷ���Դ����Ҫ��CURSOR��TEXTģ�顣
\param N/A.CNcomment: ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_DEINITFAILED

\see \n
::HI_GO_InitExt
*/
HI_S32 HI_GO_DeInitExt();

/** 
\brief Obtains the version information.CNcomment:��ȡ�汾��Ϣ
\attention \n
N/A.CNcomment:��
\param[out] ppVersion Output address of the version information string. The value cannot be empty.CNcomment: �汾��Ϣ�ַ��������ַ������Ϊ��
\param[out] ppBuildTime Build Output address of the time string. The value cannot be empty.CNcomment:Buildʱ���ַ��������ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

\see \n
N/A.CNcomment:��
*/
HI_S32 HI_GO_GetVersion(HI_CHAR ** ppVersion, HI_CHAR **ppBuildTime);

/**
\brief Converts the data on a surface into a .bmp picture for output.CNcomment: ��Surface�е�����ת����BMP��ʽͼ�������
\attention \n
The output file is [year]-[date]-[hour]-[min]-[second]-[ms].bmp.CNcomment: ����ļ���Ϊ[year]-[date]-[hour]-[min]-[second]-[ms].bmp
The output picture must be a 16-bit bitmap.CNcomment: ���ͼƬ�̶�Ϊ16λͼ��
It is recommended that you call HI_GO_EncodeToFile rather than HI_GO_Surface2Bmp.CNcomment: ����ʹ��HI_GO_EncodeToFile�ӿ�������˽ӿ�

\param[in] Surface Data to be captured.CNcomment: ��Ҫ���н��������ݡ�
\param[in] pRect Pointer to a rectangle. If this parameter is not set, it indicates the entire surface.CNcomment: ����ָ��,Ϊ�ձ�ʾ����surface��


\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVSURFACEPF
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NOMEM

*/
HI_S32	HI_GO_Surface2Bmp(HI_HANDLE Surface, const HI_RECT *pRect);

/** 
\brief Enables the memory management module.CNcomment:ʹ���ڴ����ģ��
\attention \n
The memory management module is disabled by default. Before using the memory statistics function, you must call this 
API to enable the memory management module.
CNcomment:Ĭ��������ڷ�ʹ��״̬��ֻ�иýӿڴ򿪺����ʹ���ڴ�ͳ�ƹ���

\param[in] bEnable Whether to enable the memory statistics function.CNcomment:�Ƿ����ڴ�ͳ�ƹ���

\retval ::HI_SUCCESS

*/
HI_S32 HI_GO_EnableMemMng(HI_BOOL bEnable);

/**
\brief 
\attention \n
\param[out] pbEnable 

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

*/

/** 
\brief Obtains the enable status of the memory management module.CNcomment:��ȡ�ڴ����ģ��ʹ��״̬
\attention \n
\param[out] pbEnable Whether to obtain the enable status of the memory statistics function.CNcomment:��ȡ�ڴ�ͳ�ƹ����Ƿ���

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR

*/
HI_S32 HI_GO_GetMemMngStatus(HI_BOOL *pbEnable);

/** 
\brief Outputs the general information about the system memory.CNcomment:���ϵͳ�ڴ��������Ϣ
\attention \n
\param N/A.CNcomment:��

\retval ::HI_SUCCESS
*/
HI_S32 HI_GO_SysMemQuene(HI_VOID);

/** 
\brief Outputs the general information about the media memory zone (MMZ).CNcomment: ���MMZ�ڴ��������Ϣ��
\attention \n
\param N/A.CNcomment:��

\retval ::HI_SUCCESS

*/
HI_S32 HI_GO_MMZMemQuene(HI_VOID);

/** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_COMM_H__ */
