#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait() {
  int id=running->syscall_args[0];
  
  ListHead process_sems = running->sem_descriptors;
  SemDescriptor *sem_des = SemDescriptorList_byFd(&process_sems, id);

  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMFDNOTFOUND;
    return;
  }

  Semaphore *sem = sem_des->semaphore;

  if(!sem) {
    running->syscall_retvalue = DSOS_ESEMWAIT;
    return;
  }
  else if(sem->count <= 0) { // Running must wait
    (sem->count)--;    

    List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem*)sem_des->ptr_wtr);

    running->status = Waiting;
    List_insert(&waiting_list, waiting_list.last, (ListItem *)running);

    if(ready_list.first) {
      running=(PCB*) List_detach(&ready_list, ready_list.first); 
      running->syscall_retvalue = 0; 
    }
    else {
      running = 0;
      running->syscall_retvalue = 0;
    }
  }
  else {
   (sem->count)--;
   running->syscall_retvalue = 0;
  }
}
