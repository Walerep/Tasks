#include "shared_params.h"

int main() {
  int fd_server;
  struct sockaddr_in server, client;
  struct udphdr *udp_head;

  char msg[] = "Hello\n";      //  Сообщение
  char recv_buf[BUFFER_SIZE];  //  Буффер на прием
  char datagram[BUFFER_SIZE];  //  Буффер для отправки
  char *payload;               //  Буффер для сообщения

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_RAW, IPPROTO_UDP);
  if (fd_server == -1) err_exit("socket creation fail");
  memset(datagram, 0, BUFFER_SIZE);
  udp_head = (struct udphdr *)datagram;

  //  Заполнение заголовка
  udp_head->source = htons(CLIENT_PORT);
  udp_head->dest = htons(SERVER_PORT);
  udp_head->len = htons(sizeof(struct udphdr) + sizeof(msg));
  udp_head->check = 0;

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
             (struct sockaddr *)&server, (socklen_t)server_size) < 0){
              err_exit("send fail");
             }
  printf("Sended : %s\n", msg);
  //  Принятие сообщения
  memset(&client, 0, sizeof(struct sockaddr_in));
  while (1) {
    if (recvfrom(fd_server, recv_buf, BUFFER_SIZE, 0,
                 (struct sockaddr *)&server, (socklen_t *)&server_size) == -1){
              err_exit("send fail");
             }
    udp_head = (struct udphdr *)(recv_buf + sizeof(struct iphdr));
    if (udp_head->dest == htons(CLIENT_PORT)) break;
  }
  payload = recv_buf + sizeof(struct iphdr) + sizeof(struct udphdr);
  printf("Recived : %s", payload);

  close(fd_server);

  return 0;
}
