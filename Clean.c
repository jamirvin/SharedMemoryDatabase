#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pwd.h>
#include "header.h"

void output_database(struct StudentInfo* infoptr, char* outfile) {
    int i = 0;
    FILE* fp = fopen(outfile, "w");
    if(fp == NULL) {
        perror("Clean: Output file could not be opened.");
        exit(1);
    }
    
    // Print each item in the database until an item is reached
    // that is empty, or until the end of the database is reached
    while(strcmp(infoptr[i].name, "") && i < MAX_DATABASE_SIZE) {
        
        fprintf(fp, "%s\n", infoptr[i].name);
        fprintf(fp, "%s\n", infoptr[i].sID);
        fprintf(fp, "%s\n", infoptr[i].address);
        fprintf(fp, "%s\n", infoptr[i].telNumber);
            
        ++i;
    } 

    fclose(fp);
    sleep(10); // For debugging and testing purposes
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <Output File>\n", argv[0]);
        exit(2);
    }

    // Load database and semaphores from shared memory
    struct StudentInfo* infoptr = getDatabase(false);
    // Get ids for releasing
    int semaset = semget(SEMA_KEY, 0, 0);
    int id = shmget(KEY, SEGSIZE * MAX_DATABASE_SIZE, 0);
    int reads_id = shmget(READ_KEY, READ_SIZE, 0);

    Wait(semaset, 0);

    // Update student record with the new data
    output_database(infoptr, argv[1]);
    shmdt((char  *)infoptr); /* detach the shared memory segment */
    shmctl(id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
    shmctl(reads_id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
    semctl(semaset,0,IPC_RMID); /*Remove the semaphore set */

    Signal(semaset, 0);
}
