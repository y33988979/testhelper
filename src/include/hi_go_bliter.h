
/**
 \file
 \brief Describes the header file of the bit block transfer (Bliter) module.CNcomment:bliterͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author s37678
 \date 2008-5-23
 */

#ifndef __HI_GO_BLITER_H__
#define __HI_GO_BLITER_H__

#include "hi_go_comm.h"
#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/


/****************************** Error Number ********************************/


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_3_3_1 */
/** @{ */  /** <!-- ��bliter�� */

/** Pixel-level alpha blending operation*/
/**CNcomment: ���ؼ�alpha��ϲ������� */
typedef enum
{
    HIGO_COMPOPT_NONE = 0, /**<Porter/Duff None-blending operation*//**CNcomment:< Porter/Duff None��ϲ��� */
    HIGO_COMPOPT_CLEAR,    /**<Porter/Duff clear blending operation*//**CNcomment:< Porter/Duff clear��ϲ��� */
    HIGO_COMPOPT_SRC     , /**<Porter/Duff SRC blending operation*//**CNcomment:< Porter/Duff Src��ϲ��� */ 
    HIGO_COMPOPT_SRCOVER , /**<Porter/Duff Srcover blending operation*//**CNcomment:< Porter/Duff Srcover��ϲ��� */ 
    HIGO_COMPOPT_DSTOVER , /**<Porter/Duff DstOver blending operation*/ /**CNcomment:< Porter/Duff DstOver��ϲ��� */ 
    HIGO_COMPOPT_SRCIN   , /**<Porter/Duff SrcIn blending operation*//**CNcomment:< Porter/Duff SrcIn��ϲ��� */ 
    HIGO_COMPOPT_DSTIN   , /**<Porter/Duff DstIn blending operation*/ /**CNcomment:< Porter/Duff DstIn��ϲ��� */ 
    HIGO_COMPOPT_SRCOUT  , /**<Porter/Duff SrcOut blending operation*//**CNcomment:< Porter/Duff SrcOut��ϲ��� */ 
    HIGO_COMPOPT_DSTOUT  , /**<Porter/Duff DstOut blending operation*/ /**CNcomment:< Porter/Duff DstOut��ϲ��� */ 
    HIGO_COMPOPT_SRCATOP , /**<Porter/Duff SrcAtop blending operation*/ /**CNcomment:< Porter/Duff SrcAtop��ϲ��� */ 
    HIGO_COMPOPT_DSTATOP , /**<Porter/Duff DstAtop blending operation*//**CNcomment:< Porter/Duff DstAtop��ϲ��� */ 
    HIGO_COMPOPT_ADD     ,  /**<Porter/Duff DstAtop blending operation*/ /**CNcomment:< Porter/Duff DstAtop��ϲ��� */ 
    HIGO_COMPOPT_XOR     , /**<Porter/Duff Xor blending operation*/ /**CNcomment:< Porter/Duff Xor��ϲ��� */ 
    HIGO_COMPOPT_DST     , /**<Porter/Duff DstAtop blending operation*/ /**CNcomment:< Porter/Duff DstAtop��ϲ��� */ 
    HIGO_COMPOPT_AKS,      /**<Assume that the destination surface is not transparent. After the alpha blending is performed, the source alpha is retained.*//**CNcomment:< ����Ŀ��surfaceΪ��͸������alpha��ϣ��������Դalpha */
    HIGO_COMPOPT_AKD,      /**<Assume that the destination surface is not transparent. After the alpha blending is performed, the destination alpha is retained.*//**CNcomment:< ����Ŀ��surfaceΪ��͸������alpha��ϣ��������Ŀ��alpha */

    HIGO_COMPOPT_BUTT
} HIGO_COMPOPT_E;

/** Operation mode corresponding to colorkey*//**CNcomment: Colorkey��Ӧ�Ĳ�����ʽ */
typedef enum
{
    HIGO_CKEY_NONE = 0, /**<Do not use the colorkey.*//**CNcomment:< ��ʹ��colorkey */
    HIGO_CKEY_SRC,      /**<Use the source colorkey.*//**CNcomment:< ʹ��Դcolorkey */
    HIGO_CKEY_DST,     /**<Use the destination colorkey.*//**CNcomment:< ʹ��Ŀ��colorkey */

    HIGO_CKEY_BUTT
} HIGO_CKEY_E;

