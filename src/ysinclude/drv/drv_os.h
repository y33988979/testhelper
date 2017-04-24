/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : osdrv.h

    Description :嵌入式操作系统的系统调用函数封装

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-16
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2006-02-07
    作    者：Cao Songtao
    说    明：
    1. 修改由于头文件包含造成的第三方移植重复定义问题
    2. 由于重新构建结构不便于后续版本维护故包含stlie.h文件

*******************************************************************************/

#ifndef _DRV_OS_H_
#define _DRV_OS_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include <semaphore.h> /* for sem_t */
#include <pthread.h> /* for sem_t */
#include <time.h>
#include "drv_typedef.h"
#include "stbtype.h"
#include "drv_memory.h"

#ifndef  DRVOS_PRINT_LEVEL
#define  DRVOS_PRINT_LEVEL
typedef enum DRVOS_PrintLevel
{
	PRINT_LEVEL_PTI_DATA = 1,//打印数据接收过程
	PRINT_LEVEL_MEM_STATUS = 2,//打印内存空间状态
	PRINT_LEVEL_TASK_STATUS = 4,//打印任务状态
	PRINT_LEVEL_ALL =(int)0xffffffff//打印所有调试内容
}DRVOS_PrintLevel_e;
#endif


enum
{
    DRV_OS_ERROR_HANDLE = DRV_OS_BASE + 1,
    DRV_OS_ERROR_MUTEX_CREATE,
    DRV_OS_ERROR_SEMAPHORE_CREATE,
    DRV_OS_ERROR_SEMAPHORE_TIMEOUT
};

typedef struct OS_Device_s
{
    int TaskCreateNb;
    int TaskInitNb;
    int PartialPartitionNb;
    int FullPartitionNb;
    int MessageQueueNb;
    int SemaphoreNb;
    int MutexNb;
    int MaxPriority;
}OS_Device_t;

enum
{
    DRV_OS_TIMEOUT_INFINITY  = (U32_YS)(0xffffffff),
    DRV_OS_TIMEOUT_IMMEDIATE = 0
};

typedef void * DRV_MutexHandle;


#define DRVOS_MSEL  200


/* The following, DRVOS_InitMode_e, isn't supported on Linux */
#if 1//semaphore
  #ifndef DRVOS_SEM_INITMODE
    #define  DRVOS_SEM_INITMODE
    typedef enum
    {
      DRVOS_INIT_FIFO_WAIT = 1,
      DRVOS_INIT_FIFO_TIMEOUT,
      DRVOS_INIT_PRIO_WAIT,
      DRVOS_INIT_PRIO_TIMEOUT
    }DRVOS_InitMode_e;
  #endif

  #ifndef DRVOS_SEM_WAITMODE
    #define  DRVOS_SEM_WAITMODE
    typedef enum
    {
      DRVOS_WAIT_NO = -1,
      DRVOS_WAIT_INFINITY = 0,
      DRVOS_WAIT_FINITY = 1  
    }DRVOS_WaitMode_e;
  #endif
#endif

typedef enum
{
  OS_KILL_TASK_ON_EXIT_FUN = 0,
  OS_KILL_TASK_NO_EXIT_FUN = 1
}DRVOS_TaskKillFlag_e;

#if 1//time
  #ifndef DRVOS_TimeClock_t
    #define  DRVOS_TimeClock_t  U32_YS
  #endif
  
  #ifndef DRVOS_TimeMS_t
    #define  DRVOS_TimeMS_t  U32_YS
  #endif

#endif

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*
 * memory partition is not supported on Linux.
 * Application programs are not supposed to look inside DRVOS_Partition_t.
 */
   typedef struct
  {
    U32_YS PartSize;
    U32_YS PartUsedSize;
    U32_YS PartFreeSize;
    U32_YS PartLargestFreeBlockSize;
  }DRVOS_PartitionStatus_t;

typedef yspartition_t DRVOS_Partition_t;  /*for init function */
typedef DRVOS_Partition_t* DRVOS_Partition_p;  /* memory partition handle */

