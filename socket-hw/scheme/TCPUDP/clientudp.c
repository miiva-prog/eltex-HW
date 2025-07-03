#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT_HOST 10001
#define PROTOCOL 0
#define IP "127.0.0.1"
#define MAX_PORT 10300
#define MAX_SERVERS 10

int main() {
  struct sockaddr_in server;
  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT_HOST);
  server.sin_addr.s_addr = inet_addr(IP);

  char buffer[] = "DD";

  sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server,
         sizeof(server));

  time_t times;

  for (int n = 0; n < MAX_SERVERS; n++) {
    if (recv(fd, &times, sizeof(times), 0) <= 0)
      break;

    printf("time: %s", ctime(&times));
  }

  close(fd);

  return 0;
}