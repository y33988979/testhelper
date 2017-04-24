/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_vi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/03/26
  Description   :
  History       :
  1.Date        : 2010/03/26
    Author      : j00131665
    Modification: Created file

*******************************************************************************/
/** 
 * \file
 * \brief Describes the information about video input (VI). CNcomment: �ṩVI�������Ϣ
 */

#ifndef  __HI_UNF_VI_H__
#define  __HI_UNF_VI_H__

#include "hi_type.h"
#include "hi_unf_common.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif


/********************************Error Code Definition********************************/
/** \addtogroup      H_3_6 */
/** @{ */  /** <!-- ��VI�� */

/**<Incorrect input parameter */
/**<CNcomment: ���������� */
#define HI_UNF_VI_INVALID_PARA_U32   (HI_U32)(-1)	

/** @} */  /** <!-- ====End of Error Code Definition==== */




/********************************Struct Definition********************************/
/** \addtogroup      H_2_2_10 */
/** @{ */  /** <!-- ��VI�� */

/**Channel type in BT.1120 YC separation mode */
/**CNcomment: BT.1120 YC����ģʽͨ������ */
typedef enum hiUNF_VI_CHN_YC_SEL_E
{
    HI_UNF_VI_CHN_YC_SEL_Y = 0,		/**Y separation channel in BT.1120 YC separation mode*/ /**<CNcomment: BT.1120 YC����ģʽ��Y����ͨ�� */
    HI_UNF_VI_CHN_YC_SEL_C,			/**C separation channel in BT.1120 YC separation mode*/ /**<CNcomment: BT.1120 YC����ģʽ��C����ͨ�� */
    HI_UNF_VI_CHN_YC_SEL_BUTT
}HI_UNF_VI_CHN_YC_SEL_E;

/**Storage type */
/**CNcomment: �洢���� */
typedef enum hiUNF_VI_STOREMETHOD_E
{
    HI_UNF_VI_STORE_METHOD_PNYUV = 0,  /**<plannar YCbCr */
    HI_UNF_VI_STORE_METHOD_SPNYC,      /**<Semi planar YC, chroma first, then lum */ /**<CNcomment: semi_plannar YC 422��ɫ��V��ǰ��U�ں� */
    HI_UNF_VI_STORE_METHOD_PKYUV,      /**<package yuv */
    HI_UNF_VI_STORE_METHOD_BUTT     
} HI_UNF_VI_STOREMETHOD_E;
/**Odd/even field select */
/**CNcomment: ��ż��ѡ��ģʽ */
typedef enum hiUNF_VI_CAPSEL_E
{
    HI_UNF_VI_CAPSEL_ODD=0,     /**<Collect the data of the odd field only */ /**<CNcomment: �����������ɼ� */
    HI_UNF_VI_CAPSEL_EVEN,      /**<Collect the data of the even field only */ /**<CNcomment: ����ż�����ɼ� */
    HI_UNF_VI_CAPSEL_BOTH,      /**<Collect the data of both odd and even fields */ /**<CNcomment: ����������ż�������ɼ� */
    HI_UNF_VI_CAPSEL_BUTT
} HI_UNF_VI_CAPSEL_E;

/**Frame/field storage mode */
/**CNcomment: ֡���洢ģʽ */
typedef enum hiUNF_VI_STORE_MODE_E
{
    HI_UNF_VI_STORE_FIELD = 0,   /**<Field storage mode */ /**<CNcomment: ���洢ģʽ */
    HI_UNF_VI_STORE_FRAME,       /**<Frame storage mode */ /**<CNcomment: ֡�洢ģʽ */
    HI_UNF_VI_STORE_BUTT
}HI_UNF_VI_STORE_MODE_E;

/**Input port */
/**CNcomment: ����˿� */
typedef enum hiUNF_VI_E
{
    HI_UNF_VI_PORT0   = 0x0,  /**<Port 0, only this port can be configured in BT1120 mode */ /**<CNcomment: �˿�0��BT1120ģʽ��ֻ�����ô˶˿� */
    HI_UNF_VI_PORT1,          /**<CNcomment: �˿�1 */
    HI_UNF_VI_BUTT
}HI_UNF_VI_E;

