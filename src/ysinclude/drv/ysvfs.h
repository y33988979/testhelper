#ifndef _YSVFS_H_
#define _YSVFS_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <dirent.h>
#include <sys/statfs.h>
#include <sys/stat.h>
#include <drv_typedef.h>
#define MAX_PATH_LENGTH  512
#define VFS_DIRENT_NAME_LEN_MAX 255
#define MAX_DEVICE 16       //����ܹ��ص��豸��Ŀ
#define MAX_VOLUME 16       //ÿ���豸������ܹ��صķ�����Ŀ
#define VOLUME_INDEX 16     //usbdevice.devinfo.logicname �� ��16λ ���������ϲ㶨����̷��±� (c  or  d  or  e ...)
char CurrentPath[MAX_PATH_LENGTH];

typedef enum DRV_VfsUnitMode_e 
{
	DRV_VFS_UNIT_EMPTY = 0,     /* ��Ŀ¼ */
	DRV_VFS_UNIT_DIR = 1,     /* Ŀ¼ */
	DRV_VFS_UNIT_FILE_R,     /* �ɶ��ļ� */
	DRV_VFS_UNIT_FILE_W,     /* ��д�ļ� */
	DRV_VFS_UNIT_FILE_RW     /* �ɶ�д�ļ� */
} DRV_VfsUnitMode_t;

enum
{
	DRV_VFS_CURRENT_DIR_COMPLETE = 1,
	DRV_VFS_ROOT_DIR_EMPTY,			
	DRV_VFS_ROOT_DIR_INVALID,
	DRV_VFS_CURRENT_DIR_INVALID,
	DRV_VFS_FILE_PATH_INVALID,
	DRV_VFS_DIR_PATH_INVALID,
	DRV_VFS_DIR_HANDLE_INVALID,
	DRV_NO_MOUNT_POINT,
    DRV_GET_FS_INFO_FAULT,
};

typedef struct DRV_VfsStat_s {
  U32_YS  Vfs_mode;     /* mode */
  U32_YS        Vfs_size;     /* total size, in bytes */
  U32_YS        Vfs_blksize;  /* blocksize for filesystem I/O */
  U32_YS        Vfs_atime;    /* time of last access */
  U32_YS        Vfs_mtime;    /* time of last modification */
  U32_YS        Vfs_ctime;    /* time of last status change */
}DRV_VfsStat;


typedef struct DRV_VfsUnitInfo_s 
{
    U32_YS Flag; 			//0 ��ʾ�Ǹ��ļ��ڵ㣬1��ʾ��Ŀ¼�ڵ�/
    U8_YS UnitName[VFS_DIRENT_NAME_LEN_MAX + 1]; 		//�ڵ����ַ������������ϼ�·��/
    U8_YS LongName[VFS_DIRENT_NAME_LEN_MAX + 1];   //����OSD��ʾ
    void 	*Handle; 			//�������ײ�ʵ���߹���������Ϣʹ��/
    DRV_VfsUnitMode_t  UnitMode;     /* mode */
	/*����Ŀ¼�����漸����������Ч��*/
    U32_YS  UnitSize;     /* total size, in bytes */
    U32_YS  UnitBlkSize;  /* blocksize for filesystem I/O */
    U32_YS  UnitAtime;    /* time of last access */
    U32_YS  UnitMtime;    /* time of last modification */
    U32_YS  UnitCtime;    /* time of last status change */
}DRV_VfsUnitInfo_t;

typedef enum DRV_VfsSeekWhence_e
{
    DRV_VFS_SEEK_SET=0,
    DRV_VFS_SEEK_CUR,
    DRV_VFS_SEEK_END,    
}DRV_VfsSeekWhence;

enum
{	
	DRV_VFS_VOLUME_FULL=1,     /*�ռ�����*/
        DRV_VFS_IS_DIR,
        DRV_VFS_IS_READONLY,
        DRV_VFS_FILE_IS_BUSY,
        DRV_VFS_IS_NOT_DIRNAME,
        DRV_VFS_IS_NOT_EMPTY,
        DRV_VFS_IN_DIFFERENT_VOLUME,
        DRV_VFS_GET_INFO_FAULT,
	
};
/*
 * vfs_info struct
 */
