/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : usbmng.h

    Description : 

    Others:      

    History:  1. xyan creat 2011-9-20


*******************************************************************************/
#ifndef	_YSMHP_
#define	_YSMHP_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "ysplayrec.h"
#include "ysoslist.h"
#include "midware.h"
#include "usb.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define USB_DISK_NAME_MAX_LENGTH			60
#define USB_PART_NAME_MAX_LENGTH			60
#define USB_FILE_NAME_MAX_LENGTH			256
#define USB_MAX_MOUNT_NUMBER				16
#define USB_MAX_PART_MOUNT_NUMBER			16
#define USBAPP_MAX_DISK_PATH_LENGTH			40
#define PVR_PROG_NAME_MAX_LENGTH			50
#define PVR_PROG_PATH_MAX_LENGTH			512

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

typedef enum
{
	USBAPP_FILE_NODE=0,	//文件节点
	USBAPP_DIR_NODE=1,	//目录节点
	USBAPP_NODE_COUNT
}NodeFlag_t;

typedef enum
{
	USB_INVALID_PROCESS,
	USB_RECORD_PROCESS,		//USB录制处理
	USB_PLAY_PROCESS,		//USB播放处理
	USB_ERROR_PROCESS,		//USB错误处理
	USB_PROCESS_COUNT
}Usb_Manager_Process_t;

typedef enum
{
	USB_RECORD_INVALID,
	USB_RECORD_START,		//开始录制
	USB_RECORD_PAUSE,		//暂停录制
	USB_RECORD_RESUME,		//恢复录制
	USB_RECORD_STOP,		//停止录制
	USB_RECORD_COUNT
}Usb_Record_Process_t;

typedef enum
{
	USB_PLAY_INVALID,
	USB_PLAY_START,			//开始播放
	USB_PLAY_PAUSE,			//暂停播放
	USB_PLAY_RESUME,		//暂停恢复播放
	USB_PLAY_STOP,			//停止播放
	USB_PLAY_FORWARD,		//快进播放
	USB_PLAY_BACKWARD,		//快退播放
	USB_PLAY_NORMALWARD,	//速度恢复播放
	USB_PLAY_SEEK,	        //选时播放
	USB_PLAY_COUNT
}Usb_Play_Process_t;

typedef enum
{
	USB_INVALID_ERROR,
	USB_RECORD_ERROR,		//录制错误
	USB_PLAY_ERROR,			//播放错误
	USB_USB_ERROR,			//USB错误
	USB_ERROR_COUNT
}Usb_Error_Process_t;

typedef enum
{
	USB_NO_ERROR,
	USB_PLAY_REACH_END,			//节目播放到达尾部 1
	USB_PLAY_REACH_BEGIN,		//节目播放到达头部 2
	USB_PLAY_DECODE_ERROR,		//节目播放出现错误 3
	USB_PLAY_FILE_FIND_FAILED, 	//播放文件没有找到 4
	USB_PLAY_READ_FILE_FAILED, 	//播放文件读取失败 5
	USB_PLAY_SEEK_FAILED,		//播放文件跳转失败 6
	USB_RECORD_WRITE_FAILED,	//录制中写文件失败 7
	USB_RECORD_FINISH,			//录制文件完成 8
	USB_PLAY_REACH_LIVE,		//时移追上直播 9
	USB_RECORD_SPACE_NOT_ENOUGH,//录制磁盘空间不足 10
	USB_RECORD_FAILED,			//录制失败 11
	USB_PLAY_FAILED,			//播放失败 12
	USB_RECORDING,				//录制中 13
	USB_STOP_PVR_BY_USBOUT,		//PVR设定Usb拔出而停止PVR 14
	USB_EXIT_APP_BY_USBOUT,		//Usb应用使用Usb拔出而退出 15
	USB_RECORD_CONFLICT_WITH_CHANGE_TS,//录制时切频冲突 16
	USB_INSERT,	                //U盘插入 17
	USB_TIMESHIFT_BEGIN,        //进入时移 18
	USB_MNGTASK_ERROR_COUNT
}Usb_Manager_Task_Error_t;

typedef enum
{
	DLG_PROCESS_BASE = 0x12345,
	DLG_PROCESS_NONE,			//不需要应用处理的Dlg
	DLG_PROCESS_REFRESH,		//需要应用处理的刷新Dlg
	DLG_PROCESS_RETURN,			//需要应用处理的返回Dlg
	DLG_PROCESS_EXIT,			//需要应用处理的退出Dlg
	USB_DLG_PROCESS_COUNT
}Dlg_Process_Type_t;

