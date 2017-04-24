/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_system.h : dhcp sys_dep ( os layer) head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _DHCP_SYSTEM_H_
#define _DHCP_SYSTEM_H_

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_client.h"

#include "drv_os.h"

#define SNDMSG  1
#define RCVMSG  2

typedef struct msg_buf
{
    u32_t mtype; /* msg type must be >0 */
    int mtext; /* msg data */
}MSG_BUF_T;


u32_t sys_new_thread(void (* thread)(void *arg), void *arg, int prio, char *name);
void sys_delete_thread(u32_t thread_id);
//void sys_msgqueue_init();
//int sys_msgqueue_init(u32_t msg_project_id);
DRVOS_MsgQueue_p sys_msgqueue_init(u32_t msg_project_id);
//int sys_send_msg(APP_MSG_T *msg, char *msg_queue);
//int sys_send_msg(int msgqueueid, APP_MSG_T *msg, char *msg_queue);
int sys_send_msg(DRVOS_MsgQueue_p msgqueueid, APP_MSG_T *msg, char *msg_queue);
//void sys_recv_msg(APP_MSG_T *msg, char *msg_queue);
//void sys_recv_msg(int msgqueueid, APP_MSG_T *msg, char *msg_queue);
void sys_recv_msg(DRVOS_MsgQueue_p msgqueueid, APP_MSG_T *msg, char *msg_queue);
//void sys_msgqueue_release();
//void sys_msgqueue_release(int msgqueueid);
void sys_msgqueue_release(DRVOS_MsgQueue_p msgqueueid);
//int sys_sem_init(int init_val);
DRVOS_Sem_t* sys_sem_init(int init_val);
//int sys_sem_wait(int semid);
int sys_sem_wait(DRVOS_Sem_t* semid);
//int sys_sem_signal(int semid);
int sys_sem_signal(DRVOS_Sem_t *semid);
//int sys_sem_release(int semid);
int sys_sem_release(DRVOS_Sem_t * semid);
void sys_delay(int times);
void *dhcp_mem_malloc(u32_t size);
void dhcp_mem_free(void *pmem);
void *dhcp_mem_realloc(void *memptr, u32_t size);

#endif /* _DHCP_SYSTEM_H_ */

