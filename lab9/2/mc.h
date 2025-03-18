#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#define COUNT_STR 43
#define SIZE_FILE 255
#define SIZE_NAME 40

void sig_winch(int signo);

void search_file(FILE *file, int index, char *result, int size);

size_t write_file(FILE *file, char *result);