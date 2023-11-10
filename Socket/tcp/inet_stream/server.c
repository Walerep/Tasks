#include "shared_params.h"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int main() {
  int fd_server, fd_client;
  struct sockaddr_in server, client;

  char msg[] = "You finaly awake\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка адреса сервера
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(PORT_NUM);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  //  Привязка сокета
  if (bind(fd_server, (struct sockaddr *)&server, sizeof(server)) < 0)
    err_exit("bind fail");

  //  Ожидание подключений
  if (listen(fd_server, MAX_CLIENTS) < 0) err_exit("listen fail");

  printf("Server started. Wating for connection...\n");

  //  Принятие подключения
  int client_size = sizeof(client);
  fd_client = accept(fd_server, (struct sockaddr *)&client, &client_size);
  if (fd_client < 0) err_exit("accept fail");

  //  Отправка сообщения
  if (send(fd_client, msg, sizeof(msg), 0) < 0) err_exit("send fail");

  //  Принятие сообщения
  if (recv(fd_client, recv_buf, BUFFER_SIZE, 0) < 0) err_exit("recv fail");

  printf("Sended : %s\nRecived : %s\n", msg, recv_buf);

  close(fd_server);
  close(fd_client);

  return 0;
}