/**Input interface mode */
/**CNcomment: ����ӿ�ģʽ */
typedef enum hiUNF_VI_INPUT_MODE_E
{
    HI_UNF_VI_MODE_BT656 = 0,          /**<ITU-R BT.656 YUV4:2:2 standard, 576I */ /**<CNcomment: ITU-R BT.656 YUV4:2:2 ��׼��576I */
    HI_UNF_VI_MODE_BT656_480I,		   /**<ITU-R BT.656 YUV4:2:2 standard, 480I */ /**<CNcomment: ITU-R BT.656 YUV4:2:2 ��׼��480I */
    HI_UNF_VI_MODE_BT601,              /**<ITU-R BT.601 YUV4:2:2 standard */ /**<CNcomment: ITU-R BT.601 YUV4:2:2 ��׼ */
    HI_UNF_VI_MODE_DIGITAL_CAMERA,     /**<Digital camera. The data format can be RGB or YCbCr */ /**<CNcomment: ��������ͷ�����ݿ�����RGB����YCbCr */
    HI_UNF_VI_MODE_RAW_DATA,           /**<Data input over the composite video broadcast signal (CVBS) interface. The data does not need to be processed. */ /**<CNcomment: ͨ��CVBS��������ݣ�����Ҫ���� */
    HI_UNF_VI_MODE_USB_CAM,            /**<Virtual VI. The data is not input from the hardware, but by calling the related get and put interfaces. */ /**<CNcomment: USB ����ͷ�����û�Get/Put�������� */ 
    HI_UNF_VI_MODE_VIRTUAL,            /**<CNcomment: ����VI,����Ӳ���������ݣ����û�Get/Put�������� */    
    HI_UNF_VI_MODE_BT1120_480P,	       /**<ITU-R BT.1120 YUV4:2:2 standard, 480P */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��480P */
    HI_UNF_VI_MODE_BT1120_576P,        /**<ITU-R BT.1120 YUV4:2:2 standard, 576P */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��576P */
    HI_UNF_VI_MODE_BT1120_720P,        /**<ITU-R BT.1120 YUV4:2:2 standard, 720P_50 */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��720P_50 */
    HI_UNF_VI_MODE_BT1120_720P_60,	   /**<ITU-R BT.1120 YUV4:2:2 standard, 720P_60 */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��720P_60 */
    HI_UNF_VI_MODE_BT1120_1080I,       /**<ITU-R BT.1120 YUV4:2:2 standard, 1080I_50 */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��1080I_50 */
    HI_UNF_VI_MODE_BT1120_1080I_60,    /**<ITU-R BT.1120 YUV4:2:2 standard, 1080I_60 */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��1080I_60 */
    HI_UNF_VI_MODE_BT1120_1080P,       /**<ITU-R BT.1120 YUV4:2:2 standard, 1080P */ /**<CNcomment: ITU-R BT.1120 YUV4:2:2 ��׼��1080P */
    HI_UNF_VI_MODE_BUTT
} HI_UNF_VI_INPUT_MODE_E;

/**VI frame buffer management mode */
/**CNcomment: VI֡�������ʽ */
typedef enum hiUNF_VI_BUF_MGMT_E
{
    HI_UNF_VI_BUF_ALLOC   = 0x0,  /**<VI alloc buffer */ /**<CNcomment: VI����֡BUF */
    HI_UNF_VI_BUF_MMAP,           /**<User alloc frame buffer, then maps it to VI */ /**<CNcomment: �û�����֡BUF����ӳ���VI */    
    HI_UNF_VI_BUF_BUTT
}HI_UNF_VI_BUF_MGMT_E;

