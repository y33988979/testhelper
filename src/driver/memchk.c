/*
malloc提供的钩子函数
使用GLIBC进行内存管理时可以利用malloc提供的钩子函数进行内存分配跟踪，从而发现内存泄露。 

以下代码可以作为一个桩子嵌入应用程序来检查内存泄露： 

将以下代码保存为memchk.c，直接通过include将之引入到被测试的程序中
*/
/* Prototypes for __malloc_hook, __free_hook */
#include <malloc.h>
#include <pthread.h>
#include <stdio.h>
 
#ifndef MEMCHK_LIST_SIZE
#define MEMCHK_LIST_SIZE 1000
#endif
 
typedef struct memstat
{
    unsigned int addr;
    unsigned int size;
    unsigned int caller;
    struct memstat *next;
} memstat_t;
 
memstat_t mem_list[MEMCHK_LIST_SIZE+2];
 
/* Globals */
void *(*old_malloc_hook)(size_t);
void (*old_free_hook)(void *);
memstat_t *listhead_free = &mem_list[0];
memstat_t *listhead_mem = &mem_list[MEMCHK_LIST_SIZE+1];
pthread_mutex_t hook_locker = PTHREAD_MUTEX_INITIALIZER;
 
void dump_malloc();
 
static memstat_t *remove_from_list(memstat_t *head, unsigned int addr)
{
    memstat_t *p1 = head;
    memstat_t *p2 = p1->next;
 
    while (p2 != 0)
    {
    if (p2->addr == addr)
        break;
	p1 = p2;
	p2 = p2->next;
    }
 
    if (p2 != 0) /* found */
    {
        p1->next = p2->next;
    }
 
    return p2;
}
 
static void add_to_list(memstat_t *head, memstat_t *p)
{
    p->next = head->next;
    head->next = p;
}	 
 
static void print_list(memstat_t *head)
{
    memstat_t *p = head;
    int i = 0;
 
    while (p->next != 0)
    {
        printf("No.%d\taddr=0x%x\tsize=%d\tcaller=0x%x\n", i++, p->next->addr, p->next->size, p->next->caller);
        p = p->next;
    }
    printf("\n");
}
 
void dump_malloc()
{
    if (pthread_mutex_lock(&hook_locker))
        return;
 
    print_list(listhead_mem);
 
    pthread_mutex_unlock(&hook_locker);
}
 
/* Prototypes for our hooks.  */
static void my_init_hook (void);
static void *my_malloc_hook (size_t, const void *);
static void my_free_hook (void*, const void *);
 
/* Override initializing hook from the C library. */
void (*__malloc_initialize_hook) (void) = my_init_hook;
 
static void my_init_hook (void)
{
    int i;
 
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
 
    /* first element for free*/
    listhead_free = &mem_list[0];
    /* link free list */
    for (i = 0; i < MEMCHK_LIST_SIZE; i++)
    {
        mem_list[i].addr = 0;			 
 	mem_list[i].size = 0;			
	mem_list[i].caller = 0;		
	mem_list[i].next = &mem_list[i+1];
    }
    mem_list[MEMCHK_LIST_SIZE].addr = 0;
    mem_list[MEMCHK_LIST_SIZE].size = 0;
    mem_list[MEMCHK_LIST_SIZE].caller = 0;
    mem_list[MEMCHK_LIST_SIZE].next = 0;	
 
    /* last element for busy */
    listhead_mem = &mem_list[MEMCHK_LIST_SIZE+1];
    /* clean busy list */
    listhead_mem->addr = 0;
    listhead_mem->size = 0;
    listhead_mem->caller = 0;
    listhead_mem->next = 0;
}
 
static void *my_malloc_hook (size_t size, const void *caller)
{
    void *result;
    memstat_t *p;
 
    if (pthread_mutex_lock(&hook_locker))
        return 0;
 
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    /* Call recursively */
    result = malloc (size);
    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
 
    p = remove_from_list(listhead_free, 0);
    if (p == 0)
    {
        pthread_mutex_unlock(&hook_locker);
        return 0;
    }
    p->addr = (unsigned int)result;
    p->size = size;
    p->caller = (unsigned int)caller;
    p->next = 0;
    add_to_list(listhead_mem, p);
 
    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
 
    pthread_mutex_unlock(&hook_locker);
 
    return result;
}
 
static void my_free_hook (void *ptr, const void *caller)
{
    memstat_t *p;
 
    if (pthread_mutex_lock(&hook_locker))
        return;
 
    /* Restore all old hooks */
    __malloc_hook = old_malloc_hook;
    __free_hook = old_free_hook;
    /* Call recursively */
    free (ptr);
    /* Save underlying hooks */
    old_malloc_hook = __malloc_hook;
    old_free_hook = __free_hook;
 
    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
 
    p = remove_from_list(listhead_mem, (unsigned int)ptr);
    if (p == 0)
    {
        pthread_mutex_unlock(&hook_locker);
        return;
    }
    p->addr = 0;
    p->size = 0;
    p->caller = 0;
    p->next = 0;
    add_to_list(listhead_free, p);
 
    /* Restore our own hooks */
    __malloc_hook = my_malloc_hook;
    __free_hook = my_free_hook;
 
    pthread_mutex_unlock(&hook_locker);	 
}