/** Two raster of operations (ROPs)*//**CNcomment: 2ԪROP���� */
typedef enum
{
    HIGO_ROP_BLACK = 0, /**< Blackness */
    HIGO_ROP_PSDon,     /**< ~(PS+D) */
    HIGO_ROP_PSDna,     /**< ~PS & D */
    HIGO_ROP_PSn,       /**< ~PS */
    HIGO_ROP_DPSna,     /**< PS & ~D */
    HIGO_ROP_Dn,        /**< ~D */
    HIGO_ROP_PSDx,      /**< PS^D */
    HIGO_ROP_PSDan,     /**< ~(PS&D) */
    HIGO_ROP_PSDa,      /**< PS & D */
    HIGO_ROP_PSDxn,     /**< ~(PS^D) */
    HIGO_ROP_D,         /**< D */
    HIGO_ROP_PSDno,     /**< ~PS + D */
    HIGO_ROP_PS,        /**< PS */
    HIGO_ROP_DPSno,     /**< PS + ~D */
    HIGO_ROP_PSDo,      /**< PS+D */
    HIGO_ROP_WHITE,     /**< Whiteness */

    HIGO_ROP_BUTT
} HIGO_ROP_E;

typedef enum
{
    HIGO_ROTATE_NONE = 0,
    HIGO_ROTATE_90,     /**<Rotate 90�� clockwise*//**CNcomment:< ˳ʱ����ת90�� */
    HIGO_ROTATE_180,    /**<Rotate 180�� clockwise*//**CNcomment:< ˳ʱ����ת180�� */
    HIGO_ROTATE_270,    /**<Rotate 270�� clockwise*//**CNcomment:< ˳ʱ����ת270�� */

    HIGO_ROTATE_BUTT
} HIGO_ROTATE_E;

typedef enum
{
    HIGO_MIRROR_NONE = 0,

    HIGO_MIRROR_LR,     /**<Mirror the left and the right*//**CNcomment:< ���Ҿ��� */
    HIGO_MIRROR_TB,     /**<Mirror the top and the bottom*//**CNcomment:< ���¾��� */

    HIGO_MIRROR_BUTT
} HIGO_MIRROR_E;

typedef struct 
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag*//**CNcomment:< ȫ��alphaʹ�ܱ�־ */
    HI_BOOL EnablePixelAlpha;       /**<Pixel alpha enable flag*//**CNcomment:< ����alphaʹ�ܱ�־ */
    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**CNcomment:< ����alpha���� */
    HIGO_CKEY_E    ColorKeyFrom;    /**<Colorkey operation*//**CNcomment:< ColorKey���� */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**CNcomment:< ����ROP2���� */
    HIGO_ROP_E     Rop;              /**<ROP2 operation type*//**CNcomment:< ROP2�������� */
    HIGO_ROP_E     RopAlpha;         /**<Type of the ROP alpha operation*//**CNcomment:< ROP alpha��������*/
 }HIGO_BLTOPT2_S;

/**CNcomment: blit�������� */
typedef struct
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag. If this flag is enabled, the PixelAlphaComp blending mode must be specified.*//**CNcomment:< ȫ��alphaʹ�ܱ�־,�򿪴˿���֮�⻹����ָ��PixelAlphaComp�Ļ�Ϸ�ʽ*/
    HI_BOOL EnablePixelAlpha;       /**<Pixel alpha enable flag*//**CNcomment:< ����alphaʹ�ܱ�־ */
    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**CNcomment:< ����alpha���� */
    HIGO_CKEY_E    ColorKeyFrom;    /**<colorkey operation*//**CNcomment:< ColorKey���� */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**CNcomment:< ����ROP2���� */
    HIGO_ROP_E     Rop;             /**<ROP2 operation type*//**CNcomment:< ROP2�������� */
    HIGO_ROP_E     RopAlpha;        /**<Type of the ROP alpha operation*/    /**CNcomment:< ROP alpha��������*/    
    HI_BOOL        EnableScale;      /**<Enable the scaling function*//**CNcomment:< �������� */
    HIGO_ROTATE_E  RotateType;      /**<Rotation type*//**CNcomment:< ��ת��ʽ */
    HIGO_MIRROR_E  MirrorType;      /**<Mirror type*//**CNcomment:< ����ʽ */
} HIGO_BLTOPT_S;

