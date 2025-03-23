#include "bash.h"

int main() {
  char command[50], path[25] = "/";

  while (1) {
    memset(command, 0, sizeof(command));
    printf("> ");

    int index = 0;
    char fill, arr[25];

    while ((fill = getchar()) != '\n')
      arr[index++] = fill;

    arr[index] = '\0';

    if (strcmp("exit", arr) == 0) {
      break;
    } else if (strcmp("clear", arr) == 0) {
      sprintf(command, "%s", arr);
    } else {
      search_path(arr, path);
      sprintf(command, "%s %s", arr, path);
    }

    int pid = fork();

    if (pid < 0) {
      exit(1);
    } else if (pid == 0) {
      system(command);
      exit(0);
    }

    wait();
  }

  return 0;
}