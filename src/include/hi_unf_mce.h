/******************************************************************************

  Copyright (C), 2001-2011, HiSilicon Technologies Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_pvr.h
  Version       : Initial Draft
  Author        : HiSilicon multimedia software group
  Created       : 2011-xx-xx
  Description   : hi_unf_mce.h header file
  History       :
  1.Date        : 2011/xx/xx
    Author      : 
    Modification: This file is created.

******************************************************************************/
/**
 * \file
 * \brief Describes the logo and play contents and related configuration information.
 */
#ifndef __HI_UNF_MCE_H__
#define __HI_UNF_MCE_H__

#include "hi_unf_common.h"
#include "hi_unf_disp.h"
#include "hi_unf_vo.h"
#include "hi_unf_ecs.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif  /* End of #ifdef __cplusplus */

/********************************Struct Definition********************************/
/** \addtogroup      H_2_2_12 */
/** @{ */  /** <!-- [MCE CONFIG] */

/** The name of the fastplay area must be the same as that in the flash memory.*/
#define MTD_FASTPLAY_NAME  "fastplay"
#define MTD_LOGO_NAME "logo"
#define MTD_BASEPARAM_NAME "baseparam"
/**<The fastplay area is divided into five parts:
CNcomment: fastplay ���򱻻���Ϊ5������:
 	1, base param part,  8KB
 	2, logo param part,  8KB
 	3, logo data part,   XKB
 	4, play param part,  8KB
 	5, play data part,	fastplay total size - 24KB - XKB
 */
#define PART_DEFT_PARA_SIZE		0x2000


/** play attr*//**<CNcomment: ��������*/
typedef enum	hiUNF_PIC_PLAYMODE
{
    HI_UNF_PIC_PLAYMODE_LOOP,  	/** Play loop*//** <CNcomment:ѭ��*/
    HI_UNF_PIC_PLAYMODE_ONE,   	/** Play one time*//** <CNcomment:һ��*/
    HI_UNF_PIC_PLAYMODE_BYTIME, /** Play loop by time*//** <CNcomment:ͨ��ʱ��ѭ��*/
    HI_UNF_PIC_PLAYMODE_BUTT
}HI_UNF_PIC_PLAYMODE;

/** Common attributes *//** <CNcomment:�������� */
typedef struct hiUNF_PIC_COMMINFO
{
    HI_U32 		            uCount;         	/**Number of pictures*//** <CNcomment:ͼƬ����*/
    HI_U32                  u32Xpos;        	/**The x start postion in canvas*//** <CNcomment:��canvas�е�x��ʼλ��*/
    HI_U32                  u32Ypos;        	/**The y start postion in canvas*//** <CNcomment:��canvas�е�y��ʼλ��*/
    HI_U32                  u32Width;        	/**The display width in canvas*//** <CNcomment:��canvas�е���ʾ��� */
    HI_U32                  u32Height;        	/**The display height in canvas*//** <CNcomment:��canvas�е���ʾ�߶� */
    HI_U32                  BGColor;         	/**Background color*//** <CNcomment:������ɫ*/  
    HI_UNF_PIC_PLAYMODE     PlayMode;           /**Play mode*//** <CNcomment:����ģʽ*/
    HI_U32                  uLoopCount;         /**Loopcount,0xfffffff means play util user stop it*//** <CNcomment:ѭ��������loopcount,0xfffffff means play util user stop it*/
    HI_U32 	                uPlaySecond;        /**Time of play(second)*//** <CNcomment:����ʱ�� time for play(second)*/
    HI_S8                   chVersion[8];

    HI_U32		            u32ScreenXpos;		/*Screen X pos*//*CNComment: ��ĻX����*/
	HI_U32		            u32ScreenYpos;		/*Screen Y pos*//*CNComment: ��ĻY����*/
	HI_U32		            u32ScreenWidth;		/*Screen Width*//*CNComment: ��Ļ���*/
	HI_U32		            u32ScreenHeight;	/*Screen Height*//*CNComment: ��Ļ�߶�*/
	HI_U32		            u32ScreenBGColor;	/*Screen Background color*//*CNComment: ��Ļ����ɫ*/
    
 }HI_UNF_PIC_COMMINFO;

