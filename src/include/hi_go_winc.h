
/**
 \file
 \brief Describes the header file of the window (Winc) module.CNcomment:Winc模块头文件
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
/** @{ */  /** <!—[Winc module] *//**CNcomment: <!-- 【Winc模块】 */

/**Mode of adjusting the window z-order*//**CNcomment:窗口Z序调整方式*/
typedef enum
{
    HIGO_ZORDER_MOVETOP = 0,  /**<Move to the top*//**CNcomment:<移到最顶部*/
    HIGO_ZORDER_MOVEUP,       /**<Move upwards*//**CNcomment:<向上移*/
    HIGO_ZORDER_MOVEBOTTOM,   /**<Move to the bottom*//**CNcomment:<移到最底部*/
    HIGO_ZORDER_MOVEDOWN,     /**<Move downwards*//**CNcomment:<向下移*/
    HIGO_ZORDER_BUTT
} HIGO_ZORDER_E;

/**Number of buffers used by the window*//**CNcomment:window使用buffer的方式*/
typedef enum
{
    HIGO_BUFFER_SINGLE = 0,  /**<The window uses a buffer.*//**CNcomment:<window使用单buffer*/
    HIGO_BUFFER_DOUBLE,     /**<The window uses dual buffers.*/ /**CNcomment:<window使用双buffer*/
    HIGO_BUFFER_BUTT
}HIGO_BUFFERTYPE_E;

/**Maximum number of window layers*//**CNcomment: 最大窗口层数 */
#define HIGO_WINTREE_NUM 16

typedef struct
{
    HI_HANDLE hLayer; /**<Graphics layer on which a window is displayed*//**CNcomment:<窗口显示的图层*/
    HI_RECT rect;     /**<Window range*//**CNcomment:<窗口的范围*/
    HI_U32 LayerNum;     /**<Display level of a window, ranging from 0 to 15. The greater the number, the upper the position of the window.*//**CNcomment:<窗口显示的级别,序号为0-15。数字越大的窗口越在上面*/         
    HIGO_PF_E PixelFormat;/**<Pixel format of a window*//**CNcomment:<窗口的像素格式*/
    HIGO_BUFFERTYPE_E BufferType; /**<Number of surface buffers required by a window*/ /**CNcomment:<窗口需要的surface buffer个数的设置*/ 
} HIGO_WNDINFO_S;
/** @} */  /*! <!-- Structure Definition end */


/******************************* API declaration *****************************/
/** \addtogroup      H_1_3_3_9 */
/** @{ */  /** <!—[Winc module]*//**CNcomment: <!-- 【Winc模块】 */

/**
\brief Creates a window on a specified graphics layer and sets the pixel format of a window.CNcomment:在指定图层上创建窗口
\attention \n
The new window is always located on the top of a specified graphics layer. The parts of the canvas surface that 
exceed the graphics layer are cropped.CNcomment:新创建的窗口始终在指定层的最上面, 当显示时，超出图层canvas surface范围部分会被剪切掉。
\param[in] pInfo Information for creating a window. Only the following pixel formats are supported:CNcomment:窗口创建的信息，其中像素格式只支持下面所列的像素格式:
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
position of the window.CNcomment:其中窗口层号只能在0到15的范围内(包括0和15)，层号大的窗口永远在图层号小的上面
    
\param[out] pWindow  Address for storing window handles. The value cannot be empty.CNcomment:存储窗口句柄的地址，不可为空

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
\brief Creates a window on a specified graphics layer.CNcomment:在指定图层上创建窗口
\attention \n
If the pixel format of a graphics layer contains pixel alpha, you can choose whether to use pixel alpha for the 
surface of the window.
CNcomment:当图层的像素格式带有像素alpha时，窗口的surface可以选择是否使用像素alpha
\param[in] hLayer  Handle of the graphics layer.CNcomment:图层句柄
\param[in] pRect Rectangle window region. If the value is empty, it indicates the entire desktop region.CNcomment:矩形窗口区域，为空表示整个桌面区域
\param[in] LayerNum Number of the layer where a window is located. The number ranges from 0 to 15.CNcomment:窗口所处的层号,序号为0-15。
\param[in] bHasPixelAlpha Whether a window contains pixel alpha.CNcomment:窗口是否带像素alpha
\param[out] pWindow  Address for storing window handles. The value cannot be empty.CNcomment:存储窗口句柄的地址，不可为空
\param[out] eBufferType Type of the surface buffer used by a window.CNcomment:windwo使用surface buffer的设置

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
\brief Destroys a window.CNcomment:销毁窗口
\attention \n
\param[in] Window Window handle to be destroyed.CNcomment:待销毁窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVPARAM

\see \n
::HI_GO_CreateWindow
*/
HI_S32 HI_GO_DestroyWindow(HI_HANDLE Window);

/**
\brief Obtains a window rectangle.CNcomment:获取窗口矩形
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[out] pRect Window rectangle address. The value cannot be empty.CNcomment:窗口矩形地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_SetWindowPos \n
::HI_GO_ResizeWindow
*/
HI_S32 HI_GO_GetWindowRect(HI_HANDLE Window, HI_RECT* pRect);