typedef struct
{
    HI_BOOL EnableGlobalAlpha;      /**<Global alpha enable flag*//**CNcomment:< ȫ��alphaʹ�ܱ�־ */

    HIGO_COMPOPT_E PixelAlphaComp;  /**<Pixel alpha operation*//**CNcomment:< ����alpha���� */
    HI_BOOL        EnableRop;       /**<Enable the ROP2 operation*//**CNcomment:< ����ROP2���� */
    HIGO_ROP_E     RopColor;        /**<ROP2 operation type*//**CNcomment:< ROP2�������� */
	HIGO_ROP_E     RopAlpha;
		
} HIGO_MASKOPT_S;

/**Anti-flicker level*//**CNcomment: ����˸���� */
typedef enum
{
    HIGO_DEFLICKER_AUTO = 0, /*Anti-flicker level, ranging from low to high. The higher the level, the better the anti-flicker effect, but the more blurred the picture.*//**CNcomment:����˸����ֵΪLOW~HIGH,ֵԽ����˸Ч��Խ�ã���Խģ��*/
    HIGO_DEFLICKER_LOW,
    HIGO_DEFLICKER_MIDDLE,
    HIGO_DEFLICKER_HIGH,
    HIGO_DEFLICKER_BUTT
}HIGO_DEFLICKEROPT_E;

/**Anti-flicker level*//**CNcomment: ����˸���� */
typedef struct
{
    HIGO_DEFLICKEROPT_E DefLevel;
}HIGO_DEFLICKEROPT_S;
/** @} */  /*! <!-- Structure Definition end */

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/
/** \addtogroup      H_1_3_3_1 */
/** @{ */  /** <!-- ��bliter�� */

/** 
\brief Initializes the Bliter module.CNcomment: ��ʼ��Bliterģ��
\attention \n
When ::HI_GO_Init is called, this application programming interface (API) is also called.CNcomment: ::HI_GO_Init���Ѱ����˶Ըýӿڵĵ���
\param  N/A.CNcomment: ��

\retval ::HI_SUCCESS  
\retval ::HI_FAILURE
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
::HI_GO_Init \n
::HI_GO_DeinitBliter
*/
HI_S32 HI_GO_InitBliter(HI_VOID);

/**CNcomment:  
\brief Deinitializes the Bliter module.CNcomment:ȥ��ʼ��Bliterģ��
\attention \n
When ::HI_GO_Deinit is called, this API is also called.CNcomment:::HI_GO_Deinit���Ѱ����˶Ըýӿڵĵ���
\param N/A. CNcomment:��

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT

\see \n
::HI_GO_Deinit \n
::HI_GO_InitBliter
*/

HI_S32 HI_GO_DeinitBliter(HI_VOID);

