#include "chat.h"

int main() {
  struct mq_attr msg_attr_recv, msg_attr_send;

  msg_attr_recv.mq_flags = 0;
  msg_attr_recv.mq_maxmsg = COUNT_USERS;
  msg_attr_recv.mq_msgsize = sizeof(struct user);
  msg_attr_recv.mq_curmsgs = 0;
  msg_attr_send.mq_flags = 0;
  msg_attr_send.mq_maxmsg = COUNT_USERS;
  msg_attr_send.mq_msgsize = sizeof(struct chatusers);
  msg_attr_send.mq_curmsgs = 0;

  mq_unlink(QUEUE_NAME);
  mq_unlink(QUEUE_NAME2);

  mqd_t msgrec = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0600, &msg_attr_recv);
  mqd_t msgsen = mq_open(QUEUE_NAME2, O_CREAT | O_WRONLY, 0600, &msg_attr_send);

  if (msgrec == -1 || msgsen == -1) {
    perror("mq_open");
    return -1;
  }

  struct user user_msg;
  int priority;

  while (1) {
    if (mq_receive(msgrec, (char *)&user_msg, sizeof(struct user), &priority) ==
        -1) {
      perror("mq_receive");
      return -1;
    }

    if (priority == 2) {
      if (chat.client_count < COUNT_USERS) {
        sprintf(chat.names[chat.client_count], "%s", user_msg.name);

        chat.client_count++;
      }
    } else if (priority == 1) {
      if (chat.count_message >= COUNT_USERS)
        chat.count_message = 0;

      sprintf(chat.users[chat.count_message].name, "%s", user_msg.name);
      sprintf(chat.users[chat.count_message].message, "%s", user_msg.message);

      chat.count_message++;
    }

    for (int n = 0; n < chat.client_count; n++)
      if (mq_send(msgsen, (char *)&chat, sizeof(struct chatusers), 1) == -1) {
        perror("mq_send");
        return -1;
      }
  }

  if (mq_close(msgrec) == -1 || mq_close(msgsen) == -1) {
    perror("mq_close");
    return -1;
  }

  if (mq_unlink(QUEUE_NAME) == -1 || mq_unlink(QUEUE_NAME2) == -1) {
    perror("mq_unlink");
    return -1;
  }

  return 0;
}