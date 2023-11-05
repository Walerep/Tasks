#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shared_params.h"

int main() {
  int fd_server, status;
  struct sockaddr_un server, client;

  char msg[] = "You finaly awake\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка сокета сервера
  server.sun_family = DOMAIN_TYPE;
  strncpy(server.sun_path, SOCK_NAME_SER, sizeof(server.sun_path) - 1);

  //  Привязка сокета к адресу и порту
  if (bind(fd_server, (struct sockaddr *)&server, sizeof(server)) < 0)
    err_exit("bind fail");

  printf("Server started. Wating for connection...\n");

  int client_size = sizeof(client);

  //  Принятие сообщения
  status = recvfrom(fd_server, recv_buf, BUFFER_SIZE, 0,
                    (struct sockaddr *)&client, &client_size);
  if (status < 0) err_exit("recvfrom fail");

  //  Отправка сообщения
  status = sendto(fd_server, msg, sizeof(msg), 0, (struct sockaddr *)&client,
                  (socklen_t *)client_size);
  if (status < 0) err_exit("sendto fail");

  printf("Sended : %s\nRecived : %s\n", msg, recv_buf);

  close(fd_server);
  unlink(SOCK_NAME_SER);

  return 0;
}
