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

void update_student(struct StudentInfo* infoptr, struct StudentInfo new) {
    int i = 0;
    bool found = false;
    
    // Print each item in the database until an item is reached
    // that is empty, or until the end of the database is reached
    while(strcmp(infoptr[i].name, "") && i < MAX_DATABASE_SIZE) {
        if(!strcmp(infoptr[i].sID, new.sID)) {

            strcpy(infoptr[i].name, new.name); 
            strcpy(infoptr[i].telNumber, new.telNumber);
            strcpy(infoptr[i].address, new.address);
            
            found = true;
        }
        ++i;
    } 

    if(!found) 
        printf("Record not found\n");
    else
        printf("Student record updated\n");

    sleep(10); // For debugging and testing purposes
}

void getPassword() {
    char password[50];

    printf("Enter advisor password: ");
    fgets(password, 50, stdin);

    if(!strcmp(password, "000")) {
        printf("Password Incorrect");
        exit(0);
    }
}

int main(int argc, char** argv) {
    if(argc != 5) {
        printf("Usage: %s <Student ID> <New Name> <New Phone Number> <New Address>\n", argv[0]);
        printf("Use quotes around entries that are more than one word");
        exit(2);
    }

    // Create a new StudentInfo structure to hold the new info
    struct StudentInfo new;
    strcpy(new.sID, argv[1]);
    strcpy(new.name, argv[2]);
    strcpy(new.telNumber, argv[3]);
    strcpy(new.address, argv[4]);

    getPassword();

    // Load database and semaphores from shared memory
    struct StudentInfo* infoptr = getDatabase(false);
    int* num_readers = getReadcount(false);
    int semaset = semget(SEMA_KEY, 0, 0);

    Wait(semaset, 0);

    // Update student record with the new data
    update_student(infoptr, new);

    Signal(semaset, 0);
}