/**
\brief Sets the start position of a window on the attached layer.CNcomment:设置窗口在绑定层上的起始位置
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] StartX Horizontal coordinate.CNcomment:X坐标
\param[in] StartY Vertical coordinate.CNcomment:Y坐标

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowRect
*/
HI_S32 HI_GO_SetWindowPos(HI_HANDLE Window, HI_S32 StartX, HI_S32 StartY);

/**
\brief Changes the window size.CNcomment:改变窗口大小
\attention \n
After the window size is changed, you need to redraw the window.CNcomment:改变窗口大小后，窗口需要重绘
\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] Width Window width.CNcomment:窗口宽度
\param[in] Height Window height.CNcomment: 窗口高度

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
\brief Obtains the z-order of a window. The smaller the z-order, the lower the position of the window.CNcomment:获取窗口Z序，z序值小的窗口被值大窗口覆盖。
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[out] pZOrder Address for storing the window Z-order. The value cannot be empty.CNcomment:存储窗口Z序的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_ChangeWindowZOrder
*/
HI_S32 HI_GO_GetWindowZOrder(HI_HANDLE Window, HI_U32* pZOrder);


/**
\brief Changes the z-order of a window.CNcomment:更改窗口Z序
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] EnType Window z-order adjustment mode.CNcomment:窗口Z序调整方式

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
\brief Obtains the window transparency.CNcomment:获取窗口可见度
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[out] pOpacity Address for storing the information about window transparency. The value cannot be empty.CNcomment:存储窗口不透明度的地址，不可为空，255为不透明

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_SetWindowOpacity
*/
HI_S32 HI_GO_GetWindowOpacity(HI_HANDLE Window, HI_U8* pOpacity);

/**
\brief Sets the window transparency.CNcomment:设置窗口不透明度
\attention \n
N/A.CNcomment:无
\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] Opacity Window transparency, ranging from 0 to 255. 0: opaque, 255: full transparent.CNcomment:窗口不透明度，范围为0~255, 当为0时，该窗口不可见，255时，窗口完全可见

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
::HI_GO_GetWindowOpacity
*/
HI_S32 HI_GO_SetWindowOpacity(HI_HANDLE Window, HI_U8 Opacity);

/**
\brief Sets the window colorkey.CNcomment:设置窗口的colorkey
\attention \n
\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] ColorKey Colorkey value.CNcomment:Colorkey值

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_LOCKED

\see \n
N/A.CNcomment:无
*/
HI_S32 HI_GO_SetWindowColorkey(HI_HANDLE Window, HI_COLOR ColorKey);

/**
\brief Obtains the surface handle of a window.CNcomment:获取窗口的surface句柄
\attention \n
If the window has dual buffers and you have called HI_GO_FlipWindowSurface, you need to call HI_GO_GetWindowSurface 
again to obtain another buffer for drawing.
CNcomment:如果是双buffer的window，HI_GO_FlipWindowSurface之后需要重新调用该接口，从而获取另外一块buffer进行绘制

\param[in] Window   Window handle.CNcomment:窗口句柄
\param[in] pSurface Address for storing window surfaces. The value cannot be empty.CNcomment:存储窗口surface句柄的地址，不可为空

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_NULLPTR

\see \n
::HI_GO_FlipWindowSurface \n
*/
HI_S32 HI_GO_GetWindowSurface(HI_HANDLE Window, HI_HANDLE* pSurface);

/**
\brief Refreshes a window.CNcomment:刷新窗口
\attention \n
After changing the window surface contents, you need to call the API for the changes to take effect.
CNcomment:更新窗口surface内容之后需要调用此接口才能生效

\param[in] Window Window handle.CNcomment:窗口句柄
\param[in] pRect Updated window region.CNcomment:更新的窗口区域

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无
*/
HI_S32 HI_GO_UpdateWindow(HI_HANDLE Window, const HI_RECT* pRect);

/**
\brief Changes the graphics layer to which a window belongs and retains the window.CNcomment:修改窗口所属图层，不删除窗口
\attention \n
N/A.CNcomment:无
\param[in] hWindow Window handle.CNcomment: 窗口句柄
\param[in] hNewLayer  Handle of the new layer.CNcomment:新图层句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无
*/
HI_S32 HI_GO_ChangeWindowLayer(HI_HANDLE hWindow, HI_HANDLE hNewLayer);

/**
\brief Switches the surface of a window. This function is valid only for the window that has double surfaces.CNcomment:切换window 的surface，仅对double surface的window有效
\attention \n
N/A.CNcomment:无
\param[in] hWindow Window handle.CNcomment: 窗口句柄

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_INVHANDLE

\see \n
N/A.CNcomment:无
*/

HI_S32 HI_GO_FlipWindowSurface(HI_HANDLE hWindow);
    
/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_WINC_H__ */
