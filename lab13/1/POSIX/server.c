#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>
#define QUEUE_NAME "/mqueue"
#define QUEUE_NAME2 "/mqueue2"
#define MSGSIZE 256

int main() {
  struct mq_attr msg_attr;

  msg_attr.mq_flags = 0;
  msg_attr.mq_maxmsg = 2;
  msg_attr.mq_msgsize = MSGSIZE;
  msg_attr.mq_curmsgs = 0;

  mqd_t msgsend = mq_open(QUEUE_NAME2, O_CREAT | O_RDWR, 0600, &msg_attr);

  if (msgsend == -1) {
    printf("error create\n");
    return -1;
  }

  int send = mq_send(msgsend, "Hi!", sizeof("Hi!"), 1);

  if (send == -1) {
    printf("error send\n");
    return -1;
  }

  mqd_t msrecv = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &msg_attr);

  if (msrecv == -1) {
    printf("error create\n");
    return -1;
  }

  char str[MSGSIZE];

  ssize_t recv = mq_receive(msrecv, str, sizeof(str), NULL);

  if (recv == -1) {
    printf("error recv\n");
    return -1;
  }

  printf("%s\n", str);

  if (mq_close(msgsend) == -1 || mq_close(msrecv) == -1) {
    printf("error close\n");
    return -1;
  }

  if (mq_unlink(QUEUE_NAME2) == -1 || mq_unlink(QUEUE_NAME) == -1) {
    printf("error unlink\n");
    return -1;
  }

  return 0;
}