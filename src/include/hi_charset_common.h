/**
 \file
 \brief common module
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2008-2018
 \version 1.0
 \author 
 \date 2011-10-9
 */

#ifndef __HI_CHARSET_COMMON_H__
#define __HI_CHARSET_COMMON_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
** ����������
**/
typedef enum hiCHARSET_STREAMTYPE_ENUM{
    HI_CHARSET_STREAMTYPE_UNKNOW = 0,    /*δ֪������*/
    HI_CHARSET_STREAMTYPE_BITS8  = 1,    /*���ֽ�������*/
    HI_CHARSET_STREAMTYPE_BITS16LE=2,    /*˫�ֽ�С��������*/
    HI_CHARSET_STREAMTYPE_BITS16BE=3,    /*˫�ֽڴ��������*/
    HI_CHARSET_STREAMTYPE_BITS32LE=4,    /*���ֽ�С��������*/
    HI_CHARSET_STREAMTYPE_BITS32BE=5,    /*���ֽڴ��������*/
    HI_CHARSET_STREAMTYPE_BULT,
}HI_CHARSET_STREAMTYPE_E;
/**
**  �û������첽������������
**/
typedef enum hiCHARSET_CMD_ENUM{
    HI_CHARSET_CMD_NONE = 0,  /*δ֪����*/
    HI_CHARSET_CMD_EXIT,      /*�����첽�˳�����*/
    HI_CHARSET_CMD_USERDATA,  /*�첽�����û�����*/
    HI_CHARSET_CMD_BULT,
}HI_CHARSET_CMD_E;

typedef struct hiCHARSET_FUN_S{
    /** 
    \brief �����ַ���ģ����
    \attention \n
    ��
    \param [out] handle �ַ������
    \param [in] u32Userdata �ַ���˽������
    
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*Create)(HI_HANDLE *handle, HI_U32 u32Userdata);
    /** 
    \brief ʶ�����������ͣ�����ʶ��������ʶ�����ݣ���ɺ�����ʶ����
    \attention \n
    ��
    \param [in] handle �ַ������
    \param [in] input ������
    \param [in] inLen ����������
    \param [out] codeType ���صı�������
    \param [in] s32UserData ˽������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*IdentStream)(HI_HANDLE handle, const HI_U8 *input, const HI_S32 inLen, HI_S32 s32UserData, HI_S32 *codeType);
    /** 
    \brief  ������ת�룬����ת�����������ݽ���ת�룬
    ��������������Ͳ��ԣ�����ת���������´���
    \attention \n
    ��
    \param [in] handle  �ַ������
    \param [out] out  ת���������
    \param [out] outlen  ת�������������
    \param [in] DstType  Ŀ��ת������
    \param [in] input  ����������
    \param [in] inLen  ��������������
    \param [in] SrcType  Դ��������
    \param [out] Usedlen  �Ѿ�ת�������������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*ConvStream)(HI_HANDLE handle, HI_U8 *out, HI_S32 *outlen,
        const HI_U8 *input, const HI_S32 inLen, HI_S32 SrcType, HI_S32 DstType, HI_S32* Usedlen);
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
    HI_S32 (*setDataAsync)(HI_HANDLE handle,HI_U32 u32Cmd, HI_VOID *pArg);

    /** 
    \brief �����ַ���ģ����
    \attention \n
    ��
    \param [in] handle �ַ������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    HI_S32 (*Destroy)(HI_HANDLE handle);


    /** 
    \brief  �����ַ����������ж��ֽ�������
    \attention \n
    ��
    \param [in] codetype �ַ�����
    \param [out] eStreamType ����������
    
    \retval ::HI_SUCCESS �����ɹ�
    \retval ::HI_FAILURE �����Ƿ�
    
    \see \n
    ��
    */
    
    HI_S32 (*GetStreamType)(HI_S32 codetype, HI_CHARSET_STREAMTYPE_E* eStreamType);
}HI_CHARSET_FUN_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_CHARSET_COMMON_H__ */


