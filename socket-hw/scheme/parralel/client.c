#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT_HOST 10000
#define IP "127.0.0.1"
#define PROTOCOL 0

int main() {
  struct sockaddr_in server;
  int port, fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_HOST);
  inet_pton(AF_INET, IP, &server.sin_addr);

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return -1;
  }

  if (recv(fd, &port, sizeof(port), 0) <= 0) {
    perror("recv");
    return -1;
  }

  close(fd);

  if (port == 0) {
    printf("out of range server\n");
    return 0;
  }

  printf("port: %d\n", port);

  fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_port = htons(port);

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return -1;
  }

  time_t mytime;

  recv(fd, &mytime, sizeof(mytime), 0);

  struct tm *now = localtime(&mytime);

  printf("time: %02d:%02d:%02d\n", now->tm_hour, now->tm_min, now->tm_sec);
  close(fd);

  return 0;
}