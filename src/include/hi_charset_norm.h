/**
 \file
 \brief common module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author 
 \date 2011-10-9
 */

#ifndef __HI_CHARSET_NORM_H__
#define __HI_CHARSET_NORM_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
�ַ�����������,
����ַ�������ѭ����Լ��:
1������ַ�������Ҫ���ַ����������ͺ�����ϱ�������
2����ӵı������ͱ��������
3����ӵı������ͻ���ǰ��ı������ͱ��+1
*/
typedef enum hiCHARSET_CODETYPE_ENUM {
    HI_CHARSET_CODETYPE_NONE = 0,        /**Ĭ��ֵ��������*/
    HI_CHARSET_CODETYPE_BIG5 = 1,        /**��������*/
    HI_CHARSET_CODETYPE_UTF8 = 2,        /**UTF8*/
    HI_CHARSET_CODETYPE_ISO8859_1 = 3,   /** ��ŷ��ϵ ��������1*/
    HI_CHARSET_CODETYPE_ISO8859_2 = 4,   /**��ŷ�Ͷ�ŷ��ϵ��������2*/
    HI_CHARSET_CODETYPE_ISO8859_3 = 5,   /** ŷ�޶��ϲ���ϵ��������3*/
    HI_CHARSET_CODETYPE_ISO8859_4 = 6,   /**˹������ά�ǺͲ��޵ĺ���ϵ��������4*/
    HI_CHARSET_CODETYPE_ISO8859_5 = 7,   /**�����ĺ�˹������*/
    HI_CHARSET_CODETYPE_ISO8859_6 = 8,   /**�����ĺͰ�������*/
    HI_CHARSET_CODETYPE_ISO8859_7 = 9,   /**�����ĺ�ϣ����*/
    HI_CHARSET_CODETYPE_ISO8859_8 =10,   /**�����ĺ�ϣ������*/
    HI_CHARSET_CODETYPE_ISO8859_9 =11,   /**Ϊ�������������������ģ�������5*/
    HI_CHARSET_CODETYPE_ISO8859_10=12,   /**�����ˡ���ŷ�Ͱ�˹��Ħ�˵����֣�������6*/
    HI_CHARSET_CODETYPE_ISO8859_11=13,   /**��������̩��*/
    HI_CHARSET_CODETYPE_ISO8859_13=14,   /**���޵ĺ��ܱ���ϵ��������7*/
    HI_CHARSET_CODETYPE_ISO8859_14=15,   /**�������ģ�������8*/
    HI_CHARSET_CODETYPE_ISO8859_15=16,   /**�Ľ���������1��������9*/
    HI_CHARSET_CODETYPE_ISO8859_16=17,   /**���������ģ�������10*/
    HI_CHARSET_CODETYPE_UNICODE_16LE=18, /**16λС�˵�UNICODE*/
    HI_CHARSET_CODETYPE_UNICODE_16BE=19, /**16λ��˵�UNICODE*/
    HI_CHARSET_CODETYPE_GBK=20,          /**��������GBK*/
    HI_CHARSET_CODETYPE_CP1250 = 21,     /**��ŷ*/
    HI_CHARSET_CODETYPE_CP1251 = 22,     /**�������*/
    HI_CHARSET_CODETYPE_CP1252 = 23,     /**������I*/
    HI_CHARSET_CODETYPE_CP1253 = 24,     /**ϣ����*/
    HI_CHARSET_CODETYPE_CP1254 = 25,     /**��������*/
    HI_CHARSET_CODETYPE_CP1255 = 26,     /**ϣ������*/
    HI_CHARSET_CODETYPE_CP1256 = 27,     /**��������*/
    HI_CHARSET_CODETYPE_CP1257 = 28,     /**���޵ĺ���*/
    HI_CHARSET_CODETYPE_CP1258 = 29,     /**Խ��*/
    HI_CHARSET_CODETYPE_CP874  = 30,     /**̩��*/
    HI_CHARSET_CODETYPE_UNICODE_32LE=31, /**32λС�˵�UNICODE*/
    HI_CHARSET_CODETYPE_UNICODE_32BE=32, /**32λ��˵�UNICODE*/
    HI_CHARSET_CODETYPE_BULT,
} HI_CHARSET_CODETYPE_E;

/** �汾�Ŷ��� */
typedef struct hiCHARSET_LIB_VERSION_S
{
    HI_U8 u8VersionMajor;    /**< Major version accessor element */
    HI_U8 u8VersionMinor;    /**< Minor version accessor element */
    HI_U8 u8Revision;        /**< Revision version accessor element */
    HI_U8 u8Step;            /**< Step version accessor element */
} HI_CHARSET_LIB_VERSION_S;

/** ����̽����API */
typedef struct hiCHARSET_DETECTOR_FUN_S
{
    /** 
    \brief ����˽�����ݣ�����̽����
    \attention \n
    ��
    \param [out] pDetectHandle  ������̽�������
    \param [in] u32UserData  �û�˽������

    \retval ::HI_SUCCESS ����̽�����ɹ�
    \retval ::HI_FAILURE ����ʧ��

    \see \n
    ��
    */
    HI_S32 (*detect_create)(HI_HANDLE *pDetectHandle, HI_U32 u32UserData);

    /** 
    \brief ������������̽���������
    \attention \n
    ��
    \param [in]  pDetectHandle ̽�������
    \param [in]  u8Input ����������
    \param [in]  u32Inlen �������������ȣ�����Ϊ׼ȷ������������
    \param [in]  s32UserData �û�˽������
    \param [out] s32CodeType ���ݱ�������

    \retval ::HI_SUCCESS  ������ʶ��ɹ�����ȷ���ر�������
    \retval ::HI_FAILURE ����ʶ��ʧ�ܣ����ر�����������
    
    \see \n
    ��
    */
    HI_S32 (*identstream)(HI_HANDLE handle, const HI_U8 *u8Input, const HI_S32 s32Inlen, HI_S32 s32UserData, HI_S32 *s32CodeType);

    /** 
    \brief �첽�����û����������
    \attention \n
    ��
    \param [in] handle �ַ������
    \param [in] u32Data  ˽������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*detect_setDataAsync)(HI_HANDLE handle,HI_U32 u32Cmd, HI_VOID *pArg);


    /** 
    \brief �ر�̽����
    \attention \n
    ��
    \param [in] pDetectHandle  ̽�������

    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�

    \see \n
    ��
    */
    HI_S32 (*detect_close)(HI_HANDLE pDetectHandle);
} HI_CHARSET_DETECTOR_FUN_S;

