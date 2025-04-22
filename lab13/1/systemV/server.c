#include <stdio.h>
#include <sys/msg.h>
#define KEY 10
#define FLAG_CREATE IPC_CREAT | 0666
#define MSGSIZE 128

struct msgbuf {
  long mtype;
  char mtext[MSGSIZE];
};

int main() {

  int msid = msgget(KEY, FLAG_CREATE);

  if (msid < 0) {
    printf("error create message\n");
    return -1;
  }

  struct msgbuf send_buf, recv_buf;

  send_buf.mtype = 1;

  sprintf(send_buf.mtext, "%s", "Hi!");

  size_t len_msg = sizeof(send_buf.mtext);
  int send = msgsnd(msid, &send_buf, len_msg, IPC_NOWAIT);

  if (send < 0) {
    printf("error send message\n");
    return -1;
  }

  int recv = msgrcv(msid, &recv_buf, MSGSIZE, 1, 0);

  if (recv < 0) {
    printf("error recv message\n");
    return -1;
  }

  printf("%s\n", recv_buf.mtext);

  struct msqid_ds struct_msg;

  if (msgctl(msid, IPC_RMID, &struct_msg) < 0) {
    printf("error delete");
    return -1;
  }

  return 0;
}