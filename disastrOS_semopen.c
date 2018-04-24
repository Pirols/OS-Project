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
  
  char new_sem = -1;
	
	//ALLOC THE SEMAPHORE
	Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);
	
	//CHECK MODE ERRORS
	if(mode == DSOS_SEM_OPEN_CREAT) {
		if(sem != 0) {
			running->syscall_retvalue = DSOS_ESEMALREXT);
			return;
		}
		
		new_sem = 1;
		
		sem = SemaphoreAlloc(id, value);
		if(sem == 0) {
			running->syscall_retvalue = DSOS_ESEMOPEN;
			return;
		}
	}
	else if(mode == DSOS_SEM_OPEN_LINK) {
		if(sem != 0) {
			running->syscall_retvalue = DSOS_ESEMNOTFOUND;
			return;
		}
		
		continue;
	}
	else if(mode == DSOS_SEM_OPEN_CRTLNK) {
		if(sem == 0) {
			new_sem = 1;
			
			sem = SemaphoreAlloc(id, value);
			if(sem == 0) {
				running->syscall_retvalue = DSOS_ESEMOPEN;
				return;
			}
		}
		else {
			new_sem = 0;
			continue;
		}
	}<
	
	
	//CREATE THE DESCRIPTOR AND CHECK
	SemDescriptor *sem_des = SemDescriptor_alloc(running->last_sem_fd, sem, running);
	
	if(!sem_des){
		running->syscall_retvalue = DSOS_ESEMFD;
		return;
	}
	
	//INCREMENT LAST_SEM_FD VALUE
	running->last_sem_fd++;
	
	//ADD IT TO SEM_DESCRIPTORS LIST
	if(new_sem == 1) {
		SemDescriptorPtr *sem_des_ptr = SemDescriptorPtr_alloc(sem_des);
		List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem *)sem_des);
	
	//ADD IT TO THE SEMAPHORES LIST
		sem_des->ptr = sem_des_ptr;
		List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)sem_des_ptr);
	}
	
	//RETURN THE FD TO THE RUNNING PROCESS
	running->syscall_retvalue = sem_des->fd;
}











