#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("error argument\n");
    return -1;
  }

  unlink(argv[1]);

  if (mkfifo(argv[1], 0666) == -1) {
    printf("error create pipe\n");
    return -1;
  }

  int fd = open(argv[1], O_WRONLY);

  if (fd == -1) {
    printf("error open file\n");
    return -1;
  }

  char str[4] = "Hi!";

  write(fd, str, sizeof(str));
  close(fd);

  return 0;
}