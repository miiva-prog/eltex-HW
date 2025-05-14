#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define PROTOCOL 0
#define PORT 10000
#define SIZE 1024

int main() {
  srand(time(NULL));

  struct sockaddr_in server;
  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(HOST);
  server.sin_port = htons(PORT);

  char buffer[SIZE];
  socklen_t len = sizeof(server);

  sprintf(buffer, "%d", rand() % 100);
  sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server,
         sizeof(server));

  int recv = recvfrom(fd, buffer, SIZE, 0, (struct sockaddr *)&server, &len);

  if (recv < 0) {
    perror("recvfrom");
    close(fd);
    return -1;
  }

  buffer[recv] = '\0';

  printf("%s\n", buffer);
  close(fd);

  return 0;
}