/** Picture attributes *//** <CNcomment:ͼƬ����*/
typedef struct hiUNF_PIC_PICTURE
{
	HI_U32          u32FileAddr;	/**Physical address*//** <CNcomment:�����ַ*/
	HI_U32          u32FileLen;		/**File length*//** <CNcomment:�ļ���С*/
	HI_U32          u32Xpos;    	/**x start position*//** <CNcomment:x ��ʼλ��*/
	HI_U32          u32Ypos;		/**y start position*//** <CNcomment:y ��ʼλ��*/
	HI_U32          u32Width;		/**width*//** <CNcomment:���*/
	HI_U32          u32Height;		/**height*//** <CNcomment:�߶�*/
	HI_U32          u32DelayTime;	/**Delay time*//** <CNcomment:�ӳ�ʱ�� */
}HI_UNF_PIC_PICTURE;


/**Base config information*//** <CNcomment:����������Ϣ*/
typedef struct hiUNF_MCE_BASE_PARAM_S
{
	HI_UNF_ENC_FMT_E hdFmt;				/**<High-definition (HD) video format*//**<CNcomment:������Ƶ��ʽ */
	HI_UNF_ENC_FMT_E sdFmt;				/**<Standard-definition (SD) video format*//**<CNcomment:������Ƶ��ʽ  */
	HI_UNF_DISP_INTF_TYPE_E hdIntf;		/**<HD channel type*//**<CNcomment:����ͨ������*/
  	HI_UNF_DISP_INTF_TYPE_E sdIntf;		/**<SD channel type*//**<CNcomment:����ͨ������*/
	HI_UNF_DISP_INTERFACE_S DacMode;	/**<Digital-to-analog converter (DAC) mode*//**<CNcomment:DAC ģʽ */
	HI_U32 layerformat;					/**Layer pixel format*//**<CNcomment:ͼ�����ظ�ʽ*/
	HI_U32 inrectwidth;					/**Input width*//**<CNcomment:������ */
	HI_U32 inrectheight;				/**Input height*//**<CNcomment:����߶�*/
	
	HI_U32 u32Top;
	HI_U32 u32Left;
	
	unsigned int u32HuePlus;
	unsigned int u32Saturation;
	unsigned int u32Contrast;
	unsigned int u32Brightness;
	HI_UNF_DISP_BG_COLOR_S enBgcolor;
	HI_UNF_ASPECT_RATIO_E enAspectRatio;
	HI_UNF_ASPECT_CVRS_E enAspectCvrs;
	HI_UNF_DISP_MACROVISION_MODE_E enDispMacrsn;
	
}HI_UNF_MCE_BASE_PARAM_S;


/**Logo configuration parameters*//** <CNcomment: logo ������Ϣ */
typedef struct hiUNF_MCE_LOGO_PARAM_S
{
	HI_U32 logoflag;          			/**<Is logo display or not,0:not 1:yes*//**<CNcomment:logo��ʾ��־��0������ʾ��1����ʾ*/
	HI_U32 contentLen;					/**<Size of the logo data*//**<CNcomment:logo data �Ĵ�С*/
	//HI_U32 dispTime;					/**<Display time*//**<CNcomment:��ʾʱ��*/
	unsigned int u32XPos;				/**<Display x position offset*//**<CNcomment: ��ʾx ����ƫ����*/
	unsigned int u32YPos;				/**<Display y position offset*//**<CNcomment: ��ʾy ����ƫ����*/
	unsigned int u32OutWidth;			/**<Display output width*//**<CNcomment: ��ʾͼƬ����Ŀ��*/
	unsigned int u32OutHeight;			/**<Display output height*//**<CNcomment: ��ʾͼƬ����ĸ߶�*/
	unsigned int u32BgColor;			/**<Display output backgrand color*//**<CNcomment: ��ʾͼƬ����ı���ɫ*/
}HI_UNF_MCE_LOGO_PARAM_S;

/*fastplay supports the decoding mode of audio output (AO)*//**<CNcomment:fastplay ֧�ֵ�AO�����ʽ */
typedef enum hiUNF_MCE_ADEC_TYPE_E
{
	HI_UNF_MCE_ADEC_TYPE_MP2 = 0,		/**<MP2 format*//**<CNcomment:MP2 ��ʽ */
	HI_UNF_MCE_ADEC_TYPE_MP3 = 1,		/**<MP3 format*//**<CNcomment:MP3 ��ʽ */
	HI_UNF_MCE_ADEC_TYPE_BUTT			/**<Invalid format*//**<CNcomment:��Ч ��ʽ */
}HI_UNF_MCE_ADEC_TYPE_E;