/**VI attributes */
/**CNcomment: VI���� */
typedef struct hiUNF_VI_ATTR_S
{
    HI_UNF_VI_E              enViPort;		/**<Input port */ /**<CNcomment: ����˿� */
    HI_UNF_VI_INPUT_MODE_E   enInputMode;   /**<Video input mode */ /**<CNcomment: ��Ƶ����ģʽ */
    HI_RECT_S                stInputRect;   /**<Crop region of a window */ /**<CNcomment: ���ڵĲü����� */
    HI_UNF_VI_CAPSEL_E       enCapSel;		/**<Odd/even field select */ /**<CNcomment: ��ż���ɼ�ѡ�� */

    HI_UNF_VI_CHN_YC_SEL_E   enChnYC;       /**<Valid only in BT1120 mode */ /**<CNcomment: ֻ����bt1120ģʽ����Ч */
    HI_UNF_VI_STOREMETHOD_E  enStoreMethod; /**<Video format */ /**<CNcomment: ��Ƶ��ʽ */
    HI_UNF_VI_STORE_MODE_E	 enStoreMode;   /**<Frame/field mode */ /**<CNcomment: ֡��ģʽ */
	HI_UNF_VI_BUF_MGMT_E     enBufMgmtMode; /**<VI frame buffer management mode */ /**<CNcomment: VI֡�������ʽ */

    HI_U32 u32YStride;		/**<Stride of the Y component in planar mode. It is the only stride in other modes. */ /**<CNcomment: planarģʽ��Y��stride������ģʽ��Ψһstride */
    HI_U32 u32CStride;		/**<Stride of the C component in planar mode. It is invalid in other modes. */ /**<CNcomment: planarģʽ��C��stride������ģʽ��Ч */
    HI_U32 u32ViBufAddr;	/**<Buffer address of frame buffer, user does not need to config in alloc mode;
                                Virtual address of user mode that user inputs frame buffer in MMAP mode */
                            /**<CNcomment: ֡buf��ַ, ALLOCģʽ���û�����Ҫ���ã�VI����������ַ
    									   MMAPģʽ���û�����֡buf���û�̬�����ַ */
    HI_U32 u32FrameSize;    /**<Size of the memory used by a frame, output parameter. You do not need to configure it. */
                            /**<CNcomment: һ֡ռ�õ��ڴ�ռ�,����������û�����Ҫ���� */
    HI_U32 u32BufSize;		/**<Size of the frame buffer, output parameter. You do not need to configure it. */
                            /**<CNcomment: ֡buf�Ĵ�С,����������û�����Ҫ���� */
    HI_U32 u32BufNum;       /**<Number of available buffers */ /**<CNcomment: ���õ�֡buf�ĸ��� */
} HI_UNF_VI_ATTR_S;

/**Attributes of the VI buffer */
/**CNcomment: VI Buffer ���� */
typedef struct  hiUNF_VI_BUF_S
{ 
    HI_U32                      u32FrameIndex;	/**<Frame index */ /**<CNcomment: ֡���� */
    HI_UNF_VIDEO_FORMAT_E       enPixelFormat;  /**<Sampling format of the input video */ /**<CNcomment: ������Ƶ������ʽ */
    HI_U32                      u32Width;       /**<Input width */ /**<CNcomment: ������ */
    HI_U32                      u32Height;      /**<Input height */ /**<CNcomment: ����߶� */
                                                        
    HI_UNF_VIDEO_FIELD_MODE_E   enField;        /**<Sampling field type, top field, bottom field, or all */ /**<CNcomment: �ɳ����ͣ�top/bottom/all */
    HI_U32                      u32DownScaled;  /**<Whether to perform down sampling */ /**<CNcomment: �Ƿ�Ҫ�²��� */
	HI_UNF_VI_BUF_MGMT_E        enBufMode;      /**<VI frame buffer management mode */ /**<CNcomment: VI֡�������ʽ */

    /* Only u32PhyAddr[0] is valid in RGB or package storage mode.
       Only u32PhyAddr[0-1] is valid in semi-planar storage mode.
       The validity of pVirAddr and u32Stride is the same as the validity of u32PhyAddr. */
    /* CNcomment: �����RGB����package�洢ģʽ��ֻ��u32PhyAddr[0]��Ч.
       �����semi-planar�洢ģʽ��ֻ��u32PhyAddr[0-1]��Ч,
       ��Ӧ��pVirAddr��u32StrideҲ��u32PhyAddr��Ч��һ��. */
    HI_U32                      u32PhyAddr[3];        /**<Physical address of the A, B, or C component *//**<CNcomment: A/B/C ���������ַ */	
    HI_VOID                     *pVirAddr[3];         /**<Virtual address of the A, B, or C component */ /**<CNcomment: A/B/C ���������ַ */
    HI_U32                      u32Stride[3];         /**<Stride of the A, B, or C component */ /**<CNcomment: A/B/C ����stride */

    HI_U32                      u32PtsMs;             /**<Presentation time stamp */ /**<CNcomment: ʱ��� */                     
}HI_UNF_VI_BUF_S;

