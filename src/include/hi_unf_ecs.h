/******************************************************************************

  Copyright (C), 2001-2011, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name     : hi_unf_ecs_type.h
Version       : Initial draft
Author        : HiSilicon multimedia software group
Created Date   : 2008-06-05
Last Modified by:
Description   : Application programming interfaces (APIs) of the external chip software (ECS)
Function List :
Change History:
 ******************************************************************************/
#ifndef __HI_UNF_ECS_TYPE_H__
#define __HI_UNF_ECS_TYPE_H__

#include "hi_common.h"
#include "hi_error_ecs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define HI_I2C_MAX_NUM_USER (15)    /**<Maximum I2C channel ID*/ /**<CNcomment:���I2Cͨ����*/

#define HI_UNF_DISEQC_MSG_MAX_LENGTH (6)    /**<DiSEqC message length*/  /**<CNcomment:DiSEqC��Ϣ����*/
#define HI_UNF_DISEQC_MAX_REPEAT_TIMES (4)  /**<DiSEqC message max repeat times*/
#define MAX_TS_LINE 11                      /** The NO of ts lines that can be configured*/ /** CNcomment:�����õ�ts�ź�������*/

#ifdef DISEQC_SUPPORT
#define DISEQC_MAX_MOTOR_PISITION (255) /**<DiSEqC motor max stored position*/ 
#endif /* DISEQC_SUPPORT */
/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_1 */
/** @{ */  /** <!-- [QAM] */

/**Quadrature amplitude modulation (QAM) mode*/
/**CNcomment: QAM���Ʒ�ʽ*/
typedef enum hiUNF_QAM_TYPE_E
{
    HI_UNF_MOD_TYPE_DEFAULT,          /**<Default QAM mode. The default QAM mode is HI_UNF_MOD_TYPE_QAM_64 at present.*/   /**<CNcomment:Ĭ�ϵ�QAM����, ��ǰϵͳĬ��ΪHI_UNF_MOD_TYPE_QAM_64 */
    HI_UNF_MOD_TYPE_QAM_16 = 0x100,   /**<Enumeration corresponding to the 16QAM mode*/                                    /**<CNcomment:16QAM��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_QAM_32,           /**<Enumeration corresponding to the 32QAM mode*/                                    /**<CNcomment:32QAM��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_QAM_64,           /**<Enumeration corresponding to the 64QAM mode*/                                    /**<CNcomment:64QAM��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_QAM_128,          /**<Enumeration corresponding to the 128QAM mode*/                                   /**<CNcomment:128QAM��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_QAM_256,          /**<Enumeration corresponding to the 256QAM mode*/                                   /**<CNcomment:256QAM��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_QAM_512,          /**<Enumeration corresponding to the 512QAM mode*/                                   /**<CNcomment:512QAM��Ӧ��ö��ֵ*/

    HI_UNF_MOD_TYPE_BPSK = 0x200,     /**<Enumeration corresponding to the binary phase shift keying (BPSK) QAM mode. This mode is not supported at present.*/         /**<CNcomment:BPSK QAM��Ӧ��ö��ֵ���ݲ�֧��*/
    HI_UNF_MOD_TYPE_QPSK = 0x300,     /**<Enumeration corresponding to the quaternary phase shift keying (QPSK) QAM mode. This mode is not supported at present.*/     /**<CNcomment:QPSK QAM��Ӧ��ö��ֵ���ݲ�֧��*/
    HI_UNF_MOD_TYPE_DQPSK,
    HI_UNF_MOD_TYPE_8PSK,             /**<Enumeration corresponding to the 8 phase shift keying (8PSK) mode*/              /**<CNcomment:8PSK��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_16APSK,           /**<Enumeration corresponding to the 16-Ary Amplitude and Phase Shift Keying (QPSK) mode*/      /**<CNcomment:16APSK��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_32APSK,           /**<Enumeration corresponding to the 32-Ary Amplitude and Phase Shift Keying (QPSK) mode*/      /**<CNcomment:32APSK��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_8VSB,           /**<Enumeration corresponding to (8VSB) mode*/      /**<CNcomment:8VSB��Ӧ��ö��ֵ*/
    HI_UNF_MOD_TYPE_16VSB,           /**<Enumeration corresponding to (16VSB) mode*/      /**<CNcomment:16VSB��Ӧ��ö��ֵ*/

    /**<Enumeration corresponding to the auto mode. For DVB-S/S2, if detect modulation type fail, it will return auto*/
    /**<CNcomment:�����źŵ��Ʒ�ʽ�Զ���⣬������ʧ�ܷ���AUTO*/
    HI_UNF_MOD_TYPE_AUTO,

    HI_UNF_MOD_TYPE_BUTT              /**<Invalid QAM mode*/       /**<CNcomment:�Ƿ���QAM����ö��ֵ*/
}HI_UNF_MODULATION_TYPE_E;

/**Frequency locking status of the tuner*/
/**CNcomment:TUNER��Ƶ״̬*/
typedef enum  hiUNF_TUNER_LOCK_STATUS_E
{
    HI_UNF_TUNER_SIGNAL_DROPPED = 0,      /**<The signal is not locked.*/  /**<CNcomment:�ź�δ����*/
    HI_UNF_TUNER_SIGNAL_LOCKED,            /**<The signal is locked.*/  /**<CNcomment:�ź�������*/
    HI_UNF_TUNER_SIGNAL_BUTT             /**<Invalid value*/   /**<CNcomment:�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_LOCK_STATUS_E;

/**Output mode of the tuner*/
/**CNcomment:TUNER���ģʽ */
typedef enum hiUNF_TUNER_OUTPUT_MODE_E
{
    HI_UNF_TUNER_OUTPUT_MODE_DEFAULT ,            /**<Parallel mode valid*/        /**<CNcomment:����ģʽvalid*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,     /**<Parallel mode valid*/     /**<CNcomment:����ģʽvalid*/
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B,     /**<Parallel mode burst*/     /**<CNcomment:����ģʽburst*/
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL,              /**<Serial mode 74.25M*/  /**<CNcomment:����ģ74.25M*/ 
    HI_UNF_TUNER_OUTPUT_MODE_SERIAL_50,           /**<Serial mode 50M*/     /**<CNcomment:����ģ50M*/
    HI_UNF_TUNER_OUTPUT_MODE_BUTT                 /**<Invalid value*/       /**<CNcomment:�Ƿ�ֵ*/
}HI_UNF_TUNER_OUPUT_MODE_E;

/**Signal type of the tuner*/
/**CNcomment:TUNER�ź�����*/
typedef enum    hiTUNER_SIG_TYPE_E
{
    HI_UNF_TUNER_SIG_TYPE_CAB = 0  ,   /**<Cable signal*/      /**<CNcomment:CABLE�ź�*/
    HI_UNF_TUNER_SIG_TYPE_SAT ,        /**<Satellite signal*/  /**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T  ,       /**<Terrestrial signal*//**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_DVB_T2  ,       /**<Terrestrial signal*//**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_ISDB_T  ,       /**<Terrestrial signal*//**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_ATSC_T  ,       /**<Terrestrial signal*//**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_DTMB  ,       /**<Terrestrial signal*//**<CNcomment:�����ź�*/
    HI_UNF_TUNER_SIG_TYPE_BUTT         /**<Invalid value*/     /**<CNcomment:�Ƿ�ֵ*/
}HI_UNF_TUNER_SIG_TYPE_E;

/**Inter-integrated (I2C) channel used by the tuner*/
/**CNcomment:TUNERʹ�õ�I2Cͨ��*/
typedef enum hiUNF_TUNER_I2cChannel
{
    HI_UNF_I2C_CHANNEL_0 = 0,      /**<I2C channel 0*/                               /**<CNcomment:I2Cͨ��0*/
    HI_UNF_I2C_CHANNEL_1,          /**<I2C channel 1*/                               /**<CNcomment:I2Cͨ��1*/
    HI_UNF_I2C_CHANNEL_2,          /**<I2C channel 2*/                               /**<CNcomment:I2Cͨ��2*/
    HI_UNF_I2C_CHANNEL_3,          /**<I2C channel 3*/                               /**<CNcomment:I2Cͨ��3*/
    HI_UNF_I2C_CHANNEL_QAM = 4,    /**<Dedicated I2C channel of the built-in QAM*/   /**<CNcomment:����QAMר��I2Cͨ�� */
    HI_UNF_I2C_CHANNEL_5,          /**GPIO simulates I2C*/                           /**CNcomment:GPIOģ��I2C*/
    HI_UNF_I2C_CHANNEL_6,               
    HI_UNF_I2C_CHANNEL_7,
    HI_UNF_I2C_CHANNEL_8,               
    HI_UNF_I2C_CHANNEL_9,               
    HI_UNF_I2C_CHANNEL_10,
    HI_UNF_I2C_CHANNEL_11,               
    HI_UNF_I2C_CHANNEL_12,               
    HI_UNF_I2C_CHANNEL_13,
    HI_UNF_I2C_CHANNEL_14,               
    HI_UNF_I2C_CHANNEL_15,               
    HI_UNF_I2C_CHANNEL_MAX        /**<Invalid value*/                               /**<CNcomment:�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_I2cChannel_E;

/**type of GPIO interrupt*/
/** CNcomment:GPIO �ж�����*/
typedef enum hiUNF_GPIO_INTTYPE_E
{
    HI_UNF_GPIO_INTTYPE_UP,          /**spring by the up edge*/                 /**<CNcomment:�����ش���*/
	HI_UNF_GPIO_INTTYPE_DOWN,          /**spring by the down edge*/               /**<CNcomment:�½��ش���*/
	HI_UNF_GPIO_INTTYPE_UPDOWN,        /**spring by both the up and down edge*/   /**<CNcomment:˫�ش���*/
	HI_UNF_GPIO_INTTYPE_HIGH,          /**spring by the high level*/              /**<CNcomment:�ߵ�ƽ����*/
	HI_UNF_GPIO_INTTYPE_LOW,           /**spring by the low level*/               /**<CNcomment:�͵�ƽ����*/
	HI_UNF_GPIO_INTTYPE_BUTT,          /**<Invalid value*/                        /**<CNcomment:�Ƿ��߽�ֵ*/
}HI_UNF_GPIO_INTTYPE_E ;

/**type of TUNER device*/
/**CNcomment:TUNER�豸����*/
typedef enum    hiUNF_TUNER_DEV_TYPE_E
{
    HI_UNF_TUNER_DEV_TYPE_XG_3BL,               /**<XG_3BL*/
    HI_UNF_TUNER_DEV_TYPE_CD1616,               /**<CD1616*/
    HI_UNF_TUNER_DEV_TYPE_ALPS_TDAE,            /**<ALPS_TDAE*/
    HI_UNF_TUNER_DEV_TYPE_TDCC,                 /**<TDCC*/
    HI_UNF_TUNER_DEV_TYPE_TDA18250,             /**<TDA18250*/
    HI_UNF_TUNER_DEV_TYPE_CD1616_DOUBLE,
    HI_UNF_TUNER_DEV_TYPE_MT2081,				        /**<MT2081*/
    HI_UNF_TUNER_DEV_TYPE_TMX7070X,             /**<THOMSON7070X*/
    HI_UNF_TUNER_DEV_TYPE_R820C,             /**<R820C*/
	HI_UNF_TUNER_DEV_TYPE_MXL203,               /**< MXL203 */
    HI_UNF_TUNER_DEV_TYPE_AV2011,               /**<AV2011*/
    HI_UNF_TUNER_DEV_TYPE_SHARP7903,            /**<SHARP7903*/
    HI_UNF_TUNER_DEV_TYPE_MXL101,
    HI_UNF_TUNER_DEV_TYPE_MXL603,
    HI_UNF_TUNER_DEV_TYPE_IT9170,
    HI_UNF_TUNER_DEV_TYPE_IT9133,
    HI_UNF_TUNER_DEV_TYPE_TDA6651,
    HI_UNF_TUNER_DEV_TYPE_TDA18250B,             /**<TDA18250B*/

    HI_UNF_TUNER_DEV_TYPE_BUTT,                 /**<Invalid value*/   /**<CNcomment:�Ƿ��߽�ֵ*/
}HI_UNF_TUNER_DEV_TYPE_E ;

/**Type of the QAM device*/
/**CNcomment:QAM�豸����*/
typedef enum    hiUNF_DEMOD_DEV_TYPE_E
{
    HI_UNF_DEMOD_DEV_TYPE_NONE,                /**<Not supported*/    /**<CNcomment:��֧��*/  
    HI_UNF_DEMOD_DEV_TYPE_3130I= 0x100,        /**<Internal QAM*/     /**<CNcomment:�ڲ�QAM*/
    HI_UNF_DEMOD_DEV_TYPE_3130E,               /**<External Hi3130*/  /**<CNcomment:�ⲿQAM hi3130оƬ*/
    HI_UNF_DEMOD_DEV_TYPE_J83B,                /**<suppoort j83b*/    /**<CNcomment:֧��j83b*/
    HI_UNF_DEMOD_DEV_TYPE_AVL6211,             /**<Availink 6211*/    /**<CNcomment:֧��Availink 6211*/
    HI_UNF_DEMOD_DEV_TYPE_MXL101,
    HI_UNF_DEMOD_DEV_TYPE_MN88472,
    HI_UNF_DEMOD_DEV_TYPE_IT9170,
    HI_UNF_DEMOD_DEV_TYPE_IT9133,

    HI_UNF_DEMOD_DEV_TYPE_BUTT,                /**<Invalid value*/    /**<CNcomment:�Ƿ��߽�ֵ*/
}HI_UNF_DEMOD_DEV_TYPE_E ;

/**Defines the cable transmission signal.*/
/**CNcomment:����CABLE�����ź�*/
typedef struct  hiUNF_CAB_CONNECT_PARA_S
{
    HI_U32                      u32Freq;            /**<Frequency, in kHz*/      /**<CNcomment:Ƶ�ʣ���λ��kHz*/
    HI_U32                      u32SymbolRate ;     /**<Symbol rate, in bit/s*/  /**<CNcomment:�����ʣ���λbps */
    HI_UNF_MODULATION_TYPE_E    enModType ;         /**<QAM mode*/               /**<CNcomment:QAM���Ʒ�ʽ*/
    HI_BOOL                     bReverse ;          /**<Spectrum reverse mode*/  /**<CNcomment:Ƶ�׵��ô���ʽ*/
}HI_UNF_CAB_CONNECT_PARA_S ;

/*Not used currently*/
/*CNcomment:��δʹ��*/
typedef enum hiUNF_TUNER_FE_GUARD_INTV_E
{
    HI_UNF_TUNER_FE_GUARD_INTV_DEFALUT = 0 ,    /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_128       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_32       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_16       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_8        ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_1_4        ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_19_128       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_19_256       ,       /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_BUTT     ,       /**< */
}HI_UNF_TUNER_FE_GUARD_INTV_E;

/*Not used currently*/
/*CNcomment:��δʹ��*/
typedef enum hiUNF_TUNER_FE_DEPUNC_RATE_E
{
    HI_UNF_TUNER_FE_DEPUNC_RATE_DEFAULT = 0 ,    /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_12      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_23      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_34      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_45      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_56      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_67      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_78      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_89      ,        /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_BUTT    ,        /**< */
}HI_UNF_TUNER_FE_DEPUNC_RATE_E ;

/*Not used currently*/
/*CNcomment:��δʹ��*/
typedef enum hiUNF_TUNER_FE_FFT_E
{
    HI_UNF_TUNER_FE_FFT_DEFAULT = 0,   /**< */
    HI_UNF_TUNER_FE_FFT_1K ,           /**< */
    HI_UNF_TUNER_FE_FFT_2K ,           /**< */
    HI_UNF_TUNER_FE_FFT_4K ,           /**< */
    HI_UNF_TUNER_FE_FFT_8K ,           /**< */
    HI_UNF_TUNER_FE_FFT_16K ,           /**< */
    HI_UNF_TUNER_FE_FFT_32K ,           /**< */
    HI_UNF_TUNER_FE_FFT_BUTT ,         /**< */
}HI_UNF_TUNER_FE_FFT_E;

/*Not used currently*/
/*CNcomment:��δʹ��*/
typedef enum hiUNF_TUNER_FE_HIERARCHY_E
{
    HI_UNF_TUNER_FE_HIERARCHY_DEFAULT = 0 ,   /**< */
    HI_UNF_TUNER_FE_HIERARCHY_NO,             /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA1,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA2,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_ALHPA4,         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_BUTT,           /**< */
}HI_UNF_TUNER_FE_HIERARCHY_E ;

/*CNcomment*/
typedef enum hiUNF_TUNER_TS_PRIORITY_E
{
    HI_UNF_TUNER_TS_PRIORITY_NONE = 0 ,   /**< */
    HI_UNF_TUNER_TS_PRIORITY_HP,             /**< */
    HI_UNF_TUNER_TS_PRIORITY_LP,         /**< */
    HI_UNF_TUNER_TS_PRIORITY_BUTT,           /**< */
}HI_UNF_TUNER_TS_PRIORITY_E ;

typedef struct  hiUNF_TER_CONNECT_PARA_S
{
    HI_U32          u32Freq     ;                    /**<freq in KHz */
    HI_U32          u32BandWidth    ;                /**<bandwidth in KHz */
    HI_UNF_MODULATION_TYPE_E enModType  ;            /**< */
    HI_BOOL                     bReverse ;          /**<Spectrum reverse mode*/  /**<CNcomment:Ƶ�׷�ת����ʽ*/
}HI_UNF_TER_CONNECT_PARA_S ;

#if 0
/*CNcomment:��δʹ��*/
typedef struct  hiUNF_TUNER_TER_ATTR_S
{
    HI_UNF_TUNER_FE_DEPUNC_RATE_E   enHpCode ;      /**< */
    HI_UNF_TUNER_FE_DEPUNC_RATE_E   enLpCode ;      /**< */
    HI_UNF_TUNER_FE_GUARD_INTV_E    enGuardIntv ;   /**< */
    HI_UNF_TUNER_FE_FFT_E       enFftType ;         /**< */
    HI_UNF_TUNER_FE_HIERARCHY_E enHierType ;        /**< */

}HI_UNF_TUNER_TER_ATTR_S;
#endif

/**TS clock polarization*/
/*CNcomment:TSʱ�Ӽ���*/
typedef enum hiUNF_TUNER_TSCLK_POLAR_E
{
    HI_UNF_TUNER_TSCLK_POLAR_FALLING,       /**<Falling edge*/      /**<CNcomment:�½���*/
    HI_UNF_TUNER_TSCLK_POLAR_RISING,        /**<Rising edge*/       /**<CNcomment:������*/
    HI_UNF_TUNER_TSCLK_POLAR_BUTT           /**<Invalid value*/     /**<CNcomment:�Ƿ��߽�ֵ*/
} HI_UNF_TUNER_TSCLK_POLAR_E;

/**TS format*/
typedef enum hiUNF_TUNER_TS_FORMAT_E
{
    HI_UNF_TUNER_TS_FORMAT_TS,              /**<188*/
    HI_UNF_TUNER_TS_FORMAT_TSP,             /**<204*/
    HI_UNF_TUNER_TS_FORMAT_BUTT             /**<Invalid value*/     /**<CNcomment:�Ƿ��߽�ֵ*/
} HI_UNF_TUNER_TS_FORMAT_E;

/**TS serial PIN*/
typedef enum hiUNF_TUNER_TS_SERIAL_PIN_E
{
    HI_UNF_TUNER_TS_SERIAL_PIN_0,           /**<Serial pin 0, default*/
    HI_UNF_TUNER_TS_SERIAL_PIN_7,           /**<Serial pin 7*/
    HI_UNF_TUNER_TS_SERIAL_PIN_BUTT         /**<Invalid value*/     /**<CNcomment:�Ƿ��߽�ֵ*/
} HI_UNF_TUNER_TS_SERIAL_PIN_E;

/** Tuner RF AGC mode */
typedef enum hiUNF_TUNER_RFAGC_MODE_E
{
    /**< Inverted polarization, default.This setting is used for a tuner whose gain decreases with increased AGC voltage */
    HI_UNF_TUNER_RFAGC_INVERT, 

    /**< Normal polarization. This setting is used for a tuner whose gain increases with increased AGC voltage */
    HI_UNF_TUNER_RFAGC_NORMAL,
    HI_UNF_TUNER_RFAGC_BUTT
} HI_UNF_TUNER_RFAGC_MODE_E;

/** Tuner IQ spectrum mode */
typedef enum hiUNF_TUNER_IQSPECTRUM_MODE_E
{
    HI_UNF_TUNER_IQSPECTRUM_NORMAL,         /**<The received signal spectrum is not inverted */
    HI_UNF_TUNER_IQSPECTRUM_INVERT,         /**<The received signal spectrum is inverted */
    HI_UNF_TUNER_IQSPECTRUM_BUTT
} HI_UNF_TUNER_IQSPECTRUM_MODE_E;

/** DiSEqC Wave Mode */
typedef enum hiUNF_TUNER_DISEQCWAVE_MODE_E
{
    HI_UNF_TUNER_DISEQCWAVE_NORMAL,         /**<Waveform produced by demod */
    HI_UNF_TUNER_DISEQCWAVE_ENVELOPE,       /**<Waveform produced by LNB control device */
    HI_UNF_TUNER_DISEQCWAVE_BUTT
} HI_UNF_TUNER_DISEQCWAVE_MODE_E;

/** LNB power supply and control device */
typedef enum hiUNF_LNBCTRL_DEV_TYPE_E
{
    HI_UNF_LNBCTRL_DEV_TYPE_NONE,           /**<No LNB control device */
    HI_UNF_LNBCTRL_DEV_TYPE_MPS8125,        /**<MPS8125 */
    HI_UNF_LNBCTRL_DEV_TYPE_ISL9492,        /**<ISL9492 */
    HI_UNF_LNBCTRL_DEV_TYPE_BUTT
} HI_UNF_LNBCTRL_DEV_TYPE_E;

typedef struct  hiUNF_TUNER_SAT_ATTR_S
{
    HI_U32                         u32ResetGpioNo;  /**<Demod reset GPIO NO. */
    HI_U32                         u32DemodClk;     /**<Demod reference clock frequency, KHz */
#if 0
    HI_U16                         u16DemodAddress; /**<Demod slave address */
    HI_U16                         u16TunerAddress; /**<Tuner slave address */
#endif
    HI_U16                         u16TunerMaxLPF;  /**<Tuner max LFP, MHz */
    HI_U16                         u16TunerI2CClk;  /**<Tuner I2C clock, kHz */
    HI_UNF_TUNER_RFAGC_MODE_E      enRFAGC;         /**<Tuner RF AGC mode */
    HI_UNF_TUNER_IQSPECTRUM_MODE_E enIQSpectrum;    /**<Tuner IQ spectrum mode */
    HI_UNF_TUNER_TSCLK_POLAR_E     enTSClkPolar;    /**<TS clock polarization */
    HI_UNF_TUNER_TS_FORMAT_E       enTSFormat;      /**<TS format */
    HI_UNF_TUNER_TS_SERIAL_PIN_E   enTSSerialPIN;   /**<TS serial PIN */
    HI_UNF_TUNER_DISEQCWAVE_MODE_E enDiSEqCWave;    /**<DiSEqC Wave Mode */
    HI_UNF_LNBCTRL_DEV_TYPE_E      enLNBCtrlDev;    /**<LNB power supply and control device */
    HI_U16                         u16LNBDevAddress;/**<LNB control device address */
} HI_UNF_TUNER_SAT_ATTR_S;

typedef struct  hiUNF_TUNER_TER_ATTR_S
{
    HI_U32                         u32ResetGpioNo;  /**<Demod reset GPIO NO. */
} HI_UNF_TUNER_TER_ATTR_S;
/** Polarization type */
typedef enum hiUNF_TUNER_FE_POLARIZATION_E
{
    HI_UNF_TUNER_FE_POLARIZATION_H,         /**< Horizontal Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_V,         /**< Vertical Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_L,         /**< Left-hand circular Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_R,         /**< Right-hand circular Polarization */
    HI_UNF_TUNER_FE_POLARIZATION_BUTT,      /**< Invalid */
} HI_UNF_TUNER_FE_POLARIZATION_E;

/** FEC Type */
typedef enum hiUNF_TUNER_FE_FECTYPE_E
{
    HI_UNF_TUNER_FE_FEC_DVBS,
    HI_UNF_TUNER_FE_FEC_DVBS2,
    HI_UNF_TUNER_FE_FEC_BUTT
} HI_UNF_TUNER_FE_FECTYPE_E;

/** FEC Rate */
typedef enum hiUNF_TUNER_FE_FECRATE_E
{
    HI_UNF_TUNER_FE_FEC_AUTO = 0,
    HI_UNF_TUNER_FE_FEC_1_2,
    HI_UNF_TUNER_FE_FEC_2_3,
    HI_UNF_TUNER_FE_FEC_3_4,
    HI_UNF_TUNER_FE_FEC_4_5,
    HI_UNF_TUNER_FE_FEC_5_6,
    HI_UNF_TUNER_FE_FEC_6_7,
    HI_UNF_TUNER_FE_FEC_7_8,
    HI_UNF_TUNER_FE_FEC_8_9,
    HI_UNF_TUNER_FE_FEC_9_10,
    HI_UNF_TUNER_FE_FEC_1_4,
    HI_UNF_TUNER_FE_FEC_1_3,
    HI_UNF_TUNER_FE_FEC_2_5,
    HI_UNF_TUNER_FE_FEC_3_5,
    HI_UNF_TUNER_FE_FECRATE_BUTT
} HI_UNF_TUNER_FE_FECRATE_E;

/** LNB type */
typedef enum hiUNF_TUNER_FE_FE_LNBTYPE_E
{
    HI_UNF_TUNER_FE_LNB_SINGLE_FREQUENCY,   /**<Single LO frequency */
    HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY,     /**<Dual LO frequency */
    HI_UNF_TUNER_FE_LNB_TYPE_BUTT
} HI_UNF_TUNER_FE_LNB_TYPE_E;

/** LNB band type */
typedef enum hiUNF_TUNER_FE_LNB_BAND_E
{
    HI_UNF_TUNER_FE_LNB_BAND_C,             /**<C */
    HI_UNF_TUNER_FE_LNB_BAND_KU,            /**<Ku */
    HI_UNF_TUNER_FE_LNB_BAND_BUTT
} HI_UNF_TUNER_FE_LNB_BAND_E;

/** LNB power control */
typedef enum hiUNF_TUNER_FE_LNB_POWER_E
{
    HI_UNF_TUNER_FE_LNB_POWER_OFF,          /**<LNB power off */
    HI_UNF_TUNER_FE_LNB_POWER_ON,           /**<LNB power auto, 13V/18V, default */
    HI_UNF_TUNER_FE_LNB_POWER_ENHANCED,     /**<LNB power auto, 14V/19V, some LNB control device can support.*/
    HI_UNF_TUNER_FE_LNB_POWER_BUTT
} HI_UNF_TUNER_FE_LNB_POWER_E;

/** LNB 22K tone status, for Ku band LNB */
typedef enum hiUNF_TUNER_FE_LNB_22K_E
{
    HI_UNF_TUNER_FE_LNB_22K_OFF,            /**<default */
    HI_UNF_TUNER_FE_LNB_22K_ON,
    HI_UNF_TUNER_FE_LNB_22K_BUTT
} HI_UNF_TUNER_FE_LNB_22K_E;

/** 0/12V switch */
typedef enum hiUNF_TUNER_SWITCH_0_12V_E
{
    HI_UNF_TUNER_SWITCH_0_12V_NONE,         /**< None, default */
    HI_UNF_TUNER_SWITCH_0_12V_0,            /**< 0V */
    HI_UNF_TUNER_SWITCH_0_12V_12,           /**< 12V */
    HI_UNF_TUNER_SWITCH_0_12V_BUTT
} HI_UNF_TUNER_SWITCH_0_12V_E;

/** 22KHz switch */
typedef enum hiUNF_TUNER_SWITCH_22K_E
{
    HI_UNF_TUNER_SWITCH_22K_NONE,           /**< None, default */
    HI_UNF_TUNER_SWITCH_22K_0,              /**< 0 */
    HI_UNF_TUNER_SWITCH_22K_22,             /**< 22KHz */
    HI_UNF_TUNER_SWITCH_22K_BUTT
} HI_UNF_TUNER_SWITCH_22K_E;

/** Tone burst switch */
typedef enum hiUNF_TUNER_SWITCH_TONEBURST_E
{
    HI_UNF_TUNER_SWITCH_TONEBURST_NONE,     /**< Don't send tone burst, default */
    HI_UNF_TUNER_SWITCH_TONEBURST_0,        /**< Tone burst 0 */
    HI_UNF_TUNER_SWITCH_TONEBURST_1,        /**< Tone burst 1 */
    HI_UNF_TUNER_SWITCH_TONEBURST_BUTT
} HI_UNF_TUNER_SWITCH_TONEBURST_E;

/**Structure of the satellite transmission signal.*/
/**CNcomment:����Satellite�����ź�*/
typedef struct  hiUNF_SAT_CONNECT_PARA_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:����Ƶ�ʣ���λ��kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:�����ʣ���λbps */
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:������ʽ*/
} HI_UNF_SAT_CONNECT_PARA_S;

/**Structure of the satellite transmission signal's detailed information.*/
/**CNcomment:�����ź���ϸ��Ϣ*/
typedef struct  hiUNF_TUNER_SAT_SIGNALINFO_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:����Ƶ�ʣ���λ��kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:�����ʣ���λbps */
    HI_UNF_MODULATION_TYPE_E       enModType;       /**<Modulation type*/            /**<CNcomment:���Ʒ�ʽ*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:������ʽ*/
    HI_UNF_TUNER_FE_FECTYPE_E      enFECType;       /**<FEC type*/
    HI_UNF_TUNER_FE_FECRATE_E      enFECRate;       /**<FEC rate*/
} HI_UNF_TUNER_SAT_SIGNALINFO_S;

typedef enum hiUNF_TUNER_T2_PLP_TYPE_E
{   
    HI_UNF_TUNER_T2_PLP_TYPE_COM=0, 
    HI_UNF_TUNER_T2_PLP_TYPE_DAT1,  
    HI_UNF_TUNER_T2_PLP_TYPE_DAT2,
    HI_UNF_TUNER_T2_PLP_TYPE_BUTT   
} HI_UNF_TUNER_T2_PLP_TYPE_E;

/**Structure of the terrestrial transmission signal's detailed information.*/
/**CNcomment:�����ź���ϸ��Ϣ*/
typedef struct  hiUNF_TUNER_TER_SIGNALINFO_S
{
    HI_U32                         u32Freq;         /**<Frequency, in kHz*/ /**<CNcomment:Ƶ�ʣ���λ��kHz*/
    HI_U32                         u32BandWidth;   /**<Band width, in KHz*/      /**<CNcomment:������λKHz */
    HI_UNF_MODULATION_TYPE_E       enModType;       /**<Modulation type*/            /**<CNcomment:���Ʒ�ʽ*/
    HI_UNF_TUNER_FE_FECRATE_E      enFECRate;       /**<FEC rate*/
    HI_UNF_TUNER_FE_GUARD_INTV_E enGuardIntv;
    HI_UNF_TUNER_FE_FFT_E enFFTMode;
    HI_UNF_TUNER_FE_HIERARCHY_E enHierMod; /**<Hierarchical Modulation and alpha, only used in DVB-T*/
    HI_UNF_TUNER_TS_PRIORITY_E enTsPriority; /**<The TS priority, only used in DVB-T*/
} HI_UNF_TUNER_TER_SIGNALINFO_S;
/**CNcomment:TUNER����*/
typedef struct hiUNF_TUNER_SIGNALINFO_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType;              /**<Signal transmission type*/  /**<CNcomment:�ź�����*/

    union
    {
        HI_UNF_TUNER_SAT_SIGNALINFO_S stSat;        /**<Signal info of satellite*/  /**<CNcomment:�����ź���Ϣ*/
        HI_UNF_TUNER_TER_SIGNALINFO_S stTer;        /**<Signal info of terrestrial*/  /**<CNcomment:�����ź���Ϣ*/
    } unSignalInfo;
} HI_UNF_TUNER_SIGNALINFO_S;

