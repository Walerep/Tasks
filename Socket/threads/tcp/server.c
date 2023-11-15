#include "shared_params.h"

#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

void * client_handler(void *arg) {
  if (arg < 0) printf("Invalid file descriptor passed to client_handler\n");
  int fd_client = *((int *)arg);
  free(arg);
  time_t cur_time = time(NULL);
  //  Отправка сообщения
  pid_t pid = getpid();
  printf("SENDING FROM PID : %d\n", pid);
  if (send(fd_client, &cur_time, sizeof(cur_time), 0) < 0) thread_err_exit("send fail");
  //  Закрытие дескриптора
  if (close(fd_client) < 0) thread_err_exit("close fail");
  return NULL;
}


int main() {
  
  int fd_server, fd_client;
  struct sockaddr_in server, client;

  //  Создание сокета
  fd_server = socket(DOMAIN_TYPE, SOCK_TYPE, 0);
  if (fd_server < 0) err_exit("socet creation fail");

  //  Настройка адреса сервера
  server.sin_family = DOMAIN_TYPE;
  server.sin_port = htons(PORT_NUM);
  server.sin_addr.s_addr = SOCK_ADDR;

  //  Привязка сокета
  if (bind(fd_server, (struct sockaddr *)&server, sizeof(server)) < 0)
    err_exit("bind fail");

  //  Ожидание подключений
  if (listen(fd_server, MAX_CLIENTS) < 0) err_exit("listen fail");

  printf("Server started. Wating for connection...\n");

  //  Принятие подключения
  
  while (1) {
    pthread_t thread_id;
    socklen_t client_size = sizeof(client);
    fd_client = accept(fd_server, (struct sockaddr *)&client, &client_size);
    if (fd_client < 0) err_exit("accept fail");
    int *thread_socket = malloc(sizeof(int));
    *thread_socket = fd_client;
    if (pthread_create(&thread_id, NULL, client_handler, thread_socket) < 0) err_exit("pthread_create fail");
    pthread_detach(thread_id);
  }
  close(fd_client);
  return 0;
}
