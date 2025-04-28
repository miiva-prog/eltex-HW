#include <fcntl.h>
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define NAME_SHM "/Shmname"
#define NAME_SEM "/Semname"
#define MSGSIZE 256
#define SIZE_NAME 20
#define COUNT_USERS 10

struct user {
  char message[MSGSIZE];
  char name[SIZE_NAME];
};

struct chatusers {
  struct user users[COUNT_USERS];
  int client_count;
  int count_message;
  char names[COUNT_USERS][SIZE_NAME];
};