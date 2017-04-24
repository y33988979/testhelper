/* Default value for partitions */
/* ---------------------------- */
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include <sttbx.h>
#include "drv_memory.h"
#include "drv_sys.h"
#include "drv_basic.h"
/* Libcurl definitions */
/* ------------------- */
#if defined(ST_OSLINUX)
typedef off_t curl_off_t;
typedef struct
{
 char       location[4096]; /* File URL                   */
 curl_off_t offset;         /* Current offset in the file */
 curl_off_t fileSize;       /* File Size                  */
 void *     hidden;         /* Private Data               */
} HTTPCURLContext;
#endif


/* Local definition for file system */
/* -------------------------------- */
typedef enum SYS_FileSystem_s
{
 SYS_FS_NONE,
 SYS_FS_VFS,
 SYS_FS_LOCAL,
 SYS_FS_HTTP
} SYS_FileSystem_t;

/* Local definition for file handle */
/* -------------------------------- */
typedef struct SYS_FileHandle_s
{
 SYS_FileSystem_t  FileSystem;
 void             *FileHandle;
} SYS_FileHandle_t;


/* ========================================================================
   Name:        SYS_FOpen
   Description: Open a file
   ======================================================================== */

void *SYS_FOpen(const char *filename,const char *mode)
{
 SYS_FileHandle_t *File;

 /* Allocate a file handle descriptor */
 /* ================================= */
 File=(SYS_FileHandle_t *)DRV_OSMalloc(SystemPartition,sizeof(SYS_FileHandle_t));
 if (File==NULL)
  {
   return(NULL);
  }
memset((U8*)File,0,sizeof(SYS_FileHandle_t));
 File->FileHandle=NULL;

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
  {
   U32 i,Flags      = O_LARGEFILE;
   File->FileSystem = SYS_FS_LOCAL;
   for (i=0;i<strlen(mode);i++)
    {
     if (mode[i]=='r') { Flags=Flags | O_RDONLY;                        }
     if (mode[i]=='w') { Flags=Flags | O_CREAT | O_WRONLY;              }
     if (mode[i]=='a') { Flags=Flags | O_CREAT | O_APPEND | O_WRONLY;   }
     if (mode[i]=='+') { Flags=(Flags&(~(O_RDONLY|O_WRONLY))) | O_RDWR; }
     if (mode[i]=='t') { Flags=Flags | O_TRUNC;                         }
     if (mode[i]=='d') { Flags=Flags/* FIX ME : | O_DIRECT*/;           }
    }
   File->FileHandle = (void *)open(filename,Flags);
   if ((U32)File->FileHandle==0xFFFFFFFF)
    {
     File->FileHandle=NULL;
    }
   else
    {
     if ((Flags&O_CREAT)==O_CREAT)
      {
       chmod(filename,S_IRWXU|S_IRGRP|S_IROTH);
      }
    }
  }
#endif

 /* Return file system */
 /* ================== */
 if (File->FileHandle==NULL)
  {
   DRV_OSFree(SystemPartition,(U32)File);
   File=NULL;
  }

 /* Return file handle */
 /* ================== */
 return(File);
}

/* ========================================================================
   Name:        SYS_FClose
   Description: Close a file
   ======================================================================== */

