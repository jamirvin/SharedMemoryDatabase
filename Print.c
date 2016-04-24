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

void output_database(struct StudentInfo* infoptr) {
    int i = 0;
    
    // Output formatted header
    printf("%-20s %-12s %-12s %s\n", "Name:", "Student ID:", "Phone:", "Address:");

    // Print each item in the database until an item is reached
    // that is empty, or until the end of the database is reached
    while(strcmp(infoptr[i].name, "") && i < MAX_DATABASE_SIZE) {
        printf("%-20s %-12s %-12s %s\n",
             infoptr[i].name, infoptr[i].sID, infoptr[i].telNumber, infoptr[i].address);
        ++i;
    } 

    sleep(10); // For debugging and testing purposes
}

int main() {

    // Load database and semaphores from shared memory
    struct StudentInfo* infoptr = getDatabase(false);
    int* num_readers = getReadcount(false);
    int semaset = semget(SEMA_KEY, 0, 0);

    // Wait on semaphores to ensure there are no other
    // writers to num_readers and that there are no active
    // writers to the database
    Wait(semaset, 1);
    (*num_readers)++;
    if( (*num_readers) == 1) {
        Wait(semaset, 0);
    }
    Signal(semaset, 1);

    // Print the database to the screen
    output_database(infoptr);

    // Decrement num_readers and if this was the last reader,
    // signal that the database is free
    Wait(semaset, 1);
    (*num_readers)--;
    if( (*num_readers) == 0) {
        Signal(semaset, 0);
    }
    Signal(semaset, 1);
}
