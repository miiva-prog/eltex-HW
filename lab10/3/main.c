#include "bash.h"

int main() {
  char *command[50], path[25] = "/";

  while (1) {
    printf("> ");

    int index = 0;
    char fill, arr[25];

    while ((fill = getchar()) != '\n')
      arr[index++] = fill;

    arr[index] = '\0';

    if (strcmp("exit", arr) == 0) {
      break;
    } else if (strcmp("clear", arr) == 0) {
      index = search_arguments(arr, command);
    } else {
      search_path(arr, path);

      index = search_arguments(arr, command);

      if (index > 1 || strcmp("ls", command[0]) == 0) {
        command[index] = path;
        command[index + 1] = NULL;
      }
    }

    int pid = fork();

    if (pid < 0) {
      exit(1);
    } else if (pid == 0) {
      execvp(command[0], command);
      exit(0);
    }

    wait();
  }

  return 0;
}