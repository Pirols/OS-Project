#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait(){
  int id=running->syscall_args[0];

  Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);
  // Is it enough to look for the semaphore in the global list?
  // Shall I look to the process' semaphores?

  if(!sem) {
    running->syscall_retvalue = DSOS_ESEMWAIT;
    return;
  }
  // There is no need of mutual exclusion since we're not supporting multithreading sor multiprocessing.

  if(sem->count > 0) {
    (sem->count)--;
    running->syscall_retvalue = 0;
  }
  else {
    // Change running->status to 0x3 ( Waiting )
    // Move running to the waiting_list
    // Schedule a ready process as running
    // Maybe internal_schedule or swapcontext(disastros.c)?
    int pid = running->pid;
    running->status = Waiting;
    List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, pid);
    List_insert(&waiting_list, waiting_list.last, running);
    running = ready_list.first;
    running->status = Running;
    List_detach(&ready_list, running);

    running->return_value = 0;
    // TO BE CHECKED! I'm actually changing the ret_value of a process which actually didn't start the syscall!!
  }
}
