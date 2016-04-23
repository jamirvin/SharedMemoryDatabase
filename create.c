#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

main()
{
  int i,id;
  struct StudentInfo *infoptr;
  int sema_set;

  id = shmget(KEY, SEGSIZE,IPC_CREAT|0666);/* get shared memory to store data*/
  if (id <0){
    perror("create: shmget failed");
    exit(1);
}


  infoptr=(struct StudentInfo *)shmat(id,0,0);/*attach the shared memory segment
                       to the process's address space */
  if (infoptr <= (struct StudentInfo *) (0)) {
    perror("create: shmat failed");
    exit(2);
}

  sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS
                           semaphores*/
  if ((sema_set < 0) ){
    perror("create: semget failed");
    exit(2);
  }
  
/* store data in the shared memory segment */  
  strcpy(infoptr->fName,"Joe");
  strcpy(infoptr->lName, "Smith");
  strcpy(infoptr->telNumber, "606-111-2222");
  strcpy(infoptr->whoModified, " ");

 
/*print the contents of the shared memory segment */ 

  for(i=0;i<10;i++)
    {
      printf("the value of sema_set=%d\n", sema_set);
      Wait(sema_set,1); 
      printf("Name: %s %s\nPhone Number: %s\n",
         infoptr->fName,infoptr->lName,infoptr->telNumber);
      printf("Last modified by: %s\n \n ", infoptr->whoModified);
      sleep(2);
      Signal(sema_set,1);
  }

  shmdt((char  *)infoptr); /* detach the shared memory segment */
  shmctl(id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
  semctl(sema_set,0,IPC_RMID); /*Remove the semaphore set */
  exit(0);
}
