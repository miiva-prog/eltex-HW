#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 10000
#define DEST_PORT 9000
#define IP "127.0.0.1"
#define SIZE_HEAD_BUFF 4

struct msg_UDP_IP {
  struct iphdr ip_header;
  struct udphdr udp_header;
  uint16_t buffer[SIZE_HEAD_BUFF];
};

int main() {
  struct sockaddr_in client;
  struct msg_UDP_IP head;
  int flag = 1, fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&client, 0, sizeof(client));

  head.ip_header.tot_len = htons(sizeof(struct msg_UDP_IP));
  head.ip_header.version = 4;
  head.ip_header.check = 0;
  head.ip_header.saddr = htons(PORT);
  head.ip_header.daddr = htons(DEST_PORT);
  head.ip_header.id = 0;
  head.ip_header.ihl = 5;
  head.ip_header.protocol = 17;
  head.ip_header.ttl = 255;
  head.ip_header.frag_off = 0;
  head.ip_header.tos = 0;
  head.udp_header.check = 0;
  head.udp_header.len = htons(sizeof(struct udphdr) + SIZE_HEAD_BUFF);
  head.udp_header.source = htons(PORT);
  head.udp_header.dest = htons(DEST_PORT);
  client.sin_family = AF_INET;
  client.sin_port = 0;

  strcpy(head.buffer, "DD");

  if (inet_pton(AF_INET, IP, &client.sin_addr.s_addr) <= 0) {
    perror("inet_pton");
    return -1;
  }

  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    return -1;
  }

  sendto(fd, &head, sizeof(head), 0, (struct sockaddr_in *)&client,
         sizeof(client));

  return 0;
}