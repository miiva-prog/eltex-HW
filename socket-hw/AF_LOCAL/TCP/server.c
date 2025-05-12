#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define PATH "/tmp/server.socket"
#define MAX_CLIENT 3
#define PROTOCOL 0
#define SIZE 1024

int main() {
  struct sockaddr_un server, client;
  int fd = socket(AF_LOCAL, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  unlink(PATH);
  memset(&server, 0, sizeof(server));
  strcpy(server.sun_path, PATH);

  server.sun_family = AF_LOCAL;

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  if (listen(fd, MAX_CLIENT) == -1) {
    perror("listen");
    close(fd);
    return -1;
  }

  socklen_t len = sizeof(client);
  char buffer[SIZE];

  while (1) {
    int fdclient = accept(fd, (struct sockaddr *)&client, &len);

    if (fdclient == -1) {
      perror("accept");
      close(fd);
      return -1;
    }

    int recviced = recv(fdclient, buffer, sizeof(buffer), 0);

    if (recviced < 0) {
      perror("recv");
      close(fd);
      return -1;
    }

    buffer[recviced] = '\0';

    printf("%s\n", buffer);
    sprintf(buffer, "%d", atoi(buffer) + 1);
    send(fdclient, buffer, sizeof(buffer), 0);
  }

  close(fd);
  unlink(PATH);

  return 0;
}