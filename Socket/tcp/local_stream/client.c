#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "shared_params.h"

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024


int main() {
  int fd_server;
  struct sockaddr_un server;

  char msg[] = "Hello\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка адреса сервера
  server.sun_family = DOMAIN_TYPE;
  strncpy(server.sun_path, SOCK_NAME, sizeof(server.sun_path) - 1);

  //  Соединение с сервером
  if(connect(fd_server, (struct sockaddr *) &server, sizeof(server)) < 0) err_exit("connection fail");
  
  printf("Connected to server...\n");

  //  Отправка сообщения
  if (send(fd_server, msg, sizeof(msg), 0) < 0) err_exit("send fail");

  //  Принятие сообщения
  if (recv(fd_server, recv_buf, BUFFER_SIZE, 0) < 0) err_exit("recv fail");

  printf("Sended : %s\nRecived : %s\n", msg, recv_buf);

  close(fd_server);

  return 0;
}