/**fastplay supports the decoding mode of video output (VO)*//**<CNcomment:fastplay ֧�ֵ�VO�����ʽ */
typedef enum hiUNF_MCE_VDEC_TYPE_E
{
	HI_UNF_MCE_VDEC_TYPE_MPEG2 = 0,		/**<MPEG2 format*//**<CNcomment:MPEG2  ��ʽ */
	HI_UNF_MCE_VDEC_TYPE_MPEG4 = 1,		/**<MPEG4 format*//**<CNcomment:MPEG4  ��ʽ */
	HI_UNF_MCE_VDEC_TYPE_H264  = 2,		/**<H264 format*//**<CNcomment:H264  ��ʽ */
	HI_UNF_MCE_VDEC_TYPE_AVS   = 3,		/**<AVS format*//**<CNcomment:AVS   ��ʽ */
	HI_UNF_MCE_VDEC_TYPE_BUTT			/**<Invalid format*//**<CNcomment:��Ч ��ʽ */
}HI_UNF_MCE_VDEC_TYPE_E;



/**Parameters for digital video broadcasting (DVB) play configuration*//**<CNcomment:DVB������Ϣ  */
typedef struct hiUNF_MCE_DVB_PARAM_S
{
	HI_U32 PcrPid;						/**<Program clock reference (PCR) packet ID (PID)*//**<CNcomment:pcr pid*/
	HI_U32 u32PlayTime;				    /*<Play time *//*<CNcomment:����ʱ�� */
	HI_U32 VElementPid;					/**<Video PID*//**<CNcomment:��Ƶ pid*/
	HI_U32 AElementPid;					/**<Audio PID*//**<CNcomment:��Ƶ pid*/
	HI_UNF_MCE_VDEC_TYPE_E VideoType;	/**<Video type*//**<CNcomment:��Ƶ ����*/
	HI_UNF_MCE_ADEC_TYPE_E AudioType;	/**<Audio type*//**<CNcomment:��Ƶ ����*/
	HI_U32 tunerId;						/**<Tuner ID. The default value is 0.*//**<CNcomment:tuner Id ��Ĭ��ֵ��0*/
	HI_U32 tunerFreq;					/**<Tuner frequency*//**<CNcomment:tuner Ƶ��*/
	HI_U32 tunerSrate;					/**<tuner symbol rate*/
	HI_U32 tunerQam;					/**<Tuner quadrature amplitude modulation (QAM)*//**<CNcomment:tuner QAM */
	HI_U32 volume;						/**<volume of output*/
	HI_UNF_TRACK_MODE_E trackMode;		/**<Track mode. Only HI_UNF_TRACK_MODE_STEREO is supported.*//**<CNcomment:��֧��������*/
	HI_UNF_VO_DEV_MODE_E devMode;		/**Working mode of the VO device*//**<CNcomment:dev �� vo ģʽ  */
    HI_UNF_TUNER_DEV_TYPE_E          tunerDevType;
	HI_UNF_DEMOD_DEV_TYPE_E          demoDev;
	HI_UNF_TUNER_I2cChannel_E        I2cChannel; 
}HI_UNF_MCE_DVB_PARAM_S;

/**Play parameter configuration of transport stream (TS) files*//**<CNcomment:TS�ļ�������Ϣ */
typedef struct hiUNF_MCE_TSFILE_PARAM_S
{
	HI_U32 contentLen;					/**File length*//** <CNcomment:�ļ����� */
	HI_BOOL IsCycle;					/*Default cycle*//**<CNcomment:�Ƿ�ѭ����Ĭ��Ϊѭ��*/
	HI_U32   u32PlayTime;				/**<Play time *//**<CNcomment:����ʱ�� */
	HI_U32 PcrPid;						/**<pcr pid*/
	HI_U32 VElementPid;					/**<Video PID*//**<CNcomment:��Ƶ pid*/	
	HI_U32 AElementPid;					/**<Audio PID*//**<CNcomment:��Ƶ pid*/
	HI_UNF_MCE_VDEC_TYPE_E VideoType;	/**<Video type*//**<CNcomment:��Ƶ ����*/
	HI_UNF_MCE_ADEC_TYPE_E AudioType;	/**<Audio type*//**<CNcomment:��Ƶ ����*/
	HI_U32 volume;						/**volume of output */
	HI_UNF_TRACK_MODE_E trackMode;		/**<Track mode. Only HI_UNF_TRACK_MODE_STEREO is supported.*//**<CNcomment:��֧��������*/
	HI_UNF_VO_DEV_MODE_E devMode;		/**vo mode of dev  *//**<CNcomment:dev �� vo ģʽ  */
}HI_UNF_MCE_TSFILE_PARAM_S;


