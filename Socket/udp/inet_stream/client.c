#include "shared_params.h"

int main() {
  int fd_server, status;
  struct sockaddr_in server, client;

  char msg[] = "Hello\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка сокета клиента
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(PORT_NUM);
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  //  Соединение с сервером
  status = connect(fd_server, (struct sockaddr *)&server, sizeof(server));
  if (status < 0) err_exit("connect fail");

  printf("Connected to server...\n");

  //  Отправка сообщения
  if (send(fd_server, msg, sizeof(msg), 0) < 0) err_exit("send fail");

  //  Принятие сообщения
  if (recv(fd_server, recv_buf, BUFFER_SIZE, 0) < 0) err_exit("recv fail");

  printf("Sended : %s\nRecived : %s\n", msg, recv_buf);

  close(fd_server);

  return 0;
}
