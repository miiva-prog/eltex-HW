#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    perror("argumets");
    return -1;
  }

  int fd = open(argv[1], O_RDWR);

  if (fd == -1) {
    perror("open");
    return -1;
  }

  char str[10];

  read(fd, str, sizeof(str));
  kill(atoi(str), SIGUSR1);
  close(fd);

  return 0;
}