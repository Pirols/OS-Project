#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"
#include "linked_list.h"
#include "disastrOS_globals.h"

void internal_semOpen(){
	int id=running->syscall_args[0];
	int value=running->syscall_args[1];
	int mode=running->syscall_args[2];

	
	//ALLOC THE SEMAPHORE
	ListHead semaphores_used = semaphores_list;
	Semaphore *sem = SemaphoreList_byId(&semaphores_used, id);
	int fd = running->last_sem_fd;

	//CHECK MODE
	if(mode == DSOS_SEMOPEN_CREATE) {
		if(sem) {
			running->syscall_retvalue = DSOS_ESEMALREADYEXIST;
			return;
		}
		
		sem = Semaphore_alloc(id, value);
		if(!sem) {
			running->syscall_retvalue = DSOS_ESEMOPEN;
			return;
		}

		List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
	}
	else if(mode == DSOS_SEMOPEN_LINK) {
		if(!sem) {
			running->syscall_retvalue = DSOS_ESEMNOTFOUND;
			return;
		}
	}
	else if(mode == DSOS_SEMOPEN_LNKCRT) {
		if(!sem) {
			sem = Semaphore_alloc(id, value);
			if(!sem) {
				running->syscall_retvalue = DSOS_ESEMOPEN;
				return;
			}

			List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);
		}
	}
	else {
		printf("Insert a correct value for semopen mode!\n");
		running->syscall_retvalue = DSOS_ESEMOPEN;
		return;
	}
	
	//check that the semaphore is not already opened in the process
	ListHead sem_opened_local = running->sem_descriptors;
	SemDescriptor *check = SemDescriptorFind_byID(&sem_opened_local, id);
	if(check) {
		running->syscall_retvalue = check->fd;
		return;
	}
	else {
		//CREATE THE DESCRIPTOR AND CHECK
		//we have to create the semdescriptor
		(running->last_sem_fd)++;

		SemDescriptor* sem_des = SemDescriptor_alloc(fd, sem, running);
		if(!sem_des){
			running->syscall_retvalue = DSOS_ESEMFD;
			return;
		}
	
		//ADD IT TO THE SEMAPHORES LIST
		//if(new_sem) {
		SemDescriptorPtr *sem_des_ptr = SemDescriptorPtr_alloc(sem_des);
		sem_des->ptr = sem_des_ptr;
		List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)sem_des_ptr);
	
		SemDescriptorPtr *sem_des_ptr_wtr = SemDescriptorPtr_alloc(sem_des);
		if(!sem_des_ptr_wtr) {
			running->syscall_retvalue = DSOS_ESEMPTR;
			return;
		}
		sem_des->ptr_wtr = sem_des_ptr_wtr;

		List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem *)sem_des);
		List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)sem_des_ptr);
			
		//RETURN THE FD TO THE RUNNING PROCESS
		running->syscall_retvalue = fd;
	}
}
