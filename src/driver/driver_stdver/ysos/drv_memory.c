#include <drv_memory.h>

#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "drv_os.h"


#define blockheader_t partition_heap_block_t
#define __next partition_heap_block_next
#define __blocksize partition_heap_block_blocksize

#define partition_base        u.partition_heap.partition_heap_base
#define partition_size        u.partition_heap.partition_heap_size
#define partition_top         u.partition_heap.partition_heap_top
#define partition_free_head   u.partition_heap.partition_heap_free_head
#define partition_free_tail   u.partition_heap.partition_heap_free_tail
#define partition_dummy       u.partition_heap.partition_heap_dummy
#define invalid_pointer(heap, userp, blockp) \
  ((int)(userp) % sizeof(int) != 0 ||  \
   (int *)blockp >= (heap)->partition_top ||  \
   (blockp)->__next != (blockheader_t *)CHECK || \
   ((blockp)->__blocksize & ~(sizeof(blockheader_t) - 1)) != (blockp)->__blocksize)
#define CHECK 0x80000001

yspartition_t*       partition_list = NULL;
sem_t partition_list_semaphore;
sem_t *m_pSemPartition = &partition_list_semaphore;


void partition_initialize(void)
{
    int status;

    status = sem_init(m_pSemPartition, 0, 1);
    if (status == -1) {
        printf("DRV_OsSemaphoreCreate: Error, sem_init failure.\n");
        return ;
    }

#if 0
    sm_create("PLS", 1, (SM_GLOBAL | SM_FIFO | SM_UNBOUNDED), 
                      &partition_list_semaphore);
#endif
}

void task_align(void** Stack, size_t *StackSize)
{
  int ptr = (int)*Stack;
  int size = (int)*StackSize;
  int adjust;

  if ((ptr & 3) != 0) {
    adjust = 4 - (ptr & 3);
    ptr += adjust;
    size -= adjust;
  }

  size &= (~3);

  *Stack = (void*)ptr;
  *StackSize = (size_t)size;
}


void partition_remove(yspartition_t *Partition)
{
  yspartition_t *prev;  
  
    sem_wait(m_pSemPartition);
 

  if (partition_list == Partition) {
    partition_list = Partition->partition_next;
  } else {
    for (prev = partition_list; prev != NULL; prev = prev->partition_next) {
      if (prev->partition_next == Partition) {
        prev->partition_next = Partition->partition_next;
        break;
      }
    }
  }

    sem_post(m_pSemPartition);
}

void partition_delete_heap(yspartition_t* Partition)
{
  sem_wait(&(Partition->partition_lock));    

  partition_remove(Partition);
  sem_destroy(&(Partition->partition_lock));
}

void *memory_allocate_heap(yspartition_t *p, size_t n) 
{
  blockheader_t *fptr, *prev_fptr, *return_ptr;
  int found = 0;

  if ((int) n  <= 0)
      return (void *)NULL;

  
  if (sem_wait(&(p->partition_lock)) != 0)
    return (void *)NULL;
     
  n += sizeof(blockheader_t);
  n = ((n + sizeof(blockheader_t) - 1) & ~(sizeof(blockheader_t) - 1));

  prev_fptr = ( blockheader_t *) p->partition_free_head;
  fptr = ((blockheader_t *) p->partition_free_head)->__next;
  while (!found)
  {
    if (fptr != (blockheader_t *) p->partition_free_head)
    {
      if (fptr->__blocksize >= n)  
      {
        if (fptr->__blocksize - n >= 2 * sizeof(blockheader_t))
        {
          fptr->__blocksize -= n;
          fptr = (blockheader_t *)((char *)fptr + fptr->__blocksize);
          fptr->__blocksize = n;
        }
        else  
          prev_fptr->__next = fptr->__next;

        fptr->__next = (blockheader_t *)CHECK;
        if (fptr == p->partition_free_tail)
          p->partition_free_tail = prev_fptr;
        return_ptr = fptr + 1; 
        found = 1;
      }

      prev_fptr = fptr;
      fptr = fptr->__next;
    }
    else
    {
        partition_heap_display(p);
        sem_post(&(p->partition_lock));
        return (void *)NULL;
    }
  }
  sem_post(&(p->partition_lock));

  return (void *)return_ptr;
}
void *memory_allocate_heap_nosem(yspartition_t *p, size_t n) 
{
  blockheader_t *fptr, *prev_fptr, *return_ptr;
  int found = 0;

  if ((int) n  <= 0)
      return (void *)NULL;
     
  n += sizeof(blockheader_t);
  n = ((n + sizeof(blockheader_t) - 1) & ~(sizeof(blockheader_t) - 1));

  prev_fptr = ( blockheader_t *) p->partition_free_head;
  fptr = ((blockheader_t *) p->partition_free_head)->__next;
  while (!found)
  {
    if (fptr != (blockheader_t *) p->partition_free_head)
    {
      if (fptr->__blocksize >= n)  
      {
        if (fptr->__blocksize - n >= 2 * sizeof(blockheader_t))
        {
          fptr->__blocksize -= n;
          fptr = (blockheader_t *)((char *)fptr + fptr->__blocksize);
          fptr->__blocksize = n;
        }
        else  
          prev_fptr->__next = fptr->__next;

        fptr->__next = (blockheader_t *)CHECK;
        if (fptr == p->partition_free_tail)
          p->partition_free_tail = prev_fptr;
        return_ptr = fptr + 1; 
        found = 1;
      }

      prev_fptr = fptr;
      fptr = fptr->__next;
    }
    else
    {
        partition_heap_display(p);
        return (void *)NULL;
    }
  }

  return (void *)return_ptr;
}

