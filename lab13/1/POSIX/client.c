#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/msg.h>
#define QUEUE_NAME "/mqueue"
#define QUEUE_NAME2 "/mqueue2"
#define MSGSIZE 256

int main() {
  mqd_t msgrecv = mq_open(QUEUE_NAME2, O_RDWR);

  if (msgrecv == -1) {
    printf("error open\n");
    return -1;
  }

  char str[MSGSIZE];

  ssize_t recv = mq_receive(msgrecv, str, sizeof(str), NULL);

  if (recv == -1) {
    printf("error recv\n");
    return -1;
  }

  printf("%s\n", str);

  mqd_t msgsend = mq_open(QUEUE_NAME, O_RDWR);

  int send = mq_send(msgsend, "Hello!", sizeof("Hello!"), 1);

  if (send == -1) {
    printf("error send\n");
    return -1;
  }

  if (mq_close(msgrecv) == -1 || mq_close(msgsend) == -1) {
    printf("error close\n");
    return -1;
  }

  return 0;
}