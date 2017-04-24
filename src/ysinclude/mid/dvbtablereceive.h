/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbtablereceive.h

    Description : 标准表数据管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSSDTREC_
#define _YSSDTREC_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "msg.h"
#include "dvb.h"
#include "dvbtablemgr.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define RECEIVE_STANDARD_TABLE_TASK_MAX_MSGS			500		/*The most number of msg the buffer can hold*/
#define RECEIVE_STANDARD_TABLE_TASK_MSG_QUEUE_SIZE	DRVOS_MESSAGE_QUEUE_MEMSIZE(20, RECEIVE_STANDARD_TABLE_TASK_MAX_MSGS)
#define RECEIVE_STANDARD_TABLE_TASK_PARTITION_SIZE	(64*1024)
#define GET_RECEIVE_STANDARD_TABLE_TASK_MSG_TIMEOUT_MS	(20)
#define EITSD_ACTUAL_RECEIVE_TIMEOUT_TIME           DRV_OS_TimeGetTickPerSecond()/500
#define EITSD_OTHER_RECEIVE_TIMEOUT_TIME            DRV_OS_TimeGetTickPerSecond()/250

#ifndef GET_DATA_USE_POINTER_REPLACE
#define EPG_DATA_RECEIVE_BUFFER_SIZE                (1024*256)  //所有5119版本使用
#else
#define EPG_DATA_RECEIVE_BUFFER_SIZE                (1024)
#endif

/*DVB模块使用Filter个数限定*/
#define MID_MAX_REQUEST_NUMBER                      24

//标准表接收任务中一次接收PMT的个数
#define DVB_MAX_PMT_REQUEST_NUMBER                  15

/*EPG接收中所占Filter个数*/
#ifdef _AVIT_DBC_
#define EIT_MAX_SERVICE_REQUEST_NUMBER		        8
#else
#define EIT_MAX_SERVICE_REQUEST_NUMBER		        20
#endif

/*数据模块最多使用的FILTER总数*/
#define STANDARD_TABLE_MAX_REQUEST_NUMBER           20

/*定义DVB模块Msgcode*/
#define START_STANDARD_TABLE_RECEIVING			    (DVB_MSGCODE_BASE+0x01)
#define STOP_STANDARD_TABLE_RECEIVING				(DVB_MSGCODE_BASE+0x02)
#define FINISH_STANDARD_TABLE_RECEIVING				(DVB_MSGCODE_BASE+0x03)
#define FILTER_REPORT					            (DVB_MSGCODE_BASE+0x04)		
#define EIT_SCHEDULE_RECEIVE                        (DVB_MSGCODE_BASE+0x05)
#define EIT_SCHEDULE_RECEIVE_COMPLETE               (DVB_MSGCODE_BASE+0x06)
#define EIT_OVERDUE_DATA_TRIM						(DVB_MSGCODE_BASE+0x07)

//用于单子表多Setion处理
#define SIGLE_TABLE_MULTY_SECTION_REPORT            (DVB_MSGCODE_BASE+0x08)

//用于多子表多Section处理
#define MULTY_TABLE_MULTY_SECTION_REPORT            (DVB_MSGCODE_BASE+0x09)

//用于单FILTER 多Section 处理
#define MULTY_SECTION_REPORT                    	(DVB_MSGCODE_BASE+0x0a)
#define MULTY_SECTION_TIMEOUT                   	(DVB_MSGCODE_BASE+0x0b)
#define MULTY_SECTION_COMPLETE                  	(DVB_MSGCODE_BASE+0x0c)

/*定义标准表处理状态*/
#define SDT_TABLE_OTHER_PENGDING                    0x1000
#define PMT_TABLE_PENGDING			                0x0010
#define NIT_RECEIVE_MASK                            0x0001
#define PAT_RECEIVE_MASK                            0x0004                       
#define SDT_RECEIVE_MASK                            0x0001        
#define PMT_RECEIVE_MASK                            0x0004 

/*定义EIT表处理状态*/
#define EIT_TABLE_SCHEDULE_ACTUAL_PENDING				0x1000
#define EIT_TABLE_SCHEDULE_OTHER_PENDING				0x2000
#define EIT_TABLE_PRESENT_AND_FOLLOWING_ACTUAL_PENDING	0x4000
#define EIT_TABLE_PRESENT_AND_FOLLOWING_OTHER_PENDING	0x8000
#define EIT_TABLE_PENGDING								0xf000
        
/*定义EIT的PF和Schedule接收状态*/
#define EIT_SD_RECEIVE_MASK                         	0x0001
#define EIT_PF_RECEIVE_MASK                         	0x0002 

/*定义返回错误类型*/
#define FINISH_WITHOUT_ERROR                      	(DVB_ERROR_BASE+0x01)
#define FINISH_WITH_TIMEOUT_ERROR                 	(DVB_ERROR_BASE+0x02)
#define FINISH_WITH_CRC_ERROR                     	(DVB_ERROR_BASE+0x03)
#define FINISH_WITH_PARSE_ERROR                   	(DVB_ERROR_BASE+0x04)
#define FINISH_WITH_DATALOCK_FAILED_ERROR         	(DVB_ERROR_BASE+0x05)

/*定义返回中断类型*/
#define STOP_BY_OTHER                             	0x00001121
#define STOP_BY_SELF                              	0x00001122
#define STOP_BY_PRIORITY					        0x00001123
#define STOP_BY_LOCK_FAILED                       	0x00001124

/*定义表格接收组合*/
#define RECEIVETABLE_NONE                          	0x00000000  //NONE
#define RECEIVETABLE_ALL                           	0x55555555  //ALL
#define RECEIVETABLE_SYSSET                        	0x00000001	//NIT_ACTUAL
#define RECEIVETABLE_SYSSET_SDT                 	0x00004001  //NIT_ACTUAL,SDT_OTHER
#define RECEIVETABLE_SYSSET_BAT						0x00000401  //NIT_ACTUAL,BAT
#define RECEIVETABLE_SYSSET_BAT_SDT					0x00004401  //NIT_ACTUAL,BAT,SDT_OTHER

#define RECEIVETABLE_PSI_TIMING                    	0x00001150  //PAT,PMT,CAT,SDT_ACTUAL
#define RECEIVETABLE_CA_TIMING                     	0x00000150	//PAT,PMT,CAT
#define RECEIVETABLE_PSI_SIMPLE_TIMING             	0x10000150  //PAT,PMT,CAT,RST
#define RECEIVETABLE_PATPMT                        	0x00000050	//PAT,PMT
#define RECEIVETABLE_HANDWORK                      	0x10001150  //PAT,PMT,CAT,SDT_ACTUAL,RST
#define RECEIVETABLE_BAT_HANDWORK                  	0x00001550  //PAT,PMT,CAT,SDT_ACTUAL,BAT
#define RECEIVETABLE_BAT                           	0x00000400  //BAT
#define RECEIVETABLE_SDT_PAT_ACTUALL               	0x00001010  //PAT,SDT_ACTUAL
#define RECEIVETABLE_TIME_TIMING                   	0x01000000	//TDT

//后台更新NIT、BAT所用
#define RECEIVETABLE_NIT_FOR_UPDATE                	0x10000001  //NIT_ACTUAL
#define RECEIVETABLE_BAT_FOR_UPDATE                	0x10000400  //BAT

//多Filter接收EIT模式使用
#define RECEIVETABLE_SI_TIMING                     	0x00500000	//EIT_PF_ACTUAL,EIT_PF_OTHER
#define RECEIVETABLE_EPG                           	0x00410000  //EIT_SCHEDULE_ACTUAL,EIT_PF_OTHER

//单Filter接收EIT模式使用
#define RECEIVETABLE_EIT_ACTUAL						0x00110000  //EIT_SCHEDULE_ACTUAL,EIT_PF_ACTUAL
#define RECEIVETABLE_EIT_OTHER					   	0x00440000  //EIT_SCHEDULE_OTHER,EIT_PF_OTHER
#define RECEIVETABLE_PF         				   	0x10500000  //EIT_PF_ACTUAL,EIT_PF_OTHER
#define RECEIVETABLE_SCHEDULE       				0x00050000  //EIT_SCHEDULE_ACTUAL,EIT_SCHEDULE_OTHER
#define RECEIVETABLE_EIT                     		0x10550000  //EIT_SCHEDULE_ACTUAL,EIT_SCHEDULE_OTHER,EIT_PF_ACTUAL,EIT_PF_OTHER,RST

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum
{
	DVB_TASK_FREE,
	DVB_TASK_RUNNING
} db_module_state_t;

typedef enum
{	
	NIT_TABLE_ACTUAL,						/* build the actual NIT table */
	NIT_TABLE_OTHER,						/* build the other NIT table*/
	PAT_TABLE,								/* build PAT table */
	PMT_TABLE,								/* build PMT table */
	CAT_TABLE,								/* build CAT table */
	SDT_TABLE_ACTUAL,						/* build SDT table */
	SDT_TABLE_OTHER,						/* build the SDT table for other ts*/
	BAT_TABLE,								/* build BAT table */
	EIT_TABLE_SCHEDULE_ACTUAL,				/* build the actual schedule EIT table */
	EIT_TABLE_SCHEDULE_OTHER,				/* build the other schedule EIT table */
	EIT_TABLE_PRESENT_FOLLOWING_ACTUAL,		/* bulld the actual present and following EIT table */
	EIT_TABLE_PRESENT_FOLLOWING_OTHER,		/* bulld the other present and following EIT table */
	TDT_TABLE,								/* build TDT table */
	TOT_TABLE,								/* build TOT table */
	PRIVATE_TABLE,							/* build Private table*/
	EIT_TABLE,								/* build All EIT table*/
	EIT_ALL_PF_TABLE,						/* build PF_A and PF_O table*/
	EIT_ALL_SCHEDULE_TABLE,					/* build Schedule_A and Schedule_O table*/
	EIT_ACTUALL_TABLE,						/* build PF_A and Schedule_A table*/
	EIT_OTHER_TABLE,						/* build PF_O and Schedule O table*/
	NULL_TABLE								/* invalid table*/
} DVBStandardTableNextToRequest_t;