void *memory_allocate_heap_noprint(yspartition_t *p, size_t n) 
{
  blockheader_t *fptr, *prev_fptr, *return_ptr;
  int found = 0;

  if ((int) n  <= 0)
      return (void *)NULL;

  
  if (sem_wait(&(p->partition_lock)) != 0)
    return (void *)NULL;
     
  n += sizeof(blockheader_t);
  n = ((n + sizeof(blockheader_t) - 1) & ~(sizeof(blockheader_t) - 1));

  prev_fptr = ( blockheader_t *) p->partition_free_head;
  fptr = ((blockheader_t *) p->partition_free_head)->__next;
  while (!found)
  {
    if (fptr != (blockheader_t *) p->partition_free_head)
    {
      if (fptr->__blocksize >= n)  
      {
        if (fptr->__blocksize - n >= 2 * sizeof(blockheader_t))
        {
          fptr->__blocksize -= n;
          fptr = (blockheader_t *)((char *)fptr + fptr->__blocksize);
          fptr->__blocksize = n;
        }
        else  
          prev_fptr->__next = fptr->__next;

        fptr->__next = (blockheader_t *)CHECK;
        if (fptr == p->partition_free_tail)
          p->partition_free_tail = prev_fptr;
        return_ptr = fptr + 1; 
        found = 1;
      }

      prev_fptr = fptr;
      fptr = fptr->__next;
    }
    else
    {
        sem_post(&(p->partition_lock));
        return (void *)NULL;
    }
  }
  sem_post(&(p->partition_lock));

  return (void *)return_ptr;
}

void memory_deallocate_heap(yspartition_t *p, void *ptr)
{
  blockheader_t *bptr, *sptr, *prev;

    if (ptr == NULL)
      {
    return;
      }
    
  if (sem_wait(&(p->partition_lock)) != 0)
    return;

  bptr = (blockheader_t *)ptr - 1;


  if (invalid_pointer(p, ptr, bptr))
  {
      sem_post(&(p->partition_lock));
      return;
  }
  (bptr)->__next = NULL;

  prev = p->partition_free_head;
  sptr = p->partition_free_head;
  while (sptr != p->partition_free_tail && bptr < sptr->__next)
  {
    prev = sptr;
    sptr = sptr->__next;
  }

  if (sptr == p->partition_free_tail)
    p->partition_free_tail = bptr;


  if (sptr->__next != p->partition_free_head && 
      (char *)sptr->__next + sptr->__next->__blocksize == (char *)bptr)
  {
    sptr->__next->__blocksize += bptr->__blocksize;
    bptr = sptr->__next;
  }
  else
  {
    bptr->__next = sptr->__next;
  }


  if (sptr != p->partition_free_head && 
      (char *)bptr + bptr->__blocksize == (char *)sptr)
  {
    bptr->__blocksize += sptr->__blocksize;
    prev->__next = bptr;
  }
  else
  {
    sptr->__next = bptr;
  }
  sem_post(&(p->partition_lock));

}
void memory_deallocate_heap_nosem(yspartition_t *p, void *ptr)
{
  blockheader_t *bptr, *sptr, *prev;

    if (ptr == NULL)
      {
    return;
      }
    
  bptr = (blockheader_t *)ptr - 1;


  if (invalid_pointer(p, ptr, bptr))
    {
      return;
    }
  (bptr)->__next = NULL;

  prev = p->partition_free_head;
  sptr = p->partition_free_head;
  while (sptr != p->partition_free_tail && bptr < sptr->__next)
  {
    prev = sptr;
    sptr = sptr->__next;
  }

  if (sptr == p->partition_free_tail)
    p->partition_free_tail = bptr;


  if (sptr->__next != p->partition_free_head && 
      (char *)sptr->__next + sptr->__next->__blocksize == (char *)bptr)
  {
    sptr->__next->__blocksize += bptr->__blocksize;
    bptr = sptr->__next;
  }
  else
  {
    bptr->__next = sptr->__next;
  }


  if (sptr != p->partition_free_head && 
      (char *)bptr + bptr->__blocksize == (char *)sptr)
  {
    bptr->__blocksize += sptr->__blocksize;
    prev->__next = bptr;
  }
  else
  {
    sptr->__next = bptr;
  }

}


