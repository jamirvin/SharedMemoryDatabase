#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "header.h"

void load_data(struct StudentInfo* infoptr, int* num_readers, char* filename) {
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
        int entry = counter / 4;
        if(line[read-1] == '\n') {
            line[read-1] = '\0';
        }

        switch(type) {
            case 0:
                strcpy(infoptr[entry].name, line);
                //printf("%s\n", infoptr[entry].name);
                break;
            case 1:
                strcpy(infoptr[entry].sID, line);
                //printf("%s\n", infoptr[entry].sID);
                break;
            case 2:
                strcpy(infoptr[entry].address, line);
                //printf("%s\n", infoptr[entry].address);
                break;
            case 3:
                strcpy(infoptr[entry].telNumber, line);
                //printf("%s\n", infoptr[entry].telNumber);
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

    infoptr = getDatabase(true);
    num_readers = getReadcount(true);
 
    // Get the set of semaphores
    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);
    if(sema_set < 0) {
        perror("Load: semget failed");
        exit(2);
    }

    Wait(sema_set, 0);
    load_data(infoptr, num_readers, argv[1]);
    Signal(sema_set, 0);
}
