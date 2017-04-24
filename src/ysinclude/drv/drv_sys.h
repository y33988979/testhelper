/*****************************************************************************

File Name: drv_sys.h

Description: 

*****************************************************************************/

/* Define to prevent recursive inclusion */

#ifndef _DRV_SYS_H_
#define _DRV_SYS_H_
/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

/* Includes --------------------------------------------------------------- */

#include <stddefs.h>
#if defined(ST_OSLINUX)
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <fcntl.h>
#include <unistd.h>
#endif
/* Exported Types ------------------------------------------------------ */
#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

/* Local definition for file system */
/* -------------------------------- */
#if defined(ST_OSLINUX)
typedef struct stat64         SYS_Stat_t;
typedef struct statfs         SYS_Statfs_t;
typedef struct dirent         SYS_Dirent_t;
#define SYS_IS_DIR(mode)      ((mode) & 0x00000004) /* To be used with the mode of a dirent entry         */
#define SYS_IS_DIR_STAT(mode) S_ISDIR(mode)         /* To be used with the mode of on a SYS_Stat_t object */
#define SYS_IS_W(mode)        (((mode) & S_IWUSR) || ((mode) & S_IWGRP) || ((mode) & S_IWOTH))
#define SYS_IS_R(mode)        (((mode) & S_IRUSR) || ((mode) & S_IRGRP) || ((mode) & S_IROTH))
#define SYS_IS_RW(mode)       (SYS_IS_W(mode) && SYS_IS_R(mode)
#endif

/* Exported Constants -------------------------------------------------- */

/* Exported Variables -------------------------------------------------- */

void *SYS_FOpen(const char *filename,const char *mode);
U32 SYS_FClose(void *FileContext);
U32 SYS_FRead(void *ptr,U32 size,U32 nmemb,void *FileContext);
U32 SYS_FWrite(void *ptr,U32 size,U32 nmemb,void *FileContext);
U32 SYS_FSeek(void *FileContext,S64 offset,U32 whence);
S64 SYS_FTell(void *FileContext);
U32 SYS_GetString(U8 *Buffer,U32 Size);
U32 SYS_GetKey(U32 *Key);
U32 SYS_PollKey(U32 *Key);
U32            SYS_FRemove(const char *filename);
U32            SYS_FStat(const char *filename,SYS_Stat_t *stats);
U32            SYS_FStatfs(const char *filename,SYS_Statfs_t *statsfs);
void          *SYS_FOpenDir(const char *filename);
U32            SYS_FReadDir(void *directory,SYS_Dirent_t *dirent);
U32            SYS_FCloseDir(void *directory);
U32            SYS_FMkDir(const char *filename);
U32            SYS_FRmDir(const char *filename);
U8             SYS_ReadRegDev8         (U32 PhysicalAddress);
U16            SYS_ReadRegDev16LE      (U32 PhysicalAddress);
U32            SYS_ReadRegDev32LE      (U32 PhysicalAddress);
void           SYS_WriteRegDev8        (U32 PhysicalAddress,U8  Value);
void           SYS_WriteRegDev16LE     (U32 PhysicalAddress,U16 Value);
void           SYS_WriteRegDev32LE     (U32 PhysicalAddress,U32 Value);
void           SYS_SetBitsDev16LE      (U32 PhysicalAddress,U16 Mask);
void           SYS_ClearBitsDev16LE    (U32 PhysicalAddress,U16 Mask);
void           SYS_SetBitsDev32LE      (U32 PhysicalAddress,U32 Mask);
void           SYS_ClearBitsDev32LE    (U32 PhysicalAddress,U32 Mask);
U32            SYS_MapRegisters        (U32 PhysicalAddress,U32 Size,char *Name);
void           SYS_UnmapRegisters      (U32 VirtualAddress ,U32 Size);
/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif /* _DRV_SYS_H_ */
