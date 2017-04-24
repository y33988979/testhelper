/*****************************************************************************/

/*                Copyright 2005 - 2005, Huawei Tech. Co., Ltd.              */
/*                           ALL RIGHTS RESERVED                             */
/*                                                                           */
/* FileName: hi_peffect.h                                                    */
/* Version: 01a                                                              */
/*                                                                           */
/* History:                                                                  */
/* 1. 01a,2008-12-27,z37062 Create this file.                                */
/*****************************************************************************/
#ifndef __HI_EFFECT_H__
#define __HI_EFFECT_H__

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#include <hi_type.h>
#include <hi_go.h>


/** HiEffect ��Ŀ����ID */
#define HIEFFECT_ERR_APPID (0x80000000L + 0x40000000L)
/** HiEffect ģ��ID */
#define HIEFFECT_ERR_MODUID 0x00000000
typedef enum hiEFFECTLOG_ERRLEVEL_E
{
    HIEFFECT_LOG_LEVEL_DEBUG = 0,  /* debug-level                                  */
    HIEFFECT_LOG_LEVEL_INFO,       /* informational                                */
    HIEFFECT_LOG_LEVEL_NOTICE,     /* normal but significant condition             */
    HIEFFECT_LOG_LEVEL_WARNING,    /* warning conditions                           */
    HIEFFECT_LOG_LEVEL_ERROR,      /* error conditions                             */
    HIEFFECT_LOG_LEVEL_CRIT,       /* critical conditions                          */
    HIEFFECT_LOG_LEVEL_ALERT,      /* action must be taken immediately             */
    HIEFFECT_LOG_LEVEL_FATAL,      /* just for compatibility with previous version */
    HIEFFECT_LOG_LEVEL_BUTT
} HIEFFECT_LOG_ERRLEVEL_E;

/** HiEffect �����붨��� */
#define HIEFFECT_DEF_ERR(module, errid) \
    ((HI_S32)((HIEFFECT_ERR_APPID) | ((module) << 16) | ((HIEFFECT_LOG_LEVEL_ERROR) << 13) | (errid)))


enum HIEFFECT_ErrorCode_E
{
    ERR_HIEFFECT_CODE_UNDEF,               /*Î´¶¨ÒåÃüÁîÂë*/
    ERR_HIEFFECT_PTR_NULL,                 /*Ö¸Õë²ÎÊýÎª¿Õ*/
    ERR_HIEFFECT_NO_OPEN,                  /*Éè±¸Ã»ÓÐ´ò¿ª*/
    ERR_HIEFFECT_OPENED,                   /*Éè±¸ÒÑ¾­´ò¿ª*/
    ERR_HIEFFECT_CLOSED,                   /*Éè±¸ÒÑ¾­¹Ø±Õ*/

    /* New */
    ERR_HIEFFECT_INVALID_DEVICE,           /* invalid device */
    ERR_HIEFFECT_INVALID_HANDLE,
    ERR_HIEFFECT_INVALID_PARAMETER,        /* invalid parameter */
    ERR_HIEFFECT_INVALID_OPERATION,
    ERR_HIEFFECT_ADDR_FAULT,               /* address fault */
    ERR_HIEFFECT_NO_MEM                    /* mem alloc fail */
};

#define HI_ERR_HIEFFECT_CODE_UNDEF\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_CODE_UNDEF)

#define HI_ERR_HIEFFECT_PTR_NULL\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_PTR_NULL)

#define HI_ERR_HIEFFECT_OPENED\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_OPENED)

#define HI_ERR_HIEFFECT_CLOSED\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_CLOSED)

#define HI_ERR_HIEFFECT_NO_OPEN\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_NO_OPEN)


#define HI_ERR_HIEFFECT_INVALID_DEVICE\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_DEVICE)

#define HI_ERR_HIEFFECT_INVALID_HANDLE\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_HANDLE)

#define HI_ERR_HIEFFECT_INVALID_PARAMETER\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_PARAMETER)

#define HI_ERR_HIEFFECT_INVALID_OPERATION\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_INVALID_OPERATION)

#define HI_ERR_HIEFFECT_ADDR_FAULT\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_ADDR_FAULT)

#define HI_ERR_HIEFFECT_NO_MEM\
    HIEFFECT_DEF_ERR(HIEFFECT_ERR_MODUID, ERR_HIEFFECT_NO_MEM)


/*��Ļˢ�»ص�����*/
typedef HI_S32 (*pRefresh_Callback)(HI_VOID);
typedef enum hiEFFECT_MODE_E
{
    HI_EFFECT_NONE = 0, 				/*������ʾ*/
    HI_EFFECT_TURNPAGE, 				/*��ҳ*/
    HI_EFFECT_ROLLPAGE, 				/*����*/
    HI_EFFECT_VERTICALSHUTTER,		/* ��ֱ��Ҷ��*/
    HI_EFFECT_HORIZONTALSHUTTER,		/* ˮƽ��Ҷ��*/
    HI_EFFECT_LEFTIN,					/* ������*/
    HI_EFFECT_TOPIN, 					/* ���ϳ��*/
    HI_EFFECT_TRANSIN, 				/* ��������*/
    HI_EFFECT_ROTATE,  				/*����*/
    HI_EFFECT_CENTEROUT,  				/*���뽥��*/
    HI_EFFECT_CENTERIN, 				/*���뽥��*/
} EFFECT_MODE_E;

typedef enum hiEFFECT_SPEED
{
    HI_EFFECT_SPEED_FAST   = 1,
    HI_EFFECT_SPEED_NORMAL = 2,
    HI_EFFECT_SPEED_SLOW = 4,
} EFFECT_SPEED;

/*��Ч��ʼ������,
ע����Ļˢ�»ص�������������ʾģʽ
bLetfbox ΪHI_TRUE, ��ʾͼ���������Ļ������һ��ʱ��Ӻڱ�
bLetfbox ΪHI_FALSE, ��ʾͼ�����ų�ȫ��,���ͼ�α�������Ļ������һ��
�ᵼ�±���
ע:�������ַ�ʽ,ͼ��ȫ����ʾ
 */
extern HI_S32  HI_Effect_Init(pRefresh_Callback pfCallBack, HI_BOOL bLetfbox);
extern HI_S32  HI_Effect_Deinit(HI_VOID);

/*��Ч���ź���, �ú���Ϊ��������,��Ч��ɲ��˳�
����surface�����ظ�ʽ���뱣��һ��
Ŀǰ֧�ֵĸ�ʽ:
    HIGO_PF_4444,
    HIGO_PF_0444,
    HIGO_PF_1555,
    HIGO_PF_0555,
    HIGO_PF_565,
 */
extern HI_S32  HI_Effect_Play(HI_HANDLE hNewSurface, HI_HANDLE hScreenSurface, EFFECT_MODE_E mode);

/**
   ������Ч�����
*/
extern HI_S32 HI_Effect_SetLayer(HI_HANDLE Layer);

/*��Ч������ֹ,������߰�����Ӧ�ٶ�*/
extern HI_S32  HI_Effect_Stop(HI_VOID);

/*��Ч�����ٶ�*/
extern HI_S32  HI_Effect_SetSpeed(EFFECT_SPEED eSpeed);
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_PEFFECT_H__ */
