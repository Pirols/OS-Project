#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semTest() {
    int id = running->syscall_args[0];

    ListHead process_sems = running->sem_descriptors;
    SemDescriptor *sem_des = SemDescriptorList_byFd(&process_sems, id);

    if(!sem_des) {
        running->syscall_retvalue = DSOS_ESEMFDNOTFOUND;
        return;
    }

    Semaphore *sem = sem_des->semaphore;

    if(!sem) {
        running->syscall_retvalue = DSOS_ESEMTEST;
        return;
    }

    running->syscall_retvalue = 0;

    if(sem->count >= 1) (running->syscall_retvalue)++;
}