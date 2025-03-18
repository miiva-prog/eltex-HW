#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  char command[50];

  while (1) {
    memset(command, 0, sizeof(command));
    printf("> ");

    int index = 0;
    char fill;

    while ((fill = getchar()) != '\n')
      command[index++] = fill;

    if (strcmp("exit", command) == 0)
      break;

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