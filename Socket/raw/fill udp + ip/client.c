#include "shared_params.h"

void fill_ip_header(struct iphdr *ip_head, uint8_t protocol, uint32_t src_ip,
                    uint32_t dest_ip, uint8_t ttl) {
  ip_head->version = 4;             //  Версия IPv4
  ip_head->ihl = 5;                 //  Размер заголовка IPv4
  ip_head->tos = 0;                 //  Флаг сервиса
  ip_head->tot_len = 0;             //  Длина заголовка
  ip_head->id = 0;                  //  Идентификатор пакета
  ip_head->frag_off = 0;            //  Флаг фрагментации
  ip_head->ttl = ttl;               //  Время жизни
  ip_head->protocol = protocol;     //  Протокол
  ip_head->check = 0;               //  Чек-сумма
  ip_head->saddr = htonl(src_ip);   //  Адрес отправителя
  ip_head->daddr = htonl(dest_ip);  //  Адрес получателя
}

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
  struct iphdr *ip_head;
  struct udphdr *udp_head;

  char msg[] = "Hello\n";      //  Сообщение
  char datagram[BUFFER_SIZE];  //  Буффер для отправки
  char *payload;               //  Буффер для сообщения

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_RAW, IPPROTO_UDP);
  if (fd_server == -1) err_exit("socket creation fail");
  memset(datagram, 0, BUFFER_SIZE);
  ip_head = (struct iphdr *)datagram;
  udp_head = (struct udphdr *)(datagram + sizeof(struct iphdr));

  //  Настройка сокета
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = 0;
  server.sin_addr.s_addr = htonl(CLIENT_IP);

  //  Заполнение заголовка UDP
  fill_udp_header(udp_head, CLIENT_PORT, SERVER_PORT,
                  (sizeof(struct udphdr) + sizeof(msg)), 0);

  //  Включаем заголовок IP в данные пакета
  int opt = 1;
  if (setsockopt(fd_server, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt)) == -1)
    err_exit("setsockopt fail");

  //  Заполнение заголовка IP
  fill_ip_header(ip_head, IPPROTO_UDP, CLIENT_IP, SERVER_IP, 128);

  //  Заполнение сообщения
  payload = (char *)(sizeof(struct iphdr) + sizeof(struct udphdr) + datagram);
  strncpy(payload, msg, sizeof(msg));

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
