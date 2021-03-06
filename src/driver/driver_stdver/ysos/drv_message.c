/*
 * Message queue functions
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "drv_os.h"
#include "drv_basic.h"


#define message_malloc_mem malloc
#define message_free_mem free

#define CURRENT_MODULE DRV_MODULE_OS

static void drv_message_delete_process(void *memory)
{
    message_free_mem(memory);
}

static void drv_message_half_add(DRVOS_QueueHalf_t* MessageHalf, void* UsrMessage)
{
    DRVOS_HDR_t* Message;

    if (!UsrMessage) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_half_add: NULL UsrMessage\n");
        return;
    }

    Message = (DRVOS_HDR_t*)UsrMessage - 1;

    DRV_OS_MutexLock(MessageHalf->message_half_mutex);
 
    Message->message_hdr_next = NULL;
    if (MessageHalf->message_half_head == NULL)
        MessageHalf->message_half_head = Message;
    else
        MessageHalf->message_half_tail->message_hdr_next = Message;
    MessageHalf->message_half_tail = Message;

    DRV_OS_MutexRelease(MessageHalf->message_half_mutex);

    DRV_OSSemSignal(&MessageHalf->message_half_sem);

    return;
}

static void drv_message_half_add_HPP(DRVOS_QueueHalf_t* MessageHalf, void* UsrMessage)
{
    DRVOS_HDR_t* Message;


    if (!UsrMessage) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_half_add_HPP: NULL UsrMessage\n");
        return;
    }

    Message = (DRVOS_HDR_t*)UsrMessage - 1;

    DRV_OS_MutexLock(MessageHalf->message_half_mutex);

    Message->message_hdr_next = MessageHalf->message_half_head;
    if (MessageHalf->message_half_head == NULL)
        MessageHalf->message_half_tail = Message;
    MessageHalf->message_half_head = Message;

    DRV_OS_MutexRelease(MessageHalf->message_half_mutex);

    DRV_OSSemSignal(&MessageHalf->message_half_sem);

    return;
}

static void* drv_message_half_remove(DRVOS_QueueHalf_t* MessageHalf)
{
    DRVOS_HDR_t* Message;

    if(DRV_OS_SemWait(&MessageHalf->message_half_sem,DRVOS_WAIT_INFINITY,0) != 0)
        return NULL;

    DRV_OS_MutexLock(MessageHalf->message_half_mutex);

    Message                        = MessageHalf->message_half_head;
    MessageHalf->message_half_head = Message->message_hdr_next;

    DRV_OS_MutexRelease(MessageHalf->message_half_mutex);

    return (void*)(Message + 1);
}

static void* drv_message_half_remove_timeout(DRVOS_QueueHalf_t* MessageHalf, unsigned int time_out)
{
    DRVOS_HDR_t* Message;

    if(DRV_OS_SemWait(&MessageHalf->message_half_sem,DRVOS_WAIT_FINITY, time_out) != 0)
        return NULL;

    DRV_OS_MutexLock(MessageHalf->message_half_mutex);

    Message                        = MessageHalf->message_half_head;
    MessageHalf->message_half_head = Message->message_hdr_next;

    DRV_OS_MutexRelease(MessageHalf->message_half_mutex);

    return (void*)(Message + 1);
}

static void* drv_message_claim (DRVOS_MsgQueue_t* MessageQueue)
{
    /* other assertions defered to drv_message_half_remove */
    return drv_message_half_remove(&MessageQueue->message_queue_free);
}
static void* drv_message_claim_timeout (DRVOS_MsgQueue_t* MessageQueue, unsigned int time_out)
{
    /* other assertions defered to drv_message_half_remove_timeout */
    return drv_message_half_remove_timeout(&MessageQueue->message_queue_free, time_out);
}

