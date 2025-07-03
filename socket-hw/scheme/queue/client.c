#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT_HOST 10000
#define PROTOCOL 0
#define IP "127.0.0.1"
#define MAX_PORT 10300

int main() {
  struct sockaddr_in server, client;
  int port = 10001, fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_HOST);

  if (inet_pton(AF_INET, IP, &server.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return -1;
  }

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("connect");
    return -1;
  }

  int fd_client = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd_client < 0) {
    perror("socket");
    close(fd);
    return -1;
  }

  client.sin_family = AF_INET;

  if (inet_pton(AF_INET, IP, &client.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    close(fd_client);
    return -1;
  }

  while (1) {
    client.sin_port = htons(port);

    if (bind(fd_client, (struct sockaddr *)&client, sizeof(client)) == 0)
      break;

    port++;

    if (port > MAX_PORT) {
      printf("out of range port\n");
      return -1;
    }
  }

  if (listen(fd_client, 1) < 0) {
    perror("listen");
    close(fd_client);
    close(fd);
    return -1;
  }

  send(fd, &port, sizeof(port), 0);
  close(fd);
  printf("port: %d\n", port);

  int fd_accept = accept(fd_client, NULL, NULL);

  if (fd_accept < 0) {
    perror("accept");
    return -1;
  }

  time_t times;

  if (recv(fd_accept, &times, sizeof(times), 0) < 0) {
    perror("recv");
    close(fd_accept);
    return -1;
  }

  struct tm *time_now = localtime(&times);

  printf("time: %02d:%02d:%02d\n", time_now->tm_hour, time_now->tm_min,
         time_now->tm_sec);
  close(fd_client);
  close(fd_accept);

  return 0;
}