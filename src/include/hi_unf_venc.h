/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_venc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/04/07
  Description   :
  History       :
  1.Date        : 2010/04/07
    Author      : j00131665
    Modification: Created file

*******************************************************************************/
/** 
 * \file
 * \brief Describes the information about video encoding (VENC). CNcomment: �ṩVENC�������Ϣ
 */

#ifndef  __HI_UNF_VENC_H__
#define  __HI_UNF_VENC_H__

#include "hi_unf_common.h"
#include "hi_unf_vi.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif

/********************************Macro Definition********************************/
/** \addtogroup      H_2_2_11 */
/** @{ */  /** <!-- ��VENC�� */

/**<Maximum encoding width*/
/**<CNcomment: ��������*/
#define HI_UNF_VENC_MAX_WIDTH       (1920)	
/**<Minimum encoding width*/	
/**<CNcomment: ��С������*/	
#define HI_UNF_VENC_MIN_WIDTH       (96)
/**<Maximum encoding height*/			  
/**<CNcomment: ������߶�*/
#define HI_UNF_VENC_MAX_HEIGTH      (1080)		
/**<Minimum encoding height*/
/**<CNcomment: ��С����߶�*/	
#define HI_UNF_VENC_MIN_HEIGTH      (96)	
/**<Picture size alignment (in bytes)*/		  
/**<CNcomment: ͼ���С�����ֽ�*/
#define HI_UNF_VENC_PIC_SZIE_ALIGN  (16)	
/**<Maximum buffer (in bytes)*/		  
/**<CNcomment: ��󻺴��ֽ�*/
#define HI_UNF_VENC_MAX_BUF_SIZE    (20*1024*1024)	
/**<Minimum buffer (in bytes)*/
/**<CNcomment: ��С�����ֽ�*/ 
#define HI_UNF_VENC_MIN_BUF_SIZE    (256*1024)	
/**<Minimum group of picture (GOP)*/
/**<CNcomment: ��СGOP*/
#define HI_UNF_VENC_MIN_GOP         (1)		
/**<Maximum output bit rate*/		  
/**<CNcomment: ����������*/
#define HI_UNF_VENC_MAX_bps         (42*1024*1024)
/**<Minimum output bit rate*/	
/**<CNcomment: ��С�������*/
#define HI_UNF_VENC_MIN_bps         (32*1024)	
/**<Maximum split size (in bytes)*/	
/**<CNcomment: ���Split�ֽ�*/
#define HI_UNF_VENC_MAX_SPLIT_SIZE  (0xFFFF)	
/**<Minimum split size (in bytes)*/	
/**<CNcomment: ��СSplit�ֽ�*/
#define HI_UNF_VENC_MIN_SPLIT_SIZE  (512)	
/**<Maximum quantization level*/		  
/**<CNcomment: �����������*/
#define HI_UNF_VENC_MAX_Q_VALUE     (99)	
/**<Minimum quantization level*/ 	 
/**<CNcomment: ��С��������*/ 
#define HI_UNF_VENC_MIN_Q_VALUE     (1)	
/**Maximum frame rate*/			  
/**<CNcomment: ���֡��*/
#define HI_UNF_VENC_MAX_fps         (30)
/**Minimum frame rate*/		
/**<CNcomment: ��С֡��*/	  
#define HI_UNF_VENC_MIN_fps         (1)		
/**<Size of reserved bytes for the bit rate*/		 
/**<CNcomment: ���ʱ����ֽڴ�С*/
#define HI_UNF_VENC_STREAM_RESERV_SIZE 48			  
/** @} */  /** <!-- ==== Macro Definition end ==== */

/********************************Struct Definition********************************/
/** \addtogroup      H_2_2_16 */
/** @{ */  /** <!-- ��VENC�� */

/**H.264 NALU type*/
/**CNcomment: H.264NALU���� */
typedef enum hiUNF_H264E_NALU_TYPE_E
{
    HI_UNF_H264E_NALU_PSLICE = 1,       /**<P slice NALU*/
    HI_UNF_H264E_NALU_ISLICE = 5,       /**<I slice NALU*/
    HI_UNF_H264E_NALU_SEI = 6,          /**<SEI NALU*/
    HI_UNF_H264E_NALU_SPS = 7,          /**<SPS NALU*/
    HI_UNF_H264E_NALU_PPS = 8,          /**<PPS NALU*/
    HI_UNF_H264E_NALU_BUTT
} HI_UNF_H264E_NALU_TYPE_E;

