#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>

#define PORT_TCP_LISTENER 10000
#define PORT_UDP_LISTENER 10001
#define PORT_BASE 10002
#define IP "127.0.0.1"
#define PROTOCOL 0
#define MAX_SERVER 10
#define MAX_PORT 10999

struct sockaddr_in clients[MAX_SERVER];
int next_port = PORT_BASE;

void *tcp_server();

void *udp_server(void *arg);

int main() {
  struct sockaddr_in tcp_addr, udp_addr;
  pthread_t threads[MAX_SERVER];
  int udp_client_count = 0, thread_count = 0;
  int flag = 1, fd_tcp = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  if (fd_tcp < 0) {
    perror("socket");
    return -1;
  }

  tcp_addr.sin_family = AF_INET;
  tcp_addr.sin_addr.s_addr = INADDR_ANY;
  tcp_addr.sin_port = htons(PORT_TCP_LISTENER);

  if (setsockopt(fd_tcp, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    return -1;
  }

  if (bind(fd_tcp, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
    perror("bind");
    return -1;
  }

  if (listen(fd_tcp, MAX_SERVER) < 0) {
    perror("listen");
    return -1;
  }

  int fd_udp = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd_udp < 0) {
    perror("socket");
    close(fd_tcp);
    return -1;
  }

  udp_addr.sin_family = AF_INET;
  udp_addr.sin_addr.s_addr = INADDR_ANY;
  udp_addr.sin_port = htons(PORT_UDP_LISTENER);

  if (bind(fd_udp, (struct sockaddr *)&udp_addr, sizeof(udp_addr)) == -1) {
    perror("bind");
    close(fd_udp);
    return -1;
  }

  int epfd = epoll_create1(0);
  struct epoll_event ev = {.events = EPOLLIN}, events[MAX_SERVER];

  ev.data.fd = fd_tcp;
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd_tcp, &ev);

  ev.data.fd = fd_udp;
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd_udp, &ev);

  while (thread_count < MAX_SERVER) {
    int nfds = epoll_wait(epfd, events, MAX_SERVER, -1);

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == fd_tcp) {
        int client = accept(fd_tcp, NULL, NULL);

        if (client == -1)
          continue;

        int new_port = next_port;

        pthread_create(&threads[thread_count++], NULL, tcp_server, NULL);
        send(client, &new_port, sizeof(new_port), 0);
        close(client);
      } else if (events[i].data.fd == fd_udp) {
        char buffer[10];

        socklen_t len = sizeof(clients[udp_client_count]);

        recvfrom(fd_udp, buffer, sizeof(buffer), 0,
                 (struct sockaddr *)&clients[udp_client_count], &len);

        int *index = malloc(sizeof(int));

        *index = udp_client_count++;

        pthread_create(&threads[thread_count++], NULL, udp_server, index);
      }

      printf("port: %d server: %d\n", next_port - 1, thread_count);
    }
  }

  for (int n = 0; n < thread_count; n++)
    pthread_join(threads[n], NULL);

  close(fd_tcp);
  close(fd_udp);

  return 0;
}

void *tcp_server() {
  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;

  int local_port = __sync_fetch_and_add(&next_port, 1);

  server.sin_port = htons(local_port);

  int fd = socket(AF_INET, SOCK_STREAM, PROTOCOL);

  while (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    local_port = __sync_fetch_and_add(&next_port, 1);
    server.sin_port = htons(local_port);

    if (local_port > MAX_PORT) {
      printf("out of range port\n");
      return NULL;
    }
  }

  if (listen(fd, 2) < 0) {
    perror("listen");
    close(fd);
    return NULL;
  }

  int fd_client = accept(fd, NULL, NULL);

  if (fd_client < 0) {
    perror("accept");
    close(fd);
    return NULL;
  }

  time_t times;

  for (int n = 0; n < MAX_SERVER; n++) {
    times = time(NULL);

    send(fd_client, &times, sizeof(times), 0);
    sleep(2);
  }

  close(fd_client);
  close(fd);

  return NULL;
}

void *udp_server(void *arg) {
  int index = *(int *)arg;

  free(arg);

  struct sockaddr_in server;

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;

  int local_port = __sync_fetch_and_add(&next_port, 1);

  server.sin_port = htons(local_port);

  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  while (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    local_port = __sync_fetch_and_add(&next_port, 1);

    server.sin_port = htons(local_port);

    if (local_port > MAX_PORT) {
      printf("out of range port\n");
      return NULL;
    }
  }

  time_t times;

  for (int n = 0; n < MAX_SERVER; n++) {
    times = time(NULL);

    sendto(fd, &times, sizeof(times), 0, (struct sockaddr *)&clients[index],
           sizeof(clients[index]));
    sleep(2);
  }

  close(fd);

  return NULL;
}