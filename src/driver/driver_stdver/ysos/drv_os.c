/*****************************************************************************
 *
 * Description: 
 *    椹卞姩灞侽S妯″潡瀹炵幇鏂囦欢
 *
 * Revision History:
 *    Date          Author         Modification
 * ---------------------------------------------------------------------
 * 1. 2004-03-12    Cao Songtao    鍒涘缓
 * 2. 2005-12-29    Qiujordan      澧炲姞浜掓枼鐨勫皝瑁
 *
 *****************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <errno.h>

#include "drv_basic.h"
#include "drv_os.h"

/* 获取系统运行情况所需接口，包含sysinfo结构体信息等*/
#include <linux/kernel.h>     
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

typedef void (*Task_Fun)(void*);
typedef void (*TaskOnexit_Fun)(pthread_t, int);

#if 1
#define DEBUG_PRINT(format,arg...) printf(format,##arg)
#else
#define DEBUG_PRINT(format,arg...)  
#endif

#define  OS_MAX_OUT_BUFFER  512

#define CURRENT_MODULE DRV_MODULE_OS
S32_YS m_OS_ErrPrintMask = 0xffffffff;
S32_YS m_OS_DebugPrintMask = 0xffffffff;


OS_Device_t mOsDevice ={0};

void Err_Print(char *Format_p, ...)
{
    va_list Argument;
    if (0 == m_OS_ErrPrintMask)
        return ;

    va_start(Argument, Format_p);
    vfprintf(stdout, Format_p, Argument);
    va_end(Argument);
}

void Debug_Print(DRVOS_PrintLevel_e level, char *Format_p, ...)
{
    va_list Argument;
  if (level & m_OS_DebugPrintMask)
  {

    va_start(Argument, Format_p);
    vfprintf(stdout, Format_p, Argument);
    va_end(Argument);
  }

}

void Open_Err_Print_Output()
{
	m_OS_ErrPrintMask = 0xffffffff;
}

void Close_Err_Print_Output()
{
	m_OS_ErrPrintMask = 0;
}
void Open_Debug_Print_Output()
{
	m_OS_DebugPrintMask = 0xffffffff;
}

void Close_Debug_Print_Output()
{
	m_OS_DebugPrintMask = 0;
}

