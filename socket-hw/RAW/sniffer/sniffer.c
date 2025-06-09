#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SIZE 1024

int main() {
  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  char buffer[SIZE];

  while (1)
    if (recv(fd, buffer, SIZE, 0) < 0) {
      perror("recv");
      close(fd);
      return -1;
    } else {
      printf("%s\n", buffer);
    }

  close(fd);

  return 0;
}