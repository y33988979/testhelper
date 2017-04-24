/**
 \file
 \brief Describes the header file of the font module. CNcomment:HiFont ����ӿ�ͷ�ļ�
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2011-2018
 \version 1.0
 \date 2011-8-25
 */
 
#ifndef _HI_FONT_H
#define _HI_FONT_H

#include "hi_type.h"
#ifndef WIN32

#ifdef __cplusplus
extern "C" {
#endif

/**Common err code of font module*/
/**CNcomment:HiFont ���������� */
typedef enum
{
    HI_FONT_ERRNOTINIT = 0,       /**The module is not initialized*//**CNcomment:δ��ʼ��*/
    HI_FONT_ERRINITFAILED,	  /**The module fails to be initialized*//**CNcomment:��ʼ��ʧ��*/
    HI_FONT_ERRDEINITFAILED,    /**The module fails to be deinitialized*//**CNcomment:ȥ��ʼ��ʧ��*/
    HI_FONT_ERRNULLPTR,		  /**The input pointer is null*//**CNcomment:��ָ�����*/
    HI_FONT_ERRINVHANDLE,	  /**The input handle is invalid*//**CNcomment:��Ч�������*/
    HI_FONT_ERRNOMEM,		  /**The memory is insufficient*//**CNcomment:�ڴ治��*/
    HI_FONT_ERRINTERNAL,
    HI_FONT_ERRINVFILE,            /**The file operation fails because the file is invalid*//**CNcomment:��Ч�ļ�·��*/
    HI_FONT_ERRINVPARAM,		  /**The parameter is invalid*//**CNcomment:��Ч����*/		
    HI_FONT_ERRUNSUPPORTED,   /**The operation is invalid*//**CNcomment:��֧�ֲ���*/
    HI_FONT_ERRMMAP,		   /**Mem-mapping fails*//**CNcomment:�ڴ�ӳ��ʧ��*/
    HI_FONT_ERRGETCHARINDEX, /**Obtaining char index fails*//**CNcomment:��ȡ�ַ�����ʧ��*/
    HI_FONT_ERRUNKNOWCHARSET,  /**Unknown charset*//**CNcomment:δ֪�ַ���*/
    HI_FONT_ERRBUTT
} HI_FONT_ERRE;




typedef struct tagGlyphMetrics {
	HI_U8 boxWidth;	 /*Bitmap width*//*CNcomment:λͼ��ռ�Ŀ��*/
	HI_U8 boxHeight;  /*Bitmap height*//*CNcomment:λͼ��ռ�ĸ߶�*/
	HI_S8 posX;  	 /*Horizon offset between character and bitmap*//*CNcomment:��������ռ��λ�õ�λͼ�� X�����ϵ�ƫ��*/
	HI_S8 posY;  	 /*Vertical offset between character and bitmap*//*CNcomment:��������ռ��λ�õ�λͼ�� Y �����ϵ�ƫ��*/
	HI_U8 incX; 	/*Horizon offset after drawing bitmap*//*CNcomment:�����λͼ֮�� X �����ϵ�ƫ��*/
	HI_U8 incY; 	/*Vertical offset after drawing bitmap*//*CNcomment:�����λͼ֮�� Y �����ϵ�ƫ��*/
} GlyphMetrics;





/** 
\brief Initialize font beforing calling other font API.  
CNcomment:�����ļ���ʼ��, ��ʹ���ֿ�ӿ�ǰ��ʼ���ֿ�.
\attention \n
ASCII charset file and GB2312 charset file cannot be empty.
GB2312 charset doesn't contain ascii character.
CNcomment:ASCII �ֿ��ļ�·����GB2312�ֿ��ļ�·��������Ϊ�գ�
GB2312�ֿ��в�����ascii �ַ���

\param[in] pAsciiFontFile    ASCII charset file path. CNcomment:ASCII�ֿ��ļ�·��   
\param[in] pGb2312FontFile  GB2312 charset file path. CNcomment:GB2312�ֿ��ļ�·��

\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVFILE
\retval ::HI_FONT_ERRINITFAILED
\see \n
N/A
*/
HI_S32 HI_BitmapFont_Init( HI_CHAR*  pAsciiFontFile,  HI_CHAR* pGb2312FontFile);



/** 
\brief Denitialize font to release resources after calling other font API. 
CNcomment:����ȥ��ʼ��,��ʹ�����ֿ�ӿں�ر��ļ�,�ͷ���Դ.
\attention \n
\param[in] 
\retval ::HI_SUCCESS 
\see \n
N/A
*/
HI_S32 HI_BitmapFont_DeInit();


/*********************************************************************************************/

/** 
\brief Obtains character model info of queried character. 
CNcomment:��ȡ����ѯ�ַ�����ģ��Ϣ.
\attention \n
str contains only one queried character. The first character will take effect if there are
more than one character.
CNcomment:str һ��ֻ����һ������ѯ�ַ�.����һ���ַ�ֻȡ��һ����
\paam[in]  str Queried character. ����ѯ���ַ�
\param[out]  pData Pointer to character's address in character lib. CNcomment:��ָ����ָ���ֵΪ���ַ����ֿ��е��׵�ַ
\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVPARAM
\retval ::HI_FONT_ERRUNKNOWCHARSET
\see \n
N/A
*/
HI_S32 HI_BitmapFont_GetGlyphData(HI_CHAR * str, HI_U8** pData);

/** 
\brief Obtains width and height info of queried character.  
CNcomment:��ȡ����ѯ�ַ�����ģ�Ŀ����Ϣ
\attention \n
str contains only one queried character. The first character will take effect if there are
more than one character.
CNcomment:str һ��ֻ����һ������ѯ�ַ�.����һ���ַ�ֻȡ��һ����
\paam[in]  str Queried character. CNcomment:����ѯ���ַ�
\param[out]  pWidth widht. CNcomment:��ģ���
\param[out]  pHeight height. CNcomment:��ģ�߶�

\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVPARAM
\retval ::HI_FONT_ERRUNKNOWCHARSET
\see \n
N/A
*/
HI_S32 HI_BitmapFont_GetGlyphSize(HI_CHAR * str, HI_S32 *pWidth, HI_S32 *pHeight);


/*********************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif //

#endif


