/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_system.c : dhcp sys_dep ( os layer) implement

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_system.h"
#include "drv_os.h"

//#define DHCP_DEBUG printf

#if 1
/**************************** os layer begin *************************/


u32_t sys_new_thread(void (* thread)(void *arg), void *arg, int prio, char *name)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
{
    int ret;
    pthread_t tid;
#if 0
    pthread_attr_t thread_attr;

    int size=0;
    int status=0;

    status = pthread_attr_init(&thread_attr);
    if(status == 0){
        printf(" create attr!\n ");
    }

    status = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if(status == 0)
    {
        printf("set detach\n");
    }

    //#ifdef _POSIX_THREAD_ATTR_STACKSIZE

    status = pthread_attr_getstacksize(&thread_attr, &size);
    if(status == 0)
    {
        printf("get stack size %lx\n", size);
    }

    status = pthread_attr_setstacksize(&thread_attr, PTHREAD_STACK_MIN*1024);
    if(status == 0)
    {
        printf("set stack size\n");
    }

    status = pthread_attr_getstacksize(&thread_attr, &size);
    if(status == 0)
    {
        printf("new stack size is %lx\n", size);
    }
    //#endif
#endif
    ret = pthread_create(&tid, NULL, (void *)thread, arg);
    if(ret != 0){
        DHCP_DEBUG("pthread_create id %d error !\n",tid);
        return 0;
    }
    return (u32_t)tid;
}
#endif


    return 0;
}

void sys_delete_thread(u32_t thread_id)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
{
    /* delete pthread */
}
#endif


    return;
}


DRVOS_MsgQueue_p sys_msgqueue_init(u32_t msg_project_id)
{
    /* return msg queue pointer */
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
{
    key_t mqkey;
    int msg_queue_id;
    char filename[] = "dhcp_linux_file";
    mqkey = ftok(filename, msg_project_id);
    if(mqkey == -1)
    {
        DHCP_DEBUG("ftok error \n");
        exit(-1);
    }
    msg_project_id++; /* next generate a different msg key */

    msg_queue_id = msgget(mqkey, IPC_CREAT/*|IPC_EXCL|0666*/);
    if(msg_queue_id == -1)
    {
        DHCP_DEBUG("msgget error %d %d %d %d %d %d \n", errno,EACCES, EEXIST, ENOENT, ENOMEM, ENOSPC);
        exit(-1);
    }
    return msg_queue_id;
}
#endif

#if defined(__LINUX_SYS__)
{
    return DRV_OS_MsgQueueCreate("dhcp_msg", sizeof(APP_MSG_T), 100, DRVOS_INIT_FIFO_TIMEOUT);
}
#endif


    return;
}

int sys_send_msg(DRVOS_MsgQueue_p msgqueueid, APP_MSG_T *msg, char *msg_queue)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
{
    MSG_BUF_T snd_msg;
    snd_msg.mtype = SNDMSG;
    snd_msg.mtext = msg->type;
    //printf("in sys send msg %ld, %ld\n",msgqueueid, snd_msg.mtext);
    msgsnd(msgqueueid, &snd_msg, sizeof(snd_msg.mtext), 0);
    //printf("out sys send msg\n");
    return TRUE;
}
#endif

#if defined(__LINUX_SYS__)
{
    int ret;
    APP_MSG_T *app_msg = NULL;
    
    app_msg = (APP_MSG_T *)DRV_OS_MsgClaim(msgqueueid, DRVOS_WAIT_NO, 0);
    if(app_msg == NULL){
        DHCP_DEBUG("sys send msg error : claim error \n");
        return -1;
    }
    
    app_msg->sockfd = msg->sockfd;
    app_msg->type = msg->type;
    DRV_OSMsgSend(msgqueueid, (void *)app_msg);
    return 1;
}
#endif


    return;
}

void sys_recv_msg(DRVOS_MsgQueue_p msgqueueid, APP_MSG_T *msg, char *msg_queue)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
{
    MSG_BUF_T rcv_msg;
    //printf("in sys recv msg \n");
    msgrcv(msgqueueid, &rcv_msg, sizeof(rcv_msg.mtext), SNDMSG, 0);
    //printf("out sys recv msg %d\n", rcv_msg.mtext);
    msg->type = rcv_msg.mtext;
    return;
}
#endif

#if defined(__LINUX_SYS__)
{
    APP_MSG_T *rcv_msg = NULL;
    rcv_msg = (APP_MSG_T *) DRV_OS_MsgReceive(msgqueueid,DRVOS_WAIT_INFINITY, 0);
    if(rcv_msg != NULL){
        msg->sockfd = rcv_msg->sockfd;
        msg->type = rcv_msg->type;
        DRV_OSMsgRelease(msgqueueid, rcv_msg);
        return;
    }
    DHCP_DEBUG("sys recv msg error : osmsgreceive error \n");
    return;
}
#endif


    return;
}

