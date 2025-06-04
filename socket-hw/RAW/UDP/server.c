#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9000
#define PROTOCOL 0
#define SIZE 1024

int main() {
  struct sockaddr_in server, client;
  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  socklen_t len = sizeof(client);
  char buffer[SIZE];

  while (1) {
    int recv = recvfrom(fd, buffer, SIZE, 0, (struct sockaddr *)&client, &len);

    if (recv < 0) {
      perror("recvfrom");
      close(fd);
      return -1;
    }

    buffer[recv] = '\0';

    printf("%s\n", buffer);

    buffer[0] = '6';

    sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client,
           sizeof(client));
  }

  close(fd);

  return 0;
}