/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : // �ļ���

    Version   : 1.0

    Description : // ������ϸ˵���˳����ļ���ɵ���Ҫ���ܣ�������ģ��
                  // �����Ľӿڣ����ֵ��ȡֵ��Χ�����弰������Ŀ�
                  // �ơ�˳�򡢶����������ȹ�ϵ

    Others:       // �������ݵ�˵��
 
*******************************************************************************/

#ifndef YS_TYPEDEF_H__
#define YS_TYPEDEF_H__

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
  #ifndef CHAR_YS
    #define CHAR_YS char
  #endif

  #ifndef S8_YS
    #define S8_YS signed char
  #endif
  
  #ifndef S16_YS
    #define S16_YS signed short
  #endif
  
  #ifndef S32_YS
    #define S32_YS signed int
  #endif
  
  #ifndef U8_YS
    #define U8_YS unsigned char
  #endif
  
  #ifndef U16_YS
    #define U16_YS unsigned short
  #endif
  
  #ifndef U32_YS
    #define U32_YS unsigned int
  #endif
  
  #ifndef F32_YS
    #define F32_YS float
  #endif
  
  #ifndef F64_YS
    #define F64_YS double
  #endif
  
  #ifndef BOOL_YS
    #define BOOL_YS S32_YS//��ʹ��S8_YS,����TRUE_YS����һ��
  #endif
  
  #ifndef TRUE_YS
    #define TRUE_YS  (1 == 1)
  #endif
  
  #ifndef FALSE_YS
    #define FALSE_YS  (!TRUE_YS)
  #endif
  
  #ifndef  NULL_YS
    #define NULL_YS  (void*)0
  #endif

  #ifndef IN_YS
    #define IN_YS
  #endif
  
  #ifndef OUT_YS
    #define OUT_YS
  #endif
  
  #ifndef INOUT_YS
    #define INOUT_YS
  #endif
  
typedef const char * YS_Version_t;


#endif //YS_TYPEDEF_H__



