#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SIZE 1024
#define NAME_IFACE "docker0"
#define SIZE_MAC_ADDR 6
#define PORT 10000
#define DEST_PORT 9000
#define SIZE_PAYLOAD 64

struct msg_UDP_IP_ETH {
  struct iphdr ip_header;
  struct udphdr udp_header;
  struct ether_header eth_header;
  uint16_t buffer[SIZE_PAYLOAD];
};

int main() {
  struct sockaddr_ll sender;
  struct msg_UDP_IP_ETH head;
  uint8_t mac_src[SIZE_MAC_ADDR] = {0x02, 0x42, 0xf1, 0xeb, 0xf1, 0xd1};
  uint8_t mac_dest[SIZE_MAC_ADDR] = {0x00, 0x45, 0xe2, 0x6a, 0xb0, 0x33};
  int flag = 1, fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  if (fd < 0) {
    perror("socket");
    return -1;
  }

  memset(&sender, 0, sizeof(sender));

  head.ip_header.tot_len = htons(sizeof(struct msg_UDP_IP_ETH));
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
  head.udp_header.len = htons(sizeof(struct udphdr) + SIZE_PAYLOAD);
  head.udp_header.source = htons(PORT);
  head.udp_header.dest = htons(DEST_PORT);
  head.eth_header.ether_type = htons(ETH_P_IP);
  sender.sll_family = AF_PACKET;
  sender.sll_halen = 6;
  sender.sll_ifindex = if_nametoindex(NAME_IFACE);

  strcpy(head.buffer, "DD");
  strcpy(mac_src, sender.sll_addr);
  strcpy(head.eth_header.ether_shost, mac_src);
  strcpy(head.eth_header.ether_dhost, mac_dest);
  sendto(fd, &head, sizeof(head), 0, (struct sockaddr_ll *)&sender,
         sizeof(sender));

  char buffer[SIZE];

  while (1) {
    if (recv(fd, buffer, SIZE, 0) < 0) {
      perror("recv");
      close(fd);
      return -1;
    }

    if (htons(head.eth_header.ether_type) == ETH_P_IP) {
      char *recv_buffer = buffer + sizeof(struct iphdr) +
                          sizeof(struct udphdr) + sizeof(struct ether_header);

      if (htons(head.udp_header.dest) == DEST_PORT) {
        printf("%s\n", recv_buffer);
        break;
      }
    }
  }

  close(fd);

  return 0;
}