/**H.263 NALU type*/
/**CNcomment: H.263NALU���� */
typedef enum hiUNF_H263E_PACK_TYPE_E
{
    HI_UNF_H263E_NALU_PSLICE = 1,       /**<P slice NALU*/
    HI_UNF_H263E_NALU_ISLICE = 5,       /**<I slice NALU*/
    HI_UNF_H263E_NALU_SEI = 6,          /**<SEI NALU*/
    HI_UNF_H263E_NALU_SPS = 7,          /**<SPS NALU*/
    HI_UNF_H263E_NALU_PPS = 8,          /**<PPS NALU*/
    HI_UNF_H263E_NALU_BUTT
} HI_UNF_H263E_PACK_TYPE_E;

/**JPEG package type*/
/**CNcomment: JPEG������� */
typedef enum hiUNF_JPEGE_PACK_TYPE_E
{
    HI_UNF_JPEGE_PACK_ECS = 5,          /**<ECS package*/ /**<CNcomment: ECS ��*/
    HI_UNF_JPEGE_PACK_APP = 6,          /**<APP package*/ /**<CNcomment: APP ��*/
    HI_UNF_JPEGE_PACK_VDO = 7,          /**<VDO package*/ /**<CNcomment: VDO ��*/
    HI_UNF_JPEGE_PACK_PIC = 8,          /**<PIC package*/ /**<CNcomment: PIC ��*/
    HI_UNF_JPEGE_PACK_BUTT
} HI_UNF_JPEGE_PACK_TYPE_E;

/**MPEG4 package type*/
/**CNcomment: MPEG4������� */
typedef enum hiUNF_MPEG4E_PACK_TYPE_E
{
    HI_UNF_MPEG4E_PACK_VO = 1,          /**<VO package*/ /**<CNcomment: VO ��*/
    HI_UNF_MPEG4E_PACK_VOS = 2,         /**<VOS package*/ /**<CNcomment: VOS ��*/
    HI_UNF_MPEG4E_PACK_VOL = 3,         /**<VOL package*/ /**<CNcomment: VOL ��*/
    HI_UNF_MPEG4E_PACK_VOP = 4,         /**<VOP package*/ /**<CNcomment: VOP ��*/
    HI_UNF_MPEG4E_PACK_SLICE = 5        /**<Slice package*/ /**<CNcomment: SLICE ��*/	
} HI_UNF_MPEG4E_PACK_TYPE_E;

/**Data type*/
/**CNcomment: �������� */
typedef union hiUNF_VENC_DATA_TYPE_U
{
    HI_UNF_H264E_NALU_TYPE_E   enH264EType;	/**<H.264 encoding data*/ /**<CNcomment: 264��������*/	
    HI_UNF_H263E_PACK_TYPE_E   enH263EType;	/**<H.263 encoding data*/ /**<CNcomment: 263��������*/	
    HI_UNF_JPEGE_PACK_TYPE_E   enJPEGEType;	/**<JPEG encoding data*/ /**<CNcomment: JPEG��������*/	
    HI_UNF_MPEG4E_PACK_TYPE_E  enMPEG4EType;/**<MPEG4 encoding data*/ /**<CNcomment: MPEG4��������*/		
}HI_UNF_VENC_DATA_TYPE_U;

