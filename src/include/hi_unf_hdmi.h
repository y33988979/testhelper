
/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009-12-10
  Description   :
  History       :
  1.Date        : 2009-12-10
    Author      : QuYaxin 46153
    Modification: Created file

*******************************************************************************/

#ifndef __HI_UNF_HDMI_H__
#define __HI_UNF_HDMI_H__

/* add include here */
#include "hi_unf_common.h"
#include "hi_unf_sound.h"

#ifdef __cplusplus
extern "C" {
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2_9 */


/*the max audio capability count*//**CNcomment:�����Ƶ���������� */
#define HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT     15 

/*the max audio smprate count*//**CNcomment:�����Ƶ�����ʸ��� */
#define HI_UNF_HDMI_MAX_AUDIO_SMPRATE_COUNT 10 

/*the max infoframe length*//**CNcomment:< �����Ϣ֡���� */
#define HI_UNF_HDMI_MAX_INFOFRAME_LEN   0X20   

/*HDMI default value*//**CNcomment:< HDMI ȱʡֵ */
#define HI_UNF_HDMI_DEFAULT_SETTING     0x00

/*HDMI interface ID *//**CNcomment:HDMI�ӿ�ID  */
typedef enum hiUNF_HDMI_ID_E
{
    HI_UNF_HDMI_ID_0         = 0,          /*HDMI interface 0*/ /**CNcomment:< HDMI�ӿ�0 */
    HI_UNF_HDMI_ID_BUTT
} HI_UNF_HDMI_ID_E;

/*HDMI event type*//**CNcomment: HDMI�¼����� */
typedef enum hiUNF_HDMI_EVENT_TYPE_E
{
    HI_UNF_HDMI_EVENT_HOTPLUG = 0x10,       /*HDMI HotPlug event type*//**CNcomment:<HDMI�Ȳ���¼� */
    HI_UNF_HDMI_EVENT_NO_PLUG,              /*HDMI unplug event type*//**CNcomment:<HDMI Cableû������ �¼� */
    HI_UNF_HDMI_EVENT_EDID_FAIL,            /*HDMI read edid fail event type*//**CNcomment:<HDMI EDID��ȡʧ���¼� */
    HI_UNF_HDMI_EVENT_HDCP_FAIL,            /*HDCP authentication fail event type *//**CNcomment:<HDCP��֤ʧ���¼� */
    HI_UNF_HDMI_EVENT_HDCP_SUCCESS,         /*HDCP authentication succeed event type*//**CNcomment:<HDCP��֤�ɹ� */
    HI_UNF_HDMI_EVENT_RSEN_CONNECT,         /*TMDS link is connected*/   
    HI_UNF_HDMI_EVENT_RSEN_DISCONNECT,      /*TMDS link is disconnected*/    
    HI_UNF_HDMI_EVENT_HDCP_USERSETTING,     /**<HDMI Reset */
    HI_UNF_HDMI_EVENT_BUTT
}HI_UNF_HDMI_EVENT_TYPE_E;

/*Video color space mode*//**CNcomment:��Ƶ��ɫ�ռ�����*/
typedef enum hiUNF_HDMI_VIDEO_MODE
{
    HI_UNF_HDMI_VIDEO_MODE_RGB444,          /*RGB444 output mode*//**CNcomment:<RGB444���ģʽ */ 
    HI_UNF_HDMI_VIDEO_MODE_YCBCR422,        /*YCBCR422 output mode*//**<CNcomment:YCBCR422���ģʽ */ 
    HI_UNF_HDMI_VIDEO_MODE_YCBCR444,        /*YCBCR444 output mode*//**CNcomment:<YCBCR444���ģʽ */ 
        
    HI_UNF_HDMI_VIDEO_MODE_BUTT    
}HI_UNF_HDMI_VIDEO_MODE_E;


/*HDMI Deep color mode*//**CNcomment: HDMI Deep Color ģʽ */
typedef enum hiUNF_HDMI_DEEP_COLOR_E
{
    HI_UNF_HDMI_DEEP_COLOR_24BIT = 0x00,    /*HDMI Deep color 24bit mode*//**CNcomment:< HDMI Deep Color 24bit ģʽ  */
    HI_UNF_HDMI_DEEP_COLOR_30BIT,           /*HDMI Deep color 30bit mode*//**CNcomment:< HDMI Deep Color 30bit ģʽ  */
    HI_UNF_HDMI_DEEP_COLOR_36BIT,           /*HDMI Deep color 36bit mode*//**CNcomment:< HDMI Deep Color 36bit ģʽ  */  
    HI_UNF_HDMI_DEEP_COLOR_OFF   = 0xff,
    HI_UNF_HDMI_DEEP_COLOR_BUTT
}HI_UNF_HDMI_DEEP_COLOR_E;

/*HDMI AVI infoframe BarInfo enum*//**CNcomment: HDMI AVI��Ϣ֡ BarInfo ö�� */
typedef enum hiUNF_HDMI_BARINFO_E
{
   HDMI_BAR_INFO_NOT_VALID,                 /**< Bar Data not valid */
   HDMI_BAR_INFO_V,                         /**< Vertical bar data valid */
   HDMI_BAR_INFO_H,                         /**< Horizental bar data valid */
   HDMI_BAR_INFO_VH                         /**< Horizental and Vertical bar data valid */
}HI_UNF_HDMI_BARINFO_E;

/*HDMI AVI infofram ScanInfo enum*//**CNcomment: HDMI AVI��Ϣ֡ ScanInfo ö�� */
typedef enum hiUNF_HDMI_SCANINFO_E
{
    HDMI_SCAN_INFO_NO_DATA      = 0,        /**< No Scan information*/
    HDMI_SCAN_INFO_OVERSCANNED  = 1,        /**< Scan information, Overscanned (for television) */
    HDMI_SCAN_INFO_UNDERSCANNED = 2,        /**< Scan information, Underscanned (for computer) */
    HDMI_SCAN_INFO_FUTURE
}HI_UNF_HDMI_SCANINFO_E;

/*HDMI AVI InfoFrame picture scan enum*//**CNcomment: HDMI AVI��Ϣ֡ Picture scan ö�� */
typedef enum hiUNF_HDMI_PICTURE_SCALING_E
{
    HDMI_PICTURE_NON_UNIFORM_SCALING,       /**< No Known, non-uniform picture scaling  */
    HDMI_PICTURE_SCALING_H,                 /**< Picture has been scaled horizentally */
    HDMI_PICTURE_SCALING_V,                 /**< Picture has been scaled Vertically */
    HDMI_PICTURE_SCALING_HV                 /**< Picture has been scaled Horizentally and Vertically   */
 }HI_UNF_HDMI_PICTURE_SCALING_E;

/*HDMI AVI InfoFrame colorimetry enum*//**CNcomment: HDMI AVI��Ϣ֡ ɫ�ȿռ� ö�� */
typedef enum hiUNF_HDMI_COLORSPACE_E
{
    HDMI_COLORIMETRY_NO_DATA,               /*Colorimetry No Data option*//**CNcomment:< Colorimetry No Dataѡ�� */
    HDMI_COLORIMETRY_ITU601,                /*Colorimetry ITU601 option*//**CNcomment:< Colorimetry ITU601ɫ�ȿռ�ѡ�� */
    HDMI_COLORIMETRY_ITU709,                /*Colorimetry ITU709 option*//**CNcomment:< Colorimetry ITU709ɫ�ȿռ�ѡ�� */
    HDMI_COLORIMETRY_EXTENDED,              /*Colorimetry extended option*//**CNcomment:< Colorimetry ��չѡ�� */
    HDMI_COLORIMETRY_XVYCC_601,             /*Colorimetry xvYCC601 extened option*//**CNcomment:< Colorimetry xvYCC601��չѡ�� */
    HDMI_COLORIMETRY_XVYCC_709              /*Colorimetry xvYCC709 extened option*//**CNcomment:< Colorimetry xvYCC709��չѡ�� */
}HI_UNF_HDMI_COLORSPACE_E;

/*HDMI AVI InfoFrame RGB range enum*//**CNcomment: HDMI AVI��Ϣ֡ RGBɫ�ȷ�Χ ö�� */
typedef enum hiUNF_HDMI_RGB_QUAN_RAGE_E
{
    HDMI_RGB_QUANTIZATION_DEFAULT_RANGE,    /**< Defaulr range, it depends on the video format */
    HDMI_RGB_QUANTIZATION_LIMITED_RANGE,    /**< Limited quantization range of 220 levels when receiving a CE video format*/
    HDMI_RGB_QUANTIZATION_FULL_RANGE        /**< Full quantization range of 256 levels when receiving an IT video format*/
}HI_UNF_HDMI_RGB_QUAN_RAGE_E;

/**HDMI AVI InfoFrame YCC quantization range enum *//** CNcomment:HDMI AVI��Ϣ֡ YCCɫ�ȷ�Χ ö�� */
typedef enum hiUNF_HDMI_YCC_QUAN_RAGE_E
{
    HDMI_YCC_QUANTIZATION_LIMITED_RANGE,    /**< Limited quantization range of 220 levels when receiving a CE video format*/
    HDMI_YCC_QUANTIZATION_FULL_RANGE        /**< Full quantization range of 256 levels when receiving an IT video format*/
}HI_UNF_HDMI_YCC_QUAN_RAGE_E;

/*HDMI AVI InfoFrame AVI video content type enum*//**CNcomment:HDMI AVI��Ϣ֡ AVI��Ƶ���ݵ����� ö�� */
typedef enum hiUNF_HDMI_CONTENT_TYPE_E
{
    HDMI_CONTNET_GRAPHIC,                   /**< Graphics type*/
    HDMI_CONTNET_PHOTO,                     /**< Photo type*/
    HDMI_CONTNET_CINEMA,                    /**< Cinema type*/
    HDMI_CONTNET_GAME                       /**< Game type*/
}HI_UNF_HDMI_CONTENT_TYPE_E;

/*HDMI Audio InfoFrame Audio type enum*//**CNcomment:HDMI AUD��Ϣ֡ ��Ƶ���� ö�� */
typedef enum hiUNF_HDMI_CODING_TYPE_E
{
    HDMI_AUDIO_CODING_REFER_STREAM_HEAD,  /*Audio coding type, refer stream,default type*//**CNcomment:< ��Ƶ�������ͣ��ο���������ȱʡ��ʽ */
    HDMI_AUDIO_CODING_PCM,                /*Audio coding PCM type*//**CNcomment:< ��Ƶ����PCM��ʽ */
    HDMI_AUDIO_CODING_AC3,                /*Audio coding AC3 type*//**CNcomment:< ��Ƶ����AC3��ʽ */
    HDMI_AUDIO_CODING_MPEG1,              /*Audio coding MPEG1 type*//**CNcomment:< ��Ƶ����MPEG1��ʽ */
    HDMI_AUDIO_CODING_MP3,                /*Audio coding MP3 type*//**CNcomment:< ��Ƶ����MP3��ʽ */
    HDMI_AUDIO_CODING_MPEG2,              /*Audio coding MPEG2 type*//**CNcomment:< ��Ƶ����MPEG2��ʽ */
    HDMI_AUDIO_CODING_AAC,                /*Audio coding AAC type*//**CNcomment:< ��Ƶ����AAC��ʽ */
    HDMI_AUDIO_CODING_DTS,                /*Audio coding DTS type*//**CNcomment:< ��Ƶ����DTS��ʽ */
    HDMI_AUDIO_CODING_DDPLUS,             /*Audio coding DDPLUS type*//**CNcomment:< ��Ƶ����DDPLUS��ʽ */
    HDMI_AUDIO_CODING_MLP,                /*Audio coding MLP type*//**CNcomment:< ��Ƶ����MLP��ʽ */
    HDMI_AUDIO_CODING_WMA,                /*Audio coding WMA type*//**CNcomment:< ��Ƶ����WMA��ʽ */
    HDMI_AUDIO_CODING_MAX
}HI_UNF_HDMI_CODING_TYPE_E;

/*HDMI contain output mode enum */
/**CNcomment: HDMIǣ�����ģʽ ö�� */
typedef enum hiUNF_HDMI_FORCE_ACTION_E
{
    HI_UNF_HDMI_FORCE_NULL,               /*standard mode*//**CNcomment:< ��׼ģʽ */
    HI_UNF_HDMI_FORCE_HDMI,               /*Force HDMI mode output*//**CNcomment:< ǿ�ư�HDMI��ʽ��� */
    HI_UNF_HDMI_FORCE_DVI,                /*Force DVI mode output*//**CNcomment:< ǿ�ư�DVI��ʽ��� */
    HI_UNF_HDMI_INIT_BOOT_CONFIG          /*Set HDMI init config by boot,test only*//**CNcomment:< ʹ��boot��HDMI���õĲ�����ʼ��HDMI, Test Only */
}HI_UNF_HDMI_FORCE_ACTION_E;

/*The hotplug callback function interface */
/**CNcomment: �ӿ��Ȳ�λص����� */
typedef void (*HI_UNF_HDMI_CallBack)(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData);

/*HDMI interface init parameter*/
/** CNcomment:HDMI�ӿڳ�ʼ������ */
typedef struct hiUNF_HDMI_INIT_PARA_S
{
    HI_UNF_HDMI_CallBack pfnHdmiEventCallback;    /*event callback*//**CNcomment:< �¼��ص� */
    HI_VOID             *pCallBackArgs;           /*the private data of callback function parameter *//**CNcomment:< �ص���������˽������ */
    HI_UNF_HDMI_FORCE_ACTION_E enForceMode;       /*0:standard protocol mode,you must set 0 when HDMI ATC test;1: Force  HDMI mode;2: force DVI mode*//**CNcomment:< 0:����׼Э��ģʽ HDMI ATC���Ա���ʹ��0,1:ǿ��HDMIģʽ,2:ǿ��ΪDVIģʽ */
}HI_UNF_HDMI_INIT_PARA_S;

/*HDMI sink capability of interface*/
/**CNcomment: HDMI sink �ӿ������� */
typedef struct hiUNF_HDMI_SINK_CAPABILITY_S
{
    HI_BOOL             bConnected;               /*The Device is connected or disconnected *//**CNcomment:< �豸�Ƿ����� */
    HI_BOOL             bSupportHdmi;             /*The Device suppot HDMI or not,the device is DVI when nonsupport HDMI*//**CNcomment:< �豸�Ƿ�֧��HDMI�������֧�֣���ΪDVI�豸.*/
    HI_BOOL             bIsSinkPowerOn;           /*The sink is PowerOn or not*//**CNcomment:< Sink�豸�Ƿ��ϵ� */
    HI_BOOL             bIsRealEDID;              /*The flag of the sink get real edid or not;HI_TRUE:the sink get real edid,HI_FALSE:force set flag*//**CNcomment:< EDID�Ƿ���������sink�豸��ȡ��־��HI_TRUEΪ��ȷ��ȡ��HI_FASEΪǿ�����ý�� */

    HI_UNF_ENC_FMT_E    enNativeVideoFormat;      /*The sink native video format*//**CNcomment:< ��ʾ�豸����ֱ��� */
    HI_BOOL             bVideoFmtSupported[HI_UNF_ENC_FMT_BUTT]; /*video capability,HI_TRUE:support the video display format;HI_FALSE:nonsupport the video display foramt*//**CNcomment:< ��Ƶ������,HI_TRUE��ʾ֧��������ʾ��ʽ��HI_FALSE��ʾ��֧�� */
    HI_BOOL             bSupportYCbCr;            /*The sink is support YCBCR or not flag;HI_FLASE:only support RGB*//**CNcomment:< �Ƿ�֧��YCBCR��ʾ�� ���ΪHI_FALSE,��ʾֻ֧��RGB��ʾ */
    
    HI_BOOL             bSupportxvYCC601;         /*The sink is support xcYCC601 or not flag*//**CNcomment:< �Ƿ�֧��xvYCC601��ɫ��ʽ */
    HI_BOOL             bSupportxvYCC709;         /*The sink is support xvYCC709 or not flag*//**CNcomment:< �Ƿ�֧��xvYCC709��ɫ��ʽ */
    HI_U8               u8MDBit;                  /*xvYCC601 support transmit profile;1:P0,2:P1,4:P2*//**CNcomment:< xvYCC601֧�ֵĴ���profile:1:P0,2:P1,4:P2 */
    
    HI_BOOL             bAudioFmtSupported[HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT]; /*Audio capability,reference EIA-CEA-861-D,table 37,HI_TRUE:support this Audio type;HI_FALSE,nonsupport this Audio type*//**CNcomment:< ��Ƶ������, ��ο�EIA-CEA-861-D ��37;HI_TRUE��ʾ֧��������ʾ��ʽ��HI_FALSE��ʾ��֧�� */
    HI_U32              u32AudioSampleRateSupported[HI_UNF_HDMI_MAX_AUDIO_SMPRATE_COUNT]; /*PCM smprate capability,0: illegal value,other is support PCM smprate *//**CNcomment:< PCM��Ƶ��������������0Ϊ�Ƿ�ֵ������Ϊ֧�ֵ���Ƶ������ */
    HI_U32              u32MaxPcmChannels;        /*Audio max PCM Channels number*//**< CNcomment:��Ƶ����PCMͨ���� */
    HI_U8               u8Speaker;                /*Speaker location,please reference EIA-CEA-D the definition of SpekearDATABlock*//**CNcomment:< ������λ�ã���ο�EIA-CEA-861-D��SpeakerDATABlock�Ķ��� */
    
    HI_U8               u8IDManufactureName[4];   /*Manufacture name*//**CNcomment:< �豸���̱�ʶ */
    HI_U32              u32IDProductCode;         /*Product code*//**CNcomment:< �豸ID */
    HI_U32              u32IDSerialNumber;        /*Serial numeber of Manufacture*//**CNcomment:< �豸���к� */
    HI_U32              u32WeekOfManufacture;     /*the week of manufacture*//**CNcomment:< �豸��������(��) */
    HI_U32              u32YearOfManufacture;     /*the year of manufacture*//**CNcomment:< �豸��������(��) */
    HI_U8               u8Version;                /*the version of manufacture*//**CNcomment:< �豸�汾�� */
    HI_U8               u8Revision;               /*the revision of manufacture*//**CNcomment:< �豸�Ӱ汾�� */
    HI_U8               u8EDIDExternBlockNum;     /*the extern block number of EDID*//**CNcomment:< EDID��չ����Ŀ */

    HI_U8               u8IEERegId[3];            /**< 24-bit IEEE Registration Identifier (0x000C03) */
    HI_BOOL             bIsPhyAddrValid;          /*the flag of phyiscs address is valid or not*//**CNcomment:< CEC�����ַ�Ƿ���Ч��־ */   
    HI_U8               u8PhyAddr_A;              /*phyiscs address A of CEC*//**CNcomment:< CEC�����ַA */
    HI_U8               u8PhyAddr_B;              /*phyiscs address B of CEC*//**CNcomment:< CEC�����ַB */  
    HI_U8               u8PhyAddr_C;              /*phyiscs address C of CEC*//**CNcomment:< CEC�����ַC */  
    HI_U8               u8PhyAddr_D;              /*phyiscs address D of CEC*//**CNcomment:< CEC�����ַD */  
    HI_BOOL             bSupportDVIDual;          /*the DVI support dual-link or not *//**CNcomment:< �Ƿ�֧�� DVI dual-link ���� */
    HI_BOOL             bSupportDeepColorYCBCR444;/*the Deep Color support YCBCR444 or not*//**CNcomment:< �Ƿ�֧�� YCBCR 4:4:4  Deep Color ģʽ */
    HI_BOOL             bSupportDeepColor30Bit;   /*the Deep Color support 30 bit  or not *//**CNcomment:< �Ƿ�֧��Deep Color 30bit ģʽ */
    HI_BOOL             bSupportDeepColor36Bit;   /*the Deep Color support 36 bit  or not *//**< CNcomment:�Ƿ�֧��Deep Color 36bit ģʽ */
    HI_BOOL             bSupportDeepColor48Bit;   /*the Deep Color support 48 bit  or not *//**< CNcomment:�Ƿ�֧��Deep Color 48bit ģʽ */
    HI_BOOL             bSupportAI;               /*support AI or not *//**CNcomment:< �Ƿ�֧�� Supports_AI ģʽ */
    HI_U32              u8MaxTMDSClock;           /*the max TMDS clock*//**CNcomment:< ���TMDSʱ�� */
    HI_BOOL             bI_Latency_Fields_Present;/*the flag of latency time*//**CNcomment:< ��ʱ��־λ */
    HI_BOOL             bLatency_Fields_Present;  /**< whether Video_Latency and Audio_Latency fields are present */
    HI_BOOL             bHDMI_Video_Present;      /*special Video format*//**CNcomment:< �������Ƶ��ʽ */
    HI_U8               u8Video_Latency;          /*the latency of video*//**CNcomment:< ��Ƶ��ʱ */
    HI_U8               u8Audio_Latency;          /*the latency of audio*//**CNcomment:< ��Ƶ��ʱ */
    HI_U8               u8Interlaced_Video_Latency;/*the latency of interlaced video mode*//**CNcomment:< ������Ƶģʽ�µ���Ƶ��ʱ */
    HI_U8               u8Interlaced_Audio_Latency;/*the latency of interlaced audio mode*//**CNcomment:< ������Ƶģʽ�µ���Ƶ��ʱ */
} HI_UNF_HDMI_SINK_CAPABILITY_S;

/*the config parameter of HDMI interface*/
/**CNcomment:HDMI �ӿڲ������� */
typedef struct hiUNF_HDMI_ATTR_S
{
	HI_BOOL                 bEnableHdmi;         /*force to HDMI or DVI,the value must set before HI_UNF_HDMI_Start or behind HI_UNF_HDMI_Stop*//**CNcomment:< �Ƿ�ǿ��HDMI,����ΪDVI.��ֵ������ HI_UNF_HDMI_Start֮ǰ����HI_UNF_HDMI_Stop֮������  */

    HI_BOOL                 bEnableVideo;        /*parameter must set HI_TRUE,or the HDMI diver will force to set HI_TRUE*//**CNcomment:< ������HI_TRUE, �����HI_FALSE:HDMI������ǿ������ΪHI_TRUE */
    HI_UNF_ENC_FMT_E        enVideoFmt;          /*video fromat ,the format must consistent with display  config*//**CNcomment:< ��Ƶ��ʽ,�˲�����Ҫ��Display���õ���ʽ����һ�� */
    
    HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;       /*HDMI output vedio mode VIDEO_MODE_YCBCR,VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 *//**CNcomment:< HDMI�����Ƶģʽ��VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 */
    HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode;    /*Deep Color output mode,defualt: HI_UNF_HDMI_DEEP_COLOR_24BIT *//**CNcomment:< DeepColor���ģʽ, Ĭ��ΪHI_UNF_HDMI_DEEP_COLOR_24BIT */
    HI_BOOL                 bxvYCCMode;          /*the xvYCC output mode,default:HI_FALSE*//**CNcomment:< xvYCC���ģʽ��Ĭ��ΪHI_FALSE */

    HI_BOOL                 bEnableAudio;        /*Enable flag of Audio*//**CNcomment:< �Ƿ�Enable��Ƶ */
    HI_UNF_SND_INTERFACE_E  enSoundIntf;         /*the origin of Sound,suggestion set HI_UNF_SND_INTERFACE_I2S,the parameter need consistent with Ao input *//**CNcomment:< HDMI��Ƶ��Դ, ����HI_UNF_SND_INTERFACE_I2S,�˲�����Ҫ��AO���뱣��һ�� */
    HI_BOOL                 bIsMultiChannel;     /*set mutiChannel or stereo ;0:stereo,1:mutichannel fixup 8 channel *//**CNcomment:< ������������������0:��������1:�������̶�Ϊ8���� */
    HI_UNF_SAMPLE_RATE_E    enSampleRate;        /*the samplerate of audio,this parameter consistent with AO config *//**CNcomment:< PCM��Ƶ������,�˲�����Ҫ��AO�����ñ���һ�� */
    HI_U8                   u8DownSampleParm;    /*PCM parameter of dowmsample,default 0*//**CNcomment:< PCM��Ƶ����downsample�����ʵĲ�����Ĭ��Ϊ0 */
    
    HI_UNF_BIT_DEPTH_E      enBitDepth;          /*the audio bit depth,defualt 16,this parameter consistent with AO config*//**CNcomment:< ��Ƶλ��Ĭ��Ϊ16,�˲�����Ҫ��AO�����ñ���һ�� */
    HI_U8                   u8I2SCtlVbit;        /*reserve:config 0,I2S control(0x7A:0x1D)*//**CNcomment:< ������������Ϊ0, I2S control (0x7A:0x1D) */

    HI_BOOL                 bEnableAviInfoFrame; /*Enable flag of AVI InfoFrame,suggestion:enable *//**CNcomment:< �Ƿ�ʹ�� AVI InfoFrame������ʹ�� */
    HI_BOOL                 bEnableAudInfoFrame; /*Enable flag of Audio InfoFrame,suggestion:enable*//**CNcomment:< �Ƿ�ʹ�� AUDIO InfoFrame������ʹ�� */
    HI_BOOL                 bEnableSpdInfoFrame; /*Enable flag of SPD info frame,suggestion:disable*//**CNcomment:< �Ƿ�ʹ�� SPD InfoFrame�� ����ر� */
    HI_BOOL                 bEnableMpegInfoFrame;/*Enable flag of MPEG info frame,suggestion:disable*//**CNcomment:< �Ƿ�ʹ�� MPEG InfoFrame�� ����ر� */

    HI_U32                  bDebugFlag;          /*the flag of hdmi dubug,suggestion:disable*//**CNcomment:< �Ƿ�ʹ�� ��hdmi�ڲ�debug��Ϣ�� ����ر� */
    HI_BOOL                 bHDCPEnable;         /*0:HDCP disable mode,1:eable HDCP mode*//**CNcomment:< 0:HDCP�����1:HDCPģʽ�� */
    
    HI_BOOL                 b3DEnable;           /*0:disable 3d,1,enable 3d mode*//**CNcomment:< 0:3D�����1:3Dģʽ�� */
    HI_U32                  u83DParam;           /*3D Parameter,defualt HI_FALSE*//**CNcomment:< 3D Parameter, Ĭ��ΪHI_FALSE */
} HI_UNF_HDMI_ATTR_S;

/*HDMI infoFrame type definition*/
/**CNcomment: HDMI InfoFrame ���Ͷ��� */
typedef enum tagHI_UNF_HDMI_INFOFRAME_TYPE_E
{
    HI_INFOFRAME_TYPE_AVI,          /*HDMI AVI InfoFrame type defintion*//**CNcomment:< HDMI AVI InfoFrame ���Ͷ��� */
    HI_INFOFRAME_TYPE_SPD,          /*HDMI SPD InfoFrame type defintion*//**CNcomment:< HDMI SPD InfoFrame ���Ͷ��� */
    HI_INFOFRAME_TYPE_AUDIO,        /*HDMI AUDIO InfoFrame type defintion*//**CNcomment:< HDMI AUDIO InfoFrame ���Ͷ��� */    
    HI_INFOFRAME_TYPE_MPEG,         /*HDMI MPEG InfoFrame type defintion*//**CNcomment:< HDMI MPEG InfoFrame ���Ͷ��� */
    HI_INFOFRAME_TYPE_VENDORSPEC,   /*HDMI Specific InfoFrame type defintion*//**CNcomment:< HDMI Vendor Specific InfoFrame ���Ͷ��� */  
    HI_INFOFRAME_TYPE_BUTT 
}HI_UNF_HDMI_INFOFRAME_TYPE_E;

/*HDMI AVI InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI AVI InfoFrame �����ṹ, ��ο�EIA-CEA-861-D */
typedef struct hiUNF_HDMI_AVI_INFOFRAME_VER2_S
{
    HI_UNF_ENC_FMT_E               enTimingMode;            /*AVI video timing format*//**CNcomment:< AVI��Ƶtiming��ʽ */
    HI_UNF_HDMI_VIDEO_MODE_E       enOutputType;            /*AVI video output color space*//**CNcomment:< AVI��Ƶ�����ɫ��ʽ */   
    HI_BOOL                        bActive_Infor_Present;   /*AVI video Active_Infor_Present flag*//**CNcomment:< AVI��ƵActive_Infor_Present��־λ */
    HI_UNF_HDMI_BARINFO_E          enBarInfo;               /*AVI video BarInfo type*//**CNcomment:< AVI��ƵBarInfo���� */
    HI_UNF_HDMI_SCANINFO_E         enScanInfo;              /*AVI video ScanInfo type*//**CNcomment:< AVI��ƵScanInfo���� */
    HI_UNF_HDMI_COLORSPACE_E       enColorimetry;           /*AVI video Colorimetry type*//**CNcomment:< AVI��ƵColorimetry���� */
    HI_UNF_ASPECT_RATIO_E          enAspectRatio;           /*AVI video AspectRatio type*//**CNcomment:< AVI��Ƶ��߱ȸ�ʽ */
    HI_UNF_ASPECT_RATIO_E          enActiveAspectRatio;     /*AVI video Active AspectRatio type*//**CNcomment:< AVI��Ƶ��Ч��߱ȸ�ʽ */
    HI_UNF_HDMI_PICTURE_SCALING_E  enPictureScaling;        /*AVI video picture scaling type*//**CNcomment:< AVI��Ƶscaling��ʽ */
    HI_UNF_HDMI_RGB_QUAN_RAGE_E    enRGBQuantization;       /*AVI video RGB Quantization*//**CNcomment:< AVI��ƵRGBɫ�ȷ�Χ */
    HI_BOOL                        bIsITContent;            /*AVI video ITContent flag*//**CNcomment:< AVI��ƵITcontent */
    HI_U32                         u32PixelRepetition;      /*AVI video Pixel Repetition flag*//**CNcomment:< AVI��Ƶ�����ش���־λ */

    HI_UNF_HDMI_CONTENT_TYPE_E     enContentType;           /*AVI video content type*//**CNcomment:< AVI��Ƶ���ݵ����� */
    HI_UNF_HDMI_YCC_QUAN_RAGE_E    enYCCQuantization;       /*AVI video YCC Quantization*//*CNcomment:*< AVI��ƵYCCɫ�ȷ�Χ */
    
    HI_U32                         u32LineNEndofTopBar;     /*AVI video EndofTopBar coordinate,defualt:0 *//**CNcomment:< AVI��ƵEndofTopBar���꣬ȱʡΪ0 */
    HI_U32                         u32LineNStartofBotBar;   /*AVI video StartofBotBar coordinate,defualt:0*//**CNcomment:< AVI��ƵStartofBotBar���꣬ȱʡΪ0 */
    HI_U32                         u32PixelNEndofLeftBar;   /*AVI video EndofLeft coordinate,defualt:0*//**CNcomment:< AVI��ƵEndofLeft���꣬ȱʡΪ0 */
    HI_U32                         u32PixelNStartofRightBar;/*AVI video StartofRightBar coordinate,defualt:0*//**CNcomment:< AVI��ƵStartofRightBar���꣬ȱʡΪ0 */ 
}HI_UNF_HDMI_AVI_INFOFRAME_VER2_S;
/*HDMI AUDIO InfoFrame parameter struct ,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI AUDIO InfoFrame �����ṹ, ��ο�EIA-CEA-861-D */
typedef struct hiUNF_HDMI_AUD_INFOFRAME_VER1_S
{
    HI_U32                         u32ChannelCount;          /*audio frequency channel count*//**CNcomment:< ��Ƶ ������ */
    HI_UNF_HDMI_CODING_TYPE_E      enCodingType;             /*audio frequency coding type,default 0;Refer to Stream Header*//**CNcomment:< ��Ƶ �������ͣ�ȱʡΪ0��Refer to Stream Header */
    HI_U32                         u32SampleSize;            /*audio frequency sample size,default 0,Refer to Stream Header*//**CNcomment:< ��Ƶ ������С��ȱʡΪ0��Refer to Stream Header */
    HI_U32                         u32SamplingFrequency;     /*audio frequency sampling frequency ,default 0,Refer to Stream Header*//**CNcomment:< ��Ƶ ����Ƶ�ʣ�ȱʡΪ0��Refer to Stream Header */
    HI_U32                         u32ChannelAlloc;          /*audio frequency channel allocable ,default 0,Refer to Stream Header*//**CNcomment:< ��Ƶ �������䣬ȱʡΪ0��Refer to Stream Header */
    HI_U32                         u32LevelShift;            /*audio frequency Levelshift ,default 0,Refer to Stream Header*//**CNcomment:< ��Ƶ Levelshift��ȱʡΪ0��Refer to Stream Header */
    HI_BOOL                        u32DownmixInhibit;        /*audio frequency DownmixInhibit ,default 0,Refer to Stream Header*//**CNcomment:< ��Ƶ DownmixInhibit��ȱʡΪ0��Refer to Stream Header */
}HI_UNF_HDMI_AUD_INFOFRAME_VER1_S;

/*HDMI Source Product Description InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI Source Product Description InfoFrame �����ṹ , ��ο�EIA-CEA-861-D */
typedef struct hiUNF_HDMI_SPD_INFOFRAME_S
{
    HI_U8                          u8VendorName[8];          /*vendor name*//**CNcomment:< �������� */
    HI_U8                          u8ProductDescription[16]; /*product Description*//**CNcomment:< ��Ʒ������ */  
}HI_UNF_HDMI_SPD_INFOFRAME_S;

/*HDMI Source HDMI MEPG SOURCE Description InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI MEPG SOURCE Description InfoFrame �����ṹ , ��ο�EIA-CEA-861-D */
typedef struct hiUNF_HDMI_MPEGSOURCE_INFOFRAME_S
{
    HI_U32                         u32MPEGBitRate;           /*MPEG bit Rate*//**CNcomment:< MPEGλ�� */
    HI_BOOL                        bIsFieldRepeated;         /*FieldRepeater flag*//**CNcomment:< FieldRepeater��־λ */
}HI_UNF_HDMI_MPEGSOURCE_INFOFRAME_S;

/*HDMI Vendor Specific InfoFrame parameter struct,please reference EIA-CEA-861-D*/
/**CNcomment: HDMI Vendor Specific InfoFrame �����ṹ , ��ο�EIA-CEA-861-D */
typedef struct hiUNF_HDMI_VENDORSPEC_INFOFRAME_S
{
    HI_U32                         u32RegistrationId;       /**CNcomment:< ע��ID�� */
}HI_UNF_HDMI_VENDORSPEC_INFOFRAME_S;

/*HDMI HDMI InfoFrame unit struct*/
/**CNcomment: HDMI InfoFrame ���϶����� */
typedef union hiUNF_HDMI_INFOFRAME_UNIT_U
{
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S   stAVIInfoFrame;      /*AVI FrameInfo*//**CNcomment:< AVI��Ϣ֡ */
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S   stAUDInfoFrame;      /*Audio FrameInfo*//**CNcomment:< AUD��Ϣ֡ */
    HI_UNF_HDMI_SPD_INFOFRAME_S        stSPDInfoFrame;      /*SPD FrameInfo*//**CNcomment:< SPD��Ϣ֡ */
    HI_UNF_HDMI_MPEGSOURCE_INFOFRAME_S stMPEGSourceInfoFrame;/*MPEGSource FrameInfo*//**CNcomment:< MPEGSource��Ϣ֡ */
    HI_UNF_HDMI_VENDORSPEC_INFOFRAME_S stVendorSpecInfoFrame;/*VS FrameInfo*//**CNcomment:< VS��Ϣ֡ */
}HI_UNF_HMDI_INFORFRAME_UNIT_U;

/*HDMI InfoFrame struct */
/**CNcomment: HDMI InfoFrame ���ݽṹ */
typedef struct hiUNF_HDMI_INFOFRAME_S
{
    HI_UNF_HDMI_INFOFRAME_TYPE_E    enInfoFrameType;  /*InfoFrame type*//**CNcomment:<InfoFrame���� */
    HI_UNF_HMDI_INFORFRAME_UNIT_U   unInforUnit;      /*InfoFrame unit data*//**CNcomment:<InfoFrame���� */  
}HI_UNF_HDMI_INFOFRAME_S;

typedef enum hiUNF_HDMI_3D_FORMAT_E
{
    HI_UNF_3D_FRAME_PACKETING                 = 0x00,
    HI_UNF_3D_FIELD_ALTERNATIVE               = 0x01,
    HI_UNF_3D_LINE_ALTERNATIVE                = 0x02,
    HI_UNF_3D_SIDE_BY_SIDE_FULL               = 0x03,
    HI_UNF_3D_L_DEPTH                         = 0x04,
    HI_UNF_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH = 0x05,
    HI_UNF_3D_TOP_AND_BOTTOM                  = 0x06,
    HI_UNF_3D_SIDE_BY_SIDE_HALF               = 0x08,
    HI_UNF_3D_MAX_BUTT
}HI_UNF_HDMI_3D_FORMAT_E;

/**<  CEC */
/**< CEC interrelated Opcode:Please refer to CEC 15 Message Descriptions */
/**CNcomment:< CEC ��ص�Opcode:Please refer to CEC 15 Message Descriptions */
/**< General Protocol messages*/
#define CEC_OPCODE_FEATURE_ABORT                  0X00  /**< <Feature Abort> Used as a response to indicate that the device does not support the requested message type, or that it cannot execute it at the present time. */
#define CEC_OPCODE_ABORT_MESSAGE                  0XFF  /**< <Abort> Message This message is reserved for testing purposes. */
/**< One Touch Play Feature*/
#define CEC_OPCODE_ACTIVE_SOURCE                  0X82  /**< <Active Source> Used by a new source to indicate that it has started to transmit a stream OR used in response to a <Request Active Source>  */
#define CEC_OPCODE_IMAGE_VIEW_ON                  0X04  /**< <Image View On> Sent by a source device to the TV whenever it enters the active state (alternatively it may send <Text View On>). */
#define CEC_OPCODE_TEXT_VIEW_ON                   0X0D  /**< <Text View On> As <Image View On>, but should also remove any text, menus and PIP windows from the TV's display.*/
/**< Routing Control Feature*/
#define CEC_OPCODE_INACTIVE_SOURCE                0X9D  /**< <Inactive Source> Used by the currently active source to inform the TV that it has no video to be presented to the user, or is going into standby as the result of a local user command on the device. */
#define CEC_OPCODE_REQUEST_ACTIVE_SOURCE          0X85  /**< <Request Active Source> Used by a new device to discover the status of the system. */
#define CEC_OPCODE_ROUTING_CHANGE                 0X80  /**< <Routing Change> Sent by a CEC Switch when it is manually switched to inform all other devices on the network that the active route below the switch has changed. */
#define CEC_OPCODE_ROUTING_INFORMATION            0X81  /**< <Routing Information> Sent by a CEC Switch to indicate the active route below the switch. */
#define CEC_OPCODE_SET_STREAM_PATH                0X86  /**< <Set Stream Path> Used by the TV to request a streaming path from the specified physical address. */
/**< Standby Feature*/
#define CEC_OPCODE_STANDBY                        0X36  /**< <Standby> Switches one or all devices into standby mode. Can be used as a broadcast message or be addressed to a specific device. See section CEC 13.3 for important notes on the use of this message */
/**< One Touch Record Feature*/
#define CEC_OPCODE_RECORD_OFF                     0X0B  /**< <Record Off> Requests a device to stop a recording. */
#define CEC_OPCODE_RECORD_ON                      0X09  /**< <Record On> Attempt to record the specified source. */
#define CEC_OPCODE_RECORD_STATUS                  0X0A  /**< <Record Status> Used by a Recording Device to inform the initiator of the message <Record On> about its status. */
#define CEC_OPCODE_RECORD_TV_SCREEN               0X0F  /**< <Record TV Screen> Request by the Recording Device to record the presently displayed source. */
/**< Timer Programming Feature*/
#define CEC_OPCODE_CLEAR_ANALOGUE_TIMER           0X33  /**< <Clear Analogue Timer> Used to clear an Analogue timer block of a device. */
#define CEC_OPCODE_CLEAR_DIGITAL_TIMER            0X99  /**< <Clear Digital Timer> Used to clear a Digital timer block of a device. */
#define CEC_OPCODE_CLEAR_EXTERNAL_TIMER           0XA1  /**< <Clear External Timer> Used to clear an External timer block of a device. */
#define CEC_OPCODE_SET_ANALOGUE_TIMER             0X34  /**< <Set Analogue Timer> Used to set a single timer block on an Analogue Recording Device. */
#define CEC_OPCODE_SET_DIGITAL_TIMER              0X97  /**< <Set Digital Timer> Used to set a single timer block on a Digital Recording Device. */
#define CEC_OPCODE_SET_EXTERNAL_TIMER             0XA2  /**< <Set External Timer> Used to set a single timer block to record from an external device. */
#define CEC_OPCODE_SET_TIMER_PROGRAM_TITLE        0X67  /**< <Set Timer Program Title> Used to set the name of a program associated with a timer block. Sent directly after sending a <Set Analogue Timer> or <Set Digital Timer> message. The name is then associated with that timer block. */
#define CEC_OPCODE_TIMER_CLEARED_STATUS           0X43  /**< <Timer Cleared Status> Used to give the status of a <Clear Analogue Timer>, <Clear Digital Timer> or <Clear External Timer> message. */
#define CEC_OPCODE_TIMER_STATUS                   0X35  /**< <Timer Status> Used to send timer status to the initiator of a <Set Timer> message. */
/**< System Information Feature*/
#define CEC_OPCODE_CEC_VERSION                    0X9E  /**< <CEC Version> Used to indicate the supported CEC version, in response to a <Get CEC Version> */
#define CEC_OPCODE_GET_CEC_VERSION                0X9F  /**< <Get CEC Version> Used by a device to enquire which version of CEC the target supports */
#define CEC_OPCODE_GIVE_PHYSICAL_ADDRESS          0X83  /**< <Give Physical Address> A request to a device to return its physical address. */
#define CEC_OPCODE_REPORT_PHYSICAL_ADDRESS        0X84  /**< <Report Physical Address> Used to inform all other devices of the mapping between physical and logical address of the initiator. */
#define CEC_OPCODE_GET_MENU_LANGUAGE              0X91  /**< <Get Menu Language> Sent by a device capable of character generation (for OSD and Menus) to a TV in order to discover the currently selected Menu language. Also used by a TV during installation to discover the currently set menu language of other devices. */
#define CEC_OPCODE_SET_MENU_LANGUAGE              0X32  /**< <Set Menu Language> Used by a TV or another device to indicate the menu language. */
/**< Deck Control Feature*/
#define CEC_OPCODE_DECK_CONTROL                   0X42  /**< <Deck Control> Used to control a device's media functions. */
#define CEC_OPCODE_DECK_STATUS                    0X1B  /**< <Deck Status> Used to provide a deck's status to the initiator of the <Give Deck Status> message. */
#define CEC_OPCODE_GIVE_DECK_STATUS               0X1A  /**< <Give Deck Status> Used to request the status of a device, regardless of whether or not it is the current active source. */
#define CEC_OPCODE_PLAY                           0X41  /**< <Play> Used to control the playback behaviour of a source device. */
/**< Tuner Control Feature*/
#define CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS       0X08  /**< <Give Tuner Device Status> Used to request the status of a tuner device. */
#define CEC_OPCODE_SELECT_ANALOGUE_SERVICE        0X92  /**< <Select Analogue Service> Directly selects an Analogue TV service */
#define CEC_OPCODE_SELECT_DIGITAL_SERVICE         0X93  /**< <Select Digital Service> Directly selects a Digital TV, Radio or Data Broadcast Service */
#define CEC_OPCODE_TUNER_DEVICE_STATUS            0X07  /**< <Tuner Device Status> Use by a tuner device to provide its status to the initiator of the <Give Tuner Device Status> message. */
#define CEC_OPCODE_TUNER_STEP_DECREMENT           0X06  /**< <Tuner Step Decrement> Used to tune to next lowest service in a tuner's service list. Can be used for PIP. */
#define CEC_OPCODE_TUNER_STEP_INCREMENT           0X05  /**< <Tuner Step Increment> Used to tune to next highest service in a tuner's service list. Can be used for PIP. */
/**< Vendor Specific Command*/
#define CEC_OPCODE_DEVICE_VENDOR_ID               0X87  /**< <Device Vendor ID> Reports the vendor ID of this device. */
#define CEC_OPCODE_GIVE_DEVICE_VENDOR_ID          0X8C  /**< <Give Device Vendor ID> Requests the Vendor ID from a device. */
#define CEC_OPCODE_VENDOR_COMMAND                 0X89  /**< <Vendor Command> Allows vendor specific commands to be sent between two devices. */
#define CEC_OPCODE_VENDOR_COMMAND_WITH_ID         0XA0  /**< <Vendor Command With ID> Allows vendor specific commands to be sent between two devices or broadcast. */
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN      0X8A  /**< <Vendor Remote Button Down> Indicates that a remote control button has been depressed. */
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP        0X8B  /**< <Vendor Remote Button Up> Indicates that a remote control button (the last button pressed indicated by the Vendor Remote Button Down message) has been released. */
/**< OSD Display Feature*/
#define CEC_OPCODE_SET_OSD_STRING                 0X64  /**< <Set OSD String> Used to send a text message to output on a TV. */
#define CEC_OPCODE_GIVE_OSD_NAME                  0X46  /**< <Give OSD Name> Used to request the preferred OSD name of a device for use in menus associated with that device. */
#define CEC_OPCODE_SET_OSD_NAME                   0X47  /**< <Set OSD Name> Used to set the preferred OSD name of a device for use in menus associated with that device. */
/**< Device Menu Control Feature*/
#define CEC_OPCODE_MENU_REQUEST                   0X8D  /**< <Menu Request> A request from the TV for a device to show/remove a menu or to query if a device is currently showing a menu. */
#define CEC_OPCODE_MENU_STATUS                    0X8E  /**< <Menu Status> Used to indicate to the TV that the device is showing/has removed a menu and requests the remote control keys to be passed though. */
#define CEC_OPCODE_USER_CONTROL_PRESSED           0X44  /**< <User Control Pressed> Used to indicate that the user pressed a remote control button or switched from one remote control button to another. */
#define CEC_OPCODE_USER_CONTROL_RELEASED          0X45  /**< <User Control Released> Indicates that user released a remote control button (the last one indicated by the <User Control Pressed> message) */
/**< Power Status Feature*/
#define CEC_OPCODE_GIVE_DEVICE_POWER_STATUS       0X8F  /**< <Give Device Power Status> Used to determine the current power status of a target device */
#define CEC_OPCODE_REPORT_POWER_STATUS            0X90  /**< <Report Power Status> Used to inform a requesting device of the current power status */
/**< System Audio Control Feature*/
#define CEC_OPCODE_GIVE_AUDIO_STATUS              0X71  /**< <Give Audio Status> Requests an amplifier to send its volume and mute status */
#define CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS  0x7D  /**< <Give System Audio Mode Status> Requests the status of the System Audio Mode */
#define CEC_OPCODE_REPORT_AUDIO_STATUS            0X7A  /**< <Report Audio Status> Reports an amplifier's volume and mute status */
#define CEC_OPCODE_SET_SYSTEM_AUDIO_MODE          0X72  /**< <Set System Audio Mode> Turns the System Audio Mode On or Off. */
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST      0X70  /**< <System Audio Mode Request> A device implementing System Audio Control and which has volume control RC buttons (eg TV or STB) requests to use System Audio Mode to the amplifier */
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS       0X7E  /**< <System Audio Mode Status> Reports the current status of the System Audio Mode */
/**< Audio Rate Control Feature*/
#define CEC_OPCODE_SET_AUDIO_RATE                 0X9A  /**< <Set Audio Rate> Used to control audio rate from Source Device. */

#define CEC_OPCODE_POLLING_MESSAGE                0XFE/**<  POLL message have no opcode, So, we just use this value */

typedef enum hiUNF_CEC_LOGICALADD_S
{
    HI_UNF_CEC_LOGICALADD_TV               = 0X00,
    HI_UNF_CEC_LOGICALADD_RECORDDEV_1      = 0X01,
    HI_UNF_CEC_LOGICALADD_RECORDDEV_2      = 0X02,
    HI_UNF_CEC_LOGICALADD_TUNER_1          = 0X03,
    HI_UNF_CEC_LOGICALADD_PLAYDEV_1        = 0X04,
    HI_UNF_CEC_LOGICALADD_AUDIOSYSTEM      = 0X05,
    HI_UNF_CEC_LOGICALADD_TUNER_2          = 0X06,
    HI_UNF_CEC_LOGICALADD_TUNER_3          = 0X07,
    HI_UNF_CEC_LOGICALADD_PLAYDEV_2        = 0X08,
    HI_UNF_CEC_LOGICALADD_RECORDDEV_3      = 0X09,
    HI_UNF_CEC_LOGICALADD_TUNER_4          = 0X0A,
    HI_UNF_CEC_LOGICALADD_PLAYDEV_3        = 0X0B,
    HI_UNF_CEC_LOGICALADD_RESERVED_1       = 0X0C,
    HI_UNF_CEC_LOGICALADD_RESERVED_2       = 0X0D,
    HI_UNF_CEC_LOGICALADD_SPECIALUSE       = 0X0E,
    HI_UNF_CEC_LOGICALADD_BROADCAST        = 0X0F,
    HI_UNF_CEC_LOGICALADD_BUTT
}HI_UNF_CEC_LOGICALADD_S;

typedef enum hiUNF_CEC_CMDTYPE_E
{
    HI_UNF_CEC_STRUCTCOMMAND,                    /*CEC struct command*//**CNcomment:< CEC �ṹ���� */
    HI_UNF_CEC_RAWCOMMAND,                       /*CEC raw command*//**CNcomment:< CEC ԭʼ���� */
    HI_UNF_CEC_BUTT
}HI_UNF_CEC_CMDTYPE_E;

typedef struct hiUNF_CEC_RAWDATA_S
{
    HI_U8                              u8Length;
    HI_U8                              u8Data[15];    
}HI_UNF_CEC_RAWDATA_S;

typedef enum hiUNF_CEC_UICMD_E                   /**< User Interface Command Opcode */
{
    HI_UNF_CEC_UICMD_SELECT                      = 0x00,
    HI_UNF_CEC_UICMD_UP                          = 0x01,
    HI_UNF_CEC_UICMD_DOWN                        = 0x02,
    HI_UNF_CEC_UICMD_LEFT                        = 0x03,
    HI_UNF_CEC_UICMD_RIGHT                       = 0x04,
    HI_UNF_CEC_UICMD_RIGHT_UP                    = 0x05,
    HI_UNF_CEC_UICMD_RIGHT_DOWN                  = 0x06,
    HI_UNF_CEC_UICMD_LEFT_UP                     = 0x07,
    HI_UNF_CEC_UICMD_LEFT_DOWN                   = 0x08,
    HI_UNF_CEC_UICMD_ROOT_MENU                   = 0x09,
    HI_UNF_CEC_UICMD_SETUP_MENU                  = 0x0A,
    HI_UNF_CEC_UICMD_CONTENTS_MENU               = 0x0B,
    HI_UNF_CEC_UICMD_FAVORITE_MENU               = 0x0C,
    HI_UNF_CEC_UICMD_EXIT                        = 0x0D,
    HI_UNF_CEC_UICMD_NUM_0                       = 0x20,
    HI_UNF_CEC_UICMD_NUM_1                       = 0x21,
    HI_UNF_CEC_UICMD_NUM_2                       = 0x22,
    HI_UNF_CEC_UICMD_NUM_3                       = 0x23,
    HI_UNF_CEC_UICMD_NUM_4                       = 0x24,
    HI_UNF_CEC_UICMD_NUM_5                       = 0x25,
    HI_UNF_CEC_UICMD_NUM_6                       = 0x26,
    HI_UNF_CEC_UICMD_NUM_7                       = 0x27,
    HI_UNF_CEC_UICMD_NUM_8                       = 0x28,
    HI_UNF_CEC_UICMD_NUM_9                       = 0x29,
    HI_UNF_CEC_UICMD_DOT                         = 0x2A,
    HI_UNF_CEC_UICMD_ENTER                       = 0x2B,
    HI_UNF_CEC_UICMD_CLEAR                       = 0x2C,
    HI_UNF_CEC_UICMD_NEXT_FAVORITE               = 0x2F,
    HI_UNF_CEC_UICMD_CHANNEL_UP                  = 0x30,
    HI_UNF_CEC_UICMD_CHANNEL_DOWN                = 0x31,
    HI_UNF_CEC_UICMD_PREVIOUS_CHANNEL            = 0x32,
    HI_UNF_CEC_UICMD_SOUND_SELECT                = 0x33,
    HI_UNF_CEC_UICMD_INPUT_SELECT                = 0x34,
    HI_UNF_CEC_UICMD_DISPLAY_INFORMATION         = 0x35,
    HI_UNF_CEC_UICMD_HELP                        = 0x36,
    HI_UNF_CEC_UICMD_PAGE_UP                     = 0x37,
    HI_UNF_CEC_UICMD_PAGE_DOWN                   = 0x38,
    HI_UNF_CEC_UICMD_POWER                       = 0x40,
    HI_UNF_CEC_UICMD_VOLUME_UP                   = 0x41,
    HI_UNF_CEC_UICMD_VOLUME_DOWN                 = 0x42,
    HI_UNF_CEC_UICMD_MUTE                        = 0x43,
    HI_UNF_CEC_UICMD_PLAY                        = 0x44,
    HI_UNF_CEC_UICMD_STOP                        = 0x45,
    HI_UNF_CEC_UICMD_PAUSE                       = 0x46,
    HI_UNF_CEC_UICMD_RECORD                      = 0x47,
    HI_UNF_CEC_UICMD_REWIND                      = 0x48,
    HI_UNF_CEC_UICMD_FAST_FORWARD                = 0x49,
    HI_UNF_CEC_UICMD_EJECT                       = 0x4A,
    HI_UNF_CEC_UICMD_FORWARD                     = 0x4B,
    HI_UNF_CEC_UICMD_BACKWARD                    = 0x4C,
    HI_UNF_CEC_UICMD_STOP_RECORD                 = 0x4D,
    HI_UNF_CEC_UICMD_PAUSE_RECORD                = 0x4E,
    HI_UNF_CEC_UICMD_ANGLE                       = 0x50,
    HI_UNF_CEC_UICMD_SUBPICTURE                  = 0x51,
    HI_UNF_CEC_UICMD_VIDEO_ON_DEMAND             = 0x52,
    HI_UNF_CEC_UICMD_ELECTRONIC_PROGRAM_GUIDE    = 0x53,
    HI_UNF_CEC_UICMD_TIMER_PROGRAMMING           = 0x54,
    HI_UNF_CEC_UICMD_INITIAL_CONFIGURATION       = 0x55,
    HI_UNF_CEC_UICMD_PLAY_FUNCTION               = 0x60,
    HI_UNF_CEC_UICMD_PAUSE_PLAY_FUNCTION         = 0x61,
    HI_UNF_CEC_UICMD_RECORD_FUNCTION             = 0x62,
    HI_UNF_CEC_UICMD_PAUSE_RECORD_FUNCTION       = 0x63,
    HI_UNF_CEC_UICMD_STOP_FUNCTION               = 0x64,
    HI_UNF_CEC_UICMD_MUTE_FUNCTION               = 0x65,
    HI_UNF_CEC_UICMD_RESTORE_VOLUME_FUNCTION     = 0x66,
    HI_UNF_CEC_UICMD_TUNE_FUNCTION               = 0x67,
    HI_UNF_CEC_UICMD_SELECT_MEDIA_FUNCTION       = 0x68,
    HI_UNF_CEC_UICMD_SELECT_AV_INPUT_FUNCTION    = 0x69,
    HI_UNF_CEC_UICMD_SELECT_AUDIO_INPUT_FUNCTION = 0x6A,
    HI_UNF_CEC_UICMD_POWER_TOGGLE_FUNCTION       = 0x6B,
    HI_UNF_CEC_UICMD_POWER_OFF_FUNCTION          = 0x6C,
    HI_UNF_CEC_UICMD_POWER_ON_FUNCTION           = 0x6D,
    HI_UNF_CEC_UICMD_F1_BLUE                     = 0x71,
    HI_UNF_CEC_UICMD_F2_RED                      = 0x72,
    HI_UNF_CEC_UICMD_F3_GREEN                    = 0x73,
    HI_UNF_CEC_UICMD_F4_YELLOW                   = 0x74,
    HI_UNF_CEC_UICMD_F5                          = 0x75,
    HI_UNF_CEC_UICMD_DATA                        = 0x76
}HI_UNF_CEC_UICMD_E;                                     /* User Control Code */

typedef union hiUNF_CEC_Operand_t
{
    HI_UNF_CEC_RAWDATA_S               stRawData;
    HI_UNF_CEC_UICMD_E                 stUIOpcode;
}HI_UNF_CEC_Operand_t;

/*CEC struct command*/
/*CNcomment: CEC ����ṹ */

/*HDMI CEC parameter struct*/
/**CNcomment: HDMI CEC �����ṹ */
typedef struct hiUNF_HDMI_CEC_CMD_S
{
    HI_UNF_CEC_LOGICALADD_S  enSrcAdd;               /*CEC struct command*//**CNcomment:< CEC ����ṹ */
    HI_UNF_CEC_LOGICALADD_S  enDstAdd;
    HI_U8                    u8Opcode;
    HI_UNF_CEC_Operand_t     unOperand;   
}HI_UNF_HDMI_CEC_CMD_S;

typedef struct hiUNF_HDMI_CEC_STATUS_S
{
    HI_BOOL bEnable;                                 /*the flag of CEC work,HI_TRUE:CEC work enable,HI_FALSE:CEC no work ,other parameter no effect*/ /**CNcomment:< CEC �����������λ�����ΪHI_TRUE,CEC�����������������ΪHI_FASLE,CEC������������Ч�����Ҳ��ܹ��� */
    HI_U8   u8PhysicalAddr[4];                       /*CEC physics address*/ /**CNcomment:< CEC �����ַ */
    HI_U8   u8LogicalAddr;                           /*CEC logic address,defualt 0x03*//**CNcomment:< CEC �߼���ַ��Ĭ��Ϊ��0x03. */
    HI_U8   u8Network[HI_UNF_CEC_LOGICALADD_BUTT];   /*CEC network struct ,1:the device can response CEC command*/ /**CNcomment:< CEC ��������·�ṹ��Ϊ1��ʾ���豸�ܹ���ӦCEC���� */
}HI_UNF_HDMI_CEC_STATUS_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      H_1_2_7 */
/** 
\brief the whole initialization of the hdmi. CNcomment:HDMI�ӿ����������ʼ��
\attention  this func should be called before vo_init and after disp_init. CNcomment:������DISP����Setup֮���VO����Setup֮ǰ��
\param[in] pstHdmiPara  the initial params. CNcomment:��ʼ������.
\the memeber: pfnHdmiEventCallback of HI_UNF_HDMI_INIT_PARA_S should be inited in one thread. and only be effectived in the first thread,other process can't get the callback event. CNcomment:�Ӳ���:pfnHdmiEventCallback ��hdmi�ص��¼��ڶ��߳���ֻ�е�һ�����ý�����Ч���������̲��ܻ�ûص��¼���
\retval HI_SUCCESS  success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Init(HI_UNF_HDMI_INIT_PARA_S *pstHdmiPara);


/** 
\brief deinit the hdmi. CNcomment:HDMI�ӿ�ȥ��ʼ��
\attention  this must be called after vo exited and before disp  exited . CNcomment:������VO����Exit֮���DISP����Exit֮ǰ��
\param CNcomment:��
\retval HI_SUCCESS      success.CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_DeInit(void);


/** 
\brief create hdmi device. CNcomment:����HDMI�豸
\attention \n
\param[in] enHdmi  hdmi device id.CNcomment:Ҫ�򿪵�HDMI�豸 
\retval HI_SUCCESS     success.  CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi);


/** 
\brief close the handler created by  HI_UNF_HDMI_Open. CNcomment:������HI_UNF_HDMI_Open�����ľ��
\attention \n
\param[in] enHdmi  hdmi device id. CNcomment:HDMI�豸ID
\retval HI_SUCCESS     success.  CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi);


/** 
\brief to get the capability of sink connect to stbbox. CNcomment:��ѯ��ȡHDMI Sink�豸��������
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI�豸ID
\param[out] pCapability  the capability of the sink .CNcomment:SINK������
\retval HI_SUCCESS       success.  CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi. CNcomment:��ο�MPI������
\see ::HI_UNF_HDMI_SINK_CAPABILITY_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_SINK_CAPABILITY_S *pCapability);


/** 
\brief set the attr of given hdmi interface .CNcomment:����HDMI�ӿ����ԡ�
\attention \n
\param[in] enHdmi      hdmi device id.CNcomment:HDMI�豸ID
\param[in] pstAttr     the attr of given hdmi interface.CNcomment:HDMI�ӿ�����
\retval HI_SUCCESS     success.  CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see ::HI_UNF_HDMI_ATTR_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr);


/** 
\brief get the current attr of the give hdmi interface. CNcomment:��ѯHDMI�ӿڵ�ǰ����
\attention \n
\param[in] enHdmi hdmi device id.CNcomment: HDMI�豸ID
\param[out] pstAttr     the attr of given hdmi interface. CNcomment:HDMI�ӿ�����
\retval HI_SUCCESS      success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see HI_UNF_HDMI_ATTR_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr);


/** 
\brief get the cec working status. CNcomment:��ȡCEC״̬
\attention  user can get the cec working status through this func. CNcomment:�ͻ�����ͨ���ýӿڻ�ȡCEC����״̬\n
\param[in] enHdmi       hdmi device id.CNcomment:HDMI�豸ID
\param[in] pStatus     get cec working status. CNcomment:CEC ״̬
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus);


/** 
\brief send the cec data.CNcomment:����CEC ����
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI�豸ID
\param[in] pCECCmd      the cec cmd data.CNcomment:Cec Command ����
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd);


/** 
\brief get the cec data received. CNcomment:���յ�CEC ����
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI�豸ID
\param[in] pCECCmd    the cec cmd data. CNcomment:Cec Command ����
\param[in] timeout    timeout for getting cec cmd, unit: 10ms;
\retval HI_SUCCESS      success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see ::HI_UNF_HDMI_CEC_CMD_S\n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout);

/** 
\brief enable the cec func
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI�豸ID
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_E enHdmi);

/** 
\brief disable the cec func
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI�豸ID
\retval HI_SUCCESS      success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_CEC_Disable(HI_UNF_HDMI_ID_E enHdmi);

/** 
\brief  set and send infoframe. CNcomment:���ò�����InfoFrame
\attention \n
\param[in] enHdmi       hdmi device id.CNcomment:HDMI�豸ID
\param[in] pstInfoFrame the inforframe content.CNcomment:InfoFrame����
\retval HI_SUCCESS      success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);

/** 
\brief get the infoframe infor. CNcomment:��ȡ���ڷ��͵�InfoFrame����Ϣ
\attention \n
\param[in] enHdmi       hdmi device id. CNcomment:HDMI�豸ID
\param[in] enInfoFrameType the info frame type such as avi or audio or gcp etc. CNcomment:InfoFrame����
\param[in] pstInfoFrame   the inforframe content.CNcomment:InfoFrame����
\retval HI_SUCCESS      success.CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);



/** 
\brief  start the hdmi works.
\attention \n
this should be called after HI_UNF_HDMI_SetAttr.
CNcomment:����HDMI�ӿڡ�һ����HI_UNF_HDMI_SetAttr֮�����
\param[in] enHdmi   hdmi device id.CNcomment:HDMI�豸ID
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi);

/** 
\brief stop  the hdmi.
\attention \n
this func should be called  when hdmi plug out.
CNcomment:��HDMI�߱��γ���HDMI�ص�����Ӧ�õ��øú�����
\param[in] enHdmi  hdmi device id. CNcomment:HDMI�豸ID
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi);

/** 
\brief set the deep color mode .CNcomment:����HDMI DeepColorģʽ
\attention \n
\param[in] enHdmi   hdmi device id. CNcomment:HDMI�豸ID
\param[in] enDeepColor deep color mode,please refer to the HI_UNF_HDMI_DEEP_COLOR_E definiton.CNcomment:DeepColorģʽ����ο�::HI_UNF_HDMI_DEEP_COLOR_E  
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor);

/** 
\brief switch the xvycc on or off. CNcomment:����HDMI xvYCC ģʽ
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI�豸ID
\param[in] bEnalbe   whether to enable xvycc mode or not .CNcomment:�Ƿ�ʹ��xvYCCģʽ
\retval HI_SUCCESS    success.  CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnalbe);

/** 
\brief switch the mute of av  on or off. CNcomment:����HDMI AV mute ģʽ
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI�豸ID
\param[in] bAvMute   whether to mute the av.CNcomment:�Ƿ�mute AV
\retval HI_SUCCESS     success. CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
��
*/
HI_S32 HI_UNF_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute);

/** 
\brief get the edid information forcelly.CNcomment:ǿ�ƻ�ȡEDIDԭʼ����,�ýӿ�Ϊ�ض�ƽ̨��Ҫ��ȡԭʼEDID�Ľӿ�
\attention \n
\param[in] enHdmi   hdmi device id.CNcomment:HDMI�豸ID
\param[in] *u8Edid  the buffer allocated externally, buffer size must be 512. CNcomment:��ȡԭʼEDID����buffer��EDID�汾��ͬ�������������ݲ�ͬ��ȡ����buffer ��СΪ512,�û����Լ������;
\param[in] *u32EdidLength  the data lenth of  original edid. CNcomment:��ȡԭʼEDID���ݳ���
\retval HI_SUCCESS   success.   CNcomment:�ɹ�
\retval please refer to the err code definitino of mpi.CNcomment:��ο�MPI������
\see \n
CNcomment:��
*/
HI_S32 HI_UNF_HDMI_Force_GetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength);

/** @} */  /** <!-- ==== API declaration end ==== */
HI_S32 HI_UNF_HDMI_ReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength);

#ifdef __cplusplus
}
#endif
#endif /* __HI_UNF_HDMI_H__ */

