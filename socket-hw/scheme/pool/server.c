#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_SERVERS 10
#define PORT_HOST 10000
#define MAX_PORT 10300
#define PROTOCOL 0

struct server_data {
  int port;
  int busy;
};

struct server_data arr_server[MAX_SERVERS];

void *server_pth(void *arg);

int main() {
  pthread_t threads[MAX_SERVERS];
  struct sockaddr_in server;
  int *index, opt = 1;

  for (int n = 0; n < MAX_SERVERS; n++) {
    arr_server[n].port = PORT_HOST + 1 + n;
    arr_server[n].busy = 1;
    index = malloc(sizeof(int));
    *index = n;
    pthread_create(&threads[n], NULL, server_pth, index);
  }

  int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    perror("setsockopt");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT_HOST);

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("bind");
    return -1;
  }

  if (listen(fd, MAX_SERVERS) == -1) {
    perror("listen");
    return -1;
  }

  for (int n = 0; n < MAX_SERVERS; n++)
    printf("%d - port %d\n", n, arr_server[n].port);

  while (1) {
    int fd_client = accept(fd, NULL, NULL);

    if (fd_client == -1) {
      perror("accept");
      continue;
    }

    int flag = 0;

    for (int n = 0; n < MAX_SERVERS; n++) {
      if (arr_server[n].busy) {
        send(fd_client, &arr_server[n].port, sizeof(int), 0);
        close(fd_client);
        flag = 1;
        break;
      }
    }

    if (!flag) {
      send(fd_client, 0, sizeof(int), 0);
      close(fd_client);
    }
  }

  return 0;
}

void *server_pth(void *arg) {
  int index = *((int *)arg);
  free(arg);

  struct sockaddr_in server;

  int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return NULL;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(arr_server[index].port);

  while (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    arr_server[index].port++;

    if (arr_server[index].port > MAX_PORT) {
      perror("bind");
      close(fd);
      return NULL;
    }

    server.sin_port = htons(arr_server[index].port);
  }

  if (listen(fd, 1) == -1) {
    perror("listen");
    close(fd);
    return NULL;
  }

  arr_server[index].busy = 1;

  int fd_client = accept(fd, NULL, NULL);

  if (fd_client == -1) {
    perror("accept");
    close(fd);
    return NULL;
  }

  printf("connect client - %d\n", arr_server[index].port);

  time_t times = time(NULL);

  arr_server[index].busy = 0;

  send(fd_client, &times, sizeof(times), 0);
  close(fd_client);
  close(fd);

  return NULL;
}