#ifndef _DRV_MEMORY_H_
#define _DRV_MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <semaphore.h>

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

typedef unsigned int size_t;


typedef struct partition_heap_block_s
{
  struct partition_heap_block_s* partition_heap_block_next;
  size_t                         partition_heap_block_blocksize;
} partition_heap_block_t;

typedef struct
{
  void*                   partition_heap_base;
  size_t                  partition_heap_size;
  int*                    partition_heap_top;
  partition_heap_block_t* partition_heap_free_head;
  partition_heap_block_t* partition_heap_free_tail;
  partition_heap_block_t  partition_heap_dummy;
} partition_heap_t;


typedef struct partition_s
{
    int created;
    struct partition_s*             partition_next;
    sem_t                           partition_lock;
    union {
        partition_heap_t              partition_heap;
    } u;
}yspartition_t;

void partition_initialize(void);

void partition_delete_heap(yspartition_t* Partition);

void *memory_allocate_heap(yspartition_t *p, size_t n); 

void memory_deallocate_heap(yspartition_t *p, void *ptr);

void *memory_reallocate_heap(yspartition_t *heapp, void *ptr, size_t size);

int partition_init_heap(yspartition_t *Partition, void* Base, size_t Size);

void partition_heap_display(yspartition_t *l);

yspartition_t *partition_create_heap(void* Base, size_t Size);

void partition_delete (yspartition_t* Partition);

#ifdef __cplusplus
}
#endif
#endif