/** LNB configurating parameters */
typedef struct hiUNF_TUNER_FE_LNB_CONFIG_S
{
    HI_UNF_TUNER_FE_LNB_TYPE_E  enLNBType;      /**<LNB type*/                              /**<CNcomment:LNB����*/
    HI_U32                      u32LowLO;       /**< Low Local Oscillator Frequency, MHz */ /**<CNcomment:LNB�ͱ���Ƶ�ʣ���λMHz*/
    HI_U32                      u32HighLO;      /**< High Local Oscillator Frequency, MHz*/ /**<CNcomment:LNB�߱���Ƶ�ʣ���λMHz*/
    HI_UNF_TUNER_FE_LNB_BAND_E  enLNBBand;      /**< LNB band, C or Ku */                   /**<CNcomment:LNB���Σ�C��Ku*/
} HI_UNF_TUNER_FE_LNB_CONFIG_S;

typedef struct tag_TunerAttr_S
{
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;     /**<Tuner type*/                              /**<CNcomment:TUNER����*/
    HI_U32 u32TunerAddr;
} HI_TunerAttr_S;

typedef struct tag_DemodAttr_S
{
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;     /**<QAM type*/                                /**<CNcomment:QAM����*/
    HI_U32 u32DemodAddr;
} HI_DemodAttr_S;

/**CNcomment:TUNER����*/
typedef struct  hiTUNER_ATTR_S
{
    HI_UNF_TUNER_SIG_TYPE_E     enSigType ;         /**<Signal type*/                             /**<CNcomment:�ź�����*/
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;     /**<Tuner type*/                              /**<CNcomment:TUNER����*/
    HI_U32 u32TunerAddr;                                      /*The i2c address of tuner, if demod type is 3130I,3130E or J83B, you cannot set it*/
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;     /**<QAM type*/                                /**<CNcomment:QAM����*/
    HI_U32 u32DemodAddr;                                   /*The i2c address of demod, if demod type is 3130I,3130E or J83B, you cannot set it*/
    HI_UNF_TUNER_OUPUT_MODE_E   enOutputMode ;      /**<Output mode of transport streams (TSs)*/  /**<CNcomment:TS�����ģʽ*/
    HI_UNF_TUNER_I2cChannel_E   enI2cChannel;       /**<I2C channel used by the tuner*/           /**<CNcomment:TUNERʹ�õ�I2Cͨ��*/

    union
    {
        HI_UNF_TUNER_TER_ATTR_S stTer ;         /**<Attributes of the terrestrial signal. */  /**<CNcomment:�����ź�����*/
        HI_UNF_TUNER_SAT_ATTR_S stSat;          /**<Attributes of the satellite signal.*/     /**<CNcomment:�����ź�����*/
    }unTunerAttr ;
}HI_UNF_TUNER_ATTR_S ;

/**Frequency locking parameters of the tuner*/
/**CNcomment:TUNER��Ƶ����*/
typedef struct  hiUNF_TUNER_CONNECT_PARA_S
{
    HI_UNF_TUNER_SIG_TYPE_E enSigType ;        /**<Signal type*/                 /**<CNcomment:�ź�����*/

    union
    {
        HI_UNF_CAB_CONNECT_PARA_S   stCab ;   /**<Cable transmission signal*/    /**<CNcomment:CABLE�����ź�*/
        HI_UNF_TER_CONNECT_PARA_S stTer ;       /**<Terrestrial transmission signal*/ /**<CNcomment:���洫���ź�*/
        HI_UNF_SAT_CONNECT_PARA_S stSat;        /**<Satellite transmission signal*/   /**<CNcomment:���Ǵ����ź�*/
    }unConnectPara;
}HI_UNF_TUNER_CONNECT_PARA_S ;

/**Frequency locking status and parameters of the tuner*/
/**CNcomment:TUNER��Ƶ״̬����Ƶ����*/
typedef struct  hiUNF_TUNER_STATUS_S
{
    HI_UNF_TUNER_LOCK_STATUS_E  enLockStatus ;  /**<Frequency locking status*/               /**<CNcomment:��Ƶ״̬*/
    HI_UNF_TUNER_CONNECT_PARA_S stConnectPara;  /**<Actual frequency locking parameters*/    /**<CNcomment:ʵ����Ƶ����*/
}HI_UNF_TUNER_STATUS_S ;
/**Tuner blind scan type*/
/**CNcomment:TUNERäɨ��ʽ*/
typedef enum hiUNF_TUNER_BLINDSCAN_MODE_E
{
    HI_UNF_TUNER_BLINDSCAN_MODE_AUTO = 0,       /**<Blind scan automatically*/  /**<CNcomment:�Զ�ɨ��*/
    HI_UNF_TUNER_BLINDSCAN_MODE_MANUAL,         /**<Blind scan manually*/       /**<CNcomment:�ֶ�ɨ��*/
    HI_UNF_TUNER_BLINDSCAN_MODE_BUTT
} HI_UNF_TUNER_BLINDSCAN_MODE_E;

/**Definition of blind scan event type*/
/**CNcomment:TUNERäɨ�¼�*/
typedef enum hiUNF_TUNER_BLINDSCAN_EVT_E
{
    HI_UNF_TUNER_BLINDSCAN_EVT_STATUS,          /**<New status*/        /**<CNcomment:״̬�仯*/
    HI_UNF_TUNER_BLINDSCAN_EVT_PROGRESS,        /**<New Porgress */     /**<CNcomment:���ȱ仯*/
    HI_UNF_TUNER_BLINDSCAN_EVT_NEWRESULT,       /**<Find new channel*/  /**<CNcomment:��Ƶ��*/
    HI_UNF_TUNER_BLINDSCAN_EVT_BUTT
} HI_UNF_TUNER_BLINDSCAN_EVT_E;

/**Definition of tuner blind scan status*/
/**CNcomment:TUNERäɨ״̬*/
typedef enum hiUNF_TUNER_BLINDSCAN_STATUS_E
{
    HI_UNF_TUNER_BLINDSCAN_STATUS_IDLE,         /**<Idel*/              /**<����*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_SCANNING,     /**<Scanning*/          /**<ɨ����*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_FINISH,       /**<Finish*/            /**<�ɹ����*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_QUIT,         /**<User quit*/         /**<�û��˳�*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_FAIL,         /**<Scan fail*/         /**<ɨ��ʧ��*/
    HI_UNF_TUNER_BLINDSCAN_STATUS_BUTT
} HI_UNF_TUNER_BLINDSCAN_STATUS_E;

/**Structure of satellite TP*/
/**CNcomment:TUNERɨ��TP��Ϣ*/
typedef struct  hiUNF_TUNER_SAT_TPINFO_S
{
    HI_U32                         u32Freq;         /**<Downlink frequency, in kHz*/ /**<CNcomment:����Ƶ�ʣ���λ��kHz*/
    HI_U32                         u32SymbolRate;   /**<Symbol rate, in bit/s*/      /**<CNcomment:�����ʣ���λbps */
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;         /**<Polarization type*/          /**<CNcomment:������ʽ*/
} HI_UNF_TUNER_SAT_TPINFO_S;

#if 0
/**Result of tuner blind scan */
/**CNcomment:TUNERäɨ�����Ϣ*/
typedef union hiUNF_TUNER_BLINDSCAN_RESULT_U
{
    HI_UNF_TUNER_SAT_TPINFO_S stSat;                /**<Information of new channel*/ /**<CNcomment:��Ƶ����Ϣ*/
} HI_UNF_TUNER_BLINDSCAN_RESULT_U;
#endif

/**Notify structure of tuner blind scan */
/**CNcomment:TUNERäɨ֪ͨ��Ϣ*/
typedef union hiUNF_TUNER_BLINDSCAN_NOTIFY_U
{
    HI_UNF_TUNER_BLINDSCAN_STATUS_E* penStatus;             /**<Scanning status*/
    HI_U16*                          pu16ProgressPercent;   /**<Scanning progress*/
    HI_UNF_TUNER_SAT_TPINFO_S* pstResult;             /**<Scanning result*/
} HI_UNF_TUNER_BLINDSCAN_NOTIFY_U;

/**Parameter of the satellite tuner blind scan */
/**CNcomment:����TUNERäɨ����*/
typedef struct hiUNF_TUNER_SAT_BLINDSCAN_PARA_S
{
    /**<LNB Polarization type, only take effect in manual blind scan mode*/
    /**<CNcomment:LNB������ʽ���Զ�ɨ��ģʽ������Ч*/
    HI_UNF_TUNER_FE_POLARIZATION_E enPolar;
    
    /**<LNB 22K signal status, for Ku band LNB which has dual LO, 22K ON will select high LO and 22K off select low LO,
        only take effect in manual blind scan mode*/
    /**<CNcomment:LNB 22K״̬������Ku����˫����LNB��ONѡ��߱���OFFѡ��ͱ����Զ�ɨ��ģʽ������Ч*/
    HI_UNF_TUNER_FE_LNB_22K_E      enLNB22K;

    /**<Blind scan start IF, in kHz, only take effect in manual blind scan mode */
    /**<CNcomment:äɨ��ʼƵ��(��Ƶ)����λ��kHz���Զ�ɨ��ģʽ������Ч*/
    HI_U32                         u32StartFreq;   

    /**<Blind scan stop IF, in kHz, only take effect in manual blind scan mode */
    /**<CNcomment:äɨ����Ƶ��(��Ƶ)����λ��kHz���Զ�ɨ��ģʽ������Ч*/
    HI_U32                         u32StopFreq; 

    /**<The execution of the blind scan may change the 13/18V or 22K status. 
        If you use any DiSEqC device which need send command when 13/18V or 22K status change,
        you should registe a callback here. Otherwise, you can set NULL here.*/
    /**<CNcomment:äɨ���̿��ܻ��л�������ʽ��22K�����������ĳЩDiSEqC�豸��Ҫ����13/18V��22K�ģ�
        ��ע������ص������û���ã���ɴ�NULL */
    HI_VOID (*pfnDISEQCSet)(HI_U32 u32TunerId, HI_UNF_TUNER_FE_POLARIZATION_E enPolar,
                            HI_UNF_TUNER_FE_LNB_22K_E enLNB22K);

    /**<Callback when scan status change, scan progress change or find new channel.*/
    /**<CNcomment:ɨ��״̬����Ȱٷֱȷ����仯ʱ�������µ�Ƶ��ʱ�ص�*/
    HI_VOID (*pfnEVTNotify)(HI_UNF_TUNER_BLINDSCAN_EVT_E enEVT, HI_UNF_TUNER_BLINDSCAN_NOTIFY_U * punNotify);
} HI_UNF_TUNER_SAT_BLINDSCAN_PARA_S;

/**Parameter of the tuner blind scan */
/**CNcomment:TUNERäɨ����*/
typedef struct hiUNF_TUNER_BLINDSCAN_PARA_S
{
    HI_UNF_TUNER_BLINDSCAN_MODE_E enMode;
    union
    {
        HI_UNF_TUNER_SAT_BLINDSCAN_PARA_S stSat;
    } unScanPara;
} HI_UNF_TUNER_BLINDSCAN_PARA_S;


/**DiSEqC Level*/
typedef enum hiUNF_TUNER_DISEQC_LEVEL_E
{
    HI_UNF_TUNER_DISEQC_LEVEL_1_X,      /**<1.x, one way*/                  /**<1.x������ */
    HI_UNF_TUNER_DISEQC_LEVEL_2_X,      /**<2.x, two way, support reply*/   /**<2.x��˫��֧��Reply*/
    HI_UNF_TUNER_DISEQC_LEVEL_BUTT
} HI_UNF_TUNER_DISEQC_LEVEL_E;

/**Receive status of DiSEqC reply massage */
/**CNcomment:DiSEqC��Ϣ����״̬*/
typedef enum hiUNF_TUNER_DISEQC_RECV_STATUS_E
{
    HI_UNF_TUNER_DISEQC_RECV_OK,        /**<Receive successfully*/          /**<���ճɹ�*/
    HI_UNF_TUNER_DISEQC_RECV_UNSUPPORT, /**<Device don't support reply*/    /**<�豸��֧�ֻش�*/    
    HI_UNF_TUNER_DISEQC_RECV_TIMEOUT,   /**<Receive timeout*/               /**<���ճ�ʱ*/
    HI_UNF_TUNER_DISEQC_RECV_ERROR,     /**<Receive fail*/                  /**<���ճ���*/
    HI_UNF_TUNER_DISEQC_RECV_BUTT
} HI_UNF_TUNER_DISEQC_RECV_STATUS_E;

/**Structure of the DiSEqC send massage */
/**CNcomment:DiSEqC������Ϣ�ṹ*/
typedef struct hiUNF_TUNER_DISEQC_SENDMSG_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E     enLevel;        /**<Device device level*/
    HI_UNF_TUNER_SWITCH_TONEBURST_E enToneBurst;    /**<Tone Burst */
    HI_U8                           au8Msg[HI_UNF_DISEQC_MSG_MAX_LENGTH]; /**<Message data*/
    HI_U8                           u8Length;       /**<Message length*/
    HI_U8                           u8RepeatTimes;  /**<Message repeat times*/
} HI_UNF_TUNER_DISEQC_SENDMSG_S;

/**Structure of the DiSEqC reply massage */
/**CNcomment:DiSEqC������Ϣ�ṹ*/
typedef struct hiUNF_TUNER_DISEQC_RECVMSG_S
{
    HI_UNF_TUNER_DISEQC_RECV_STATUS_E enStatus;     /**<Recieve status*/
    HI_U8                             au8Msg[HI_UNF_DISEQC_MSG_MAX_LENGTH]; /**<Recieve message data*/
    HI_U8                             u8Length;     /**<Recieve message length*/
} HI_UNF_TUNER_DISEQC_RECVMSG_S;

#ifdef DISEQC_SUPPORT
/**DiSEqC Switch port */
typedef enum hiUNF_TUNER_DISEQC_SWITCH_PORT_E
{
    HI_UNF_TUNER_DISEQC_SWITCH_NONE = 0,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_1,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_2,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_3,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_4,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_5,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_6,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_7,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_8,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_9,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_10,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_11,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_12,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_13,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_14,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_15,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_16,
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_BUTT
} HI_UNF_TUNER_DISEQC_SWITCH_PORT_E;

/**Parameter for DiSEqC 1.0/2.0 switch
   Some DiSEqC device need set polarization(13/18V) and 22K, you can set them here. */
/**CNcomment:DiSEqC 1.0/2.0 ���ز���
   ��ЩDiSEqC�豸��Ҫ���ü�����ʽ��22K�ģ�������������豸����Ҫ���������� */
typedef struct hiUNF_TUNER_DISEQC_SWITCH4PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E       enLevel;  /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E enPort;   /**<DiSEqC switch port*/
    HI_UNF_TUNER_FE_POLARIZATION_E    enPolar;  /**<Polarization type */
    HI_UNF_TUNER_FE_LNB_22K_E         enLNB22K; /**<22K status*/
} HI_UNF_TUNER_DISEQC_SWITCH4PORT_S;

/**Parameter for DiSEqC 1.1/2.1 switch */
/**CNcomment:DiSEqC 1.1/2.1 ���ز��� */
typedef struct hiUNF_TUNER_DISEQC_SWITCH16PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E       enLevel;  /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E enPort;   /**<DiSEqC switch port*/
} HI_UNF_TUNER_DISEQC_SWITCH16PORT_S;

/**DiSEqC motor limit setting*/
/**CNcomment:DiSEqC��Ｋ������ */
typedef enum hiUNF_TUNER_DISEQC_LIMIT_E
{
    HI_UNF_TUNER_DISEQC_LIMIT_OFF,              /**<Disable Limits*/
    HI_UNF_TUNER_DISEQC_LIMIT_EAST,             /**<Set East Limit*/
    HI_UNF_TUNER_DISEQC_LIMIT_WEST,             /**<Set West Limit*/
    HI_UNF_TUNER_DISEQC_LIMIT_BUTT
} HI_UNF_TUNER_DISEQC_LIMIT_E;

/**Difinition of DiSEqC motor move direction*/
/**CNcomment:DiSEqC����ƶ����� */
typedef enum hiUNF_TUNER_DISEQC_MOVE_DIR_E
{
    HI_UNF_TUNER_DISEQC_MOVE_DIR_EAST,          /**<Move east*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_WEST,          /**<Move west*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_BUTT
} HI_UNF_TUNER_DISEQC_MOVE_DIR_E;

/**Difinition of DiSEqC motor move type*/
/**CNcomment:DiSEqC����ƶ���ʽ*/
typedef enum hiUNF_TUNER_DISEQC_MOVE_TYPE_E
{
    HI_UNF_TUNER_DISEQC_MOVE_STEP_SLOW,         /**<1 step one time, default*/
    HI_UNF_TUNER_DISEQC_MOVE_STEP_FAST,         /**<5 step one time*/
    HI_UNF_TUNER_DISEQC_MOVE_CONTINUE,          /**<Continuous moving*/
    HI_UNF_TUNER_DISEQC_MOVE_TYPE_BUTT
} HI_UNF_TUNER_DISEQC_MOVE_TYPE_E;

/**Parameter for DiSEqC motor store position*/
/**CNcomment:���ߴ洢λ�ò���*/
typedef struct hiUNF_TUNER_DISEQC_POSITION_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U32                      u32Pos;         /**<Index of position, 0-255*/
} HI_UNF_TUNER_DISEQC_POSITION_S;

/**Parameter for DiSEqC motor limit setting*/
/**CNcomment:����Limit���ò���*/
typedef struct hiUNF_TUNER_DISEQC_LIMIT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_LIMIT_E enLimit;        /**<Limit setting*/
} HI_UNF_TUNER_DISEQC_LIMIT_S;

/**Parameter for DiSEqC motor moving*/
/**CNcomment:DiSEqC����ƶ�����*/
typedef struct hiUNF_TUNER_DISEQC_MOVE_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E     enLevel;    /**<DiSEqC device level*/
    HI_UNF_TUNER_DISEQC_MOVE_DIR_E  enDir;      /**<Moving direction */
    HI_UNF_TUNER_DISEQC_MOVE_TYPE_E enType;     /**<Moving type */
} HI_UNF_TUNER_DISEQC_MOVE_S;

/**Parameter for DiSEqC motor recalculate*/
/**CNcomment:DiSEqC�����ؼ������*/
typedef struct hiUNF_TUNER_DISEQC_RECALCULATE_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U8                       u8Para1;        /**<Parameter 1 */
    HI_U8                       u8Para2;        /**<Parameter 2 */
    HI_U8                       u8Para3;        /**<Parameter 3 */
    HI_U8                       u8Reserve;      /**<Reserve */
} HI_UNF_TUNER_DISEQC_RECALCULATE_S;

/**Parameter for USALS*/
/**CNcomment:USALS ����*/
typedef struct hiUNF_TUNER_DISEQC_USALS_PARA_S
{
    HI_U16 u16LocalLongitude;   /**<local longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude) */
    HI_U16 u16LocalLatitude;    /**<local latitude, is 10*latitude, in param N:0-900, S:900-1800(1800-latitude)*/
    HI_U16 u16SatLongitude;     /**<sat longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude)  */
    HI_U16 u16Angular;          /**<calculate result, out param */
} HI_UNF_TUNER_DISEQC_USALS_PARA_S;

/**Parameter for USALS goto angular*/
/**CNcomment:USALS�ǶȲ���*/
typedef struct hiUNF_TUNER_DISEQC_USALS_ANGULAR_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E enLevel;        /**<DiSEqC device level*/
    HI_U16                      u16Angular;     /**<Angular, calculated by HI_UNF_TUNER_DISEQC_CalcAngular()*/
} HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S;
#endif /* DISEQC_SUPPORT */

/** @} */  /** <!-- ==== Structure Definition end ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_2 */
/** @{ */  /** <!-- [SCI] */

/**Output configuration of the smart card interface clock (SCICLK) pin*/
/**CNcomment:SCICLK�����������*/
typedef enum  hiUNF_SCI_CLK_MODE_E
{
    HI_UNF_SCI_CLK_MODE_CMOS = 0,   /**<Complementary metal-oxide semiconductor (CMOS) output*/   /**<CNcomment:CMOS���*/
    HI_UNF_SCI_CLK_MODE_OD,         /**<Open drain (OD) output*/                                  /**<CNcomment:OD���*/
    HI_UNF_SCI_CLK_MODE_BUTT
}HI_UNF_SCI_CLK_MODE_E;

/**SCI port*/
/**CNcomment:SCI �˿� */
typedef enum hiUNF_SCI_PORT_E
{
    HI_UNF_SCI_PORT0,      /**< SCI port 0*/  /**<CNcomment:SCI�˿�0*/
    HI_UNF_SCI_PORT1,      /**< SCI port 1*/  /**<CNcomment:SCI�˿�1*/
    HI_UNF_SCI_PORT_BUTT     
}HI_UNF_SCI_PORT_E;

/**Status of the SCI card*/
/**CNcomment:���ܿ�״̬ */
typedef enum hiUNF_SCI_STATUS_E
{
    HI_UNF_SCI_STATUS_UNINIT     = 0,   /**<The SCI card is not initialized.*/               /**<CNcomment: SCIδ��ʼ�� */
    HI_UNF_SCI_STATUS_FIRSTINIT,        /**<The SCI card is being initialized.*/             /**<CNcomment:SCI��ʼ��������*/
    HI_UNF_SCI_STATUS_NOCARD,           /**<There is no SCI card.*/                          /**<CNcomment:�޿� */
    HI_UNF_SCI_STATUS_INACTIVECARD,     /**<The SCI card is not activated (unavailable).*/   /**<CNcomment:��δ��ɼ������Ч�� */
 //   HI_UNF_SCI_STATUS_CARDFAULT,		  /**<The SCI card is faulty.*/                        /**<CNcomment:������*/
    HI_UNF_SCI_STATUS_WAITATR,          /**<The SCI card is waiting for the ATR data.*/      /**<CNcomment:�ȴ�ATR*/
    HI_UNF_SCI_STATUS_READATR,          /**<The SCI card is receiving the ATR data.*/        /**<CNcomment:���ڽ���ATR*/
    HI_UNF_SCI_STATUS_READY,            /**<The SCI card is available (activated).*/         /**<CNcomment:������ʹ�ã������ */
    HI_UNF_SCI_STATUS_RX,               /**<The SCI card is busy receiving data.*/           /**<CNcomment:��æ�����������У� */
    HI_UNF_SCI_STATUS_TX                /**<The SCI card is busy transmitting data.*/        /**<CNcomment:��æ�����������У� */
} HI_UNF_SCI_STATUS_E;

/**SCI protocol*/
/**CNcomment:SCI Э�� */
typedef enum hiUNF_SCI_PROTOCOL_E
{
    HI_UNF_SCI_PROTOCOL_T0,    /**<7816 T0 protocol*/   /**<CNcomment:7816 T0 Э�� */
    HI_UNF_SCI_PROTOCOL_T1,    /**<7816 T1 protocol*/   /**<CNcomment:7816 T1 Э�� */
    HI_UNF_SCI_PROTOCOL_T14 ,  /**<7816 T14 protocol*/  /**<CNcomment:7816 T14 Э�� */
    HI_UNF_SCI_PROTOCOL_BUTT
}HI_UNF_SCI_PROTOCOL_E;

/**SCI active level*/
/**CNcomment:SCI��Ч��ƽ*/
typedef enum hiUNF_SCI_LEVEL_E
{
    HI_UNF_SCI_LEVEL_LOW,    /**<Active low*/   /**<CNcomment:�͵�ƽ��Ч */
    HI_UNF_SCI_LEVEL_HIGH,   /**<Active high*/  /**<CNcomment:�ߵ�ƽ��Ч */
    HI_UNF_SCI_LEVEL_BUTT      
}HI_UNF_SCI_LEVEL_E ;

/**SCI system parameters*/
/**CNcomment:SCI ϵͳ���� */
typedef struct hiUNF_SCI_PARAMS_S
{
	HI_UNF_SCI_PORT_E enSciPort;		      /**<SCI port ID*/                                          	/**<CNcomment:SCI �˿ں� */
	HI_UNF_SCI_PROTOCOL_E enProtocolType; /**<Used protocol type*/                                    /**<CNcomment:ʹ�õ�Э������ */
	HI_U32 ActalClkRate;	                /**<Actual clock rate conversion factor F*/	                /**<CNcomment:ʵ�ʵ�F ֵʱ��ת������ */
	HI_U32 ActalBitRate;                	/**<Actual bit rate conversion factor D*/	                  /**<CNcomment:ʵ�ʵ�D ֵ������ת������ */
	HI_U32 Fi;			                      /**<Clock factor returned by the answer to reset (ATR)*/  	/**<CNcomment:ATR ���ص�ʱ������ */
	HI_U32 Di;			                      /**<Bit rate factor returned by the ATR*/                 	/**<CNcomment:ATR ���صı��������� */
	HI_U32 GuardDelay;	                  /**<Extra guard time N*/	                                  /**<CNcomment:N ֵ�������ӵı���ʱ��*/
	HI_U32 CharTimeouts;	                /**<Character timeout of T0 or T1*/                       	/**<CNcomment:T0 ��T1���ַ���ʱʱ��*/
	HI_U32 BlockTimeouts;	                /**<Block timeout of T1 */                                  /**<CNcomment:T1�Ŀ鳬ʱʱ��*/
	HI_U32 TxRetries;			                /**<Number of transmission retries*/                      	/**<CNcomment:�������Դ���*/
}HI_UNF_SCI_PARAMS_S,*HI_UNF_SCI_PARAMS_S_PTR;


/** @} */  /** <!-- ==== Structure Definition End ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_3 */
/** @{ */  /** <!-- [E2PROM] */

/**Defines the model of the electrically erasable programmable read-only memory (E2PROM).*/
/**CNcomment:E2PROM�ͺŶ���*/
typedef enum hiUNF_E2PROM_CHIP_E
{
    HI_UNF_E2PROM_CHIP_AT24C01,   /**<E2PROM model: AT24C01*/   /**<CNcomment:E2PROM�ͺţ�AT24C01 */
    HI_UNF_E2PROM_CHIP_AT24C02,   /**<E2PROM model: AT24C02*/   /**<CNcomment:E2PROM�ͺţ�AT24C02 */
    HI_UNF_E2PROM_CHIP_AT24C04,   /**<E2PROM model: T24C04*/    /**<CNcomment:E2PROM�ͺţ�AT24C04 */
    HI_UNF_E2PROM_CHIP_AT24C08,   /**<E2PROM model: AT24C08*/   /**<CNcomment:E2PROM�ͺţ�AT24C08 */
    HI_UNF_E2PROM_CHIP_AT24C16,   /**<E2PROM model: AT24C16*/   /**<CNcomment:E2PROM�ͺţ�AT24C16 */
    HI_UNF_E2PROM_CHIP_AT24C32,   /**<E2PROM model: AT24C32*/   /**<CNcomment:E2PROM�ͺţ�AT24C32 */
    HI_UNF_E2PROM_CHIP_AT24C64,   /**<E2PROM model: AT24C64*/   /**<CNcomment:E2PROM�ͺţ�AT24C64 */
    HI_UNF_E2PROM_CHIP_AT24C128,  /**<E2PROM model: AT24C128*/  /**<CNcomment:E2PROM�ͺţ�AT24C128 */
    HI_UNF_E2PROM_CHIP_AT24C256,  /**<E2PROM model: AT24C256*/  /**<CNcomment:E2PROM�ͺţ�AT24C256 */
    HI_UNF_E2PROM_CHIP_M24LC01,   /**<E2PROM model: M24LC01*/   /**<CNcomment:E2PROM�ͺţ�M24LC01 */
    HI_UNF_E2PROM_CHIP_M24LC02,   /**<E2PROM model: M24LC02*/   /**<CNcomment:E2PROM�ͺţ�M24LC02 */
    HI_UNF_E2PROM_CHIP_M24LC04,   /**<E2PROM model: M24LC04*/   /**<CNcomment:E2PROM�ͺţ�M24LC04 */
    HI_UNF_E2PROM_CHIP_M24LC08,   /**<E2PROM model: M24LC08*/   /**<CNcomment:E2PROM�ͺţ�M24LC08 */
    HI_UNF_E2PROM_CHIP_M24LC16,   /**<E2PROM model: M24LC16*/   /**<CNcomment:E2PROM�ͺţ�M24LC16 */
    HI_UNF_E2PROM_CHIP_M24LC32,   /**<E2PROM model: M24LC32*/   /**<CNcomment:E2PROM�ͺţ�M24LC32 */
    HI_UNF_E2PROM_CHIP_M24LC64,   /**<E2PROM model: M24LC64*/   /**<CNcomment:E2PROM�ͺţ�M24LC64 */
    HI_UNF_E2PROM_CHIP_M24LC128,  /**<E2PROM model: M24LC128*/  /**<CNcomment:E2PROM�ͺţ�M24LC128 */
    HI_UNF_E2PROM_CHIP_M24LC256,  /**<E2PROM model: M24LC256*/  /**<CNcomment:E2PROM�ͺţ�M24LC256 */

    HI_UNF_E2PROM_CHIP_BUTT
}HI_UNF_E2PROM_CHIP_E;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_4 */
/** @{ */  /** <!-- [IR] */

/**Code type supported by the infrared (IR) module*/
/**CNcomment:IR֧�ֵ�����*/
typedef enum hiUNF_IR_CODE_E
{
    HI_UNF_IR_CODE_NEC_SIMPLE = 0,  /**<NEC with simple repeat code*/   /**<CNcomment:NEC with simple repeat code����*/
    HI_UNF_IR_CODE_TC9012,          /**<TC9012 code*/                   /**<CNcomment:TC9012����*/
    HI_UNF_IR_CODE_NEC_FULL,        /**<NEC with full repeat code*/     /**<CNcomment:NEC with full repeat code����*/
    HI_UNF_IR_CODE_SONY_12BIT,      /**<SONY 12-bit code */             /**<CNcomment:SONY 12BIT����*/
    HI_UNF_IR_CODE_RAW,      	      /**<Raw code*/	                    /**<CNcomment:raw ����*/
    HI_UNF_IR_CODE_BUTT
}HI_UNF_IR_CODE_E;