static void drv_message_send (DRVOS_MsgQueue_t*  MessageQueue, void* Message)
{
    /* other assertions defered to drv_message_half_add */
    drv_message_half_add(&MessageQueue->message_queue_queue, Message);
    return;
}
static void drv_message_send_HPP (DRVOS_MsgQueue_t*  MessageQueue, void* Message)
{
    /* other assertions defered to drv_message_half_add */
    drv_message_half_add_HPP(&MessageQueue->message_queue_queue, Message);
    return;
}

static void* drv_message_receive (DRVOS_MsgQueue_t* MessageQueue)
{
    /* other assertions defered to drv_message_half_remove */
    return drv_message_half_remove(&MessageQueue->message_queue_queue);
}

static void* drv_message_receive_timeout (DRVOS_MsgQueue_t* MessageQueue, unsigned int time_out)
{
    /* other assertions defered to drv_message_half_remove_timeout */
    return drv_message_half_remove_timeout(&MessageQueue->message_queue_queue, time_out);
}

static void drv_message_release (DRVOS_MsgQueue_t* MessageQueue, void* Message)
{
    /* other assertions defered to drv_message_half_add */
    drv_message_half_add(&MessageQueue->message_queue_free, Message);
}

static int drv_message_init_generic(DRVOS_MsgQueue_t* MessageQueue, void* memory,
                                unsigned int ElementSize, unsigned int NoElements,
                                int (*SemInit_f)(DRVOS_Sem_t*, int))
{
    DRVOS_HDR_t**   PrevMsgP;
    DRVOS_HDR_t*    CurrMsg;
    int               i;
    unsigned int            Needed;

    if (!MessageQueue) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_init_generic: NULL MessageQueue\n");
        return -1;
    }

    if (!memory) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_init_generic: NULL memory\n");
        return -1;
    }
  
    ElementSize = (ElementSize + 3) & ~3;
    Needed = ElementSize + sizeof(DRVOS_HDR_t);

    CurrMsg = (DRVOS_HDR_t*)memory;  
    PrevMsgP = &MessageQueue->message_queue_free.message_half_head;

    for (i = 1; i < NoElements; i++) 
    {
        *PrevMsgP = CurrMsg;
        PrevMsgP = &CurrMsg->message_hdr_next;
        CurrMsg = (DRVOS_HDR_t*)((char*)CurrMsg + Needed);
    }
    *PrevMsgP = CurrMsg;
    MessageQueue->message_queue_free.message_half_tail = CurrMsg;
    CurrMsg->message_hdr_next = NULL;

    MessageQueue->message_queue_queue.message_half_head = NULL;
    MessageQueue->message_queue_queue.message_half_tail = NULL;

    SemInit_f(&MessageQueue->message_queue_free.message_half_sem, NoElements);
    SemInit_f(&MessageQueue->message_queue_queue.message_half_sem, 0);

    DRV_OS_MutexCreate(NULL,&MessageQueue->message_queue_free.message_half_mutex);
    DRV_OS_MutexCreate(NULL,&MessageQueue->message_queue_queue.message_half_mutex);

    MessageQueue->message_queue_memory = memory;

    MessageQueue->delet = NULL;
  
    return 0;
}

static int drv_message_init_queue(DRVOS_MsgQueue_t* MessageQueue, void* memory,
                              unsigned int ElementSize, unsigned int NoElements)
{
    /* assertions defered to drv_message_init_queue */
    return drv_message_init_generic(MessageQueue, memory, ElementSize, NoElements,
                                DRV_OSSemInit);
}
static int drv_message_init_queue_timeout(DRVOS_MsgQueue_t* MessageQueue, void* memory,
                                      unsigned int ElementSize, unsigned int NoElements)
{
    /* assertions defered to drv_message_init_generic */
    return drv_message_init_generic(MessageQueue, memory, ElementSize, NoElements,
                                DRV_OSSemInit);
}

