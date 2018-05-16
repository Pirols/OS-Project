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
  int ret;
  
  printf("sono dentro la close\n");

  SemDescriptor *sem_des = SemDescriptorList_byFd(&(running->sem_descriptors), sem_fd);
  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }
  else {
    printf("TAAAAAPPOOOOOST!!!\n");
  }

  Semaphore *sem = sem_des->semaphore;

  List_detach(&sem->descriptors, (ListItem *)sem_des->ptr);

  if(!(sem->descriptors).size) {
    sem = (Semaphore *)List_detach(&semaphores_list, (ListItem *)sem);
    ret = Semaphore_free(sem);
    if(ret != 0) {
      printf("ERRORE FREE NELLA CLOSE RIGA 28\n");
      running->syscall_retvalue = DSOS_ESEMCLOSE;
      return;
    }
  }

  SemDescriptorPtr *sem_des_ptr = sem_des->ptr;
  SemDescriptorPtr *sem_des_ptr_wtr = sem_des->ptr_wtr;

  ret = SemDescriptor_free(sem_des);
  if(ret != 0) {
    printf("ERRORE FREE NELLA CLOSE RIGA 39\n");
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  ret = SemDescriptorPtr_free(sem_des_ptr);
  if(ret != 0) {
    printf("ERRORE FREE NELLA CLOSE RIGA 46\n");
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  ret = SemDescriptorPtr_free(sem_des_ptr_wtr);
  if(ret != 0) {
    printf("ERRORE FREE NELLA CLOSE RIGA 53\n");
    running->syscall_retvalue = DSOS_ESEMCLOSE;
    return;
  }

  printf("TUTTO OK\n");

  running->syscall_retvalue = 0;
  return;
}
