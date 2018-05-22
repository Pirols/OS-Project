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
void sleeperFunction(void* args) {
  printf("Hello, I am the sleeper, and I sleep %d\n", disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void Producer(void* args) {
  int i, ret;

  printf("\n[Prod%d]Hello, I'm starting a producer\n", running->pid);

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Prod%d]sem_fill opened with fd: %d\n", running->pid, sem_fill);

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_empty < 0) disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Prod%d]sem_empty opened with fd: %d\n", running->pid, sem_empty);

  int sem_mut = disastrOS_openSemaphore(SEM_MUTEX, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_mut < 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Prod%d]sem_mut opened with fd: %d\n\n", running->pid, sem_mut);

  for(i = 0; i < ROUNDS; i++) {
    if(!disastrOS_testSemaphore(SEM_EMPTY)) printf("[Prod%d]Going to be blocked.\n", running->pid);
    disastrOS_waitSemaphore(sem_empty);
    disastrOS_waitSemaphore(sem_mut);

    printf("[Prod%d]Critical section!\n", running->pid);
    action[write_index] = running->pid;
    write_index = (write_index + 1) % BUFFER_LENGTH_SEM;

    disastrOS_postSemaphore(sem_mut);
    disastrOS_postSemaphore(sem_fill);
  } 

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("\n[Prod%d]sem_fill closed!\n", running->pid);

  ret = disastrOS_closeSemaphore(sem_empty); 
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Prod%d]sem_empty closed!\n", running->pid);

  ret = disastrOS_closeSemaphore(sem_mut);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Prod%d]sem_mut closed!\n\n", running->pid);

  disastrOS_exit(disastrOS_getpid()+1);
}

void Consumer(void* args) {

  int i, ret;

  printf("\n[Cons%d]Hello, I'm starting a consumer\n", running->pid);

  int sem_fill = disastrOS_openSemaphore(SEM_FILL, 0, DSOS_SEMOPEN_LNKCRT);
  if(sem_fill < 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Cons%d]sem_fill opened with fd: %d\n", running->pid, sem_fill);

  int sem_empty = disastrOS_openSemaphore(SEM_EMPTY, BUFFER_LENGTH_SEM, DSOS_SEMOPEN_LNKCRT);
  if(sem_empty < 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Cons%d]sem_empty opened with fd: %d\n", running->pid, sem_empty);

  int sem_mut = disastrOS_openSemaphore(SEM_MUTEX, 1, DSOS_SEMOPEN_LNKCRT);
  if(sem_mut < 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Cons%d]sem_mut opened with fd: %d\n\n", running->pid, sem_mut);

  for(i = 0; i < ROUNDS; i++) {
    if(!disastrOS_testSemaphore(SEM_FILL)) printf("[Cons%d]Going to be blocked.\n", running->pid);
    disastrOS_waitSemaphore(sem_fill);
    disastrOS_waitSemaphore(sem_mut);

    printf("[Cons%d]Critical section!\n", running->pid);
    int lastAction = action[read_index];
    read_index = (read_index + 1) % BUFFER_LENGTH_SEM;
    x += lastAction;
    if (read_index % 10 == 0) {
      printf("[Cons%d]After the last %d actions balance is now %d.\n\n", running->pid, ROUNDS, x);
    }

    disastrOS_postSemaphore(sem_mut);
    disastrOS_postSemaphore(sem_empty);
  } 

  ret = disastrOS_closeSemaphore(sem_fill);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("\n[Cons%d]sem_fill closed!\n", running->pid);

  ret = disastrOS_closeSemaphore(sem_empty); 
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Cons%d]sem_empty closed!\n", running->pid);

  ret = disastrOS_closeSemaphore(sem_mut);
  if(ret != 0)  disastrOS_exit(disastrOS_getpid()+1);
  else printf("[Cons%d]sem_mut closed!\n\n", running->pid);

  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();

  disastrOS_spawn(sleeperFunction, 0);

  int children = 0, i = 0, pid, retval;
  for (; i < 2; ++i) {
    disastrOS_spawn(Producer, 0);
    children++;
  }

  for (i = 0; i < 2; ++i) {
    disastrOS_spawn(Consumer, 0);
    children++;
  }

  while(children>0 && (pid = disastrOS_wait(0, &retval))>=0) {
    printf("initFunction, child: %d terminated, retval: %d, alive: %d \n", pid, retval, children);
    --children;
  }

  disastrOS_printStatus();

  disastrOS_shutdown();
}

int main(int argc, char** argv) {
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}