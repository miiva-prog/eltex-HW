#include "chat.h"

extern WINDOW *win_msg, *win_users;

int main() {
  initscr();
  echo();
  curs_set(0);

  int height, width;

  getmaxyx(stdscr, height, width);

  win_msg = newwin(height - 2, width / 2, 0, 0);
  win_users = newwin(height - 2, width / 2, 0, width / 2);

  scrollok(win_msg, TRUE);
  scrollok(win_users, TRUE);

  msgsend = mq_open(QUEUE_NAME, O_WRONLY);
  msgrecv = mq_open(QUEUE_NAME2, O_RDONLY);

  if (msgsend == -1 || msgrecv == -1) {
    endwin();
    perror("mq_open");
    return -1;
  }

  pthread_t pth_msg;

  pthread_create(&pth_msg, NULL, get_msg, NULL);
  mvprintw(height - 2, 0, "nickname: ");
  getnstr(user_msg.name, SIZE_NAME);

  if (mq_send(msgsend, (char *)&user_msg, sizeof(struct user), 2) == -1) {
    endwin();
    perror("mq_send");
    return -1;
  }

  while (1) {
    move(height - 1, 0);
    clrtoeol();
    printw("Message:");
    getnstr(user_msg.message, MSGSIZE);

    if (mq_send(msgsend, (char *)&user_msg, sizeof(struct user), 1) == -1) {
      endwin();
      perror("mq_send");
      return -1;
    }

    memset(user_msg.message, 0, sizeof(user_msg.message));
  }

  pthread_join(pth_msg, NULL);

  if (mq_close(msgsend) == -1 || mq_close(msgrecv) == -1) {
    perror("mq_close");
    return -1;
  }

  endwin();

  return 0;
}