typedef struct
{
    pthread_t ThreadId;
    U32_YS NeedFree;
    void *Func;
    void *Para;
    int Priority;
    char Name[24];
}DRVOS_Task_t;

typedef pthread_attr_t DRVOS_Tdesc_t;

typedef DRVOS_Task_t* DRVOS_Task_p;  /* task handle */

typedef struct DRVOS_Sem
{
  sem_t sem;
  void(*delet)(void*);
}DRVOS_Sem_t;

  #ifndef DRVOS_TASK_FUN
    #define  DRVOS_TASK_FUN
    typedef void (*DRVOS_Task_Fun)(void *param);
  #endif
  
  #ifndef DRVOS_TASK_ONEXIT_FUN
    #define  DRVOS_TASK_ONEXIT_FUN
    typedef void (*DRVOS_TaskOnexit_Fun)(DRVOS_Task_t*, S32_YS);
  #endif

  #ifndef DRVOS_APP_TASK_STACK_MIN_SIZE
    #define  DRVOS_APP_TASK_STACK_MIN_SIZE  1024
  #endif

typedef DRVOS_Sem_t *DRV_SemHandle;


#define OS_IN_RANGE(v,a,b)  (((v)>=(a)) && ((v)<=(b)))
#define OS_MAX(v,w) (((v)>(w))?(v):(w))
#define OS_MIN(v,w) (((v)>(w))?(w):(v))
#define OS_LONGMAX  2147483647   /*Maximum S32 value*/

void Err_Print(char *Format_p, ...);
void Debug_Print(DRVOS_PrintLevel_e level, char *Format_p, ...);

#if 1/*memory function*/
/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OSMalloc(DRVOS_Partition_p Partition_p, size_t Size);

/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OS_MallocCLR(DRVOS_Partition_p Partition_p, size_t Size);

/*************************************************
 * Function:       
 * Description: Allocate a block of memory from a partition
 * Input:        
 * Output:         
 * Return:        
 * Others:      void* memory_allocate(partition_t *part, size_t size)
 *************************************************/
void *DRV_OSMalloc_NoPrint(DRVOS_Partition_p Partition_p, size_t Size);

/*************************************************
 * Function:       
 * Description:   Reallocate a block of memory from a partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:     void* memory_reallocate(partition_t *part, void* block, size_t size)
 *************************************************/
void *DRV_OS_Realloc(DRVOS_Partition_p Partition_p, void *Block_p, size_t Size);

/*************************************************
 * Function:       
 * Description:   Free a block of memory back to a partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:         void memory_deallocate(partition_t *part, void* block)
 *************************************************/
void DRV_OSFree(DRVOS_Partition_p Partition_p, void *Block_p);

/*************************************************
 * Function:       DRV_OSPartCreate
 * Description:   creat a partition from memory.
 * Input:          
 * Output:         
 * Return:        
 * Others:         
 *************************************************/
DRVOS_Partition_p DRV_OSPartCreate(CHAR_YS *name,void *Memory_p, size_t Size);

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
int DRV_OSPartInit(CHAR_YS *name,DRVOS_Partition_p Partition_p, void *Memory_p, size_t Size);

/*************************************************
 * Function:       DRV_OSPartDel
 * Description:   delete a partition.
 * Input:          
 * Output:         
 * Return:        
 * Others:         
 *************************************************/
DRV_ErrCode DRV_OSPartDel(DRVOS_Partition_p Partition_p);

/*************************************************
* Function:       DRV_OSPartStatus
* Description:   Get status of a partition.
* Input:          StatusMask   选择希望了解信息
* Output:         
* Return:        
* Others:      
*************************************************/
BOOL_YS DRV_OSPartStatus(DRVOS_Partition_t *part,DRVOS_PartitionStatus_t *status);

/*************************************************
* Function:       DRV_OSGetSysPartition
* Description:   Get SystemPartition point.
* Input:          
* Output:         
* Return:        
* Others:    int partition_status(partition_t* , partition_status_t* , partition_status_flags_t )     
*************************************************/
DRVOS_Partition_p DRV_OSGetSysPartition();

