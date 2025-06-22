#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IP "255.255.255.255"
#define PORT 10000
#define PROTOCOL 0
#define SIZE 1024

int main() {
  struct sockaddr_in server;
  int flag = 1, fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    close(fd);
    return -1;
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (inet_pton(AF_INET, MULTI_IP, &server.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return -1;
  }

  char buffer[SIZE] = "DD";

  sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr_in *)&server,
         sizeof(server));
  close(fd);

  return 0;
}