typedef enum
{
	USB_TASK_FREE_STATUS,	//USB空闲状态
	USB_TASK_RECORD_STATUS,	//USB录制状态
	USB_TASK_PLAY_STATUS,	//USB播放状态
	USB_TASK_STATUS_COUNT
}Usb_Manager_Task_Status_t;

typedef enum
{
	USB_PLAY_VIDEO,	//USB播放视频模式
	USB_PLAY_AUDIO,	//USB播放音频模式
	USB_PLAY_MODE_COUNT
}Usb_Play_Mode_t;

typedef enum
{
	PVR_RECORD_ICON_HIDE,	//录制图标隐藏
	PVR_RECORD_ICON_SHOW,	//录制图标显示
	PVR_RECORD_ICON_COUNT
}Pvr_Record_Icon_Status_t;

typedef enum 
{
	USB_USER_MSG_INVALID = USB_USER_BASE,
	USB_USER_PLAY_MP3_REACH_END,/*MP3播放结束消息*/
	USB_USER_COUNT
}USB_UserMsg_e;

typedef enum 
{
	PVR_USER_MSG_INVALID = PVR_USER_BASE,
	PVR_USER_COUNT
}PVR_UserMsg_e;

typedef enum
{
	PLAYID_DEFAULT = 0,	//PVR以外的palyid
	PLAYID_PVR = 1,	//PVR专用
	PLAYID_DEFAULT_COUNT
}PLAYID_t;

typedef struct FileInfo
{
	unsigned char 	NodeFlag;			//节点类型:0为文件节点,1为目录节点
	unsigned char	FileType;			//文件类型(文件节点有效)
	char 			UnitName[USB_FILE_NAME_MAX_LENGTH];	//逻辑文件名字(ntfs utf8)
	char 			FileName[USB_FILE_NAME_MAX_LENGTH];	//显示文件名字(ntfs gb2312)
	DRV_VfsUnitMode_t FileMode;
	unsigned int 	FileSize;			//文件大小
	unsigned int	FileCTime;			//文件创建时间
	unsigned int	FileMtime;			//文件修改时间
}FileInfo_t;

/*保存录制节目信息结构*/
typedef struct PvrProgRecordInfo
{
	unsigned short 	usChannelNo;
	unsigned short	usServiceNo;
	unsigned char 	ucServiceType;
	unsigned char 	ucProgName[PVR_PROG_NAME_MAX_LENGTH];
	unsigned int 	uiStartRecTime;
	unsigned int 	uiRecordTime;
	unsigned char 	ucPidNum;
	DRV_PlayRecStreamData_t Pids[DRV_PLAYREC_MAX_PIDS];
}PvrProgRecordInfo_t;

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time 	  : 2012-10-26
  Author		  :   wwqing
  Function Name   : Usb_CheckFileName
  Description	  : 从输入的文件全名中查找非法字符
  Input 		  : 文件全名指针
  Outut 		  : 无非法字符返回TRUE, 有非法字符 返回 False
  Calls 		  : 
  Called by 	  :
  Global Variables: 
  Others		  : 
*****************************************************************************/
bool Usb_CheckFileName(char *FileName);