/** @} */  /** <!-- ==== Struct Definition End ==== */

/********************************API Declaration********************************/
/** \addtogroup      H_1_2_8 */
/** @{ */  /** <!-- ��VI�� */

/** 
\brief Initializes the video input unit (VIU). CNcomment: ��ʼ��VI
\attention \n
Before calling the VIU, you must call this application programming interface (API). CNcomment: ����VIģ��Ҫ�����ȵ��øýӿ�
\param[in] N/A CNcomment: ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_DEV_NOT_EXIST No VI device exists. CNcomment: �豸������
\retval ::HI_ERR_VI_NOT_DEV_FILE The file is not a VI file. CNcomment: �ļ����豸
\retval ::HI_ERR_VI_DEV_OPENED The VI device fails to be started. CNcomment: ���豸ʧ��
\see \n
N/A ��
*/
HI_S32 HI_UNF_VI_Init(HI_VOID);


/** 
\brief Deinitializes the VIU. CNcomment: ȥ��ʼ��VI
\attention \n
\param[in] N/A CNcomment: ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_DEV_NOT_EXIST No VI device exists. CNcomment: �豸������
\retval ::HI_ERR_VI_DEV_CLOSE_ERR The VI device fails to be stopped. CNcomment: �ر�VI �豸ʧ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_DeInit(HI_VOID);


/** 
\brief Obtains the attributes of a VI port. CNcomment: ��ȡVI�˿�Ĭ������
\attention \n
By default, port 0 is used, half D1 and BT656 formats are used, the data of the odd field is collected, and the semi-planar YUV4:2:0 data is output.
CNcomment: Ĭ��Port=0, HALF-D1, BT656���ɼ�ż�������SemiPlanner YUV4:2:0��ʽ����
\param[in] pstAttr Pointer to VI attributes CNcomment: VI����ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_GetDefaultAttr(HI_UNF_VI_ATTR_S *pstAttr);


/** 
\brief Set the attributes of a VI port. CNcomment: ����VI Port����
\attention \n
This reserved API is combined with HI_UNF_VI_Create, and cannot be called dynamically.
CNcomment: Ԥ���ӿڣ���Create��һ�����ɶ�̬����
\param[in] hVi VI handle CNcomment: VI���
\param[in] pstAttr  Pointer to VI attributes CNcomment: VI����ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\retval ::HI_ERR_VI_CHN_INIT_BUF_ERR An error occurs when the buffer is initialized. CNcomment: ��ʼ��buffer����
\retval ::HI_ERR_VI_INVALID_PARA The attribute parameters are incorrect. CNcomment: ���Բ�������
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_SetAttr(HI_HANDLE hVi, HI_UNF_VI_ATTR_S *pstAttr);


/** 
\brief Obtains the attributes of a VI port. CNcomment: ��ȡVI Port����
\attention \n
\param[in] hVi VI handle CNcomment: VI���
\param[out] pstAttr Pointer to VI attributes CNcomment: VI����ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_GetAttr(HI_HANDLE hVi, HI_UNF_VI_ATTR_S *pstAttr);


/** 
\brief Enables a VI port to create a VI handle. CNcomment: ��VI Port������Vi���
\attention \n
Before calling the VIU, you must call this API.\n
Unless the virtual VI is used, the port ID must correspond to the hardware. That is, the hardware and software must use the same port\n
CNcomment: ����VI ģ����Ҫ���ȵ��øýӿ�\n
CNcomment: ����ʹ������VI, ����PortID��Ҫ��Ӳ����Ӧ,Ӳ����ʹ�����ĸ�Port,��ô���Ҳ����ʹ���ĸ�Port.\n
\param[in] pstAttr CNcomment: Pointer to VI attributes VI����ָ��
\param[in] phandle CNcomment: hVi Pointer to the VI handle VI���ָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\retval ::HI_ERR_VI_CHN_INIT_BUF_ERR An error occurs when the buffer is initialized. CNcomment: ��ʼ��buffer����
\retval ::HI_ERR_VI_INVALID_PARA The attribute parameters are incorrect. CNcomment: ���Բ�������
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_Create(HI_UNF_VI_ATTR_S *pstAttr, HI_HANDLE *phandle);


/** 
\brief Disable a VI port to destroy the handle. CNcomment: �ر�VI Port�����پ��
\attention \n
\param[in] hVi VI handle CNcomment: VI���
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_Destroy(HI_HANDLE hVi);


/** 
\brief Obtains the address of a frame buffer over a VI port. CNcomment: ��ȡ����VI Port һ֡ͼ��Buffer
\attention \n
If a virtual VI port is created, you can obtain the VI buffer for stuffing the YUV data by calling this API.\n
This API is used to store the picture captured by the USB camera in the VI frame buffer.\n
CNcomment: ����û�������������VI Port��һ������USB����ͷ\n
CNcomment: �û����ô˽ӿڴ�VI��ȡһ֡BUF������YUV����\n
\param[in] hVi VI handle CNcomment: VI���
\param[out] pViBuf Pointer to the VI buffer CNcomment: VI bufferָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_GetFrame(HI_HANDLE hVi, HI_UNF_VI_BUF_S * pViBuf);


/** 
\brief Releases the VI frame buffer for the driver.  CNcomment: ��������VI Port һ֡ͼ��Buffer����������Port����
\attention
In HI_UNF_VI_BUF_ALLOC mode, HI_UNF_VI_GetFrame gets the address of one YUV frame data, after copying YUV data, \n
user can use this function to inform VI data is ready. \n
In HI_UNF_VI_BUF_MMAP mode(only used in USB camera), when camera captures one frame, \n
user can use this function to inform VI data is ready. \n
CNcomment: ������ΪHI_UNF_VI_BUF_ALLOCģʽʱ��HI_UNF_VI_GetFrame��ȡ��һ֡YUV���ݵĵ�ַ��\n
					���ҿ�����YUV���ݺ󣬾Ϳ��Ե��ô˽ӿ�֪ͨVI�����Ѿ�׼����\n
������ΪHI_UNF_VI_BUF_MMAPģʽʱ(������USB����ͷ)������ͷ����һ֡ͼ��\n
									�Ϳ��Ե��ô˽ӿ�֪ͨVI�����Ѿ�׼����\n					
\param[in] hVi VI handle CNcomment: VI���
\param[in] pViBuf Pointer to the VI buffer CNcomment: VI bufferָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_PutFrame(HI_HANDLE hVi, const HI_UNF_VI_BUF_S * pViBuf);


/** 
\brief Obtains the address of a YUV picture. CNcomment: ��ȡһ֡YUV ͼ���ַ
\attention \n
You do not need to call this API generally. It is recommended that you bind the VIU to the video encoder (VENC) or video output unit (VOU) to implement the encoding function or VIVO loopback function.
CNcomment: һ������£��û�����Ҫ���ô˽ӿڣ��Ƽ�ֱ�ӽ�VI��VENC����VO���а��Ա�ʵ�ֱ������VIVO���ع��ܡ�
\param[in] hVi VI handle CNcomment: VI���
\param[out] pViBuf Pointer to the VI buffer CNcomment: VI bufferָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_AcquireFrame(HI_HANDLE hVi, HI_UNF_VI_BUF_S * pViBuf);


/** 
\brief Releases the frame buffer for the VIU after the VENC or VOU obtains a YUV picture.
CNcomment: �û��黹һ֡YUV ͼ���ַ
\attention \n
You do not need to call this API generally. It is recommended that you bind the VIU to the VENC or VOU to implement the encoding function or VIVO loopback function.
CNcomment: һ������£��û�����Ҫ���ô˽ӿڣ��Ƽ�ֱ�ӽ�VI��VENC����VO���а��Ա�ʵ�ֱ������VIVO���ع��ܡ�
\param[in] hVi VI handle CNcomment: VI���
\param[in] pViBuf Pointer to the VI buffer CNcomment: VI bufferָ��
\retval ::HI_SUCCESS Success CNcomment: �ɹ�
\retval ::HI_ERR_VI_CHN_NOT_EXIST The handle is incorrect. CNcomment: �������
\retval ::HI_ERR_VI_NULL_PTR The pointer is null. CNcomment: ��ָ��
\see \n
N/A CNcomment: ��
*/
HI_S32 HI_UNF_VI_ReleaseFrame(HI_HANDLE hVi, const HI_UNF_VI_BUF_S * pViBuf);
/** @} */  /** <!-- ====API Declaration End ==== */


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif
#endif
