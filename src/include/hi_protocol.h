#ifndef __HI_PROTOCOL_H__
#define __HI_PROTOCOL_H__

#include "hi_type.h"

#ifdef __cplusplus
extern "C"{
#endif

/***********enum**************/
#define TRANSCODER_PROTOCOL_MEM_MAX     (16)
/*****************************************
	transcoder module status machine enum
******************************************/
/*****************************************
	ת��ģ��Э��״̬��
******************************************/
typedef enum eTran_ProtocolStatus
{
	TRANSCODER_PROTOCOL_INIT = 0,
	TRANSCODER_PROTOCOL_STOP,
	TRANSCODER_PROTOCOL_START,
	TRANSCODER_PROTOCOL_STATUS_BUTT,
}Transcoder_ProtocolStatus;

typedef enum eWFD_External_Method
{
    WFD_SOURCE_METHOD_INIT = 0,
    WFD_SOURCE_METHOD_RESET = 1,
    WFD_SOURCE_METHOD_SEEK = 2,
    WFD_SOURCE_METHOD_CHANGE = 3,
    WFD_SOURCE_METHOD_BUTT,
}WFD_External_Method_E;

/****************************************************
	ת��ģ��ת��Э�����������
*****************************************************/
/****************************************************
        transcoder module protocol server type
*****************************************************/
typedef enum eTran_ProtocolType
{
    TRANSCODER_PROTOCOL_HTTP = 1,
    TRANSCODER_PROTOCOL_RTSP = 2,
    TRANSCODER_PROTOCOL_VTRL_DRV = 4,
    TRANSCODER_PROTOCOL_FAST_STREAM = 8,
    TRANSCODER_PROTOCOL_SAMPLE = 16,
    TRANSCODER_PROTOCOL_BUTT,
}Transcoder_ProtocolType;

#define TRANSCODER_COMMAND_BASE 0xA0000000
/****************************************************
	ת��ģ��Protocol ����ģ���ύ�������
*****************************************************/
/****************************************************
     Protocol send Command to Transcoder type 
*****************************************************/
typedef enum eProto_CallBackCommand
{
    PROTOCOL_COMMAND_PAUSE = TRANSCODER_COMMAND_BASE + 1,
    PROTOCOL_COMMAND_SEEK = TRANSCODER_COMMAND_BASE + 2,
    PROTOCOL_COMMAND_SETATTR = TRANSCODER_COMMAND_BASE + 3,
    PROTOCOL_COMMAND_SETSOURCE = TRANSCODER_COMMAND_BASE + 4,
    PROTOCOL_COMMAND_SETMUTE = TRANSCODER_COMMAND_BASE + 5,
}Protocol_CallBackCommand;
/****************************************************
	ת��ģ��ת��Э��������򿪲���
*****************************************************/
/****************************************************
      Protocol create handle Protocol Attr
*****************************************************/
typedef struct tagProtocolAttr
{
    HI_S32 s32Port;             /*Э��Ķ˿�*/
    HI_U32 u32ProtocolType;     /*Э�������*/
    HI_HANDLE SourceHandle;     /*Э��������Դ*/
    HI_S32 s32UserNum;          /*Э��֧�ֵ��û�����*/
}ProtocolAttr;


/** 
brief Protocolģ���ʼ��
attention 
param[in] NULL
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
    
/** 
brief Protocol  initialization function
attention 
param[in] NULL
retval ::HI_SUCCESS     success
HI_FAILURE   failure
*/
HI_S32 HI_Protocol_Init();
/** 
brief Protocolģ��ȥ��ʼ��
attention 
param[in] NULL
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/

/** 
brief Protocol uninitialization function
attention 
param[in] NULL
retval ::HI_SUCCESS    success
	 HI_FAILURE         failure
*/
HI_S32 HI_Protocol_DeInit();
/** 
brief Protocolģ������Protocol���
attention �˽ӿ����û����ã���Transcoderû�б�Ȼ���Ⱥ�˳��
param[in] ProtocolAttr* pstTranAttr ���õ�Protocol������
param[out] HI_HANDLE* hProtocolHandle ���ɵ�Protocol���
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Protocol  module create Protocol handle
attention this function called by user, not have relationship with transcoder
param[in] ProtocolAttr* pstTranAttr  protocol create attr
param[out] HI_HANDLE* hProtocolHandle creative protocol handle
retval ::HI_SUCCESS  success
HI_FAILURE failure
*/
HI_S32 HI_Protocol_Create(HI_HANDLE* hProtocolHandle,const ProtocolAttr* pstTranAttr);
/** 
brief Protocol  module destroy protocol handle
attention  this function called by user, not have relationship with transcoder
param[in] HI_HANDLE hProtocolHandle need destory protocol  handle
retval ::HI_SUCCESS success
	 HI_FAILURE failure
*/
HI_S32 HI_Protocol_destory(HI_HANDLE hProtocolHandle);
/** 
brief Protocolע�ắ��
attention :�˺�������Transcoder_create����ܵ���,���û�������Դ�����ϴ�仯����Ҫ�ȵ���HI_Protocol_UnRegistHandle��
	  Ȼ����ñ����������磺���ű����ļ�ʱ�������ļ�������ɣ��ִ�ͷ���ŵ����������dvb���ֻ�̨�������
param[in] HI_HANDLE hTranHandle Ҫ�󶨵�Transcoder�ľ��
                HI_HANDLE pProtocolHandle Ҫ�ﶨ��Protocol���
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/

/** 
brief Protocol regist handle function
attention :this function called by user, must after called after transcoder create function
	   when the source of HiTranscoder happens large change,need called HI_Protocol_UnRegistHandle then this
	   function,for example:when play local file,happens rewind,need play from begin,or dvb change channel.
param[in] HI_HANDLE hTranHandle  need regist Transcoder handle
                HI_HANDLE pProtocolHandle need regist Protocol handle
retval ::HI_SUCCESS   success
	 HI_FAILURE       failure
*/
HI_S32 HI_Protocol_RegistHandle(HI_HANDLE pProtocolHandle,HI_HANDLE hTranHandle);
/** 
brief Protocolȥע�ắ��
attention :�˺�������Transcoder_create����ܵ���,���û�������Դ�����ϴ�仯����Ҫ�ȵ��ñ�����
	   ���磺���ű����ļ�ʱ�������ļ�������ɣ��ִ�ͷ���ŵ����������dvb���ֻ�̨�������
param[in] HI_HANDLE hTranHandle Ҫȥ�󶨵�Transcoder�ľ��
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Protocol unregist handle function
attention :this function called by user, must after called after transcoder create function,
	   when the source of HiTranscoder happens large change,need called this function
	   for example:when play local file,happens rewind,need play from begin,or dvb change channel.
param[in] HI_HANDLE hTranHandle  need unregist Transcoder handle
              HI_HANDLE pProtocolHandle need unregist Protocol handle
retval ::HI_SUCCESS   success
            HI_FAILURE       failure
*/
HI_S32 HI_Protocol_UnRegistHandle(HI_HANDLE pProtocolHandle,HI_HANDLE hTranHandle);

/** 
brief Protocol ע���ⲿЭ�麯��
attention :�˺�������HI_Protocol_Init����ܵ���
param[in] HI_CHAR* pLibProtocolName Ҫע��Ŀ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Protocol regist external protocol server
attention :this function called by user, must after called after protocol init function
param[in] HI_CHAR* pLibProtocolName  need regist external .so name
retval ::HI_SUCCESS   success
            HI_FAILURE       failure
*/
HI_S32 HI_Protocol_RegistProtocol(const HI_CHAR* pLibProtocolName);

/** 
brief Protocol ȥע���ⲿЭ�麯��
attention :�˺�������HI_Protocol_Init����ܵ���
param[in] HI_CHAR* pLibProtocolName Ҫȥע��Ŀ�����
retval ::HI_SUCCESS �ɹ�
	 HI_FAILURE ʧ��
*/
/** 
brief Protocol unregist external protocol server
attention :this function called by user, must after called after protocol init function
param[in] HI_CHAR* pLibProtocolName  need unregist external .so name
retval ::HI_SUCCESS   success
            HI_FAILURE       failure
*/
HI_S32 HI_Protocol_DeRegistProtocol(const HI_CHAR* pLibProtocolName);

HI_S32 HI_Protocol_WFD_SetClientCommand(HI_HANDLE hProtocolHandle,HI_U32 u32Command,HI_BOOL bBufferFlag);

#ifdef __cplusplus
}
#endif


#endif