/**List of IR code type. For details about HI_UNF_IR_STD, see HI_UNF_IR_CODE_E.*/
/**CNcomment:IR�����б�,HI_UNF_IR_STD���������HI_UNF_IR_CODE_E */
typedef enum hiUNF_IR_PROTOCOL_E
{
    HI_UNF_IR_NEC,
	  HI_UNF_IR_RC6A=10,
	  HI_UNF_IR_RC5,	
    HI_UNF_IR_LOW_LATENCY_PROTOCOL,
    HI_UNF_IR_RC6_MODE0,         /**<For RC6 mode 0*/
    HI_UNF_IR_RCMM,              /**<For RCMM 24/32 protocol*/
    HI_UNF_IR_RUWIDO,            /**<For Ruwido protocol*/
    HI_UNF_IR_RCRF8,             /**<For RCRF8 protocol*/
    HI_UNF_IR_MULTIPLE,          /**<For multiple protocol support*/
    HI_UNF_IR_RMAP,              /**<For RMAP protocol with Mancester coding*/
   	HI_UNF_IR_RSTEP,             /**<For RSTEP protocol*/
    HI_UNF_IR_RMAP_DOUBLEBIT,    /**<For RMAP protocol with Double bit coding*/
    HI_UNF_IR_LOW_LATENCY_PRO_PROTOCOL,
    HI_UNF_IR_XMP,
    HI_UNF_IR_USER_DEFINED,
    HI_UNF_IR_PROTOCOL_BUTT
} HI_UNF_IR_PROTOCOL_E;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_5 */
/** @{ */  /** <!-- [KEYLED] */
/**keyled type*/
/**CNcomment:KEYLED���� */
typedef enum  hiUNF_KEYLED_TYPE_E
{
    HI_UNF_KEYLED_TYPE_STD = 0x0, /**<KEYLED type inherent in HiSilicon chips*/   /**<CNcomment:��˼оƬ�Դ���KEYLED���� */
    HI_UNF_KEYLED_TYPE_PT6961,    /**<KEYLED type: PT6961*/                       /**<CNcomment:KEYLED�ͺţ�PT6961 */
    HI_UNF_KEYLED_TYPE_CT1642,    /**<KEYLED type: CT1642*/                       /**<CNcomment:KEYLED�ͺţ�CT1642 */
    HI_UNF_KEYLED_TYPE_PT6964,	  /**<KEYLED type: PT6964*/ 
    HI_UNF_KEYLED_TYPE_BUTT       
}HI_UNF_KEYLED_TYPE_E;

/**state of key*/
/**CNcomment:����״̬*/
typedef enum  hiUNF_KEY_STATUS_E
{
    HI_UNF_KEY_STATUS_DOWN = 0 ,   /**<Pressed*/   /**<CNcomment:���°��� */
    HI_UNF_KEY_STATUS_HOLD ,       /**<Hold*/      /**<CNcomment:��ס���� */
    HI_UNF_KEY_STATUS_UP ,         /**<Released*/  /**<CNcomment:̧�𰴼� */

    HI_UNF_KEY_STATUS_BUTT
}HI_UNF_KEY_STATUS_E ;

/**Blink frequency level of the LED*/
/**CNcomment:LED��˸Ƶ�ʼ���*/
typedef enum hiUNF_KEYLED_LEVEL_E
{
    HI_UNF_KEYLED_LEVEL_1 = 0x01,   /**<Level 1, slowest*/  /**<CNcomment:����1,��˸Ƶ������*/
    HI_UNF_KEYLED_LEVEL_2,          /**<Level 2*/           /**<CNcomment:����2 */
    HI_UNF_KEYLED_LEVEL_3,          /**<Level 3*/           /**<CNcomment:����3 */
    HI_UNF_KEYLED_LEVEL_4,          /**<Level 4*/           /**<CNcomment:����4 */
    HI_UNF_KEYLED_LEVEL_5,          /**<Level 5, fastest*/  /**<CNcomment:����5 , ��˸Ƶ�����*/

    HI_UNF_KEYLED_LEVEL_BUTT
}HI_UNF_KEYLED_LEVEL_E;

/**Blink sequence of LEDs*/
/**CNcomment:��˸��LED���*/
typedef enum hiUNF_KEYLED_LIGHT_E
{
    HI_UNF_KEYLED_LIGHT_1 = 0x01,   /**<The first LED blinks.*/   /**<CNcomment:��1��LED��˸*/
    HI_UNF_KEYLED_LIGHT_2,          /**<The second LED blinks.*/  /**<CNcomment:��2��LED��˸*/
    HI_UNF_KEYLED_LIGHT_3,          /**<The third LED blinks.*/   /**<CNcomment:��3��LED��˸*/
    HI_UNF_KEYLED_LIGHT_4,          /**<The fourth LED blinks.*/  /**<CNcomment:��4��LED��˸*/
    HI_UNF_KEYLED_LIGHT_ALL,        /**<*All LEDs blink.*/        /**<CNcomment:����LED����˸*/
    HI_UNF_KEYLED_LIGHT_NONE,       /**<All LEDs do not blink.*/  /**<CNcomment:����LED������˸*/

    HI_UNF_KEYLED_LIGHT_BUTT
}HI_UNF_KEYLED_LIGHT_E;

/**Display time of each LED*/
/**CNcomment:LED��ʾʱ��*/
typedef struct hiUNF_KEYLED_Time_S
{
    HI_U32 u32Hour;           /**<Hour*/   /**<CNcomment:ʱ*/
    HI_U32 u32Minute;         /**<Minute*/ /**<CNcomment:��*/
}HI_UNF_KEYLED_TIME_S, *HI_UNF_KEYLED_TIME_S_PTR;

/** @} */  /** <!-- ==== Structure Definition End ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_6 */
/** @{ */  /** <!-- [ARM MODE] */
/**ARM mode*/
/**CNcomment:ARM������ģʽ*/
typedef enum hiUNF_PMOC_MODE_E
{
    HI_UNF_PMOC_MODE_NORMAL = 0,    /**<Normal*/
    HI_UNF_PMOC_MODE_SLOW,          /**<Slow*/
    HI_UNF_PMOC_MODE_DOZE,          /**<Doze*/
    HI_UNF_PMOC_MODE_SLEEP,         /**<Sleep*/

    HI_UNF_PMOC_MODE_BUTT
}HI_UNF_PMOC_MODE_E;
/** @} */  /** <!-- ==== Structure Definition End ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_7 */
/** @{ */  /** <!-- [PM] */

/**standby woken type*/
/**CNcomment:���������ѵ����� */
typedef enum hiUNF_PMOC_ACTUAL_WKUP_E
{
     HI_UNF_PMOC_WKUP_IR  =0,     /**<Woken by the IR module*/                /**<CNcomment:��IR���� */
     HI_UNF_PMOC_WKUP_KEYLED,     /**<Woken by the keys on the front panel*/  /**<CNcomment:��ǰ���KEY���� */
     HI_UNF_PMOC_WKUP_TIMEOUT,    /**<Woken by the timing interrupt*/         /**<CNcomment:����ʱ�жϻ��� */
     HI_UNF_PMOC_WKUP_BUTT        
}HI_UNF_PMOC_ACTUAL_WKUP_E;

typedef enum hiUNF_PMOC_SCENE_E
{
     HI_UNF_PMOC_SCENE_STANDARD  =0, /**<Standard scenario*/                    /**<CNcomment:��׼���� */
     HI_UNF_PMOC_SCENE_ETH,          /**<Forward scenario over the ETH port */  /**<CNcomment:����ת������ */
     HI_UNF_PMOC_SCENE_PHONE,        /**<Calling scenario*/                     /**<CNcomment:ͨ������ */
     HI_UNF_PMOC_SCENE_BUTT        
}HI_UNF_PMOC_SCENE_E;

#define PMOC_WKUP_IRKEY_MAXNUM    6
/**Defines the standby wake-up conditions.*/
/**CNcomment:�������ѵ���������*/
typedef struct hiUNF_PMOC_WKUP_S
{
	/**<Number of supported values of the power key. For the raw IR remote control, the number cannot be greater than the maximum key value 5 that is defined by the macro definition PMOC_WKUP_IRKEY_MAXNUM. Only one key value is supported for other remote controls.*/	
  /**<CNcomment:�ܹ�֧��power��ֵ������raw �ͺ���ң��: ���ܳ�ԽPMOC_WKUP_IRKEY_MAXNUM(<=5)�궨������������ֵ����������ֻ��֧��һ����ֵ*/  
    HI_U32 u32IrPmocNum;	
    			
    HI_U32 u32IrPowerKey0[PMOC_WKUP_IRKEY_MAXNUM];	/**<Lower-bit value of the power key on an IR remote control*/	/**<CNcomment:����ң�� power ��λ��ֵ */
    HI_U32 u32IrPowerKey1[PMOC_WKUP_IRKEY_MAXNUM];	/**<Upper-bit value of the power key on an IR remote control*/	/**<CNcomment:����ң�� power ��λ��ֵ */
    HI_U32 u32KeypadPowerKey;   		              	/**<Value of the power key*/                                  	/**<CNcomment:���� power��ֵ */
    HI_U32 u32WakeUpTime ; 			                  	/**<Preconfigured Wake-up time, in second*/                    	/**<CNcomment:���Ѷ�ʱʱ�� ,��λΪ ��  */
}HI_UNF_PMOC_WKUP_S, *HI_UNF_PMOC_WKUP_S_PTR;

/**Configures the time displayed on the front panel in standby mode.*/
/**CNcomment:����ʱ��ǰ�����ʾ��ʱ������ */
typedef struct hiUNF_PMOC_TIME_S
{
    HI_U32  	u32Hour;       /**<Hour*/     /**<CNcomment:ʱ */
    HI_U32  	u32Minute;     /**<Minute*/   /**<CNcomment:�� */
    HI_U32  	u32Second;     /**<Second*/   /**<CNcomment:�� */
}HI_UNF_PMOC_TIME_S,*HI_UNF_PMOC_TIME_S_PTR;

/**Configures the display mode of the front panel in standby mode.*/
/**CNcomment:����ʱ��ǰ�����ʾ���� */
typedef struct hiUNF_PMOC_STANDBY_MODE_S
{
    HI_U32		u32Mode;	         /**<0: no display; 1: display the digits represented by u32DispCod; 2: display the time represented by stTimeInfo*/     /**<CNcomment:0 : ����ʾ 1 : ��ʾu32DispCode���������; 2 : ��ʾstTimeInfo�����ʱ��. */
    HI_U32		u32DispCode;	     /**<Digits displayed on the front panel when u32Mode is 1*/                                                             /**<CNcomment:u32ModeΪ1ʱ��ǰ�����ʾ������*/
	HI_UNF_PMOC_TIME_S stTimeInfo; /**<Time displayed on the front panel when u32Mode is 2*/	                                                             /**<CNcomment:u32ModeΪ2ʱ��ǰ�����ʾ��ʱ��*/
}HI_UNF_PMOC_STANDBY_MODE_S, *HI_UNF_PMOC_STANDBY_MODE_S_PTR;


/**Type of the device that is woken in standby mode*/
/**CNcomment:�������ѵ��豸���� */
typedef struct hiUNF_PMOC_DEV_TYPE_S
{
	HI_UNF_IR_CODE_E  irtype;	    /**<Type of the IR remote control*/	 /**<CNcomment:����ң������ */
	HI_UNF_KEYLED_TYPE_E  kltype; /**<Type of the front panel*/        /**<CNcomment:ǰ������� */
}HI_UNF_PMOC_DEV_TYPE_S, *HI_UNF_PMOC_DEV_TYPE_S_PTR;

/** @} */  /** <!-- ==== Structure Definition End ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_8 */
/** @{ */  /** <!-- [WDG] */
/**Response mode of the watchdog (WDG)*/
/**CNcomment:WDG��Ӧģʽ*/
typedef enum hiUNF_WDG_RESPOND_MODE_E
{
    HI_UNF_WDG_RESPOND_MODE_FIRSTRESET = 0, /**<The WDG resets the system for the first time when the WDG is not fed.*/                         /**<CNcomment:WDG��һ�θ�λ*/
    HI_UNF_WDG_RESPOND_MODE_SECONDRESET,    /**<The WDG interrupts the system for the first time, and resets the system for the second time.*/  /**<CNcomment:WDG��һ���жϣ��ڶ��θ�λ*/

    HI_UNF_WDG_RESPOND_MODE_BUTT
}HI_UNF_WDG_RESPOND_MODE_E;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_9 */
/** @{ */  /** <!-- [I2C] */

/**Rate type of the I2C module*/
/**CNcomment:I2C���������� */
typedef enum hiUNF_I2C_RATE_E
{
		HI_UNF_I2C_RATE_10K = 0,   /**<Standard rate: 10 kbit/s*/   /**<CNcomment:��׼���ʣ�10kbit/s*/
    HI_UNF_I2C_RATE_50K , 	   /**<Standard rate: 50 kbit/s*/   /**<CNcomment:��׼���ʣ�50kbit/s*/
    HI_UNF_I2C_RATE_100K ,     /**<Standard rate: 100 kbit/s*/  /**<CNcomment:��׼���ʣ�100kbit/s*/
    HI_UNF_I2C_RATE_200K ,     /**<Standard rate: 200 kbit/s*/  /**<CNcomment:��׼���ʣ�200kbit/s*/
    HI_UNF_I2C_RATE_300K ,     /**<Standard rate: 300 kbit/s*/  /**<CNcomment:��׼���ʣ�300kbit/s*/
    HI_UNF_I2C_RATE_400K,      /**<Fast rate: 400 kbit/s*/      /**<CNcomment:�������ʣ�400kbit/s*/

    HI_UNF_I2C_RATE_BUTT
}HI_UNF_I2C_RATE_E;

/** @} */  /** <!-- ==== Structure Definition End ==== */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_10 */
/** @{ */  /** <!-- [CIPHER] */


#define HI_UNF_CIPHER_MIN_CRYPT_LEN      8       /**min length of encrypt, unit: byte*/  /**<CNcomment: ��С�ӽ��ܳ��ȣ���λbyte */
#define HI_UNF_CIPHER_MAX_CRYPT_LEN      0xfffff /**max length of encrypt, unit: byte*/  /**<CNcomment: ���ӽ��ܳ��ȣ���λbyte */

/**CIPHER work mode*/
/**CNcomment:CIPHER����ģʽ*/
typedef enum hiHI_UNF_CIPHER_WORK_MODE_E
{
    HI_UNF_CIPHER_WORK_MODE_ECB = 0x0,   /**<Electronic codebook (ECB) mode*/    /**<CNcomment:ECBģʽ��*/
    HI_UNF_CIPHER_WORK_MODE_CBC = 0x1,   /**<Cipher block chaining (CBC) mode*/  /**<CNcomment:CBCģʽ��*/
    HI_UNF_CIPHER_WORK_MODE_CFB = 0x2,   /**<Cipher feedback (CFB) mode*/        /**<CNcomment:CFBģʽ��*/
    HI_UNF_CIPHER_WORK_MODE_OFB = 0x3,   /**<Output feedback (OFB) mode*/        /**<CNcomment:OFBģʽ��*/
    HI_UNF_CIPHER_WORK_MODE_CTR = 0x4,   /**<Counter (CTR) mode*/                /**<CNcomment:CTRģʽ��*/
    HI_UNF_CIPHER_WORK_MODE_BUTT = 0x5    

}HI_UNF_CIPHER_WORK_MODE_E;

/**Cipher algorithm*/
/**CNcomment:CIPHER�����㷨*/
typedef enum hiHI_UNF_CIPHER_ALG_E
{
    HI_UNF_CIPHER_ALG_DES  = 0x0,  /**<Data encryption standard (DES) algorithm*/     /**<CNcomment: DES�㷨*/
    HI_UNF_CIPHER_ALG_3DES = 0x1,  /**<3DES algorithm*/                               /**< CNcomment:3DES�㷨*/
    HI_UNF_CIPHER_ALG_AES  = 0x2,  /**<Advanced encryption standard (AES) algorithm*/ /**<CNcomment: AES�㷨*/
    HI_UNF_CIPHER_ALG_BUTT = 0x3   
}HI_UNF_CIPHER_ALG_E;

/**Key length*/
/**CNcomment: ��Կ����*/
typedef enum hiHI_UNF_CIPHER_KEY_LENGTH_E
{
    HI_UNF_CIPHER_KEY_AES_128BIT = 0x0,  /**<128-bit key for the AES algorithm*/ /**<CNcomment:AES���㷽ʽ�²���128bit��Կ����*/
    HI_UNF_CIPHER_KEY_AES_192BIT = 0x1,  /**<192-bit key for the AES algorithm*/ /**<CNcomment:AES���㷽ʽ�²���192bit��Կ����*/
    HI_UNF_CIPHER_KEY_AES_256BIT = 0x2,  /**<256-bit key for the AES algorithm*/ /**<CNcomment:AES���㷽ʽ�²���256bit��Կ����*/
    HI_UNF_CIPHER_KEY_DES_3KEY = 0x2,    /**<Three keys for the DES algorithm*/  /**<CNcomment:DES���㷽ʽ�²���3��key */
    HI_UNF_CIPHER_KEY_DES_2KEY = 0x3,    /**<Two keys for the DES algorithm*/    /**<CNcomment: DES���㷽ʽ�²���2��key  */
}HI_UNF_CIPHER_KEY_LENGTH_E;

/**Cipher bit width*/
/**CNcomment: ����λ��*/
typedef enum hiHI_UNF_CIPHER_BIT_WIDTH_E
{
    HI_UNF_CIPHER_BIT_WIDTH_64BIT  = 0x0,  /**<64-bit width*/   /**<CNcomment:64bitλ��*/
    HI_UNF_CIPHER_BIT_WIDTH_8BIT   = 0x1,  /**<8-bit width*/    /**<CNcomment:8bitλ��*/
    HI_UNF_CIPHER_BIT_WIDTH_1BIT   = 0x2,  /**<1-bit width*/    /**<CNcomment:1bitλ��*/
    HI_UNF_CIPHER_BIT_WIDTH_128BIT = 0x3,  /**<128-bit width*/  /**<CNcomment:128bitλ��*/
}HI_UNF_CIPHER_BIT_WIDTH_E;

//////////////////////////////////////////////
/**symbol of cipher control parameter exchange*/
/**CNcomment:���ܿ��Ʋ��������־ */
typedef struct hiUNF_CIPHER_CTRL_CHANGE_FLAG_S
{
    HI_U32   bit1IV:1;              /**<CNcomment:������� */    
    HI_U32   bitsResv:31;           /**<CNcomment:����  */
}HI_UNF_CIPHER_CTRL_CHANGE_FLAG_S;

/**symbol of using which key ladder for cipher*/
/**CNcomment:ʹ���ĸ�key ladder��־ */
typedef enum hiUNF_CIPHER_CA_TYPE_E
{
    HI_UNF_CIPHER_CA_TYPE_R2R = 0,    /**<Using R2R key ladder*/                                                    /**<CNcomment:ʹ��R2R key ladder */
    HI_UNF_CIPHER_CA_TYPE_SP  = 1,    /**<Using SP key ladder*/                                                    /**<CNcomment:ʹ��SP key ladder */
}HI_UNF_CIPHER_CA_TYPE_E;

/**Structure of the cipher control information*/
/**CNcomment:���ܿ�����Ϣ�ṹ */
typedef struct hiHI_UNF_CIPHER_CTRL_S
{
    HI_U32                              u32Key[8];    /**<Key*/                                                                                                       /**<CNcomment:��Կ*/
    HI_U32                              u32IV[4];     /**<Initialization vector (IV)*/                                                                                /**<CNcomment:��ʼ����*/
    HI_BOOL                             bKeyByCA;     /**<Encryption using advanced conditional access (CA) or decryption using keys*/                                /**<CNcomment:�Ƿ�ʹ�ø߰�ȫCA���ܻ����KEY*/
    HI_UNF_CIPHER_CA_TYPE_E             enCaType;     /**<Using the R2R key or SP key when using advanced CA*/                                                  /**< CNcomment:ʹ�ø߰�ȫCAʱʹ��SP Key��R2R Key*/
    HI_UNF_CIPHER_ALG_E                 enAlg;        /**<Cipher algorithm*/                                                                                          /**<CNcomment:�����㷨*/
    HI_UNF_CIPHER_BIT_WIDTH_E           enBitWidth;   /**<Bit width for encryption or decryption*/                                                                    /**<CNcomment:���ܻ���ܵ�λ��*/
    HI_UNF_CIPHER_WORK_MODE_E           enWorkMode;   /**<Operating mode*/                                                                                            /**<CNcomment:����ģʽ*/
    HI_UNF_CIPHER_KEY_LENGTH_E          enKeyLen;     /**<Key length*/                                                                                                /**<CNcomment:��Կ���� */
    HI_UNF_CIPHER_CTRL_CHANGE_FLAG_S    stChangeFlags;/**control information exchange choices, we default all woulde be change except they have been in the choices*/ /**<CNcomment:������Ϣ���ѡ�ѡ����û�б�ʶ����Ĭ��ȫ�����  */  
} HI_UNF_CIPHER_CTRL_S;

/**cipher data*/
/**CNcomment:�ӽ������� */
typedef struct hiHI_UNF_CIPHER_DATA_S
{
    HI_U32 u32SrcPhyAddr;     /**phy address of the original data*/   /**<CNcomment:Դ���������ַ*/
    HI_U32 u32DestPhyAddr;    /**phy address of the purpose data*/    /**<CNcomment:Ŀ�����������ַ*/
    HI_U32 u32ByteLength;     /**cigher data length*/                 /**<CNcomment:�ӽ������ݳ���*/
} HI_UNF_CIPHER_DATA_S;

typedef enum hiHI_UNF_CIPHER_HASH_TYPE_E
{
    HI_UNF_CIPHER_HASH_TYPE_SHA1 = 0x0,
    HI_UNF_CIPHER_HASH_TYPE_SHA256 = 0x1,
    HI_UNF_CIPHER_HASH_TYPE_BUTT,
}HI_UNF_CIPHER_HASH_TYPE_E;


/** @} */  /** <!-- ==== Structure Definition End ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_12 */
/** @{ */  /** <!-- [OTP] */
#define OTP_HDCP_KEY_LEN   320

typedef enum hiHI_UNF_OTP_AREATYPE_E
{
    HI_UNF_OTP_AREATYPE_HDCPKEY,
    HI_UNF_OTP_AREATYPE_BUTT
}HI_UNF_OTP_AREATYPE_E;



typedef struct hiUNF_DECRYPT_HDCP_S
{
	HI_U8 u8KSV[5];                         /**<HDCP KSV:40bits, Orignal data */
	HI_U8 u8PrivateKey[280];                /**<HDCP Device Private key:40*56bits, Orignal data */
}HI_UNF_DECRYPT_HDCP_S;

typedef struct hiUNF_ENCRYPT_HDCP_S
{
	HI_U8 u8EncryptKey[384];                /**<HDCP Encryption key */
}HI_UNF_ENCRYPT_HDCP_S;

typedef struct
{
    HI_BOOL EncryptionFlag;                 /**<HI_TRUE:Encryption, HI_FALSE: Uncryption */
    
    union
    {
        HI_UNF_DECRYPT_HDCP_S DecryptData;  /**<Orignal data, if EncryptionFlag == HI_FALSE */
        HI_UNF_ENCRYPT_HDCP_S EncryptData;  /**<Encrpytion data, if EncryptionFlag == HI_TRUE */
    }key;

    HI_U32 Reserved;                        /**<Reserved for future use */
}HI_UNF_OTP_HDCPKEY_S;

typedef struct
{
    HI_BOOL EncryptionFlag;                 /**<HI_TRUE:Encryption, HI_FALSE: Uncryption */
    
    union
    {
        HI_UNF_DECRYPT_HDCP_S DecryptData;  /**<Orignal data, if EncryptionFlag == HI_FALSE */
        HI_UNF_ENCRYPT_HDCP_S EncryptData;  /**<Encrpytion data, if EncryptionFlag == HI_TRUE */
    }key;

    HI_U32 Reserved;                        /**<Reserved for future use */
}HI_UNF_ADVCA_HDCPKEY_S;

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_4_12 */
/** @{ */  /** <!-- [CI] */

/**support CI Port*/
/**CNcomment:֧�ֵ�CI Port */
typedef enum hiUNF_CI_PORT_E
{
    HI_UNF_CI_PORT_0,
    HI_UNF_CI_PORT_1,
    HI_UNF_CI_PORT_BUTT
} HI_UNF_CI_PORT_E;

/**support CI PCMCIA Cards(Module)*/
/**CNcomment:֧�ֵ�CI PCMCIA Cards(Module) */
typedef enum hiUNF_CI_PCCD_E
{
    HI_UNF_CI_PCCD_A,
    HI_UNF_CI_PCCD_B,
    HI_UNF_CI_PCCD_BUTT
} HI_UNF_CI_PCCD_E;

/**Module on-off power control*/
/**CNcomment:Module���ص�Դ���� */
typedef enum hiUNF_CI_PCCD_CTRLPOWER_E
{
    HI_UNF_CI_PCCD_CTRLPOWER_ON,
    HI_UNF_CI_PCCD_CTRLPOWER_OFF,
    HI_UNF_CI_PCCD_CTRLPOWER_BUFF
} HI_UNF_CI_PCCD_CTRLPOWER_E;

/**Module ready state:ready/busy*/
/**CNcomment:Module����״̬:ready/busy */
typedef enum hiUNF_CI_PCCD_READY_E
{
    HI_UNF_CI_PCCD_BUSY,
    HI_UNF_CI_PCCD_READY
} HI_UNF_CI_PCCD_READY_E, *HI_UNF_CI_PCCD_READY_E_PTR;

/**Module online state:present/absent*/
/**CNcomment:Module����״̬:present/absent */
typedef enum hiUNF_CI_PCCD_STATUS_E
{
    HI_UNF_CI_PCCD_STATUS_ABSENT,
    HI_UNF_CI_PCCD_STATUS_PRESENT,
    HI_UNF_CI_PCCD_STATUS_BUTT
} HI_UNF_CI_PCCD_STATUS_E, *HI_UNF_CI_PCCD_STATUS_E_PTR;

/**card call mode*/
/**CNcomment:������ģʽ */
typedef enum hiUNF_CI_PCCD_ACCESSMODE_E
{
    HI_UNF_CI_PCCD_ACCESS_ATTR,
    HI_UNF_CI_PCCD_ACCESS_IO,
    HI_UNF_CI_PCCD_ACCESS_COMMON,
    HI_UNF_CI_PCCD_ACCESS_BUTT
} HI_UNF_CI_PCCD_ACCESSMODE_E;

/**get state register mask code*/
/**CNcomment:��ȡ״̬�Ĵ������� */
typedef enum hiUNF_CI_PCCD_STATUS_REG_E
{
    HI_UNF_CI_PCCD_STATUS_BIT_ALL,  /**get all bits*/               /**<CNcomment:��ȡ����λ */
    HI_UNF_CI_PCCD_STATUS_BIT_DA,   /**get Data Available bit*/     /**<CNcomment:��ȡData Availableλ */
    HI_UNF_CI_PCCD_STATUS_BIT_FR,   /**get Free bit*/               /**<CNcomment:��ȡFreeλ */
    HI_UNF_CI_PCCD_STATUS_BIT_RE,   /**get Read Error bit*/         /**<CNcomment:��ȡRead Errorλ */
    HI_UNF_CI_PCCD_STATUS_BIT_WE,   /**get Write Error bit*/        /**<CNcomment:��ȡWrite Errorλ */
    HI_UNF_CI_PCCD_STATUS_BIT_BUTT
} HI_UNF_CI_PCCD_STATUS_BIT_E;

/* Added begin 2012-04-24 l00185424: support various CI device */
/**The mode of CI Transport Stream Interface connection */
/**CNcomment:CI TSI���ӷ�ʽ */
typedef enum hiUNF_CI_TSI_MODE_E
{
    HI_UNF_CI_TSI_DAISY_CHAINED,      /* Daisy-chained */
    HI_UNF_CI_TSI_INDEPENDENT,        /* Independent, non-daisy-chained */
    HI_UNF_CI_TSI_BUTT
}HI_UNF_CI_TSI_MODE_E;

/**The mode of TS pass to the CI device*/
/**CNcomment:��CI�豸��TS��ģʽ */
typedef enum hiUNF_CI_TS_MODE_E
{
    HI_UNF_CI_TS_SERIAL,         /* serial */
    HI_UNF_CI_TS_PARALLEL,       /* Parallel TS, default */
    HI_UNF_CI_TS_USB2SERIAL,     /* USB input, serial output */
    HI_UNF_CI_TS_USB2PARALLEL,   /* USB input, parallel output */
    HI_UNF_CI_TS_BUTT
}HI_UNF_CI_TS_MODE_E;

/**CI device*/
/**CNcomment:֧�ֵ�CI�豸 */
typedef enum hiUNF_CI_DEV_E
{
    HI_UNF_CI_DEV_CIMAX,        /* CIMaX, default */
    HI_UNF_CI_DEV_CIMAXPLUS,    /* CIMaX+ */
    HI_UNF_CI_DEV_HICI,         /* HICI */
    HI_UNF_CI_DEV_STARCI2WIN,   /* StarCI2Win */
    HI_UNF_CI_DEV_BUTT
}HI_UNF_CI_DEV_E;

/**CIMaX device configuration*/
/**CNcomment:CIMaX�豸���� */
typedef struct hiUNF_CI_CIMAX_ATTR_S
{
    HI_U32  u32SMIBitWidth;
    HI_U32  u32SMIBaseAddr;         /* SMI Base Address */
    HI_U32  u32I2cNum;              /* I2C Group */
    HI_U8   u8DevAddress;           /* I2C device address */
}HI_UNF_CI_CIMAX_ATTR_S, *HI_UNF_CI_CIMAX_ATTR_S_PTR;

/**CIMaX+ device configuration*/
/**CNcomment:CIMaX+�豸���� */
typedef struct hiUNF_CI_CIMAXPLUS_ATTR_S
{
    HI_U32  u32ResetGpioNo; /* The GPIO to reset the device */
}HI_UNF_CI_CIMAXPLUS_ATTR_S, *HI_UNF_CI_CIMAXPLUS_ATTR_S_PTR;

/**CI device configuration*/
/**CNcomment:CI�豸���� */
typedef struct hiUNF_CI_ATTR_S
{
    HI_UNF_CI_DEV_E enDevType;      /* CI device type */    
    HI_UNF_CI_TSI_MODE_E enTSIMode; /* The mode of CI Transport Stream Interface connection */
    /* The mode of TS pass to the CI device */
    HI_UNF_CI_TS_MODE_E enTSMode[HI_UNF_CI_PCCD_BUTT]; 
    union
    {
        HI_UNF_CI_CIMAX_ATTR_S stCIMaX;
        HI_UNF_CI_CIMAXPLUS_ATTR_S stCIMaXPlus;
    }unDevAttr;
}HI_UNF_CI_ATTR_S, *HI_UNF_CI_ATTR_S_PTR;
/* Added end 2012-04-24 l00185424: support various CI device */
/**TS control command*/
/**CNcomment:TS�������� */
typedef enum hiUNF_CI_PCCD_TSCTRL_E
{
    HI_UNF_CI_PCCD_TSCTRL_BYPASS,
    HI_UNF_CI_PCCD_TSCTRL_SETMODE,
    HI_UNF_CI_PCCD_TSCTRL_WRITETS,
    HI_UNF_CI_PCCD_TSCTRL_BUTT
}HI_UNF_CI_PCCD_TSCTRL_E;
/**USB TS Write parameter*/
/**CNcomment:USB�������� */
typedef struct hiUNF_CI_PCCD_TSWRITE_S
{
    HI_U8* pu8Data;     /**<Write data [in]*/
    HI_U32 u32Size;     /**<Write data size [in]*/
}HI_UNF_CI_PCCD_TSWRITE_S;

/**TS control parameter*/
/**CNcomment:TS���Ʋ��� */
typedef union 
{
    struct
    {
        HI_BOOL bByPass;    /**<TURE means bypass, FALSE means pass-through*/  /**<CNcomment:TRUE��ʾbypass, FALSE��ʾpass-through */
    }stByPass;

    struct
    {
        HI_UNF_CI_TS_MODE_E enTSMode;        /**<TS mode: parallel/serial/USB */
    }stMode;

    HI_UNF_CI_PCCD_TSWRITE_S stWrite;
    
    /* For extend */
}HI_UNF_CI_PCCD_TSCTRL_PARAM_U;


/** @} */  /** <!-- ==== Structure Definition end ==== */


//////////////////////////////////////////////
//////////////////////////////////////////////

