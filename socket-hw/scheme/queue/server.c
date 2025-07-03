#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PORT_HOST 10000
#define PROTOCOL 0
#define IP "127.0.0.1"
#define MAX_SERVER 10

struct list {
  struct list *next;
  int port;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct list *servers;
int count_server = 0;

void *server_pth();

int main() {
  struct sockaddr_in server;
  struct list *queue = malloc(sizeof(struct list));
  int flag = 1, fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  queue->next = NULL;
  queue->port = -1;
  servers = queue;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT_HOST);

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    return -1;
  }

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind");
    return -1;
  }

  if (listen(fd, MAX_SERVER) < 0) {
    perror("listen");
    return -1;
  }

  pthread_t servers[MAX_SERVER];

  for (int n = 0; n < MAX_SERVER; n++)
    pthread_create(&servers[n], NULL, server_pth, NULL);

  while (1) {
    int fd_client = accept(fd, NULL, NULL);

    if (fd_client < 0) {
      perror("accept");
      return -1;
    }

    if (recv(fd_client, &queue->port, sizeof(int), 0) < 0) {
      perror("recv");
      close(fd_client);
      return -1;
    }

    queue->next = malloc(sizeof(struct list));
    queue = queue->next;
    queue->next = NULL;
    queue->port = -1;

    close(fd_client);
  }

  return 0;
}

void *server_pth() {
  int number = ++count_server;

  while (1) {
    sleep(5);

    int port = -1;

    pthread_mutex_lock(&mutex);

    if (servers && servers->port != -1) {
      struct list *next = servers->next;

      port = servers->port;

      if (next) {
        free(servers);
        servers = next;
      } else {
        servers->port = -1;
      }
    }

    pthread_mutex_unlock(&mutex);

    if (port != -1) {
      struct sockaddr_in client;

      client.sin_family = AF_INET;
      client.sin_addr.s_addr = INADDR_ANY;
      client.sin_port = htons(port);

      if (inet_pton(AF_INET, IP, &client.sin_addr) <= 0) {
        perror("inet_pton");
        return NULL;
      }

      int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

      if (connect(fd, (struct sockaddr *)&client, sizeof(client)) < 0) {
        perror("connect");
        close(fd);
        continue;
      }

      time_t times = time(NULL);

      printf("%d - port %d\n", number, port);
      send(fd, &times, sizeof(times), 0);
      close(fd);
    }
  }

  return NULL;
}