typedef enum DRV_VfsFsType_e {
  DRV_VFS_FS_TYPE_NONE    = 0,
  DRV_VFS_FS_TYPE_UNKNOWN = 2001,
  DRV_VFS_FS_TYPE_FAT12   = 12,
  DRV_VFS_FS_TYPE_FAT16   = 16,
  DRV_VFS_FS_TYPE_FAT32   = 32,
  DRV_VFS_FS_TYPE_E2FS    = 226,
  DRV_VFS_FS_TYPE_E3FS    = 227
} DRV_VfsFsType_t;

typedef struct DRV_VfsInfo_s {
  DRV_VfsFsType_t fs_type;         /* Type of underlying file system */
  U32_YS free_blocks;     /* Number of remaing (512 byte) blocks */
  U32_YS allocation_unit; /* Number of (512 byte) blocks in an allocation unit */
  U32_YS total_size;      /* Total size of volume in (512 byte) blocks */
  char panfu;
} DRV_VfsInfo_t;

//for test disk  partition
#define DRV_MAX_VOL_NAME_LEN 16

typedef void (*callback)();


typedef struct DRV_Volume_info{

DRV_VfsInfo_t  vfs_info[16];
U32_YS monutnumber;/*���صķ�����*/
}DRV_Volume_info_t;

extern void *usbhotplug(void);
DRV_Handle  DRV_VfsPathUserToDrv(S8_YS *userpath,S8_YS *drvpath);

/*
*DRV_VfsOpenFile()
* �ɹ�����handle(ֵ��ȷ��,��0)
* ���󷵻�:-1,DRV_VFS_VOLUME_FULL,DRV_VFS_IS_DIR,DRV_VFS_IS_READONLY.
*                    ������      ,�򿪵���Ŀ¼��,��������ֻ����ʽ����
*/
DRV_Handle DRV_VfsOpenFile(S8_YS* Path,S8_YS *Mode);

/*
*DRV_VfsCloseFile
*�ɹ�����0
* ���󷵻�:-1
*/
DRV_ErrCode DRV_VfsCloseFile(DRV_Handle FileHandle);

/*
*DRV_VfsDeleteFile
*�ɹ�����0
*���󷵻�:-1,DRV_VFS_FILE_IS_BUSY
*             �ļ���ʹ��
*/
DRV_ErrCode DRV_VfsDeleteFile(S8_YS* Path);

/*
*DRV_VfsCreateDir
*�ɹ�����0
*���󷵻�:-1,DRV_VFS_VOLUME_FULL
*             ������
*/
DRV_ErrCode DRV_VfsCreateDir(S8_YS *Path);
/*
*DRV_VfsRename�޸��ļ���Ŀ¼������,�����ƶ��ļ�
*�ɹ�����0
*���󷵻�:-1,DRV_VFS_IN_DIFFERENT_VOLUME
*             Դ�ļ���Ŀ���ļ�����һ������
*/
DRV_ErrCode DRV_VfsRename(S8_YS* Dst,S8_YS* Src);

/*
*DRV_VfsReadFile
*�ɹ����ض�ȡ���ֽ���
*���󷵻�:-1
*/
S32_YS DRV_VfsReadFile(DRV_Handle fd,void* buffer,U32_YS Length);

/*
*DRV_VfsWriteFile
*�ɹ�����д����ֽ���
*���󷵻�:-1
*/
S32_YS DRV_VfsWriteFile(DRV_Handle fd,void* buffer,U32_YS Length);

/*
*DRV_VfsSeekFile
*�ɹ�����0
*���󷵻�:-1
*/

DRV_ErrCode DRV_VfsSeekFile(DRV_Handle fd,S32_YS offset,DRV_VfsSeekWhence  Reference);

S32_YS DRV_VfsLseekFile(DRV_Handle fd,S32_YS offset,S32_YS  Reference);
S32_YS DRV_VfsFstatFile(DRV_Handle fd,DRV_VfsStat  *buf);
DRV_ErrCode DRV_VfsFflush(DRV_Handle FileHandle);
/*
*DRV_VfsTellFile
*�ɹ�����0
*���󷵻�:-1
*/
U32_YS DRV_VfsTellFile(DRV_Handle fd);

