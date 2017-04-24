#include <stddefs.h>
#include <hi_struct.h>
#include "drv_os.h"
#include "drv_debug.h"
//#define DEBUG_AVMEM

typedef struct SYS_MemHandle_s
{
	HI_MMZ_BUF_S MmzBuf;
 	struct SYS_MemHandle_s *MemoryNext;           /* Next allocation block                                  */
} SYS_MemHandle_t;

static SYS_MemHandle_t *SYS_MemList=NULL;


static DRV_MutexHandle m_AvmemMutex= NULL;
/*返回的是用户态地址*/
void* drv_AvmemAllocateUncached(U32 Avmem_Number, U32 Size,U32 Alignment)
{
    ST_ErrorCode_t errCode;
    SYS_MemHandle_t * Mem;

    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);

    Mem=(SYS_MemHandle_t *)malloc(sizeof(SYS_MemHandle_t));
    if (Mem==NULL)
    {
        return(0);
    }
    
    Mem->MmzBuf.bufsize      = Size;
    //Mem->MmzBuf.MemoryAlignment = Alignment;
    Mem->MemoryNext      = NULL;
   errCode = HI_MMZ_Malloc(&(Mem->MmzBuf));  
    if(errCode != ST_NO_ERROR)
    {
        STAPI_ERROR(errCode);
        return NULL;
    }

    /* Lock the access of the list */
    DRV_OS_MutexLock(m_AvmemMutex);
    if (SYS_MemList==NULL)
    {
        /* The list is now started with this element */
        SYS_MemList=Mem;
    }
    else
    {
        /* Add the new element on the top of the list */
        Mem->MemoryNext=SYS_MemList;
        SYS_MemList=Mem;
    }
    /* Unlock the access of the list */
    DRV_OS_MutexRelease(m_AvmemMutex);
#ifdef DEBUG_AVMEM    
	printf("------allocate 0x%08x,MemoryPrivate 0x%08x\n",Mem->MmzBuf.user_viraddr,Mem->MmzBuf.kernel_viraddr);
#endif	
    return((U32)Mem->MmzBuf.user_viraddr);
}
/*返回的是用户态地址*/
void* drv_AvmemAllocateCached(U32 Avmem_Number,U32 Size,U32 Alignment)
{
    ST_ErrorCode_t errCode;
    SYS_MemHandle_t * Mem;

    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);

    Mem=(SYS_MemHandle_t *)malloc(sizeof(SYS_MemHandle_t));
    if (Mem==NULL)
    {
        return(0);
    }
    
    Mem->MmzBuf.bufsize      = Size;
    //Mem->MmzBuf.MemoryAlignment = Alignment;
    Mem->MemoryNext      = NULL;
   errCode = HI_MMZ_Malloc(&(Mem->MmzBuf));  
    if(errCode != ST_NO_ERROR)
    {
        STAPI_ERROR(errCode);
        return NULL;
    }

    /* Lock the access of the list */
    DRV_OS_MutexLock(m_AvmemMutex);
    if (SYS_MemList==NULL)
    {
        /* The list is now started with this element */
        SYS_MemList=Mem;
    }
    else
    {
        /* Add the new element on the top of the list */
        Mem->MemoryNext=SYS_MemList;
        SYS_MemList=Mem;
    }
    /* Unlock the access of the list */
    DRV_OS_MutexRelease(m_AvmemMutex);
#ifdef DEBUG_AVMEM    
	printf("------allocate 0x%08x,MemoryPrivate 0x%08x\n",Mem->MmzBuf.user_viraddr,Mem->MmzBuf.kernel_viraddr);
#endif	
    return((U32)Mem->MmzBuf.user_viraddr);
}

