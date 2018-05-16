#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  int id=running->syscall_args[0];

  //Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);
  // Is it enough to look for the semaphore in the global list?
  // Shall I look to the process' semaphores?

  SemDescriptor *sem_des = SemDescriptorFind_byID(&running->sem_descriptors, (ListItem *)id);

  Semaphore *sem = sem_des->semaphore;

  if(!sem) {
    running->syscall_retvalue = DSOS_ESEMWAIT;
    return;
  }
  // There is no need of mutual exclusion since we're not supporting multithreading sor multiprocessing.

  if(sem->count > 0) {
    printf("sem_count: %d\n\n", sem->count);
    (sem->count)--;
    running->syscall_retvalue = 0;
  }
  else {
    printf("sem_count < 0\n\n");
    // Change running->status to 0x3 ( Waiting )
    // Move running to the waiting_list
    // Schedule a ready process as running
    // Maybe internal_schedule or swapcontext(disastros.c)?
    (sem->count)--;

    printf("decrementato count\n");

    List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem*)sem_des->ptr_wtr);
    //int pid = running->pid;
    running->status = Waiting;
    //List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem *)pid);
    List_insert(&waiting_list, waiting_list.last, (ListItem *)running);

    if(ready_list.first) {
      running = ready_list.first;
      running->status = Running;
      running = (PCB *)List_detach(&ready_list, (ListItem *)running);
    }
    else {
      running = 0;
      printf("zzzzz notte belliiii\n\n\n");
    }

    running->return_value = 0;
    // TO BE CHECKED! I'm actually changing the ret_value of a process which actually didn't start the syscall!!
  }
}
