#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"
#include "disastrOS_globals.h"
#include "disastrOS_constants.h"

int read_index, write_index, x;
int action[BUFFER_LENGTH_SEM];

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

  printf("[Prod %d]Hello, I'm starting a producer\n", running->pid);

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Prod %d]sem_fill opened with fd: %d\n", disastrOS_getpid(), sem_fill);

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_empty < 0) disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Prod %d]sem_empty opened with fd: %d\n", disastrOS_getpid(), sem_empty);

  int sem_mut = disastrOS_openSemaphore(SEM_MUTEX, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Prod %d]sem_mut opened with fd: %d\n", disastrOS_getpid(), sem_mut);

  for(i = 0; i < ROUNDS; i++) {
    disastrOS_waitSemaphore(sem_empty);
    disastrOS_waitSemaphore(sem_mut);

    printf("[Prod %d]I'm the producer and I'm in the critical section!\n", running->pid);
    action[write_index] = running->pid;
    write_index = (write_index + 1) % BUFFER_LENGTH_SEM;

    disastrOS_postSemaphore(sem_mut);
    disastrOS_postSemaphore(sem_fill);
  } 

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_fill closed\n");

  ret = disastrOS_closeSemaphore(sem_empty); 
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_empty closed\n");

  ret = disastrOS_closeSemaphore(sem_mut);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_mut1 closed\n");

  disastrOS_exit(disastrOS_getpid()+1);
}

void Consumer(void* args){

  int i, ret;

  printf("[Cons %d]Hello, I'm starting a consumer\n", running->pid);

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Cons %d]sem_fill opened with fd: %d\n", disastrOS_getpid(), sem_fill);

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Cons %d]sem_empty opened with fd: %d\n", disastrOS_getpid(), sem_empty);

  int sem_mut = disastrOS_openSemaphore(SEM_MUTEX, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("[Cons %d]sem_mut opened with fd: %d\n", disastrOS_getpid(), sem_mut);

  for(i = 0; i < ROUNDS; i++) {
    disastrOS_waitSemaphore(sem_fill);
    disastrOS_waitSemaphore(sem_mut);

    printf("[Cons %d]I'm the consumer and I'm in the critical section!\n", disastrOS_getpid());
    int lastAction = action[read_index];
    read_index = (read_index + 1) % BUFFER_LENGTH_SEM;
    x += lastAction;
    /*if (read_index % 10 == 0) {
      printf("[Cons %d]After the last 10 action balance is now %d.\n", running->pid, x);
    }*/

    disastrOS_postSemaphore(sem_mut);
    disastrOS_postSemaphore(sem_empty);
  } 

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_fill closed\n");

  ret = disastrOS_closeSemaphore(sem_empty);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_empty closed\n");

  ret = disastrOS_closeSemaphore(sem_mut);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  //else printf("sem_mut2 closed\n");

  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started pid=%d\n",running->pid);
  disastrOS_spawn(sleeperFunction, 0);

  write_index=0;
  read_index=0;
  printf("I feel like to spawn 10 nice processes\n");
  int children=0;
  int i;
  //int fd[10];
  for (i=0; i<5; ++i) {
    /*int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource\n");
    fd[i]=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd[i]);*/
    disastrOS_spawn(Producer, 0);
    children++;
  }

  for (; i<10; ++i) {
    /*int type=0;
    int mode=DSOS_CREATE;
    printf("mode: %d\n", mode);
    printf("opening resource\n");
    fd[i]=disastrOS_openResource(i,type,mode);
    printf("fd=%d\n", fd[i]);*/
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
  /*for (i=0; i<10; ++i) {
    printf("closing resource %d\n",fd[i]);
    disastrOS_closeResource(fd[i]);
    disastrOS_destroyResource(i);
  }*/

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
  // spawn an init process

  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