/******************************* API declaration *****************************/
/** \addtogroup      H_1_4_1 */
/** @{ */  /** <!-- [SCI] */
/** 
\brief Initializes the SCI.   CNcomment:\brief SCI��ʼ��
\attention \n
\param  N/A                   CNcomment:\param ��
\retval 0 Success             CNcomment:\retval 0 �ɹ�

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_Init(HI_VOID);

/** 
\brief Deinitializes the SCI. CNcomment:\brief SCIȥ��ʼ��
\attention \n
\param  N/A                   CNcomment:\param ��
\retval 0 Success             CNcomment:\retval 0 �ɹ�

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_DeInit(HI_VOID);

/** 
\brief Starts an SCI device to initialize it based on the input port and protocol. If there is an SCI card, the card is also initialized.
CNcomment:\brief ��SCI��Smart Card Interface���豸���������Ķ˿ں�Э�飬����SCI�ӿ��豸�ĳ�ʼ������������ڣ���ͬʱ�Կ����г�ʼ��

\attention \n
This application programming interface (API) cannot be called repeatedly.\n
After an SCI device is started, it is initialized based on the default configuration.\n
If you modify the configuration, you need to call HI_UNF_SCI_ResetCard for the modifications to take effect.\n
If you remove and then insert the SCI card, you need to call HI_UNF_SCI_ResetCard to reset the card.
You need to set frequency parameters when enabling an SCI device. The actual SCI clock provided by the chip is obtained by using the clock divider.\n
The clock divider is calculated based on the externally transferred clock frequency parameters. The fractional part of the clock divider is discarded during calculation. \n
Therefore, an enhanced error occurs when the SCI clock is obtained by dividing the system clock by the clock divider. You need to set the actual output frequency of the \n
SCI card based on the close output frequency. The SCI clock is calculated as follows: Fsci_clk = Frefclk/[(Clkicc + 1) x 2]. Where, Frefclk is the 96 MHz system clock, \n
Fsci_clk is the SCI clock required by peripherals, and clkicc is the required clock divider of registers. clkicc is calculated as follows: Clkicc = Frefclk/(2 x Fsci_clk) - 1. \n
The clkicc may be a floating point value, but only the integral part is used. Therefore, the configured frequency is different from the actual frequency.\n

CNcomment: �ظ��򿪻�ʧ��\n
CNcomment: �򿪺�SCI�豸����Ĭ�����ö��豸���г�ʼ��\n
CNcomment: ֮��������������ã���Ҫ����HI_UNF_SCI_ResetCard��ʹ������Ч\n
CNcomment: ֮������Կ����аβ壬��Ҫ����HI_UNF_SCI_ResetCard�����и�λ��
CNcomment: ���豸��ʱ����Ҫ����Ƶ�ʲ�������оƬʵ�ʸ�SCI ��ʱ�������ⲿ�����
CNcomment: ʱ��Ƶ�ʲ�������ķ�Ƶ���ӷ�Ƶ��������Ϊ���㹫ʽ����������ķ�Ƶ���ӵ�С�����֣�
CNcomment: ����ͨ��ϵͳʱ�ӷ�Ƶ֮�������ʱ�ӻ����������ʵ�����ÿ�����������ӽ���ֵ��
CNcomment: ���ú�Ȩ�⣬�����㷨:Fsci_clk = Frefclk/((clkicc+1)*2);����Frefclk��ϵͳ96Mʱ�ӣFFsci_clk ���ⲿ��Ҫ���õ�
CNcomment: sciʱ��,ʵ��Ҫ����Ĵ�����Ƶ����clkicc = Frefclk/(2*Fsci_clk) -1;clkicc ��ʱ���������Ǹ���������ֻȡ����
CNcomment: �������õ�Ƶ����ʵ��Ƶ����ƫ�

\param[in] enSciPort     ID of an SCI port. The port ID can be 0 or 1.  CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1
\param[in] enSciProtocol Protocol type                                  CNcomment:Э������
\param[in] u32Frequency    Frequency of the SCI card to be set, in kHz. For the T0 and T1 cards, the frequency ranges from 1 MHz to 5 MHz; for the T14 card, the frequency must be set to 6 MHz.  CNcomment:Ҫ���õ�SCI��Ƶ�ʡ�����T0��T1����֧��Ƶ��1MHz��5MHz������T14����ֻ֧��6MHz����λΪkhz
\retval 0 Success                                                       CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  SCI  No SCI device is started.           CNcomment:�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT   The SCI device has been started.     CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.             CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.            CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT The SCI device fails to be started.    CNcomment:��SCI�豸ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_Open(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency);

/** 
\brief Stops an SCI device.
CNcomment:\brief �ر�SCI�豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ��رջ�ʧ�ܡ�

\param[in] enSciPort     ID of an SCI port. The port ID can be 0 or 1.  CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1
\retval 0 Success                                                       CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.               CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.              CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.            CNcomment:�Ƿ�����
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_Close(HI_UNF_SCI_PORT_E enSciPort);


/** 
\brief Performs a warm reset on an SCI card.
CNcomment:\brief �ȸ�λSCI����

\attention \n
If no SCI card is inserted, the reset fails.\n
If you modify the configuration of an SCI device, you need to call HI_S32 HI_UNF_SCI_ResetCard for the modifications to take effect.\n
In addition, if you remove and insert an SCI card after calling HI_UNF_SCI_Open, you also need to call HI_S32 HI_UNF_SCI_ResetCard to reset the card.
CNcomment:û�в��뿨����λ��ʧ��\n
CNcomment: ��������SCI�豸�����ú���Ҫ���øýӿ�ʹ������Ч\n
CNcomment: �ڵ��ô�SCI�豸�ӿں󣬶Կ����аβ壬Ҳ��Ҫ���øýӿڽ��и�λ��

\param[in] enSciPort   ID of an SCI port. The port ID can be 0 or 1                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in] bWarmResetValid   Reset mode. The value can be 0 or 1. 0: warm reset; 1: cold reset (recommended)    CNcomment:��λ��ʽ��ȡֵ��ΧΪ0��1��0:warm reset,1:cold reset���Ƽ������ַ�ʽ����
\retval 0 Success                                                                                               CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT The SCI device has been started.                                               CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.                                                     CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET A warm reset fails to be performed.                                           CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE The SCI card fails to be deactivated.                                      CNcomment:ȥ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_ResetCard(HI_UNF_SCI_PORT_E enSciPort, HI_BOOL bWarmResetValid);


/** 
\brief Deactivates an SCI card.
CNcomment:\brief ȥ����SCI����

\attention \n
After an SCI card is deactivated, the card cannot be read or written until it is reset.
CNcomment:ȥ������޷���д���ݡ�ֻ�����¸�λ���󣬲��ܼ�����д��

\param[in] enSciPort ID of an SCI port. The port ID can be 0 or 1.         CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\retval 0   Success                                                        CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT No SCI device is started.                    CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT   The SCI device has been started.        CNcomment: SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.                 CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.               CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.      CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET A warm reset fails to be performed.      CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE The SCI card fails to be deactivated. CNcomment:ȥ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_DeactiveCard(HI_UNF_SCI_PORT_E enSciPort);


/** 
\brief Obtains the ATR data of an SCI card.
CNcomment:\brief ��ȡSCI��ATR���ݡ�

\attention \n
N/A 
\param[in]  enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                            CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in]  pu8AtrBuf   Length of the ATR data read from the buffer                                             CNcomment:ATR ���ݶ�ȡ buffer ���ȡ�
\param[in] u32AtrBufSize  Actual number of ATR data segments                                                    CNcomment:ʵ�ʻ�ȡ��ATR���ݸ�����
\param[in]  pu8AtrRcvCount Address for storing the obtained ATR data                                            CNcomment:��ȡ��ATR���ݵĴ洢��ַ��
\retval 0   Success                                                                                             CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                       CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.                                                      CNcomment: �Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment: ��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET A warm reset fails to be performed.                                           CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS  The card status fails to be obtained.                                   CNcomment: ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment: ���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE The reception operation fails.                                              CNcomment: ���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL The block timeout fails to be set.                                      CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN The length of the data that is not transmitted fails to be obtained.   CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN The VCCEN signal fails to be set.                                       CNcomment: ����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS The card status fails to be obtained in block mode.                  CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT The port ID is incorrect.                                                     CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL The protocol type is incorrect.                                           CNcomment:�����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY The clock frequency is invalid.                                          CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH The length is invalid.                                                       CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT The data reception times out.                                              CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL The timeout is invalid.                                                CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL The level is invalid.                                                    CNcomment:�Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT Obtaining the card status times out.                                    CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE  The operating mode of the CLK signal fails to be set.                CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_GetATR(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pu8AtrBuf, HI_U32 u32AtrBufSize, HI_U8 *pu8AtrRcvCount);


/** 
\brief Obtains the status of an SCI card.
CNcomment:\brief ��ȡSCI��״̬��

\attention \n
This API is a non-block API. You can transfer data to an SCI card by calling HI_UNF_SCI_Send or HI_UNF_SCI_Receive only when the card status is HI_UNF_SCI_STATUS_READY.
CNcomment:�˽ӿ����������������ڿ���״̬ΪHI_UNF_SCI_STATUS_READYʱ���ſɵ���HI_UNF_SCI_Send��HI_UNF_SCI_Receive�ӿ��뿨�������ݽ���\n

\param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[out] penSCIStatus  Status of an SCI card                                                                 CNcomment: SCI��״̬��
\retval 0 Success                                                                                               CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.                                                      CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET  A warm reset fails to be performed.                                          CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE The SCI card fails to be deactivated.                                      CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment: ��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS The card status fails to be obtained.                                    CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE  The reception operation fails.                                             CNcomment: ���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL The block timeout fails to be set.                                      CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN  The length of the data that is not transmitted fails to be obtained.  CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN  The VCCEN signal fails to be set.                                      CNcomment:����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS The card status fails to be obtained in block mode.                  CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT The port ID is incorrect.                                                     CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL  The protocol type is incorrect.                                          CNcomment: �����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY  The clock frequency is invalid.                                         CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH  The length is invalid.                                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT The data reception times out.                                              CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL The level is invalid.                                                    CNcomment:�Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT  Obtaining the card status times out.                                   CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE The operating mode of the CLK signal fails to be set.                 CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_GetCardStatus(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_STATUS_E *penSciStatus);


/** 
\brief Transmits data to an SCI card.
CNcomment:\brief ��SCI���������ݡ�

\attention \n
Data must be transmitted based on application protocols.\n
Data cannot be transmitted consecutively.\n
In addition, data is transmitted and received in block mode. Therefore, when a large number of data is being transmitted, the transmission may fail if the timeout is too small.
CNcomment:����������Ҫ����Ӧ��Э��������\n
CNcomment: �����������η��Ͷ��м䲻��������\n
CNcomment: ���Ͷ�ȡ���������ģ���˷��ʹ���������ʱ���ܵ���ʱʱ������ƣ������ʱֵ�Ƚ϶̣��п��ܷ���ʧ�ܡ�
            
\param[in] enSciPort   ID of an SCI port. The port ID can be 0 or 1.                                            CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in]  pSciSendBuf Address for storing the data to be transmitted                                          CNcomment:�������ݵĴ洢��ַ��
\param[in] u32SendLen  Number of data segments (in byte) to be transmitted                                      CNcomment:�������ݵĸ���,��λΪ BYTE��
\param[in]  pu32ActLen Number of transmitted data segments (in byte)                                            CNcomment:ʵ�ʷ������ݸ���,��λΪ BYTE��
\param[in] u32TimeoutMs Wait timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block                        CNcomment:�ȴ���ʱֵ, ��λ�Ǻ���, 0 - ������, 0xFFFFFFFF-����������
\retval 0 Success                                                                                               CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  SCI  No SCI device is started.                                                   CNcomment:�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT The SCI device has been started.                                               CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.                                                     CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                                     CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET A warm reset fails to be performed.                                           CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS  The card status fails to be obtained.                                   CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment: ���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE The reception operation fails.                                              CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL  The block timeout fails to be set.                                     CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN The length of the data that is not transmitted fails to be obtained.   CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN The VCCEN signal fails to be set.                                       CNcomment:����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS The card status fails to be obtained in block mode.                  CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT The port ID is incorrect.                                                     CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL The protocol type is incorrect.                                           CNcomment:�����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY  The clock frequency is invalid.                                         CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH  The length is invalid.                                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT  The data reception times out.                                             CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL The level is invalid.                                                    CNcomment: �Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT  Obtaining the card status times out.                                   CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE  The operating mode of the CLK signal fails to be set.                CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_Send(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf, HI_U32 u32SendLen, HI_U32 *pu32ActLen, HI_U32 u32TimeoutMs);


/** 
\brief Receives data from an SCI card.
CNcomment:\brief ��SCI���������ݡ�

\attention \n
You must set the data length obtained by each upper-layer application based on the protocol. If the length of the obtained data is greater than that of the returned data, this API is returned after timeout occurs.
CNcomment:�ϲ�Ӧ�ó���������Э������������ȡ�ĳ��ȣ����ϣ����ȡ�ĳ��ȳ���ʵ���ܹ����صĳ��ȣ���ֻ�ܵȵ���ʱ���ڲ��ܷ��ء�

\param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in]  pSciReceiveBuf   Address for storing the received data                                              CNcomment:�������ݵĴ洢��ַ��
\param[in] u32ReceiveLen  Number of data segments (in byte) to be received                                      CNcomment:�����������ݵĸ���,,��λΪ BYTE��
\param[in]  pu32ActLen   Number of received data segments (in byte)                                             CNcomment:ʵ�ʽ������ݸ���,��λΪ BYTE��
\param[in] u32TimeOutMs  Wait timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block                       CNcomment:�ȴ���ʱֵ, ��λ�Ǻ���, 0 - ������, 0xFFFFFFFF-����������
\retval 0 Success                                                                                               CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                       CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.                                                     CNcomment: �Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT The SCI device fails to be started.                                            CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET  A warm reset fails to be performed.                                          CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS The card status fails to be obtained.                                    CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE  The reception operation fails.                                             CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL The block timeout fails to be set.                                      CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN The length of the data that is not transmitted fails to be obtained.   CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN  The VCCEN signal fails to be set.                                      CNcomment: ����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT  The DETECT signal fails to be set.                                    CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS   The card status fails to be obtained in block mode.                CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT The port ID is incorrect.                                                     CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT  The data reception times out.                                             CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT Obtaining the card status times out.                                    CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE The operating mode of the CLK signal fails to be set.                 CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_Receive(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciReceiveBuf, HI_U32 u32ReceiveLen, HI_U32 *pu32ActLen, HI_U32 u32TimeOutMs);


/** 
\brief Sets the active level of the VCCEN signal.
CNcomment:\brief ����VCCEN�ź��ߵ���Ч��ƽ��
\attention \n
The active level needs to be set based on the circuits of hardware, and the low level is active by default.\n
After changing the active level, you need to call HI_UNF_SCI_ResetCard for the modification take effect.
CNcomment:��Ҫ����Ӳ����·�������ã�Ĭ��Ϊ�͵�ƽ��Ч\n
CNcomment: ���Ĵ������ú���Ҫ����HI_UNF_SCI_ResetCard����ʹ�µ�������Ч��
         
\param[in] enSciPort ID of an SCI port. The port ID can be 0 or 1.                                              CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in] enSciVcc  Active level of a signal. HI_FALSE: active low; HI_TRUE: active high  Active level of a signal. HI_FALSE: active low; HI_TRUE: active high  CNcomment:�ź��ߵ���Ч��ƽ��HI_FALSE���͵�ƽ��Ч��HI_TRUE���ߵ�ƽ��Ч��
\retval 0   Success                                                                                             CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                                       CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT   The SCI device has been started.                                             CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.                                                     CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET  A warm reset fails to be performed.                                          CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS The card status fails to be obtained.                                    CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND The transmission operation fails.                                              CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE  The reception operation fails.                                             CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL The block timeout fails to be set.                                      CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN he length of the data that is not transmitted fails to be obtained.    CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN The VCCEN signal fails to be set.                                       CNcomment:����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS  The card status fails to be obtained in block mode.                 CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT  The data reception times out.                                             CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL The level is invalid.                                                    CNcomment: �Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT Obtaining the card status times out.                                    CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE  The operating mode of the CLK signal fails to be set.                CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_ConfigVccEn(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciVcc);


/** 
\brief Sets the active level of the DETECT signal.
CNcomment:\brief ����DETECT�ź��ߵ���Ч��ƽ��

\attention \n
The active level needs to be set based on the circuits of hardware, and the high level is active by default.\n
After changing the active level, you need to call HI_UNF_SCI_ResetCard for the modification take effect.
CNcomment:��Ҫ����Ӳ����·�������ã�Ĭ��Ϊ�ߵ�ƽ��Ч\n
CNcomment: ���Ĵ������ú���Ҫ����HI_UNF_SCI_ResetCard����ʹ�µ�������Ч��
        
\param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in] enSciDetect Active level of a signal. HI_FALSE: active low; HI_TRUE: active high                     CNcomment:�ź��ߵ���Ч��ƽ��HI_FALSE���͵�ƽ��Ч��HI_TRUE���ߵ�ƽ��Ч��
\retval 0  Success                                                                                              CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.                                                      CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET   A warm reset fails to be performed.                                         CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS The card status fails to be obtained.                                    CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE  The reception operation fails.                                             CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL  The block timeout fails to be set.                                     CNcomment: ����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN  The length of the data that is not transmitted fails to be obtained.  CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN  The VCCEN signal fails to be set.                                      CNcomment:����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT  The DETECT signal fails to be set.                                    CNcomment: ����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS The card status fails to be obtained in block mode.                  CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT  The port ID is incorrect.                                                    CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL  The protocol type is incorrect.                                          CNcomment:�����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY  The clock frequency is invalid.                                         CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH  The length is invalid.                                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT   The data reception times out.                                            CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL  The level is invalid.                                                   CNcomment:�Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT  Obtaining the card status times out.                                   CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE  The operating mode of the CLK signal fails to be set.                CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_ConfigDetect(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_LEVEL_E enSciDetect);


/** 
\brief Sets the mode of a clock signal.
CNcomment:\brief ����ʱ���ߵ�ģʽ��
\attention \n
The mode needs to be set based on the circuits of hardware, and the OD mode is selected by default.\n
After changing the mode, you need to call HI_UNF_SCI_ResetCard for the modification take effect.
CNcomment:��Ҫ����Ӳ����·�������ã�Ĭ��ΪODģʽ\n
CNcomment: ���Ĵ������ú���Ҫ����HI_UNF_SCI_ResetCard����ʹ�µ�������Ч��

\param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in] enClkMode  Mode of a clock signal                                                                    CNcomment:ʱ���ߵ�ģʽ��
\retval 0 Success                                                                                               CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT  The pointer is invalid.                                                     CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET  A warm reset fails to be performed.                                          CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS  The card status fails to be obtained.                                   CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE   The reception operation fails.                                            CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL The block timeout fails to be set.                                      CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN  The length of the data that is not transmitted fails to be obtained.  CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN  The VCCEN signal fails to be set.                                      CNcomment: ����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS  The card status fails to be obtained in block mode.                 CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT  The port ID is incorrect.                                                    CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL The protocol type is incorrect.                                           CNcomment:�����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY   The clock frequency is invalid.                                        CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH  The length is invalid.                                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT  The data reception times out.                                             CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL  The level is invalid.                                                   CNcomment:�Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT   Obtaining the card status times out.                                  CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE The operating mode of the CLK signal fails to be set.                 CNcomment:����CLK�ź��߹���ģʽʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_SCI_ConfigClkMode(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_CLK_MODE_E enClkMode);


/** 
\brief Changes the card slot.
CNcomment:\brief �л����ۡ�

\attention \n
When you call HI_UNF_SCI_Open, the primary card is used by default, that is, port 0 is used.\n
After you switch to a new SCI device, all the preceding SCI APIs are called to operate the new SCI device.\n
In addition, after a new SCI device is switched, the device is initialized based on the default configuration.\n
If any modifications are made, you need to call HI_UNF_SCI_ResetCard for the modifications to take effect.\n
If the SCI port to be switched to is the same as the current active port, an error code is returned.
CNcomment:����HI_UNF_SCI_Openʱ��Ĭ�ϴ򿪵�Ϊ���������˿�0\n
CNcomment: �л����µ�SCI�豸����������SCI�ӿڼ����µ�SCI�豸���в���\n
CNcomment: �л����µ�SCI�豸����Ĭ�����ö��豸���г�ʼ��\n
CNcomment: ֮��������������ã���Ҫ����HI_UNF_SCI_ResetCard��ʹ������Ч\n
CNcomment: ϣ���л�����SCI�˿��뵱ǰ��Ч�˿���ͬʱ���᷵��ʧ�ܡ�

\param[in] enSciPort  ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1��
\param[in] enSciProtocol  Protocol type                                                                         CNcomment:Э�����͡�
\param[in] u32Frequency Frequency of the SCI card to be set, in kHz. For the T0 and T1 cards, the frequency ranges from 1 MHz to 5 MHz; for the T14 card, the frequency must be set to 6 MHz. CNcomment:Ҫ���õ�SCI��Ƶ�ʡ�����T0��T1����֧��Ƶ��1MHz~5MHz������T14����ֻ֧��6MHz����λΪkHz��
\retval 0  Success                                                                                              CNcomment:�ɹ�
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                        CNcomment:SCI�豸δ��
\retval ::HI_ERR_SCI_REPEAT_INIT  The SCI device has been started.                                              CNcomment:SCI�豸�ظ���
\retval ::HI_ERR_SCI_INVALID_POINT The pointer is invalid.                                                      CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                                    CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_FAILED_INIT  The SCI device fails to be started.                                           CNcomment:��SCI�豸ʧ��
\retval ::HI_ERR_SCI_FAILED_RESET A warm reset fails to be performed.                                           CNcomment:�ȸ�λʧ��
\retval ::HI_ERR_SCI_FAILED_DEACTIVE  The SCI card fails to be deactivated.                                     CNcomment:ȥ����ʧ��
\retval ::HI_ERR_SCI_FAILED_GETATR  The ATR data fails to be obtained.                                          CNcomment:��ȡATR����ʧ��
\retval ::HI_ERR_SCI_FAILED_CARDSTATUS  The card status fails to be obtained.                                   CNcomment:��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_FAILED_SEND  The transmission operation fails.                                             CNcomment:���Ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_RECEIVE  The reception operation fails.                                             CNcomment:���ղ���ʧ��
\retval ::HI_ERR_SCI_FAILED_TIMEOUT_VAL  The block timeout fails to be set.                                     CNcomment:����������ʱʧ��
\retval ::HI_ERR_SCI_FAILED_GET_DATA_LEN  The length of the data that is not transmitted fails to be obtained.  CNcomment:��ȡδ���ͳ���ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGVCCEN  The VCCEN signal fails to be set.                                      CNcomment:����VCCEN�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_CONFIGDETECT The DETECT signal fails to be set.                                     CNcomment:����DETECT�ź�ʧ��
\retval ::HI_ERR_SCI_FAILED_BLK_CARDSTATUS The card status fails to be obtained in block mode.                  CNcomment:������ʽ��ȡ��״̬ʧ��
\retval ::HI_ERR_SCI_INVALID_PORT The port ID is incorrect.                                                     CNcomment:����Ķ˿ں�
\retval ::HI_ERR_SCI_INVALID_PROTOCOL  The protocol type is incorrect.                                          CNcomment:�����Э������
\retval ::HI_ERR_SCI_INVALID_FREQUENCY   The clock frequency is invalid.                                        CNcomment:�Ƿ���ʱ��Ƶ��
\retval ::HI_ERR_SCI_INVALID_LENTH  The length is invalid.                                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NO_ATR  There is no ATR data.                                                              CNcomment:��ATR����
\retval ::HI_ERR_SCI_CARD_STATUS  The card status is incorrect.                                                 CNcomment:��״̬����
\retval ::HI_ERR_SCI_RECEIVE_TIMEOUT  The data reception times out.                                             CNcomment:���ղ�����ʱ
\retval ::HI_ERR_SCI_INVALID_TIMEOUT_VAL  The timeout is invalid.                                               CNcomment:�Ƿ���ʱʱ��ֵ
\retval ::HI_ERR_SCI_INVALID_LEVEL_VAL The level is invalid.                                                    CNcomment:�Ƿ��ĵ�ƽ
\retval ::HI_ERR_SCI_FAILED_BLK_TIMEOUT  Obtaining the card status times out.                                   CNcomment:��ȡ��״̬��ʱ
\retval ::HI_ERR_SCI_FAILED_CONFIGSCIMODE  The operating mode of the CLK signal fails to be set.                CNcomment:����CLK�ź��߹���ģʽʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SwitchCard(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PROTOCOL_E enSciProtocol, HI_U32 u32Frequency);

/** 
\brief Sets the clock rate factor (in ETU) and baud rate regulator factor for special cards.
CNcomment:\brief �����⿨��Ҫ����ָ��etu ʱ�������ӣ������ʵ�������

\attention \n
This API needs to be called only for special cards.
The configured values must match the clock rate conversion factor F and bit rate regulator factor D defined in the protocol.
You can also set the factors to the values defined in the card specifications. Note that the values must be set correctly.
CNcomment:ֻ������Ҫ��Ŀ���Ҫ���ã���ͨ������Ҫ���ô˽ӿ����ã�
CNcomment: ���õ�ֵҪ��Э���й涨��Fʱ��ת��ת�����ӡ�D�����ʵ������Ӽ��ݣ�
CNcomment: ��������Ϊ���淶�й涨��ֵ��������������

\param [in] 	enSciPort	ID of an SCI port. The port ID can be 0 or 1.                                             CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	u32ClkFactor  Clock rate conversion factor, ranging from 372 to 2048. For details, see the factor F and card features in the protocol.	CNcomment:ʱ��ת������372��2048 ���������òο�Э���F���ӺͿ�����
\param [in] 	u32BaudFactor	Baud rate regulator factor 1, 2 x n (n = 1-16). For details, see the factor D and card features in the protocol. CNcomment:������У������1��2*n (n=1��16) ���������òο�Э��D���ӺͿ�����    
\retval  0 	Success                                                                                               CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_NOT_INIT  No SCI device is started.                                                          CNcomment:SCI�豸δ�� 
\retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                                       CNcomment:�Ƿ����� 

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SetEtuFactor(HI_UNF_SCI_PORT_E enSciPort,HI_U32 u32ClkFactor,HI_U32 u32BaudFactor);

/** 
\brief \brief Sets the extra guard time for transmitting two consecutive bytes from the terminal to an IC card. The guard time is the interval between the start edges of two consecutive bytes.
CNcomment:\brief �����ն���IC�����������������ֽ���ʼ�ض������ӵļ��ʱ��
\attention \n
This setting is performed based on related features only for the cards that require extra character guard time.
In general, the default value is used or the interval guard time is automatically set based on the ART analysis result.
CNcomment:ֻ������Ҫ��Ŀ�����Ҫ���������ַ�����ʱ��Ĳ��ܸ��ݿ�������������ã�
CNcomment: �����������á�һ��ʹ��ϵͳĬ��ֵ������ϵͳ����ATR�����Զ����á�
\param [in] 	enSciPort	ID of an SCI port. The port ID can be 0 or 1.      CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	u32GuardTime  Extra guard time, ranging from 0 to 254 ETU	   CNcomment:�����������ķ�Χ0��254  etu
\retval 0  Success	                                                       CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                  CNcomment:SCI�豸δ�� 
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.               CNcomment:�Ƿ����� 

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SetGuardTime(HI_UNF_SCI_PORT_E enSciPort,HI_U32 u32GuardTime);


/** 
\brief Sets the baud rate supported by the SCI card through protocol and parameters selection (PPS) negotiation or performs negotiation by using the F and D factors returned by the ATR to switch the protocol of a card.
CNcomment:\brief ͨ��PPS Э�̿������ÿ���֧�ֵĲ����ʻ�����ATR ���ص�F��D���ӽ���Э�̣�֧�ֶ�Э�鿨��Э���л�

\attention \n
PPS negotiation is available only when the SCI card supports this function.
The negotiated baud rate must be supported by the SCI card.
The command words for negotiation must comply with the specifications defined in section 9.2 "PPS request and response" in the 7816-3 protocol.
CNcomment:Ҫ����PPS Э�����ȿ�����֧�ָù��ܣ�
CNcomment: ҪЭ�̵Ĳ��������ݱ����ǿ����涨��֧�ֵĲ����ʷ�Χ��
CNcomment: ����Э�̵������������7816-3 ��9.2 PPS request and response ��

\param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                                 CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	pSciSendBuf  PPS negotiation command combined based on card specifications. If the default negotiation mode is used, the command word can be left empty.   CNcomment:���ݿ��淶��ϵ�PPSЭ�������ʹ��ϵͳĬ�ϵ�Э�̷�ʽ�������ֵ����ݿ�Ϊ��
\param [in]	Sendlen	   Length of the command word to be transmitted, ranging from 0 bytes to 5 bytes. The value 0 indicates that the default negotiation mode is used.   CNcomment: ���������ֵĳ���(0��5),����Ϊ0��ʾʹ��ϵͳĬ�ϵ�Э�����ʽ��
\param [in]	RecTimeouts  PPS response timeout after commands are transmitted. The value ranges from 1 ms to 10000 ms.  CNcomment: ����������֮�󣬽���PPS ��Ӧ�ĳ�ʱʱ��(1-10000),��λ�Ǻ���
\retval 0 	Success                                                                               CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_NOT_INIT   No SCI device is started.                                         CNcomment:SCI�豸δ�� 
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                                      CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NULL_PTR		The pointer is null.                                              CNcomment:��ָ��
\retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                          CNcomment:�����õ�ѡ��
\retval ::HI_ERR_SCI_RECEIVE_ERR  A reception error occurs.                                       CNcomment:���մ���
\retval ::HI_ERR_SCI_PPS_PTYPE_ERR 	The protocol type for PPS negotiation is incorrect.           CNcomment:PPSЭ�̵�Э�����ʹ���
\retval ::HI_ERR_SCI_PPS_FACTOR_ERR 	The F factor and D factor for PPS negotiation are incorrect. CNcomment:PPSЭ�̵�F��D���Ӵ���
\retval ::HI_ERR_SCI_PPS_NOTSUPPORT_ERR  The PPS negotiation type is not supported.               CNcomment:��֧�ֵ�PPSЭ������

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_NegotiatePPS(HI_UNF_SCI_PORT_E enSciPort, HI_U8 *pSciSendBuf,HI_U32 Sendlen, HI_U32 RecTimeouts);


/** 
\brief  Obtains the PPS data for the card response after PPS negotiation is successful.
CNcomment:\brief  PPS Э�̳ɹ�֮��,ͨ���ýӿڿɻ�ÿ���Ӧ��PPS ����
\attention \n
This API takes effect only after PPS negotiation is successful.
CNcomment:Ҫ��PPS Э�̳ɹ�֮��,ʹ�øýӿڲ���ʵ������

\param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                   CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [out] 	pRecBuf   Address of the buffer for storing the data to be received   CNcomment:�����յ�����buffer ��ַ
\param [out]	pDataLen	  Required length of the received data                      CNcomment:�����������ݵĳ��� 
\retval 0  Success	                                                                CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                        CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NULL_PTR	The pointer is null.                                	CNcomment:��ָ��
\retval ::HI_ERR_SCI_PPS_NOTSUPPORT_ERR  The PPS negotiation type is not supported. CNcomment:��֧�ֵ�PPSЭ������

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_GetPPSResponData(HI_UNF_SCI_PORT_E enSciPort, HI_U8* pRecBuf, HI_U32* pDataLen);


/** 
brief  Obtains the SCI parameter values including the protocol, ATR clock factor, ATR bit rate factor, actual clock factor, actual bit rate factor, number of retries, guard delay, character wait timeout, and block wait timeout.
CNcomment:\brief  ��ȡsci ��������Э�顢ATR ʱ�Ӻͱ��������ӡ�ʵ�����õ�ʱ�����Ӻͱ��������ӣ�
CNcomment: �������Դ�����GuardDelay ���ַ��ȴ���ʱ����ȴ���ʱ��
\attention \n
This API is available only after HI_UNF_SCI_Init and HI_UNF_SCI_ResetCard are called..
CNcomment:Ҫ�� ����ʼ����λ֮��,����ʹ�øýӿ�

\param [in] 	enSciPort	 ID of an SCI port. The port ID is 0 or 1.                      CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [out] 	pParams   Pointer to the data type of the data structure to be received   CNcomment:�����յĽṹ����������ָ��
\retval 0 	Success                                                                     CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                             CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_NULL_PTR		The pointer is null.                                    CNcomment:��ָ��
\retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                CNcomment:�����õ�ѡ��
\retval ::HI_ERR_SCI_NOT_INIT  	The SCI device is not initialized.                      CNcomment:û��ɳ�ʼ��

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_GetParams(HI_UNF_SCI_PORT_E enSciPort, HI_UNF_SCI_PARAMS_S_PTR pParams);


/** 
\brief  Sets the character timeout of T0 and T1 separately based on the current protocol type.
CNcomment:\brief  ���ݵ�ǰЭ�����ͷֱ������T0��T1���ַ���ʱʱ��

\attention \n
The timeout can be set only after the SCI card is reset successfully. The protocol type must be consistent with the current card type. In general, the timeout is obtained by parsing the ATR. That is, the timeout needs to be set only when card communication fails.
CNcomment:Ҫ�ȿ���λ�ɹ�֮��,���ܽ������ã�Э�����Ͳ��������뵱ǰ������һ�£�һ��������Ǹ���ATR������ֻ�����⿨�������Ҫ���á�

\param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.    CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	enSciProtocol  Protocol type                           CNcomment:Э������
\param [in]	MaxCharTime  Maximum character timeout. The value of MaxCharTime for T0 ranges from 960 to 244800, and the value of MaxCharTime for T1 ranges from 12 to 32779.	  CNcomment:���������ַ���ʱʱ�䣬���÷�ΧT0(960��244800),T1(12��32779)
\retval 0 	Success                                                  CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.         CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.             CNcomment:�����õ�ѡ��
\retval ::HI_ERR_SCI_NO_ATR  	The SCI card fails to be reset.        CNcomment:û��λ�ɹ�

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SetCharTimeout(HI_UNF_SCI_PORT_E enSciPort,  HI_UNF_SCI_PROTOCOL_E  enSciProtocol, HI_U32 MaxCharTime);


/** 
\brief  Sets the block timeout of T1.
CNcomment:\brief  ����T1�Ŀ鳬ʱʱ��

\attention \n
The timeout can be set only after the SCI card is reset successfully. In general, the timeout is obtained by parsing the ATR. That is, the timeout needs to be set only in special cases
CNcomment:Ҫ�ȿ���λ�ɹ�֮��,���ܽ������ã�һ��������Ǹ���ATR������ֻ�����⿨�������Ҫ���á�

\param [in] 	enSciPort	 ID of an SCI port. The port ID is 0 or 1.                         CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	MaxBlockTime   Maximum block timeout. The value ranges from 971 to 491531.   CNcomment:���ÿ鳬ʱ�����ֵ����Χ(971��491531)
\retval 0 	Success                                                                        CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_INVALID_PARA The parameter is invalid.                                CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                                   CNcomment:�����õ�ѡ��
\retval ::HI_ERR_SCI_NO_ATR  		The SCI card fails to be reset.                            CNcomment:û��λ�ɹ�

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SetBlockTimeout(HI_UNF_SCI_PORT_E enSciPort,HI_U32 MaxBlockTime);


/** 
\brief  Sets the maximum number of transmission retries after a check error occurs.
CNcomment:\brief  ����У�������ط���������

\attention \n
The number can be set only after the SCI card is reset successfully.
CNcomment:Ҫ�ȿ���λ�ɹ�֮��,���ܽ�������

\param [in] 	enSciPort	ID of an SCI port. The port ID is 0 or 1.                    CNcomment:SCI�˿ںţ�ȡֵ��ΧΪ0��1 
\param [in] 	TxRetryTimes   Number of transmission retries, ranging from 0 to 7.    CNcomment:������Χ(0��7)
\retval 0 	Success                                                                  CNcomment:�ɹ� 
\retval ::HI_ERR_SCI_INVALID_PARA  The parameter is invalid.                         CNcomment:�Ƿ�����
\retval ::HI_ERR_SCI_INVALID_OPT	The option is invalid.                             CNcomment:�����õ�ѡ��
\retval ::HI_ERR_SCI_NO_ATR  	The SCI card fails to be reset.	                       CNcomment:û��λ�ɹ�

\see \n
N/A
*/
HI_S32 HI_UNF_SCI_SetTxRetries(HI_UNF_SCI_PORT_E enSciPort,HI_U32 TxRetryTimes);