/*****************************************************************************
  Date&Time 	  : 2009-04-24
  Author		  :   xyan
  Function Name   : SetCurrentPlayID
  Description	  : 设置当前使用的PlayID
  Input 		  :
  Outut 		  :
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  : 
*****************************************************************************/
void SetCurrentPlayID(unsigned int Playid);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetCurrentRecordTsNo
  Description     : 获取当前节目录制的TsNo
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 
*****************************************************************************/
unsigned short GetCurrentRecordTsNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetCurrentRecordTsNo
  Description     : 设置当前节目录制的TsNo
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 
*****************************************************************************/
void SetCurrentRecordTsNo(unsigned short usTsNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPvrUseDiskNo
  Description     : 获取PVR保存所使用的磁盘号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 返回的磁盘号为底层磁盘devinfo数组下标
*****************************************************************************/
unsigned char GetPvrUseDiskNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetPvrUseDiskNo
  Description     : 设置PVR保存所使用的磁盘号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 设置的磁盘号为底层磁盘devinfo数组下标
*****************************************************************************/
void SetPvrUseDiskNo(unsigned char ucDiskNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPvrUsePartNo
  Description     : 获取PVR保存所使用的分区号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 返回的分区号为底层分区数组下标
*****************************************************************************/
unsigned char GetPvrUsePartNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetPvrUsePartNo
  Description     : 设置PVR保存所使用的分区号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 设置的分区号为底层分区数组下标
*****************************************************************************/
void SetPvrUsePartNo(unsigned char ucPartNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbAppUseDiskNo
  Description     : 获取UsbApp所使用的磁盘号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 返回的磁盘号为底层磁盘devinfo数组下标
*****************************************************************************/
signed char GetUsbAppUseDiskNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetUsbAppUseDiskNo
  Description     : 设置UsbApps所使用的磁盘号
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : 设置的磁盘号为底层磁盘devinfo数组下标
*****************************************************************************/
void SetUsbAppUseDiskNo(signed char scDiskNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetMountDiskNumber
  Description     : 获取已经挂载的磁盘总个数
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
int GetMountDiskNumber(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbListInit
  Description     : USB磁盘、分区、文件链表初始化
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbListInit(DRVOS_Partition_t *Partition);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbListTerm
  Description     : 删除USB磁盘、分区、文件链表
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbListTerm(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbDiskList
  Description     : 获取Disk链表指针
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
list_t *GetUsbDiskList(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbPartList
  Description     : 获取Part链表指针
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
list_t *GetUsbPartList(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbFileList
  Description     : 获取File链表指针
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
list_t *GetUsbFileList(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetDiskInfoByDiskIndex
  Description     : 通过传入的DiskIndex到DiskList链表中获取对应磁盘的信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
DiskInfo_t *GetDiskInfoByDiskIndex(int DiskIndex);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetDiskInfoByDiskDevNo
  Description     : 通过传入的DiskDevNo到DiskList链表中获取对应磁盘的信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : DiskDevNo对应底层磁盘devinfo数组下标
*****************************************************************************/
DiskInfo_t *GetDiskInfoByDiskDevNo(int DiskDevNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPartInfoByPartIndex
  Description     : 通过传入的PartIndex到PartList链表中获取对应分区的信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
PartInfo_t *GetPartInfoByPartIndex(int PartIndex);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPartInfoByPartNo
  Description     : 通过传入的PartNo到PartList链表中获取对应分区的信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
PartInfo_t *GetPartInfoByPartNo(int PartNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPartSymbol
  Description     : 获取分区对应盘符
  Input           : DiskNo:对应底层磁盘数组下标;PartNo:对应底层分区数组下标
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
char GetPartSymbol(int DiskNo,int PartNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : CheckSymbolValid
  Description     : 校核盘符有效性
  Input           : Symbol:传入盘符
  Outut           : TRUE:有效;FALSE:无效
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL CheckSymbolValid(char Symbol);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : CheckFileListNameRepeat
  Description     : 对比文件链表中是否存在与输入名字一样的节点
  Input           : 文件链表指针,名字指针
  Outut           : TRUE为文件链表中已有该名字的文件,FALSE为没有该名字的文件
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL CheckFileListNameRepeat(list_t *FileList,char *Name);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetFileInfoByName
  Description     : 通过传入的Name到FileList链表中获取对应文件信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
FileInfo_t *GetFileInfoByName(char *Name);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetFileTypeByName
  Description     : 通过文件名字获取文件类型
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
unsigned char GetFileTypeByName(char *FileName);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  xyan
  Function Name   : UsbDiskListInit
  Description     : 初始化Usb磁盘设备链表
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbDiskListInit(DRVOS_Partition_t *Partition);

/*****************************************************************************
  Date&Time       : 2011-08-24
  Author          :  xyan
  Function Name   : UsbPartListInit
  Description     : 初始化Usb磁盘下分区链表
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbPartListInit(DRVOS_Partition_t *Partition,int DiskIndex);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbFileListInit
  Description     : 初始化目录下文件链表
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbFileListInit(DRVOS_Partition_t *Partition,char *vfspath,unsigned char showmode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :    xyan
  Function Name   : JudgeUsbPathRoot
  Description     : 判断是否为根目录
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL JudgeUsbPathRoot(char *Path);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbSetPrePath
  Description     : 将路径设置为上一级目录路径
  Input           : 设置的路径
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbSetPrePath(char *Path);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbSetRootPath
  Description     : 将路径设置为根目录路径
  Input           : 设置的路径
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbSetRootPath(char *Path);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbPathDepth
  Description     : 获取路径深度
  Input           : 文件路径
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
int GetUsbPathDepth(char *Path);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : JudgeUsbManagerStatus
  Description     : 判断Usb管理任务运行状态
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL JudgeUsbManagerStatus(Usb_Manager_Task_Status_t Stutus);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : CheckPvrUsbValid
  Description     : 检查PVR设置的Usb及分区是否有效
  Input           :
  Outut           : 0为有效,1为无挂载Usb,2为Pvr设定磁盘无效,3为Pvr设定分区无效
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
int CheckPvrUsbValid(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   wpeng
  Function Name   : SetTimeShiftStatus
  Description     : 设置时移状态标志
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :TRUE:时移 FALSE:录制
*****************************************************************************/
void SetTimeShiftStatus(BOOL Status);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbPlayMode
  Description     : 获取USB的播放模式
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
Usb_Play_Mode_t GetUsbPlayMode(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetUsbPlaySeekTime
  Description     : 设置选时播放时间
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void SetUsbPlaySeekTime(unsigned int TimeInms);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbPlayStatus
  Description     : 获取USB的播放状态参数信息
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL GetUsbPlayStatus(U32 PlayId,DRV_PlayRecPlaybackStatus_t* Status,U32* TatalTimeInms_p,U32* BeginTimeInms_p,U32* EndTimeInms_p,U32* CurrentTimeInms_p,S32* Speed);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : PvrRecordParaSet
  Description     : 设置PVR录制结构
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void PvrRecordParaSet(DRV_PlayRecStartRecordParams_t *pSetPara);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : PvrRecordInfoSet
  Description     : 设置PVR录制信息结构
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void PvrRecordInfoSet(PvrProgRecordInfo_t *pSetInfo);

/*****************************************************************************
  Date&Time       : 2011-09-27
  Author          :    wpeng
  Function Name   : PvrRecordSetParams
  Description     : 设置录制参数
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL PvrRecordSetParams(DRV_PlayRecStartRecordParams_t *pSetPara, unsigned int TimeTotalMin, unsigned int SizeLimitInMB);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : PvrReadRecProgInfo
  Description     : 读取PVR节目信息文件
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL PvrReadRecProgInfo(char *FilePath,PvrProgRecordInfo_t *PvrProgRecInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : PvrRecordConflictDlgInit
  Description     : 切频与Pvr录制频点冲突Dlg初始化
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void PvrRecordConflictDlgInit(DlgInfo_t *pDlgInfo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SendMsgToUsbManagerTask
  Description     : 发送消息到Usb管理任务
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL SendMsgToUsbManagerTask(MSG_MsgInfo_t *pMsg);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SendStopRecordCommandToUsbMngTask
  Description     : 发送停止录制消息到Usb管理任务
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void SendStopRecordCommandToUsbMngTask(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPvrComponentType
  Description     : 将StreamType转换成Pvr的类型
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
DRV_PlayRecStreamType_t GetPvrComponentType(unsigned char StreamType);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbPlayVideoParaSet
  Description     : 设置视频播放结构
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbPlayVideoParaSet(DRV_PlayRecStartPlayParams_t *pSetPara);

/*****************************************************************************
  Date&Time       : 2013-07-11
  Author          :   hjchao
  Function Name   : g_CurPlayPara
  Description     : 为当前播放参数设置回调函数
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbCallbackSet(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbPlayAudioParaSet
  Description     : 设置音频播放结构
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbPlayAudioParaSet(STAUD_StartParams_t *pSetPara,DRV_AudioInjectSource *pDataSource);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceStartRecord
  Description     : 强制启动节目录制
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceStartRecord(unsigned int RecordId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceStopRecord
  Description     : 强制停止节目录制
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceStopRecord(unsigned int RecordId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceStartPlay
  Description     : 强制启动音视频播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL UsbForceStartPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceStopPlay
  Description     : 强制停止节目播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL UsbForceStopPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForcePausePlay
  Description     : 强制暂停节目播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForcePausePlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceResumePlay
  Description     : 强制恢复节目播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceResumePlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceForwardPlay
  Description     : 强制快进播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceForwardPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceBackwardPlay
  Description     : 强制快退播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceBackwardPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceNormalwardPlay
  Description     : 强制从快进快退中恢复播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceNormalwardPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbForceSeekPlay
  Description     : 强制选时播放
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbForceSeekPlay(unsigned int PlayId);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbManagerTaskInit
  Description     : USB管理任务初始化
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
BOOL UsbManagerTaskInit(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : UsbManagerTaskTerm
  Description     : 结束USB管理任务
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbManagerTaskTerm(void);

#endif
