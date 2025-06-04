#include <ctype.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 10000
#define DEST_PORT 9000
#define PROTOCOL 0
#define SIZE 1024
#define HEADER_SIZE 8
#define IP "127.0.0.1"
#define SIZE_UDP 4

struct msg_UDP {
  struct udphdr header;
  uint16_t buffer[SIZE_UDP];
};

int main() {
  struct sockaddr_in client;
  struct msg_UDP udp_head;
  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  udp_head.header.len = htons(sizeof(struct msg_UDP));
  udp_head.header.source = htons(PORT);
  udp_head.header.dest = htons(DEST_PORT);
  udp_head.header.check = 0;
  client.sin_family = AF_INET;
  client.sin_port = htons(0);

  strcpy(udp_head.buffer, "100");

  if (inet_pton(AF_INET, IP, &(client.sin_addr.s_addr)) <= 0) {
    perror("inet_pton");
    return -1;
  }

  if (sendto(fd, &udp_head, sizeof(udp_head), 0, (struct sockaddr *)&client,
             sizeof(client)) < 0) {
    perror("sendto");
    return -1;
  }

  uint8_t buffer[SIZE];
  struct msg_UDP *msg;

  while (1) {
    if (recv(fd, buffer, SIZE, 0) < 0) {
      perror("recv");
      return -1;
    }

    uint8_t shift = (buffer[0] & 0x0F) * 4;

    msg = (struct msg_UDP *)(buffer + shift);

    if (ntohs(msg->header.dest) == DEST_PORT) {
      printf("%s\n", (char *)msg->buffer);
      break;
    }
  }

  return 0;
}