/** @} */  /** <!-- ==== API Declaration End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_2 */
/** @{ */  /** <!-- [E2PROM] */
/*---- e2prom ----*/
/** 
\brief Starts the E2PROM device.
CNcomment:\brief ��E2PROM��Electrically-Erasable Programmable Read-Only Memory���豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ��򿪻�ʧ�ܡ�

\param  N/A                                                                   CNcomment:��
\retval 0 Success                                                             CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.       CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT The E2PROM device has been initialized.   CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT The E2PROM pointer is invalid.          CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA   The E2PROM parameter is invalid.       CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized.CNcomment: E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_READ  The E2PROM data fails to be read.        CNcomment:E2PROM���ݶ�ȡʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_Open(HI_VOID);


/** 
\brief Stops the E2PROM device.
CNcomment:\brief �ر�E2PROM�豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ��رջ�ʧ�ܡ�

\param N/A                                                                       CNcomment:��
\retval 0 Success                                                                CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.          CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT The E2PROM device has been initialized .     CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT  The E2PROM pointer is invalid.            CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA  The E2PROM parameter is invalid.           CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized.  CNcomment:E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_READ The E2PROM data fails to be read.            CNcomment:E2PROM���ݶ�ȡʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_Close(HI_VOID);


/** 
\brief Reads data from the E2PROM device.
CNcomment:\brief ��E2PROM�豸�����ݡ�
\attention \n
Ensure that the I2C module is available before calling this API.
If the length of the input address u32Addr is greater than the total address length of the chip, no read operation is performed on the E2PROM device.\n
If the input pointer pu8Buf is null, no read operation is performed on the E2PROM device.\n
If the input address u32Addr is valid, but the sum of u32Addr and u32Length is greater than the total address length of the chip, no write operation is performed on the E2PROM device.
CNcomment:���ô˽ӿ�ǰӦȷ��I2Cģ�����\n
CNcomment: ������ĵ�ַu32Addr����оƬ��ַ�ܳ��ȣ��򲻻��E2PROM���ж�����\n
CNcomment: �������ָ��pu8BufΪ�գ��򲻻��E2PROM���ж�����\n
CNcomment: �������ַu32Addr�Ϸ�����u32Addr��u32Length�ĺʹ���оƬ��ַ�ܳ��ȣ��򲻻��E2PROM���ж�������

\param[in] u32Addr  On-chip offset address                                      CNcomment:Ƭ��ƫ�Ƶ�ַ��
\param[out]  pu8Buf  Buffer for storing the data to be read                     CNcomment:��Ŵ���ȡ�����ݡ�
\param[in] u32Length  Length of the data to be read                             CNcomment:��ȡ���ݵĳ��ȡ�
\retval 0  Success                                                              CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.         CNcomment: E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT  The E2PROM device has been initialized.    CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT The E2PROM pointer is invalid.            CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA  The E2PROM parameter is invalid.          CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized. CNcomment:E2PROM�豸��ʼ��ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_Read (HI_U32 u32Addr, HI_U8 * pu8Buf, HI_U32 u32Length);


/** 
\brief Writes data to the E2PROM device.
CNcomment:\brief ��E2PROM�豸д���ݡ�

\attention \n
Ensure that the I2C module is available before calling this API.
If the length of the input address u32Addr is greater than the total address length of the chip, no write operation is performed on the E2PROM device.\n
If the input pointer pu8Buf is null, no write operation is performed on the E2PROM device.\n
If the input address u32Addr is valid, but the sum of u32Addr and u32Length is greater than the total address length of the chip, no write operation is performed on the E2PROM device.
CNcomment:���ô˽ӿ�ǰӦȷ��I2Cģ�����\n
CNcomment: ������ĵ�ַu32Addr����оƬ��ַ�ܳ��ȣ��򲻻��E2PROM����д����\n
CNcomment: �������ָ��pu8BufΪ�գ��򲻻��E2PROM����д����\n
CNcomment: �������ַu32Addr�Ϸ�����u32Addr��u32Length�ĺʹ���оƬ��ַ�ܳ��ȣ��򲻻��E2PROM����д������

\param[in] u32Addr  On-chip offset address                                      CNcomment:Ƭ��ƫ�Ƶ�ַ��
\param[in]  pu8Buf  Buffer for storing the data to be written                   CNcomment:��Ŵ�д������ݡ�
\param[in] u32Length  Length of the data to be written                          CNcomment:Ҫд������ݳ��ȡ�
\retval 0  Success                                                              CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.         CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT  The E2PROM device has been initialized.    CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT  The E2PROM pointer is invalid.           CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA   The E2PROM parameter is invalid.         CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized. CNcomment:E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_READ  The E2PROM data fails to be read.          CNcomment:E2PROM���ݶ�ȡʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_Write (HI_U32 u32Addr, HI_U8 * pu8Buf, HI_U32 u32Length);


/** 
\brief Sets the type of the E2PROM chip.
CNcomment:\brief ����E2PROMоƬ���͡�

\attention \n
Ensure that the I2C module is available before calling this API.
Before calling HI_UNF_E2PROM_Read and HI_UNF_E2PROM_Write, you must call this API.
At present, only the M24LC16 chip is tested.
CNcomment:���ô˽ӿ�ǰӦȷ��I2Cģ�����\n
CNcomment: �˽ӿڱ����ڵ���HI_UNF_E2PROM_Read��HI_UNF_E2PROM_Write֮ǰ����\n
CNcomment: Ŀǰ��ʵ����M24LC16оƬ���������͵�оƬδ���в��ԡ�
\param[in] enChip  Type of the E2PROM chip                                  CNcomment: E2PROMоƬ���͡�
                   HI_UNF_E2PROM_CHIP_AT24C01�� The chip type is AT24C01.   CNcomment:оƬ����ΪAT24C01��
                   HI_UNF_E2PROM_CHIP_AT24C02�� The chip type is AT24C02.   CNcomment:оƬ����ΪAT24C02��
                   HI_UNF_E2PROM_CHIP_AT24C04��The chip type is AT24C04.    CNcomment:оƬ����ΪAT24C04��
                   HI_UNF_E2PROM_CHIP_AT24C08��The chip type is AT24C08.    CNcomment:оƬ����ΪAT24C08��
                   HI_UNF_E2PROM_CHIP_AT24C16�� The chip type is AT24C16.   CNcomment:оƬ����ΪAT24C16��
                   HI_UNF_E2PROM_CHIP_AT24C32��The chip type is AT24C32.    CNcomment:оƬ����ΪAT24C32��
                   HI_UNF_E2PROM_CHIP_AT24C64��The chip type is AT24C64.    CNcomment:оƬ����ΪAT24C64��
                   HI_UNF_E2PROM_CHIP_AT24C128��The chip type is AT24C128.  CNcomment:оƬ����ΪAT24C128��
                   HI_UNF_E2PROM_CHIP_AT24C256��The chip type is AT24C256.  CNcomment:оƬ����ΪAT24C256��
                   HI_UNF_E2PROM_CHIP_M24LC01��The chip type is M24LC01.    CNcomment:оƬ����ΪM24LC01��
                   HI_UNF_E2PROM_CHIP_M24LC02��The chip type is M24LC02.    CNcomment:оƬ����ΪM24LC02��
                   HI_UNF_E2PROM_CHIP_M24LC04��The chip type is M24LC04.    CNcomment:оƬ����ΪM24LC04��
                   HI_UNF_E2PROM_CHIP_M24LC08��The chip type is M24LC08.    CNcomment:оƬ����ΪM24LC08��
                   HI_UNF_E2PROM_CHIP_M24LC16��The chip type is M24LC16.    CNcomment:оƬ����ΪM24LC16��
                   HI_UNF_E2PROM_CHIP_M24LC32��The chip type is M24LC32.    CNcomment:оƬ����ΪM24LC32��
                   HI_UNF_E2PROM_CHIP_M24LC64��The chip type is M24LC64.    CNcomment:оƬ����ΪM24LC64��
                   HI_UNF_E2PROM_CHIP_M24LC128��The chip type is M24LC128.  CNcomment:оƬ����ΪM24LC128��
                   HI_UNF_E2PROM_CHIP_M24LC256��The chip type is M24LC256.  CNcomment:оƬ����ΪM24LC256��
\retval 0 Success                                                           CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.     CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT The E2PROM device has been initialized. CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT The E2PROM pointer is invalid.        CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA  The E2PROM parameter is invalid.      CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized.  CNcomment:E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_READ  The E2PROM data fails to be read.      CNcomment:E2PROM���ݶ�ȡʧ��
\retval ::HI_ERR_E2PROM_FAILED_WRITE  The E2PROM data fails to be written.  CNcomment:E2PROM����д��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_SETCHIPTYPE  The E2PROM chip fails to be selected. CNcomment:E2PROMѡ��оƬʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_SetChipType(HI_UNF_E2PROM_CHIP_E enChip);


/** 
\brief Sets the I2C bus address of the E2PROM device.
CNcomment:\brief ����E2PROM��I2C���ߵ�ַ��

\attention \n
The address is related to the hardware layout. For details, see the corresponding hardware design manuals.
CNcomment:��ַ��Ӳ�������йأ���ο�Ӳ����ơ�

\param[in] u32E2promAddr  I2C bus address of the E2PROM device                            CNcomment:E2PROM��I2C���ߵ�ַ��
\retval 0 Success                                                                         CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT   The E2PROM device is not initialized.                  CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT  The E2PROM device has been initialized.              CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT  The E2PROM pointer is invalid.                     CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA  The E2PROM parameter is invalid.                    CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized.           CNcomment:E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_READ  The E2PROM data fails to be read.                    CNcomment:E2PROM���ݶ�ȡʧ��
\retval ::HI_ERR_E2PROM_FAILED_WRITE  The E2PROM data fails to be written.                CNcomment:E2PROM����д��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_SETADDR  The address of the E2PROM device fails to be set. CNcomment:E2PROM���õ�ַʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_SetAddr(HI_U32 u32E2promAddr);


/** 
\brief Sets the ID of the I2C channel used by the E2PROM device.
CNcomment:\brief ����E2PROM��ʹ�õ�I2Cͨ���š�

\attention \n
N/A
\param[in] u32I2cNum  ID of the I2C channel used by the E2PROM device, ranging from 0 to HI_I2C_MAX_NUM_USER   CNcomment:E2PROM���õ�I2Cͨ���ţ���Χ0��HI_I2C_MAX_NUM_USER
\retval 0  Success                                                                                             CNcomment:�ɹ�
\retval ::HI_ERR_E2PROM_NOT_INIT  The E2PROM device is not initialized.                                        CNcomment:E2PROM�豸δ��ʼ��
\retval ::HI_ERR_E2PROM_REPEAT_INIT  The E2PROM device has been initialized.                                   CNcomment:E2PROM�豸�ظ���ʼ��
\retval ::HI_ERR_E2PROM_INVALID_POINT The E2PROM pointer is invalid.                                           CNcomment:E2PROM��Чָ��
\retval ::HI_ERR_E2PROM_INVALID_PARA  The E2PROM parameter is invalid.                                         CNcomment:E2PROM��Ч����
\retval ::HI_ERR_E2PROM_FAILED_INIT  The E2PROM device fails to be initialized.                                CNcomment:E2PROM�豸��ʼ��ʧ��
\retval ::HI_ERR_E2PROM_FAILED_SETI2CNUM  The I2C channel of the E2PROM device fails to be set.                CNcomment:E2PROM����I2Cͨ��ʧ��
\retval ::HI_ERR_E2PROM_NOT_INIT The E2PROM device is not initialized.                                         CNcomment:E2PROM�豸δ��ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_E2PROM_SetI2cNum(HI_U32 u32I2cNum);

/** @} */  /** <!-- ==== API Declaration End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_3 */
/** @{ */  /** <!-- [I2C] */
/*---- i2c ----*/
/** 
\brief Starts the I2C device.
CNcomment:\brief ��I2C��the Inter-Integrated Circuit���豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ����ô˽ӿڻ�ʧ�ܡ�

\param N/A                                                               CNcomment:��
\retval 0 Success                                                        CNcomment:�ɹ�
\retval ::HI_ERR_I2C_NOT_INIT  I2CThe I2C device is not initialized.     CNcomment:�豸δ��ʼ��
\retval ::HI_ERR_I2C_REPEAT_INIT  The I2C device has been initialized.   CNcomment:I2C�豸�ظ���ʼ��
\retval ::HI_ERR_I2C_INVALID_POINT  The I2C pointer is invalid.          CNcomment:I2C��Чָ��
\retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.         CNcomment:I2C��Ч����
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_Open (HI_VOID);


/** 
\brief  Closes the I2C device.
CNcomment:\brief �ر�I2C�豸��

\attention \n
This API is called after I2C operations are completed.\n
This API cannot be called repeatedly.
CNcomment:��I2C������������ô˽ӿ�\n
CNcomment: �ظ����ô˽ӿڻ�ʧ�ܡ�

\param N/A                                                        CNcomment:��
\retval 0 Success                                                 CNcomment: �ɹ�
\retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized. CNcomment:I2C�豸δ��ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_Close(HI_VOID);

/** 
The I2C device is not initialized.
CNcomment:\brief ����һ·GpioI2c

\attention \n
If the specified GPIO pins are used, this API fails to be called.\n
CNcomment:���Gpio�ѱ�ռ�û�ʧ��\n

\param[in] u32I2cNum  ID of the obtained I2C bus                            CNcomment:�õ�I2C���ߺš�
\param[in] whichGpioClock  Group ID of the clock pin, ranging from 0 to 12  CNcomment:Clock�ܽ���ţ�ȡֵ��ΧΪ0��12
\param[in] whichGpioData   Group ID of the data pin, ranging from 0 to 12   CNcomment:Data�ܽ���ţ� ȡֵ��ΧΪ0��12
\param[in] clockBit  Clock pin ID, ranging from 0 to 7                      CNcomment:Clock�ܽ�λ�ţ�ȡֵ��ΧΪ0��7
\param[in] dataBit   Data pin ID, ranging from 0 to 7                       CNcomment:Data�ܽ�λ�ţ� ȡֵ��ΧΪ0��7
\retval 0 Success                                                           CNcomment:�ɹ�
\retval ::HI_ERR_GPIO_INVALID_PARA  The parameter is invalid.               CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_CreateGpioI2c(HI_U32 *u32I2cNum, HI_U32 whichGpioClock, HI_U32 whichGpioData, HI_U32 clockBit, HI_U32 dataBit);

/** 
\brief Destroys a inter-integrated circuit (I2C) channel that simulates the general-purpose input/output (GPIO) function.
CNcomment:\brief ����һ·GpioI2c

\attention \n
If the I2C channel is not used, a code indicating success is returned.\n
CNcomment:�����GpioI2cδʹ�ã�ֱ�ӷ��سɹ�\n

\param[in] u32I2cNum ID of the I2C bus to be destroyed        CNcomment:Ҫ���ٵ�I2C���ߺš�
\retval 0  Success                                            CNcomment:�ɹ�
\retval ::HI_ERR_GPIO_INVALID_PARA  The parameter is invalid. CNcomment:�����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_DestroyGpioI2c(HI_U32 u32I2cNum);
/** 
\brief Reads data by using the I2C bus.
CNcomment:\brief ͨ��I2C�����ݡ�

\attention \n
N/A
\param[in] u32I2cNum  I2C bus of the device to be read           CNcomment:����ȡ�豸ʹ�õ�I2C���ߡ�
\param[in] u8DevAddress  Address of a device on the I2C bus      CNcomment:�豸��I2C�����ϵĵ�ַ��
\param[in] u32RegAddr  On-chip offset address of a device        CNcomment:�豸��Ƭ��ƫ�Ƶ�ַ��
\param[in] u32RegAddrCount  Length of an on-chip offset address. CNcomment:Ƭ��ƫ�Ƶ�ַ�ĳ��ȵ�λ��
                      1: 8-bit sub address                       CNcomment:1����ʾ8bit�ӵ�ַ��
                      2: 16-bit sub address                      CNcomment:2����ʾ16bit�ӵ�ַ��
                      3: 24-bit sub address                      CNcomment:3����ʾ24bit�ӵ�ַ��
                      4: 32-bit sub address                      CNcomment:4����ʾ32bit�ӵ�ַ��

\param[out] pu8Buf   Buffer for storing the data to be read                            CNcomment:��Buffer����Ŷ�ȡ���ݡ�
\param[in] u32Length  Length of the data to be read                                    CNcomment:Ҫ��ȡ�����ݳ��ȡ�
\retval 0 Success                                                                      CNcomment:�ɹ�
\retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                      CNcomment:I2C�豸δ��ʼ��
\retval ::HI_ERR_I2C_INVALID_POINT  The I2C pointer is invalid.                        CNcomment:I2C��Чָ��
\retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                       CNcomment:I2C��Ч����
\retval ::HI_ERR_I2C_FAILED_INIT  The I2C device fails to be initialized.              CNcomment:I2C�豸��ʼ��ʧ��
\retval ::HI_ERR_I2C_FAILED_READ  Data fails to be read by using the I2C bus.          CNcomment:I2C������ʧ��
\retval ::HI_ERR_I2C_FAILED_SETRATE The transfer rate of the I2C bus fails to be set.  CNcomment:I2C��������ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_Read(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                       HI_U32 u32RegAddrCount, HI_U8 *pu8Buf, HI_U32 u32Length);
                       

/** 
\brief Writes data by using the I2C bus. That is, you can call this API to write data to the device mounted on the I2C bus through the I2C channel.
CNcomment:\brief ͨ��I2Cд���ݡ�ͨ��I2Cͨ������I2C�����Ϲҽӵ��豸����д������

\attention \n
N/A
\param[in] u32I2cNum  I2C bus of the device to be written         CNcomment:��д�豸ʹ�õ�I2C���ߡ�
\param[in] u8DevAddress  Address of a device on the I2C bus       CNcomment:�豸��I2C�����ϵĵ�ַ��
\param[in] u32RegAddr  On-chip offset address of a device         CNcomment:�豸��Ƭ��ƫ�Ƶ�ַ��
\param[in] u32RegAddrCount Length of an on-chip offset address.   CNcomment:Ƭ��ƫ�Ƶ�ַ�ĳ��ȵ�λ��
                    1: 8-bit sub address                          CNcomment:1����ʾ8bit�ӵ�ַ��
                    2: 16-bit sub address                         CNcomment:2����ʾ16bit�ӵ�ַ��
                    3: 24-bit sub address                         CNcomment:3����ʾ24bit�ӵ�ַ��
                    4: 32-bit sub address                         CNcomment:4����ʾ32bit�ӵ�ַ��
                    
\param[in]  pu8Buf   Buffer for storing the data to be written                         CNcomment:дBuffer����Ŵ�д�����ݡ�
\param[in] u32Length  Length of the data to be written                                 CNcomment:Ҫд������ݵĳ��ȡ�
\retval 0  Success                                                                     CNcomment:�ɹ�
\retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                      CNcomment:I2C�豸δ��ʼ��
\retval ::HI_ERR_I2C_REPEAT_INIT  The I2C device has been initialized.                 CNcomment:I2C�豸�ظ���ʼ��
\retval ::HI_ERR_I2C_INVALID_POINT  The I2C pointer is invalid.                        CNcomment:I2C��Чָ��
\retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                       CNcomment:I2C��Ч����
\retval ::HI_ERR_I2C_FAILED_INIT  The I2C device fails to be initialized.              CNcomment:I2C�豸��ʼ��ʧ��
\retval ::HI_ERR_I2C_FAILED_WRITE  Data fails to be written by using the I2C bus.      CNcomment:I2Cд����ʧ��
\retval ::HI_ERR_I2C_FAILED_SETRATE  The transfer rate of the I2C bus fails to be set. CNcomment:I2C��������ʧ��
\see \n
N/A
*/                      
HI_S32 HI_UNF_I2C_Write(HI_U32 u32I2cNum, HI_U8 u8DevAddress, HI_U32 u32RegAddr,
                       HI_U32  u32RegAddrCount, HI_U8 * pu8Buf, HI_U32 u32Length);


