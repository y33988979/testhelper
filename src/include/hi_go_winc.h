
/**
 \file
 \brief Describes the header file of the window (Winc) module.CNcomment:Wincģ��ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-8-5
 */

#ifndef __HI_GO_WINC_H__
#define __HI_GO_WINC_H__

#include "hi_type.h"
#include "hi_go_comm.h"
#include "hi_go_surface.h"
#ifdef __cplusplus
extern "C" {
#endif


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_3_3_9 */
/** @{ */  /** <!��[Winc module] *//**CNcomment: <!-- ��Wincģ�顿 */

/**Mode of adjusting the window z-order*//**CNcomment:����Z�������ʽ*/
typedef enum
{
    HIGO_ZORDER_MOVETOP = 0,  /**<Move to the top*//**CNcomment:<�Ƶ����*/
    HIGO_ZORDER_MOVEUP,       /**<Move upwards*//**CNcomment:<������*/
    HIGO_ZORDER_MOVEBOTTOM,   /**<Move to the bottom*//**CNcomment:<�Ƶ���ײ�*/
    HIGO_ZORDER_MOVEDOWN,     /**<Move downwards*//**CNcomment:<������*/
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;

/**Number of buffers used by the window*//**CNcomment:windowʹ��buffer�ķ�ʽ*/
typedef enum
{
    HIGO_BUFFER_SINGLE = 0,  /**<The window uses a buffer.*//**CNcomment:<windowʹ�õ�buffer*/
    HIGO_BUFFER_DOUBLE,     /**<The window uses dual buffers.*/ /**CNcomment:<windowʹ��˫buffer*/
    HIGO_BUFFER_BUTT
}HIGO_BUFFERTYPE_E;

/**Maximum number of window layers*//**CNcomment: ��󴰿ڲ��� */
#define HIGO_WINTREE_NUM 16

typedef struct
{
    HI_HANDLE hLayer; /**<Graphics layer on which a window is displayed*//**CNcomment:<������ʾ��ͼ��*/
    HI_RECT rect;     /**<Window range*//**CNcomment:<���ڵķ�Χ*/
    HI_U32 LayerNum;     /**<Display level of a window, ranging from 0 to 15. The greater the number, the upper the position of the window.*//**CNcomment:<������ʾ�ļ���,���Ϊ0-15������Խ��Ĵ���Խ������*/         
    HIGO_PF_E PixelFormat;/**<Pixel format of a window*//**CNcomment:<���ڵ����ظ�ʽ*/
    HIGO_BUFFERTYPE_E BufferType; /**<Number of surface buffers required by a window*/ /**CNcomment:<������Ҫ��surface buffer����������*/ 
} HIGO_WNDINFO_S;
/** @} */  /*! <!-- Structure Definition end */


/******************************* API declaration *****************************/
/** \addtogroup      H_1_3_3_9 */
/** @{ */  /** <!��[Winc module]*//**CNcomment: <!-- ��Wincģ�顿 */

/**
\brief Creates a window on a specified graphics layer and sets the pixel format of a window.CNcomment:��ָ��ͼ���ϴ�������
\attention \n
The new window is always located on the top of a specified graphics layer. The parts of the canvas surface that 
exceed the graphics layer are cropped.CNcomment:�´����Ĵ���ʼ����ָ�����������, ����ʾʱ������ͼ��canvas surface��Χ���ֻᱻ���е���
\param[in] pInfo Information for creating a window. Only the following pixel formats are supported:CNcomment:���ڴ�������Ϣ���������ظ�ʽֻ֧���������е����ظ�ʽ:
    HIGO_PF_CLUT8
    HIGO_PF_4444
    HIGO_PF_0444
    HIGO_PF_1555
    HIGO_PF_0555
    HIGO_PF_565
    HIGO_PF_8565
    HIGO_PF_8888
    HIGO_PF_0888
    The number of the layer where a window is located must range from 0 to 15. The greater the ID, the upper the 
position of the window.CNcomment:���д��ڲ��ֻ����0��15�ķ�Χ��(����0��15)����Ŵ�Ĵ�����Զ��ͼ���С������
    
\param[out] pWindow  Address for storing window handles. The value cannot be empty.CNcomment:�洢���ھ���ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVPF

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindowEx(const HIGO_WNDINFO_S* pInfo, HI_HANDLE* pWindow);

/**
\brief Creates a window on a specified graphics layer.CNcomment:��ָ��ͼ���ϴ�������
\attention \n
If the pixel format of a graphics layer contains pixel alpha, you can choose whether to use pixel alpha for the 
surface of the window.
CNcomment:��ͼ������ظ�ʽ��������alphaʱ�����ڵ�surface����ѡ���Ƿ�ʹ������alpha
\param[in] hLayer  Handle of the graphics layer.CNcomment:ͼ����
\param[in] pRect Rectangle window region. If the value is empty, it indicates the entire desktop region.CNcomment:���δ�������Ϊ�ձ�ʾ������������
\param[in] LayerNum Number of the layer where a window is located. The number ranges from 0 to 15.CNcomment:���������Ĳ��,���Ϊ0-15��
\param[in] bHasPixelAlpha Whether a window contains pixel alpha.CNcomment:�����Ƿ������alpha
\param[out] pWindow  Address for storing window handles. The value cannot be empty.CNcomment:�洢���ھ���ĵ�ַ������Ϊ��
\param[out] eBufferType Type of the surface buffer used by a window.CNcomment:windwoʹ��surface buffer������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_DestroyWindow
*/
HI_S32 HI_GO_CreateWindow(HI_HANDLE hLayer, const HI_RECT* pRect,
                          HI_U32 LayerNum, HI_BOOL bHasPixelAlpha,
                          HI_HANDLE* pWindow,HIGO_BUFFERTYPE_E eBufferType);

/**
\brief Destroys a window.CNcomment:���ٴ���
\attention \n
\param[in] Window Window handle to be destroyed.CNcomment:�����ٴ��ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM

\see \n
::HI_GO_CreateWindow
*/
HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window);

/**
\brief Obtains a window rectangle.CNcomment:��ȡ���ھ���
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[out] pRect Window rectangle address. The value cannot be empty.CNcomment:���ھ��ε�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetWindowPos \n
::HI_GO_ResizeWindow
*/
HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief Sets the start position of a window on the attached layer.CNcomment:���ô����ڰ󶨲��ϵ���ʼλ��
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[in] StartX Horizontal coordinate.CNcomment:X����
\param[in] StartY Vertical coordinate.CNcomment:Y����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

/**
\brief Changes the window size.CNcomment:�ı䴰�ڴ�С
\attention \n
After the window size is changed, you need to redraw the window.CNcomment:�ı䴰�ڴ�С�󣬴�����Ҫ�ػ�
\param[in] Window Window handle.CNcomment:���ھ��
\param[in] Width Window width.CNcomment:���ڿ��
\param[in] Height Window height.CNcomment: ���ڸ߶�

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVSURFACESIZE
\retval ::HIGO_ERR_INVSURFACEPF
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_NOMEM

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_ResizeWindow(HI_HANDLE Window, HI_S32 Width, HI_S32 Height);

/**
\brief Obtains the z-order of a window. The smaller the z-order, the lower the position of the window.CNcomment:��ȡ����Z��z��ֵС�Ĵ��ڱ�ֵ�󴰿ڸ��ǡ�
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[out] pZOrder Address for storing the window Z-order. The value cannot be empty.CNcomment:�洢����Z��ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_ChangeWindowZOrder
*/
HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32* pZOrder);


