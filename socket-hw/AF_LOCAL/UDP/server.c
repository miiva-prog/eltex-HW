#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define PATH "/tmp/server.socket"
#define PROTOCOL 0
#define SIZE 1024

int main() {
  struct sockaddr_un server, client;
  int fd = socket(AF_LOCAL, SOCK_DGRAM, PROTOCOL);

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

  char buffer[SIZE];

  while (1) {
    socklen_t len = sizeof(client);
    int recv = recvfrom(fd, buffer, SIZE, 0, (struct sockaddr *)&client, &len);

    if (recv < 0) {
      perror("recvfrom");
      close(fd);
      unlink(PATH);
      return -1;
    }

    buffer[recv] = '\0';

    printf("%s\n", buffer);
    sprintf(buffer, "%d", atoi(buffer) + 1);
    sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *)&client,
           sizeof(client));
  }

  close(fd);
  unlink(PATH);

  return 0;
}