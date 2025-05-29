#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 10000
#define PROTOCOL 0
#define SIZE 1024
#define IP "255.255.255.255"

int main() {
  struct sockaddr_in server, client;
  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&server, 0, sizeof(server));

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (inet_pton(AF_INET, IP, &server.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return -1;
  }

  if (bind(fd, (struct sockaddr_in *)&server, sizeof(server)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  char buffer[SIZE];
  socklen_t len = sizeof(client);

  if (recvfrom(fd, buffer, SIZE, 0, (struct sockaddr_in *)&client, &len) < 0) {
    perror("recvfrom");
    close(fd);
    return -1;
  }

  printf("%s\n", buffer);
  close(fd);

  return 0;
}