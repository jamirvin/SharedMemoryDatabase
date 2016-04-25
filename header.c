#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

/* Lock the semaphore n of the semaphore set semaph */

void Wait(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  -1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* Unlock the semaphore n of the semaphore set semaph */
void Signal(int semaph, int n)
{
  struct sembuf sop;
  sop.sem_num = n;
  sop.sem_op =  1;
  sop.sem_flg = 0;
  semop(semaph,&sop,1);
}

/* make an array of n semaphores with key k */
int GetSemaphs(key_t k, int n)
{
  int semid, i;

  /* get a set of n semaphores with the given key k */
  
  if ((semid=semget(k,n,IPC_CREAT|0666))!= -1){
    for (i=0;i<n;i++)
      Signal(semid,i); /* unlock all the semaphores */
  }
  return semid;
}

/*
 * Create function for getting database so it isn't
 * needed in all of the other files 
 */
struct StudentInfo* getDatabase(bool create) {
    int id;
    struct StudentInfo* infoptr;
    int options = create ? IPC_CREAT|0666 : 0;

    // Create the shared memory block big enough to hold 20 database
    // entries.
    id = shmget(KEY, SEGSIZE * MAX_DATABASE_SIZE, options);
    if(id < 0) {
        perror("Load: shmget failed");
        exit(1);
    }

    // Attach the shared memory to this process memory
    infoptr = (struct StudentInfo*)shmat(id, 0, 0);
    if(infoptr <= (struct StudentInfo*)(0)) {
        perror("Load: shmat failed");
        exit(2);
    }

    return infoptr;
}

int* getReadcount(bool create) {
    int id;
    int* readers;
    int options = create ? IPC_CREAT|0666 : 0;

    // Create the shared memory block big enough to hold 20 database
    // entries.
    id = shmget(READ_KEY, READ_SIZE, options);
    if(id < 0) {
        perror("Load: shmget failed");
        exit(1);
    }

    // Attach the shared memory to this process memory
    readers = (int*)shmat(id, 0, 0);
    if(readers <= (int*)(0)) {
        perror("Load: shmat failed");
        exit(1);
    }

    return readers;
}
