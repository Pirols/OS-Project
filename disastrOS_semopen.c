#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "pool_allocator.h"

static char _sem_item_buffer[SEM_ITEM_BUFFER_SIZE];
static Pool_Allocator sem_item_allocator;

void internal_semOpen(){
	int id=running->syscall_args[0];
  int value=running->syscall_args[1];
	
	//ALLOC THE SEMAPHORE
	Semaphore *sem = SemaphoreList_byId(semaphores_list, id);
	
	//CHECK IF SOMETHING WENT WRONG
	if(!sem){
		running->syscall_retvalue = DSOS_ESEMOPEN;
		return;
	}
	
	//CREATE THE DESCRIPTOR AND CHECK
	Descriptor *sem_des = Descriptor_alloc(running->last_sem_fd, sem, running);
	
	if(!sem_des){
		running->syscall_retvalue = DSOS_ESEMFD;
		return;
	}
	
	//INCREMENT LAST_SEM_FD VALUE
	running->last_sem_fd++;
}











