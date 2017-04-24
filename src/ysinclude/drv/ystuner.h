#ifndef _YSTUNER_H_
#define _YSTUNER_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include "drv_typedef.h"
#include "stddefs.h"

/* ����Locked��ʾTuner������״̬������NewCarrier��ʾ�Ƿ����µ���Ƶ���� */
typedef void (*DRV_TunerCallback)(BOOL_YS Locked);

typedef enum YSTUNER_TunerType_e 
{
	YSTUNER_TUNER_TDCCG051F,
	YSTUNER_TUNER_TDCCG151F,
	YSTUNER_TUNER_NUM_MAX		
}YSTUNER_TunerType_t;

/*DEMODE��TUNER��LNB����*/
typedef enum YSTUNER_LnbType_e
{
    YSTUNER_LNB_NONE ,    /* null LNB driver */
    YSTUNER_LNB_STV0299,  /* built in control of the LNB  */
    YSTUNER_LNB_STV0399,
    YSTUNER_LNB_STEM,     /* I2C switch does the LNB work */
    YSTUNER_LNB_LNB21,    /* I2C controlled LNBP21 -> used for tone & DiSEqC*/
    YSTUNER_LNB_LNBH21    /* I2C controlled LNBPH21 -> used for tone & DiSEqC*/
} YSTUNER_LnbType_t;


/* �������õĵ�г��������Ĵ��� */

/*��������*/

typedef enum YSTUNER_SearchType_e  
{
    YSTUNER_NORMAL_SEARCH    = 0, /*������ͨɨ��*/
    YSTUNER_BLIND_SEARCH     = 1, /*����äɨ,ɨ��������ܱ�����Ϊ�Զ�ֵ*/
    YSTUNER_HARD_SEARCH      = 2  /*ɨ��������ܱ�����Ϊ�Զ�ֵ��ƫ��ֵ*/
} YSTUNER_SearchType_t;

#if defined(cab5197)
typedef enum YSTUNER_Modulation_e /*���Ʒ�ʽ*/
{

    YSTUNER_MOD_ALL    = 0x3FF, /* Logical OR of all MODs */
    YSTUNER_MOD_QPSK   = 1,
    YSTUNER_MOD_4QAM   = (1 << 3),
    YSTUNER_MOD_16QAM  = (1 << 4),
    YSTUNER_MOD_64QAM  = (1 << 6)
} YSTUNER_Modulation_t;

#else//sat5189
typedef enum YSTUNER_Modulation_e /*���Ʒ�ʽ*/
{
    YSTUNER_MOD_NONE   = 0x00,  /* Modulation unknown */
    YSTUNER_MOD_ALL    = 0x3FF, /* Logical OR of all MODs */
    YSTUNER_MOD_QPSK   = 1,
    YSTUNER_MOD_8PSK   = (1 << 1),
    YSTUNER_MOD_QAM    = (1 << 2),
    YSTUNER_MOD_4QAM   = (1 << 3),
    YSTUNER_MOD_16QAM  = (1 << 4),
    YSTUNER_MOD_32QAM  = (1 << 5),
    YSTUNER_MOD_64QAM  = (1 << 6),
    YSTUNER_MOD_128QAM = (1 << 7),
    YSTUNER_MOD_256QAM = (1 << 8),
    YSTUNER_MOD_BPSK   = (1 << 9),
    YSTUNER_MOD_16APSK,
    YSTUNER_MOD_32APSK,
    YSTUNER_MOD_8VSB  =  (1 << 10)
} YSTUNER_Modulation_t;
#endif

typedef enum YSTUNER_Spectrum_e  /*Ƶ�׷�ת */
{
    YSTUNER_INVERSION_NONE = 0,
    YSTUNER_INVERSION      = 1,
    YSTUNER_INVERSION_AUTO = 2,
    YSTUNER_INVERSION_UNK  = 4
} YSTUNER_Spectrum_t;

