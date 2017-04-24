#ifndef __HI_MUXER_H__
#define __HI_MUXER_H__

#include "hi_type.h"
#include "hi_transcoder.h"
//#include "hi_tran_mbuf.h"

#ifdef __cplusplus
extern "C"{
#endif


#define TRANSCODER_MUXER_MEM_MAX (16)

/*********************************************
	ת��ģ��ת���ļ���װ����
**********************************************/
    
/*********************************************
       transcoder muxer file type
**********************************************/
typedef enum eTran_Muxerype
{
    TRANSCODER_MUTEX_FLV = 1,
    TRANSCODER_MUTEX_TS = 2,
    TRANSCODER_MUTEX_RTP = 4,
    TRANSCODER_MUTEX_ES = 8,
    TRANSCODER_MUTEX_HIES = 16,
    TRANSCODER_MUTEX_BUTT,
}Transcoder_MuxerType;

/*********************************************
	ת��ģ��ת���ļ���װ�������
**********************************************/
/*********************************************
       transcoder muxer create attr
**********************************************/
typedef struct tagTran_MuxerAttr
{
    Transcoder_MediaCodec audioCodec;		/*��ƵCodecID*/
    Transcoder_MediaCodec videoCodec;		/*��ƵCodecID*/
    HI_S32 s32Width;				/*��Ƶ�ֱ���--��*/
    HI_S32 s32Height; 				/*��Ƶ�ֱ���--��*/
    HI_S32 s32VideoBitRate;				/*��Ƶ������*/
    HI_S32 s32VideoFrameRate;			/*��Ƶ֡��*/
    HI_S32 s32AudioSampleRate;			/*��Ƶ������*/
    HI_S32 s32AudioBandWidth;			/*��Ƶ�������*/
    HI_S32 s32AudioChannels;            /*��Ƶ��������������*/
}Transcoder_MuxerAttr;

/** 
brief muxerģ���ʼ��
attention 
param[in] NULL
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer module initialization function
attention 
param[in] NULL
retval ::HI_SUCCESS    success
     HI_FAILURE    failure
*/
HI_S32 HI_Muxer_Init();
/** 
brief muxerģ��ȥ��ʼ��
attention 
param[in] NULL
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer module uninitialization function
attention 
param[in] NULL
retval ::HI_SUCCESS    success
     HI_FAILURE    failure
*/
HI_S32 HI_Muxer_DeInit();
/** 
brief muxerģ�鴴�����
attention �˽ӿ���protocol���ã���������һ��muxer�Ľӿ�
param[in] HI_U32 muxerType muxer������
param[OUT] HI_HANDLE* hMuxerHandle    ���ɵ�muxer�ľ��
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer  create handle function 
attention this function called by user and protocol��create a new muxer handle
param[in] HI_U32 muxerType muxer type
param[OUT] HI_HANDLE* hMuxerHandle    create muxer handle
retval ::HI_SUCCESS      success
           HI_FAILURE       failure
*/
HI_S32 HI_Muxer_Create(HI_HANDLE* hMuxerHandle,HI_U32 muxerType,Transcoder_MuxerAttr* muxerAttr);
/** 
brief muxerģ�����پ��
attention �˽ӿ���protocol���ã���������һ��muxer�Ľӿ�
param[in] HI_HANDLE hMuxerHandle muxer�ľ��
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer  destroy handle function 
attention this function called by user and protocol��destroy a new muxer handle
param[in] HI_HANDLE hMuxerHandle muxer handle
retval ::HI_SUCCESS      success
           HI_FAILURE       failure
*/
HI_S32 HI_Muxer_Destroy(HI_HANDLE hMuxerHandle);
/** 
brief muxerģ���ȡͷ����
attention �˽ӿ���protocol���ã�������ȡ��һ��ͷ���ݣ�������֡
param[in] HI_HANDLE hMuxerHandle   muxer�ľ��
param[OUT] HI_CHAR* pTAGFrame   TAG�����ݻ���λ��
[IN/OUT]          HI_S32* ps32Len   ��ͨ TAG���ݵĻ���������(IN)
                                                  Muxer֮������ݰ�����(OUT)
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/

/** 
brief muxer  get file header
attention this function called by user and protocol,get the file header or special frame
param[in] HI_HANDLE hMuxerHandle  muxer  handle
param[OUT] HI_CHAR* pTAGFrame   TAG buffer address
[IN/OUT]          HI_S32* ps32Len   TAG buffer initial len(IN)
                                                  after muxer buffer len(OUT)
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
HI_S32 HI_Muxer_GetHeader(HI_HANDLE hMuxerHandle,HI_CHAR* pTAGFrame,HI_S32* ps32Len);
/** 
brief muxerģ���ȡ��ͨ����
attention �˽ӿ���protocol���ã�������ȡһ����װ����ͨ TAG����
param[in] HI_U32 muxerType muxer������
                TRANSCODER_STRAM* pstTranFrame ��Transcoder�ж�ȡ������֡
param[OUT] HI_CHAR* pTAGFrame  ���ɵ���ͨ��TAG����
         [IN/OUT]          HI_S32* ps32Len   ��ͨ TAG���ݵĻ���������(IN)
                                                           Muxer֮������ݰ�����
                   
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer get normal frame
attention this function called by user and protocol,muxer a ES frame to a Muxer TAG
param[in] HI_U32 muxerType muxer  type
    TRANSCODER_STRAM* pstTranFrame  ES stream get from Transcoder
param[OUT] HI_CHAR* pTAGFrame  create TAG Frame
     [OUT/IN]   HI_S32* ps32Len     TAG buffer init len[in] / TAG buffer used len[OUT]
       
retval ::HI_SUCCESS  success
HI_FAILURE failure
*/
HI_S32 HI_Muxer_DoMuxer(HI_HANDLE hMuxerHandle,HI_CHAR* pTAGFrame,HI_S32* ps32Len,TRANSCODER_STRAM* pstTranFrame);
/** 
brief muxerģ������˽������
attention �˽ӿ���protocol���ã���������һ��muxer��˽������
param[in] HI_HANDLE hMuxerHandle muxerģ��ľ��
                HI_VOID* stAttr ���õ�˽�в���
                HI_S32 s32Len ˽�в����ĳ���
                   
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief muxer set private attr
attention this function called by user and protocol,to set muxer private attr
param[in] HI_HANDLE hMuxerHandle muxer  handle
            HI_VOID* stAttr  private attr defined by protocol specific module
            HI_S32 s32Len   the private attr data len
               
retval ::HI_SUCCESS  success
 HI_FAILURE failure
*/
HI_S32 HI_Muxer_SetPrivateData(HI_HANDLE hMuxerHandle,HI_VOID* stAttr,HI_S32 s32Len);

/** 
brief Muxer ע���ⲿЭ�麯��
attention :�˺�������HI_Muxer_Init����ܵ���
param[in] HI_CHAR* pLibMuxerName Ҫע��Ŀ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Muxer register external muxer lib 
attention :called after HI_Muxer_Init
param[in] HI_CHAR* pLibMuxerName the name of external muxer lib
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Muxer_RegistMuxer(const HI_CHAR* pLibMuxerName);
/** 
brief Muxer ȥע���ⲿЭ�麯��
attention :�˺�������HI_Muxer_Init����ܵ���
param[in] HI_CHAR* pLibMuxerName Ҫȥע��Ŀ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Muxer  unregister external muxer lib 
attention :called after HI_Muxer_Init
param[in] HI_CHAR* pLibMuxerName the name of external muxer lib would unregiste
retval ::HI_SUCCESS success
     HI_FAILURE failure
*/
HI_S32 HI_Muxer_DeRegistMuxer(const HI_CHAR* pLibMuxerName);
#ifdef __cplusplus
}
#endif


#endif
