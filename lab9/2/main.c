#include <signal.h>
#include <stdlib.h>
#include <termios.h>

#include "mc.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("error arguments\n");
    return -1;
  }

  FILE *file = fopen(argv[1], "w");

  if (file == NULL) {
    printf("can't open file\n");
    return -1;
  }

  char name_file[SIZE_NAME] = "/", name_file2[SIZE_NAME] = "/";
  size_t count_dir = 0, count_dir2 = 0;
  WINDOW *wnd, *subwnd, *subwnd2;
  char arr_file[SIZE_FILE];
  int ch;

  sprintf(arr_file, "ls -a / > %s | ls -a / > %s", argv[1], argv[2]);
  system(arr_file);
  memset(arr_file, 0, sizeof(arr_file));
  fclose(file);

  file = fopen(argv[1], "r");

  if (file == NULL) {
    printf("can't open file\n");
    return -1;
  }

  count_dir = count_dir2 = write_file(file, arr_file);

  fclose(file);
  initscr();
  signal(SIGWINCH, sig_winch);
  start_color();
  refresh();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_RED);

  wnd = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);

  wattron(wnd, COLOR_PAIR(2));
  box(wnd, '|', '-');

  subwnd = derwin(wnd, getmaxy(stdscr) - 1, getmaxx(stdscr) / 2 - 1, 1, 1);
  subwnd2 = derwin(wnd, getmaxy(stdscr) - 1, getmaxx(stdscr) / 2 - 1, 1,
                   getmaxx(stdscr) / 2);

  wbkgd(subwnd, COLOR_PAIR(1));
  wattron(subwnd, A_BOLD);
  wbkgd(subwnd2, COLOR_PAIR(1));
  wattron(subwnd2, A_BOLD);
  wprintw(subwnd, arr_file);
  wprintw(subwnd2, arr_file);

  size_t x = 1, y = 1;

  noecho();
  curs_set(1);
  keypad(stdscr, TRUE);
  wmove(wnd, y, x);

  while ((ch = getch()) != 'q') {
    switch (ch) {
    case KEY_DOWN:
      if (x == 1) {
        if (y < count_dir && y < COUNT_STR)
          y++;
      } else {
        if (y < count_dir2 && y < COUNT_STR)
          y++;
      }
      break;
    case KEY_UP:
      if (y > 1)
        y--;
      break;
    case '\t':
      if (x == 1)
        x = getmaxx(stdscr) / 2;
      else
        x = 1;

      y = 1;
      break;
    case '\n':
      if (x == 1) {
        wclear(subwnd);
        wrefresh(subwnd);
      } else {
        wclear(subwnd2);
        wrefresh(subwnd2);
      }

      char command[100], str_name[SIZE_NAME];

      if (x == 1)
        strcpy(str_name, argv[1]);
      else
        strcpy(str_name, argv[2]);

      file = fopen(str_name, "r");

      if (file == NULL) {
        printf("can't open file\n");
        return -1;
      }

      if (x == 1)
        search_file(file, y - 1, name_file, sizeof(name_file));
      else
        search_file(file, y - 1, name_file2, sizeof(name_file2));

      fclose(file);

      file = fopen(str_name, "w");

      if (file == NULL) {
        printf("can't open file\n");
        return -1;
      }

      if (x == 1)
        sprintf(command, "(ls -a %s) > %s", name_file, str_name);
      else
        sprintf(command, "(ls -a %s) > %s", name_file2, str_name);

      system(command);
      fclose(file);

      file = fopen(str_name, "r");

      if (file == NULL) {
        printf("can't open file\n");
        return -1;
      }

      memset(arr_file, 0, sizeof(arr_file));

      if (x == 1)
        count_dir = write_file(file, arr_file);
      else
        count_dir2 = write_file(file, arr_file);

      fclose(file);

      if (x == 1)
        wprintw(subwnd, arr_file);
      else
        wprintw(subwnd2, arr_file);

      x = (x != 1) ? getmaxx(stdscr) / 2 : 1;
      y = 1;
    default:
      break;
    }
    wmove(wnd, y, x);
    box(wnd, '|', '-');
    wrefresh(wnd);
  }

  wrefresh(subwnd);
  wrefresh(subwnd2);
  delwin(subwnd);
  delwin(subwnd2);
  delwin(wnd);
  refresh();
  getch();
  endwin();
  exit(EXIT_FAILURE);
}