typedef enum YSTUNER_FECRate_e /*ǰ��������� (sat & ter)*/
{
    YSTUNER_FEC_NONE = 0x00,    /* no FEC rate specified */
    YSTUNER_FEC_ALL = 0xFFFF,     /* Logical OR of all FECs */
    YSTUNER_FEC_1_2 = 1,
    YSTUNER_FEC_2_3 = (1 << 1),
    YSTUNER_FEC_3_4 = (1 << 2),
    YSTUNER_FEC_4_5 = (1 << 3),
    YSTUNER_FEC_5_6 = (1 << 4),
    YSTUNER_FEC_6_7 = (1 << 5),
    YSTUNER_FEC_7_8 = (1 << 6),
    YSTUNER_FEC_8_9 = (1 << 7),
    YSTUNER_FEC_1_4 = (1 << 8),
    YSTUNER_FEC_1_3 = (1 << 9),
    YSTUNER_FEC_2_5 = (1 << 10),
    YSTUNER_FEC_3_5 = (1 << 11),
    YSTUNER_FEC_9_10= (1 << 12)
} YSTUNER_FECRate_t;


/*���ǹ㲥����*/
typedef enum YSTUNER_Polarization_e /* ������ʽ (sat),ʹ�õ���λ,��λ���ڹ��翪�� */
{
    YSTUNER_PLR_ALL        = 0x07, /* Logical OR of all PLRs */
    YSTUNER_PLR_HORIZONTAL = 1,
    YSTUNER_PLR_VERTICAL   = (1 << 1),
    YSTUNER_LNB_OFF        = (1 << 2)  /* Add for LNB off-> in case tuner is fed from modulator */
} YSTUNER_Polarization_t;

/*���ǹ㲥����*/
typedef enum YSTUNER_LNB_0v_12v_e /*0/12v����ʱ��ʹ��*/
{
    YSTUNER_LNB_0V_12V_OFF = 0,
    YSTUNER_LNB_0V = 1,
    YSTUNER_LNB_12V = 2
} YSTUNER_LNB_0v_12v_t;

typedef enum YSTUNER_LNB_DisEqc_e /*DisEqc����ʱ��ʹ��*/
{
    YSTUNER_LNB_DISEQC_OFF = 0,
    YSTUNER_LNB_DISEQC_1,
    YSTUNER_LNB_DISEQC_2,
    YSTUNER_LNB_DISEQC_3,
    YSTUNER_LNB_DISEQC_4,
    YSTUNER_LNB_DISEQC_5,
    YSTUNER_LNB_DISEQC_6,
    YSTUNER_LNB_DISEQC_7,
    YSTUNER_LNB_DISEQC_8
} YSTUNER_LNB_DisEqc_t;


typedef enum YSTUNER_Polar_Control_e /*0/12v����ʱ��ʹ��*/
{
    YSTUNER_POLAR_CONTROL_AUTO = 0,
    YSTUNER_POLAR_CONTROL_13V = 1,
    YSTUNER_POLAR_CONTROL_18V = 2
} YSTUNER_LNB_Polar_Control_t;

typedef enum YSTUNER_LNB_Power_e /*���翪�� (sat),ʹ�ø���λ,��λ���ڼ���ѡ�� */
{
    YSTUNER_LNB_POWER_OFF = 0,
    YSTUNER_LNB_POWER_ON  = (1<<4),
    YSTUNER_LNB_POWER_ALL = 0Xf0
} YSTUNER_LNB_Power_t;

typedef enum YSTUNER_LNBToneState_e /* LNB tone state (sat) */
{
    YSTUNER_LNB_TONE_DEFAULT,   /* Default (current) LNB state */
    YSTUNER_LNB_TONE_OFF,       /* LNB disabled */
    YSTUNER_LNB_TONE_22KHZ      /* LNB set to 22kHz */
} YSTUNER_LNBToneState_t;


typedef enum YSTUNER_DiSEqCCommand_e /*DiSEqC�������� (Sat)*/
{
    YSTUNER_DiSEqC_TONE_BURST_OFF,					/* Generic */
	YSTUNER_DiSEqC_TONE_BURST_OFF_F22_HIGH,			/* f22 pin high after tone off */
	YSTUNER_DiSEqC_TONE_BURST_OFF_F22_LOW,  		/* f22 pin low after tone off*/
	YSTUNER_DiSEqC_CONTINUOUS_TONE_BURST_ON,  		/* unmodulated */
	YSTUNER_DiSEqC_TONE_BURST_SEND_0_UNMODULATED,   /* send of 0 for 12.5 ms ;continuous tone*/
	YSTUNER_DiSEqC_TONE_BURST_SEND_0_MODULATED,     /* 0-2/3 duty cycle tone*/
	YSTUNER_DiSEqC_TONE_BURST_SEND_1, 				/* 1-1/3 duty cycle tone*/
	YSTUNER_DiSEqC_COMMAND							/* DiSEqC (1.2/2)command   */
} YSTUNER_DiSEqCCommand_t;