/**Parameter configuration of animation*//**<CNcomment:animationģʽ������Ϣ */
typedef struct hiUNF_MCE_GPHFILE_PARAM_S
{
	HI_UNF_PIC_COMMINFO comInfo;	   /**<Animation common information*/
	HI_UNF_PIC_PICTURE	*ppicInfo;	   /**<Animation picture display information*/
}HI_UNF_MCE_GPHFILE_PARAM_S;


/**Play type*//**<CNcomment:��������*/
typedef enum hiUNF_MCE_PLAY_TYPE_E
{
	HI_UNF_MCE_TYPE_PLAY_DVB	 = 0, 	/**<DVB type*/ /**<CNcomment:DVB ���� */ 
	HI_UNF_MCE_TYPE_PLAY_TSFILE  = 1,   /**<TS file type*/ /**<CNcomment:ts �ļ����� */ 
	HI_UNF_MCE_TYPE_PLAY_GPHFILE = 2,  	/**<Image animation file type*//**<CNcomment:animation ���� */ 
	HI_UNF_MCE_TYPE_PLAY_BUTT  			/**<Invalid type *//**<CNcomment:��Ч���� */
}HI_UNF_MCE_PLAY_TYPE_E;


/**The fastplay part that will be operated.*//**<CNcomment:fastplay������Ϣ */
typedef enum hiUNF_PARTION_TYPE_E
{
	HI_UNF_PARTION_TYPE_BASE  = 0,  	 /**<Configuration related to video display*//**<CNcomment:base ������Ϣ */ 
	HI_UNF_PARTION_TYPE_LOGO  = 1,  	 /**<Logo configuration and logo data*//**<CNcomment:logo ������Ϣ������ */
	HI_UNF_PARTION_TYPE_PLAY  = 2, 	 	/**<Play configuration and play data*//**<CNcomment:play ������Ϣ������*/
	HI_UNF_PARTION_TYPE_BUTT  = 3		/**<Invalid operation *//**<CNcomment:��Ч���� */
}HI_UNF_PARTION_TYPE_E;


/**Play configuration*//**<CNcomment:����������Ϣ */
typedef struct hiUNF_MCE_PLAY_PARAM_S
{
	HI_UNF_MCE_PLAY_TYPE_E playType;				/**<Play type*//**<CNcomment:��������*/
	HI_U32 fastplayflag;                            /*The fastplay is play or not. 0: not play, 1:play*//**<CNcomment:�Ƿ񲥷� ;0 :������, 1:����*/
	union 
	{
		HI_UNF_MCE_DVB_PARAM_S     dvbParam;		/**<DVB configuration *//**<CNcomment:DVB ������Ϣ */
		HI_UNF_MCE_TSFILE_PARAM_S  tsFileParam;	    /**<TS configuration *//**<CNcomment:ts ������Ϣ */
		HI_UNF_MCE_GPHFILE_PARAM_S gphFileParam;	/**<Animation configuration *//**<CNcomment:animation ������Ϣ */
 	}param;
}HI_UNF_MCE_PLAY_PARAM_S;

/**< MCE stop mode */
typedef enum hiUNF_MCE_STOPMODE_E
{
    HI_UNF_MCE_STOPMODE_STILL = 0,  /**< MCE stop with last frame  */ /**<CNcomment: stop�������һ֡*/
    HI_UNF_MCE_STOPMODE_BLACK = 1,  /**< MCE stop with black frame */ /**<CNcomment: stop�����*/
    HI_UNF_MCE_STOPMODE_BUTT
} HI_UNF_MCE_STOPMODE_E;

