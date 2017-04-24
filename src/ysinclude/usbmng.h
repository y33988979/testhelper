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
	USBAPP_FILE_NODE=0,	//�ļ��ڵ�
	USBAPP_DIR_NODE=1,	//Ŀ¼�ڵ�
	USBAPP_NODE_COUNT
}NodeFlag_t;

typedef enum
{
	USB_INVALID_PROCESS,
	USB_RECORD_PROCESS,		//USB¼�ƴ���
	USB_PLAY_PROCESS,		//USB���Ŵ���
	USB_ERROR_PROCESS,		//USB������
	USB_PROCESS_COUNT
}Usb_Manager_Process_t;

typedef enum
{
	USB_RECORD_INVALID,
	USB_RECORD_START,		//��ʼ¼��
	USB_RECORD_PAUSE,		//��ͣ¼��
	USB_RECORD_RESUME,		//�ָ�¼��
	USB_RECORD_STOP,		//ֹͣ¼��
	USB_RECORD_COUNT
}Usb_Record_Process_t;

typedef enum
{
	USB_PLAY_INVALID,
	USB_PLAY_START,			//��ʼ����
	USB_PLAY_PAUSE,			//��ͣ����
	USB_PLAY_RESUME,		//��ͣ�ָ�����
	USB_PLAY_STOP,			//ֹͣ����
	USB_PLAY_FORWARD,		//�������
	USB_PLAY_BACKWARD,		//���˲���
	USB_PLAY_NORMALWARD,	//�ٶȻָ�����
	USB_PLAY_SEEK,	        //ѡʱ����
	USB_PLAY_COUNT
}Usb_Play_Process_t;

typedef enum
{
	USB_INVALID_ERROR,
	USB_RECORD_ERROR,		//¼�ƴ���
	USB_PLAY_ERROR,			//���Ŵ���
	USB_USB_ERROR,			//USB����
	USB_ERROR_COUNT
}Usb_Error_Process_t;

typedef enum
{
	USB_NO_ERROR,
	USB_PLAY_REACH_END,			//��Ŀ���ŵ���β�� 1
	USB_PLAY_REACH_BEGIN,		//��Ŀ���ŵ���ͷ�� 2
	USB_PLAY_DECODE_ERROR,		//��Ŀ���ų��ִ��� 3
	USB_PLAY_FILE_FIND_FAILED, 	//�����ļ�û���ҵ� 4
	USB_PLAY_READ_FILE_FAILED, 	//�����ļ���ȡʧ�� 5
	USB_PLAY_SEEK_FAILED,		//�����ļ���תʧ�� 6
	USB_RECORD_WRITE_FAILED,	//¼����д�ļ�ʧ�� 7
	USB_RECORD_FINISH,			//¼���ļ���� 8
	USB_PLAY_REACH_LIVE,		//ʱ��׷��ֱ�� 9
	USB_RECORD_SPACE_NOT_ENOUGH,//¼�ƴ��̿ռ䲻�� 10
	USB_RECORD_FAILED,			//¼��ʧ�� 11
	USB_PLAY_FAILED,			//����ʧ�� 12
	USB_RECORDING,				//¼���� 13
	USB_STOP_PVR_BY_USBOUT,		//PVR�趨Usb�γ���ֹͣPVR 14
	USB_EXIT_APP_BY_USBOUT,		//UsbӦ��ʹ��Usb�γ����˳� 15
	USB_RECORD_CONFLICT_WITH_CHANGE_TS,//¼��ʱ��Ƶ��ͻ 16
	USB_INSERT,	                //U�̲��� 17
	USB_TIMESHIFT_BEGIN,        //����ʱ�� 18
	USB_MNGTASK_ERROR_COUNT
}Usb_Manager_Task_Error_t;

typedef enum
{
	DLG_PROCESS_BASE = 0x12345,
	DLG_PROCESS_NONE,			//����ҪӦ�ô����Dlg
	DLG_PROCESS_REFRESH,		//��ҪӦ�ô����ˢ��Dlg
	DLG_PROCESS_RETURN,			//��ҪӦ�ô���ķ���Dlg
	DLG_PROCESS_EXIT,			//��ҪӦ�ô�����˳�Dlg
	USB_DLG_PROCESS_COUNT
}Dlg_Process_Type_t;

