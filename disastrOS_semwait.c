#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait() {
  int id=running->syscall_args[0];
  
  SemDescriptor *sem_des = SemDescriptorFind_byID(&(running->sem_descriptors), id);

  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMFDNOTFOUND;
    return;
  }

  Semaphore *sem = sem_des->semaphore;

  if(!sem) {
    running->syscall_retvalue = DSOS_ESEMWAIT;
    return;
  }

  (sem->count)--;

  if(sem->count < 0) { // Running must wait

    List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem*)sem_des->ptr_wtr);

    running->status = Waiting;
    List_insert(&waiting_list, waiting_list.last, (ListItem *)running);

    if(ready_list.first) {
      running=(PCB*) List_detach(&ready_list, ready_list.first); 
      running->status = Running;    
      // Do we need to make a context swap?
    }
    else running = 0;
  }
  
  running->syscall_retvalue = 0;
  // TO BE CHECKED! I'm actually changing the ret_value of a process which actually didn't start the syscall!!

}