/**  
\brief Fills in a rectangle.CNcomment:�������
\attention \n
N/A.CNcomment:��
\param[in] Surface Surface handle.CNcomment:Surface���
\param[in] pRect Size of the rectangle to be filled in. If the parameter is not set, it indicates that the entire 
surface is filled in.CNcomment: �����δ�С��Ϊ�ձ�ʾ�������surface
\param[in] Color Color Fill color. For the RGB format, 32-bit color is filled in; for the palette, the color index (0-255) 
is filled in.CNcomment:�����ɫ,ע�������RGB��ʽ��ͳһ���32λɫ�������ɫ�壬��ֱ�������ɫ����(0 ~ 255)��
\param[in] CompositeOpt Blending mode.CNcomment:��Ϸ�ʽ

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_OUTOFPAL
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_FillRect(HI_HANDLE Surface, const HI_RECT* pRect, HI_COLOR Color, HIGO_COMPOPT_E CompositeOpt);

/**  
\brief Transfers bit blocks. During the transfer, color space conversion (CSC), scaling, and rotation are supported.CNcomment:λ����ƣ��ڰ��ƹ����У�����ʵ��ɫ�ʿռ�ת�������š���ת����
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
Scaling, rotation, and mirror cannot be combined. \n
For scaling, rotation, and mirror, the source and destination pixel formats must be the same, but the format cannot 
be YUV or CLUT.\n
CNcomment:ɫ�ʿռ�ת����֧��YUV��RGBת�� \n
����֧�ֲ�������colorkey��alpha��ROP��colorkey+ROP\
���š���ת���񲻿����ʹ�� \n
���š���ת����Ҫ��Դ��Ŀ�����ظ�ʽ��ȫ��ͬ����������YUV��ʽ��CLUT��ʽ \n

\param[in] SrcSurface Source surface handle.CNcomment:Դsurface���
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:���Ƶ�Դ����Ϊ�ձ�ʾ����Դsurface����
\param[in] DstSurface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pBlitOpt Blending mode.CNcomment:��Ϸ�ʽ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_Blit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                   HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                   const HIGO_BLTOPT_S* pBlitOpt);

/**  
\brief Draws a line segment. Cropping is not supported.CNcomment:�����߶Σ���֧�ֲü�
\attention \n
Cropping is not supported. Users must keep the entire line within the surface region.
CNcomment:��֧�ֲü���ʹ���߱��뱣֤����ֱ����surface������

\param[in] Surface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] x0 Horizontal coordinate of the start point.CNcomment:���x����
\param[in] y0 Vertical coordinate of the start point.CNcomment:���y����
\param[in] x1 Horizontal coordinate of the end point.CNcomment:�յ�x����
\param[in] y1 Vertical coordinate of the end point .CNcomment:�յ�y���� 
\param[in] color Line segment color.CNcomment:�߶���ɫ

\retval ::HI_SUCCESS 
\retval ::HI_FAILURE
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_DrawLine(HI_HANDLE Surface, HI_S32 x0, HI_S32 y0, HI_S32 x1, HI_S32 y1, HI_COLOR color);

/**  
\brief Draws an ellipse.CNcomment:������Բ
\attention \n
Cropping is not supported. Users must keep the entire ellipse within the surface region.
CNcomment:��֧�ֲü���ʹ���߱��뱣֤������Բ��surface������

\param[in] Surface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] sx Horizontal coordinate of the ellipse center.CNcomment:Բ��x����
\param[in] sy Vertical coordinate of the ellipse center.CNcomment:Բ��y����
\param[in] rx X axis radius .CNcomment:x��뾶 
\param[in] ry Y axis radius.CNcomment:y��뾶
\param[in] color Ellipse color.CNcomment:��Բ��ɫ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_DrawEllipse(HI_HANDLE Surface, HI_S32 sx, HI_S32 sy, HI_S32 rx, HI_S32 ry, HI_COLOR color);

/**  
\brief Draws a circle. Cropping is not supported.CNcomment:����Բ����֧�ֲü�
\attention \n
Cropping is not supported. Users must keep the entire circle within the surface region.
CNcomment:��֧�ֲü���ʹ���߱��뱣֤����Բ��surface������

\param[in] Surface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] x Horizontal coordinate of the circle center.CNcomment:Բ��x����
\param[in] y Vertical coordinate of the circle center.CNcomment:Բ��y����
\param[in] r Radius.CNcomment:�뾶
\param[in] color Circle color. The circle is filled in with 32-bit colors.CNcomment:Բ��ɫ, ��ɫ����32λɫ�������

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_DrawCircle(HI_HANDLE Surface, HI_S32 x, HI_S32 y, HI_S32 r, HI_COLOR color);

/**  
\brief Draws a rectangle.CNcomment:���ƾ���
\attention \n
If the rectangle is empty, it indicates that the rectangle size is the same as surface.
Cropping is not supported; therefore, you must ensure that the entire rectangle is within the surface.
CNcomment:����Ϊ�ձ�ʾ������δ�С��surface��ͬ
��֧�ֲü���ʹ���߱��뱣֤����������surface������

\param[in] Surface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] pRect Rectangle region.CNcomment:��������
\param[in] color Rectangle color.CNcomment:������ɫ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_UNSUPPORTED

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_DrawRect(HI_HANDLE Surface, const HI_RECT* pRect, HI_COLOR color);

/**  
\brief Performs the mask ROP or mask blend transfer on the raster bitmap.CNcomment:�Թ�դλͼ����Mask Rop����Mask Blend���Ʋ���
\attention \n
The mask bitmap is in A1 or A8 format.
You need to overlay the mask bitmap with the source bitmap, overlay the result with the destination bitmap, and then 
output the final result to the destination bitmap.
If the ROP and blending operation are perform at the same time, only the ROP takes effect.
CNcomment:Mask��A1��A8��λͼSurface��
���Ƚ�Maskλͼ��Դλͼ��һ�ε��ӣ�Ȼ���õ��ӵĽ����Ŀ�������������Ŀ��λͼ
Rop��Blend���ͬʱֻ����һ������Ч�ģ����߶�ѡֻ��Rop��Ч

\param[in] SrcSurface Source surface handle.CNcomment:Դsurface���
\param[in] pSrcRect Source rectangle.CNcomment:Դ����
\param[in] DstSurface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] pDstRect Destination rectangle.CNcomment:Ŀ�����
\param[in] MaskSurface Mask surface handle.CNcomment:MASK surface���
\param[in] pMaskRect Mask rectangle.CNcomment:MASK����
\param[in] pOpt Operation option.CNcomment:����ѡ��

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVRECT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:��
*/