/**ת����API */
typedef struct hiCHARSET_CONVERTER_FUN_S
{
    /** 
    \brief ����ת��Դ�������ͺ�Ŀ�ı������ͣ�����ʶ����
    \attention \n
    ��
    \param [in] s32SrcType  Դ��������
    \param [in] s32DstType  Ŀ�ı�������

    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE ����ʧ��

    \see \n
    ��
    */
    HI_S32 (*conv_find)(HI_S32 s32SrcType, HI_S32 s32DstType);

    /** 
    \brief ����ת����
    \attention \n
    ��
    \param [out] pConvHandle  ������ת�������
    \param [in] u32UserData  �û�˽������

    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE ����ʧ��

    \see \n
    ��
    */
    HI_S32 (*conv_create)(HI_HANDLE *pConvHandle, HI_U32 u32UserData);
    


    /** 
    \brief ��������������ת��
    \attention \n
    ��
    \param [in]  pConvHandle ת�������
    \param [in]  u8Input ����������
    \param [in]  u32Inlen ��������������
    \param [out] u8Output ת�������
    \param [out] u32Outlen ת������ݳ���
    \param [out] u32Usedlen �Ѿ�ת������ݳ���

    \retval ::HI_SUCCESS  ת��ɹ�
    \retval ::HI_FAILURE    ת��ʧ��
    
    \see \n
    ��
    */
    HI_S32 (*convstream)(HI_HANDLE pConvHandle, const HI_U8 *u8Input, const HI_S32 s32Inlen, HI_U8 *u8Output, HI_S32 *s32Outlen, HI_S32 s32SrcType, HI_S32 s32DstType, HI_S32 *s32Usedlen);

    /** 
    \brief �첽�����û����������
    \attention \n
    ��
    \param [in] handle �ַ������
    \param [in] u32Data  ˽������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*conv_setDataAsync)(HI_HANDLE handle,HI_U32 u32Cmd, HI_VOID *pArg);


    /** 
    \brief �ر�ת����
    \attention \n
    ��
    \param [in] pConvHandle  ת�������

    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�

    \see \n
    ��
    */
    HI_S32 (*conv_close)(HI_HANDLE pConvHandle);
} HI_CHARSET_CONVERTER_FUN_S;

/*******************************************************
HI_CHARSET_S g_stCharset_entry=
{
    .pszDllName  = (const HI_PCHAR )"libhicharset.so",
    .stLibVersion = {1, 0, 0, 0},
    .pszCharSetDesc = (const HI_PCHAR )"hicharset detector and converter",
    .u32Priority   = 0,
    
    .stDetectorFun.detect_create = ,
    .stDetectorFun.identstream   = ,
    .stDetectorFun.detect_setDataAsync = ,
    .stDetectorFun.detect_close  = ,
    
    .stConverterFun.conv_find   = ,
    .stConverterFun.conv_create = ,
    .stConverterFun.convstream  = ,
    .stConverterFun.conv_setDataAsync = ,
    .stConverterFun.conv_close = ,

    .next           = NULL,
    .pDllModule     = NULL,
    .u32PrivateData = 0,
};
********************************************************/


typedef struct hiCHARSET_S
{
    /**< �ַ������� */
    const HI_CHAR *pszDllName;

    /**< �ַ����汾�� */
    const HI_CHARSET_LIB_VERSION_S stLibVersion;

    /**< �ַ������� */
    const HI_CHAR *pszCharSetDesc;

    /**< ���ȼ�����0x0 - 0xFFFFFFFF��0xFFFFFFFF���ȼ���ߣ���������Ч */
    HI_U32 u32Priority;

    /**< �ַ���̽�������ʽӿڣ�̽����ʵ�ֱ�����ѭ���¹���:
         1������̽������̽�⾫��Ҳ�ߣ����ȼ�Խ�ߣ������ȷ�ϱ������ͣ���ʧ�ܷ���
         2������ַ���û��ʵ��̽�������ܣ����뽫�ò�������ΪNULL;
         */

    HI_CHARSET_DETECTOR_FUN_S stDetectorFun;

    /**<ת�����ӿڣ��ýӿ���ѭ���¹���:
         1������ַ���û��ʵ�ָýӿڣ����뽫�ò�������ΪNULL;
         2���ò����������ⲿָ������: �ַ������Բ�ʵ�ָýӿڣ�
         */

    HI_CHARSET_CONVERTER_FUN_S stConverterFun;

    struct hiCHARSET_S *next;

    /**< ��̬���� */
    HI_VOID  *pDllModule;

    /**< ˽������ */
    HI_U32 u32PrivateData;
} HI_CHARSET_S;

/**
**��ȡ�ַ�������Сֵ
**/
extern HI_S32 HI_CHARSET_GetMinCodetype();

/**
**��ȡ�ַ��������ֵ
**/
extern HI_S32 HI_CHARSET_GetMaxCodetype();

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif



