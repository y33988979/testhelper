/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : midcas.h

    Description : CA模块对外使用的接口封装管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _MIDCAS_H__
#define _MIDCAS_H__

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "appys.h"
#include "midware.h"
#include "ca_filter.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
/*---------- ECM_PID设置的操作类型 ---------*/
#define MIDCAS_LIST_OK          0x00
#define MIDCAS_LIST_FIRST       0x01
#define MIDCAS_LIST_ADD         0x02

#define MIDCAS_SC_OUT 			1
#define MIDCAS_SC_IN 			2
#define MIDCAS_SC_INVALID 		3

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*最高的两位表示主版本，在重大修改后变更中间两位表示副版本，在添加新功能后变更
* 最后两位表示修改号，在修改BUG  后变更每次修改程序后都需要更改以下版本号
*
* 注意:  有两个地方需要修改，一是版本字符串，另一个是版本数值!!!
*/
typedef struct CAS_LibraryInfo_s
{
	unsigned char  Name[16];      	/* 模块名字 */
	unsigned char  VerString[16];  	/* 模块版本字符串 */
	unsigned char  Builder[32];    	/* 编译者机器名 */
	unsigned char  BuildDate[16];  	/* 编译日期 */
	unsigned char  BuildTime[16];  	/* 编译时间 */
	int SoftVersion;    			/* 模块版本 */
	int HardVersion;   				/* 硬件版本 */
	unsigned char  Reserved[24];   	/* 保留字段 */
}CAS_LibraryInfo;    /* 总长度为128字节 */

//主要用于同方CA、数码视讯CA,添加ECM信息时的数据结构
typedef struct _MIDCASServiceInfo{
    unsigned short  m_wEcmPid;   	/* 节目相应控制信息的PID */
    unsigned char   m_byServiceNum; /* 当前PID下的节目个数 */
    unsigned char   m_byReserved;   /* 保留 */
    unsigned short  m_wServiceID[4];/* 当前PID下的节目ID列表 */
}SMIDCASServiceInfo;

