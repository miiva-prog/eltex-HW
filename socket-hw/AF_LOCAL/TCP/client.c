#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>

#define PATH_SERVER "/tmp/server.socket"
#define PATH_CLIENT "/tmp/client.socket"
#define PROTOCOL 0
#define SIZE 1024

int main() {
  srand(time(NULL));

  struct sockaddr_un server, client;
  int fd = socket(AF_LOCAL, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  unlink(PATH_CLIENT);
  memset(&client, 0, sizeof(client));
  strcpy(client.sun_path, PATH_CLIENT);

  client.sun_family = AF_LOCAL;

  if (bind(fd, (struct sockaddr *)&client, sizeof(client)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  memset(&server, 0, sizeof(server));
  strcpy(server.sun_path, PATH_SERVER);

  server.sun_family = AF_LOCAL;

  if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect");
    close(fd);
    return -1;
  }

  char buffer[SIZE];

  sprintf(buffer, "%d", rand() % 100);
  send(fd, buffer, sizeof(buffer), 0);

  int recviced = recv(fd, buffer, sizeof(buffer), 0);

  if (recviced < 0) {
    perror("recv");
    close(fd);
    return -1;
  }

  buffer[recviced] = '\0';

  printf("%s\n", buffer);
  close(fd);
  unlink(PATH_CLIENT);

  return 0;
}