#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"
#include "disastrOS_constants.h"
#include "disastrOS_globals.h"

void initFunction(void *args);
void Producer(void *args);
void Consumer(void *args);

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void Producer(void* args){
  int i, ret;

  printf("Hello, I'm starting a producer with pid: %d\n",disastrOS_getpid());

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0) {
    printf("[TEST_PROD] error opening sem_fill!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_empty < 0) {
    printf("[TEST_PROD] error opening sem_empty!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  int sem_mut1 = disastrOS_openSemaphore(SEM_MUTEX1, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0) {
    printf("[TEST_PROD] error opening sem_mut1!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  /*for(i=0; i<ROUNDS; ++i) {
    disastrOS_waitSemaphore(sem_empty);
    disastrOS_waitSemaphore(sem_mut1);

    //ROBA DA SCRIVERE DOPO

    disastrOS_postSemaphore(sem_mut1);
    disastrOS_postSemaphore(sem_fill);
  } */

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0) {
    printf("[TEST_PROD] error closing sem_fill!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  disastrOS_closeSemaphore(sem_empty);
  if(ret != 0) {
    printf("[TEST_PROD] error closing sem_empty!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  disastrOS_closeSemaphore(sem_mut1);
  if(ret != 0) {
    printf("[TEST_PROD] error closing sem_mut1!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }
}

void Consumer(void* args){
  int i, ret;

  printf("Hello, I'm starting a consumer with pid: %d\n",disastrOS_getpid());

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0) {
    printf("[TEST_CONS] error opening sem_fill!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_empty < 0) {
    printf("[TEST_CONS] error opening sem_empty!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  int sem_mut2 = disastrOS_openSemaphore(SEM_MUTEX1, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_mut2 < 0) {
    printf("[TEST_CONS] error opening sem_mut2!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  for(i=0; i<ROUNDS; ++i) {
    disastrOS_waitSemaphore(sem_fill);
    disastrOS_waitSemaphore(sem_mut2);

    //ROBA DA SCRIVERE DOPO

    disastrOS_postSemaphore(sem_mut2);
    disastrOS_postSemaphore(sem_empty);
  } 

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0) {
    printf("[TEST_CONS] error closing sem_fill!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  disastrOS_closeSemaphore(sem_empty);
  if(ret != 0) {
    printf("[TEST_CONS] error closing sem_empty!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }

  disastrOS_closeSemaphore(sem_mut2);
  if(ret != 0) {
    printf("[TEST_CONS] error closing sem_mut2!\n");
    disastrOS_exit(disastrOS_getpid()+1);
  }
}

void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);

  int write_index = 0;
  int read_index = 0;

  printf("I feel like to spawn 10 nice threads\n");
  
  int children=0;
  int fd[10];
  int i;
  
  for (int i=0; i<5; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    fd[i]=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd[i]);
    disastrOS_spawn(Producer, 0);
    children++;
  }

  for(; i<10; ++i) {
    int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource (and creating if necessary)\n");
    fd[i]=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd[i]);
    disastrOS_spawn(Consumer, 0);
    children++;
  }

  int retval;
  int pid;
  while(children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, children);
    --children;
  }

  for(i=0; i<10; ++i) {
    printf("closing resource %d\n", fd[i]);
    disastrOS_closeResource(fd[i]);
    disastrOS_destroyResource(i);
  }

  disastrOS_printStatus();

  printf("shutdown!\n");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  //printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
