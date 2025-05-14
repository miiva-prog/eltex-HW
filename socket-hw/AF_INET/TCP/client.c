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
  int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(HOST);
  server.sin_port = htons(PORT);

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect");
    close(fd);
    return -1;
  }

  char buffer[SIZE];

  sprintf(buffer, "%d", rand() % 100);
  send(fd, buffer, sizeof(buffer), 0);

  int recvied = recv(fd, buffer, SIZE, 0);

  if (recvied < 0) {
    perror("recv");
    close(fd);
    return -1;
  }

  buffer[recvied] = '\0';

  printf("%s\n", buffer);
  close(fd);

  return 0;
}