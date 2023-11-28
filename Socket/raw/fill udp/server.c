#include "shared_params.h"

int main() {
  int fd_server, status;
  struct sockaddr_in server, client;

  char msg[] = "You finaly awake\n";
  char recv_buf[BUFFER_SIZE];

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка сокета сервера
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(SERVER_PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  //  Привязка сокета
  if (bind(fd_server, (struct sockaddr *)&server, sizeof(server)) < 0)
    err_exit("bind fail");

  printf("Server started. Wating for connection...\n");

  int client_size = sizeof(client);

  //  Принятие сообщения
  status = recvfrom(fd_server, recv_buf, BUFFER_SIZE, 0,
                    (struct sockaddr *)&client, (socklen_t *)&client_size);
  if (status < 0) err_exit("recvfrom fail");
  printf("Recived : %s\n", recv_buf);

  //  Отправка сообщения
  status = sendto(fd_server, msg, sizeof(msg), 0, (struct sockaddr *)&client,
                  (socklen_t)client_size);
  if (status < 0) err_exit("sendto fail");

  printf("Sended : %s\n", msg);

  close(fd_server);

  return 0;
}