/*Output stream attributes*/
/**CNcomment: ����������� */
typedef struct hiVENC_STREAM_S
{
    HI_U8                   au8Reserved[HI_UNF_VENC_STREAM_RESERV_SIZE];  /**<inner use, don't touch */
    HI_U8                   *pu8Addr[2];       /**<Stream virtual address. Two addresses are used in cyclic buffer loopback mode.*/	/**<CNcomment: ���������ַ��ѭ��buffer����ʱ��2����ַ*/	
    HI_U32                  u32PhyAddr[2];     /**<Stream physical address. Two addresses are used in cyclic buffer loopback mode.*/ /**<CNcomment: ���������ַ��ѭ��buffer����ʱ��2����ַ*/	
    HI_U32                  u32SlcLen[2];      /**<Stream length*/ /**<CNcomment: ��������*/		
    HI_U32                  u32PTS0;           /**<PTS0*/	
    HI_U32                  u32PTS1;           /**<PTS1*/	
    HI_U32                  bFrameEnd;         /**<Frame end or not*/ /**<CNcomment: �Ƿ�Ϊ֡����*/
    HI_UNF_VENC_DATA_TYPE_U enDataType;        /**Encoding data type*/ /**<CNcomment: ������������*/
}HI_UNF_VENC_STREAM_S;

/*Storage type of the input data*/
/**CNcomment: �������ݴ洢���� */
typedef enum hiUNF_VENC_YUV_TYPE_E
{
    HI_UNF_VENC_YUV_420=0,        /**<YUV420 mode*/ /**<CNcomment: YUV420ģʽ*/
    HI_UNF_VENC_YUV_422,          /**<YUV422 mode*/	/**<CNcomment: YUV422ģʽ*/	
    HI_UNF_VENC_YUV_444,          /**<YUV444 mode*/ /**<CNcomment: YUV444ģʽ*/
    HI_UNF_VENC_YUV_BUTT
}HI_UNF_VENC_YUV_TYPE_E;

/*Storage mode of the input data*/
/**CNcomment: �������ݴ洢ģʽ */
typedef enum hiUNF_VENC_STORE_TYPE_E
{
    HI_UNF_VENC_STORE_SEMIPLANNAR=0,    /**<Semi-planar mode*/ /**<CNcomment: semi plannarģʽ*/
    HI_UNF_VENC_STORE_PLANNAR,          /**<Planar mode*/ /**<CNcomment: plannarģʽ*/
    HI_UNF_VENC_STORE_PACKAGE,          /**<Package mode*/ /**<CNcomment: packageģʽ*/
    HI_UNF_VENC_STORE_BUTT	
}HI_UNF_VENC_STORE_TYPE_E;

/**Storage sequence of package YUV*/
/**CNcomment: package YUV���˳�� */
typedef enum hiUNF_VENC_PACKAGE_SEQ_E
{
    HI_UNF_VENC_PACKAGE_Y0UY1V =0,	    /**<Y0U0Y1V0 mode*/
    HI_UNF_VENC_PACKAGE_Y0UVY1 ,	    /**<Y0U0V0Y1 mode*/
    HI_UNF_VENC_PACKAGE_Y0Y1UV ,	    /**<Y0Y1U0V0 mode*/
    HI_UNF_VENC_PACKAGE_Y0Y1VU ,	    /**<Y0Y1V0U0 mode*/
    HI_UNF_VENC_PACKAGE_Y0VY1U ,	    /**<Y0V0Y1U0 mode*/
    HI_UNF_VENC_PACKAGE_Y0VUY1 ,	    /**<Y0V0U0Y1 mode*/
    
    HI_UNF_VENC_PACKAGE_UY0Y1V ,	    /**<U0Y0Y1V0 mode*/
    HI_UNF_VENC_PACKAGE_UY0VY1 ,	    /**<U0Y0V0Y1 mode*/
    HI_UNF_VENC_PACKAGE_UY1Y0V ,	    /**<U0Y1Y0V0 mode*/
    HI_UNF_VENC_PACKAGE_UY1VY0 ,	    /**<U0Y1V0Y0 mode*/
    HI_UNF_VENC_PACKAGE_UVY1Y0 ,	    /**<U0V0Y1Y0 mode*/
    HI_UNF_VENC_PACKAGE_UVY0Y1 ,	    /**<U0V0Y0Y1 mode*/

    HI_UNF_VENC_PACKAGE_Y1UY0V ,	    /**<Y1U0Y0V0 mode*/
    HI_UNF_VENC_PACKAGE_Y1UVY0 ,	    /**<Y1U0V0Y0 mode*/
    HI_UNF_VENC_PACKAGE_Y1Y0UV ,	    /**<Y1Y0U0V0 mode*/
    HI_UNF_VENC_PACKAGE_Y1Y0VU ,	    /**<Y1Y0V0U0 mode*/
    HI_UNF_VENC_PACKAGE_Y1VY0U ,	    /**<Y1V0Y0U0 mode*/
    HI_UNF_VENC_PACKAGE_Y1VUY0 ,	    /**<Y1V0U0Y0 mode*/

    HI_UNF_VENC_PACKAGE_VY0Y1U ,	    /**<V0Y0Y1U0 mode*/
    HI_UNF_VENC_PACKAGE_VY0UY1 ,	    /**<V0Y0U0Y1 mode*/
    HI_UNF_VENC_PACKAGE_VY1Y0U ,	    /**<V0Y1Y0U0 mode*/
    HI_UNF_VENC_PACKAGE_VY1UY0 ,	    /**<V0Y1U0Y0 mode*/
    HI_UNF_VENC_PACKAGE_VUY1Y0 ,	    /**<V0U0Y1Y0 mode*/
    HI_UNF_VENC_PACKAGE_VUY0Y1 ,	    /**<V0U0Y0Y1 mode*/	
    HI_UNF_VENC_PACKAGE_BUTT	
}HI_UNF_VENC_PACKAGE_SEQ_E;

