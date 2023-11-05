#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_params.h"

int main() {
  int fd_server, status;
  struct sockaddr_un server, client;

  char msg[] = "Hello\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка сокета клиента
  client.sun_family = DOMAIN_TYPE;
  strncpy(client.sun_path, SOCK_NAME_CLI, sizeof(client.sun_path) - 1);

  //  Привязка сокета к адресу и порту
  if (bind(fd_server, (struct sockaddr *)&client, sizeof(client)) < 0)
    err_exit("bind fail");

  //  Настройка сокета сервера
  server.sun_family = DOMAIN_TYPE;
  strncpy(server.sun_path, SOCK_NAME_SER, sizeof(server.sun_path) - 1);

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
  unlink(SOCK_NAME_CLI);

  return 0;
}