static DRVOS_MsgQueue_t* drv_message_create_generic (unsigned int ElementSize,
                                                 unsigned int NoElements, 
                                                 int (*SemInit_f)(DRVOS_Sem_t*, int))
{
    DRVOS_MsgQueue_t* MessageQueue;
    void* messages;
    unsigned int Needed;

    /* This calculation needs to be the same as the one performed in
     * message_queue_init() */
    ElementSize = (ElementSize + 3) & ~3;
    Needed = ElementSize + sizeof(DRVOS_HDR_t);

    messages = message_malloc_mem(Needed * NoElements);
    if (messages == NULL)
        return NULL;

    MessageQueue = message_malloc_mem(sizeof(DRVOS_MsgQueue_t));
    if (MessageQueue == NULL) 
    {
        message_free_mem(messages);
        messages = NULL;
    } 
    else 
    {
        if(drv_message_init_generic(MessageQueue, messages, ElementSize, NoElements,
                                SemInit_f) == 0)
        {
            MessageQueue->delet = drv_message_delete_process;  
        }
        else
        {
            message_free_mem(MessageQueue);
            MessageQueue = NULL;
            message_free_mem(messages);
            messages = NULL;      
        }
    }
 
    return MessageQueue;
}

static DRVOS_MsgQueue_t* drv_message_create_queue(unsigned int ElementSize,
                                              unsigned int NoElements)
{
    /* assertions defered to drv_message_create_generic */
    return drv_message_create_generic(ElementSize, NoElements, 
                                  DRV_OSSemInit);
}

static DRVOS_MsgQueue_t* drv_message_create_queue_timeout(unsigned int ElementSize,
                                                      unsigned int NoElements)
{
    /* assertions defered to drv_message_create_generic */
    return drv_message_create_generic(ElementSize, NoElements,
                                  DRV_OSSemInit);
}


static void drv_message_uninit_queue(DRVOS_MsgQueue_t* MessageQueue)
{

    if (!MessageQueue) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_uninit_queue: NULL MessageQueue\n");
        return;
    }

    DRV_OS_SemDel(&MessageQueue->message_queue_free.message_half_sem);

    DRV_OS_SemDel(&MessageQueue->message_queue_queue.message_half_sem);

    DRV_OS_MutexDel(MessageQueue->message_queue_free.message_half_mutex);

    DRV_OS_MutexDel(MessageQueue->message_queue_queue.message_half_mutex);

    return;
}

static void drv_message_delete_queue (DRVOS_MsgQueue_t* MessageQueue)
{/*????not think that deleted process when the thread locking on the queue*/
    if (!MessageQueue) 
    {
        Err_Print("[DRV:S] FATAL! drv_message_delete_queue: NULL MessageQueue\n");
        return;
    }

    drv_message_uninit_queue(MessageQueue);
  
    if(MessageQueue->delet == drv_message_delete_process)
    {
        MessageQueue->delet(MessageQueue->message_queue_memory);
        MessageQueue->delet(MessageQueue);
    }

    return;
}
extern OS_Device_t mOsDevice;