typedef enum YSTUNER_ModeCode_e /*DVBS2*/
{
    YSTUNER_MODECODE_DUMMY_PLF,
	YSTUNER_MODECODE_QPSK_14,
	YSTUNER_MODECODE_QPSK_13,
	YSTUNER_MODECODE_QPSK_25,
	YSTUNER_MODECODE_QPSK_12,
	YSTUNER_MODECODE_QPSK_35,
	YSTUNER_MODECODE_QPSK_23,
	YSTUNER_MODECODE_QPSK_34,
	YSTUNER_MODECODE_QPSK_45,
	YSTUNER_MODECODE_QPSK_56,
	YSTUNER_MODECODE_QPSK_89,
    YSTUNER_MODECODE_QPSK_910,
	YSTUNER_MODECODE_8PSK_35,
	YSTUNER_MODECODE_8PSK_23,
	YSTUNER_MODECODE_8PSK_34,
	YSTUNER_MODECODE_8PSK_56,
	YSTUNER_MODECODE_8PSK_89,
	YSTUNER_MODECODE_8PSK_910,
	YSTUNER_MODECODE_16APSK_23,
	YSTUNER_MODECODE_16APSK_34,
	YSTUNER_MODECODE_16APSK_45,
	YSTUNER_MODECODE_16APSK_56,
	YSTUNER_MODECODE_16APSK_89,
	YSTUNER_MODECODE_16APSK_910,
	YSTUNER_MODECODE_32APSK_34,
	YSTUNER_MODECODE_32APSK_45,
	YSTUNER_MODECODE_32APSK_56,
	YSTUNER_MODECODE_32APSK_89,
	YSTUNER_MODECODE_32APSK_910,
	YSTUNER_MODECODE_ALL

} YSTUNER_ModeCode_t;

/*����㲥����*/
typedef enum YSTUNER_FFTMode_e /*OFDM FFT ���� (TER)*/
{
    YSTUNER_FFTMODE_2K,
    YSTUNER_FFTMODE_8K,
    YSTUNER_FFTMODE_AUTO
} YSTUNER_FFTMode_t;
#if 0
typedef enum YSTUNER_ChannelBW_e /* (ter) */
{
	YSTUNER_CHAN_BW_NONE =0x00,
    YSTUNER_CHAN_BW_6M  = 6,
    YSTUNER_CHAN_BW_7M  = 7,
    YSTUNER_CHAN_BW_8M  = 8
} YSTUNER_ChannelBW_t;
#else
/*ChannelSpacing of DVBT*/
typedef enum YSTUNER_ChSpacing_e /* (ter) */
{
	YSTUNER_ChSpacing_NONE =0x00,
    YSTUNER_ChSpacing_6M  = 6,
    YSTUNER_ChSpacing_7M  = 7,
    YSTUNER_ChSpacing_8M  = 8
} YSTUNER_ChSpacing_t;
#endif
typedef enum YSTUNER_Guard_e /*OFDM ���� (TER)*/
{
    YSTUNER_GUARD_1_32,               /* Guard interval = 1/32 */
    YSTUNER_GUARD_1_16,               /* Guard interval = 1/16 */
    YSTUNER_GUARD_1_8,                /* Guard interval = 1/8  */
    YSTUNER_GUARD_1_4,                 /* Guard interval = 1/4  */
    YSTUNER_GUARD_ALL  =0xFF                /* Guard interval = 1/4  */

} YSTUNER_Guard_t;

typedef enum YSTUNER_Hierarchy_e /*OFDM �ȼ� (TER)*/
{
    YSTUNER_HIER_NONE,              /* Regular modulation */
    YSTUNER_HIER_1,                 /* Hierarchical modulation a = 1 */
    YSTUNER_HIER_2,                 /* Hierarchical modulation a = 2 */
    YSTUNER_HIER_4,                  /* Hierarchical modulation a = 4 */
    YSTUNER_HIER_ALL                  /* Hierarchical modulation a = 4 */
} YSTUNER_Hierarchy_t;