/**CNcomment: ����ͨ������ */
typedef struct hiUNF_VENC_CHN_ATTR_S
{
    HI_U32                      u32Width;         /**<Width, 16-byte aligned. The width cannot be configured dynamically.*/ /**<CNcomment: ��ȣ�16�ֽڶ���,��֧�ֶ�̬����  */
    HI_U32                      u32Height;        /**<Height, 16-byte aligned. The height cannot be configured dynamically.*/ /**<CNcomment: �߶ȣ�16�ֽڶ���,��֧�ֶ�̬���� */
    HI_UNF_VCODEC_TYPE_E        enVencType;	      /**<Encoder type*/ /**<CNcomment: ���������� */
    HI_U32                      u32StrmBufSize;	  /**<Stream buffer size*/ /**<CNcomment: ����buffer��С */

    HI_UNF_VENC_YUV_TYPE_E      enYuvSampleType;  /**<Storage type. Only the package mode is supported in the formats of 420 422 and 422.*/ /**<CNcomment: �洢���ͣ�420 422, 422��ʽֻ֧��package  */
    HI_UNF_VENC_STORE_TYPE_E    enYuvStoreType;   /**<Storage format. Only the semi-planar format or package format is supported.*/ /**<CNcomment: �洢��ʽ, Ŀǰֻ֧��semiplanar ���� package*/
    HI_U32                      u32RotationAngle; /**<Rotation angle. This parameter cannot be set.*/ /**<CNcomment: ��ת�Ƕ�,��Ч����  */

	HI_UNF_VENC_PACKAGE_SEQ_E   enPackageSel;     /**<Storage sequence of package YUV*/ /**<CNcomment: package YUV���˳��*/ 	
    HI_BOOL                     bSlcSplitEn;      /**<Slice split enable*/ /**<CNcomment: �Ƿ�ʹ�ָܷ�slice */ 	
    HI_U32                      u32SplitSize;     /**<Split size. The size is in the unit of minimum coded unit (MCU) for the JPEG format and byte for the MP4 format. The split size can be ignored in H.263 format.*/ 
                                                  /**<CNcomment: �ָ�Ĵ�С��JPGE����MCUΪ��λ,H264����MP4���ֽ�Ϊ��λ, H263������ */
    HI_U32                      u32Qlevel;        /**<The value ranges from 1 to 99 for the JPGE, and is set to 0 for other formats.*/ /**<CNcomment: JPGEȡֵ��Χ1-99����������Ϊ0 */
    
    HI_U32                      u32TargetBitRate; /**<RC parameter for the VENC. It can be set dynamically.*/ /**<CNcomment: Venc����RC����,�ɶ�̬���� */
    HI_U32                      u32TargetFrmRate; /**<Target frame rate. It can be set dynamically.*/ /**<CNcomment: Ŀ��֡��,�ɶ�̬���� */ 
    HI_U32                      u32InputFrmRate;  /**<Input frame rate. It can be set dynamically. The value of u32TargetFrmRate is less than or equal to the value of u32InputFrmRate.*/ /**<CNcomment: ����֡��,�ɶ�̬����,u32TargetFrmRate <= u32InputFrmRate */ 
    HI_U32                      u32Gop;			  /**<GOP size. It can be set dynamically.*/ /**<CNcomment: GOP��С,�ɶ�̬���� */ 	
	HI_BOOL        				bSendToNet;       /**<Whether to send current data streams over the network. If yes, data streams are sent over the network; if no, data streams are retained.
                                                      You can also obtain streams by calling HI_S32 HI_UNF_VENC_AcquireStream. If you set bSendToNet to enabled, you cannot obtain streams by calling HI_S32 HI_UNF_VENC_AcquireStream. Otherwise, streams cannot be obtained in time and network delay occurs.*/
                                                  /**<CNcomment: �Ƿ��͵����磬 �����������磬���򱣳�����������
                                                     ���û�ͨ���ӿڻ�ȡ��������ͬʱ������·�������û���ȡ�ò���ʱ����������ʱ */
	HI_UNF_VP_NET_PARA_S        stNetCfgPara;     /**<Network parameter. It is valid when bSendToNet is TRUE.*/ /**<CNcomment: ���������bSendToNetΪ��ʱ��Ч */
    
}HI_UNF_VENC_CHN_ATTR_S;


