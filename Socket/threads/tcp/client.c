#include "shared_params.h"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void *socket_work() { return NULL; }

int main() {
  int fd_server;
  struct sockaddr_in server;
  struct tm *time;
  time_t recv_buf;

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server == -1) err_exit("socet creation fail");

  //  Настройка адреса сервера
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(PORT_NUM);
  server.sin_addr.s_addr = SOCK_ADDR;

  //  Соединение с сервером
  if (connect(fd_server, (struct sockaddr *)&server, sizeof(server)) < 0)
    err_exit("connect fail");
  //  Принятие сообщения
  if (recv(fd_server, &recv_buf, sizeof(recv_buf), 0) < 0)
    err_exit("recv fail");
  time = localtime(&recv_buf);
  printf("%d : %d : %d\n%d : %d : %d\n", 
         time->tm_mday, time->tm_mon, time->tm_year, 
         time->tm_hour, time->tm_min, time->tm_sec);
  if (close(fd_server) < 0) thread_err_exit("close fail");

  return 0;
}