static void add_to_freelist(yspartition_t *heapp, blockheader_t *fptr, size_t n)
{
  fptr->__blocksize = n;
  fptr->__next = (blockheader_t *)CHECK;
  memory_deallocate_heap_nosem(heapp, fptr + 1);
}

void *memory_reallocate_heap(yspartition_t *heapp, void *ptr, size_t size)
{
  blockheader_t *bptr;
  size_t cur_size;


  if (ptr == NULL)
    return memory_allocate_heap(heapp, size);

  if (size == 0)
  {
    memory_deallocate_heap(heapp, ptr);
    return NULL;
  }
  
  if (sem_wait(&(heapp->partition_lock)) != 0)
    return (void *)NULL;

  bptr = (blockheader_t *)ptr - 1;

  if (invalid_pointer(heapp, ptr, bptr)) {
    sem_post(&(heapp->partition_lock));
    return (void *)NULL;
  }

  cur_size = bptr->__blocksize;
  size += sizeof(blockheader_t);
  size = ((size + sizeof(blockheader_t) - 1) & ~(sizeof(blockheader_t) - 1));
  if (size < cur_size)
  {
    if (cur_size - size >= sizeof(blockheader_t) * 2)
    {
      add_to_freelist(heapp, (blockheader_t *)((char *)bptr+size), cur_size - size);
      bptr->__blocksize = size;
    }
    sem_post(&(heapp->partition_lock));
    return ptr;
  }
  else
  {
    blockheader_t *sptr = heapp->partition_free_head, *fptr, *aptr = NULL;

    if (cur_size == size)
    {
      sem_post(&(heapp->partition_lock));
      return ptr;
    }
    fptr = (blockheader_t *)((char *)bptr + cur_size);
    while (sptr->__next != heapp->partition_free_head)
    {
      if (sptr->__next == fptr && 
          sptr->__next->__blocksize + cur_size >= size)
      {
        aptr = sptr;
        break;
      }
      sptr = sptr->__next;
    }
    if (aptr != NULL)
    {
      bptr->__blocksize += aptr->__next->__blocksize;
      if (aptr->__next == heapp->partition_free_tail)
        heapp->partition_free_tail = aptr;
      aptr->__next = aptr->__next->__next;
      if (bptr->__blocksize - size >= sizeof(blockheader_t) * 2)
      {
        add_to_freelist(heapp, (blockheader_t *)((char *)bptr+size), bptr->__blocksize - size);
        bptr->__blocksize = size;
      }
      sem_post(&(heapp->partition_lock));
      return ptr;
    }
    else
    {
      blockheader_t *nptr;

      nptr = memory_allocate_heap_nosem(heapp, size - sizeof(blockheader_t));

      if (nptr == NULL)
      {
        sem_post(&(heapp->partition_lock));
        return NULL;
      }
      memcpy(nptr, ptr, cur_size - sizeof(blockheader_t));

      memory_deallocate_heap_nosem(heapp, ptr);
      sem_post(&(heapp->partition_lock));
      return nptr;
    }
  }
}

void partition_add(yspartition_t *Partition)
{
  yspartition_t* p;

    sem_init(&(Partition->partition_lock), 0, 1);
    sem_wait(&partition_list_semaphore);

  for (p = partition_list; p != NULL; p = p->partition_next) {
    if (p == Partition) {
      break;
    }
  }

  if (p == NULL) {
    Partition->partition_next = partition_list;
    partition_list = Partition;
  }

    sem_post(&partition_list_semaphore);
}