typedef enum
{
	USB_TASK_FREE_STATUS,	//USB����״̬
	USB_TASK_RECORD_STATUS,	//USB¼��״̬
	USB_TASK_PLAY_STATUS,	//USB����״̬
	USB_TASK_STATUS_COUNT
}Usb_Manager_Task_Status_t;

typedef enum
{
	USB_PLAY_VIDEO,	//USB������Ƶģʽ
	USB_PLAY_AUDIO,	//USB������Ƶģʽ
	USB_PLAY_MODE_COUNT
}Usb_Play_Mode_t;

typedef enum
{
	PVR_RECORD_ICON_HIDE,	//¼��ͼ������
	PVR_RECORD_ICON_SHOW,	//¼��ͼ����ʾ
	PVR_RECORD_ICON_COUNT
}Pvr_Record_Icon_Status_t;

typedef enum 
{
	USB_USER_MSG_INVALID = USB_USER_BASE,
	USB_USER_PLAY_MP3_REACH_END,/*MP3���Ž�����Ϣ*/
	USB_USER_COUNT
}USB_UserMsg_e;

typedef enum 
{
	PVR_USER_MSG_INVALID = PVR_USER_BASE,
	PVR_USER_COUNT
}PVR_UserMsg_e;

typedef enum
{
	PLAYID_DEFAULT = 0,	//PVR�����palyid
	PLAYID_PVR = 1,	//PVRר��
	PLAYID_DEFAULT_COUNT
}PLAYID_t;

typedef struct FileInfo
{
	unsigned char 	NodeFlag;			//�ڵ�����:0Ϊ�ļ��ڵ�,1ΪĿ¼�ڵ�
	unsigned char	FileType;			//�ļ�����(�ļ��ڵ���Ч)
	char 			UnitName[USB_FILE_NAME_MAX_LENGTH];	//�߼��ļ�����(ntfs utf8)
	char 			FileName[USB_FILE_NAME_MAX_LENGTH];	//��ʾ�ļ�����(ntfs gb2312)
	DRV_VfsUnitMode_t FileMode;
	unsigned int 	FileSize;			//�ļ���С
	unsigned int	FileCTime;			//�ļ�����ʱ��
	unsigned int	FileMtime;			//�ļ��޸�ʱ��
}FileInfo_t;

