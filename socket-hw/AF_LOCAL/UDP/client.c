#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
  int fd = socket(AF_LOCAL, SOCK_DGRAM, PROTOCOL);

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
  server.sun_family = AF_LOCAL;
  strcpy(server.sun_path, PATH_SERVER);

  socklen_t len = sizeof(server);
  char buffer[SIZE];

  sprintf(buffer, "%d", rand() % 100);
  sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server,
         sizeof(server));

  int recv = recvfrom(fd, buffer, SIZE, 0, (struct sockaddr *)&server, &len);

  if (recv < 0) {
    perror("recvfrom");
    close(fd);
    unlink(PATH_CLIENT);
    return -1;
  }

  buffer[recv] = '\0';

  printf("%s\n", buffer);
  close(fd);
  unlink(PATH_CLIENT);

  return 0;
}