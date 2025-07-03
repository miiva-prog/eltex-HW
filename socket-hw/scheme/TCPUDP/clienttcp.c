#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT_HOST 10000
#define PROTOCOL 0
#define IP "127.0.0.1"
#define MAX_PORT 10300
#define MAX_SERVERS 10

int main() {
  struct sockaddr_in server, host;
  int port, fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_HOST);
  server.sin_addr.s_addr = inet_addr(IP);

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect");
    close(fd);
    return -1;
  }

  if (recv(fd, &port, sizeof(port), 0) < 0) {
    perror("recv");
    close(fd);
    return -1;
  }

  printf("port: %d\n", port);
  close(fd);
  sleep(1);

  fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  host.sin_family = AF_INET;
  host.sin_port = htons(port);
  host.sin_addr.s_addr = inet_addr(IP);

  if (connect(fd, (struct sockaddr *)&host, sizeof(host)) == -1) {
    perror("connect");
    close(fd);
    close(fd);
    return -1;
  }

  time_t times;

  for (int n = 0; n < MAX_SERVERS; n++) {
    if (recv(fd, &times, sizeof(times), 0) <= 0)
      break;

    printf("time: %s", ctime(&times));
  }

  close(fd);

  return 0;
}