/*************************************************
 * Function:       drv_OSDeletFree
 * Description:   free memory.
 * Input:          
 * Output:         
 * Return:        
 * Others:         
 *************************************************/
void drv_OSDeletFree(void *memory);

#endif


#if 1/*task function*/
DRVOS_Task_t *DRV_OS_TaskCreate(char  *Name,
                               void  *Function,
                               int    Priority,
                               size_t StackSize,
                               void  *Param,
                               int    Flags);
int DRV_OSTaskInit(char         *Name,
                   void          *Function,
                   DRVOS_Task_t  *Task,
                   DRVOS_Tdesc_t *Tdesc,
                   int            Priority,
                   void          *Stack,
                   size_t         StackSize,
                   void          *Param,
                   int            Flags);
void DRV_OSTaskExit(int Param);
int DRV_OS_TaskKill(DRVOS_Task_p Task, S32_YS param, DRVOS_TaskKillFlag_e flag);
int DRV_OSTaskDelete(DRVOS_Task_p Task);
void DRV_OS_TaskLock(void);
void DRV_OS_TaskUnlock(void);
int DRV_OSTaskPrioritySet(DRVOS_Task_t *Task_p, S32_YS NewPriority);
void DRV_OS_TaskOnExitSet(DRVOS_TaskOnexit_Fun function);
void DRV_OS_TaskDelay(unsigned int Delay);
void DRV_OS_TaskDelay_MS(unsigned int Delay);
int DRV_OS_TaskWait(DRVOS_Task_t    *Task_p, 
                   DRVOS_WaitMode_e WaitMode, 
                   int              MsToWait);

void DRV_OS_TaskDelay_MS(unsigned int Delay);

#endif


#if 1/*interrupt function*/
void DRV_OS_InterruptLock(void);
void DRV_OS_InterruptUnlock(void);
#endif


#if 1 /*Mutex fuction*/
typedef struct DRVOS_Mutex
{
  pthread_mutex_t mutex;
    int NeedFree;
}DRVOS_Mutex_t;

DRV_ErrCode DRV_OS_MutexCreate(CHAR_YS *name, DRV_MutexHandle *Mutex_p);
DRV_ErrCode DRV_OS_MutexDel(DRV_MutexHandle Mutex_p);
DRV_ErrCode DRV_OS_MutexLock(DRV_MutexHandle Mutex_p);
DRV_ErrCode DRV_OS_MutexRelease(DRV_MutexHandle Mutex_p);
#endif


#if 1    /* Semaphore fuction */

int DRV_OSSemInit(DRVOS_Sem_t *Semaphore, S32_YS InitialTokenCount);
DRVOS_Sem_t* DRV_OS_SemCreate(CHAR_YS *name, S32_YS InitialTokenCount, DRVOS_InitMode_e SemMode);
int DRV_OS_SemWait(DRVOS_Sem_t *Semaphore, DRVOS_WaitMode_e WaitMode, U32_YS MsToWait);
int DRV_OS_SemWait_MS(DRVOS_Sem_t *Semaphore, DRVOS_WaitMode_e WaitMode, U32_YS MsToWait);
void DRV_OS_SemDel(DRVOS_Sem_t *Semaphore);
int DRV_OSSemSignal(DRVOS_Sem_t *Semaphore);
int DRV_OSSemGetVaule(DRVOS_Sem_t *Semaphore,S32 *Vaule);

#endif



#if 1    /* Queue fuction */
typedef struct DRVOS_HDR_s
{
  struct DRVOS_HDR_s* message_hdr_next;
}DRVOS_HDR_t;

typedef struct DRVOS_QueueHalf_s
{
   DRVOS_HDR_t*       message_half_head;
   DRVOS_HDR_t*       message_half_tail;
   DRVOS_Sem_t		message_half_sem;	/* used for sync purpose */
   DRV_MutexHandle	message_half_mutex;	/* guard queue ops */
}DRVOS_QueueHalf_t;