/** 
\brief Sets the transfer rate of the I2C bus.
CNcomment:\brief ����I2C�Ĵ������ʡ�

\attention \n
If you do not call this API to set the transfer rate, the rate 100 kbit/s is used by default.
CNcomment:��������ô˽ӿڣ�ϵͳ����100Kbit/s��Ϊȱʡ���ʡ�

\param[in] u32I2cNum  D of channel corresponding to the device to be written on the I2C bus                         CNcomment:��д�豸��I2C�����ϵ�ͨ���š�
\param[in] enI2cRate  I2C clock rate. For details about the definition, see the description of ::HI_UNF_I2C_RATE_E. CNcomment:I2Cʱ�����ʡ�������ο�::HI_UNF_I2C_RATE_E
\retval 0  Success                                                                                                  CNcomment:�ɹ�
\retval ::HI_ERR_I2C_NOT_INIT  The I2C device is not initialized.                                                   CNcomment:I2C�豸δ��ʼ��
\retval ::HI_ERR_I2C_REPEAT_INIT  The I2C device has been initialized.                                              CNcomment:I2C�豸�ظ���ʼ��
\retval ::HI_ERR_I2C_INVALID_POINT  The I2C pointer is invalid.                                                     CNcomment:I2C��Чָ��
\retval ::HI_ERR_I2C_INVALID_PARA  The I2C parameter is invalid.                                                    CNcomment:I2C��Ч����
\retval ::HI_ERR_I2C_FAILED_INIT  The I2C device fails to be initialized.                                           CNcomment:I2C�豸��ʼ��ʧ��
\retval ::HI_ERR_I2C_FAILED_READ  Data fails to be read by using the I2C bus.                                       CNcomment:I2C������ʧ��
\retval ::HI_ERR_I2C_FAILED_WRITE  Data fails to be written by using the I2C bus.                                   CNcomment:I2Cд����ʧ��
\retval ::HI_ERR_I2C_FAILED_SETRATE  The transfer rate of the I2C bus fails to be set.                              CNcomment:I2C��������ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_I2C_SetRate(HI_U32 u32I2cNum, HI_UNF_I2C_RATE_E enI2cRate);

/** @} */  /** <!-- ==== API Declaration End ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_4 */
/** @{ */  /** <!-- [KEYLED] */
/**---- keyled ----*/
/**
\brief Initializes the KEYLED module.
CNcomment:\brief ��ʼ��KEYLEDģ�顣

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:�ظ����ñ��ӿڣ��᷵�سɹ���

\param N/A                                        CNcomment:��
\retval ::HI_SUCCESS Success                      CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.     CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_Init(HI_VOID);


/**
\brief Deinitializes the KEYLED module.
CNcomment:\brief ȥ��ʼ��KEYLEDģ�顣

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:�ظ����ñ��ӿڣ��᷵�سɹ���

\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_DeInit(HI_VOID);

/**
\brief Selects the type of the KEYLED.
CNcomment:\brief ѡ��KEYLED�������͡�

\attention \n
It is recommended to call this API once after the KEYLED module is initialized. By default, the standard KEYLED type is selected.
CNcomment:�����ʼ����ֻѡ��һ�Σ���Ҫ�ظ����ã�Ĭ��Ϊ��׼KEYLEDģʽ��

\param[in] enKeyLedType   KEYLED type         CNcomment:keyled���͡� 
\retval ::HI_SUCCESS   Success                CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType);

/**
\brief Enables the key function.
CNcomment:\brief �򿪰������ܡ�
\attention \n
The KEYLED module starts to receive key values after the key function is enabled.\n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:�򿪺�ʼ���հ���\n
CNcomment: �ظ����ñ��ӿڣ��᷵�سɹ���
\param N/A                                     CNcomment:��
\retval ::HI_SUCCESS Success                   CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.  CNcomment: APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Open(HI_VOID);

/**
\brief Disables the key function.
CNcomment:\brief �رհ������ܡ�

\attention \n
The KEYLED module stops receiving key IDs after the key function is disabled.\n
This API is valid when it is called for the first time. If this API is called repeatedly, the error code HI_SUCCESS is returned.
CNcomment:�رպ�ֹͣ���հ���\n
CNcomment: ���ӿڵ�һ�ε���������,�ظ����÷��سɹ���

\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Close(HI_VOID);

/**
\brief Clears the key values that are not received.
CNcomment:\brief ��յ�ǰû�н��յİ�����

\attention \n
This API is used to clear the buffer for storing the key values. In this way, the initial state is returned.
CNcomment:��հ���buffer���ص���ʱ״̬��
\param N/A Success                            CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_Reset(HI_VOID);


/**
\brief Obtains a key value.
CNcomment:\brief ��ȡһ������ֵ��

\attention \n
This API is used to obtain the key value that is received first in the buffer. After this API is called, the key value and key status are returned.\n
The key status contains pressed and released.\n
This API needs to work with ::HI_UNF_KEY_SetBlockTime.\n
If there are no key values, the API waits until timeout occurs. In this case, the error code ::HI_ERR_KEYLED_TIMEOUT is returned.\n
If the timeout is set to 0 in non-block mode, the error code ::HI_ERR_KEYLED_NO_NEW_KEY is returned when there are no key values.
If the timeout is set to 0xFFFFFFFF, it indicates infinite wait.
CNcomment:��ȡbuffer�����絽��İ���ֵ�����ذ���ֵ����ֵ��״̬\n
CNcomment: ״̬�������º͵���\n
CNcomment: �ӿ����::HI_UNF_KEY_SetBlockTime�ӿ�һ��ʹ��\n
CNcomment: û�а���ʱ�����еȴ�����ʱ���˳������س�ʱ������\n
CNcomment: ��ʱֵΪ0ʱΪ�������ӿڣ�û�а���ֱ�ӷ���û�а���������\n
CNcomment: ��ʱֵ����Ϊ0xffffffff��һֱ�ȴ���
\param[out] pu32PressStatus   Key status, pressed or released           CNcomment:����״̬�ǰ��»��ǵ���
\param[out] pu32KeyId  Key value                                        CNcomment:����ֵ��
\retval ::HI_SUCCESS  Success                                           CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized. CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_KEYLED_NULL_PTR  The pointer is null.                  CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_KEYLED_NO_NEW_KEY  There are no key values.            CNcomment:û�а���
\retval ::HI_ERR_KEYLED_TIMEOUT  Waiting for key values times out.      CNcomment:�ȴ�������ʱ
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);

/**
\brief Sets the timeout of reading key IDs.
CNcomment:\brief ���ð�����ȡ�ĳ�ʱʱ�䡣

\attention \n
If the timeout is set to 0, the API is not blocked. If there is no key ID, the error code HI_ERR_KEYLED_NO_NEW_KEY is returned.\n
If the timeout is set to 0xFFFFFFFF, it indicates infinite wait.
CNcomment:��ʱֵΪ0ʱ��������û�а���ֱ�ӷ���û�а���������\n
CNcomment: ��ʱֵ����Ϊ0xffffffff��һֱ�ȴ���
\param[in] u32BlockTimeMs   Timeout, in ms                               CNcomment:��ʱʱ�䣬��λms��
\retval ::HI_SUCCESS Success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:ģ��û�г�ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32BlockTimeMs);


/**
\brief Set the time of pressing a key repeatedly.
CNcomment:\brief �����ظ�������ʱ�䡣

\attention \n
If a key is pressed for a period of time, it considers that the key is pressed repeatedly.\n
The time ranges from 108 ms to 65536 ms. If the time is not within the range, the time is automatically restricted to this range.
The default time is 200 ms.
CNcomment:��ĳ�������ڰ���״̬���������õ�ʱ��ֵ�ж�Ϊ�ظ�����\n
CNcomment: �ظ����������÷�Χ��108ms��65536ms֮�䣬���������Χ���Զ�Լ�������޻�������\n
CNcomment: Ĭ�ϵ��ظ������ж�ʱ��Ϊ200ms��

\param[in] u32RepTimeMs  Time of pressing a key repeatedly, in ms       CNcomment:�ж�Ϊ�ظ�������ʱ�䣬��λms��
\retval ::HI_SUCCESS Success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized. CNcomment:ģ��û�г�ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_RepKeyTimeoutVal(HI_U32 u32RepTimeMs);

/**
\brief Enables or disables the function of reporting the same key ID.
CNcomment:\brief �����Ƿ�ʹ���ظ�������

\attention \n
The ID of a pressed key is reported consecutively only after the report function is enabled.
CNcomment:ֻ��ʹ�ܺ�һֱ��סĳ���������������ϱ���ֵ��

\param[in] u32IsRepKey Repeat key report enable. 1: enabled; 0: disabled   CNcomment:�Ƿ�ʹ���ظ�������1 ʹ�ܣ�0 ��ʹ�ܡ�
\retval ::HI_SUCCESS Success                                               CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                              CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.    CNcomment:ģ��û�г�ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKey);

/**
\brief Enables or disables the function of reporting the released status of a key.
CNcomment:\brief �����Ƿ��ϱ���������

\attention \n
The released statuses of keys are reported only after the report function is enabled. Otherwise, only the pressed statuses are reported.
CNcomment:ʹ�ܺ�Ż��ϱ����������¼�����Ȼֻ�ϱ������¼���

\param[in] u32IsKeyUp Repeat key report enable. 1: enabled; 0: disabled  CNcomment:�Ƿ�ʹ���ظ�������1 ʹ�ܣ�0 ��ʹ�ܡ�
\retval ::HI_SUCCESS Success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:ģ��û�г�ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUp);

/**
\brief Enables the display function of LEDs.
CNcomment:\brief ���������ʾ���ܡ�

\attention \n
The LEDs work only after the display function is enabled.\n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:�򿪺�Ż�������ܵ���ʾ\n
CNcomment: �ظ����ñ��ӿڣ��᷵�سɹ���

\param N/A                                     CNcomment:��
\retval ::HI_SUCCESS Success                   CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.  CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Open(HI_VOID);

/**
\brief Disables the display function of LEDs.
CNcomment:\brief �ر��������ʾ���ܡ�

\attention \n
The LEDs do not work after the display function is disabled.\n
This API is valid when it is called for the first time. If this API is called repeatedly, the error code HI_SUCCESS is returned.
CNcomment:�رպ�ֹͣ�������ʾ\n
CNcomment: ���ӿڵ�һ�ε���������,�ظ����÷��سɹ���

\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Close(HI_VOID);


/**
\brief Displays characters based on the type of the input code.
CNcomment:\brief ���ݴ��������ֵ������ʾ��

\attention \n
Each LED displays a character that corresponds to the value of one byte.\n
During displaying, the values of the four characters to be displayed need to be combined to form a 32-bit unsigned int data segment. The data serves as the parameter value of this API.\n
The four LEDs from left to right display bit[7:0], bit[15:8], bit[23:16], and bit[31:24] of the data respectively.\n
For example, if you want to display "1234" on LEDs, you need to combine the display values corresponding to these characters to form the following unsigned int variable:\n
  Unsigned int parameter = 0x03 + (0x9F <<8) + (0x25<<16) + (0x0D <<24);\n
Where, 0x03, 0x9F, 0x25, and 0x0D are the display values of 1, 2, 3, and 4 respectively.\n
The following shows the mapping between the segments of a 7-segment LED and data bits:\n
       --7--       \n
      |     |      \n
     2|     |6     \n
      |--1--       \n
      |     |      \n
     3|     |5     \n
       --4-- .0    \n
The common display characters and values of the LEDs on HiSilicon demo board are as follows in the case of co-anode (the display characters and values need to be reversed in the case of co-cathode):
  Digitals 0-9: 0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09\n
  Upper-case letters A-Z (the characters that cannot be displayed are expressed as 0xFF):\n
     0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,\n
     0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF\n
  Lower-case letters a-z (the characters that cannot be displayed are expressed as 0xFF):\n
    0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,\n
    0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
CNcomment:	ÿ�������Ҫ��ʾ���ַ���Ӧһ���ֽڵ���ʾֵ��\n
CNcomment: ��ʾʱ��Ҫ��4 ������ʾ�ַ���Ӧ����ʾֵ��ϳ�һ��unsiged int��32bit��������Ϊ�˽ӿڵĲ���ֵ��\n
CNcomment: �����ҵ�4 ��LED ������ʾ���ݵ�bit[7:0]��bit[15:8]��bit[23:16]��bit[31:24]��\n
CNcomment: ���磺��LED ����ʾ��1234��������Ҫ����Щ�ַ���Ӧ����ʾֵ��ϳ�һ��Unsigned int �ı�����\n
CNcomment:   Unsigned int parameter = 0x03 + (0x9F <<8) + (0x25<<16) + (0x0D <<24)��\n
CNcomment: ���У�0x03��0x9F��0x25��0x0D �ֱ��ǡ�1������2������3������4������ʾֵ��\n
CNcomment:�߶�����ܶ��������ݵ�bit��Ӧ��ϵ����:\n
CNcomment:       --7--       \n
CNcomment:      |     |      \n
CNcomment:     2|     |6     \n
CNcomment:      |--1--       \n
CNcomment:      |     |      \n
CNcomment:     3|     |5     \n
CNcomment:       --4-- .0    \n
CNcomment:��˼DEMO������ܳ�����ʾ�ַ������Ӧ����ʾֵ����(��Թ�������������Ҫȡ��)��\n
CNcomment:  . �ַ�0-9��0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09\n
CNcomment:  . ��д��ĸA-Z��������ʾ����0xFF ��ʾ����\n
CNcomment:     0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,\n
CNcomment:     0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF\n
CNcomment:  . Сд��ĸa-z��������ʾ����0xFF ��ʾ����\n
CNcomment:    0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,\n
CNcomment:   0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
\param[in] u32CodeValue 4-digit value to be displayed                   CNcomment:��Ҫ��ʾ��4λ��ֵ��
\retval ::HI_SUCCESS Success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized. CNcomment:ģ��û�г�ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_LED_Display(HI_U32 u32CodeValue);


/**
\brief Displays the time on LEDs.
CNcomment:\brief �����������ʾʱ�䡣

\attention \n
The values of hour and minute need to be converted into corresponding decimal values. For example, if you want to display 22:45, you need to set hour to 22 and set minute to 45.\n
Before calling this API, you need to call HI_UNF_LED_SetFlashPin to set the statuses of display pins to HI_UNF_KEYLED_LIGHT_ALL.\n
After the start time is set, the time is displayed on LEDs. At the same time, the colon (:) in the time blinks at the frequency of 1 Hz.\n
Because the time is displayed in 24-hour format, the maximum value of hour is 23, and the maximum value of minute is 59.
CNcomment:Сʱ������ֻ�����ó���Ӧ��ʮ����ֵ�����ˡ�������ʾ22��45����hour=22��minute=45��\n
CNcomment: ��Ҫ�ȵ���HI_UNF_LED_SetFlashPin ������ʾ�ܽ�ΪHI_UNF_KEYLED_LIGHT_ALL\n
CNcomment: ����ʱ�����ݽṹ��Ȼ�������������ʾ����ʾʱ���ͬʱð�Ż���1HZ��Ƶ����˸\n
CNcomment: ʱ��ķ�Χע�ⰴ24Сʱ�ƣ����֧��СʱΪ23����Ϊ59��

\param[in] stLedTime Time                                                CNcomment:ʱ�䡣
\retval ::HI_SUCCESS  Success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_KEYLED_INVALID_PARA  The input parameter is invalid.    CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_LED_DisplayTime(HI_UNF_KEYLED_TIME_S stLedTime);

/**
\brief Sets the ID of the blinked LED.
CNcomment:\brief ����Ҫ��˸��LED ��š�

\attention \n
The IDs of the LEDs on the board are 1, 2, 3, and 4 from left to right.
CNcomment:�����ϵ�����ܵ���Ŵ���������Ϊ��1��2��3��4��

\param[in] enPin  Pin ID                                                 CNcomment:Ҫ��ʾ����š�
\retval ::HI_SUCCESS Success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_KEYLED_INVALID_PARA  The input parameter is invalid.    CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_E enPin);

/**
\brief Sets the blink level of an LED.
CNcomment:\brief ����LED ��˸����

\attention \n
The higher the level, the faster an LED blinks.
CNcomment:����Խ����˸�ٶ�Խ�졣

\param[in] enLevel  Blink level                                          CNcomment:��˸����
\retval ::HI_SUCCESS Success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE Calling this API fails.                             CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_KEYLED_NOT_INIT  The KEYLED module is not initialized.  CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_KEYLED_INVALID_PARA The input parameter is invalid.     CNcomment:��������Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_E enLevel);

/* ��ʾ��Ƶ��1:����0:�ر�  jzby wrwei 20130123 */
/*modi start*/
HI_S32 HI_UNF_LED_DispLockLed(int lock);
/*modi end*/
/** @} */  /** <!-- ==== API Declaration End ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_5 */
/** @{ */  /** <!-- [PM] */
/*---- pm ----*/
/** 
\brief Starts the power management on chip (PMoC) device.
CNcomment:\brief �򿪵͹���ģ���豸��

\attention \n
The ARM can be switched to another operating mode and then to low-power mode only after the PMoC device is started.
CNcomment:�ڴ򿪵͹���ģ���豸�󣬲����л�ARM�Ĺ���ģʽ���̶�����͹���״̬��

\param N/A                                                               CNcomment:��
\retval 0 Success                                                        CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  The PMoC device is not started.          CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.             CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.            CNcomment:�Ƿ�����
\retval ::HI_ERR_PMoC_INVALID_MODE  The system running mode is invalid.  CNcomment:�Ƿ���ϵͳ����ģʽ
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_Init(HI_VOID);

/** 
\brief Stops the PMoC device.
CNcomment:\brief �رյ͹����豸��

\attention \n
N/A
\param  N/A                                                             CNcomment:��
\retval 0 Success                                                       CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  PMoC  The PMoC device is not started.   CNcomment:�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.            CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.           CNcomment:�Ƿ�����
\retval ::HI_ERR_PMoC_INVALID_MODE  The system running mode is invalid. CNcomment:�Ƿ���ϵͳ����ģʽ
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_DeInit(HI_VOID);

/** 
\brief Sets the mode to which the ARM is switched and obtains the wake-up mode.
CNcomment:\brief ����ARMҪ�л�����ģʽ��ͬʱ��ȡARM���ѵķ�ʽ��

\attention \n
If the ARM enters the HI_UNF_PMOC_MODE_SLEEP mode, programs stop running at once. The subsequent code can be executed only after the ARM is woken up.\n
Before enabling the standby mode, you must start the IR device, key device on the front panel, and real-time clock (RTC) device; otherwise, the interrupts of these devices are masked. As a result, the corresponding wake-up modes cannot be achieved.\n
Do not access DDRs after the ARM enters the sleep mode. Therefore, the functions of display output, video output, DEMUX input, and audio/video decoding must be disabled.
CNcomment:ARM����HI_UNF_PMOC_MODE_SLEEP�����ģʽ���������ֹͣ���У������ڻ��Ѻ���ܼ���ִ�к���Ĵ���\n
CNcomment: �������ģʽǰ�������IR��ң�������豸��ǰ���KEY�豸��RTC�豸�������Ӧģ����жϱ����Σ�����ʵ�ָ���ģʽ�Ļ���\n
CNcomment: ���������ʱ������ȷ������DDR���з��ʡ����Ա���ر���ʾ�������Ƶ�����DEMUX���롢����Ƶ����ȡ�

\param[in] enSystemMode   Mode of the ARM in low-power mode    
			     CNcomment: �õ͹�����ARM����ģʽ��
                         Because the operating frequency of the ARM is reduced, but the power consumption is not reduced significantly in HI_UNF_PMOC_MODE_SLOW and
                         HI_UNF_PMOC_MODE_DOZE modes, the HI_UNF_PMOC_MODE_SLEEP mode is recommended.
                         If the ARM stops in HI_UNF_PMOC_MODE_SLEEP mode, it can be returned to the normal mode.					
			     CNcomment:����HI_UNF_PMOC_MODE_SLOW��
                         CNcomment:HI_UNF_PMOC_MODE_DOZEֻ����ARM����Ƶ�ʣ����Ľ��Ͳ������ԣ��Ƽ�ʹ��HI_UNF_PMOC_MODE_SLEEP�����ģʽ��
                         CNcomment:��ģʽ�³���ֹͣ���У����غ���Normalģʽ��
                         
\param[in]  penWakeUpStatus Wake-up mode returned from the HI_UNF_PMOC_MODE_SLEEP mode. For details about the definition, see the description of ::HI_UNF_PMOC_ACTUAL_WKUP_E. CNcomment: ��HI_UNF_PMOC_MODE_SLEEP״̬����ʱ�Ļ��ѷ�ʽ�����庬����ο�::HI_UNF_PMOC_ACTUAL_WKUP_E
\retval 0 Success                                                          CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT   The PMoC device is not started.           CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.               CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.              CNcomment:�Ƿ�����
\retval ::HI_ERR_PMoC_INVALID_MODE  The system running mode is invalid.    CNcomment:�Ƿ���ϵͳ����ģʽ
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SwitchSystemMode(HI_UNF_PMOC_MODE_E enSystemMode, HI_UNF_PMOC_ACTUAL_WKUP_E * penWakeUpStatus);


/** 
\brief Sets the wake-up mode of the ARM.
CNcomment:\brief ����ARM���ѷ�ʽ��

\attention \n
You can query the original IR code values by referring to IR remote manuals.\n
In addition, you can query the relationships between the keys and key IDs by checking the connection between hardware and chips.
CNcomment:IRԭʼ��ֵ����ͨ��ң������ʹ���ֲ�õ�\n
CNcomment:�������ֵ�Ķ�Ӧ��ϵ����ͨ��Ӳ����оƬ�����ӹ�ϵ�õ���

\param[in] pstAttr  Wake-up mode of the ARM                             CNcomment:ARM���ѷ�ʽ��
\retval 0 Success                                                       CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  The PMoC device is not started.         CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.            CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.           CNcomment:�Ƿ�����
\retval ::HI_ERR_PMoC_INVALID_MODE  The system running mode is invalid. CNcomment:�Ƿ���ϵͳ����ģʽ
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SetWakeUpAttr(HI_UNF_PMOC_WKUP_S_PTR pstAttr);

/**
\brief set display content when wake up 
CNcomment:\brief ���û�����ʾ���ݡ�

\attention \n
content that the panel display when standby
CNcomment:����ʱ�����ʾָ������\n

\param[in] pstStandbyMode  display content:time or channle or no display   CNcomment:��ʾ����:ʱ�� or Ƶ�� or ����ʾ��
\retval 0  success                                                         CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT pmoc device do not open PMoC                CNcomment:�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.               CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.              CNcomment:�Ƿ�����
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SetStandbyDispMode(HI_UNF_PMOC_STANDBY_MODE_S_PTR pstStandbyMode);

/**
\brief Obtains the current system mode.
CNcomment:\brief ��ȡϵͳ��ǰģʽ��

\attention \n
This API is forward compatible and is valid only in normal state.\n
CNcomment:��������״̬����Ч���ӿ�ǰ�����\n

\param[in] penSystemMode  Mode to be obtained                   CNcomment:��Ҫ��ȡ��ģʽ��
\retval 0 Success                                               CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  The PMoC device is not started. CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.    CNcomment:�Ƿ�ָ��
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_ReadSystemMode(HI_UNF_PMOC_MODE_E * penSystemMode);

/** 
\brief Sets the operating scenario.
CNcomment:\brief ���ù���������

\attention \n
Only the standby scenario is supported currently.\n
CNcomment:Ŀǰ��֧�ֱ�׼��������\n

\param[in] eScene  Standby scenario                               CNcomment:����������
\retval 0 Success                                                 CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT    The PMoC device is not started. CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.      CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA  The parameter is invalid.     CNcomment:�Ƿ�����
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SetScene(HI_UNF_PMOC_SCENE_E eScene);


/** 
\brief Sets the types of the devices related to standby such as front panel and IR remote control.
CNcomment:\brief ����Ŀǰǰ���ͬ����ң�صȴ�������豸�����͡�

\attention \n
The remote control supports only HI_UNF_IR_CODE_NEC_SIMPLE.\n
The front panel supports HI_UNF_KEYLED_TYPE_STD and HI_UNF_KEYLED_TYPE_PT6961.\n
CNcomment:ң�ؽ�֧��  HI_UNF_IR_CODE_NEC_SIMPLE ����\n
CNcomment:ǰ���֧��	HI_UNF_KEYLED_TYPE_STD ͬ HI_UNF_KEYLED_TYPE_PT6961 ��������\n	

\param[in] pdevType  Standby device that needs to be supported by the MCU       CNcomment:��ҪMCU֧�ֵĴ����豸��
\retval 0 Success                                                               CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT The PMoC device is not started.                  CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.                    CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA   The parameter is invalid.                  CNcomment:�Ƿ�����
\retval ::HI_ERR_PMOC_FAILED_SETDEV   The setting fails.                        CNcomment:����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SetDevType(HI_UNF_PMOC_DEV_TYPE_S_PTR pdevType);


/** 
\brief Loads the microprogrammed control unit (MCU) codes.
CNcomment:\brief ���ڼ���MCU���롣

\attention \n
This API is only for debugging. After MCU codes are compiled, you can load them by calling this API. This avoids the compilation of hi_c51.ko.\
CNcomment:��������ʹ�ã�MCU���������ɺ󣬿���ʹ������ӿ����룬����ÿ�ζ�����hi_c51.ko\n	

\param[in] pMcuCode Contents o f binary MCU codes               CNcomment:MCU�����ƴ�������
\param[in] CodeSize  Size of binary MCU codes                   CNcomment:MCU�����ƴ��볤��
\retval 0 Success                                               CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  The PMoC device is not started. CNcomment:PMoC�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.    CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA   The parameter is invalid.  CNcomment:�Ƿ�����
\retval ::HI_ERR_PMOC_FAILED_SETDEV   setting fail              CNcomment:����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_LoadMcuCode(HI_U8 *pMcuCode, HI_U32 CodeSize);


/** 
\brief when standby, GPIO pin control 12v and 5v power,relate to hardware
CNcomment:\brief ���ڴ���ʱ,GPIO�ܽſ���12V0��5V0��Դ����Ӳ����ء�

\attention \n
when standby, the outside power controled by STANDBY_PWROFF pin, the function can be ignored
when some part of outside power controled by GPIO pin, use this function will decrease standby power
CNcomment:����ʱ��Χ��Դȫ���ɹܽ�STANDBY_PWROFF����ʱ���������ýӿڣ�
CNcomment:��������Χ��Դ��GPIO�ܽ�ȥ����ʱ,������øýӿ����ڽ��ʹ������ġ�

\param[in] u32GpioNo  GPIO pin index, choose the fifth[40,47] CNcomment:GPIO�ܽ�������,ȡ��5��Ϊ[40, 47]
\param[in] bHighOrLow GPIO output control                     CNcomment:GPIO�ܽ��������
\retval 0  success                                            CNcomment:�ɹ�
\retval ::HI_ERR_PMoC_NOT_INIT  PMoC  device do not open      CNcomment:�豸δ��
\retval ::HI_ERR_PMoC_INVALID_POINT  The pointer is invalid.  CNcomment:�Ƿ�ָ��
\retval ::HI_ERR_PMoC_INVALID_PARA   The parameter is invalid.CNcomment:�Ƿ�����
\retval ::HI_ERR_PMOC_FAILED_SETDEV  setting fail             CNcomment:����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_PMOC_SetPwrOffGpio(HI_U32 u32GpioNo,HI_BOOL bHighOrLow);


/** @} */  /** <!-- ==== API declaration end ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_6 */
/** @{ */  /** <!-- [IR] */
/*---IR---*/
/** 
\brief Starts the IR device.
CNcomment:\brief ��IR�豸��

\attention \n
This API cannot be called repeatedly. Key IDs can be received only after you can start the IR device, and then call HI_UNF_IR_Enable.
CNcomment:�ظ����û�ʧ�ܣ����豸������ٵ���HI_UNF_IR_Enable�����������ռ�ֵ��

\param N/A                                                                   CNcomment:��
\retval 0 Success                                                            CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT   The IR device is not initialized.             CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT The pointer is invalid.                    CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                  CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE    The code type is invalid.            CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Open(HI_VOID);

/**
\brief Stops the IR device.
CNcomment:\brief �ر�IR�豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ����ùرջ�ʧ�ܡ�

\param  N/A                                                                  CNcomment:��
\retval 0 Success                                                            CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.              CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT   The pointer is invalid.                  CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                  CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid  CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.              CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Close(HI_VOID);


/** 
\brief Enables the IR device.
CNcomment:\brief ʹ��IR�豸��
\attention \n
N/A
\param[in] bEnable  IR enable. HI_TRUE: enabled; HI_FALSE: disabled           CNcomment:IR ʹ�ܿ���, HI_TRUE ʹ��, HI_FALSE ���á�
\retval 0 Success                                                             CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.               CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT   The pointer is invalid.                   CNcomment: ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                   CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid  CNcomment: �����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE   The code type is invalid.              CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Enable ( HI_BOOL bEnable);

/** 
\brief Obtains the protocol type of a remote control.
CNcomment:\brief ��ȡң������Э������ ��

\attention \n
To obtain the protocol type of an IR remote control, you need to call HI_UNF_IR_GetValue and HI_UNF_IR_GetProtocol in sequence.
CNcomment:��IR_TYPE!=IR_LIRCʱ��Ч ���ڵ���HI_UNF_IR_GetValue֮�����

\param[out]  penProtocol  protocol type pointer,reference HI_UNF_IR_PROTOCOL_E for detail   CNcomment:Э������ָ��  ���庬����ο�::HI_UNF_IR_PROTOCOL_E
\retval 0 Success                                    CNcomment:�ɹ�
\retval ::HI_ERR_IR_NULL_PTR   The pointer is null.  CNcomment:ָ��Ϊ��
\see \n
N/A
*/

HI_S32 HI_UNF_IR_GetProtocol(HI_UNF_IR_PROTOCOL_E *penProtocol);

/**
\brief obtain name of remote control protocol
CNcomment:\brief ��ȡң������Э������ ��

\attention \n
when IR_TYPE=IR_LIRC is effective, transfer if after HI_UNF_IR_GetValue
CNcomment:��IR_TYPE=IR_LIRCʱ��Ч �ڵ���HI_UNF_IR_GetValue֮���ٵ���

\param[out]  pProtocolName  used to save first address of the protocol name buffer    CNcomment:���ڴ洢Э�����ƵĻ������׵�ַ
\param[in]   s32BufLen      used to save length of the protocol name buffer           CNcomment:���ڴ洢Э�����ƵĻ���������
\retval ::HI_SUCCESS        success                                                   CNcomment:�ɹ�
\retval ::HI_ERR_IR_INVALID_PARA  invalid patameter                                   CNcomment:������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetProtocolName(HI_CHAR *pProtocolName,HI_S32 s32BufLen);


/** 
\brief Obtains the key values and key statuses of the remote control.
CNcomment:\brief ��ȡң�����İ���ֵ�Ͱ���״̬ ��

\attention \n
When you call HI_UNF_IR_GetValue in block mode, an error code is returned if timeout occurs. You can call HI_UNF_IR_SetBlockTime to set the timeout u32BlockTime.
If the u32UserCode parameter of HI_UNF_IR_SelectUserCode is 0, pu32KeyId contains the complete code value (including the user code and data code) of a key. If u32UserCode is not 0, pu32KeyId contains only the data code of a key.
CNcomment:������ʽ�£�����HI_UNF_IR_GetValue����ʱ������ʱ������HI_UNF_IR_SetBlockTime�������ó�ʱʱ��u32BlockTime��ʱ���ش�����Ϣ\n
CNcomment:���HI_UNF_IR_SelectUserCode�ӿڲ���u32UserCodeΪ0����pu32KeyId����������������ֵ���û���������룩������û��벻Ϊ0����pu32KeyIdֻ����������������

\param[out]  penPressStatus  Key status. For details about the definition, see the description of ::HI_UNF_KEY_STATUS_E.  CNcomment:����״̬�����庬����ο�::HI_UNF_KEY_STATUS_E
\param[out]  pu64KeyId  Key value                                                     CNcomment:����ֵ
\param[in] u32TimeoutMs  Timeout (in ms). 0: not blocked; 0xFFFFFFFF: infinite block  CNcomment:��ʱֵ, ��λ�Ǻ���, 0 - ������, 0xFFFFFFFF-��������
\retval 0 Success                                                                     CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                       CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT  The pointer is invalid.                            CNcomment: ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                           CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid          CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid        CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.                       CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
#define HI_UNF_IR_GetValue(penPressStatus, pu64KeyId, u32TimeoutMs) HI_UNF_IR_GetValueWithProtocol(penPressStatus, pu64KeyId, NULL, 0, u32TimeoutMs)

HI_S32 HI_UNF_IR_GetValueWithProtocol(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId, 
        HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs);
/** 
\brief Set key fetch or symbol fetch from ir driver.
CNcomment:\brief �趨�Ӻ���������ȡ���Ǽ�ֵ�������ƽ ��

\attention \n

\param[in] mode 0 means key mode. 1 means symbol mode                                 CNcomment:0��ȡ��ֵ��1��ȡ���ƽ
\retval 0 Success                                                                     CNcomment:�ɹ�
\retval :: others on fail                                                             CNcomment:����ֵΪʧ�ܡ�  
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetFetchMode(HI_S32 s32mode);

/** 
\brief Obtains the raw symbols from ir driver.
CNcomment:\brief ��ȡң���������ƽ ��

\attention \n

\param[out]  pu64lower  lower pluse value                                             CNcomment:���ƽ�Եĵ�λ����Ӧ���ȵͺ�߸�ʽ�ĵ͵�ƽ
\param[out]  pu64upper  upper space value                                             CNcomment:���ƽ�Եĸ�λ����Ӧ���ȵͺ�߸�ʽ�ĸߵ�ƽ
\param[in] s32TimeoutMs read timeout .                                               CNcomment:����ʱʱ�䡣

\retval 0 Success                                                                     CNcomment:�ɹ�
        others on fail.                                                               CNcomment:����ֵΪʧ�ܡ�  
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetSymbol(HI_U64 *pu64First, HI_U64* pu64Second, HI_U32 u32TimeoutMs);

/** 
\brief Enables or disables the function of reporting the released status of a key.
CNcomment:\brief �����Ƿ��ϱ���������״̬��

\attention \n
The function is enabled by default.
CNcomment:�粻���ã�Ĭ��Ϊ�򿪡�

\param[in] bEnable      Key released enable.  CNcomment:����������Ч��
                        0: disabled           CNcomment:0���رգ�
                        1: enabled            CNcomment: 1��ʹ�ܡ�
\retval 0  Success                                                      CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.         CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT  The pointer is invalid.              CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.             CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid   CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.                CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable);


/** 
\brief Enables or disables the function of reporting the same key value. If keys are pressed and held down, data is continuously transmitted to the receive buffer. Therefore, you can enable or disable this function for applications as required.
CNcomment:\brief �����Ƿ��ϱ��ظ�������һֱ����ʱ�����ݻ�ԴԴ���ϵ����뵽���ջ����������Ӧ�ó�����Ը�����Ҫ�������Ƿ��ϱ��ظ�������

\attention \n
The function is enabled by default.\n
This API must work with HI_UNF_IR_RepKeyTimeoutVal. The API HI_UNF_IR_RepKeyTimeoutVal is used to set the interval of reporting the same key value.\n
If the function of reporting the same key value is enabled, the keys are pressed and held down, and the interval is set to 300 ms, data is reported once every 300 ms.
If the function is disabled, data is reported only once regardless of how long the keys are hold down.
CNcomment:�粻���ã�Ĭ��Ϊ��\n
CNcomment:�˽ӿ���Ҫ��HI_UNF_IR_RepKeyTimeoutVal�������ʹ�ã���HI_UNF_IR_RepKeyTimeoutVal�����ϱ��ظ������ļ��\n
CNcomment:���ʹ�����ظ������ϱ���������һֱ���ڰ���״̬�������Ϊ300���룬��ÿ300������ϱ�һ������\n
CNcomment:�����ֹ���ظ������ϱ������۰��¶೤ʱ�䣬ֻ�ϱ�һ�����ݡ�
\param[in] bEnable     Repeat key report enable.    CNcomment:���������ظ��������ܡ�
                       0: disabled                  CNcomment:0���رգ�
                       1: enabled                   CNcomment:1��ʹ�ܡ�
\retval 0 Success 
\retval ::HI_ERR_IR_NOT_INIT   The IR device is not initialized.               CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT  The pointer is invalid.                     CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                   CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid   CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.                CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable);


/** 
\brief Sets the interval (in ms) of reporting the same key value.
CNcomment:\brief �����ϱ��ظ�������ʱ��������λΪms ��

\attention \n
This API is unavailable if the function of reporting the same key value is disabled by calling HI_UNF_IR_IsRepKey.
CNcomment:��HI_UNF_IR_IsRepKey����Ϊ���ϱ��ظ�����ʱ���˽ӿ�������Ч��

\param[in] u32TimeoutMs   Interval of reporting the same key value. The recommended interval ranges from 108 ms to 65,536 ms. CNcomment:�ϱ��ظ�������ʱ����������ֵ��108ms��65536ms ��
\retval 0 Success CNcomment:                                                   CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT The pointer is invalid.                      CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.                    CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid   CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid  CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.                CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs);

/** 
\brief Sets the code type of the remote control.
CNcomment:\brief ����ң�������͡�

\attention \n
The default code type is HI_UNF_IR_CODE_NEC_SIMPLE.
CNcomment:�粻���ã�Ĭ��ΪHI_UNF_IR_CODE_NEC_SIMPLE���͡�

\param[in] enIRCode Four standard code types of the remote control are supported by default.  CNcomment:Ĭ��֧��4�ֱ�׼ң�������ͣ�
                     HI_UNF_IR_CODE_NEC_SIMPLE��  Nec With Simple code                        CNcomment: Nec With Simple���ͣ�
                     HI_UNF_IR_CODE_TC9012��  TC9012 code                                     CNcomment:TC9012���ͣ�
                     HI_UNF_IR_CODE_NEC_FULL��  Nec With Full code                            CNcomment:Nec With Full���ͣ�
                     HI_UNF_IR_CODE_SONY_12BIT�� Sony 12-Bit code                             CNcomment:Sony12Bit���͡�
\retval 0 Success                                                CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.  CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT  The pointer is invalid.       CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA  The parameter is invalid.      CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE The code type is invalid.   CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_E enIRCode);


/** 
\brief  Resets the IR device.
CNcomment:\brief  ��λIR�豸��

\attention \n
This API is used to delete the key values in the buffer. After the values are deleted, you do not need to set the user code again.
CNcomment:ֻ�ǰ�buffer�еļ�ֵ��������֮����Ҫ���������û��롣

\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::HI_ERR_IR_NOT_INIT  The IR device is not initialized.                 CNcomment:IR�豸δ��ʼ��
\retval ::HI_ERR_IR_INVALID_POINT  The pointer is invalid.                      CNcomment:ָ��Ϊ��
\retval ::HI_ERR_IR_INVALID_PARA   The parameter is invalid.                    CNcomment:�����Ƿ�
\retval ::HI_ERR_IR_INVALID_ISKEYUP_ARG  The parameter of iskeyup is invalid    CNcomment:�����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_ISREPKEY_ARG  The parameter of isrepkey is invalid  CNcomment:�ظ����Ƿ��ϱ������Ƿ�
\retval ::HI_ERR_IR_INVALID_CODETYPE  The code type is invalid.                 CNcomment:���Ͳ����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_Reset(HI_VOID);

/** 
\brief enable a infrared code.
CNcomment:\brief  ����ĳ�����ң����Э�顣

\attention \n
?
\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::Others on fail. CNcomment:����ֵʧ�ܡ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName);

/** 
\brief disable a infrared code.
CNcomment:\brief  ����ĳ�����ң����Э�顣

\attention \n

\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::Others on fail. CNcomment:����ֵʧ�ܡ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName);

/** 
\brief get the enable or disable stat of  a infrared code.
CNcomment:\brief  ��ȡĳ�����ң����Э�鼤��״̬��

\attention \n

\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::Others on fail. CNcomment:����ֵʧ�ܡ�
\see \n
N/A
*/
HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled);


