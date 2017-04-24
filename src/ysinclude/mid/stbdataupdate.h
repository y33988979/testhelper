/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataupdate.h

    Description : 后台数据更新管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdata_Standard_
#define _YSstbdata_Standard_
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define ManufacturerCode_JIUZHOU        202
#define TFCAS_CODEDOWNLOAD_DESCRIPTOR   0xA1
#define TFCAS_JZ_ID                     0x03df
#define DTVIA_TAG                       0x93
#define SMSX_NIT_LOCK_DESCRIPTOR        0x5f

#define UPDATE_POWEROFF_DLG_TIME	    60
#define RECEIVE_TDT_MAX_TIMES           3

/*设置后台更新数据(默认更新TDT、EIT、九州升级和NIT,开发人员根据具体情况修改)*/
#define UPDATE_DEFAULT_DATA				31  

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct NITDataSection
{
    unsigned char ucNitSection[1024];
    unsigned short usSectionLen;
    BOOL bInUsed;
}NITDataSection_t;

typedef enum UpdateDataType
{
	UPDATE_INVALID_DATA=0,	/*后台不更新数据*/
	UPDATE_TDT_DATA=1,      /*后台更新TDT数据*/
	UPDATE_EIT_DATA=2,      /*后台更新EIT数据*/
	UPDATE_NIT_DATA=4,      /*后台更新NIT数据*/
	UPDATE_JZUPGRADE_DATA=8,/*后台更新九州升级信息*/
	UPDATE_CAT_DATA=16,     /*后台更新CAT数据*/
	UPDATE_BAT_DATA=32      /*后台更新BAT数据*/
	/*剩下的保留*/
}UpdateDataType_e;

typedef enum DataChangeType
{
	NO_DATA_CHANGE = 0,
	CA_PARA_CHANGE = 1,
	AV_PARA_CHANGE = 2,
	PROG_CHANGE = 3
}DataChangeType_e;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : StartReceiveNIT
  Description     : 开始更新NIT 数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void StartReceiveNIT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : FinishReceiveNIT
  Description     : 结束更新NIT 数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void FinishReceiveNIT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITReceiveStatus
  Description     : 获取更新NIT 数据状态
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetNITReceiveStatus(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : IsNITReceived
  Description     : 获取更新NIT 结束状态
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL IsNITReceived(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITDataLength
  Description     : 获取NIT 数据长度(用于同方CA区域控制功能)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short GetNITDataLength(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITData
  Description     : 获取NIT 数据(同方CA没有收到NIT获取假数据)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char *GetNITData(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetRegionNetworkID
  Description     : 获取NetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short GetRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetRegionNetworkID
  Description     : 设置NetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetRegionNetworkID(unsigned short usNetworkID);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_WriteRegionNetworkID
  Description     : 将NetworkID写入E2P
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_WriteRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_ReadRegionNetworkID
  Description     : 从E2P中读取NetworkID
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_ReadRegionNetworkID(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITVersionChangedTag
  Description     : 获取NIT版本号是否改变标示
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetNITVersionChangedTag(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetNITVersionChangedTag
  Description     : 设置NIT版本号是否改变标示
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetNITVersionChangedTag(BOOL tag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNITVersion
  Description     : 获取NIT版本号
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char GetNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetNITVersion
  Description     : 设置NIT版本号
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetNITVersion(unsigned char version);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_WriteNITVersion
  Description     : 将NIT版本号写入E2P
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_WriteNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : NVM_ReadNITVersion
  Description     : 从E2P读出NIT版本号
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void NVM_ReadNITVersion(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateNIT
  Description     : 后台更新NIT(可以打断的)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateNIT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateNITForSTBStart
  Description     : 开机时接收NIT (不可打断的)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL UpdateNITForSTBStart(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateEITForSTBStart
  Description     : 启动时收取EPG数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateEITForSTBStart(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateEIT
  Description     : 后台更新EIT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateEIT( DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : EITReceiveComplete
  Description     : 判断启动时收取EPG数据是否结束
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL EITReceiveComplete(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateTDT
  Description     : 后台更新TDT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateTDT(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateTDTStart
  Description     : 启动时更新TDT
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateTDTStart(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : TDTReceived
  Description     : 判断TDT是否收到
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL TDTReceived(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateBAT
  Description     : 后台更新BAT数据(用于更新BAT分类信息)
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 注意!此函数只能更新BAT分类信息,所以在申请BAT接收的时候,只能
  					申请为分类信息的BAT.
*****************************************************************************/ 
void UpdateBAT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateCAT
  Description     : 后台更新PAT、PMT、CAT、SDT数据
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateCAT(DRVOS_Partition_t *pPtn,DRVOS_MsgQueue_t *pstMsgQueue);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetSTBStandby
  Description     : 进入待机状态
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SetSTBStandby(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdateSTBLedDisplayTime
  Description     : 更新机顶盒待机状态下的时间显示
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateSTBLedDisplayTime(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UpdatePowerOffProcess
  Description     : 后台检测自动开关机
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdatePowerOffProcess(void);

#endif