typedef struct DRVOS_MsgQueue_s
{
    char Name[16];
    int MsgNb;
    DRVOS_QueueHalf_t                      message_queue_free;
    DRVOS_QueueHalf_t                      message_queue_queue;
    void*                message_queue_memory;
    void(*delet)(void*);
}DRVOS_MsgQueue_t;

typedef void* DRVOS_MsgQueue_p;  /* message queue handle */

#define DRVOS_MESSAGE_QUEUE_MEMSIZE(maxMessageSize, maxMessages) \
  (((((maxMessageSize) + 3) & ~3) + sizeof(DRVOS_HDR_t)) * (maxMessages))

DRVOS_MsgQueue_p DRV_OS_MsgQueueCreate(char            *Name,
                                       size_t           MessageSize,
                                       unsigned int     Messages,
                                       DRVOS_InitMode_e MsgMode);
int DRV_OSMsgQueueInitManual(char *Name,
                       DRVOS_MsgQueue_t *msgQueue, void *MsgQueueMem,
                       size_t MessageSize, unsigned int Messages, 
                       DRVOS_InitMode_e MsgMode);

DRVOS_MsgQueue_p DRV_OSMsgQueueInit(char *Name,
                       DRVOS_Partition_p Partition,
                       size_t MessageSize, unsigned int Messages, 
                       DRVOS_InitMode_e MsgMode);
void DRV_OSMsgQueueDel(DRVOS_MsgQueue_p Queue);
void* DRV_OS_MsgClaim(DRVOS_MsgQueue_p queue,
                     DRVOS_WaitMode_e WaitMode, 
                     int MsToWait);
void* DRV_OS_MsgClaim_MS(DRVOS_MsgQueue_p queue,
                     DRVOS_WaitMode_e WaitMode, 
                     int MsToWait);
void DRV_OSMsgSend(DRVOS_MsgQueue_p Queue, void *Message);
void DRV_OS_MsgSend_HPP(DRVOS_MsgQueue_p queue, void *Message);
void* DRV_OS_MsgReceive(DRVOS_MsgQueue_p queue,
                       DRVOS_WaitMode_e WaitMode, 
                       int MsToWait);
void* DRV_OS_MsgReceive_MS(DRVOS_MsgQueue_p queue,
                       DRVOS_WaitMode_e WaitMode, 
                       int MsToWait);
void DRV_OSMsgRelease(DRVOS_MsgQueue_p queue, void *Message);
#endif


#if 1    /* Time. */

void drv_OsTimeInit(void);
unsigned int DRV_OS_TimeMs2Ticks(unsigned int Ms);
unsigned int DRV_OS_TimeTicks2Ms(unsigned int Ticks);
U32_YS DRV_OS_TimeGetTickPerSecond(void);
unsigned int DRV_OS_TimeNow(void);// 得到ms时间
unsigned int DRV_OS_TimeNow_MS(void);// 得到ms时间
unsigned int DRV_OS_TimeNow_US(void);// 得到纳秒时间
int DRV_OS_TimeAfter(unsigned int Time1, unsigned int Time2);
int DRV_OS_TimeAfter_MS(unsigned int Time1, unsigned int Time2);

unsigned int DRV_OS_TimeMinus(unsigned int Time1, unsigned int Time2);
unsigned int DRV_OS_TimePlus(unsigned int Time1, unsigned int Time2);
unsigned int DRV_OS_TimeWait(unsigned int WaitTime);
unsigned int DRV_OS_TimeMinus_MS(unsigned int Time1, unsigned int Time2);
unsigned int DRV_OS_TimePlus_MS(unsigned int Time1, unsigned int Time2);
unsigned int DRV_OS_TimeWait_MS(unsigned int WaitTime);


#endif



void MidIntoa(int x, char *p, int radix);
void DRV_OsInfo(void (*Print_p)(char *format,...));
int DRV_SysInfo(int cmd, int *SysInfo);
int DRV_GetCpuStatus(int cmd, int *CPUInfo);
int DRV_GetCpuOccupy(void);
int system_vfork(const char *command);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif



