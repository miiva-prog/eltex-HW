#include "chat.h"

struct chatusers chat;
struct user user_msg;
mqd_t msgsend, msgrecv;
WINDOW *win_msg, *win_users;

void *get_msg() {
  while (1) {
    if (mq_receive(msgrecv, (char *)&chat, sizeof(struct chatusers), NULL) ==
        -1) {
      mvprintw(0, 0, "error recv\n");
      refresh();
      return NULL;
    }

    werase(win_msg);
    werase(win_users);
    mvwprintw(win_msg, 0, 0, "Message");
    mvwprintw(win_users, 0, 0, "User");

    for (int n = 1; n < chat.count_message + 1; n++)
      mvwprintw(win_msg, n, 0, "(%s):%s", chat.users[n - 1].name,
                chat.users[n - 1].message);

    for (int n = 1; n < chat.client_count + 1; n++)
      mvwprintw(win_users, n, 0, "[%s]", chat.names[n - 1]);

    wrefresh(win_msg);
    wrefresh(win_users);
  }

  return NULL;
}