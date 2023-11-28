#include "shared_params.h"

void fill_udp_header(struct udphdr *udp_head, uint16_t src_port,
                     uint16_t dest_port, uint16_t length, uint16_t checksum) {
  udp_head->source = htons(src_port);
  udp_head->dest = htons(dest_port);
  udp_head->len = htons(length);
  udp_head->check = checksum;
}

void recive_msg(int fd_server, struct sockaddr_in *server,
                struct sockaddr_in *client, struct udphdr *udp_head,
                char *payload) {
  char recv_buf[BUFFER_SIZE];  //  Буффер на прием
  int server_size = sizeof(server);

  //  Принятие сообщения
  memset(client, 0, sizeof(struct sockaddr_in));
  while (1) {
    if (recvfrom(fd_server, recv_buf, BUFFER_SIZE, 0,
                 (struct sockaddr *)&server, (socklen_t *)&server_size) == -1) {
      err_exit("send fail");
    }
    udp_head = (struct udphdr *)(recv_buf + sizeof(struct iphdr));
    if (udp_head->dest == htons(CLIENT_PORT)) break;
  }
  payload = recv_buf + sizeof(struct iphdr) + sizeof(struct udphdr);
  printf("Recived : %s", payload);
}

int main() {
  int fd_server;
  struct sockaddr_in server, client;
  struct udphdr *udp_head;

  char msg[] = "Hello\n";      //  Сообщение
  char datagram[BUFFER_SIZE];  //  Буффер для отправки
  char *payload;               //  Буффер для сообщения

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_RAW, IPPROTO_UDP);
  if (fd_server == -1) err_exit("socket creation fail");
  memset(datagram, 0, BUFFER_SIZE);
  udp_head = (struct udphdr *)datagram;

  //  Заполнение заголовка
  fill_udp_header(udp_head, CLIENT_PORT, SERVER_PORT,
                  sizeof(struct udphdr) + sizeof(msg), 0);

  //  Заполнение сообщения
  payload = (char *)(datagram + sizeof(struct udphdr));
  strncpy(payload, msg, sizeof(msg));

  //  Настройка сокета
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  printf("Working...\n");

  int server_size = sizeof(server);

  //  Отправка сообщения
  if (sendto(fd_server, datagram, sizeof(datagram), 0,
             (struct sockaddr *)&server, (socklen_t)server_size) < 0) {
    err_exit("send fail");
  }
  printf("Sended : %s\n", msg);
  //  Принятие сообщения
  recive_msg(fd_server, &server, &client, udp_head, payload);

  close(fd_server);

  return 0;
}