#if 1/*memory function*/
/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OSMalloc(DRVOS_Partition_p Partition_p, size_t Size)
{
    void *pAddress = NULL;

    if(Partition_p == NULL || Size == 0)
    {
/*        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER); */ //dyin 杩欎釜閿欒浼氭湁寰堝
    }
    else
    {
        if(Partition_p == 0xFFFFFFFF)
        {
            Partition_p = SystemPartition;
        }
        pAddress = memory_allocate_heap(Partition_p, Size);
        if(pAddress == NULL)
        {
            YSDRIVER_ERRORCODE_M(DRV_ERROR_NO_MEMORY,"size[%d]",Size);
        }
        else
        {
            YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    
    return pAddress;
}

/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OS_MallocCLR(DRVOS_Partition_p Partition_p, size_t Size)
{
    void *pAddress = NULL;

    if(Partition_p == NULL || Size == 0)
    {
        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        if(Partition_p == 0xFFFFFFFF)
        {
            Partition_p = SystemPartition;
        }
        pAddress = memory_allocate_heap(Partition_p, Size);
        if(pAddress == NULL)
        {
            YSDRIVER_ERRORCODE_M(DRV_ERROR_NO_MEMORY,"size[%d]",Size);
        }
        else
        {
            memset(pAddress,0,Size);
            YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    
    return pAddress;
}

/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OSMalloc_NoPrint(DRVOS_Partition_p Partition_p, size_t Size)
{
    void *pAddress = NULL;

    if(Partition_p == NULL || Size == 0)
    {
		//YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        if(Partition_p == 0xFFFFFFFF)
        {
            Partition_p = SystemPartition;
        }
        pAddress = memory_allocate_heap_noprint(Partition_p, Size);
        if(pAddress == NULL)
        {
            //YSDRIVER_ERRORCODE_M(DRV_ERROR_NO_MEMORY,"size[%d]",Size);
        }
        else
        {
            //YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    
    return pAddress;
}

/*************************************************
 * Function:       
 * Description:   Reallocate a block of memory from a partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:     void* memory_reallocate(partition_t *part, void* block, size_t size)
 *************************************************/
void *DRV_OS_Realloc(DRVOS_Partition_p Partition_p, void *Block_p, size_t Size)
{
    void *pAddress = NULL;

    if(Partition_p == NULL || Block_p == NULL || Size == 0)
    {
        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        if(Partition_p == 0xFFFFFFFF)
        {
            Partition_p = SystemPartition;
        }
        
        pAddress = memory_reallocate_heap(Partition_p, Block_p, Size);

        if(pAddress == NULL)
        {
            YSDRIVER_ERRORCODE(DRV_ERROR_NO_MEMORY);
        }
        else
        {
            YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    
    return pAddress;
}

/*************************************************
 * Function:       
 * Description:   Free a block of memory back to a partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:         void memory_deallocate(partition_t *part, void* block)
 *************************************************/
void DRV_OSFree(DRVOS_Partition_p Partition_p, void *Block_p)
{
    if(Partition_p == NULL || Block_p == NULL )
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        if(Partition_p == 0xFFFFFFFF)
        {
            Partition_p = SystemPartition;
        }

        memory_deallocate_heap(Partition_p, Block_p);
    }
    
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

DRVOS_Partition_p DRV_OSPartCreate(CHAR_YS *name,void *Memory_p, size_t Size)
{
    DRVOS_Partition_p pAddress = NULL;

    if(Memory_p == NULL || Size == 0)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        
        pAddress = (DRVOS_Partition_p)partition_create_heap(Memory_p, Size);

        if(pAddress == NULL)
        {
            YSDRIVER_ERRORCODE(DRV_ERROR_NO_MEMORY);
        }
        else
        {
            mOsDevice.PartialPartitionNb ++;
            YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    if(name != NULL)    
    	printf("DRV_OSPartCreate %s,0x%08x\n",name,pAddress);
    return pAddress;
}

/*************************************************
 * Function:       
 * Description:   Initialize a heap partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:       int partition_init_heap(partition_t*,void*,size_t); 
  NOTE: this version just be used in the part space of into parameter 
          is in static space
*************************************************/
int DRV_OSPartInit(CHAR_YS *name,DRVOS_Partition_p Partition_p, void *Memory_p, size_t Size)
{
    int result = -1;

    if(Partition_p == NULL || Memory_p == NULL || Size == 0)
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);

    result = partition_init_heap(Partition_p, Memory_p, Size);

    if(result == -1)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_NO_MEMORY);
    }
    else
    {
        mOsDevice.PartialPartitionNb ++;
        YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    }
    if(name != NULL)    
    	printf("DRV_OSPartInit %s,0x%08x\n",name,Partition_p);
    return result;
}

DRV_ErrCode DRV_OSPartDel(DRVOS_Partition_p Partition_p)
{
    if(Partition_p == NULL)
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);

    partition_delete(Partition_p);
    if(mOsDevice.PartialPartitionNb <= 0)
    {
        OS_ERROR(("mOsDevice.PartitionNb is zero "));
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }
    mOsDevice.PartialPartitionNb--;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}
/*************************************************
* Function:       
* Description:   Get status of a partition.
* Input:          StatusMask   选择希望了解信息
* Output:         
* Return:        
* Others:    int partition_status(partition_t* , partition_status_t* , partition_status_flags_t )     
*************************************************/
BOOL_YS DRV_OSPartStatus(DRVOS_Partition_t *part,DRVOS_PartitionStatus_t *status)

{
  BOOL_YS IsSuccess = TRUE_YS;
  S32_YS result;
  
  if ((NULL_YS != part) && (NULL_YS != status))
  {
    result = partition_status(part, status,0);
  
    if (0 != result)
    {
      IsSuccess = FALSE_YS;
      Err_Print("DRV_OS_PartStatus failed, Part is 0x%X!\n", (S32_YS)part);
    }
    else
    {
      #ifdef DRV_OS_DEBUG
      {
        Debug_Print(PRINT_LEVEL_ALL, "^^^^^^^^^^^^^^^^^^^^^^\n");
        Debug_Print(PRINT_LEVEL_ALL, "Partition 0x%8.8X size:0x%8.8x, used:0x%8.8x, free:0x%8.8x, largest free block:0x%8.8x\n", 
          (S32_YS)part, status->PartSize, status->PartUsedSize, status->PartFreeSize, status->PartLargestFreeBlockSize);
        Debug_Print(PRINT_LEVEL_ALL, "^^^^^^^^^^^^^^^^^^^^^^\n");
      }
      #endif
    }
  }
  else
  {
    IsSuccess = FALSE_YS;
    Err_Print("DRV_OS_PartStatus parameter error, part is 0x%X, status is 0x%X!\n", (S32_YS)part, (S32_YS)status);
  }
  
  return IsSuccess;
}

DRVOS_Partition_p DRV_OSGetSysPartition()
{
    return SystemPartition;
    
}
void drv_OSDeletFree(void *memory)
{
  free(memory);
}
#endif

#if 0//moved by jyshan
DRVOS_Partition_p DRV_OsNewPartition(int Size)
{
    void *addr = NULL;
    DRVOS_Partition_p partition = NULL;
    
    addr = malloc( Size );
    if(addr == NULL)
    {
        YSDRIVER_ERRORCODE_M(DRV_ERROR_NO_MEMORY,"size:%d\n",Size);
    }
    else
    {
        memset(addr,0,Size);
        
        partition = (DRVOS_Partition_p)partition_create_heap(addr, Size);

        if(partition == NULL)
        {
            free(addr);
            YSDRIVER_ERRORCODE_M(DRV_ERROR_NO_MEMORY,"size:%d\n",Size);
        }
        else
        {
            mOsDevice.FullPartitionNb ++;
            YSDRIVER_ERRORCODE(DRV_NO_ERROR);
        }
    }
    return partition;
}

void DRV_OsFreePartition(DRVOS_Partition_p Partition_p)
{
    void * addr;
    if(Partition_p == NULL)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
        return;
    }
    addr = partition_get_baseaddr(Partition_p);
    partition_delete(Partition_p);
    free(addr);
    YSDRIVER_ERRORCODE_M(DRV_NO_ERROR,"partion_p:%x",Partition_p);

    if(mOsDevice.FullPartitionNb <= 0)
        OS_ERROR(("Full Partiton manager error\n"));
    mOsDevice.FullPartitionNb --;

    return;
}
#endif


#if 1/*task function*/
void taskfuc(void *para)
{
    DRVOS_Task_t *task = (DRVOS_Task_t*)para;

    printf("***********************pthread[%s] tid:%ld\n",task->Name,(long)syscall(SYS_gettid));
    return *((void *(*)(void*))(task->Func))(task->Para);
}
    



/*************************************************
 * Function:       
 * Description:   Create an task.
 * Input:          
 * Output:         
 * Return:        
 * Others:   task_t* task_create(void (*)(void*), 
              void* , 
              size_t ,
              int , 
              const char* , 
              task_flags_t );
      void (*Task_Fun)(void*);
*************************************************/
DRVOS_Task_t *DRV_OS_TaskCreate(char  *Name,
                               void  *Function,
                               int    Priority,
                               size_t StackSize,
                               void  *Param,
                               int    Flags)
{
    DRV_ErrCode errCode = DRV_NO_ERROR;
    DRVOS_Task_t *newTask = NULL;
    struct sched_param  sparam;
    pthread_attr_t      attr;

    /* To avoid warnings */
    if(Priority > 50)
    	Priority = 50;
    pthread_attr_init(&attr);
    if (Priority >= 0)
    {
        if (Priority < sched_get_priority_min(SCHED_RR))
            Priority = sched_get_priority_min(SCHED_RR);

        if (Priority > sched_get_priority_max(SCHED_RR))
            Priority = sched_get_priority_max(SCHED_RR);

        sparam.sched_priority = Priority;
        if(Priority == 50)
        {
          pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
          printf("High priority Task: [%s] priority[%d]\n",Name,Priority);
        }
        pthread_attr_setschedpolicy(&attr, SCHED_RR);
        pthread_attr_setschedparam(&attr, &sparam);
    }
#if 0
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#endif    
    /* 澶氬ぇ鍚堥�傚憿? glibc浼氫繚璇佹渶灏弒tack */
    pthread_attr_setstacksize(&attr, StackSize); 

    newTask = (DRVOS_Task_t *) DRV_OSMalloc(0xFFFFFFFF, sizeof(DRVOS_Task_t));
    if (newTask == NULL)
    {
        errCode = ST_ERROR_NO_MEMORY;
    }
    else
    {
        newTask->Func = Function;
        newTask->Para = Param;
        newTask->Priority = Priority;
        strncpy(newTask->Name,Name,32);
        errCode = pthread_create(&newTask->ThreadId, &attr, (void *(*)(void*))taskfuc, (void *)newTask);
        if(errCode != 0)
        {
            DRV_OSFree(0xFFFFFFFF, newTask);
            newTask = NULL;
            OS_ERROR(("pthread_create{%s]failed[0x%x]\n",Name,errCode));
            errCode  = ST_ERROR_BAD_PARAMETER;
        }
        else
        {
            if(mOsDevice.MaxPriority < Priority)
            {
                mOsDevice.MaxPriority = Priority;
            }
            newTask->NeedFree = TRUE;
            mOsDevice.TaskCreateNb++;
            printf("pthread name[%s] id[%d]\n",Name,newTask->ThreadId);
        }
    }
    pthread_attr_destroy(&attr);
    YSDRIVER_ERRORCODE(errCode);
    return newTask;
}

/*************************************************
 * Function:       
 * Description:   Initialize an task.
 * Input:          
 * Output:         
 * Return:        
 * Others:      int task_init(void (*)(void*),
            void* Param,
            void* Stack,
            size_t StackSize,
            task_t* Task,
            tdesc_t* Tdesc,
            int Priority,
            const char* Name,
            task_flags_t flags);
      void (*Task_Fun)(void*);
      note: 姝ゅ嚱鏁板彧鑳戒竴娆′竴娆′娇鐢紝姣忔浣跨敤
      蹇呴』淇濊瘉鍓嶄竴娆″垱寤虹殑浠诲姟宸茬粡閿�姣侊紝鍚
      鍒欏皢鐮村潖鍓嶄竴娆″垱寤轰换鍔＄殑鎵ц鐜锛屽彲
      閫氳繃淇敼涓洪潤鎬佷换鍔￠槦鍒楁潵鏀瑰彉姝ら檺鍒躲�
*************************************************/
int DRV_OSTaskInit(char         *Name,
                   void          *Function,
                   DRVOS_Task_t  *Task,
                   DRVOS_Tdesc_t *Tdesc,
                   int            Priority,
                   void          *Stack,
                   size_t         StackSize,
                   void          *Param,
                   int            Flags)
{
    ST_ErrorCode_t errCode = ST_NO_ERROR;
    pthread_attr_t      attr;
    struct sched_param  sparam;

    pthread_attr_init(&attr);
    if (Priority >= 0)
    {
        if (Priority < sched_get_priority_min(SCHED_RR))
            Priority = sched_get_priority_min(SCHED_RR);

        if (Priority > sched_get_priority_max(SCHED_RR))
            Priority = sched_get_priority_max(SCHED_RR);

        sparam.sched_priority = Priority;
        if(Priority > 4)
        {
			//pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);
            DEBUG_PRINT("High priority Task: [%s] priority[%d]\n",Name,Priority);
        }            
        pthread_attr_setschedpolicy(&attr, SCHED_RR);
        pthread_attr_setschedparam(&attr, &sparam);
    }
    #if 0
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    #endif
    /* 澶氬ぇ鍚堥�傚憿? glibc浼氫繚璇佹渶灏弒tack */
    pthread_attr_setstacksize(&attr, StackSize); 

    if(Task == NULL)
    {
        errCode = DRV_ERROR_BAD_PARAMETER;
    }
    else
    {
        errCode = pthread_create(&Task->ThreadId, &attr, (void*)Function, (void *)Param);
        if(errCode != 0) /* some thing wrong happel*/
        {
            OS_ERROR(("pthread_create failed[0x%x]\n",errCode));
            errCode = DRV_ERROR_BAD_PARAMETER;
        }
        else
        {
            if(mOsDevice.MaxPriority < Priority)
            {
                mOsDevice.MaxPriority = Priority;
            }
            Task->NeedFree = FALSE;            
            mOsDevice.TaskInitNb++;
        }
    }
    
    pthread_attr_destroy(&attr);
    YSDRIVER_ERRORCODE_RETURN(errCode);
}

/*************************************************
 * Function:       
 * Description:   Exit the current task.
 * Input:          
 * Output:         
 * Return:        
 * Others:         void task_exit(int param)
 *************************************************/
void DRV_OSTaskExit(int Param)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

/*************************************************
 * Function:       
 * Description:   Delete an task.
 * Input:          
 * Output:         
 * Return:        
 * Others:         int task_delete(task_t* task)
 *************************************************/
int DRV_OSTaskDelete(DRVOS_Task_p Task)
{
    int result = 0;

    if (Task == NULL)
    {
        result = ST_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = pthread_kill(Task->ThreadId,0);
        if(result != ESRCH)
        {
            OS_ERROR(("the task not exit, can not delete[%d]\n",result));
            result =  ST_ERROR_TIMEOUT;
        }
        else
        {
            result = 0;
            if(Task->NeedFree == TRUE)
            {
                if(mOsDevice.TaskCreateNb <= 0)
                    OS_ERROR(("Task usage wrong\n"));
                mOsDevice.TaskCreateNb--;            
                /* Deallocate the task memory */
                DRV_OSFree(0xFFFFFFFF, Task);  
            }
            else
            {
                if(mOsDevice.TaskInitNb <= 0)
                    OS_ERROR(("Task usage wrong\n"));
                mOsDevice.TaskInitNb--;            
            }
        }
    }
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    return result;
}

/*************************************************
 * Function:       
 * Description:   Delete an task.
 * Input:          
 * Output:         
 * Return:        
 * Others:         int task_delete(task_t* task)
 *************************************************/
int DRV_OS_TaskKill(DRVOS_Task_p Task, S32_YS param, DRVOS_TaskKillFlag_e flag)
{
    int result = 0;

    if (Task == NULL)
    {
        result = ST_ERROR_BAD_PARAMETER;
    }
    else
    {
        result = pthread_cancel(Task->ThreadId);
        if(result != 0)
        {
            OS_ERROR(("cancel pthread failed[0x%x]\n",result));
        }
        else
        {
            result = pthread_join(Task->ThreadId,NULL);
            if(result != 0)
            {
                OS_ERROR(("join pthread failed[0x%x]\n",result));
            }
            if(Task->NeedFree == TRUE)
            {
                if(mOsDevice.TaskCreateNb <= 0)
                    OS_ERROR(("Task usage wrong\n"));
                mOsDevice.TaskCreateNb--;            
                /* Deallocate the task memory */
                DRV_OSFree(0xFFFFFFFF, Task);  
            }
            else
            {
                if(mOsDevice.TaskInitNb <= 0)
                    OS_ERROR(("Task usage wrong\n"));
                mOsDevice.TaskInitNb--;            
            }
        }
    }
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    return result;
}

/*************************************************
 * Function:       
 * Description:   Prevent task rescheduling.
 * Input:          
 * Output:         
 * Return:        
 * Others:    void task_lock(void);     
 *************************************************/
void DRV_OS_TaskLock(void)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

/*************************************************
 * Function:       
 * Description:   Allow task rescheduling.
 * Input:          
 * Output:         
 * Return:        
 * Others:      void task_unlock(void);   
 *************************************************/
void DRV_OS_TaskUnlock(void)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

DRVOS_Task_t *DRV_OS_TaskId(void)
{/*same*/
  return (DRVOS_Task_t*)pthread_self();
}

int DRV_OSTaskPrioritySet(DRVOS_Task_t *Task_p, S32_YS NewPriority)
{
    int result = 0;
	struct sched_param p; 
	p.sched_priority= NewPriority; 
	result = pthread_setschedparam(Task_p,SCHED_RR,&p);
    return result;
}

void DRV_OS_TaskOnExitSet(DRVOS_TaskOnexit_Fun function)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}
/*************************************************
 * Function:       
 * Description:   Delay the calling task for a period of time.
 * Input:          
 * Output:         
 * Return:        
 * Others:         void task_delay(clock_t delay)
 *************************************************/
void DRV_OS_TaskDelay(unsigned int Delay)
{
    struct timespec x;

    /* if < 2ms */
    if (Delay <= 2)
    {
        x.tv_sec  = 0;
        x.tv_nsec = 2000001;
    }
    else
    {
        x.tv_sec  = Delay / 1000;
        x.tv_nsec = (Delay % 1000) * 1000000; 
    }
    nanosleep (&x, 0);
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}
void DRV_OS_TaskDelay_MS(unsigned int Delay)
{
    struct timespec x;

    /* if < 2ms */
    if (Delay <= 2)
    {
        x.tv_sec  = 0;
        x.tv_nsec = 2000001;
    }
    else
    {
        x.tv_sec  = Delay / 1000;
        x.tv_nsec = (Delay % 1000) * 1000000; 
    }
    nanosleep (&x, 0);
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

/*************************************************
 * Function:       
 * Description:   Waits until one of a list of tasks completes.
 * Input:          
 * Output:         
 * Return:        
 * Others:   int task_wait(task_t **tasklist,int ntasks,const clock_t *timeout);
 *************************************************/
int DRV_OS_TaskWait(DRVOS_Task_t    *Task_p, 
                   DRVOS_WaitMode_e WaitMode, 
                   int              MsToWait)
{
    int result = 0;

    if(WaitMode == DRVOS_WAIT_NO)
    {
        result = pthread_tryjoin_np(Task_p->ThreadId,NULL);
    }
    else if(WaitMode == DRVOS_WAIT_FINITY)
    {
        struct timespec ts;
        struct timeval  tv;
        int sec, msec;

        gettimeofday(&tv, NULL);

        sec = MsToWait / 1000;
        msec = MsToWait - (sec * 1000);
    
        tv.tv_sec += sec;
        tv.tv_usec += msec * 1000;
    
        sec = tv.tv_usec / 1000000;
        tv.tv_usec = tv.tv_usec - (sec * 1000000);
        tv.tv_sec += sec;

        ts.tv_sec = tv.tv_sec;
        ts.tv_nsec = ((long)tv.tv_usec*1000);
        result = pthread_timedjoin_np(Task_p->ThreadId,NULL,&ts);
    }
    else if(WaitMode == DRVOS_WAIT_INFINITY)
    {
        result = pthread_join(Task_p->ThreadId,NULL);

    }
    YSDRIVER_ERRORCODE_RETURN(result);
}

#endif


#if 1
DRV_ErrCode DRV_OS_MutexCreate(CHAR_YS *name, DRV_MutexHandle *Mutex_p)
{
    int Result;
    DRVOS_Mutex_t *Mutex;
    pthread_mutexattr_t attr;
    Mutex = (DRVOS_Mutex_t*)DRV_OSMalloc(0xFFFFFFFF,sizeof(DRVOS_Mutex_t));
    if(Mutex != NULL)
    {
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE_NP);
        Result = pthread_mutex_init(&Mutex->mutex, &attr);
        if(Result != 0)
		{
            *Mutex_p = NULL;
            YSDRIVER_ERRORCODE_RETURN( DRV_OS_ERROR_MUTEX_CREATE);
		}
	    else
	    {
            Mutex->NeedFree = TRUE;
        mOsDevice.MutexNb++;
            *Mutex_p = Mutex;
	    }
	}
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}
DRV_ErrCode DRV_OS_MutexDel(DRV_MutexHandle Mutex_p)
{
    DRVOS_Mutex_t* Mutex = (DRVOS_Mutex_t*)Mutex_p;

    if(NULL == Mutex)
      return DRV_ERROR_BAD_PARAMETER;
      
    if(mOsDevice.MutexNb == 0)
        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
        
    if(pthread_mutex_destroy(&Mutex->mutex) != 0) 
    {
       OS_ERROR(("DRV_OsMutexDelete: Error, pthread_mutex_destroy failure.\n"));
    }

    if(Mutex->NeedFree == TRUE)
    {
        DRV_OSFree(0xFFFFFFFF, Mutex_p);  
    }

    mOsDevice.MutexNb--;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

DRV_ErrCode DRV_OS_MutexLock(DRV_MutexHandle Mutex_p)
{
    DRVOS_Mutex_t* Mutex = (DRVOS_Mutex_t*)Mutex_p;

	if(Mutex == NULL)
		return -1;

    pthread_mutex_lock(&Mutex->mutex);
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

DRV_ErrCode DRV_OS_MutexRelease(DRV_MutexHandle Mutex_p)
{
    DRVOS_Mutex_t* Mutex = (DRVOS_Mutex_t*)Mutex_p;

	if(Mutex == NULL)
		return -1;

    pthread_mutex_unlock(&Mutex->mutex);
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}
#endif

#if 1
int DRV_OSSemInit(DRVOS_Sem_t *Semaphore, S32_YS InitialTokenCount)
{
    BOOL_YS  Result;
    sem_t *Sem = (sem_t*)Semaphore;

    if(NULL == Sem)
      return DRV_ERROR_BAD_PARAMETER;
  
    Result = sem_init(Sem, 0, InitialTokenCount);
    if (Result != 0) 
    {
        OS_ERROR(("DRV_OS_SemInit: Error, sem_init failure.\n"));
    }
    else
    {
      Semaphore->delet = NULL;
      mOsDevice.SemaphoreNb++;
    }

    return Result;
}

DRVOS_Sem_t* DRV_OS_SemCreate(CHAR_YS *name, S32_YS InitialTokenCount, DRVOS_InitMode_e SemMode)
{
    DRVOS_Sem_t *Sem;
    int Result;

    Sem = (DRVOS_Sem_t*)malloc(sizeof(*Sem));
    if(NULL != Sem)
    {
      Result = DRV_OSSemInit(Sem, InitialTokenCount);

      if(0 != Result)
      {
        free(Sem);
        Sem = NULL;
      }
      else
      {
        Sem->delet = drv_OSDeletFree;
      }
    }
    
    return Sem;
}


void DRV_OS_SemDel(DRVOS_Sem_t *Semaphore)
{
    sem_t *sem = (sem_t*)Semaphore;

    if(NULL == sem)
      return;
      
    if(mOsDevice.SemaphoreNb == 0)
        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
        
    if(sem_destroy(sem) != 0) 
    {
        OS_ERROR(("DRV_OsSemaphoreDelete: Error, sem_destroy failure.\n"));
    }

    if(Semaphore->delet == drv_OSDeletFree)
    {
      Semaphore->delet(Semaphore);
    }
        
    mOsDevice.SemaphoreNb --;
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

int DRV_OS_SemWait(DRVOS_Sem_t *Semaphore, DRVOS_WaitMode_e WaitMode, U32_YS MsToWait)
{
    sem_t *sem = (sem_t*)Semaphore;
    int result;
    struct timespec ts;
    struct timeval  tv;
	int sec, msec;
	int time;
	if(sem == NULL)
		return -1;
    if((WaitMode == DRVOS_WAIT_FINITY) && (MsToWait == 0))
    	WaitMode = DRVOS_WAIT_NO;
    switch(WaitMode)
    {
    	case DRVOS_WAIT_FINITY:
			time = MsToWait;
		    gettimeofday(&tv, NULL);
		    sec = time / 1000;
		    msec = time - (sec * 1000);
		    tv.tv_sec += sec;
		    tv.tv_usec += msec * 1000;
		    sec = tv.tv_usec / 1000000;
		    tv.tv_usec = tv.tv_usec - (sec * 1000000);
		    tv.tv_sec += sec;
		    ts.tv_sec = tv.tv_sec;
		    ts.tv_nsec = ((long)tv.tv_usec*1000);
		    result = sem_timedwait(sem, &ts);
    	break;
    	case DRVOS_WAIT_NO:
    		result = sem_trywait(sem);
    	break;
    	case DRVOS_WAIT_INFINITY:
    		result = sem_wait(sem);
    	break;    	
    }
    return result;
}

int DRV_OS_SemWait_MS(DRVOS_Sem_t *Semaphore, DRVOS_WaitMode_e WaitMode, U32_YS MsToWait)
{
    sem_t *sem = (sem_t*)Semaphore;
    int result;
    struct timespec ts;
    struct timeval  tv;
	int sec, msec;
	int time;
	if(sem == NULL)
		return -1;
	if((MsToWait == 0)&&(WaitMode == DRVOS_WAIT_FINITY))
	    WaitMode = DRVOS_WAIT_NO;
    switch(WaitMode)
    {
    	case DRVOS_WAIT_FINITY:
			time = MsToWait;
		    gettimeofday(&tv, NULL);
		    sec = time / 1000;
		    msec = time - (sec * 1000);
		    tv.tv_sec += sec;
		    tv.tv_usec += msec * 1000;
		    sec = tv.tv_usec / 1000000;
		    tv.tv_usec = tv.tv_usec - (sec * 1000000);
		    tv.tv_sec += sec;
		    ts.tv_sec = tv.tv_sec;
		    ts.tv_nsec = ((long)tv.tv_usec*1000);
		    result = sem_timedwait(sem, &ts);
    	break;
    	case DRVOS_WAIT_NO:
    		result = sem_trywait(sem);
    	break;
    	case DRVOS_WAIT_INFINITY:
    		result = sem_wait(sem);
    	break;    	
    }
    return result;
}

int DRV_OSSemSignal(DRVOS_Sem_t *Semaphore)
{
    sem_t *sem = (sem_t*)Semaphore;

    if(NULL == sem)
      return -1;

    if(sem_post(sem) == -1)
    {
        OS_ERROR(("DRV_OsSemaphoreSignal: Error, sem_post failure.\n"));
    }
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);

    return 0;
}
int DRV_OSSemGetVaule(DRVOS_Sem_t *Semaphore,S32 *Vaule)
{
    sem_t *sem = (sem_t*)Semaphore;
	int semval;
    if(NULL == sem)
      return -1;
	sem_getvalue(sem,&semval);
	*Vaule = semval;
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

#endif

#if 1    /* Message*/
/*in drv_message.c*/
#endif


#if 1
static unsigned int ori_timesec = 0;

void drv_OsTimeInit(void)
{
    struct timeval   tv;

    if (gettimeofday(&tv, NULL) == 0)
    {
        ori_timesec =  tv.tv_sec*1000 + tv.tv_usec/1000;
    }
    else
    {
        OS_ERROR(("gettimeofday falied"));
    }
}

unsigned int DRV_OS_TimeMs2Ticks(unsigned int Ms)
{
	return (DRV_OS_TimeGetTickPerSecond() / 1000 * Ms);
}

unsigned int DRV_OS_TimeTicks2Ms(unsigned int Ticks)
{
	return (Ticks / (DRV_OS_TimeGetTickPerSecond() / 1000));
}

U32_YS DRV_OS_TimeGetTickPerSecond(void)
{
	return 1000;
}


/*************************************************
 * Function:       
 * Description:   Return the current time.
 * Input:          
 * Output:         
 * Return:        
 * Others:   clock_t time_now(void);      
 *************************************************/
unsigned int DRV_OS_TimeNow(void)
{
    struct timeval   tv;
    unsigned int          Clk = 1;	    /* Default value */

    if (gettimeofday(&tv, NULL) == 0)
    {
        Clk =  tv.tv_sec*1000
            + tv.tv_usec/1000;
    }
    else
    {
        OS_ERROR(("gettimeofday falied"));
    }

    return Clk - ori_timesec;
}

/*************************************************
 * Function:       
 * Description:   Return whether one time is after another.
 * Input:          
 * Output:         
 * Return:    Returns 1 if time1 is after time2, otherwise 0.    
 * Others:    int time_after(const clock_t time1, const clock_t time2);     
 *************************************************/
int DRV_OS_TimeAfter(unsigned int Time1, unsigned int Time2)
{
    return ((int)(Time2) - (int)(Time1) < 0);
}

/*************************************************
 * Function:       
 * Description:   Subtract two clock values.No overflow checking takes 
          place because the clock values are cyclic.
          * Input:          
          * Output:         
          * Return:     Returns the result of subtracting Time2 from Time1.   
          * Others:   clock_t time_minus(const clock_t Time1, const clock_t Time2);      
          *************************************************/
unsigned int DRV_OS_TimeMinus(unsigned int Time1, unsigned int Time2)
{
    return Time1-Time2;
}

/*************************************************
 * Function:       
 * Description:   Add two clock values.No overflow checking takes 
              place because the clock values are cyclic.
              * Input:          
              * Output:         
              * Return:        
              * Others:    clock_t time_plus(const clock_t Time1, const clock_t Time2);     
              *************************************************/
unsigned int DRV_OS_TimePlus(unsigned int Time1, unsigned int Time2)
{
    return Time1 + Time2;
}

unsigned int DRV_OS_TimeWait(unsigned int WaitTime)
{
    return DRV_OS_TimePlus(DRV_OS_TimeNow(), WaitTime);
}

/*************************************************
 * Function:       
 * Description:   Return the current time.
 * Input:          
 * Output:         
 * Return:        
 * Others:   clock_t time_now(void);      
 *************************************************/
unsigned int DRV_OS_TimeNow_MS(void)
{
    struct timeval   tv;
    unsigned int          Clk = 1;	    /* Default value */

    if (gettimeofday(&tv, NULL) == 0)
    {
        Clk =  tv.tv_sec*1000
            + tv.tv_usec/1000;
    }
    else
    {
        OS_ERROR(("gettimeofday falied"));
    }

    return Clk - ori_timesec;
}
unsigned int DRV_OS_TimeNow_US(void)
{
    struct timeval   tv;
    unsigned int          Clk = 1;	    /* Default value */

    if (gettimeofday(&tv, NULL) == 0)
    {
        Clk =  tv.tv_sec*1000*1000
            + tv.tv_usec;
    }
    else
    {
        OS_ERROR(("gettimeofday falied"));
    }

    return Clk - ori_timesec*1000;
}

/*************************************************
 * Function:       
 * Description:   Return whether one time is after another.
 * Input:          
 * Output:         
 * Return:    Returns 1 if time1 is after time2, otherwise 0.    
 * Others:    int time_after(const clock_t time1, const clock_t time2);     
 *************************************************/
int DRV_OS_TimeAfter_MS(unsigned int Time1, unsigned int Time2)
{
    return ((int)(Time2) - (int)(Time1) < 0);
}

/*************************************************
 * Function:       
 * Description:   Subtract two clock values.No overflow checking takes 
          place because the clock values are cyclic.
          * Input:          
          * Output:         
          * Return:     Returns the result of subtracting Time2 from Time1.   
          * Others:   clock_t time_minus(const clock_t Time1, const clock_t Time2);      
          *************************************************/
unsigned int DRV_OS_TimeMinus_MS(unsigned int Time1, unsigned int Time2)
{
    return Time1-Time2;
}

/*************************************************
 * Function:       
 * Description:   Add two clock values.No overflow checking takes 
              place because the clock values are cyclic.
              * Input:          
              * Output:         
              * Return:        
              * Others:    clock_t time_plus(const clock_t Time1, const clock_t Time2);     
              *************************************************/
unsigned int DRV_OS_TimePlus_MS(unsigned int Time1, unsigned int Time2)
{
    return Time1 + Time2;
}

unsigned int DRV_OS_TimeWait_MS(unsigned int WaitTime)
{
    return DRV_OS_TimePlus_MS(DRV_OS_TimeNow_MS(), WaitTime);
}

#endif

#if 1/*interrupt function*/
/*************************************************
 * Function:       
 * Description:   Locks all interrupts.
 * Input:          
 * Output:         
 * Return:        
 * Others:    void interrupt_lock(void);
 *************************************************/
void DRV_OS_InterruptLock(void)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

/*************************************************
 * Function:       
 * Description:   Unlock all interrupts.
 * Input:          
 * Output:         
 * Return:        
 * Others:  void interrupt_unlock(void);       
 *************************************************/
void DRV_OS_InterruptUnlock(void)
{
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}
#endif

#if 1
void MidIntoa( int x , char *p ,int radix)
{
  char buf[20];
  
  memset(buf, 0, 20);
  sprintf(buf,"%d",x);
  strcpy(p,buf);
  return ;
}

void DRV_OsInfo(void (*Print_p)(char *format,...))
{
    Print_p("TaskCreateNb[%d],TaskInitNb[%d],MaxPriority[%d],FullPartitonNb[%d],PartialPartitonNb[%d],MessageQueueNb[%d],SemaphoreNb[%d],MutexNb[%d],\n",
            mOsDevice.TaskCreateNb,
            mOsDevice.TaskInitNb,
            mOsDevice.MaxPriority,
            mOsDevice.FullPartitionNb,
            mOsDevice.PartialPartitionNb,
            mOsDevice.MessageQueueNb,
            mOsDevice.SemaphoreNb,
            mOsDevice.MutexNb);
}

/*****************************************************************************
  Date&Time       : 2012-09-25
  Author          : wrwei create
  Function Name   : DRV_SysInfo
  Description     : 获取系统运行时间，内存使用情况等系统使用情况
  Input           : cmd:0,系统启动到现在经过的时间  1,系统总的可用的内存大小
                        2,系统还未被使用的内存大小  3,当前进程数目    
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int DRV_SysInfo(int cmd, int *SysInfo)
{
    struct sysinfo s_info;
    struct rusage usage;
    int error;
    int info;

    error = sysinfo(&s_info);
    if(error != 0)
    {
        printf("[DRV]DRV_SysInfo get info error!\n");
        return -1;
    }

    switch(cmd)
    {
        case 0:
            info = (int)s_info.uptime;       //系统启动到现在经过的时间 ,单位s
            break;
        case 1:
            info = (int)s_info.totalram;     //系统总的可用的内存大小 ，单位B
            break;
        case 2:
            info = (int)s_info.sharedram;    //系统还未被使用的内存大小 ，单位B
            break;
        case 3:
            info = (int)s_info.procs;        //当前进程数目    
            break;
        default:
            printf("[DRV]DRV_SysInfo get info cmd error!\n");
    }

    *SysInfo = info;
    
    return 0;
}
/*****************************************************************************
  Date&Time       : 2012-09-25
  Author          : wrwei create
  Function Name   : DRV_GetCpuStatus
  Description     : 获取CPU利用率
  Input           :cmd:0,用户态占用CPU时间百分比      1,nice值为负的进程所占用CPU时间百分比
                       2,系统内核所占用CPU时间百分比  3,空闲时间所占用CPU时间百分比
                       4，从系统启动开始累计到当前时刻的总时间
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int DRV_GetCpuStatus(int cmd, int *CPUInfo)
{
    unsigned int total;
    float   user;
    float   nice;
    float   system;
    float   idle;
    char   cpu[21];
    char   text[201];
    char   strprocess[32];
    int    num=0;
    FILE   *fp;

    if(CPUInfo == NULL)
    {
        printf("[DRV]DRV_GetCpuStatus get info error!\n");
        return -1;
    }
    
    fp=fopen("/proc/stat","r");
    while(fgets(text,200,fp))
    {
        if(strstr(text,"cpu"))
        {
            sscanf(text,"%s %f %f %f %f",cpu,&user,&nice,&system,&idle);         
        }
        if(strstr(text,"procs_running"))
        {
            sscanf(text,"%s %d",strprocess,&num);
        }
    }
    fclose(fp);
    
    total=(long)(user+nice+system+idle);
    user=(user/total)*100;
    nice=(nice/total)*100;
    system=(system/total)*100;
    idle=(idle/total)*100;
    
    switch(cmd)
    {
        case 0:
            CPUInfo = (int)user;       //从系统启动开始累计到当前时刻，用户态占用CPU时间百分比
            break;
        case 1:
            CPUInfo = (int)nice;       //从系统启动开始累计到当前时刻，nice值为负的进程所占用CPU时间百分比
            break;
        case 2:
            CPUInfo = (int)system;    //从系统启动开始累计到当前时刻，系统内核所占用CPU时间百分比
            break;
        case 3:
            CPUInfo = (int)idle;      //从系统启动开始累计到当前时刻，空闲时间所占用CPU时间百分比
            break;
        case 4:
            CPUInfo = (int)total;      //从系统启动开始累计到当前时刻的总时间
            break;
        default:
            printf("[DRV]DRV_GetCpuStatus get info cmd error!\n");
    }
    
    return 0;
}

/*****************************************************************************
  Date&Time       : 2012-09-25
  Author          : wrwei create
  Function Name   : DRV_GetCpuStatus
  Description     : 获取CPU利用率
  Input           :
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 返回CPU负载百分比值
*****************************************************************************/
int DRV_GetCpuOccupy(void)
{
    unsigned int total1,total2;
    unsigned int occupytime1,occupytime2;
    int cpu_use = 0;
    float   user;
    float   nice;
    float   system;
    float   idle;
    char   cpu[21];
    char   text[201];
    FILE   *fp;

    fp = fopen("/proc/stat","r");
    while(fgets(text,200,fp))
    {
        if(strstr(text,"cpu"))
        {
            sscanf(text,"%s %f %f %f %f",cpu,&user,&nice,&system,&idle);         
        }
    }
    fclose(fp);
    total1 = (long)(user+nice+system+idle);        //从系统启动开始累计到当前时刻的总时间
    occupytime1 = (long)(user + system);           //用户态和内核态所占时间

    usleep(300000);

    fp = fopen("/proc/stat","r");
    while(fgets(text,200,fp))
    {
        if(strstr(text,"cpu"))
        {
            sscanf(text,"%s %f %f %f %f",cpu,&user,&nice,&system,&idle);         
        }
    }
    fclose(fp);
    total2 = (long)(user+nice+system+idle);       //从系统启动开始累计到当前时刻的总时间
    occupytime2 = (long)(user + system);          //用户态和内核态所占时间
    
    if((total2 - total1) != 0)
    {
        //printf("===============>>ssss = %d,bbbb = %d\n",occupytime2 - occupytime1,total2 - total1);
        cpu_use = (int)((occupytime2 - occupytime1)*100)/(total2 - total1); 
    }
    else
    {
        cpu_use = 0;
        printf("[DRV]DRV_GetCpuOccupy cpu: %u\n",cpu_use);
}

    return cpu_use;
}
int system_vfork(const char *command)
{
	int wait_val, pid;
	__sighandler_t save_quit, save_int, save_chld;

	if (command == 0)
		return 1;

	save_quit = signal(SIGQUIT, SIG_IGN);
	save_int = signal(SIGINT, SIG_IGN);
	save_chld = signal(SIGCHLD, SIG_DFL);

	if ((pid = vfork()) < 0) {
		signal(SIGQUIT, save_quit);
		signal(SIGINT, save_int);
		signal(SIGCHLD, save_chld);
		return -1;
	}
	if (pid == 0) {
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);

		execl("/bin/sh", "sh", "-c", command, (char *) 0);
		_exit(127);
	}
	/* Signals are not absolutly guarenteed with vfork */
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);

#if 0
	__printf("Waiting for child %d\n", pid);
#endif

	if (wait4(pid, &wait_val, 0, 0) == -1)
		wait_val = -1;

	signal(SIGQUIT, save_quit);
	signal(SIGINT, save_int);
	signal(SIGCHLD, save_chld);
	return wait_val;
}

#endif

