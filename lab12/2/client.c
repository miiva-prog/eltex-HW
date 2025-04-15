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

  int fd = open(argv[1], O_RDONLY);

  if (fd == -1) {
    printf("error open file\n");
    return -1;
  }

  char str[4];

  read(fd, str, sizeof(str));
  printf("%s\n", str);
  close(fd);
  unlink(argv[1]);

  return 0;
}