/**< MCE stop parameter*/
typedef struct hiUNF_MCE_STOPPARM_S
{
	HI_UNF_MCE_STOPMODE_E enStopMode; /**< MCE stop mode */ /**<CNcomment: fastplay ֹͣģʽ��0 ��֡��1 ���� */
	HI_HANDLE hNewWin;   				/**< MCE stop with still mode ,the need user create new windou handle */ /**<CNcomment: */
	HI_U32 u32PlayTime;					/**< MCE least playing time before stop . unit: s.if set u32PlayTime parameter available ,then need set  u32PlayCount to 0.*/ /**<CNcomment: ����������Ƶ���ٵĲ���ʱ�䣬��λ: �롣 ��ֵ��Ϊ����ʱ��u32PlayCount������Ϊ0*/
	HI_U32 u32PlayCount;				/**< MCE least playing count before stop. unit: cycle .if set u32PlayCount parameter available ,then need set u32PlayTime to 0 */ /**<CNcomment: ����������Ƶ�Ĳ��ŵ����ٴ�������λ: �Ρ���ֵ��Ϊ����ʱ��u32PlayTime������Ϊ0 */
} HI_UNF_MCE_STOPPARM_S;

/**< MCE is Ended parameter*/
typedef struct hiUNF_MCE_PLAYCURSTATUS_S
{
	HI_BOOL isEnded; /**< whether MCE is ended */ /**<CNcomment: fastplay �Ƿ���ֹͣ */
       HI_U32   playedTime; /**< how much time MCE has played*/ /**<CNcomment: fastplay �Ѿ����ŵ�ʱ�䳤�� */
} HI_UNF_MCE_PLAYCURSTATUS_S;

/** @} */  /** <!-- ==== Struct Definition end ==== */

/********************************API declaration********************************/
/** \addtogroup      H_1_2_10 */
/** @{ */  /** <!-- [MCE CONFIG]*/

/**
\brief  clear boot logo  CNcomment:��տ���logo
\attention \n
this use to clear boot logo when  noly boot logo change to video 
CNcomment: ֻ���տ���logo ������£�����Ӧ����Ƶǰ������տ���logo
\param N/A
\retval ::HI_SUCCESS 
\retval ::HI_FAILURE 
\see \n
N/A
*/
HI_S32 HI_UNF_MCE_ClearLogo(HI_VOID);

/**
\brief  stop fastplay and destrory fastplay source CNcomment:ֹͣfastplay,����fastplay��Դ
\attention \n
this function realized stop fastplay with black frame or still frame manner.can be set u32PlayTime or u32PlayCount begore stop.
if set them nonzero at the same time that is invalid .if set them to zore ,then call this functiom with no block.  if you want to realize
change to user app  smoothly, you need input avalid hNewWin handle parameter.
CNcomment: ���ñ��ӿڿ�ʵ��fastplay ��Ƶ���ŵ�ֹͣ������ѡ��ֹ֡ͣ��Ҳ����ѡ�����ֹͣ��
��������ֹͣ����ǰ����̲���ʱ��u32PlayTime����Сѭ������u32PlayCount�����߶���Ϊ��0 ֵ��Ч��
���߶���Ϊ0��ʾ��������������ֻ��������һ����Ч�����Ҫʵ�� ��֡ƽ���л���
��Ҫ�û�������Ч��hNewWin  ���.
\param[in]  pstStopParam  Fastplay  stop parameter. 
\retval ::HI_SUCCESS 
\retval :: HI_FAILURE   			Failure
\retval ::HI_ERR_MCE_PARAM_INVALID  Parameter invalid 
\retval ::HI_ERR_MCE_PTR_NULL	NULL pointer
\see \n
N/A
*/
HI_S32 HI_UNF_MCE_Stop(HI_UNF_MCE_STOPPARM_S *pstStopParam);

/**
\brief  get current fastplay status  CNcomment:�õ���ǰFastPlay����״̬
\attention \n
\param[out]  pPlayCurStatus current fastplay status.  CNcomment:Fastplay��ǰ�Ĳ���״̬
\retval ::HI_SUCCESS 
\retval :: HI_FAILURE   			Failure
\see \n
N/A
*/
HI_S32 HI_UNF_MCE_GetPlayCurStatus(HI_UNF_MCE_PLAYCURSTATUS_S *pPlayCurStatus);

