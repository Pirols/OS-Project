#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "pool_allocator.h"

void internal_semClose() {
  
	int id = running->syscall_args[0];
  int ret;
  
  SemDescriptor *sem_des = SemDescriptorList_byFd(&(running->sem_descriptors), id);
  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  Semaphore *sem = sem_des->semaphore;

  List_detach(&(sem->descriptors), (ListItem*)sem_des->ptr);

  if (!(sem->descriptors).size) {
    sem = (Semaphore*) List_detach(&semaphores_list, (ListItem*) sem);
    (sem->descriptors).first = 0;
    (sem->descriptors).last = 0;
    ret = Semaphore_free(sem);
    if (ret != 0x0) {
      printf("Errore Semaphore_free: %s\n",PoolAllocator_strerror((PoolAllocatorResult) ret));
      running->syscall_retvalue = DSOS_ESEMCLOSE;
      return;
    }
  }

  SemDescriptorPtr *sem_des_ptr = sem_des->ptr;
  SemDescriptorPtr *sem_des_ptr_wtr = sem_des->ptr_wtr;

  ret = SemDescriptor_free(sem_des);
  if(ret != 0x0) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  ret = SemDescriptorPtr_free(sem_des_ptr);
  if(ret != 0x0) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  ret = SemDescriptorPtr_free(sem_des_ptr_wtr);
  if(ret != 0x0) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }
  else {
    running->syscall_retvalue = 0;
    return;
  }
}