int partition_heap_init2(yspartition_t *p)
{
  blockheader_t *first_header;
  int local_heap_size;

  p->partition_free_head = 0; 

  if (p->partition_size < sizeof(blockheader_t) * 2)
    return -1;

  local_heap_size = (p->partition_size / sizeof(blockheader_t)) * sizeof(blockheader_t);
  p->partition_size = local_heap_size;

  first_header = (blockheader_t *)p->partition_base;
  first_header->__blocksize = local_heap_size;

  (p->partition_dummy).__blocksize = 0;

  (p->partition_dummy).__next = first_header;
  first_header->__next = &(p->partition_dummy);
  p->partition_free_head = &(p->partition_dummy);
  p->partition_free_tail = first_header;

  p->partition_top = (int *)((unsigned int)p->partition_base + local_heap_size );

  return 0;
}

int partition_init_heap(yspartition_t *Partition, void* Base, size_t Size)
                 
{
  task_align(&Base, &Size);

  Partition->partition_free_head =  0;
  Partition->partition_free_tail  = 0;
  Partition->partition_base = Base;
  Partition->partition_size = Size;
  Partition->partition_next = NULL;
  Partition->created = FALSE;
  if (partition_heap_init2(Partition) == -1)
    return -1;

  partition_add(Partition);
  return 0;
}

yspartition_t *partition_create_heap(void* Base, size_t Size)
                 
{
    yspartition_t *Partition = NULL;
    
    Partition = malloc(sizeof(yspartition_t));
    if (Partition == NULL)
        return NULL;
    
    partition_init_heap(Partition,Base,Size);
    Partition -> created = TRUE;
    return Partition;
}
void partition_heap_display(yspartition_t *p)
{
  blockheader_t *i;
  blockheader_t *m;


  i =  ((blockheader_t *) p->partition_free_head)->__next;
  m =  (blockheader_t *) p->partition_free_head;

  while(i != m)
  {
      printf("Block %x: %d  Next block : %x %x\n",(int)i,(int)i->__blocksize,(int)i->__next,(int)m);
      i = i->__next;
  }
}

void partition_delete (yspartition_t* Partition)
{
    partition_delete_heap((yspartition_t *)Partition);

    if(Partition->created == TRUE)
        free(Partition);
    
}

void* partition_get_baseaddr(yspartition_t* Partition)
{
    return Partition->partition_base;
}

int partition_status(yspartition_t* Partition,DRVOS_PartitionStatus_t *Status,int Flag)
{
  blockheader_t *i;
  blockheader_t *m;
  int freesize=0;
  int largestfreeblocksize=0;
  if (sem_wait(&(Partition->partition_lock)) != 0)
    return -1;

  i =  ((blockheader_t *) Partition->partition_free_head)->__next;
  m =  (blockheader_t *) Partition->partition_free_head;

  while(i != m)
  {
	freesize += ((int)i->__blocksize);
	if(((int)i->__blocksize) > largestfreeblocksize)
	  largestfreeblocksize = ((int)i->__blocksize);
     printf("Block %x: %d  Next block : %x %x\n",(int)i,(int)i->__blocksize,(int)i->__next,(int)m);
     i = i->__next;
  }
  sem_post(&(Partition->partition_lock));
  Status->PartSize = Partition->partition_size;
  Status->PartFreeSize = freesize;
  Status->PartUsedSize = Status->PartSize - freesize;
  Status->PartLargestFreeBlockSize = largestfreeblocksize;
  return 0;
}

void partition_status_printf()
{
  yspartition_t *Current;  
  DRVOS_PartitionStatus_t status;
  sem_wait(m_pSemPartition);
  Current = partition_list;
  while(Current != NULL){
  	memset(&status,0,sizeof(DRVOS_PartitionStatus_t));
    partition_status(Current,&status,0);
    printf("\npartition_status(0x%8x)\n",Current);
    printf("PartSize = %d\n", status.PartSize);
    printf("PartFreeSize = %d\n", status.PartFreeSize);
    printf("PartUsedSize = %d\n", status.PartUsedSize);
    printf("PartLargestFreeBlockSize = %d\n", status.PartLargestFreeBlockSize);
    Current = Current->partition_next;
  }
  sem_post(m_pSemPartition);
}