/** 
\brief init mce  CNcomment:MCE���ó�ʼ���ӿ�
\attention \n
Initializes the media control engine (MCE) for preparing other operations.
CNcomment:ʹ��MCE���ù������ȵ��øýӿ�
\param[in] PartType  Part to be operated.  CNcomment:�������漰���ķ���
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_OPEN_ALREADY  The MCE is already opened.  CNcomment:�豸�Ѿ���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid.  CNcomment:��������
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û��Ĭ�Ϸ���
\retval ::HI_ERR_MCE_MTD_INFO      The size is insufficient for loading data. CNcomment:ʵ�ʷ����ߴ粻���������漰�Ĵ�С
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_Init(HI_UNF_PARTION_TYPE_E  PartType);

/** 
\brief deinit mce  CNcomment:MCE����ȥ��ʼ���ӿ�
\attention \n
Destroys all MCE resources.  CNcomment:���MCE���ù������յ��øýӿ�
\param N/A
\retval N/A
\see \n
N/A
*/
HI_VOID HI_UNF_MCE_deInit(HI_VOID);

/** 
\brief Obtains the basic configuration information.  CNcomment:��ȡBASE��������Ϣ
\attention \n
N/A
\param[in] baseParam   Pointer to the basic parameters.  CNcomment:����ָ��
\retval ::HI_SUCCESS    Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_READ      The read operation fails. CNcomment:������ȡ����
\retval ::HI_ERR_MCE_BASE_INFO     Get information fails. CNcomment:�ɷ����л�ȡ��Ϣ����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetBaseParam(HI_UNF_MCE_BASE_PARAM_S *baseParam);


/** 
\brief Modifies the basic configuration information.  CNcomment:����BASE��������Ϣ
\attention \n
N/A
\param[in] baseParam    Pointer to the basic parameters.  CNcomment:����ָ��
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_BASE_INFO     The config information is error.  CNcomment:������Ϣ����
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_UpdateBaseParam(HI_UNF_MCE_BASE_PARAM_S *baseParam);

/** 
\brief Obtains the basic configuration information from the flash memory.  
CNcomment:��ȡLOGO��������Ϣ
\attention \n
N/A
\param[in] logoParam   Pointer to the logo parameter.  CNcomment:����ָ��
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails.  CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_READ      The read operation fails.  CNcomment:������ȡ����
\retval ::HI_ERR_MCE_LOGO_INFO     Get information from partition error. CNcomment:�ɷ����л�ȡ��Ϣ����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetLogoParam(HI_UNF_MCE_LOGO_PARAM_S *logoParam);

/** 
\brief Modifies the configuration of the logo parameter.  CNcomment:����LOGO��������Ϣ
\attention \n
Update logo parameter, you must invoke this function befor update data of logo
\CNcomment:����logo�������ڸ���logo����ǰ�����ȵ��ô˺���
\param[in] logoParam   Pointer to the logo parameter  CNcomment:����ָ��
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_LOGO_INFO     The config infogmation is error.  CNcomment:������Ϣ����
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_UpdateLogoParam(HI_UNF_MCE_LOGO_PARAM_S *logoParam);


/** 
\brief Obtains the logo data from the flash memory. CNcomment:��ȡLOGO��������Ϣ
\attention \n
N/A
\param[in] content Pointer to the logo data that is written from the flash memory. CNcomment:����bufָ��
\param[in] size    Size of the logo data to be written.  CNcomment:���ݳ���
\retval ::HI_SUCCESS  Success CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid. CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_READ      The read operation fails. CNcomment:������ȡ����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetLogoContent(HI_U8 *content, HI_U32 size);


/** 
\brief Modifies the logo data.  CNcomment:����LOGO��������Ϣ
\attention \n
Update data in logo partition, You must invoke HI_UNF_MCE_UpdateLogoParam to update logo parameter before invoke this function
\CNcomment:����logo���ݣ��ڴ˺���֮ǰ�����ȵ���HI_UNF_MCE_UpdateLogoParam��������logo����
\param[in] content  Pointer to the logo data that is read from the flash memory  CNcomment:����bufָ��
\param[in] size     Size of the logo data to be read  CNcomment:���ݳ���
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid.  CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails.  CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails.  CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails.  CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_UpdateLogoContent(HI_U8 *content, HI_U32 size);

/** 
\brief Obtains the playing configuration information.  CNcomment:��ȡPLAY��������Ϣ
\attention \n
N/A
\param[in]  playParam   Playing configuration information  CNcomment:����ָ��
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_READ      The read operation fails. CNcomment:������ȡ����
\retval ::HI_ERR_MCE_PLAY_INFO     Get information fails.  CNcomment:�ɷ����л�ȡ��Ϣ����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails.  CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetPlayParam(HI_UNF_MCE_PLAY_PARAM_S *playParam);

/** 
\brief   Modifies the playing configuration information.  CNcomment:����PLAY��������Ϣ
\attention \n 
Update play parameter, you must invoke this function befor update data of play
\CNcomment:����play�������ڸ���play����ǰ�����ȵ��ô˺���
\param[in]  playParam  Playing configuration information  CNcomment:����ָ��
\retval ::HI_SUCCESS    Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PLAY_INFO     The config information is invalid. CNcomment:������Ϣ����
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_UpdatePlayParam(HI_UNF_MCE_PLAY_PARAM_S *playParam);

/** 
\brief Obtains the data being playing from the flash memory.  CNcomment:��ȡPLAY��������Ϣ
\attention \n
N/A
\param[in] content  Pointer to the data that is read from the flash memory  CNcomment:����bufָ��
\param[in] size     Size of the data being played that is read from the flash memory  CNcomment:���ݳ���
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID  The parameter is invalid. CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_READ      The read operation fails. CNcomment:������ȡ����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetPlayContent(HI_U8 *content, HI_U32 size);


/** 
\brief Updates the playing data part. CNcomment:����PLAY��������Ϣ
\attention \n
Update data in play partition, You must invoke HI_UNF_MCE_UpdatePlayParam to update plat parameter before invoke this function
\CNcomment:����play���ݣ��ڴ˺���֮ǰ�����ȵ���HI_UNF_MCE_UpdatePlayParam��������play����
\param[in] content  Pointer to the data that is written to the flash memroy.  CNcomment:����bufָ��
\param[in] size    Size of the data being played.  CNcomment:���ݳ���
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid. CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_UpdatePlayContent(HI_U8 *content, HI_U32 size);


/** 
\brief  Enables or disables the logo.  CNcomment:�򿪻��߹ر�logo
\attention \n
N/A
\param[in] bLogoOpen whether to enable Logo. CNcomment:bLogoOpen����ΪHI_TRUEΪ��logo��HI_FALSEΪ�ر�logo
\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid. CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_SetLogoFlag(HI_BOOL bLogoOpen);

/** 
\brief Starts or stops playing. CNcomment:�򿪻��߹ر�Fastplay
\attention \n
N/A
\param[in] bPlayOpen  whether to enable Fastplay.  CNcomment:bLogoOpen����ΪHI_TRUEΪ��Fastplay��HI_FALSEΪ�ر�Fastplay
\retval ::HI_SUCCESS  Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid.  CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails.  CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails.  CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails.  CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_SetFastplayFlag(HI_BOOL bPlayOpen);

/** 
\brief  Obtains the status of the logo.  CNcomment:��ȡlogo�Ĵ򿪻��߹ر�Flag
\attention \n
N/A
\param[out] pbLogoOpen��Flag indicating the status of the logo.  CNcomment:��ȡlogo��(�򿪻�ر�)Flag��ָ��

\retval ::HI_SUCCESS    Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null.  CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist.  CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid.  CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails.   CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails.  CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails.  CNcomment:�����رմ���

\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetLogoFlag(HI_BOOL *pbLogoOpen);

/** 
\brief Obtains the playing status.  CNcomment:��ȡFastplay�Ĵ򿪻��߹ر�Flag
\attention \n
N/A

\param[out] pbPlayOpen      Flag indicating the playing status.  CNcomment:��ȡFastplay��(�򿪻�ر�)Flag��ָ��

\retval ::HI_SUCCESS   Success  CNcomment:�ɹ�
\retval ::HI_ERR_MCE_PTR_NULL      The pointer is null. CNcomment:����ָ��NULL
\retval ::HI_ERR_MCE_MTD_NOFIND    This part does not exist. CNcomment:û�и÷���
\retval ::HI_ERR_MCE_PARAM_INVALID The parameter is invalid. CNcomment:������Ч
\retval ::HI_ERR_MCE_MTD_OPEN      The open operation fails. CNcomment:�����򿪴���
\retval ::HI_ERR_MCE_MTD_WRITE     The write operation fails. CNcomment:����д�����
\retval ::HI_ERR_MCE_MTD_CLOSE     The close operation fails. CNcomment:�����رմ���
\see \n
N/A
*/
HI_S32  HI_UNF_MCE_GetFastplayFlag(HI_BOOL *pbPlayOpen);

/** @} */  /** <!-- ==== API declaration end ==== */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