HI_S32 HI_GO_MaskBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                      HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                      HI_HANDLE MaskSurface, const HI_RECT* pMaskRect,
                      const HIGO_MASKOPT_S* pOpt);

/**  
\brief Transfers bit blocks. During the transfer, CSC is supported.CNcomment:λ����ƣ��ڰ��ƹ����У�����ʵ��ɫ�ʿռ�ת��
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
CNcomment: ɫ�ʿռ�ת����֧��YUV��RGBת�� \n
����֧�ֲ�������colorkey��alpha��ROP��colorkey+ROP\

\param[in] SrcSurface Source surface handle.CNcomment:Դsurface���
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:���Ƶ�Դ����Ϊ�ձ�ʾ����Դsurface����
\param[in] DstSurface Destination surface handle.CNcomment:Ŀ��surface���
\param[in] pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used. CNcomment:��Ϸ�ʽ������Ϊ��ʹ��Ĭ�ϲ�������

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_BitBlit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                       HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                       const HIGO_BLTOPT2_S* pBlitOpt);

/**  
\brief Transfers bit blocks. During the transfer, CSC and scaling are supported. CNcomment:λ����ƣ��ڰ��ƹ����У�����ʵ��ɫ�ʿռ�ת��, ����
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported.\
CNcomment:ɫ�ʿռ�ת����֧��YUV��RGBת�� \n
����֧�ֲ�������colorkey��alpha��ROP��colorkey+ROP\

\param[in] SrcSurface  Source surface handle. CNcomment:Դsurface���
\param[in] pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:���Ƶ�Դ����Ϊ�ձ�ʾ����Դsurface����
\param[in] DstSurface Destination surface handle. CNcomment:Ŀ��surface���
\param[in] pDstRect pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface. CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pBlitOpt Blending mode. If the parameter is not set, default settings are used. CNcomment:��Ϸ�ʽ,����Ϊ��ʹ��Ĭ�ϲ�������

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NOMEM
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_StretchBlit (HI_HANDLE SrcSurface, const HI_RECT* pSrcRect,
                       HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                       const HIGO_BLTOPT2_S* pBlitOpt);

/** 
\brief  Fills in the pattern.CNcomment:����ģʽ���
\attention \n
\param[in] SrcSurface  Source surface handle. CNcomment:Դsurface���
\param[in] pSrcRect  Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:���Ƶ�Դ����Ϊ�ձ�ʾ����Դsurface����
\param[in] DstSurface  Destination surface handle. CNcomment:Ŀ��surface���
\param[in] pDstRect  Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface.CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pParOpt  Blending mode. If the parameter is not set, default settings are used. CNcomment:��Ϸ�ʽ,����Ϊ��ʹ��Ĭ�ϲ�������

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_PatternBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect, 
                              HI_HANDLE DstSurface, const HI_RECT * pDstRect, 
                              const HIGO_BLTOPT2_S* pParOpt);

/** 
\brief Performs anti-flicker and transfer. This API is used for the second anti-flicker.CNcomment:����˸���ƣ�����ӿ����ڵڶ��ο���˸��
\attention \n
This API is used in the following two application scenarios:
1. The automatic anti-flicker effect of a graphics layer is poor.
2. The buffer mode of a graphics layer is single-buffer mode (HIGO_LAYER_BUFFER_SINGLE). In this case, users need to 
perform anti-flicker by themselves.
When the sizes of the source bitmap and destination bitmap are different, scaling is performed automatically.
CNcomment:��������õ��ýӿ�
1 ��ͼ����Զ�����˸Ч�����Ǻܺ�ʱ������ʹ������ӿ�����һ�Ρ�
2 ��ͼ���bufferģʽ�ǵ�bufferģʽ(HIGO_LAYER_BUFFER_SINGLE)ʱ���û���Ҫ�Լ�������˸
��Դ��Ŀ���С��ͬ��ʱ���Զ��������š�

\param[in] SrcSurface  SrcSurface Source surface handle.CNcomment: Դsurface���
\param[in] pSrcRect  pSrcRect Source region for transfer. If the parameter is not set, it indicates the entire source surface.CNcomment:���Ƶ�Դ����Ϊ�ձ�ʾ����Դsurface����
\param[in] DstSurface  DstSurface Destination surface handle. CNcomment:Ŀ��surface���
\param[in] pDstRect  pDstRect Destination region for transfer. If the parameter is not set, it indicates the entire destination 
surface. CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pDefOpt  pDefOpt Anti-flicker level. If this parameter is not set, it indicates the automatic level. CNcomment:����˸����ѡ�Ϊ�ձ�ʾAUTO ����

\retval ::HI_SUCCESS
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPARAM
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_DeflickerBlit(HI_HANDLE SrcSurface, const HI_RECT* pSrcRect, 
                                 HI_HANDLE DstSurface, const HI_RECT * pDstRect, 
                                 const HIGO_DEFLICKEROPT_S* pDefOpt);


/**  
\brief Do the alpha blending between the forground surface and the backgroud surface, transfers the result to the destination surface. The background surface and the destination surface must be in the RGB color space. 
You can performs color space coversion, scale, or mirror, colorkey with alpha blending.CNcomment:�ú���ʵ��ǰ���ͱ������������Ŀ�깦�ܣ�������Ŀ��surface����λ��RGB�ռ䣬�ڵ��ӹ����п��Ը�����ɫ�ռ�ת�������š�����Ȳ�������֧����ת��
\attention \n
Only the YUV-to-RGB CSC is supported. \n
The operations of colorkey, alpha, ROP, and colorkey+ROP are supported, rotation is not supported.\
Scaling, and mirror cannot be combined. \n
For scaling, and mirror, the source and destination pixel formats must be the same, but the format cannot 
be YUV or CLUT.\n
CNcomment:ɫ�ʿռ�ת����֧��YUV��RGBת�� \n
����֧�ֲ�������colorkey��alpha��ROP��colorkey+ROP����֧������\
���š����񲻿����ʹ�� \n
���Ż���Ҫ��Դ��Ŀ�����ظ�ʽ��ȫ��ͬ����������YUV��ʽ��CLUT��ʽ \n

\param[in] BckSurface background surface handle.CNcomment:����surface���
\param[in] pBckRect backgound region for transfer. If the parameter is not set, it indicates the entire background surface.CNcomment:����surface��������Ϊ�ձ�ʾ��������surface����
\param[in] ForSurface forground surface handle.CNcomment:ǰ��surface���
\param[in] pForRect forground region for transfer. If the parameter is not set, it indicates the entire forground surface. 
surface.CNcomment:ǰ��surface��������Ϊ�ձ�ʾ����ǰ��surface����
\param[in] DstSurface destination surface handle.CNcomment:Ŀ��surface���
\param[in] pDstRect destination region for transfer. If the parameter is not set, it indicates the entire destination surface. 
surface.CNcomment:���Ƶ�Ŀ������Ϊ�ձ�ʾ����Ŀ��surface����
\param[in] pBlitOpt Blending mode.CNcomment:��Ϸ�ʽ

\retval ::HI_SUCCESS 
\retval ::HIGO_ERR_NOTINIT
\retval ::HIGO_ERR_INVMIRRORTYPE
\retval ::HIGO_ERR_INVROTATETYPE
\retval ::HIGO_ERR_INVCKEYTYPE
\retval ::HIGO_ERR_INVROPTYPE
\retval ::HIGO_ERR_NOCOLORKEY
\retval ::HIGO_ERR_INVPIXELFMT
\retval ::HIGO_ERR_INTERNAL
\retval ::HIGO_ERR_NULLPTR
\retval ::HIGO_ERR_INVHANDLE
\retval ::HIGO_ERR_INVCOMPTYPE
\retval ::HIGO_ERR_UNSUPPORTED
\retval ::HIGO_ERR_LOCKED
\retval ::HIGO_ERR_DEPEND_TDE
\retval ::HIGO_ERR_EMPTYRECT
\retval ::HIGO_ERR_OUTOFBOUNDS
\retval ::HIGO_ERR_INTERNAL

\see \n
N/A. CNcomment:��
*/
HI_S32 HI_GO_Blit3Source(HI_HANDLE BckSurface, const HI_RECT* pBckRect,
                   HI_HANDLE ForSurface, const HI_RECT* pForRect,
                   HI_HANDLE DstSurface, const HI_RECT* pDstRect,
                   const HIGO_BLTOPT_S* pBlitOpt);


/** @} */  /*! <!-- API declaration end */

#ifdef __cplusplus
}
#endif
#endif /* __HI_GO_BLITER_H__ */
