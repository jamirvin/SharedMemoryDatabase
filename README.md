# Shared Memory Database

## Description:

Tools for creating and managing a small database in shared memory 
loaded with student information.  The database can be initialized 
with a text file in the format given by sampledata.txt. Contains 
five different programs which are used for handling different 
operations. Each program enforces mutual exclusion, where there can
 be no other operations while one process is writing to the database. 
An unlimited number of readers are allowed. 

## Included Programs:

Load: Create the database in shared memory. Loads it with data in the given datafile.
Usage: `Load <data_file>`

Print: Prints the database to the screen.
Usage: `Print`

Query: Query the database for a given student ID and print the student record with that ID if found.
Usage: `Query <student_id>`

Change: Change a student record given the student's ID and their new information.
Usage: `Change <student_id> <student_name> <phone_number> <address>`
Note: Any fields with multiple words should be in quotes.

Clean: Output the database to a file in a format that can be read back in with the Load command. Destroys the shared memory segment and semaphores created.
Usage: `Clean <output_file>`

## Build Instructions:

Use included makefile. Run `make` to build all files. Each file can be specified to be built specifically as well.

## Notes:

Created to test using shared memory and semaphores to enforce mutual exclusion. The critical section of each program includes a 10 second wait for testing with the other programs to ensure mutual exclusion is enforced. 