/*Ƶ����������*/
typedef struct YSTUNER_SearchPara_S
{
    YSTUNER_SearchType_t    DVB_SearchType; //����
    YSTUNER_Spectrum_t      DVB_SpectrumIs; //��ѡ�� ALL
    /*Ƶ��ֵ����λKHz����Ƶ��������Ƶ���Ƶ����������ʼƵ��*/
    U32_YS                  DVB_Frequency; //����, DVBT&DVBC KHz,DVBS MHz ,��Ƶ��������Ƶ��ֵ
    YSTUNER_Modulation_t    DVB_Modulation; //����
    U32_YS                  DVB_SymbolRate; //����, Sym/s   
    /*Ƶ��������Ƶ�ʷ�Χ����Ƶ������������Ϊ��*/
    /*�����ֵ��Ϊ�㽫��Ƶ���������ܣ�������
    DVB_Frequency ---- DVB_Frequency+DVB_ScanBandWidth
    ��Χ�ڵ���������(MHz)Ƶ��*/
    U32_YS                  DVB_ScanBandWidth; //���룬DVBT&DVBC KHz,DVBS MHz 
    U32_YS                  DVB_ScanTimeOut; //ms
    U32_YS                  DVB_TunerReserved; //Reserved

    YSTUNER_FECRate_t       DVBT_FECRates;
    YSTUNER_FFTMode_t       DVBT_FFTMode;    //��ѡ
    YSTUNER_Guard_t         DVBT_GuardMode;
    #if 0
    YSTUNER_ChannelBW_t     DVBT_BandWidth;  //��ѡ
    #else
    YSTUNER_ChSpacing_t     DVBT_ChSpacing;  //��ѡ
    #endif

    YSTUNER_FECRate_t       DVBS_FECRates;
    U32_YS                  DVBS_LNB_LO1;  //MHz ����
    U32_YS                  DVBS_LNB_LO2;  //MHz ����
    YSTUNER_Polarization_t  DVBS_Polar;   //����
    YSTUNER_LNB_Power_t     DVBS_LNBPower;  //���翪��
    YSTUNER_LNBToneState_t  DVBS_LNBTone;  //����
    YSTUNER_ModeCode_t      DVBS2_ModeCode; //DVBS2 Only

} YSTUNER_SearchPara_t;

extern BOOL g_TunerLock;

/*****************************************************************************
* Function    : DRV_TunerInit
* Description :
*   ��ʼ��Tunerģ��.
* Note        :
******************************************************************************/
DRV_ErrCode DVBTunerInit( void);

/*****************************************************************************
* Function    : DRV_TunerEnableCallback
* Description :
*   ����Tunerģ��Ļص�����.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_TunerEnableCallback(DRV_TunerCallback Callback_f);

/*****************************************************************************
* Function    : DRV_TunerDisableCallback
* Description :
*   �ر�Tunerģ��Ļص�����.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_TunerDisableCallback(void);

/*****************************************************************************
* Function    : DRV_TunerReqCarrier
* Description :
*   ������Ƶ����.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_SetNewTS ( U32_YS Fre, U32_YS Sym, U8_YS Qam );
DRV_ErrCode DRV_DVBSSetNewTS (YSTUNER_SearchPara_t *pTunerScanPara);

/*****************************************************************************
* Function    : DRV_TunerGetSignalQuality
* Description :
*   �õ��ź���������.
* Note        :
******************************************************************************/
DRV_ErrCode GetSignalQuality( U32_YS * pCN_Ratio, U32_YS * pBitErrRate, U32_YS* pCN_dB100);

/*****************************************************************************
* Function    : DRV_TunerGetLockStatus
* Description :
*   �õ���Ƶ״̬.
* Note        :
*   ע�ⷵ��ֵΪDRV_BOOL.
******************************************************************************/
BOOL_YS DRV_TunerGetLockStatus(void);

void DRV_CurrentTuner(U32_YS  *pFre, U32_YS *pSym,  U8_YS *pQam);
void DRV_DVBSCurrentTuner(YSTUNER_SearchPara_t	*tunerpara);
void SignalDiagnoseStop();
void SignalDiagnose();

#ifdef __cplusplus
}
#endif
#endif
