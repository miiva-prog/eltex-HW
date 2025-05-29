#include <ctype.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 10000
#define PROTOCOL 0
#define INDEX 0
#define SIZE 1024
#define IP "255.255.255.255"
#define MULTI_IP "224.0.0.1"

int main() {
  struct sockaddr_in server;
  struct ip_mreqn group_sock;
  int fd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  group_sock.imr_address.s_addr = INADDR_ANY;
  group_sock.imr_ifindex = INDEX;

  if (inet_pton(AF_INET, MULTI_IP, &group_sock.imr_multiaddr.s_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return -1;
  }

  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group_sock,
                 sizeof(group_sock)) < 0) {
    perror("setsockopt");
    return -1;
  }

  if (bind(fd, (struct sockaddr_in *)&server, sizeof(server)) < 0) {
    perror("bind");
    return -1;
  }

  char buffer[SIZE];
  socklen_t len = sizeof(server);

  if (recvfrom(fd, buffer, SIZE, 0, (struct sockaddr_in *)&server, &len) < 0) {
    perror("recvfrom");
    close(fd);
    return -1;
  }

  printf("%s\n", buffer);
  close(fd);

  return 0;
}