/*删除了内存和内存的管理接口，但是链表中的指针没有删除，传入的是用户态地址*/
void drv_AvmemFree(void* addr)
{
    ST_ErrorCode_t errCode;
    SYS_MemHandle_t *Mem,*MemPrevious;
    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);
    
    DRV_OS_MutexLock(m_AvmemMutex);
    Mem=MemPrevious=SYS_MemList;
 /* Look for the memory descriptor in the list */
    while(Mem!=NULL)
    {
        if(Mem->MmzBuf.user_viraddr==(U32)addr) break;
        MemPrevious=Mem;
        Mem=Mem->MemoryNext;
    }
 /* If not found, go out */
    if(Mem==NULL)
    {
        DRV_OS_MutexRelease(m_AvmemMutex);
        return;
    }
    /* Detach the element from the list */
    if (Mem==SYS_MemList) SYS_MemList=Mem->MemoryNext;
    else MemPrevious->MemoryNext=Mem->MemoryNext;
    /* Unlock the access to the list */
    DRV_OS_MutexRelease(m_AvmemMutex);

    errCode= HI_MMZ_Free(Mem->MmzBuf);
    if(errCode!=ST_NO_ERROR)
    {
        STAPI_ERROR(errCode);
    }
#ifdef DEBUG_AVMEM    
	printf("------free 0x%08x,MemoryPrivate 0x%08x\n",addr,Mem->MmzBuf.kernel_viraddr);
#endif	
    free(Mem);
}
/*删除链表中的指针，传入的是用户态地址*/
void drv_AvmemRemoveRecord(void* addr)
{
    SYS_MemHandle_t *Mem,*MemPrevious;
    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);
    
    DRV_OS_MutexLock(m_AvmemMutex);
    Mem=MemPrevious=SYS_MemList;
 /* Look for the memory descriptor in the list */
    while(Mem!=NULL)
    {
        if(Mem->MmzBuf.user_viraddr==(U32)addr) break;
        MemPrevious=Mem;
        Mem=Mem->MemoryNext;
    }
 /* If not found, go out */
    if(Mem==NULL)
    {
        DRV_OS_MutexRelease(m_AvmemMutex);
        return;
    }
    /* Detach the element from the list */
    if (Mem==SYS_MemList) SYS_MemList=Mem->MemoryNext;
    else MemPrevious->MemoryNext=Mem->MemoryNext;
    /* Unlock the access of the list */
    DRV_OS_MutexRelease(m_AvmemMutex);
    free(Mem);
}


U32 drv_AvmemGetBufferId(void* addr)
{
    SYS_MemHandle_t *Mem,*MemPrevious;
    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);

    DRV_OS_MutexLock(m_AvmemMutex);
    Mem=MemPrevious=SYS_MemList;
 /* Look for the memory descriptor in the list */
    while(Mem!=NULL)
    {
        if(Mem->MmzBuf.user_viraddr==(U32)addr) break;
        MemPrevious=Mem;
        Mem=Mem->MemoryNext;
    }
 /* If not found, go out */
    if(Mem==NULL)
    {
        DRV_OS_MutexRelease(m_AvmemMutex);
        return 0;
    }
    /* Unlock the access of the list */
    DRV_OS_MutexRelease(m_AvmemMutex);

    return Mem->MmzBuf.kernel_viraddr;
}
/*得到内核态地址*/
U32 drv_AvmemGetDeviceAddr(void* addr)
{
    SYS_MemHandle_t *Mem,*MemPrevious;
    if(m_AvmemMutex == NULL)
        DRV_OS_MutexCreate(NULL,&m_AvmemMutex);

    DRV_OS_MutexLock(m_AvmemMutex);
    Mem=MemPrevious=SYS_MemList;
 /* Look for the memory descriptor in the list */
    while(Mem!=NULL)
    {
        if(Mem->MmzBuf.user_viraddr==(U32)addr) break;
        MemPrevious=Mem;
        Mem=Mem->MemoryNext;
    }
 /* If not found, go out */
    if(Mem==NULL)
    {
        DRV_OS_MutexRelease(m_AvmemMutex);
        return 0;
    }

    /* Unlock the access of the list */
    DRV_OS_MutexRelease(m_AvmemMutex);

    return Mem->MmzBuf.phyaddr;
}

/*返回的是用户态地址*/
void* DRV_AvmemAllocate(U32 Size,U32 Alignment)
{
	return(drv_AvmemAllocateUncached(0,Size,Alignment));
}
void DRV_AvmemFree(void* addr)
{
	drv_AvmemFree(addr);
	drv_AvmemRemoveRecord(addr);
}

