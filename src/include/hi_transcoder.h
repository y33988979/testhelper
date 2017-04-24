#ifndef __HI_TRANSCODER_H__
#define __HI_TRANSCODER_H__

#include "hi_type.h"
#include "hi_unf_venc.h"
#include "hi_unf_aenc.h"

#ifdef __cplusplus
extern "C"{
#endif


/***************��ȡ���ƽ̨*****************/
/***************software platform define *****************/
#ifdef ANDROID_TOOLCHAIN
#define SOFT_BOARD_VERSION_ANDROID 
#else
#define SOFT_BOARD_VERSION_LINUX
#endif

/***********enum**************/

/*****************************************
	��ȡ֡��������
******************************************/
/*****************************************
     transcoder media data type
******************************************/
typedef enum eTran_PAYLOAD
{
    PAYLOAD_VIDEO_NORMAL_FRAME = 1,
    PAYLOAD_VIDEO_KEY_FRAME = 5, //��Ƶ�ؼ�֡
    PAYLOAD_VIDEO_SPS_FRAME = 7,
    PAYLOAD_VIDEO_PPS_FRAME = 8,
    PAYLOAD_AUDIO_FRAME = 10, //��Ƶ֡
    PAYLOAD_MD_FRAME, //MD����
    PAYLOAD_BUTT
}TRANSCODER_PAYLOAD_E;

/*****************************************
	ת��ģ��ת������Ƶ����
******************************************/
/*****************************************
      transcoder video/audio type
******************************************/
typedef enum eTran_MediaCodec
{
    TRANSCODER_CODEC_ID_H264 = 0, 
    TRANSCODER_CODEC_ID_H263, 
    TRANSCODER_CODEC_ID_MPEG4,
    TRANSCODER_CODEC_ID_AACLC,
    TRANSCODER_CODEC_ID_AMRNB,
    TRANSCODER_CODEC_ID_G711A,
    TRANSCODER_CODEC_ID_NONE,
    TRANSCODER_CODEC_ID_BUTT
}Transcoder_MediaCodec;


typedef enum eTran_Attr_ID_E
{
    TRANSCODER_ALL_ATTR,
    TRANSCODER_VIDEO_ATTR,
    TRANSCODER_AUDIO_ATTR,
    TRANSCODER_BACKGND_ATTR,
    TRANSCODER_ATTR_BUTT
}Transcoder_Attr_ID_E;
/*****************************************
	ý������payLoad��������
******************************************/
/*****************************************
       media data payload data structure
******************************************/
typedef union hiTranscoder_DATA_TYPE_U
{
    HI_UNF_VENC_STREAM_S stVencStream;
    HI_UNF_ES_BUF_S      stAencStream;
}HI_TRAN_DATA_TYPE_U;

/*****************************************
	ý������֡�ṹ
******************************************/
    
/*****************************************
     transcoder data frame structure
******************************************/
typedef struct tagTran_STREAM_S
{
	TRANSCODER_PAYLOAD_E ePayloadType;			/*PayLoad��������*/
    HI_TRAN_DATA_TYPE_U  enDataStream;
}TRANSCODER_STRAM;


/*�ṩ���û���ָ��ص��ӿڣ����ݲ�ͬ��ָ��������Ժ���Դ��ģ�飬
��Դ��ģ��ȷ������Ӧ��ʽ

��Ϊ��̨ת�룬�˺������� Transcoderʵ��*/
/*
    supply for user callback function, modified by user, set by source module
*/
typedef HI_S32 (*HI_TRANSCODER_NODIFY_CALLBACK)(HI_HANDLE Handle,HI_U32 u32Command,HI_VOID* stAttr,HI_S32 s32Len );
/*******************************************
	ת��ģ��ת���������
********************************************/
/*******************************************
     transcoder module create attr
********************************************/
typedef struct tagTran_HandleAttr
{
    Transcoder_MediaCodec audioCodec;		/*��ƵCodecID*/
    Transcoder_MediaCodec videoCodec;		/*��ƵCodecID*/
 //   HI_U32 u32ProtocolMask;                 /*��ʱ�Ĵ�Э���mask ֵ���ɴ򿪶��Э��*/
    HI_U32 SourceType;              /*ת��ģ��Դ������ƵԴ���ͣ�Ϊ����������չԤ��*/
//    HI_S32 s32X;                    /*ת�����ʼλ�� X ����*/
//    HI_S32 s32Y;                    /*ת�����ʼλ�� Y ����*/
    HI_S32 s32Width;				/*��Ƶ�ֱ���--��*/
    HI_S32 s32Height; 				/*��Ƶ�ֱ���--��*/
    HI_S32 s32VideoBitRate;				/*��Ƶ������*/
    HI_S32 s32VideoFrameRate;			/*��Ƶ֡��*/
    HI_S32 s32AudioSampleRate;			/*��Ƶ������*/
    HI_S32 s32AudioBandWidth;			/*��Ƶ�������*/
    HI_S32 s32AudioChannels;            /*��Ƶ��������������*/
    HI_BOOL bCycle;                     /*�����ļ������Ƿ�Ҫѭ��*/
    HI_HANDLE hSrc;				/*ת������Դ�����ΪNULL����ȫ��ת��*/
    HI_TRANSCODER_NODIFY_CALLBACK pNodifySrcFunc;   /*ת��ģ���ṩ������ģ��Ļص�����,��˶˿ڻص���Դ��*/
}Transcoder_HandleAttr;

/********************************API declaration********************************/

/** 
brief ��ʼ��ת����
attention 
����ת��Ҫ�����ȵ��øýӿ�
param[in] ��
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief  transcoder initialization function
attention 
HiTranscoder module must init function
param[in] NULL
retval ::HI_SUCCESS  SUCCESS
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_Init(void);
/** 
brief ȥ��ʼ��ת����
attention 
ת���˳�Ҫ�������øýӿ�
param[in] ��
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief transcoder uninitialization function
attention 
HiTranscoder module all clear function must called behind
param[in] ��
retval ::HI_SUCCESS  success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_DeInit(void);
/** 
brief ��ת��ͨ��
attention 
��ҵ����Ҫʹ��ת������ʱ���ã���Ҫ����Transcoder_HandleAttr��ת��ͨ��
��ת�������е�hsrcΪ��ʱ�����ڰ汾���ش���
���Ҿ���
param[in] Transcoder_HandleAttr pstAttr  ����ת��ͨ��������
param[out] HI_HANDLE* pHandle  		 ������ת��ͨ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief  create transcoder function
attention 
transcoder create handle.must set Transcoder_HandleAttr attr,could call Transcoder_getDefaultAttr
when attr's property hsrc is NULL ,now version return failure
param[in] Transcoder_HandleAttr pstAttr  transcoder handle attr
param[out] HI_HANDLE* pHandle        create the handle of transcoder
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_Create(HI_HANDLE* pHandle,const Transcoder_HandleAttr *pstAttr);

/** 
brief ����ת��ͨ��
attention 
��ҵ����Ҫ�ر�ת������ʱ���ã�
��Ҫע���ʱ��״̬��Ҫ�������е�Э�飬����������
param[in] HI_HANDLE* pHandle  		 Ҫ���ٵ�ת��ͨ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief destroy transcoder handle
attention 
called this function,need destroy all protocol,
param[in] HI_HANDLE* pHandle         need destroy transcoder handle
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_Destroy(HI_HANDLE pHandle);
/** 
brief ��ȡת����������
attention 
��ҵ����Ҫ��ȡת����������
��Ҫע��˽ӿ���Ҫ��create�������ú�ʹ��
param[in] HI_HANDLE* pHandle  		 �����õ�ת��ͨ�����
param[out] Transcoder_HandleAttr *pstAttr ��ȡ��ת��ͨ��������
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief get the attr of transcoder handle
attention this function called after transcoder create handle function,
param[in] HI_HANDLE* pHandle         will get transcoder handle
param[out] Transcoder_HandleAttr *pstAttr transcoder handle attr
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_GetAttr(HI_HANDLE pHandle,Transcoder_Attr_ID_E eTranAttr,Transcoder_HandleAttr *pstAttr);
/** 
brief ����ת����������
attention 
��ҵ����Ҫ����ת����������
��Ҫע��˽ӿ���Ҫ��create�������ú�ʹ��
param[in] HI_HANDLE* pHandle  		 �����õ�ת��ͨ�����
param[in] Transcoder_HandleAttr *pstAttr ����ת��ͨ��������
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief set transcoder handle attr
attention 
need called after transcoder create function
param[in] HI_HANDLE* pHandle         the handle who need set attr
param[in] Transcoder_HandleAttr *pstAttr the attr will set to handle
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_SetAttr(HI_HANDLE pHandle,Transcoder_Attr_ID_E eTranAttr,const Transcoder_HandleAttr *pstAttr);
/** 
brief ��ȡת��ʹ����Դ���
attention 
��ҵ����Ҫ��ȡת����Դ���
��Ҫע��˽ӿ���Ҫ��init�������ú�ʹ��
param[in] HI_HANDLE* pHandle  		 �����õ�ת��ͨ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief get the resource info of transcoder module
attention 
this function called after transcoder initial function
param[in] HI_HANDLE pHandle         the handle
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_GetResourceInfo(HI_HANDLE pHandle);
/** 
brief ��ȡת��Debug ��Ϣ
attention 
��ҵ����Ҫ��ȡת��Debug ��Ϣ
��Ҫע��˽ӿ���Ҫ��init�������ú�ʹ��
param[in] HI_HANDLE* pHandle  		 �����õ�ת��ͨ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief get the debug info of transcoder
attention 
this function called after transcoder handle init
param[in] HI_HANDLE pHandle         the handle of transcoder
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_GetDebugInfo(HI_HANDLE pHandle);
/********************************API declaration********************************/

/** 
brief ����ת����״̬������start
attention 
��ת��������Stop-start״̬���ոýӿڵ�״̬��ת������
param[in] HI_HANDLE hTranHandle  ת����
         [in] HI_BOOL bBlock  �Ƿ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief set transcoder handle status --- start
attention 
 when transcoder enter stop-start status machine ,after transcoder create handle
 
param[in] HI_HANDLE hTranHandle  transcoder handle
        [in] HI_BOOL bBlock  blocked called or not 
retval ::HI_SUCCESS success
     HI_FAILURE success
*/

HI_S32 HI_Transcoder_Start(HI_HANDLE hTranHandle,HI_BOOL bBlock);
/** 
brief �첽����ת����״̬������stop
attention 
��ת��������Stop-start״̬���ոýӿڵ�״̬��ת������
param[in] HI_HANDLE hTranHandle  ת����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief set transcoder handle status --- stop
attention 
 when transcoder enter stop-start status machine ,after transcoder create handle
 
param[in] HI_HANDLE hTranHandle  transcoder handle
        [in] HI_BOOL bBlock  blocked called or not 
retval ::HI_SUCCESS success
     HI_FAILURE success
*/
HI_S32 HI_Transcoder_Stop(HI_HANDLE hTranHandle,HI_BOOL bBlock);
/** 
brief ͬ������Transcoderģ��Ļص���������ص����̸��� Command��ͬ����
        �û�ָ����ͬ��ʵ�ַ�֧
attention 
��ת��������Stop-start״̬���ոýӿڵ�״̬��ת������
param[in] HI_HANDLE hTranHandle  ת����
               HI_U32 u32Command     ת��ص���ָ��
               HI_VOID* pstAttr           ָ��Ĳ���
               I_S32 s32AttrLen           ָ������ĳ���
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief sync called Transcoder module callback function,different because of command,
attention 
when transcoder enter stop-start status machine ,after transcoder create handle
param[in] HI_HANDLE hTranHandle  transcoder handle
               HI_U32 u32Command      transcoder callback command
               HI_VOID* pstAttr           callback attr 
               I_S32 s32AttrLen           callback attr len
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_SetCommand(HI_HANDLE hTranHandle,HI_U32 u32Command,HI_VOID* pstAttr,HI_S32 s32AttrLen);

/** 
brief ע��ý�����ݶ�ȡ���
attention 
ע��ý�����ݶ����ӿڣ��ýӿ���protocolģ����е���
param[in] HI_HANDLE hTranHandle Transcoder���
param[out]HI_HANDLE* phMbufReadHandle ý�����ݵĶ�ȡ���
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief register media data read handle
attention 
register media data read handle,this function called by protocol and user
param[in] HI_HANDLE hTranHandle  transcoder handle
param[out]HI_HANDLE* phMbufReadHandle media data read handle
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_RegisterHandle(HI_HANDLE hTranHandle,HI_HANDLE* phMbufReadHandle);
/** 
brief ȥע��ý�����ݾ��
attention 
ȥע��MBUFд��ӿڣ��ýӿ���protocolģ����е���
param[in]HI_HANDLE hMbufReadHandle ý�����ݵĶ�ȡ���
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief unregister media data read handle
attention 
unregister media data read handle,this function called by protocol and user
param[in]HI_HANDLE hMbufReadHandle media data read handle
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Transcoder_DeRegisterHandle(HI_HANDLE hMbufReadHandle);
/** 
brief ��ȡý������
attention 
��ȡ�����ݿ����޷���������ʱ����HI_FAILURE

param[in] HI_HANDLE hReadHandle MBUF�Ķ�ȡ���
param[out]TRANSCODER_FRAME* pstTranFrame ��MBUF�ж�ȡ������֡�ṹ
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief ��ȡý������
attention 
��ȡ�����ݿ�������MBUF����Ϊ�գ��޷���������ʱ����HI_FAILURE

param[in] HI_HANDLE hReadHandle MBUF�Ķ�ȡ���
param[out]TRANSCODER_FRAME* pstTranFrame ��MBUF�ж�ȡ������֡�ṹ
retval ::HI_SUCCESS �ɹ�
     HI_FAILURE ʧ��
*/
HI_S32 HI_Transcoder_ReadStream(HI_HANDLE hReadHandle,TRANSCODER_STRAM* pstTranFrame);
/** 
brief ��ȡý������
attention 
��ȡ�����ݿ�������MBUF����Ϊ�գ��޷���������ʱ����HI_FAILURE

param[in] HI_HANDLE hReadHandle MBUF�Ķ�ȡ���
param[out]TRANSCODER_FRAME* pstTranFrame ��MBUF�ж�ȡ������֡�ṹ
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
HI_VOID Transcoder_getDefaultAttr(Transcoder_HandleAttr* stTranHandleAttr);

#ifdef __cplusplus
}
#endif


#endif
