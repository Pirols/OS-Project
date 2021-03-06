#pragma once

#define MAX_NUM_PROCESSES 1024
#define MAX_NUM_RESOURCES 1024
#define MAX_NUM_SEMAPHORES 1024
#define MAX_NUM_RESOURCES_PER_PROCESS 32
#define MAX_NUM_DESCRIPTORS_PER_PROCESS 32
#define MAX_NUM_SEMAPHORES_PER_PROCESS 32
#define MAX_NUM_SEMDESCRIPTORS_PER_PROCESS 32

#define STACK_SIZE        16384
// signals
#define MAX_SIGNALS 32
#define DSOS_SIGCHLD 0x1
#define DSOS_SIGHUP  0x2

// errors
#define DSOS_ESYSCALL_ARGUMENT_OUT_OF_BOUNDS -1
#define DSOS_ESYSCALL_NOT_IMPLEMENTED -2
#define DSOS_ESYSCALL_OUT_OF_RANGE -3
#define DSOS_EFORK  -4
#define DSOS_EWAIT  -5
#define DSOS_ESPAWN  -6
#define DSOS_ESLEEP  -7
#define DSOS_ERESOURCECREATE -8
#define DSOS_ERESOURCEOPEN -9
#define DSOS_ERESOURCENOEXCL -10
#define DSOS_ERESOURCENOFD -11
#define DSOS_ERESOURCECLOSE -12
#define DSOS_ERESOURCEINUSE -13

// semaphores errors
#define DSOS_ESEMOPEN -14
#define DSOS_ESEMCLOSE -15
#define DSOS_ESEMWAIT -16
#define DSOS_ESEMPOST -17
#define DSOS_ESEMTEST -18
#define DSOS_ESEMNOTFOUND -19
#define DSOS_ESEMALREADYEXIST -20
#define DSOS_ESEMPTR -21
#define DSOS_ESEMFDNOTFOUND -22
#define DSOS_ESEMFD -23
#define DSOS_ESEMCLOSEDESNF -24
#define DSOS_ESEMFREE -25
#define DSOS_ESEMDESFREE -26
#define DSOS_ESEMDESPTRFREE -27

// semaphores 
#define DSOS_SEMOPEN_CREATE 0               // Tries to create a new semaphore, if the id is already used returns an error
#define DSOS_SEMOPEN_LINK 1                 // Tries to link to an existing semaphore, if it doesn't exist returns an error
#define DSOS_SEMOPEN_LNKCRT 2               // Tries to link to an existing semaphore, if it doesn't exist it creates a new one

// test
#define SEM_FILL 0
#define SEM_EMPTY 1
#define SEM_MUTEX 2
#define BUFFER_LENGTH_SEM 5
#define ROUNDS 10

// syscall numbers
#define DSOS_MAX_SYSCALLS 32
#define DSOS_MAX_SYSCALLS_ARGS 8
#define DSOS_CALL_PREEMPT   1
#define DSOS_CALL_FORK      2
#define DSOS_CALL_WAIT      3
#define DSOS_CALL_EXIT      4
#define DSOS_CALL_SPAWN     5
#define DSOS_CALL_SLEEP     6
#define DSOS_CALL_OPEN_RESOURCE 7
#define DSOS_CALL_CLOSE_RESOURCE 8
#define DSOS_CALL_DESTROY_RESOURCE 9
#define DSOS_CALL_SHUTDOWN  10
#define DSOS_CALL_SEMOPEN   11
#define DSOS_CALL_SEMCLOSE  12
#define DSOS_CALL_SEMWAIT   13
#define DSOS_CALL_SEMPOST   14
#define DSOS_CALL_SEMTEST   15

// resources
#define DSOS_CREATE 0x1
#define DSOS_READ 0x2
#define DSOS_WRITE 0x3
#define DSOS_EXCL 0x4

// scheduling
#define ALPHA 0.5f
#define INTERVAL 100 // milliseconds for timer tick
