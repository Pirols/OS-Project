#pragma once
#include "disastrOS_pcb.h"
#include "linked_list.h"

#ifdef _DISASTROS_DEBUG_
#include <stdio.h>

#define disastrOS_debug(...) printf(__VA_ARGS__) 

#else //_DISASTROS_DEBUG_

#define disastrOS_debug(...) ;

#endif //_DISASTROS_DEBUG_

// initializes the structures and spawns a fake init process
void disastrOS_start(void (*f)(void*), void* args, char* logfile);

// generic syscall 
int disastrOS_syscall(int syscall_num, ...);

// classical process control
int disastrOS_getpid(); // this should be a syscall, but we have no memory separation, so we return just the running pid
int disastrOS_fork();
void disastrOS_exit(int exit_value);
int disastrOS_wait(int pid, int* retval);
void disastrOS_preempt();
void disastrOS_spawn(void (*f)(void*), void* args );
void disastrOS_shutdown();

// timers
void disastrOS_sleep(int);

// resources (files)
int disastrOS_openResource(int resource_id, int type, int mode);
int disastrOS_closeResource(int fd) ;
int disastrOS_destroyResource(int resource_id);

// semaphores
int disastrOS_openSemaphore(int id, int value, int mode);
int disastrOS_closeSemaphore(int id);
int disastrOS_waitSemaphore(int id);
int disastrOS_postSemaphore(int id);


// debug function, prints the state of the internal system
void disastrOS_printStatus();