/*CA系统类型枚举，增加新的cas时，在此添加对应枚举*/
typedef enum
{
	UNKNOW_CA = 0,
	CDCAS30_CA = 1,
	SMSX_CA = 2,
}CAType_t;
/*----------------------------------------------------------------------------
 *	Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_AdpInit
  Description     : 启动CAS
  Input           : 
  Outut           : 1:启动成功, 0:启动失败
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midcas_AdpInit(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_Add_CAApp
  Description     : 注册CAS应用任务
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_Add_CAApp(int StartButton,int AtMenuOrder,char *AtMenuName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_Add_EmailApp
  Description     : 注册CAS邮件任务
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_Add_EmailApp(int StartButton, int AtMenuOrder, char *AtMenuName);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SendOSDMSG
  Description     : 向CA_ADPTask发消息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_SendOSDMSG(MSG_MsgInfo_t *MsgInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPTranslateMsg
  Description     : 将消息交由CAS判断是否需要转化为CAS消息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPTranslateMsg(MSG_MsgInfo_t *MsgInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPTVCHChange
  Description     : 节目切台函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPTVCHChange(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_ADPRecChange
  Description     : 录制节目切台函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_ADPRecChange(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_OSDHideBuyMsg
  Description     : 清除CAS提示信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void midcas_OSDHideBuyMsg(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_IsIntegCAID
  Description     : 判断是否同方CAS系统ID
  Input           : 
  Outut           : 0:不是同方的SystemID,1:是同方的SystemID
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int midcas_IsIntegCAID(unsigned short usCasID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetScStatus
  Description     : 判断智能卡状态
  Input           : 
  Outut           : 0:成功,1:无卡,2:卡在机顶盒中,3:卡无效
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetScStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetScCardID
  Description     : 获取智能卡卡号
  Input           : 
  Outut           : 0:成功,1:获取卡号失败,2:卡无效,3:传入的存放空间不足
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetScCardID(int iCardIDLength, char* pSmartCardID);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StopPrivateData
  Description     : 停CAS私有数据
  Input           : 0:停ECM,1:停EMM,2:同时停ECM和EMM
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 对于TFCAS21PROEX和TFCAS21PRO支持同时停ECM和EMM,对于CDCAS30只支持停ECM
*****************************************************************************/
int midcas_StopPrivateData(int iStopType);  

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StopPSIData
  Description     : 停止CA收PSI数据
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_StopPSIData(int iStopType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_StartPSIData
  Description     : 启动CA收PSI数据
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_StartPSIData(int iStartType);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_DeleteAllEmail
  Description     : 删除CAS所有邮件
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_DeleteAllEmail(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SetOldCW
  Description     : 设置以前的CW
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_SetOldCW(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_SetInvalidCW
  Description     : 设置无效的CW
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_SetInvalidCW(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetProductID
  Description     : 获得授权的ProductID列表
  Input           : 
  Outut           : 0:获取失败,1:获取成功,2:传入的存储空间不足
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int midcas_GetProductID(int MaxCount, int *ProductSum);
        
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetOperatorIds
  Description     : 获得运营商列表
  Input           : 
  Outut           : 0:获取失败,1:获取成功
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetOperatorIds(int MaxOperatorCount, unsigned short* pusOperatorIds);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetACList
  Description     : 获得运营商的特征值
  Input           : 
  Outut           : 0:获取失败,1:获取成功
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetACList(unsigned short usTVSID, unsigned long* pACArrayInfo);

/*************************************************
* Function:       midcas_SetEcmPid
* 
* Input:            输入的Type在midcas.h中定义
*
* Description:    设置ECM PID
*
* Input:  
* Output:        
* Return:         无
*
* Others:         接口编号24
*************************************************/
void  midcas_SetEcmPid(int Type, SMIDCASServiceInfo* pServiceInfo);

/*************************************************
* Function:       midcas_SetEmmPid
* 
* Input:           EMM PID
*
* Description:    设置EMM PID
*
* Input:  
* Output:        
* Return:         无
*
* Others:         接口编号25
*************************************************/
void  midcas_SetEmmPid(unsigned short usEmmPid);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_PrintCAVersion
  Description     : 打印CA版本号
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_PrintCAVersion(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetCAVersion
  Description     : 获取CAS版本号
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned int midcas_GetCAVersion(void);

/*****************************************************************************
  Date&Time    	  : 2012-12-26
  Author          :   wwqing
  Function Name   : midcas_GetCASName
  Description     : 获取CA系统名字
  Input           : ucCasName 保存数据的内存地址，CasNameLen 可用于保存数据的长度
  Outut           : 成功返回1，失败返回0
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char midcas_GetCASName(char *ucCasName, unsigned char ucCasNameLen);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : midcas_GetPlatformID
  Description     : 获取CA平台编号
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned short midcas_GetPlatformID(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_TimeTransform
  Description     : 时间转换
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_TimeTransform(unsigned short CATime, StbTm_t *TransTime);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_WriteCASCUpInfoAddr
  Description     : 写智能卡升级成功信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_WriteCASCUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          : wpeng
  Function Name   : midcas_ReadCASCUpInfoAddr
  Description     : 读取智能卡升级信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_ReadCASCUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time       : 2012-12-26
  Author          : wwqing
  Function Name   : midcas_GetCasInitStatus
  Description     : 获取CA系统初始化状态
  Input           : 
  Outut           :  1，初始化完成，0，未初使化
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char midcas_GetCasInitStatus(void);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_UpdateSmsxPdsdData
  Description     : 更新数码视讯NIT区域锁定功能使用的pdsd数据，如果pdsd与e2p中的不同，
  					则调用DVTSTBCA_GetNitValue()告知ca
  Input           : uiPdsd
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void midcas_UpdateSmsxPdsdData(unsigned int uiPdsd);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_SetOSDRunTime
  Description     : 设置smsx ca osd滚动显示时间
  Input           : Time
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 数码ca使用
*****************************************************************************/
void midcas_SetOSDRunTime(unsigned int Time);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_GetDescrambleEcmPid
  Description     : CA获取解扰器EcmPid
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : tfca使用
*****************************************************************************/
unsigned short midcas_GetDescrambleEcmPid(DescrambleType_e DescrambleType);

/*****************************************************************************
  Date&Time       : 2013-01-10
  Author          : wwqing
  Function Name   : midcas_GetCASType
  Description     : 获取cas系统的类型
  Input           : 返回 CAType_t
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 其它模块使用此接口，以避免使用ca宏，使其它模块独立于ca模块
*****************************************************************************/
CAType_t midcas_GetCASType(void);

#endif //end of _MIDCAS_H__


