#include "chat.h"

WINDOW *win_msg, *win_users;
struct user user_msg;
struct chatusers *chat;
sem_t *sem;

void *get_msg();

int main() {
  initscr();
  echo();
  curs_set(0);

  int height, width;

  getmaxyx(stdscr, height, width);

  win_msg = newwin(height - 3, width / 2, 0, 0);
  win_users = newwin(height - 3, width / 2, 0, width / 2);

  scrollok(win_msg, TRUE);
  scrollok(win_users, TRUE);

  int shmid = shm_open(NAME_SHM, O_RDWR, 0666);

  if (shmid < 0) {
    perror("shm_open");
    return -1;
  }

  chat = mmap(NULL, sizeof(struct chatusers), PROT_READ | PROT_WRITE,
              MAP_SHARED, shmid, 0);

  if (chat == MAP_FAILED) {
    perror("mmap");
    return -1;
  }

  sem = sem_open(NAME_SEM, 0);

  if (sem == SEM_FAILED) {
    perror("sem_open");
    return -1;
  }

  pthread_t pth_msg;

  pthread_create(&pth_msg, NULL, get_msg, NULL);
  move(height - 3, 0);
  clrtoeol();
  printw("Nickname:");
  getnstr(user_msg.name, SIZE_NAME);
  sprintf(chat->names[chat->client_count], "%s", user_msg.name);

  chat->client_count++;

  sleep(1);

  while (1) {
    move(height - 2, 0);
    clrtoeol();
    printw("Message:");
    getnstr(user_msg.message, MSGSIZE);

    if (strlen(user_msg.message) == 0)
      continue;

    if (chat->count_message < COUNT_USERS) {
      sprintf(chat->users[chat->count_message].name, "%s", user_msg.name);
      sprintf(chat->users[chat->count_message].message, "%s", user_msg.message);
      chat->count_message++;
    } else {
      chat->count_message = 0;
    }

    for (int n = 0; n < chat->client_count; n++)
      sem_post(sem);

    memset(user_msg.message, 0, sizeof(user_msg.message));
  }

  curs_set(1);
  pthread_join(pth_msg, NULL);
  endwin();

  return 0;
}

void *get_msg() {
  sleep(1);

  while (1) {
    sem_wait(sem);
    werase(win_msg);
    werase(win_users);
    mvwprintw(win_msg, 0, 0, "Message");
    mvwprintw(win_users, 0, 0, "User");

    for (int n = 0; n < chat->count_message; n++)
      if (strlen(chat->users[n].name) > 0 && strlen(chat->users[n].message) > 0)
        mvwprintw(win_msg, n + 1, 0, "(%s):%s", chat->users[n].name,
                  chat->users[n].message);

    for (int n = 0; n < chat->client_count; n++)
      if (strlen(chat->names[n]) > 0)
        mvwprintw(win_users, n + 1, 0, "[%s]", chat->names[n]);

    wrefresh(win_msg);
    wrefresh(win_users);
  }

  return NULL;
}