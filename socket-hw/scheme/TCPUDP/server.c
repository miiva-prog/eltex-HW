#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>

#define PORT_TCP 10000
#define PORT_UDP 10001
#define IP "127.0.0.1"
#define PROTOCOL 0
#define MAX_CLIENTS 10
#define MAX_EVENTS 64

struct tcp_client {
  int fd;
  int sent_count;
  time_t last_sent;
};

struct udp_client {
  struct sockaddr_in addr;
  int sent_count;
  time_t last_sent;
};

int no_block(int fd);

int main() {
  struct sockaddr_in tcp, udp;
  struct epoll_event ev, events[MAX_EVENTS];
  struct tcp_client tcp_clients[MAX_CLIENTS];
  struct udp_client udp_clients[MAX_CLIENTS];
  int tcp_count = 0, udp_count = 0;

  int fd_tcp = socket(AF_INET, SOCK_STREAM, PROTOCOL);
  int flag = 1, fd_udp = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd_tcp < 0 || fd_udp < 0) {
    perror("socket");
    return -1;
  }

  if (setsockopt(fd_tcp, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    return -1;
  }

  tcp.sin_family = AF_INET;
  tcp.sin_addr.s_addr = INADDR_ANY;
  tcp.sin_port = htons(PORT_TCP);
  udp.sin_family = AF_INET;
  udp.sin_addr.s_addr = INADDR_ANY;
  udp.sin_port = htons(PORT_UDP);

  if (bind(fd_tcp, (struct sockaddr *)&tcp, sizeof(tcp)) < 0 ||
      bind(fd_udp, (struct sockaddr *)&udp, sizeof(udp)) < 0) {
    perror("bind");
    return -1;
  }

  if (listen(fd_tcp, MAX_CLIENTS) < 0) {
    perror("listen");
    return -1;
  }

  no_block(fd_tcp);
  no_block(fd_udp);

  int epfd = epoll_create1(0);

  ev.events = EPOLLIN;
  ev.data.fd = fd_tcp;

  epoll_ctl(epfd, EPOLL_CTL_ADD, fd_tcp, &ev);

  ev.data.fd = fd_udp;

  epoll_ctl(epfd, EPOLL_CTL_ADD, fd_udp, &ev);

  while (1) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, 1000);

    for (int n = 0; n < nfds; n++) {
      int fd = events[n].data.fd;

      if (fd == fd_tcp && tcp_count < MAX_CLIENTS) {
        int client_fd = accept(fd_tcp, NULL, NULL);

        if (client_fd < 0)
          continue;

        no_block(client_fd);

        ev.events = EPOLLIN | EPOLLET;

        ev.data.fd = client_fd;

        epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

        tcp_clients[tcp_count].fd = client_fd;
        tcp_clients[tcp_count].sent_count = 0;
        tcp_clients[tcp_count].last_sent = 0;
        tcp_count++;

        printf("tcp client(%d)\n", client_fd);
      } else if (fd == fd_udp && udp_count < MAX_CLIENTS) {
        char buf[10];
        struct sockaddr_in cli_addr;
        socklen_t len = sizeof(cli_addr);

        recvfrom(fd_udp, buf, sizeof(buf), 0, (struct sockaddr *)&cli_addr,
                 &len);

        udp_clients[udp_count].addr = cli_addr;
        udp_clients[udp_count].sent_count = 0;
        udp_clients[udp_count].last_sent = 0;
        udp_count++;

        printf("udp client(%d)\n", udp_count);
      }
    }

    for (int n = 0; n < tcp_count; n++) {
      time_t now = time(NULL);

      if (tcp_clients[n].sent_count < MAX_CLIENTS &&
          now - tcp_clients[n].last_sent >= 2) {
        time_t t = now;

        send(tcp_clients[n].fd, &t, sizeof(t), 0);

        tcp_clients[n].sent_count++;
        tcp_clients[n].last_sent = now;

        if (tcp_clients[n].sent_count == MAX_CLIENTS) {
          close(tcp_clients[n].fd);
          tcp_clients[n] = tcp_clients[--tcp_count];
          n--;
        }
      }
    }

    for (int n = 0; n < udp_count; n++) {
      time_t now = time(NULL);

      if (udp_clients[n].sent_count < MAX_CLIENTS &&
          now - udp_clients[n].last_sent >= 2) {
        time_t t = now;

        sendto(fd_udp, &t, sizeof(t), 0,
               (struct sockaddr *)&udp_clients[n].addr,
               sizeof(udp_clients[n].addr));

        udp_clients[n].sent_count++;
        udp_clients[n].last_sent = now;

        if (udp_clients[n].sent_count == MAX_CLIENTS) {
          udp_clients[n] = udp_clients[--udp_count];
          n--;
        }
      }
    }

    printf("tcp:%d udp:%d\n", tcp_count, udp_count);
  }

  close(fd_tcp);
  close(fd_udp);
  close(epfd);

  return 0;
}

int no_block(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);

  return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}