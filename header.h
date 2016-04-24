#define KEY  ((key_t)(27600))
#define SEGSIZE  sizeof(struct StudentInfo)

#define READ_KEY ((key_t)(827600))
#define READ_SIZE sizeof(int)

#define NUM_SEMAPHS 2
#define SEMA_KEY   ((key_t)(7600))

#define MAX_DATABASE_SIZE 20

typedef enum {
    false,
    true
} bool;

struct StudentInfo {
  char name[30];
  char sID[15];
  char address[200];
  char telNumber[15];
};

void Wait(int semaph, int n);
void Signal(int semaph, int n);
int GetSemaphs(key_t k, int n);

struct StudentInfo* getDatabase(bool create);
int* getReadcount(bool create);