U32 SYS_FClose(void *FileContext)
{
 U32 ReturnValue        = 0xFFFFFFFF;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check if pointer is NULL */
 /* ======================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }


 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=close((int)File->FileHandle);
  }
#endif

 /* Free the handle */
 /* =============== */
 DRV_OSFree(SystemPartition,(U32)File);
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FRead
   Description: Read datas from file
   ======================================================================== */

U32 SYS_FRead(void *ptr,U32 size,U32 nmemb,void *FileContext)
{
 U32 ReturnValue        = 0xFFFFFFFF;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check file parameter */
 /* ==================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }

 /* Check ptr */
 /* ========= */
 if (ptr==NULL)
  {
   printf("SYS_FRead(0x%08x):**ERROR** !!! ptr to read the datas is a NULL pointer !!!\n",FileContext);
   return(ReturnValue);
  }

 /* Check size to read */
 /* ================== */
#if 0
 if ((size>=(5*1024*1024))||(nmemb>=(5*1024*1024)))
  {
   printf("SYS_FRead(0x%08x):**ERROR** !!! Hey, crazy man, you try to read too much datas, is there a bug ? -> Discard read !!!\n",FileContext);
   return(ReturnValue);
  }
#endif

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=read((int)File->FileHandle,ptr,size*nmemb);
   ReturnValue=ReturnValue/size;
  }
#endif

 /* Return nb bytes read */
 /* ==================== */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FWrite
   Description: Write datas to file
   ======================================================================== */

U32 SYS_FWrite(void *ptr,U32 size,U32 nmemb,void *FileContext)
{
 U32 ReturnValue        = 0xFFFFFFFF;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check file parameter */
 /* ==================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }

 /* Check ptr */
 /* ========= */
 if (ptr==NULL)
  {
   printf("SYS_FWrite(0x%08x):**ERROR** !!! ptr to write the datas is a NULL pointer !!!\n",FileContext);
   return(ReturnValue);
  }

 /* Check size to write */
 /* =================== */
#if 0
 if ((size>=(5*1024*1024))||(nmemb>=(5*1024*1024)))
  {
   printf("SYS_FWrite(0x%08x):**ERROR** !!! Hey, crazy man, you try to write too much datas, is there a bug ? -> Discard write !!!\n",FileContext);
   return(ReturnValue);
  }
#endif

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=write((int)File->FileHandle,ptr,size*nmemb);
   ReturnValue=ReturnValue/size;
  }
#endif

 /* Return nb bytes read */
 /* ==================== */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FSeek
   Description: Move file position
   ======================================================================== */

U32 SYS_FSeek(void *FileContext,S64 offset,U32 whence)
{
 U32 ReturnValue        = 0xFFFFFFFF;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check file parameter */
 /* ==================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   if (lseek64((int)File->FileHandle,offset,whence)<0)
    {
     ReturnValue=0xFFFFFFFF;
    }
   else
    {
     ReturnValue=0x00000000;
    }
  }
#endif

 /* Return nb bytes read */
 /* ==================== */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FTell
   Description: Get file position
   ======================================================================== */

S64 SYS_FTell(void *FileContext)
{
 S64 ReturnValue        = 0xFFFFFFFFFFFFFFFFLL;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check file parameter */
 /* ==================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=(S64)lseek64((int)File->FileHandle,0,SEEK_CUR);
  }
#endif

 /* Return nb bytes read */
 /* ==================== */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FRemove
   Description: Remove a file
   ======================================================================== */

U32 SYS_FRemove(const char *filename)
{
 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 return(unlink(filename));
#endif
}

/* ========================================================================
   Name:        SYS_FStat
   Description: Get the information about a file
   ======================================================================== */

U32 SYS_FStat(const char *filename,SYS_Stat_t *stats)
{
 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 return(stat64(filename,stats));
#endif
}

/* ========================================================================
   Name:        SYS_FStatfs
   Description: Get the information about a file
   ======================================================================== */

U32 SYS_FStatfs(const char *filename,SYS_Statfs_t *statsfs)
{
 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 return(statfs(filename,statsfs));
#endif
}

/* ========================================================================
   Name:        SYS_FTruncate
   Description: Truncate a file
   ======================================================================== */

U32 SYS_FTruncate(void *FileContext,S64 Size)
{
 U32 ReturnValue        = 0xFFFFFFFF;
 SYS_FileHandle_t *File = (SYS_FileHandle_t *)FileContext;

 /* Check file parameter */
 /* ==================== */
 if (File==NULL)
  {
   return(ReturnValue);
  }

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (File->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=ftruncate64(fileno(File->FileHandle),Size);
  }
#endif

 /* Return error code */
 /* ================= */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FOpenDir
   Description: Open a directory
   ======================================================================== */

void *SYS_FOpenDir(const char *filename)
{
 SYS_FileHandle_t *Dir;

 /* Allocate a directory handle descriptor */
 /* ====================================== */
 Dir=(SYS_FileHandle_t *)DRV_OSMalloc(SystemPartition,sizeof(SYS_FileHandle_t));
 if (Dir==NULL)
  {
   return(NULL);
}
memset((U8*)Dir,0,sizeof(SYS_FileHandle_t));
 Dir->FileHandle=NULL;

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 {
  Dir->FileSystem = SYS_FS_LOCAL;
  Dir->FileHandle = (void *)opendir(filename);
 }
#endif

 /* Return file system */
 /* ================== */
 if (Dir->FileHandle==NULL)
  {
   DRV_OSFree(SystemPartition,(U32)Dir);
   Dir=NULL;
  }

 /* Return file handle */
 /* ================== */
 return(Dir);
}

/* ========================================================================
   Name:        SYS_FReadDir
   Description: Read the next directory entry
   ======================================================================== */

U32 SYS_FReadDir(void *DirContext,SYS_Dirent_t *Dirent)
{
 U32 ReturnValue       = 0xFFFFFFFF;
 SYS_FileHandle_t *Dir = (SYS_FileHandle_t *)DirContext;

 /* Check if pointer is NULL */
 /* ======================== */
 if (Dir==NULL)
  {
   return(ReturnValue);
  }

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (Dir->FileSystem==SYS_FS_LOCAL)
  {
   SYS_Dirent_t *d;
   if ((d=readdir(Dir->FileHandle))!=NULL)
    {
     memcpy(Dirent,d,sizeof(SYS_Dirent_t));
     ReturnValue=0;
    }
  }
#endif

 /* Return error code */
 /* ================= */
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FCloseDir
   Description: Close a directory
   ======================================================================== */

U32 SYS_FCloseDir(void *DirContext)
{
 U32 ReturnValue       = 0xFFFFFFFF;
 SYS_FileHandle_t *Dir = (SYS_FileHandle_t *)DirContext;

 /* Check if pointer is NULL */
 /* ======================== */
 if (Dir==NULL)
  {
   return(ReturnValue);
  }

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 if (Dir->FileSystem==SYS_FS_LOCAL)
  {
   ReturnValue=closedir(Dir->FileHandle);
  }
#endif

 /* Free the handle */
 /* =============== */
 DRV_OSFree(SystemPartition,(U32)Dir);
 return(ReturnValue);
}

/* ========================================================================
   Name:        SYS_FMkDir
   Description: Make a directory
   ======================================================================== */

U32 SYS_FMkDir(const char *filename)
{
 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 return(mkdir(filename,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
#endif
}

/* ========================================================================
   Name:        SYS_FRmDir
   Description: Make a directory
   ======================================================================== */

U32 SYS_FRmDir(const char *filename)
{
 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 return(rmdir(filename));
#endif
}

/* ========================================================================
   Name:        SYS_GetString
   Description: Get a string from stdin
   ======================================================================== */

U32 SYS_GetString(U8 *Buffer,U32 Size)
{

 /* For LINUX */
 /* ========= */
#if defined(ST_OSLINUX)
 {
  fgets((char *)Buffer,Size,stdin);
  return(strlen((char *)Buffer)+1);
 }
#endif
}

/* ========================================================================
   Name:        SYS_GetKey
   Description: Get a key pressed
   ======================================================================== */

U32 SYS_GetKey(U32 *Key)
{
	/* Getting the char */
	*Key=(U32)(getchar());
	return(1);
 /* For LINUX */
 /* ========= */
#if 0//defined(ST_OSLINUX)
 {
  struct termio term;
  /* Unblocking the teminal */
  ioctl(0,TCGETA,(void *)&term);
  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  /* Getting the char */
  *Key=(U32)(getchar());
  /* Blocking the teminal */
  ioctl(0,TCGETA,(void *)&term);
  term.c_lflag |= ICANON;
  term.c_lflag |= ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  return(1);
 }
#endif
}

/* ========================================================================
   Name:        SYS_PollKey
   Description: Check if there is a key pressed
   ======================================================================== */

U32 SYS_PollKey(U32 *Key)
{
	U32    ReadChar=0;
	/* Get event we want to know */
	/* If we receive one thing, get the byte now */
	ReadChar=read(0,Key,1);
	/* Purge the byte */
	/* tcflush(0,TCIOFLUSH); */
	return(ReadChar);
 /* For LINUX */
 /* ========= */
#if 0//defined(ST_OSLINUX)
 {
  U32    ReadChar=0;
  struct pollfd p;
  struct termio term,term_old;
  /* Get control of the terminal */
  ioctl(0,TCGETA,(void *)&term);
  term_old = term;
  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  ioctl(0,TCSETAW,(void *)&term);
  /* Get event we want to know */
  p.fd     = 0;
  p.events = POLLIN;
  /* If we receive one thing, get the byte now */
  if (poll(&p,1,-1)>0)
   {
    if (p.revents==POLLIN)
     {
      ReadChar=read(0,Key,1);
     }
   }
  /* Purge the byte */
  /* tcflush(0,TCIOFLUSH); */
  /* Leave control */
  ioctl(0,TCSETAW,(void *)&term_old);
  return(ReadChar);
 }
#endif
}

/* ========================================================================
   Name:        SYS_AVMEM_AddBlock

/* ========================================================================
   Name:        SYS_ReadRegDev8
   Description: Generic function to read 8 bits register
   ======================================================================== */

U8 SYS_ReadRegDev8(U32 PhysicalAddress)
{
	printf("Un support\n");
	return 0;
}

/* ========================================================================
   Name:        SYS_ReadRegDev16LE
   Description: Generic function to read 16 bits register
   ======================================================================== */

U16 SYS_ReadRegDev16LE(U32 PhysicalAddress)
{
	printf("Un support\n");
	return 0;
}

/* ========================================================================
   Name:        SYS_ReadRegDev32LE
   Description: Generic function to read 32 bits register
   ======================================================================== */

U32 SYS_ReadRegDev32LE(U32 PhysicalAddress)
{
	U32 Register_Value;

	HI_SYS_ReadRegister(PhysicalAddress,&Register_Value);

	/* Return value */
	/* ------------ */
	return(Register_Value);
}

/* ========================================================================
   Name:        SYS_WriteRegDev8
   Description: Generic function to write 8 bits register
   ======================================================================== */

void SYS_WriteRegDev8(U32 PhysicalAddress,U8 Value)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_WriteRegDev16LE
   Description: Generic function to write 16 bits register
   ======================================================================== */

void SYS_WriteRegDev16LE(U32 PhysicalAddress,U16 Value)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_WriteRegDev32LE
   Description: Generic function to write 32 bits register
   ======================================================================== */

void SYS_WriteRegDev32LE(U32 PhysicalAddress,U32 Value)
{

	HI_SYS_WriteRegister(PhysicalAddress,Value);
}

/* ========================================================================
   Name:        SYS_SetBitsDev8
   Description: Generic function to set bits in a 8 bits register
   ======================================================================== */

void SYS_SetBitsDev8(U32 PhysicalAddress,U8 Mask)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_ClearBitsDev8
   Description: Generic function to clear bits in a 8 bits register
   ======================================================================== */

void SYS_ClearBitsDev8(U32 PhysicalAddress,U8 Mask)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_SetBitsDev16LE
   Description: Generic function to set bits in a 16 bits register
   ======================================================================== */

void SYS_SetBitsDev16LE(U32 PhysicalAddress,U16 Mask)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_ClearBitsDev16LE
   Description: Generic function to clear bits in a 16 bits register
   ======================================================================== */

void SYS_ClearBitsDev16LE(U32 PhysicalAddress,U16 Mask)
{
	printf("Un support\n");
}

/* ========================================================================
   Name:        SYS_SetBitsDev32LE
   Description: Generic function to set bits in a 32 bits register
   ======================================================================== */

void SYS_SetBitsDev32LE(U32 PhysicalAddress,U32 Mask)
{
	U32 Register_Value;

	/* Read the register */
	/* ----------------- */
	HI_SYS_ReadRegister(PhysicalAddress,&Register_Value);
	Register_Value=Register_Value|Mask;
	HI_SYS_WriteRegister(PhysicalAddress,Register_Value);
}

/* ========================================================================
   Name:        SYS_ClearBitsDev32LE
   Description: Generic function to clear bits in a 32 bits register
   ======================================================================== */

void SYS_ClearBitsDev32LE(U32 PhysicalAddress,U32 Mask)
{
	U32 Register_Value;

	/* Read the register */
	/* ----------------- */
	Register_Value=HI_SYS_ReadRegister(PhysicalAddress,&Register_Value);
	Register_Value=Register_Value&(~Mask);
	HI_SYS_WriteRegister(PhysicalAddress,Register_Value);
}
/* ========================================================================
   Name:        SYS_MapRegisters
   Description: Map register region and return a virtual address
   ======================================================================== */

U32 SYS_MapRegisters(U32 PhysicalAddress,U32 Size,char *Name)
{
	printf("Un support\n");
	return 0;
}

/* ========================================================================
   Name:        SYS_UnmapRegisters
   Description: Unmap register region
   ======================================================================== */

void SYS_UnmapRegisters(U32 VirtualAddress,U32 Size)
{
	printf("Un support\n");
	return;
}



