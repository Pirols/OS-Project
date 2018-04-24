#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen(){
	int id=running->syscall_args[0];
  int value=running->syscall_args[1];
  int mode=running->syscall_args[2];
	
	//ALLOC THE SEMAPHORE
	Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);

	//CHECK IF SOMETHING WENT WRONG
	if(!sem){
		running->syscall_retvalue = DSOS_ESEMOPEN;
		return;
	}
	
	//CREATE THE DESCRIPTOR AND CHECK
	SemDescriptor *sem_des = SemDescriptor_alloc(running->last_sem_fd, sem, running);
	
	if(!sem_des){
		running->syscall_retvalue = DSOS_ESEMFD;
		return;
	}
	
	//INCREMENT LAST_SEM_FD VALUE
	running->last_sem_fd++;
	
	//ADD IT TO SEM_DESCRIPTORS LIST
	SemDescriptorPtr *sem_des_ptr = SemDescriptorPtr_alloc(sem_des);
	List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem *)sem_des);
	
	//ADD IT TO THE SEMAPHORES LIST
	sem_des->ptr = sem_des_ptr;
	List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)sem_des_ptr);
	
	//RETURN THE FD TO THE RUNNING PROCESS
	running->syscall_retvalue = sem_des->fd;
}