/** @} */  /** <!-- ==== API Declaration End ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_7 */
/** @{ */  /** <!-- [WDG] */
/*---- wdg ----*/
/** 
\brief Starts the WDG device.
CNcomment:\brief ��WDG��Watch Dog���豸��

\attention \n
By default, the WDG device is disabled after it is started. In this case, you need to call HI_UNF_WDG_Enable to enable it.\n
This API cannot be called repeatedly.
CNcomment:��֮��WDGĬ���ǽ�ֹ�ģ���Ҫ��ʽ����HI_UNF_WDG_Enableʹ��WDG�豸\N
CNcomment:�ظ��򿪻�ʧ�ܡ�

\param N/A          CNcomment:��
\retval 0 Success   CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT   The WDG device is not initialized.     CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT  The WDG input pointer is invalid.  CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_Open(HI_VOID);


/** 
\brief Stops the WDG device.
CNcomment:\brief �ر�WDG�豸��

\attention \n
This API cannot be called repeatedly.
CNcomment:�ظ��رջ�ʧ�ܡ�

\param N/A          CNcomment:��
\retval 0 Success   CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT  The WDG input pointer is invalid.  CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_Close(HI_VOID);


/** 
\brief Enables the WDG device.
CNcomment:\brief ʹ��WDG�豸��

\attention \n
You must call HI_UNF_WDG_Enable after the WDG device is started.
CNcomment:��WDG�豸�󣬱�����ʽ����ʹ�ܽӿڡ�

\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT The WDG device is not initialized.       CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT   The WDG input pointer is invalid. CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_Enable(HI_VOID);

/** 
\brief Disables the WDG device.
CNcomment:\brief ��ֹWDG�豸��

\attention \n
After calling this API, you cannot feed and reset the WDG.
CNcomment:���ô˺�����ι���͸�λ�����������á�

\param N/A         CNcomment:��
\retval 0 Success  CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT The WDG input pointer is invalid.   CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA The WDG input parameter is invalid.  CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_Disable(HI_VOID);


/** 
\brief Obtains the interval of feeding the WDG.
CNcomment:\brief ��ȡι��ʱ������

\attention \n
The interval precision is as high as 1000 ms.
CNcomment:ʱ������ȷ��1000ms��

\param[in] pu32Value  Interval of feeding the WDG, in ms              CNcomment:ι��ʱ��������λΪms
\retval 0 Success                                                     CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.     CNcomment:WDG �豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT  The WDG input pointer is invalid. CNcomment:WDG����ָ����Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_GetTimeout(HI_U32 *pu32Value);

/** 
\brief Sets the interval of feeding the WDG.
CNcomment:\brief ����ι��ʱ������

\attention \n
N/A
\param[in] u32Value  Interval of feeding the WDG, in ms                CNcomment:ι��ʱ��������λΪms
\retval 0 Success                                                      CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT The WDG device is not initialized.       CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT The WDG input pointer is invalid.   CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_SetTimeout(HI_U32 u32Value);


/** 
\brief Feeds the WDG.
CNcomment:\brief ִ��ι��������

\attention \n
N/A
\param N/A                                                             CNcomment:��
\retval 0 Success                                                      CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT  The WDG input pointer is invalid.  CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_ClearWatchDog (HI_VOID);

/** 
\brief Resets the entire system.
CNcomment:\brief ���ڸ�λ����ϵͳ��

\attention \n
N/A
\param N/A        CNcomment:��
\retval 0 Success CNcomment:�ɹ�
\retval ::HI_ERR_WDG_NOT_INIT  The WDG device is not initialized.      CNcomment:WDG�豸δ��ʼ��
\retval ::HI_ERR_WDG_INVALID_POINT The WDG input pointer is invalid.   CNcomment:WDG����ָ����Ч
\retval ::HI_ERR_WDG_INVALID_PARA  The WDG input parameter is invalid. CNcomment:WDG���������Ч
\see \n
N/A
*/
HI_S32 HI_UNF_WDG_Reset(HI_VOID);

/** @} */  /** <!-- ==== API Declaration End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_8 */
/** @{ */  /** <!-- [GPIO] */
/**---- GPIO----*/
/** 
\brief Starts the general-purpose input/output (GPIO) device.
CNcomment:\brief ��GPIO��General Purpose Input/Output���豸��

\attention \n
This API can be called repeatedly.
CNcomment:�ظ��򿪻�ɹ���

\param  N/A       CNcomment:��
\retval 0 Success CNcomment:�ɹ�
\retval ::HI_ERR_GPIO_NOT_INIT  The GPIO device is not initialized.  CNcomment:GPIO�豸δ��ʼ��
\retval ::HI_ERR_GPIO_INVALID_POINT  The pointer is invalid.         CNcomment:ָ��Ϊ��
\retval ::HI_ERR_GPIO_INVALID_PARA  The parameter is invalid.        CNcomment:�����Ƿ�
\retval ::HI_ERR_GPIO_FAILED_SWITCH2LED  The peripheral function fails to be switched to the GPIO function.                 CNcomment:���蹦���л���GPIO���ܲ���ʧ��
\retval ::HI_ERR_GPIO_FAILED_LED_DIRSET_BIT  The direction of the GPIO pin corresponding to the tuner LED fails to be set.  CNcomment:TUNERָʾ�ƶ�ӦGPIO�ܽŷ�������ʧ��
\retval ::HI_ERR_GPIO_FAILED_LED_WRITE_BIT  The GPIO pin corresponding to the tuner LED fails to be written.                CNcomment:дTUNERָʾ�ƶ�ӦGPIO�ܽŲ���ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_Open(HI_VOID);


/** 
\brief Stops the GPIO device.
CNcomment:\brief �ر�GPIO�豸��

\attention \n
This API can be called repeatedly.
CNcomment:�ظ��رջ�ɹ���
\param  N/A  CNcomment:��
\retval For details, see the definitions of error codes. CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_Close(HI_VOID);

/** 
\brief Reads data from a GPIO pin.
CNcomment:\brief ��GPIO�������Ŷ�ȡ���ݡ�

\attention The pin number is defined as follows: Pin number = GPIO group ID x 8 + GPIO pin ID in the group\n
For example, GPIO1_2 indicates pin 2 in group 1, and the pin number is 10 (1 x 8 + 2).\n
Both pin group ID and pin number are numbered from 0. Each HD chip provides GPIO pins number reference to HD chip hardware manual.\n
N/A
CNcomment:\attention �ܽźż������: �ܽź� = GPIO���*8 + GPIO�ܽ������ڵı�š�\n
CNcomment:����GPIO1_2,�����1���2�ţ���ô�ܽź�=1*8+2=10��\n
CNcomment:GPIO����ź͹ܽźŵı�Ŷ���0��ʼ������оƬ�ṩ��gpio�ܽ�����ο���ص�оƬӲ���ֲ�\n

\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[out] pbHighVolt   Pointer to the input level of a pin  CNcomment:ָ�����ͣ����عܽ������ƽ��
\retval   For details, see the definitions of error codes.    CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_ReadBit(HI_U32  u32GpioNo , HI_BOOL  *pbHighVolt );


/** 
\brief Writes data to a GPIO pin.
CNcomment:\brief ��GPIO��������������ݡ�
\attention \n
N/A
\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[in] bHighVolt  Output level of a pin              CNcomment:�ܽ������ƽ��
                      0: low level                       CNcomment:0: �͵�ƽ��
                      1: high level                      CNcomment:1���ߵ�ƽ��
                     Others: high level                  CNcomment:�������ߵ�ƽ��
\retval  For details, see the definitions of error codes. CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_WriteBit(HI_U32  u32GpioNo , HI_BOOL     bHighVolt );


/** 
\brief Sets the direction (input or output) of a GPIO pin.
CNcomment:\brief ����GPIO�������������������

\attention \n
When setting the operating mode of a GPIO pin, ensure that it works in GPIO mode only.\n
This is because the GPIO pin may be multiplexed.
CNcomment:���ö�Ӧ�ܽŵĹ�����ʽʱ�����뱣֤�ùܽŽ�������GPIOģʽ��\n
CNcomment:GPIO�ܽ��п��ܱ����á�

\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[in] bInput  Boolean variable that indicates the direction of a pin  CNcomment:������������ʶ�ܽŷ���
                   HI_TRUE: input pin  HI_TRUE��                           CNcomment:�ùܽ��������룻
                   HI_FALSE: output pin HI_FALSE��                         CNcomment:�ùܽ����������
\retval For details, see the definitions of error codes.                   CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_SetDirBit(HI_U32  u32GpioNo , HI_BOOL    bInput);


/** 
\brief Obtains the direction (input or output) of a GPIO pin.
CNcomment:\brief ��ȡGPIO�������������������

\attention \n
N/A
\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[out] pbInput   Pointer to the boolean variable that indicates the direction of a pin  CNcomment:ָ�����ͣ�ָ�򲼶��ͱ������������عܽŷ���
\retval For details, see the definitions of error codes.                                     CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_GetDirBit(HI_U32  u32GpioNo , HI_BOOL    *pbInput);


/** 
\brief Starts the cipher device.
CNcomment:\brief ����GPIO�������ŵ��ж����͡�

\attention \n
N/A
\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[in] enIntType    interrupt type                                CNcomment:�ж����͡�
\retval    For details, see the definitions of error codes.           CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_SetIntType(HI_U32 u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntType);


/** 
\brief set GPIO single pin interrupt enable
CNcomment:\brief ����GPIO�������ŵ��ж�ʹ�ܡ�
\attention \n
N/A
\param[in] u32GpioNo  Pin number, ranging from 0 to 103, ranging is different in otherness chip type     CNcomment:�ܽźţ�ȡֵ��ΧΪ0��103����ͬ��оƬ���ͷ�Χ��һ����
\param[in] bEnable  HI_TRUE: interrupt enable, HI_FALSE: interrupt disable CNcomment:HI_TRUE: �ж�ʹ�� ,HI_FALSE: �жϽ�ֹ��
\retval For details, see the definitions of error codes.                   CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_SetIntEnable(HI_U32 u32GpioNo, HI_BOOL bEnable); 


/** 
\brief query GPIO interrupt, report it if there is interrupter happen
CNcomment:\brief ��ѯGPIO�жϣ�ֻҪ��GPIO�жϾͻ��ϱ����� 
\attention \n
N/A
\param[out] p32GpioNo    get interrupt pin number        CNcomment:��ȡ�жϵĹܽźš�
\param[in] u32TimeoutMs  get interrupt timeout           CNcomment:��ȡ�жϳ�ʱʱ�䡣
\retval For details, see the definitions of error codes. CNcomment:��ο������붨���½�
\see \n
N/A
*/
HI_S32 HI_UNF_GPIO_QueryInt(HI_U32 *p32GpioNo, HI_U32 u32TimeoutMs);


/** @} */  /** <!-- ==== API declaration end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_9 */
/** @{ */  /** <!-- [CIPHER] */
/* ---CIPHER---*/
/** 
\brief  Starts the cipher device.  CNcomment:��CIPHER�豸��
\attention \n
This API is used to start the cipher device. The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:���ô˽ӿڴ�CIPHER�豸���ظ��򿪷��سɹ���
\param N/A                   CNcomment:��
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                                    CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_FAILED_INIT  The cipher device fails to be initialized.  CNcomment:CIPHER�豸��ʼ��ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Open(HI_VOID);

/** 
\brief  Stops the cipher device.
CNcomment:\brief  �ر�CIPHER�豸��
\attention \n
This API is used to stop the cipher device. If this API is called repeatedly, HI_SUCCESS is returned, but only the first operation takes effect.
CNcomment:���ô˽ӿڹر�CIPHER�豸���ظ��رշ��سɹ�����һ�������á�

\param N/A                   CNcomment:��
\retval ::HI_SUCCESS Success  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  CNcomment:CIPHER�豸δ��ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Close(HI_VOID);

/** 
\brief Creates a cipher channel to obtain the cipher handle.
CNcomment:\brief ����һ·CIPHER����ȡCIPHER�����

\attention \n
This API is used to obtain a cipher handle for encryption and decryption.
CNcomment:���ô˽ӿڻ�ȡ1��CIPHER��������ڼӽ��ܡ�

\param[out] phCipher Cipher handle                                       CNcomment:CIPHER�����
\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment: APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized.  CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.              CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_CIPHER_FAILED_GETHANDLE  The cipher handle fails to be obtained, because there are no available cipher handles. CNcomment: ��ȡCIPHER���ʧ�ܣ�û�п��е�CIPHER���

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_CreateHandle(HI_HANDLE* phCipher);

/** 
\brief Destroys existing cipher handles. CNcomment:���ٴ�����CIPHER�����
\attention \n
This API is used to destroy existing cipher handles.
CNcomment:���ô˽ӿ������Ѿ�������CIPHER�����

\param[in] hCipher Cipher handle                 CNcomment:CIPHER�����
\retval ::HI_SUCCESS Success                     CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.    CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. CNcomment:CIPHER�豸δ��ʼ��
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_DestroyHandle(HI_HANDLE hCipher);

/** 
\brief Configures the cipher control information.
CNcomment:\brief ����CIPHER������Ϣ

\attention \n
Before encryption or decryption, you must call this API to configure the cipher control information.
The first 64-bit data and the last 64-bit data should not be the same when using TDES algorithm.
CNcomment:���м��ܽ���ǰ������ʹ�ô˽ӿ�����CIPHER�Ŀ�����Ϣ��ʹ��TDES�㷨ʱ��������Կ��ǰ��64 bit���ݲ�����ͬ��

\param[in] hCipher Cipher handle                                        CNcomment:CIPHER�����
\param[in] pstCtrl Cipher control information                           CNcomment:CIPHER������Ϣ��
\retval ::HI_SUCCESS Success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_POINT  The pointer is null.             CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         CNcomment:��������
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          CNcomment:����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_ConfigHandle(HI_HANDLE hCipher, HI_UNF_CIPHER_CTRL_S* pstCtrl);

/** 
\brief Performs encryption. 
CNcomment:\brief ���м��� 

\attention \n
This API is used to perform encryption by using the cipher module.
The length of the encrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.
CNcomment:ʹ��CIPHER���м��ܲ�����TDESģʽ�¼��ܵ����ݳ���Ӧ����8�ı�����AES��Ӧ����16�ı��������⣬�������ݳ��Ȳ��ܳ���0xFFFFF��
    After this operation, the result will affect next operation
    if you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_UNF_CIPHER_ConfigHandle
    CNcomment: ���β�����ɺ󣬴˴β�������������������������һ�β�����
    CNcomment:���Ҫ�����������Ҫ���´μ��ܲ���֮ǰ����HI_UNF_CIPHER_ConfigHandle��������IV(��Ҫ����pstCtrl->stChangeFlags.bit1IVΪ1),
\param[in] hCipher Cipher handle  CNcomment:CIPHER�����
\param[in] u32SrcPhyAddr Physical address of the source data            CNcomment:Դ���������ַ��
\param[in] u32DestPhyAddr Physical address of the target data           CNcomment:Ŀ�����������ַ��
\param[in] u32ByteLength   Length of the encrypted data                 CNcomment:�������ݳ��ȡ�
\retval ::HI_SUCCESS  Success                                           CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         CNcomment:��������
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          CNcomment:����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Encrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);

/** 
\brief Performs encryption.
CNcomment:\brief ���м���

\attention \n
This API is used to perform decryption by using the cipher module.
The length of the decrypted data should be a multiple of 8 in TDES mode and 16 in AES mode. Besides, the length can not be bigger than 0xFFFFF.
CNcomment:ʹ��CIPHER���н��ܲ�����TDESģʽ�½��ܵ����ݳ���Ӧ����8�ı�����AES��Ӧ����16�ı��������⣬�������ݳ��Ȳ��ܳ���0xFFFFF��
    After this operation, the result will affect next operation
    if you want to remove vector, you need to config IV(config pstCtrl->stChangeFlags.bit1IV with 1) by transfering HI_UNF_CIPHER_ConfigHandle
CNcomment:���β�����ɺ󣬴˴β�������������������������һ�β�����
 CNcomment:���Ҫ�����������Ҫ���´ν��ܲ���֮ǰ����HI_UNF_CIPHER_ConfigHandle��������IV(��Ҫ����pstCtrl->stChangeFlags.bit1IVΪ1),
\param[in] hCipher Cipher handle                                        CNcomment:CIPHER�����
\param[in] u32SrcPhyAddr Physical address of the source data            CNcomment:Դ���������ַ��
\param[in] u32DestPhyAddr Physical address of the target data           CNcomment:Ŀ�����������ַ��
\param[in] u32ByteLength Length of the decrypted data                   CNcomment:�������ݳ��ȡ�
\retval ::HI_SUCCESS Success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  The cipher device is not initialized. CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_PARA  The parameter is invalid.         CNcomment:��������
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  The handle is invalid.          CNcomment:����Ƿ�
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_Decrypt(HI_HANDLE hCipher, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength);


/** 
\brief encrypt to multi data package
CNcomment:\brief ���ж�������ݵļ���
\attention \n
you can not cipher to more than 128 data package once time
when HI_ERR_CIPHER_BUSY return, the data package you send will not be deal, the custmer should decrease the number of data package or cipher again
note:The operation to more than one data package, every one will calculate usint vector configed by HI_UNF_CIPHER_ConfigHandle
     The font result will not affect the later result 
CNcomment:ÿ�μ��ܵ����ݰ�������಻�ܳ���128����
CNcomment:����HI_ERR_CIPHER_BUSY��ʱ���������ݰ�һ��Ҳ���ᱻ�����û���Ҫ������������ݰ������������ٴγ��Լ��ܡ�
CNcomment:   ע��: ���ڶ�����Ĳ�����ÿ������ʹ��HI_UNF_CIPHER_ConfigHandle���õ������������㣬
CNcomment:    ǰһ����������������������������һ���������㣬ÿ�������Ƕ�������ġ�
CNcomment:    ǰһ�κ������õĽ��Ҳ����Ӱ���һ�κ������õ���������
\param[in] hCipher CIPHER handle                                                                  CNcomment:CIPHER�����
\param[in] pstDataPkg data package ready for cipher                                               CNcomment:�����ܵ����ݰ���
\param[in] u32DataPkgNum  number of package ready for cipher                                      CNcomment:�����ܵ����ݰ�������
\retval ::HI_SUCCESS   success                                                                    CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                                                    CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  CIPHER device have not been initialized                         CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_PARA  parameter error                                             CNcomment:��������
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  handle invalid                                            CNcomment:����Ƿ�
\retval ::HI_ERR_CIPHER_BUSY  hardware is busy, it can not deal with all data package once time   CNcomment:Ӳ����æ���޷�һ���Դ���ȫ�������ݰ�
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_EncryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);


/** 
\brief decrypt to multi data package
CNcomment:\brief ���ж�������ݵĽ���
\attention \n
you can not decrypt to more than 128 data package once time
when HI_ERR_CIPHER_BUSY return, the data package you send will not be deal, the custmer should decrease the number of data package or decrypt again
note:The operation to more than one data package, every one will calculate usint vector configed by HI_UNF_CIPHER_ConfigHandle
     The font result will not affect the later result 
CNcomment:ÿ�ν��ܵ����ݰ�������಻�ܳ���128����
CNcomment:����HI_ERR_CIPHER_BUSY��ʱ���������ݰ�һ��Ҳ���ᱻ�����û���Ҫ������������ݰ������������ٴγ��Խ��ܡ�
CNcomment:    ע��: ���ڶ�����Ĳ�����ÿ������ʹ��HI_UNF_CIPHER_ConfigHandle���õ������������㣬
CNcomment:    ǰһ����������������������������һ���������㣬ÿ�������Ƕ�������ġ�
CNcomment:    ǰһ�κ������õĽ��Ҳ����Ӱ���һ�κ������õ���������
\param[in] hCipher CIPHER handle                                                                 CNcomment:CIPHER�����
\param[in] pstDataPkg data package ready for cipher                                              CNcomment:�����ܵ����ݰ���
\param[in] u32DataPkgNum  number of package ready for cipher                                     CNcomment:�����ܵ����ݰ�������
\retval ::HI_SUCCESS success                                                                     CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                                                    CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CIPHER_NOT_INIT  CIPHER device have not been initialized                        CNcomment:CIPHER�豸δ��ʼ��
\retval ::HI_ERR_CIPHER_INVALID_PARA  parameter error                                            CNcomment:��������
\retval ::HI_ERR_CIPHER_INVALID_HANDLE  handle invalid                                           CNcomment:����Ƿ�
\retval ::HI_ERR_CIPHER_BUSY  hardware is busy, it can not deal with all data package once time  CNcomment:Ӳ����æ���޷�һ���Դ���ȫ�������ݰ�
\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_DecryptMulti(HI_HANDLE hCipher, HI_UNF_CIPHER_DATA_S *pstDataPkg, HI_U32 u32DataPkgNum);

/** 
\brief Get the random number.
CNcomment:\brief ��ȡ�����

\attention \n
This API is used to obtain the random number from the hardware.
CNcomment:���ô˽ӿ����ڻ�ȡ�߼���������

\param[out] pu32RandomNumber The random number                           CNcomment:�������ֵ��
\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                            CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_GetRandomNumber(HI_U32 *pu32RandomNumber);

/** 
\brief Get the AES CBC-MAC.
CNcomment:\brief ��ȡAES CBC-MAC

\attention \n
This API is used to obtain the AES CBC-MAC.
CNcomment:���ô˽ӿ����ڻ�ȡAES CBC-MAC

\param[in] hCipherHandle The cipher handle                        CNcomment:Cipher���
                 pInputData: input data                                     CNcomment:��������
                 u32InputDataLen: input data length,                  CNcomment:�������ݵĳ��ȣ�����������һ��block�������Ϊ16��byte�ı��� 
                 the length should be multiple of 16 bytes if it is not the last block
                 bIsLastBlock: is last block                                CNcomment:�Ƿ����һ��block
\param[out] pOutputMAC: output CBC-MAC                          CNcomment:�����CBC-MAC                 
\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_CalcMAC(HI_HANDLE hCipherHandle, HI_U8 *pInputData, HI_U32 u32InputDataLen,
                                        HI_U8 *pOutputMAC, HI_BOOL bIsLastBlock);

/** 
\brief Init the hash module, if other program is using the hash module, the API will return failure.
CNcomment:\brief ��ʼ��HASHģ�飬�����������������ʹ��HASHģ�飬����ʧ��״̬

\attention \n
��.
CNcomment:NONE

\param[in] eShaType Hash type                                           CNcomment:hash����
                 u32TotalDataLen: the total length of data to calculate the hash  CNcomment:���ڼ���hash�����ݵ��ܳ���
 \param[out] pHashHandle: the output hash handle               CNcomment:�����hash���
\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashInit(HI_UNF_CIPHER_HASH_TYPE_E eShaType, HI_U32 u32TotalDataLen, HI_HANDLE *pHashHandle);


/** 
\brief Calculate the hash, if the size of the data to be calculated is very big and the DDR ram is not enough, this API can 
calculate the data one block by one block
CNcomment:\brief ����hashֵ�������Ҫ������������Ƚϴ󣬸ýӿڿ���
ʵ��һ��blockһ��block�ļ��㣬�����������Ƚϴ������£��ڴ治�������

\attention \n
��
CNcomment:NONE

\param[in] hHashHandle Hash ���                                  CNcomment:hash handle
               pu8InputData: �������ݻ���                   CNcomment:the input data buffer
               u32InputDataLen:�������ݵĳ���         CNcomment:the input data length
\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashUpdate(HI_HANDLE hHashHandle, HI_U8 *pu8InputData, HI_U32 u32InputDataLen);



/** 
\brief Get the final hash value, after calculate all of the data, call this API to get the final hash value and close the handle.
If there is some reason need to interupt the calculation, this API should also be call to close the handle
CNcomment:\brief ��ȡhashֵ���ڼ��������е����ݺ󣬵�������ӿڻ�ȡ���յ�
hashֵ���ýӿ�ͬʱ��ر�hash���������ڼ�������У���Ҫ�жϼ��㣬Ҳ
������øýӿڹر�hash���

\attention \n
��
CNcomment:NONE

\param[in] hHashHandle Hash ���                                  CNcomment:hash handle
\param[out] pu8OutputHash �����hashֵ                   CNcomment:the final output hash value

\retval ::HI_SUCCESS Success                                             CNcomment: �ɹ�
\retval ::HI_FAILURE  Calling this API fails.                           CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_CIPHER_HashFinal(HI_HANDLE hHashHandle, HI_U8 *pu8OutputHash);



/** @} */  /** <!-- ==== API declaration end ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_4_10 */
/** @{ */  /** <!-- [TUNER] */
/*---TUNER---*/
/** 
\brief Initializes the tuner.
CNcomment:\brief ��ʼ��TUNER�豸��

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:֧���ظ���ʼ��TUNER�豸����ʱֱ�ӷ��سɹ���
\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Init(HI_VOID);


/** 
\brief Deinitializes the tuner.
CNcomment:\brief ȥ��ʼ��TUNER�豸��

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:֧���ظ�ȥ��ʼ��TUNER�豸����ʱֱ�ӷ��سɹ���
\param  N/A                    CNcomment:��
\retval ::HI_SUCCESS Success   CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DeInit(HI_VOID);


/**
\brief  Obtains the default attributes of the tuner. 
CNcomment:\brief  ��ȡTUNER��Ĭ�����ԡ�
\attention \n
This API is available only after the tuner is initialized.
CNcomment:�˽ӿ���TUNER�豸��ʼ�������ʹ�á�
\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.             CNcomment:TUNER�˿ںţ�ȡֵΪ0-2��
\param[out] pstTunerAttr The default attributes of the tuner are returned. CNcomment:���ص�ǰTUNER��Ĭ�����ԡ�
\retval ::HI_SUCCESS Success                                               CNcomment: �ɹ�
\retval ::HI_FAILURE Calling this API fails.                               CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetDeftAttr(HI_U32  u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );


/** 
\brief Sets the current attributes of the tuner.
CNcomment:\brief ����TUNER��ǰ���ԡ�
\attention \n
N/A
\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.      CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstTunerAttr tuner attributes                            CNcomment:TUNER�����ԡ�
\retval ::HI_SUCCESS Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                       CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetAttr(HI_U32  u32tunerId , const HI_UNF_TUNER_ATTR_S *pstTunerAttr );


/** 
\brief Obtains the attributes of the tuner.
CNcomment:\brief ��ȡTUNER�����ԡ�

\attention \n
This API is available only after the tuner is initialized.
CNcomment:�˽ӿ���TUNER�豸��ʼ�������ʹ�á�

\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.                CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pstTunerAttr   The current attributes of the tuner are returned. CNcomment:���ص�ǰTUNER�����ԡ�
\retval ::HI_SUCCESS Success                                                 CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                                CNcomment: APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetAttr(HI_U32  u32tunerId , HI_UNF_TUNER_ATTR_S *pstTunerAttr );


/** 
\brief Starts the tuner.
CNcomment:\brief ��TUNER�豸��

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:֧���ظ���TUNER�豸����ʱ��ֱ�ӷ��سɹ���

\param[in] u32tunerId   tuner port ID. The port ID can be 0-2.      CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE Calling this API fails.                        CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Open (HI_U32    u32tunerId);


/** 
\brief Stops the tuner.
CNcomment:\brief �ر�TUNER�豸��

\attention \n
The error code HI_SUCCESS is returned if this API is called repeatedly.
CNcomment:֧���ظ��ر�TUNER�豸����ʱ��ֱ�ӷ��سɹ���

\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.     CNcomment:TUNER�˿ںţ�ȡֵΪ0-2 
\retval ::HI_SUCCESS Success                                      CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                     CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Close(HI_U32    u32tunerId);


/** 
\brief Locks the frequency of the tuner.
CNcomment:\brief TUNER��Ƶ��
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.             CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstConnectPara   Information about the frequency of the tuner  CNcomment:TUNER��Ƶ����Ϣ��
\param[in] u32TimeOut  Wait timeout (in ms) when the frequency of the tuner is locked. The value 0 indicates no wait, and any other value indicates the maximum wait period.  CNcomment:TUNER�������ȴ���ʱʱ�䣬0Ϊ���ȴ�������Ϊ��ȴ�ʱ�䣬��λms��
\retval ::HI_SUCCESS Success                                              CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                             CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Connect(HI_U32  u32tunerId , const HI_UNF_TUNER_CONNECT_PARA_S  *pstConnectPara,HI_U32 u32TimeOut);


/** 
\brief Obtains the frequency locking status and parameters of the tuner.
CNcomment:\brief ��ȡTUNER��Ƶ״̬����Ƶ������
\attention \n
N/A
\param[in] u32tunerId tuner port ID. The port ID can be 0-2.             CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] u32TimeOut frequency locking timeout, unit: ms, frequency locking time is related to strength of signal CNcomment:��Ƶ�ĳ�ʱʱ�䣬��λ�Ǻ��룬��Ƶʱ����ź�ǿ���й�ϵ��
               if you want to get the state of locking, the u32TimeOut should be set 100ms at least;
               if the u32TimeOut is 0, just config register without locking state, return HI_SUCCESS
               CNcomment: �����õ�����״̬����Ƶ��ʱ��С����Ϊ100ms��
               CNcomment: �����ʱʱ��Ϊ0��ֻ���üĴ��������ж�����״̬������HI_SUCCESS��
\param[out] pstTunerStatus: The current frequency locking status and parameters of the tuner are returned. Note: This parameter is valid only when HI_SUCCESS is returned.  CNcomment: ���ص�ǰTUNER����Ƶ״̬����Ƶ������ ע�⣺�˲���ֻ�ں������سɹ�������²������塣
\retval ::HI_SUCCESS Success                    CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.  CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetStatus(HI_U32    u32tunerId , HI_UNF_TUNER_STATUS_S  *pstTunerStatus);


/** 
\brief Obtains the current bit error rate (BER) of the tuner. The BER is expressed by using the scientific notation.
CNcomment:\brief ��ȡ��ǰTUNER �������ʣ��ÿ�ѧ��������ʾ��
\attention \n
N/A
\param[in] u32tunerId tuner port ID. The port ID can be 0-2.             CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu32BER  Pointer to the current BER of the tuner The error bit rate consists of three elements. Their definitions are as follows:
                      Their definitions are as follows:
                    pu32BER[0]: integral part of the base number of the BER
                    pu32BER[1]: decimal part of the base number of the BER x 1000
                    pu32BER[2]: absolute value of the exponential of the BER
                    For example, if the BER is 2.156E-7, the values of the three elements are
                    2, 156, and 7 respectively.
                    The three elements are valid only when HI_SUCCESS is returned.
                    Otherwise, the application layer sets a large value as required, for example, 0.5 (the values of the three elements are 5, 0, and 1 respectively).
                         0.5 (the values of the three elements are 5, 0, and 1 respectively).
CNcomment:\param[out] pu32BER  ָ��ǰTUNER �����ʵ�ָ�롣��ָ��ָ��һ����������Ԫ�ص����飬����Ԫ�غ�������:
CNcomment:                    pu32BER[0]:�����ʵ�������������
CNcomment:                    pu32BER[1]:�����ʵ�����С�����ֳ���1000
CNcomment:                    pu32BER[2]:������ָ������ȡ����ֵ
CNcomment:                    ����:������Ϊ2.156E-7����ô����Ԫ�ص�ȡֵ�ֱ�Ϊ
CNcomment:                    2��156��7
CNcomment:                    ������Ԫ��ֻ�ں������سɹ�������²������壬
CNcomment:                    ����Ӧ�ò�ɸ���ʵ�������������һ���ϴ�ֵ(����0.5��������Ԫ�طֱ�Ϊ5��0��1)��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE Calling this API fails.  CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetBER(HI_U32   u32tunerId , HI_U32 *pu32BER);


/** 
\brief Obtains the current signal-to-noise ratio (SNR) of the tuner.
CNcomment:\brief ��ȡ��ǰTUNER ������ȡ�
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu32SNR  Pointer to the current SNR of the tuner
                    The value of *pu32SNR ranges from 22 to 51.
                    This parameter is valid only when HI_SUCCESS is returned.
                   Otherwise, the application layer sets the parameter to a small value.
CNcomment:\param[out] pu32SNR  ָ��ǰTUNER����ȵ�ָ�롣
CNcomment:                    *pu32SNRȡֵ��ΧΪ22 ~ 51
CNcomment:                    �˲���ֻ�ں������سɹ�������²������壬
CNcomment:                    ����Ӧ�ò�ɽ���ֵ����Ϊ��Сֵ��
\retval ::HI_SUCCESS Success                   CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.  CNcomment:APIϵͳ����ʧ��

\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSNR(HI_U32   u32tunerId , HI_U32 *pu32SNR );             /* range : 0-255  */


