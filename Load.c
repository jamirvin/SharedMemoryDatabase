#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

struct StudentInfo* getDatabase() {
    int id;
    struct StudentInfo* infoptr;

    // Create the shared memory block big enough to hold 20 database
    // entries.
    id = shmget(KEY, SEGSIZE * 20, IPC_CREAT|0600);
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

int* getReadcount() {
    int id;
    int* readers;

    // Create the shared memory block big enough to hold 20 database
    // entries.
    id = shmget(READ_KEY, READ_SIZE, IPC_CREAT|0600);
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

void crit_section(struct StudentInfo* infoptr, int* num_readers, char* filename) {
    (*num_readers) = 0;
    int is = 1;
    int counter = 0;
    FILE* fp;
    size_t len = 0;
    char* line;
    ssize_t read;

    fp = fopen(filename, "r");
    if(fp == NULL) {
        perror("Load: Database file could not be opened.");
        exit(1);
    }

    while((read = getline(&line, &len, fp)) != -1) {
        int type = counter % 4;

        switch(type) {
            case 0:
                strcpy(infoptr[counter].name, line);
                break;
            case 1:
                strcpy(infoptr[counter].sID, line);
                break;
            case 2:
                strcpy(infoptr[counter].address, line);
                break;
            case 3:
                strcpy(infoptr[counter].telNumber, line);
                break;
        }
        ++counter;
    }

    sleep(10);
}

int main(int argc, char** argv) {

    // Validate input
    if (argc != 2) {
        printf("Usage: %s <input_filename>\n", argv[0]);
        exit(2);
    }

    int i;
    struct StudentInfo* infoptr;
    int sema_set;
    int* num_readers;

    infoptr = getDatabase();
    num_readers = getReadcount();
 
    // Get the set of semaphores
    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);
    if(sema_set < 0) {
        perror("Load: semget failed");
        exit(2);
    }

    Wait(sema_set, 0);
    crit_section(infoptr, num_readers, argv[1]);
    Signal(sema_set, 0);
}
