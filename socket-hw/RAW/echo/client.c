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
#define SIZE 1024

int main() {
  struct sockaddr_in client, server;
  socklen_t server_len = sizeof(server);
  int flag = 1, fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    close(fd);
    return -1;
  }

  memset(&client, 0, sizeof(client));

  client.sin_family = AF_INET;
  client.sin_port = 0;

  if (inet_pton(AF_INET, IP, &client.sin_addr.s_addr) <= 0) {
    perror("inet_pton");
    return -1;
  }

  while (1) {
    char buffer[SIZE];

    if (fgets(buffer, SIZE, stdin) == NULL)
      break;

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strlen(buffer) == 0)
      continue;

    char send_buffer[SIZE];

    memset(send_buffer, 0, sizeof(send_buffer));

    struct iphdr *ip = (struct iphdr *)send_buffer;
    struct udphdr *udp = (struct udphdr *)(send_buffer + sizeof(struct iphdr));
    char *payload = send_buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    memcpy(payload, buffer, strlen(buffer));

    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = SIZE;
    ip->id = 0;
    ip->frag_off = 0;
    ip->ttl = 255;
    ip->protocol = 17;
    ip->check = 0;
    ip->saddr = inet_addr(IP);
    ip->daddr = client.sin_addr.s_addr;
    ip->check = 0;
    udp->source = htons(PORT);
    udp->dest = htons(DEST_PORT);
    udp->len = htons(sizeof(struct udphdr) + strlen(buffer));
    udp->check = 0;

    sendto(fd, send_buffer,
           sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(buffer), 0,
           (struct sockaddr *)&client, sizeof(client));

    if (strcmp(buffer, "exit") == 0)
      break;

    memset(&buffer, 0, SIZE);

    int recv = recvfrom(fd, buffer, sizeof(buffer), 0,
                        (struct sockaddr *)&server, &server_len);

    if (recv < 0) {
      perror("recvfrom");
      close(fd);
      return -1;
    }

    buffer[recv] = '\0';

    udp = (struct udphdr *)(buffer + sizeof(struct iphdr));
    payload = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    if (ntohs(udp->source) != DEST_PORT)
      continue;

    printf("(server)%s\n", payload);
  }

  close(fd);

  return 0;
}