/*����¼�ƽ�Ŀ��Ϣ�ṹ*/
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
  Description	  : ��������ļ�ȫ���в��ҷǷ��ַ�
  Input 		  : �ļ�ȫ��ָ��
  Outut 		  : �޷Ƿ��ַ�����TRUE, �зǷ��ַ� ���� False
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
  Description	  : ���õ�ǰʹ�õ�PlayID
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
  Description     : ��ȡ��ǰ��Ŀ¼�Ƶ�TsNo
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
  Description     : ���õ�ǰ��Ŀ¼�Ƶ�TsNo
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
  Description     : ��ȡPVR������ʹ�õĴ��̺�
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���صĴ��̺�Ϊ�ײ����devinfo�����±�
*****************************************************************************/
unsigned char GetPvrUseDiskNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetPvrUseDiskNo
  Description     : ����PVR������ʹ�õĴ��̺�
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���õĴ��̺�Ϊ�ײ����devinfo�����±�
*****************************************************************************/
void SetPvrUseDiskNo(unsigned char ucDiskNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPvrUsePartNo
  Description     : ��ȡPVR������ʹ�õķ�����
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���صķ�����Ϊ�ײ���������±�
*****************************************************************************/
unsigned char GetPvrUsePartNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetPvrUsePartNo
  Description     : ����PVR������ʹ�õķ�����
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���õķ�����Ϊ�ײ���������±�
*****************************************************************************/
void SetPvrUsePartNo(unsigned char ucPartNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbAppUseDiskNo
  Description     : ��ȡUsbApp��ʹ�õĴ��̺�
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���صĴ��̺�Ϊ�ײ����devinfo�����±�
*****************************************************************************/
signed char GetUsbAppUseDiskNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetUsbAppUseDiskNo
  Description     : ����UsbApps��ʹ�õĴ��̺�
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : ���õĴ��̺�Ϊ�ײ����devinfo�����±�
*****************************************************************************/
void SetUsbAppUseDiskNo(signed char scDiskNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetMountDiskNumber
  Description     : ��ȡ�Ѿ����صĴ����ܸ���
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
  Description     : USB���̡��������ļ������ʼ��
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
  Description     : ɾ��USB���̡��������ļ�����
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
  Description     : ��ȡDisk����ָ��
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
  Description     : ��ȡPart����ָ��
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
  Description     : ��ȡFile����ָ��
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
  Description     : ͨ�������DiskIndex��DiskList�����л�ȡ��Ӧ���̵���Ϣ
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
  Description     : ͨ�������DiskDevNo��DiskList�����л�ȡ��Ӧ���̵���Ϣ
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          : DiskDevNo��Ӧ�ײ����devinfo�����±�
*****************************************************************************/
DiskInfo_t *GetDiskInfoByDiskDevNo(int DiskDevNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPartInfoByPartIndex
  Description     : ͨ�������PartIndex��PartList�����л�ȡ��Ӧ��������Ϣ
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
  Description     : ͨ�������PartNo��PartList�����л�ȡ��Ӧ��������Ϣ
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
  Description     : ��ȡ������Ӧ�̷�
  Input           : DiskNo:��Ӧ�ײ���������±�;PartNo:��Ӧ�ײ���������±�
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
  Description     : У���̷���Ч��
  Input           : Symbol:�����̷�
  Outut           : TRUE:��Ч;FALSE:��Ч
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
  Description     : �Ա��ļ��������Ƿ��������������һ���Ľڵ�
  Input           : �ļ�����ָ��,����ָ��
  Outut           : TRUEΪ�ļ����������и����ֵ��ļ�,FALSEΪû�и����ֵ��ļ�
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
  Description     : ͨ�������Name��FileList�����л�ȡ��Ӧ�ļ���Ϣ
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
  Description     : ͨ���ļ����ֻ�ȡ�ļ�����
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
  Description     : ��ʼ��Usb�����豸����
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
  Description     : ��ʼ��Usb�����·�������
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
  Description     : ��ʼ��Ŀ¼���ļ�����
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
  Description     : �ж��Ƿ�Ϊ��Ŀ¼
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
  Description     : ��·������Ϊ��һ��Ŀ¼·��
  Input           : ���õ�·��
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
  Description     : ��·������Ϊ��Ŀ¼·��
  Input           : ���õ�·��
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
  Description     : ��ȡ·�����
  Input           : �ļ�·��
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
  Description     : �ж�Usb������������״̬
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
  Description     : ���PVR���õ�Usb�������Ƿ���Ч
  Input           :
  Outut           : 0Ϊ��Ч,1Ϊ�޹���Usb,2ΪPvr�趨������Ч,3ΪPvr�趨������Ч
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
  Description     : ����ʱ��״̬��־
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :TRUE:ʱ�� FALSE:¼��
*****************************************************************************/
void SetTimeShiftStatus(BOOL Status);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetUsbPlayMode
  Description     : ��ȡUSB�Ĳ���ģʽ
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
  Description     : ����ѡʱ����ʱ��
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
  Description     : ��ȡUSB�Ĳ���״̬������Ϣ
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
  Description     : ����PVR¼�ƽṹ
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
  Description     : ����PVR¼����Ϣ�ṹ
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
  Description     : ����¼�Ʋ���
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
  Description     : ��ȡPVR��Ŀ��Ϣ�ļ�
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
  Description     : ��Ƶ��Pvr¼��Ƶ���ͻDlg��ʼ��
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
  Description     : ������Ϣ��Usb��������
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
  Description     : ����ֹͣ¼����Ϣ��Usb��������
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
  Description     : ��StreamTypeת����Pvr������
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
  Description     : ������Ƶ���Žṹ
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
  Description     : Ϊ��ǰ���Ų������ûص�����
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
  Description     : ������Ƶ���Žṹ
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
  Description     : ǿ��������Ŀ¼��
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
  Description     : ǿ��ֹͣ��Ŀ¼��
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
  Description     : ǿ����������Ƶ����
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
  Description     : ǿ��ֹͣ��Ŀ����
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
  Description     : ǿ����ͣ��Ŀ����
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
  Description     : ǿ�ƻָ���Ŀ����
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
  Description     : ǿ�ƿ������
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
  Description     : ǿ�ƿ��˲���
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
  Description     : ǿ�ƴӿ�������лָ�����
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
  Description     : ǿ��ѡʱ����
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
  Description     : USB���������ʼ��
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
  Description     : ����USB��������
  Input           :
  Outut           :
  Calls           :
  Called by       :
  Global Variables:
  Others          :
*****************************************************************************/
void UsbManagerTaskTerm(void);

#endif
