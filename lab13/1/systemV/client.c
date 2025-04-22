#include <stdio.h>
#include <sys/msg.h>
#define KEY 10
#define FLAG_CONNECT 0
#define MSGSIZE 128

struct msgbuf {
  long mtype;
  char mtext[MSGSIZE];
};

int main() {

  int msid = msgget(KEY, FLAG_CONNECT);

  if (msid < 0) {
    printf("error create message\n");
    return -1;
  }

  struct msgbuf send_buf, recv_buf;
  size_t len_msg = sizeof(send_buf.mtext);
  int recv = msgrcv(msid, &recv_buf, MSGSIZE, 1, 0);

  if (recv < 0) {
    printf("error recv message\n");
    return -1;
  }

  printf("%s\n", recv_buf.mtext);

  send_buf.mtype = 1;

  sprintf(send_buf.mtext, "%s", "Hello!");

  int send = msgsnd(msid, &send_buf, len_msg, IPC_NOWAIT);

  if (send < 0) {
    printf("error send message\n");
    return -1;
  }

  return 0;
}