/** @} */  /** <!-- ==== Struct Definition End ==== */

/********************************API declaration********************************/
/** \addtogroup      H_1_2_9 */
/** @{ */  /** <!-- ��VENC�� */

/** 
\brief Initializes the video encoder. CNcomment: ��ʼ����Ƶ������
\attention \n
Before using the VENC, you must call this API. CNcomment: ����VENCģ��Ҫ�����ȵ��øýӿ�
\param[in] N/A CNcomment: ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_DEV_NOT_EXIST No VENC device exists. CNcomment: �豸������
\retval ::HI_ERR_VENC_NOT_DEV_FILE The file is not a VENC file. CNcomment: �ļ����豸
\retval ::HI_ERR_VENC_DEV_OPEN_ERR The VENC device fails to start. CNcomment: ���豸ʧ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_Init(HI_VOID);


/** 
\brief Deinitializes the video encoder. CNcomment: ȥ��ʼ����Ƶ������
\attention \n
N/A CNcomment: ��
\param[in] N/A CNcomment: ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_DEV_CLOSE_ERR The video encoder fails to stop. CNcomment: �رձ�����ʧ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_DeInit(HI_VOID);


/** 
\brief Obtains the default attributes of a VENC channel. CNcomment: ��ȡ����ͨ��Ĭ������
\attention \n
By default, the encoding size is D1, encoding format is H.264, and a frame is a slice.
CNcomment: Ĭ��D1���룬H.264��ʽ��һ֡Ϊһ��Slice
\param[in] pstAttr Pointer to the attributes of a VENC channel. CNcomment: pstAttr ָ�����ͨ�����Ե�ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_NULL_PTR The input pointer parameter is null. CNcomment: ����ָ�����Ϊ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_GetDefaultAttr(HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Creates a VENC channel. CNcomment: ������Ƶ����ͨ��
\attention \n
You must create a VENC channel before video encoding. 
CNcomment: ������Ƶ����Ҫ�����ȴ�������ͨ�� 
\param[in] phVenc Pointer to the handle of a VENC channel. CNcomment: phVenc ָ�����ͨ�������ָ��
\param[in] pstAttr Pointer to the attributes of a VENC channel. CNcomment: pstAttr ָ�����ͨ�����Ե�ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_NULL_PTR The input pointer parameter is null. CNcomment: ����ָ������п�ָ��
\retval ::HI_ERR_VENC_CREATE_ERR The number of VENC channels exceeds the limit. CNcomment: ����ͨ��������
\retval ::HI_ERR_VENC_INVALID_PARA The channel attributes are incorrect. CNcomment: ͨ���������ô���
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\see \n
N/A
CNcomment: ��
*/
HI_S32 HI_UNF_VENC_CreateChn(HI_HANDLE *phVenc, const HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Destroys a VENC channel. CNcomment: ������Ƶ����ͨ��
\attention \n
\param[in] hVenc Handle of a VENC channel. CNcomment: CNcomment: hVenc ����ͨ�����
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No handle exists. CNcomment: ���������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_DestroyChn(HI_HANDLE hVenc);


/** 
\brief Attaches a VENC channel to the video source. CNcomment: �󶨱���ͨ������ƵԴ
\attention \n
You must call this API before performing encoding and obtaining streams. CNcomment: ��ʼ����ͻ�ȡ����֮ǰ��Ҫ���ȵ��øýӿ�
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] hSrc Data source handle CNcomment: hSrc ��ƵԴ���
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_INVALID_PARA The video source is invalid. CNcomment: ��ƵԴ����
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_AttachInput(HI_HANDLE hVenc,HI_HANDLE hSrc);


/** 
\brief Detaches a VENC channel from the video source. CNcomment: �����ƵԴ
\attention \n
You must stop encoding before calling this API.
CNcomment: ���øýӿ���Ҫ����ֹͣ����
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] hSrc Data source handle CNcomment: hSrc ��ƵԴ���
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_INVALID_PARA The video source is invalid. CNcomment: ��ƵԴ����
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_CHN_INVALID_STAT The video encoder does not stop encoding. CNcomment: ������δֹͣ����
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_DetachInput(HI_HANDLE hVenc,HI_HANDLE hSrc);


/** 
\brief Starts to perform encoding. CNcomment: ��ʼ����
\attention \n
You must initialize the video encoder, create a VENC channel, and attach the channel to the video source before calling this API.
CNcomment: ���øýӿ���Ҫ���ȳ�ʼ������������������ͨ��������ƵԴ
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\retval ::HI_SUCCESS Success �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_CHN_NO_ATTACH The VENC channel is not attached to the video source. CNcomment: ����ͨ��û�а󶨵���ƵԴ
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_Start(HI_HANDLE hVenc);


/** 
\brief Stops encoding. CNcomment: ֹͣ����
\attention \n
You must initialize the video encoder, create a VENC channel, attach the channel to the video source, and start to perform encoding before calling this API.
CNcomment: ���øýӿ���Ҫ���ȳ�ʼ������������������ͨ��������ƵԴ���Ѿ���ʼ����
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_CHN_NO_ATTACH The VENC channel is not attached to the video source. CNcomment: ����ͨ��û�а󶨵���ƵԴ
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_Stop(HI_HANDLE hVenc);

/** 
\brief Obtains VENC streams. CNcomment: ��ȡ��Ƶ��������
\attention \n
You must attach a VENC channel to the video source and start to perform encoding before calling this API.
The block time (in ms) is configurable. If u32TimeOutMs is set to 0, the block time is 0; if u32TimeOutMs is set to 0xFFFFFFFF, it indicates infinite wait. \n
If the block time is reached but no data is received, ::HI_ERR_VENC_BUF_EMPTY is returned.\n
If the wait time is 0 and there is no data, ::HI_ERR_VENC_BUF_EMPTY is returned.\n
The non-block mode is not supported. You need to set u32TimeOutMs to 0, and call usleep(1) to release the CPU after HI_ERR_VENC_BUF_EMPTY is returned.
CNcomment: ���øýӿ���Ҫ���Ȱ���ƵԴ����ʼ����
����ʱ��������ã�ʱ�䵥λΪ���룬����Ϊ0���ȴ�������Ϊ0xffffffffһֱ�ȴ���\n
����������ʱ�䣬��û�����ݵ���򷵻�::HI_ERR_VENC_BUF_EMPTY\n
����ȴ�ʱ��Ϊ0����û���������ݣ��򷵻�::HI_ERR_VENC_BUF_EMPTY\n
��ʱ��֧�ַ�����ģʽ�����u32TimeOutMs����Ϊ0,�����ڴ˽ӿڷ���HI_ERR_VENC_BUF_EMPTY��ʱ�򣬵���usleep(10000)����CPU \n
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] pstStream Pointer to the structure for storing streams CNcomment: pstStream ��������ṹ��ָ��
\param[in] u32TimeoutMs: Wait timeout, count in ms CNcomment: u32TimeoutMs���ȴ���ʱʱ�䣬��λms
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_NULL_PTR The stream pointer is null. CNcomment: ����ָ��Ϊ��
\retval ::HI_ERR_VENC_CHN_NO_ATTACH The VENC channel is not attached to the video source. CNcomment: ����ͨ��û�а󶨵���ƵԴ
\retval ::HI_ERR_VENC_BUF_EMPTY Streams fail to be obtained. CNcomment: ��ȡ����ʧ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_AcquireStream(HI_HANDLE hVenc,HI_UNF_VENC_STREAM_S *pstStream, HI_U32 u32TimeoutMs);


/** 
\brief Releases VENC streams. CNcomment: �ͷ���Ƶ��������
\attention \n
You must obtain streams and keep the streams unchanged before calling this API.
CNcomment: ���øýӿ���Ҫ���Ȼ�ȡ�������Ҳ��ܸı���������
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] pstStream Pointer to the structure for storing streams CNcomment: pstStream ��������ṹ��ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_NULL_PTR The stream pointer is null. CNcomment: ����ָ��Ϊ��
\retval ::HI_ERR_VENC_CHN_NO_ATTACH The VENC channel is not attached to the video source. CNcomment: ����ͨ��û�а󶨵���ƵԴ
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_ReleaseStream(HI_HANDLE hVenc, const HI_UNF_VENC_STREAM_S *pstStream);


/** 
\brief Sets the encoding channel attributes dynamically. CNcomment: ��̬���ñ���ͨ������
\attention \n
You must create a VENC channel before calling this API.
CNcomment: ���øýӿ���Ҫ���ȴ�������ͨ��
\param[in] Venc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] pstAttr Pointer to the attributes of a VENC channel CNcomment: pstAttr ��ű���ͨ�����Ե�ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_NULL_PTR The pointer is null. CNcomment: ָ��Ϊ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_SetAttr(HI_HANDLE hVenc,const HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Obtains the attributes of a VENC channel. CNcomment: ��ȡ����ͨ������
\attention \n
You must create a VENC channel before calling this API.
CNcomment: ���øýӿ���Ҫ���ȴ�������ͨ��
\param[in] hVenc Handle of a VENC channel CNcomment: hVenc ����ͨ�����
\param[in] pstAttr Encoding channel attribute CNcomment: pstAttr ����ͨ������
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\retval ::HI_ERR_VENC_NULL_PTR The pointer is null. CNcomment: ָ��Ϊ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_GetAttr(HI_HANDLE hVenc, HI_UNF_VENC_CHN_ATTR_S *pstAttr);


/** 
\brief Applies for I frames. CNcomment: ����I֡
\attention \n
The video encoder encodes an I frame as soon as possible after you call this API.\n
You can call this API when you set up a video call or fix errors.\n
If you call this API repeatedly in a short period, I frames may be not generated each time.\n
CNcomment: ���øýӿں󣬱������ᾡ������һ��I֡��.\n
�˽ӿ�һ�����ڿ��ӵ绰������ͨ�������ʹ���ָ�.\n
�˽ӿ�ֻ��"���������"��������һ��I֡��������ڶ�ʱ���ڶ�ε��ô˽ӿڣ���ô���ܱ�֤ÿ�ε��ö��ܶ�Ӧ�����һ��I֡��.\n
\param[in] hVencChn Handle of a VENC channel CNcomment: hVencChn ����ͨ�����
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VENC_CHN_NOT_EXIST No VENC channel handle exists. CNcomment: ����ͨ�����������
\retval ::HI_ERR_VENC_NO_INIT The video encoder is not initialized. CNcomment: ������δ��ʼ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VENC_RequestIFrame(HI_HANDLE hVencChn);
/** @} */  /** <!-- ==== API Declaration End ==== */


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif

#endif//__HI_UNF_VENC_H__