#if 1
/*************************************************
 * Function:       
 * Description:  Initialize a fixed size message queue. Need the user 
              is responsible for freeing any memory which was allocated for the queue.
              * Input:          
              * Output:         
              * Return: 
              * Others:         void message_init_queue(
                                               message_queue_t* MessageQueue,
                                               void* memory,
                                               size_t MaxMessageSize,
                                               unsigned int Messages);
                      void message_init_queue_timeout(
                                               message_queue_t* MessageQueue,
                                               void* memory,
                                               size_t MaxMessageSize,
                                               unsigned int Messages);
*************************************************/
DRVOS_MsgQueue_p DRV_OSMsgQueueInit(char *Name,
                       DRVOS_Partition_p Partition,
                       size_t MessageSize, unsigned int Messages, 
                       DRVOS_InitMode_e MsgMode)
{
    void *msgQueueMem = NULL;
    DRVOS_MsgQueue_t *msgQueue = NULL;

    msgQueue = (DRVOS_MsgQueue_t *)DRV_OSMalloc(Partition, 
                                                sizeof(*msgQueue) + DRVOS_MESSAGE_QUEUE_MEMSIZE(MessageSize, Messages));

    if(msgQueue != NULL)
    {
        msgQueueMem = (void *)((char *)msgQueue + sizeof(*msgQueue));

        switch(MsgMode)
        {
            case DRVOS_INIT_FIFO_WAIT:
                drv_message_init_queue(msgQueue, msgQueueMem,
                                   MessageSize, Messages);
                break;
            case DRVOS_INIT_FIFO_TIMEOUT:
                drv_message_init_queue_timeout(msgQueue, msgQueueMem,
                                           MessageSize, Messages);
                break;
            default:
                msgQueueMem = NULL;
                DRV_OSFree(Partition, msgQueue);
                msgQueue = NULL;
                break;
        }
    }

    if(msgQueue == NULL)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        strncpy(msgQueue->Name,Name,15);
        msgQueue->MsgNb = Messages;
        mOsDevice.MessageQueueNb++;
        YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    }

    return (DRVOS_MsgQueue_p)msgQueue;
}
int DRV_OSMsgQueueInitManual(char *Name,
                       DRVOS_MsgQueue_t *msgQueue, void *MsgQueueMem,
                       size_t MessageSize, unsigned int Messages, 
                       DRVOS_InitMode_e MsgMode)
{
    int result = DRV_NO_ERROR;
  
    if(msgQueue != NULL && MsgQueueMem != NULL)
    {
        switch(MsgMode)
        {
            case DRVOS_INIT_FIFO_WAIT:
                drv_message_init_queue(msgQueue, MsgQueueMem,
                                   MessageSize, Messages);
                break;
            case DRVOS_INIT_FIFO_TIMEOUT:
                drv_message_init_queue_timeout(msgQueue, MsgQueueMem,
                                           MessageSize, Messages);
                break;
            default:
                result = DRV_ERROR_BAD_PARAMETER;
                break;
        }
    }


    if(result != DRV_NO_ERROR)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        strncpy(msgQueue->Name,Name,15);
        msgQueue->MsgNb = Messages;
        mOsDevice.MessageQueueNb++;
        YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    }

    return result;
}

DRVOS_MsgQueue_p DRV_OS_MsgQueueCreate(char            *Name,
                                       size_t           MessageSize,
                                       unsigned int     Messages,
                                       DRVOS_InitMode_e MsgMode)
{
    DRVOS_MsgQueue_t *msgQueue = NULL;

    switch(MsgMode)
    {
        case DRVOS_INIT_FIFO_WAIT:
            msgQueue = drv_message_create_queue(MessageSize, Messages);
            break;
        case DRVOS_INIT_FIFO_TIMEOUT:
            msgQueue = drv_message_create_queue_timeout(MessageSize,
                                                    Messages);
            break;
        default:
            msgQueue = NULL;
            break;
    }

    if(msgQueue == NULL)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
    }
    else
    {
        strncpy(msgQueue->Name,Name,15);
        msgQueue->MsgNb = Messages;
        mOsDevice.MessageQueueNb++;
        YSDRIVER_ERRORCODE(DRV_NO_ERROR);
    }

    return msgQueue;
}

void DRV_OSMsgQueueDel(DRVOS_MsgQueue_p Queue)
{
    if(mOsDevice.MessageQueueNb == 0)
        YSDRIVER_ERROR(DRV_ERROR_BAD_PARAMETER);
    drv_message_delete_queue(Queue);
    mOsDevice.MessageQueueNb--;
    YSDRIVER_ERRORCODE(DRV_NO_ERROR);
}

