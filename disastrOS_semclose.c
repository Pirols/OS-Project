#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose(){
  // do stuff :)
	int sem_fd = running->syscall_args[0];
  
  SemDescriptor *sem_des = SemDescriptorList_byFd(&running->sem_descriptors, sem_fd);
  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  sem_des = (SemDescriptor *)List_detach(&running->sem_descriptors, (ListItem *)sem_des);
  assert(sem_des);

  Semaphore *sem = sem_des->semaphore;

  SemDescriptorPtr *sem_des_ptr = (SemDescriptorPtr *)List_detach(&sem->descriptors, (ListItem *)sem_des);
  assert(sem_des_ptr);
  SemDescriptor_free(sem_des);
  SemDescriptorPtr_free(sem_des_ptr);
  running->syscall_retvalue = 0;
}
