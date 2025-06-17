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
#define MAX_CLIENTS 10
#define SIZE 1024

struct Client {
  struct in_addr addr;
  int count_msg;
};

struct Client clients[MAX_CLIENTS];
int num_clients = 0;

int check_client(struct in_addr *a, struct in_addr *b);

int main() {
  struct in_addr ip_addr;
  struct sockaddr_in client, server;
  socklen_t len = sizeof(server);
  int flag = 1, fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag)) < 0) {
    perror("setsockopt");
    return -1;
  }

  while (1) {
    char buffer[SIZE];
    int countm = 1, found = 0;

    memset(&buffer, 0, SIZE);

    int recv = recvfrom(fd, &buffer, SIZE, 0, (struct sockaddr *)&server, &len);

    if (recv < 0) {
      perror("recvfrom");
      close(fd);
      return -1;
    }

    struct iphdr *ip = (struct iphdr *)&buffer;
    struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct iphdr));
    char *payload = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    if (ntohs(udp->dest) != DEST_PORT)
      continue;

    ip_addr.s_addr = ip->saddr;

    for (int n = 0; n < num_clients; n++) {
      if (check_client(&clients[n].addr, &ip_addr)) {
        if (strcmp(payload, "exit") == 0) {
          memset(&clients[n], 0, sizeof(clients[n]));
          num_clients--;
          countm = 0;
        } else {
          clients[n].count_msg++;
          countm = clients[n].count_msg;
        }

        found = 1;

        break;
      }
    }

    if (found != 1 && num_clients < MAX_CLIENTS) {
      clients[num_clients].addr = ip_addr;
      clients[num_clients].count_msg = 1;
      num_clients++;
    }

    if (countm > 0)
      snprintf(buffer, sizeof(buffer), "%s %d", payload, countm);
    else
      snprintf(buffer, sizeof(buffer), "%s", payload);

    printf("%s\n", buffer);

    char buffer_send[SIZE];

    memset(buffer_send, 0, sizeof(buffer_send));

    struct iphdr *ip_send = (struct iphdr *)buffer_send;
    struct udphdr *udp_send =
        (struct udphdr *)(buffer_send + sizeof(struct iphdr));
    char *payload_send =
        buffer_send + sizeof(struct iphdr) + sizeof(struct udphdr);

    memset(&client, 0, sizeof(client));

    ip_send->ihl = 5;
    ip_send->version = 4;
    ip_send->tos = 0;
    ip_send->tot_len = SIZE;
    ip_send->id = 0;
    ip_send->frag_off = 0;
    ip_send->ttl = 255;
    ip_send->protocol = 17;
    ip_send->check = 0;
    ip_send->saddr = ip->daddr;
    ip_send->daddr = ip->saddr;
    ip_send->check = 0;
    udp_send->source = htons(DEST_PORT);
    udp_send->dest = udp->source;
    udp_send->len = htons(sizeof(struct udphdr) + strlen(buffer));
    udp_send->check = 0;
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = ip_send->daddr;

    strcpy(payload_send, buffer);
    sendto(fd, buffer_send, sizeof(buffer_send), 0, (struct sockaddr *)&client,
           sizeof(client));
  }

  close(fd);

  return 0;
}

int check_client(struct in_addr *a, struct in_addr *b) {
  return a->s_addr == b->s_addr;
}