#include "mc.h"

void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resizeterm(size.ws_row, size.ws_col);
}

void search_file(FILE *file, int index, char *result, int size) {
  rewind(file);

  char arr[SIZE_NAME];

  for (int i = 0; i <= index; i++)
    if (fgets(arr, size, file) == NULL) {
      arr[0] = '\0';
      break;
    }

  int index_res = strlen(result) - 1;

  if (strcmp(arr, "..\n") != 0) {
    arr[strlen(arr) - 1] = '/';

    strcat(result, arr);
  } else {
    result[index_res] = '\0';

    while (result[index_res] != '/')
      result[index_res--] = '\0';
  }
}

size_t write_file(FILE *file, char *result) {
  size_t count_str = 0;
  char arr[SIZE_FILE];

  while (fgets(arr, sizeof(arr), file) != NULL) {
    strcat(result, arr);
    count_str++;

    if (count_str > COUNT_STR)
      break;
  }

  return count_str;
}