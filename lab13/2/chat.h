#include <fcntl.h>
#include <mqueue.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_NAME "/mqueue"
#define QUEUE_NAME2 "/mqueue2"
#define MSGSIZE 256
#define SIZE_NAME 20
#define COUNT_USERS 10

struct user {
  char message[MSGSIZE];
  char name[SIZE_NAME];
};

struct chatusers {
  struct user users[COUNT_USERS];
  char names[COUNT_USERS][SIZE_NAME];
  int client_count;
  int count_message;
};

extern struct chatusers chat;
extern struct user user_msg;
extern mqd_t msgsend, msgrecv;

void *get_msg();