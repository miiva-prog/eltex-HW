#include "bash.h"

int main() {
  char *command[10], *command2[10], path[25] = "/";

  while (1) {
    printf("> ");

    int index = 0, fd[2] = {0};
    char fill, arr[25];

    while ((fill = getchar()) != '\n')
      arr[index++] = fill;

    arr[index] = '\0';

    if (strcmp("exit", arr) == 0) {
      break;
    } else if (strcmp("clear", arr) == 0) {
      index = search_arguments(arr, command, command2);
    } else {
      if (strchr(arr, '|') != NULL)
        pipe(fd);

      search_path(arr, path);

      index = search_arguments(arr, command, command2);

      if (index > 1 || strcmp("ls", command[0]) == 0) {
        command[index] = path;
        command[index + 1] = NULL;
      }
    }

    int pid = fork();

    if (pid < 0) {
      exit(1);
    } else if (pid == 0) {
      dup2(fd[1], 1);
      execvp(command[0], command);
      exit(0);
    }

    if (fd[0] != 0) {
      int pid2 = fork();

      if (pid2 < 0) {
        exit(1);
      } else if (pid2 == 0) {
        dup2(fd[0], 0);
        execvp(command2[0], command2);
        exit(0);
      }
    }

    wait();
  }

  return 0;
}