void* DRV_OS_MsgClaim(DRVOS_MsgQueue_p queue,
                     DRVOS_WaitMode_e WaitMode, 
                     int MsToWait)
{
    DRVOS_MsgQueue_t *msgQueue = (DRVOS_MsgQueue_t*)queue;
    void *Message=NULL;
  
    if (queue == NULL )
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {    switch (WaitMode)
        {
            case DRVOS_WAIT_INFINITY:
                Message = drv_message_claim(queue);
                break;
            case DRVOS_WAIT_NO:
                Message = drv_message_claim_timeout(queue, 0);
                break;
            case DRVOS_WAIT_FINITY:
                Message = drv_message_claim_timeout(queue, MsToWait);
                break;
            default:
                Message = NULL;
                break;
        }

        if (NULL == Message)
        {
            OS_ERROR(("DRV_OS_MsgClaim error:Msg{Name[%s],MsgNb[%d]} full:!\n",msgQueue->Name,msgQueue->MsgNb));
        }
    }
    return Message;
}
void* DRV_OS_MsgClaim_MS(DRVOS_MsgQueue_p queue,
                     DRVOS_WaitMode_e WaitMode, 
                     int MsToWait)
{
    DRVOS_MsgQueue_t *msgQueue = (DRVOS_MsgQueue_t*)queue;
    void *Message=NULL;
  
    if (queue == NULL )
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {    switch (WaitMode)
        {
            case DRVOS_WAIT_INFINITY:
                Message = drv_message_claim(queue);
                break;
            case DRVOS_WAIT_NO:
                Message = drv_message_claim_timeout(queue, 0);
                break;
            case DRVOS_WAIT_FINITY:
                Message = drv_message_claim_timeout(queue, MsToWait);
                break;
            default:
                Message = NULL;
                break;
        }

        if (NULL == Message)
        {
            OS_ERROR(("DRV_OS_MsgClaim error:Msg{Name[%s],MsgNb[%d]} full:!\n",msgQueue->Name,msgQueue->MsgNb));
        }
    }
    return Message;
}

void DRV_OSMsgSend(DRVOS_MsgQueue_p Queue, void *Message)
{
    if (Queue == NULL || NULL == Message)
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {
        drv_message_send(Queue, Message);
    }
}

void DRV_OS_MsgSend_HPP(DRVOS_MsgQueue_p queue, void *Message)
{
    if (queue == NULL || NULL == Message)
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {
        drv_message_send_HPP(queue, Message);
    }
}

void* DRV_OS_MsgReceive(DRVOS_MsgQueue_p queue,
                       DRVOS_WaitMode_e WaitMode, 
                       int MsToWait)
{
    void *Message=NULL;
  
    if (queue == NULL )
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {
        switch (WaitMode)
        {
            case DRVOS_WAIT_INFINITY:
                Message = drv_message_receive(queue);
                break;
            case DRVOS_WAIT_NO:
                Message = drv_message_receive_timeout(queue, 0);
                break;
            case DRVOS_WAIT_FINITY:
                Message = drv_message_receive_timeout(queue, MsToWait);
                break;
            default:
                Message = NULL;
                break;
        }
    }
    return Message;
}

void* DRV_OS_MsgReceive_MS(DRVOS_MsgQueue_p queue,
                       DRVOS_WaitMode_e WaitMode, 
                       int MsToWait)
{
    void *Message=NULL;
  
    if (queue == NULL )
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {
        switch (WaitMode)
        {
            case DRVOS_WAIT_INFINITY:
                Message = drv_message_receive(queue);
                break;
            case DRVOS_WAIT_NO:
                Message = drv_message_receive_timeout(queue,0);
                break;
            case DRVOS_WAIT_FINITY:
                Message = drv_message_receive_timeout(queue, MsToWait);
                break;
            default:
                Message = NULL;
                break;
        }
    }
    return Message;
}

void DRV_OSMsgRelease(DRVOS_MsgQueue_p queue, void *Message)
{
    if (queue == NULL || NULL == Message)
    {
        OS_ERROR(("Bad Parameter\n"));
    }
    else
    {
        drv_message_release(queue, Message);
    }
}

#endif