/*
*DRV_VfsOpenDir
*�ɹ�����handle
*���󷵻�:-1,DRV_VFS_IS_NOT_DIRNAME
*               ����Ŀ¼��
*/
DRV_Handle DRV_VfsOpenDir(S8_YS *Path);


/*
*DRV_VfsCloseDir
*�ɹ�����0
*���󷵻�:-1
*/
DRV_ErrCode DRV_VfsCloseDir(DRV_Handle DirHandle);
/*
*˵��:ɾ����Ŀ¼ǰ,Ҫȷ����Ŀ¼�ڵ��ļ���Ŀ¼û�б��򿪻�ʹ��.
*����:ɾ������Ŀ¼
*/
DRV_ErrCode DRV_VfsDelete_WholeDir(S8_YS *Path);
/*
*DRV_VfsCloseDir��λĿ¼��ָ�룬ʹ��ָ��Ŀ¼���һ����¼
*�ɹ�����0
*���󷵻�:-1
*/
DRV_ErrCode DRV_VfsRewindDir(DRV_Handle DirHandle);

/*
*DRV_VfsReadDir  ��ȡĿ¼����һ���ڵ������
*�ɹ�����0
*���󷵻�:-1
*/
DRV_ErrCode DRV_VfsReadDir(DRV_Handle DirHandle,DRV_VfsUnitInfo_t* Dirent_p);

/*
*˵��:ɾ����Ŀ¼ǰ,Ҫȷ����Ŀ¼�ڵ��ļ���Ŀ¼û�б��򿪻�ʹ��.
*����:ɾ������Ŀ¼
*/
DRV_ErrCode DRV_VfsDelete_WholeDir(S8_YS *Path);
/*
*DRV_VfsChDir
*�ɹ�����0
*���󷵻�:-1
*/
DRV_ErrCode DRV_VfsChDir(S8_YS* Path);

/*
*DRV_VfsGetCwd
*�ɹ�����0
*���󷵻�:·���ĳ���
*/

U32_YS DRV_VfsGetCwd(S8_YS* Buf, U32_YS Length);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsCheckDevExist
  Description     : �жϴ����豸�Ƿ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_VfsCheckDevExist(U32_YS DeviceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsCheckDevMount
  Description     : �жϴ����豸�Ƿ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_VfsCheckDevMount(U32_YS DeviceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_GetVfsLogicDeviceNum
  Description     : ��ȡ��Ч�����豸����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U32_YS DRV_GetVfsLogicDeviceNum(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_GetVfsMountDeviceNum
  Description     : ��ȡ���ش����豸����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U32_YS DRV_GetVfsMountDeviceNum(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsCheckPartExist
  Description     : �жϷ����Ƿ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_VfsCheckPartExist(U32_YS DeviceNo,U32_YS PartNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsCheckPartMount
  Description     : �жϷ����Ƿ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_VfsCheckPartMount(U32_YS DeviceNo,U32_YS PartNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_GetVfsPartSymbol
  Description     : ��ȡ������Ӧ�̷�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
char DRV_GetVfsPartSymbol(U32_YS DeviceNo,U32_YS PartNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsGetInfo
  Description     : ��ȡ���������з����ļ�ϵͳ��Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode DRV_VfsGetInfo(DRV_Volume_info_t *Volume_info,U32_YS DeviceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsFormat
  Description     : ��ʽ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode DRV_VfsFormat(U32_YS DeviceNo,U32_YS PartNo,DRV_VfsFsType_t FormatTpye);


/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsUnMountPart
  Description     : ж��Usbָ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode DRV_VfsUnMountPart(U32_YS DeviceNo,U32_YS PartNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsUnMountDev
  Description     : ж��Usbָ���豸
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode DRV_VfsUnMountDev(U32_YS DeviceNo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   gwei
  Function Name   : DRV_VfsUnMount
  Description     : ж��Usb�����豸
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode DRV_VfsUnMount(void);


void unregister_app_plugout_callback();
void register_app_plugout_callback(callback cb); 

#ifdef __cplusplus
}
#endif
#endif