/**
\brief Changes the z-order of a window.CNcomment:���Ĵ���Z��
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[in] EnType Window z-order adjustment mode.CNcomment:����Z�������ʽ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVZORDERTYPE
\retval ::HIGO_ERR_LOCKED

\see \n
::HI_GO_GetWindowZOrder
*/
HI_S32 HI_GO_ChangeWindowZOrder(HI_HANDLE Window, HIGO_ZORDER_E EnType);

/**
\brief Obtains the window transparency.CNcomment:��ȡ���ڿɼ���
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[out] pOpacity Address for storing the information about window transparency. The value cannot be empty.CNcomment:�洢���ڲ�͸���ȵĵ�ַ������Ϊ�գ�255Ϊ��͸��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowOpacity
*/
HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8* pOpacity);

/**
\brief Sets the window transparency.CNcomment:���ô��ڲ�͸����
\attention \n
N/A.CNcomment:��
\param[in] Window Window handle.CNcomment:���ھ��
\param[in] Opacity Window transparency, ranging from 0 to 255. 0: opaque, 255: full transparent.CNcomment:���ڲ�͸���ȣ���ΧΪ0~255, ��Ϊ0ʱ���ô��ڲ��ɼ���255ʱ��������ȫ�ɼ�

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowOpacity
*/
HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

/**
\brief Sets the window colorkey.CNcomment:���ô��ڵ�colorkey
\attention \n
\param[in] Window Window handle.CNcomment:���ھ��
\param[in] ColorKey Colorkey value.CNcomment:Colorkeyֵ

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
N/A.CNcomment:��
*/
HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

/**
\brief Obtains the surface handle of a window.CNcomment:��ȡ���ڵ�surface���
\attention \n
If the window has dual buffers and you have called HI_GO_FlipWindowSurface, you need to call HI_GO_GetWindowSurface 
again to obtain another buffer for drawing.
CNcomment:�����˫buffer��window��HI_GO_FlipWindowSurface֮����Ҫ���µ��øýӿڣ��Ӷ���ȡ����һ��buffer���л���

\param[in] Window   Window handle.CNcomment:���ھ��
\param[in] pSurface Address for storing window surfaces. The value cannot be empty.CNcomment:�洢����surface����ĵ�ַ������Ϊ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_FlipWindowSurface \n
*/
HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE* pSurface);

/**
\brief Refreshes a window.CNcomment:ˢ�´���
\attention \n
After changing the window surface contents, you need to call the API for the changes to take effect.
CNcomment:���´���surface����֮����Ҫ���ô˽ӿڲ�����Ч

\param[in] Window Window handle.CNcomment:���ھ��
\param[in] pRect Updated window region.CNcomment:���µĴ�������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:��
*/
HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, const HI_RECT* pRect);

/**
\brief Changes the graphics layer to which a window belongs and retains the window.CNcomment:�޸Ĵ�������ͼ�㣬��ɾ������
\attention \n
N/A.CNcomment:��
\param[in] hWindow Window handle.CNcomment: ���ھ��
\param[in] hNewLayer  Handle of the new layer.CNcomment:��ͼ����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:��
*/
HI_S32 HI_GO_ChangeWindowLayer(HI_HANDLE hWindow, HI_HANDLE hNewLayer);

/**
\brief Switches the surface of a window. This function is valid only for the window that has double surfaces.CNcomment:�л�window ��surface������double surface��window��Ч
\attention \n
N/A.CNcomment:��
\param[in] hWindow Window handle.CNcomment: ���ھ��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:��
*/

HI_S32 HI_GO_FlipWindowSurface(HI_HANDLE hWindow);
    
/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_WINC_H__ */
