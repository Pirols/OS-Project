#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost(){
  int id=running->syscall_args[0];

  Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);
  // Is it enough to look for the semaphore in the global list?
  // Shall I look to the process' semaphores?

  if(!sem) {
    running->syscall_retvalue = DSOS_ESEMPOST;
    return;
  }
  (sem->count)++; 

  if(sem->count >= 1) {       // There are no processes waiting for this semaphore
    if(sem->waiting_descriptors.size != 0) running->syscall_retvalue = DSOS_ESEMPOST;
    else running->syscall_retvalue = 0;
  }

  else {
    // Since count is less than 0 we need to have at least one process in waiting_descriptors list
    if(sem->waiting_descriptors.size == 0) running->syscall_retvalue = DSOS_ESEMPOST;
    else {
      ListItem *descriptor = sem->waiting_descriptors.first;
      /*List_detach(&sem->waiting_descriptors, first);
      List_detach(&waiting_list, first);
      List_insert(&ready_list, ready_list.last, first);*/




      running->syscall_retvalue = 0;
    }
  }
}
