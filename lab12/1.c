#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char str[4];
  int fd[2];

  pipe(fd);

  int pid = fork();

  if (pid < 0) {
    exit(1);
  } else if (pid == 0) {
    close(fd[1]);
    read(fd[0], str, sizeof(str));
    printf("%s\n", str);
    exit(0);
  } else {
    close(fd[0]);
    write(fd[1], "Hi!", sizeof("Hi!"));
    wait();
  }

  return 0;
}