/** 
\brief Obtains the current signal strength of the tuner.
CNcomment:\brief ��ȡ��ǰTUNER���ź�ǿ�ȡ�
\attention \n
N/A
\param[in] u32tunerId  tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu32SignalStrength  Pointer to the current signal strength of the tuner
                            For cable signal, the value of *pu32SignalStrength ranges from 20~120.
                            For satellite signal, the value is 15~130��unit in dBuv.
                  This parameter is valid only when HI_SUCCESS is returned.
                  Otherwise, the application layer sets the parameter to a small value.
CNcomment:\param[out] pu32SignalStrength  ָ��ǰTUNER�ź�ǿ�ȵ�ָ�롣
CNcomment:                  ����Cable�źţ�*pu32SignalStrength��ȡֵ��ΧΪ20~120
CNcomment:                  ����Satellite�źţ�*pu32SignalStrength��ȡֵ��ΧΪ15~130����λΪdBuv
CNcomment:                  �˲���ֻ�ں������سɹ�������²������壬
CNcomment:                  ����Ӧ�ò�ɽ���ֵ����Ϊ��Сֵ��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalStrength(HI_U32   u32tunerId , HI_U32 *pu32SignalStrength );

/**
\brief Obtains the current signal quality of the tuner, returns a percentage value. 
CNcomment:\brief ��ȡ��ǰTUNER���ź����������ذٷֱȡ�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu32SignalQuality   Output pointer.                       CNcomment:ָ���ź�������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalQuality(HI_U32 u32TunerId, HI_U32 *pu32SignalQuality);

/**
\brief Obtains the actual frequency and symbol rate of the current tuner for cable and satellite
signal, for terrestrial signal, symbol rate means nothing, ignore it. 
CNcomment:\brief ��ȡ��ǰTUNER��ʵ��Ƶ�ʺͷ����ʡ�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu32Freq   Pointer to the current frequency of the tuner. CNcomment:ָ��ǰTUNERƵ���ָ�롣
\param[out] pu32Symb   Points to the current symbol rate.             CNcomment:ָ��ǰ������               
\retval ::HI_SUCCESS Success                                          CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                         CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetRealFreqSymb( HI_U32 u32TunerId, HI_U32 *pu32Freq, HI_U32 *pu32Symb );

/**
\brief Obtains current signal information of the TUNER, used in satellite and terrestrial, not necessary for cable. 
CNcomment:\brief ��ȡ��ǰTUNER���ź���Ϣ��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pstSignalInfo Pointer to a signal info structure.         CNcomment:ָ���ź���Ϣ�ṹ���ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_GetSignalInfo(HI_U32 u32TunerId, HI_UNF_TUNER_SIGNALINFO_S *pstSignalInfo);

/**
\brief Sets the LNB parameter. 
CNcomment:\brief ����LNB������
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstLNB      Pointer to a LNB parameter structure.          CNcomment:ָ��LNB�����ṹ���ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetLNBConfig(HI_U32 u32TunerId, const HI_UNF_TUNER_FE_LNB_CONFIG_S *pstLNB);

/**
\brief Sets the LNB power. 
CNcomment:\brief ����LNB���硣
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enLNBPower  The enumeration of the LNB power type.         CNcomment:LNB���緽ʽö��ֵ��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SetLNBPower(HI_U32 u32TunerId, HI_UNF_TUNER_FE_LNB_POWER_E enLNBPower, HI_UNF_TUNER_FE_POLARIZATION_E enPolar);

/**
\brief Sets PLP ID, only used in DVB-T2. 
CNcomment:\brief ���������ܵ�ID��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] u8PLPID  The PLP ID.         CNcomment:�����ܵ�ID��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_SetPLPID(HI_U32 u32TunerId, HI_U8 u8PLPID);

/**
\brief Gets PLP number, only used in DVB-T2. 
CNcomment:\brief ��ȡ�����ܵ�������
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] pu8PLPNum  The PLP number.         CNcomment:�����ܵ�������
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_GetPLPNum(HI_U32 u32TunerId, HI_U8 *pu8PLPNum);


/**
\brief Gets current PLP type, only used in DVB-T2. 
CNcomment:\brief ��ȡ��ǰ�����ܵ����͡�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[out] penPLPType  The PLP type.         CNcomment:�����ܵ����͡�
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/

HI_S32 HI_UNF_TUNER_GetCurrentPLPType(HI_U32 u32TunerId, HI_UNF_TUNER_T2_PLP_TYPE_E *penPLPType);


/**
\brief Starts blind scan. 
CNcomment:\brief ��ʼäɨ��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to the blind scan parameter.       CNcomment:ָ��äɨ������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see Please refer to definition of HI_UNF_TUNER_BLINDSCAN_PARA_S.
N/A
*/
HI_S32 HI_UNF_TUNER_BlindScanStart(HI_U32 u32TunerId, const HI_UNF_TUNER_BLINDSCAN_PARA_S *pstPara);

/**
\brief Stops blind scan. 
CNcomment:\brief ֹͣäɨ��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_BlindScanStop(HI_U32 u32TunerId);

/**
\brief TUNER standby. 
CNcomment:\brief TUNER����
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Standby(HI_U32 u32TunerId);

/**
\brief Wakes up TUNER. 
CNcomment:\brief TUNER����
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_WakeUp( HI_U32 u32TunerId);

/**
\brief Disables TUNER, the tuner will be the high impedance state. You need call it when 2 or more demods share
the same TS interface, let indicated tuner release the TS interface so other ones can use it.
CNcomment:\brief TUNER��ʹ�ܣ�tuner��������迹״̬�����ڶ�·Demod��ͬһ��TS��ʱ������̬�Ա���TS��ͻ
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Disable(HI_U32 u32TunerId);

/**
\brief Enables TUNER, cancels disable state.
CNcomment:\brief TUNERʹ��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Enable(HI_U32 u32TunerId);

/**
\brief Sends and receives DiSEqC message, only the devices supporting DiSEqC 2.x support receive message. 
CNcomment:\brief ���ͽ���DiSEqC��Ϣ����֧��DiSEqC 2.x���豸֧�ֽ�����Ϣ��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstSendMsg  The pointer to a sending message structure.    CNcomment:ָ������Ϣ�ṹ���ָ�롣
\param[out] pstRecvMsg The pointer to a receiving message structure.If your device is DiSEqC 1.x, you can pass NULL here.
CNcomment:\param[out] pstRecvMsg ָ�������Ϣ�ṹ���ָ�롣�����DiSEqC 1.x�豸��������Դ�NULL��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
/*HI_S32 HI_UNF_TUNER_DISEQC_SendRecvMessage(HI_U32 u32TunerId,
                                           HI_UNF_TUNER_DISEQC_SENDMSG_S* pstSendMsg,
                                           HI_UNF_TUNER_DISEQC_RECVMSG_S* pstRecvMsg);*/

/**
\brief Sets 0/12V switch. Don't support now. 
CNcomment:\brief ����0/12V����״̬���ݲ�֧�֡�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enPort      The enumeration of the switch port.            CNcomment:����ö��ֵ��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Switch012V(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_0_12V_E enPort);

/**
\brief Sets 22KHz switch. 
CNcomment:\brief ����22KHz����״̬��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enPort      The enumeration of the switch port.            CNcomment:����ö��ֵ��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_Switch22K(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_22K_E enPort);

/**
\brief Sets tone burst switch. 
CNcomment:\brief ����Tone burst����״̬��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enStatus    The enumeration of the switch port.            CNcomment:����ö��ֵ��
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_SwitchToneBurst(HI_U32 u32TunerId, HI_UNF_TUNER_SWITCH_TONEBURST_E enStatus);

HI_S32 HI_UNF_TUNER_SetAntennaPower(HI_U32 u32TunerId, HI_BOOL bPower);
#ifdef DISEQC_SUPPORT
/**
\brief Sets DiSEqC 1.0/2.0 switch, at most 4 port.
CNcomment:\brief ����DiSEqC 1.0/2.0���أ�����4�ڡ�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a switch parameter structure.   CNcomment:ָ�򿪹ز�����ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see Please refer to definition of HI_UNF_TUNER_DISEQC_SWITCH4PORT_S.
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Switch4Port(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_SWITCH4PORT_S* pstPara);

/**
\brief Sets DiSEqC 1.1/2.1 switch, supports 8in1, 16in1 switches. 
CNcomment:\brief ����DiSEqC 1.1/2.1���أ�֧��8�ڣ�16�ڿ��ء�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a switch parameter structure.   CNcomment:ָ�򿪹ز�����ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Switch16Port(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_SWITCH16PORT_S* pstPara);

/**
\brief Lets the DiSEqC motor store current position. 
CNcomment:\brief DiSEqC���洢��ǰλ�á�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a position parameter structure. CNcomment:ָ��λ�ò�����ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_StorePos(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);

/**
\brief Lets the DiSEqC motor move to stored position. 
CNcomment:\brief DiSEqC���ת�����洢λ�á�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a position parameter structure. CNcomment:ָ��λ�ò�����ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_GotoPos(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_POSITION_S *pstPara);

/**
\brief Enables or disables the DiSEqC motor's limit setting. 
CNcomment:\brief �򿪡��ر�DiSEqC���Ȩ�����á�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a limit parameter structure.    CNcomment:ָ��limit������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_SetLimit(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LIMIT_S* pstPara);

/**
\brief Drives DiSEqC motor. 
CNcomment:\brief ����DiSEqC����ƶ���
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a movement parameter structure. CNcomment:ָ���ƶ�������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Move(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_MOVE_S* pstPara);

/**
\brief Halts DiSEqC motor. 
CNcomment:\brief ֹͣDiSEqC����ƶ���
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enLevel     The command level of the DiSEqC motor.         CNcomment:���֧�ֵ�����ȼ���
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Stop(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief Lets the DiSEqC motor recalculate its stored positions. 
CNcomment:\brief ���¼������洢λ�á�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a recaulculate parameter structure. CNcomment:ָ���ؼ��������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Recalculate(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_RECALCULATE_S* pstPara);

/**
\brief Calculates the angular, basing on site-longitude, site-latitude and satellite-longitude. 
CNcomment:\brief USALS���ݵ��ؾ�γ�ȡ����Ǿ��ȼ������ǽǶȡ�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in/out] pstPara The pointer to a USALS parameter structure.    CNcomment:ָ��USALS���������ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_CalcAngular(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_USALS_PARA_S* pstPara);

/**
\brief Gotos the indicated angular. 
CNcomment:\brief USALS�豸ת��ĳ�Ƕȡ�
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] pstPara     The pointer to a USALS angular structure.      CNcomment:ָ��USALS�ǶȲ�����ָ�롣
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_GotoAngular(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_USALS_ANGULAR_S* pstPara);

/**
\brief Resets DiSEqC device. 
CNcomment:\brief ��λDiSEqC�豸��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:�豸֧�ֵ�����ȼ���
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Reset(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief DiSEqC device standby. 
CNcomment:\brief DiSEqC�豸������
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:�豸֧�ֵ�����ȼ���
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_Standby(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);

/**
\brief Wakes up DiSEqC device. 
CNcomment:\brief ����DiSEqC�豸��
\attention \n
N/A
\param[in] u32TunerId  Tuner port ID. The port ID can be 0-2.         CNcomment:TUNER�˿ںţ�ȡֵΪ0-2
\param[in] enLevel     The command level of the DiSEqC device.        CNcomment:�豸֧�ֵ�����ȼ���
\retval ::HI_SUCCESS   Success                                        CNcomment:�ɹ�
\retval ::HI_FAILURE   Calling this API fails.                        CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_TUNER_DISEQC_WakeUp(HI_U32 u32TunerId, HI_UNF_TUNER_DISEQC_LEVEL_E enLevel);
#endif /* DISEQC_SUPPORT */

/**
\brief write HDCP KEY
CNcomment:\brief д��HDCP KEY 
\attention \n
config just one time and can not be rework
HI_UNF_HDCP_SetHDCPKey is a new function
HI_UNF_ADVCA_SetHDCPKey is the old function used in font version
CNcomment: ��������һ�� ���ɸ���
CNcomment: HI_UNF_HDCP_SetHDCPKey Ϊ�µĽӿڣ�
CNcomment: HI_UNF_ADVCA_SetHDCPKeyΪ��ǰ�汾���ݽӿ�

\param[in] stHdcpKey    HDCP key parameter                   CNcomment:HDCP key����\n
\retval ::HI_SUCCESS    success                              CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.                CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_CA_NOT_INIT  CA has not been initialized    CNcomment:CAδ��ʼ��
\retval ::HI_ERR_CA_SETPARAM_AGAIN  repeat setting parameter CNcomment:�ظ����ò���
\see \n
::HI_UNF_OTP_HDCPKEY_S
*/
HI_S32 HI_UNF_HDCP_SetHDCPKey(HI_UNF_OTP_HDCPKEY_S stHdcpKey);
HI_S32 HI_UNF_ADVCA_SetHDCPKey(HI_UNF_ADVCA_HDCPKEY_S stHdcpKey);
HI_S32 HI_UNF_HDCP_GetKeyBurnFlag(HI_BOOL *pbKeyBurnFlag);
/** 
\brief Starts an OTP device.
CNcomment:\brief ��OTP�豸
\attention \n
N/A
\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_Open(HI_VOID);

/** 
\brief Stops an OTP device.
CNcomment:\brief �ر�OTP�豸
\attention \n
N/A
\param N/A                                    CNcomment:��
\retval ::HI_SUCCESS Success                  CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_Close(HI_VOID);

/** 
\brief      Set customer key, the customer key is used by the customer to encrypt some private data
\attention \n
N/A
\param[in] pKey              customer key to be written to OTP    
\param[in] u32KeyLen         the length of customer key, must be 16bytes          
\retval ::HI_SUCCESS           success                            
\retval ::HI_FAILURE            fail                              
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_SetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen);

/** 
\brief      Get customer key, the customer key is used by the customer to encrypt some private data
\attention \n
N/A
\param[in] pKey              buffer to store the customer key read from OTP
\param[in] u32KeyLen         the length of buffer, must be 16bytes 
\retval ::HI_SUCCESS           success
\retval ::HI_FAILURE            fail
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetCustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen);

/** 
\brief      Set stb private data, the stb private data is used by the customer to set some private data
\attention \n
N/A
\param[in] u32Offset      the offset to set the private data, should be between 0 and 15    
\param[in] u8Data          the private data          
\retval ::HI_SUCCESS    success                            
\retval ::HI_FAILURE      fail                              
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data);

/** 
\brief      Get stb private data, the stb private data is used by the customer to set some private data
\attention \n
N/A
\param[in] u32Offset            the offset to set the private data, should be between 0 and 15
\param[out] pu8Data            the data read from OTP  
\retval ::HI_SUCCESS         success
\retval ::HI_FAILURE            fail
\see \n
N/A
*/
HI_S32 HI_UNF_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data);


/** @} */  /** <!-- ==== API declaration end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_4_12 */
/** @{ */  /** <!-- [CI]*/

/** 
\brief CI init
CNcomment:\brief CI��ʼ�� 
\attention \n
N/A
\param    N/A                                         CNcomment:��
\retval ::HI_SUCCESS              success             CNcomment: �ɹ�
\retval ::HI_FAILURE              fail                CNcomment:ʧ��
\retval ::HI_ERR_CI_OPEN_ERR      opening file fail   CNcomment:���ļ�ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Init(HI_VOID);

/** 
\brief CI deinit
CNcomment:\brief CIȥ��ʼ�� 
\attention \n
N/A
\param  N/A                                              CNcomment:��
\retval ::HI_SUCCESS             success                 CNcomment:�ɹ�
\retval ::HI_FAILURE             fail                    CNcomment:ʧ��
\retval ::HI_ERR_CI_CLOSE_ERR    closing file fail       CNcomment:�ر��ļ�ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CI_DeInit(HI_VOID);

/** 
\brief Sets CI device configration.
CNcomment:\brief CI�豸�������� 
\attention \n
N/A
\param[in] enCIPort  CI Port number.                     CNcomment:������CI port
\param[in] pstCIAttr The pointer to a structure of CI device. 
                                                         CNcomment:ָ��CI�豸���Ե�ָ�롣
\retval ::HI_SUCCESS            success                  CNcomment:�ɹ�
\retval ::HI_FAILURE            fail                     CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CI_SetAttr(HI_UNF_CI_PORT_E enCIPort, const HI_UNF_CI_ATTR_S *pstCIAttr);

/** 
\brief Gets CI device configration.
CNcomment:\brief CI�豸���Ի�ȡ 
\attention \n
N/A
\param[in] enCIPort  CI Port number.                     CNcomment:������CI port
\param[in] pstCIAttr The pointer to a structure of CI device. 
                                                         CNcomment:ָ��CI�豸���Ե�ָ�롣
\retval ::HI_SUCCESS            success                  CNcomment:�ɹ�
\retval ::HI_FAILURE            fail                     CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CI_GetAttr(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_ATTR_S *pstCIAttr);

/** 
\brief open CI Port
CNcomment:\brief ��CI Port
\attention \n
N/A
\param[in] enCIPort     CI Port number                    CNcomment:������CI port
\retval ::HI_SUCCESS    success                           CNcomment:�ɹ�
\retval ::HI_FAILURE     fail                             CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter     CNcomment:��Ч����
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port   CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error   CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error  CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Open(HI_UNF_CI_PORT_E enCIPort);

/** 
\brief close CI Port
CNcomment:\brief �ر�CI Port
\attention \n
N/A
\param[in] enCIPort    CI Port number   CNcomment:������CI port
\retval ::HI_SUCCESS       success      CNcomment:�ɹ�
\retval ::HI_FAILURE       fail         CNcomment:ʧ��
\see \n
N/A
*/
HI_S32 HI_UNF_CI_Close(HI_UNF_CI_PORT_E enCIPort);

/** 
\brief open CAM
CNcomment:\brief ��CAM��
\attention \n
N/A
\param[in] enCIPort    CI Port number                                       CNcomment:������CI port
\param[in] enCardId     Card ID                                             CNcomment:������
\retval ::HI_SUCCESS    success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE     fail                                               CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA   invalid parameter                        CNcomment: ��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open   CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                     CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                     CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                    CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Open(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/**
 \brief close CAM
 CNcomment:\brief �ر�CAM��

 \attention \n
N/A
 \param[in] enCIPort   CI Port number   CNcomment:������CI port
 \param[in] enCardId   Card Id          CNcomment:������
 \retval ::HI_SUCCESS   success         CNcomment:�ɹ�
 \retval ::HI_FAILURE    fail           CNcomment:ʧ��
 \see \n
N/A
 */
HI_S32 HI_UNF_CI_PCCD_Close(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/**
 \brief CAM power switch
 CNcomment:\brief CAM����Դ����
 \attention \n
	Some solutions don't support power down for a single card, the calling of this interface
	automatically detects a power outage card plug, if it is on ,it will keep power on
    CNcomment:�еķ�����֧����Ե����ϵ磬���ñ��ӿڶϵ�ʱ���Զ�����Ƿ��п����ϣ�
    CNcomment:����У�������ϵ�
 \param[in] enCIPort    CI Port number                                       CNcomment:������CI port
 \param[in] enCardId    Card Id                                              CNcomment:������
 \param[in] enCtrlPower power on\power off                                   CNcomment:���硢�ص�
 \retval ::HI_SUCCESS      success                                           CNcomment:�ɹ�
 \retval ::HI_FAILURE      fail                                              CNcomment:ʧ��
 \retval ::HI_ERR_CI_INVALID_PARA     invalid parameter                      CNcomment:��Ч����
 \retval ::HI_ERR_CI_NOT_INIT         have not been initialized or not open  CNcomment:δ��ʼ����δ��
 \retval ::HI_ERR_CI_UNSUPPORT        unsupported CI Port                    CNcomment:��֧�ֵ�CI Port
 \retval ::HI_ERR_CI_REG_READ_ERR     read register error                    CNcomment:���Ĵ�������
 \retval ::HI_ERR_CI_REG_WRITE_ERR    write register error                   CNcomment:д�Ĵ�������
 \retval ::HI_ERR_CI_CANNOT_POWEROFF  can not power off                      CNcomment:���ܶϵ�
 \see \n
N/A
 */
HI_S32 HI_UNF_CI_PCCD_CtrlPower(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_CTRLPOWER_E enCtrlPower);

/** 
\brief CI reset
CNcomment:\brief CAM����λ
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:������CI port
\param[in] enCardId    Card Id                                                CNcomment:������
\retval ::HI_SUCCESS   success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE   fail                                                   CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                      CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open  CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                    CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                    CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                   CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Reset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM ready state checking
CNcomment:\brief CAM������״̬���
\attention \n
N/A
\param[in] enCIPort       CI Port number                                     CNcomment:������CI port
\param[in] enCardId       Card Id                                            CNcomment:������
\param[out] penCardReady  state value                                        CNcomment:״ֵ̬
\retval ::HI_SUCCESS      success                                            CNcomment:�ɹ�
\retval ::HI_FAILURE      fail                                               CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA   invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT       have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT      unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR   read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR  write register error                      CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_IsReady(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_READY_E_PTR penCardReady);

/** 
\brief check CAM if it is on  
CNcomment:\brief CAM���Ƿ���ϼ��
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:������CI port
\param[in] enCardId    Card Id                                                CNcomment:������
\param[out] penCardStatus  card up or card down                               CNcomment:�����ϻ�δ��״̬
\retval ::HI_SUCCESS        success                                           CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                              CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_Detect(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_UNF_CI_PCCD_STATUS_E_PTR penCardStatus);

/** 
\brief CAM set access module
CNcomment:\brief CAM�����÷���ģ��
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:������CI port
\param[in] enCardId    Card Id                                                CNcomment:������
\param[in] enAccessMode mode you want to set                                  CNcomment:Ҫ���õ�ģʽ
\retval ::HI_SUCCESS        success                                           CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                              CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_SetAccessMode(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_UNF_CI_PCCD_ACCESSMODE_E enAccessMode);

/** 
\brief CAM write or read state checking
CNcomment:\brief CAM����д״̬���
\attention \n
N/A
\param[in] enCIPort    CI Port number                                         CNcomment:������CI port
\param[in] enCardId    Card Id                                                CNcomment:������
\param[in] enStatus    status of the bit will check                           CNcomment:Ҫ����״̬λ
\param[out] pu8Value   status code returned                                   CNcomment:���ص�״̬��
\retval ::HI_SUCCESS        success                                           CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                              CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_IO_READ_ERR       read IO error                           CNcomment:IO������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_GetStatus (HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                HI_UNF_CI_PCCD_STATUS_BIT_E enStatus, HI_U8 *pu8Value);

/** 
\brief CAM IO read data
CNcomment:\brief CAM��IO������
\attention \n
N/A
\param[in] enCIPort       CI Port number                                      CNcomment:������CI port
\param[in] enCardId       Card Id                                             CNcomment:������
\param[out] pu8Buffer     data buffer                                         CNcomment: ���ݻ�����
\param[out] pu32ReadLen   data length that read successful                    CNcomment:�ɹ���ȡ�ĳ���
\retval ::HI_SUCCESS      success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                              CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:д�Ĵ�������
\retval ::HI_ERR_CI_IO_READ_ERR     read IO error                             CNcomment:IO������
\see \n
    HI_UNF_CI_PCCD_IOWrite()
*/
HI_S32 HI_UNF_CI_PCCD_IORead(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 *pu32ReadLen);

/** 
\brief CAM IO write data
CNcomment:\brief CAM��IOд����
\attention \n
N/A
\param[in] enCIPort       CI Port number                                      CNcomment:������CI port
\param[in] enCardId       Card Id                                             CNcomment:������
\param[in] pu8Buffer      data buffer                                         CNcomment:���ݻ�����
\param[in] u32WriteLen     byte number that have been  writen                 CNcomment:д���ֽ���
\param[out] pu32WriteOKLen  data length that write successful                 CNcomment:�ɹ�д���ֽ���
\param[in] enCIPort       operate CI port                                     CNcomment:������CI port
\param[in] enCardId       Card Id                                             CNcomment:������
\param[out] pu8Buffer     data buffer                                         CNcomment: ���ݻ�����
\param[out] pu32ReadLen   data length that read successful                    CNcomment:�ɹ���ȡ�ĳ���
\retval ::HI_SUCCESS      success                                             CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                              CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA    invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT        have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT       unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                      CNcomment:д�Ĵ�������
\retval ::HI_ERR_CI_IO_WRITE_ERR    write IO error                            CNcomment:IOд����
\see \n
    HI_UNF_CI_PCCD_IORead()
*/
HI_S32 HI_UNF_CI_PCCD_IOWrite(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                            HI_U8 *pu8Buffer, HI_U32 u32WriteLen, HI_U32 *pu32WriteOKLen);

/** 
\brief check CAM CIS information
CNcomment:\brief ����CAM��CIS��Ϣ
\attention \n
    reference EN50221.
    CNcomment:��ο��淶 EN50221.
\param[in] enCIPort       CI Port number                                         CNcomment:������CI port
\param[in] enCardId       Card Id                                                CNcomment:������
\retval ::HI_SUCCESS      success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                                 CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:�豸æ
\retval ::HI_ERR_CI_PCCD_CIS_READ      read CIS information fail                 CNcomment:��CIS��Ϣʧ��
\retval ::HI_ERR_CI_REG_READ_ERR    read register error                          CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR   write register error                         CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_CheckCIS(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief config CAM COR information
CNcomment:\brief ����CAM��COR��Ϣ
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:������CI port
\param[in] enCardId       Card Id                                                CNcomment:������
\retval ::HI_SUCCESS      success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                                 CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_PCCD_DEVICE_BUSY   device busy                               CNcomment:�豸æ
\retval ::HI_ERR_CI_ATTR_WRITE_ERR     config COR fail                           CNcomment:����CORʧ��
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:д�Ĵ�������
\see \n
    N/A
*/
HI_S32 HI_UNF_CI_PCCD_WriteCOR(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM IO reset
CNcomment:\brief CAM��IO�ڸ�λ
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:������CI port
\param[in] enCardId       Card Id                                                CNcomment:������
\retval ::HI_SUCCESS      success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                                 CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          not supported CI Port                     CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_PCCD_TIMEOUT       timeout                                   CNcomment:��ʱ
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:д�Ĵ�������
\see \n
    HI_UNF_CI_PCCD_CheckCIS()
*/
HI_S32 HI_UNF_CI_PCCD_IOReset(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId);

/** 
\brief CAM negotiate buffer size
CNcomment:\brief CAM��Э��buffer size
\attention \n
    N/A
\param[in] enCIPort      CI Port number                                          CNcomment:������CI port
\param[in] enCardId       Card Id                                                CNcomment:������
\param[in] pu16BufferSize   input  buffer size that master support               CNcomment:����������֧��buffer size
\param[out] pu16BufferSize  output buffer size after consultation                CNcomment:���Э�̺��buffer size
\retval ::HI_SUCCESS      success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                                 CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_PCCD_TIMEOUT       timeout                                   CNcomment:��ʱ
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:д�Ĵ�������
\retval ::HI_ERR_CI_IO_READ_ERR        read IO error                             CNcomment:IO������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_NegBufferSize(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                                    HI_U16 *pu16BufferSize);

/** 
\brief CAM TS control
CNcomment: \brief CAM��TS����
\attention \n
    N/A
\param[in] enCIPort       CI Port number                                         CNcomment:������CI port
\param[in] enCardId       Card Id                                                CNcomment:������
\param[in] enCMD          control command                                        CNcomment:��������
\param[in] pParam         parameter that matching command                        CNcomment:������ƥ��Ĳ���
\retval ::HI_SUCCESS      success                                                CNcomment:�ɹ�
\retval ::HI_FAILURE        fail                                                 CNcomment:ʧ��
\retval ::HI_ERR_CI_INVALID_PARA       invalid parameter                         CNcomment:��Ч����
\retval ::HI_ERR_CI_NOT_INIT           have not been initialized or not open     CNcomment:δ��ʼ����δ��
\retval ::HI_ERR_CI_UNSUPPORT          unsupported CI Port                       CNcomment:��֧�ֵ�CI Port
\retval ::HI_ERR_CI_REG_READ_ERR       read register error                       CNcomment:���Ĵ�������
\retval ::HI_ERR_CI_REG_WRITE_ERR      write register error                      CNcomment:д�Ĵ�������
\see \n
N/A
*/
HI_S32 HI_UNF_CI_PCCD_TSCtrl(HI_UNF_CI_PORT_E enCIPort, HI_UNF_CI_PCCD_E enCardId, 
                                        HI_UNF_CI_PCCD_TSCTRL_E enCMD, HI_UNF_CI_PCCD_TSCTRL_PARAM_U *pParam);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_ECS_TYPE_H__ */

