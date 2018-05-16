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

	char new_sem = 0;
	
	//ALLOC THE SEMAPHORE
	Semaphore *sem = SemaphoreList_byId(&semaphores_list, id);
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

		new_sem = 1;
	}
	else if(mode == DSOS_SEMOPEN_LINK) {
		if(!sem) {
			running->syscall_retvalue = DSOS_ESEMNOTFOUND;
			return;
		}
		new_sem = 1;
	}
	else if(mode == DSOS_SEMOPEN_LNKCRT) {
		if(!sem) {
			sem = Semaphore_alloc(id, value);
			if(!sem) {
				running->syscall_retvalue = DSOS_ESEMOPEN;
				return;
			}

			List_insert(&semaphores_list, semaphores_list.last, (ListItem*) sem);

			new_sem = 1;
		}
<<<<<<< HEAD
		else new_sem = 1;
	}
=======
		else new_sem = 0;
	} 

>>>>>>> 3416f3e02e7adafb57055745411cfe9f45f977df
	
	//controllo che non sia già aperto NEL PROCESSO 
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
		int fd = running->last_sem_fd;
		SemDescriptor* sem_des = SemDescriptor_alloc(fd, sem, running);
		if(!sem_des){
			running->syscall_retvalue = DSOS_ESEMFD;
			return;
		}
<<<<<<< HEAD
		
		//ADD IT TO SEM_DESCRIPTORS LIST
		List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem *)sem_des);
	
=======
			
>>>>>>> 3416f3e02e7adafb57055745411cfe9f45f977df
		//ADD IT TO THE SEMAPHORES LIST
		if(new_sem) {
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

			//DEBUG CODE:
			//running=(PCB*) List_detach(&ready_list, ready_list.first);
		}
	}
	
	//RETURN THE FD TO THE RUNNING PROCESS
	running->syscall_retvalue = fd;
	return;
}