typedef struct DvbSiRequest 
{
	void *pData;			
	unsigned int uiReceiveTableState;	
	signed int siPriority;			
	DRVOS_Partition_t *pPtn;	
	DvbDelivery_t stDelivery;

	unsigned short usTsID;
	unsigned short usOriginalNetworkID;
	unsigned short usServiceID;
	DRVOS_MsgQueue_t *pstMsgQueue;
}DvbSiRequest_t;

typedef struct ReceiveTableState 
{
	unsigned int NIT_ACTUAL:2;
	unsigned int NIT_OTHER:2;
	unsigned int PAT:2;
	unsigned int PMT:2;
	unsigned int CAT:2;
	unsigned int BAT:2;
	unsigned int SDT_ACTUAL:2;
	unsigned int SDT_OTHER:2;
	unsigned int EIT_SCHEDULE_ACTUAL:2;
	unsigned int EIT_SCHEDULE_OTHER:2;
	unsigned int EIT_PF_ACTUAL:2;
	unsigned int EIT_PF_OTHER:2;
	unsigned int TDT:2;
	unsigned int TOT:2;
	unsigned int RST:2;
	unsigned int SIT:2;
} ReceiveTableState_t;

typedef	union TableState
{
	unsigned int uiTableState;
	ReceiveTableState_t tReceiveTableState;
}TableState_u;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan
  Function Name   : DVB_FreeReceiveSectionData
  Description     : 释放底层上报过来的数据内存
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_FreeReceiveSectionData(unsigned char *pData);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan  
  Function Name   : DVB_GetSectionInUseMask
  Description     : 获取标准表接收模块的m_uiSectionInUseMask值
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int DVB_GetSectionInUseMask(void);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_SendCommandToStandardTableReceiveTask
  Description     : 应用向标准表接收任务发送消息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_SendCommandToStandardTableReceiveTask(unsigned int nCmd);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_RequestStandardTableReceive
  Description     : 应用向标准表接收任务发送申请消息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_RequestStandardTableReceive(unsigned int uiReceiveTableState,signed int siPriority,void *pData,DRVOS_Partition_t *pPtn,DvbDelivery_t stDelivery,
									unsigned short usTsID,unsigned short usOriginalNetworkID,unsigned short usServiceID,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_ReturnStandardTableReceiveRequest
  Description     : 标准表接收任务向应用返回申请消息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ReturnStandardTableReceiveRequest(DvbSiRequest_t  *pDvbSiRequest,unsigned int uiReturnStatus,signed int siReturnType);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_DeleteSIOfStandardTableReceiveRequest
  Description     : 应用删除标准表接收任务返回的消息的pRequest
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_DeleteSIOfStandardTableReceiveRequest(MessageInfo_t *pMsg);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_StopStandardTableReceiveByTuner
  Description     : 同时停掉EPG和标准表接收任务的数据接收
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_StopStandardTableReceiveByTuner(void);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_CleanSIStatusForStandardTableReceive
  Description     : 清除标准表接收中SI的状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_CleanSIStatusForStandardTableReceive(DvbSI_t  *pDvbSi);

/*****************************************************************************
  Date&Time    :    2004-03-29
  Author          :    llh
  Function Name   : DVB_ReceiveStandardTableTaskInit
  Description     : 标准表接收任务初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DVB_ReceiveStandardTableTaskInit(void);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : DVB_ReceiveStandardTableTaskTerm
  Description     : 标准表接收任务注销
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_ReceiveStandardTableTaskTerm(void);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : STBSet_GetMidwarePartitionDebugSwitchStatus
  Description	  : 获取EIT和标准数据接收内存调试开关状态
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
BOOL STBSet_GetMidwarePartitionDebugSwitchStatus(void);

/*****************************************************************************
  Date&Time       : 2009_11_16   
  Author          :   xyan 
  Function Name   : STBSet_SetMidwarePartitionDebugSwitchStatus
  Description	  : 设置EIT和标准数据接收内存调试开关状态
  Input 		  : 
  Outut 		  :  
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/ 
void STBSet_SetMidwarePartitionDebugSwitchStatus(BOOL flag);


#endif