void sys_msgqueue_release(DRVOS_MsgQueue_p msgqueueid)
{
    /* release msg queue */
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
{
    msgctl(msgqueueid, IPC_RMID, NULL);
    return;
}
#endif

#if defined(__LINUX_SYS__)
{
    DRV_OSMsgQueueDel(msgqueueid);
    return;
}
#endif


}

DRVOS_Sem_t* sys_sem_init(int init_val)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
    //
    key_t semkey;
    int semid;
    int sem_pro_id=2;
    char filename[] = "dhcp_sem_file";
    semkey = ftok(filename, sem_pro_id);
    if(semkey == -1)
    {
        DHCP_DEBUG("ftok error \n");
        exit(-1);
    }
    //semkey = IPC_PRIVATE;
    semid = semget(semkey, 1, IPC_CREAT/*|0666*/);
    if(semid == -1){
        DHCP_DEBUG("sys_sem_init : semget error \n");
        return -1;
    }else{
        //init sem
        if(semctl(semid, 0, SETVAL, init_val)== -1){
            DHCP_DEBUG("sys_sem_init : semctl error \n");
            return -1;
        }
        return semid;
    }
#endif

#if defined(__LINUX_SYS__)
    return DRV_OS_SemCreate(NULL,init_val,DRVOS_INIT_FIFO_TIMEOUT);
#endif

}

int sys_sem_wait(DRVOS_Sem_t* semid)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
    //
{
    int ret;
    struct sembuf sops={0,-1,SEM_UNDO};

    ret = semop(semid, &sops, 1);
    if(ret == -1){
        DHCP_DEBUG("sys_sem_wait : semop wait error %d E2BIG=%d, EACCES =%d, EAGAIN=%d, EFAULT=%d, EFBIG=%d, EIDRM=%d, EINTR=%d, EINVAL=%d, ERANGE=%d \n", errno, E2BIG,EACCES,EAGAIN, EFAULT, EFBIG, EIDRM, EINTR, EINVAL, ERANGE);
        return -1;
    }else{
        return 0;
    }
}
#endif

#if defined(__LINUX_SYS__)
{
    int ret;
    ret = DRV_OS_SemWait(semid,DRVOS_WAIT_NO, 0);
    return ret;
}
#endif

}

int sys_sem_signal(DRVOS_Sem_t *semid)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
    //
{
    int ret;
    struct sembuf sops={0, 1,SEM_UNDO};

    ret = semop(semid, &sops, 1);
    if(ret == -1){
        DHCP_DEBUG("sys_sem_signal : semop signal error \n");
        return -1;
    }else{
        return 0;
    }
}
#endif

#if defined(__LINUX_SYS__)
{
    int ret;
    ret = DRV_OSSemSignal(semid);
    return ret;
}
#endif

}

int sys_sem_release(DRVOS_Sem_t * semid)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__CUT)
{
    //
    int ret;
    
    ret = semctl(semid, 0 , IPC_RMID, 0);
    if(ret == -1){
        DHCP_DEBUG("sys_sem_release : semctl -remove error \n");
        return -1;
    }else{
        return 0;
    }
}
#endif

#if defined(__LINUX_SYS__)
{
    //
    int ret;
    DRV_OS_SemDel(semid);
    return 0;
}
#endif

}

//lock
int sys_mutex_init(pthread_mutex_t *mutex)
{
    return pthread_mutex_init(mutex, NULL);
}

int sys_mutex_lock(pthread_mutex_t *mutex)
{
    return pthread_mutex_lock(mutex);
}

int sys_mutex_unlock(pthread_mutex_t *mutex)
{
    return pthread_mutex_unlock(mutex);
}

void sys_delay(int times)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
    usleep(times*1000);
#endif

    
}

void *dhcp_mem_malloc(u32_t size)
{
    void *pmem;
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
    
    pmem = (void *)malloc(size);
    //printf("*** has malloced %lp %d\n", pmem, size);
    return pmem;
#endif


}

void dhcp_mem_free(void *pmem)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
    //printf("*** will free %lp \n", pmem);
    free((void *)pmem);
#endif


}

void *dhcp_mem_realloc(void *memptr, u32_t size)
{
#if defined(__WIN32__)
#endif

#if defined(__LINUX_SYS__)
   realloc(memptr, size);
#endif


}

/**************************** os layer end ***************************/
#endif

