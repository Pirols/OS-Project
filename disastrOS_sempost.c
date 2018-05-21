#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost() {
  int id=running->syscall_args[0];

  ListHead sem_list = running->sem_descriptors;

  SemDescriptor *sem_des = SemDescriptorList_byFd(&sem_list, id);

  if(!sem_des) {
    running->syscall_retvalue = DSOS_ESEMPOST;
    return;
  }
  
  Semaphore *sem = sem_des->semaphore;

  if(sem->count < 0) {
    // Since count is less than 0 we need to have at least one process in waiting_descriptors list
    SemDescriptorPtr *new_proc = (SemDescriptorPtr *)List_detach(&(sem->waiting_descriptors), (ListItem *)sem->waiting_descriptors.first);

    PCB *new_proc_pcb = new_proc->descriptor->pcb;

    List_detach(&waiting_list, (ListItem *)new_proc_pcb);

    new_proc_pcb->status = Ready;

    List_insert(&ready_list, ready_list.last, (ListItem *)new_proc_pcb);
  }

  (sem->count)++;
  running->syscall_retvalue = 0;
}
