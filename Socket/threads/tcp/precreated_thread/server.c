#include "shared_params.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//  структура под очередь заданий
struct queue {
  struct queue *next;
  int *fd_client;
};
typedef struct queue queue_t;

queue_t *head = NULL;
queue_t *tail = NULL;

// добавление в очередь
void enqueue(int *fd_client) {
  queue_t *newnode = malloc(sizeof(queue_t));
  newnode->fd_client = fd_client;
  newnode->next = NULL;
  if (tail == NULL) {
    head = newnode;
  } else {
    tail->next = newnode;
  }
  tail = newnode;
}

//  удаление из очереди
int *dequeue() {
  if (head == NULL) return NULL;
  int *result = head->fd_client;
  queue_t *tmp = head;
  head = head->next;
  if (head == NULL) tail = NULL;
  free(tmp);
  return result;
}

void *client_handler(void *arg) {
  if (arg < 0) printf("Invalid file descriptor passed to client_handler\n");
  int fd_client = *((int *)arg);
  time_t cur_time = time(NULL);
  //  Отправка сообщения
  printf("SENDING TO : %d -- %d bytes\n", fd_client, sizeof(time_t));
  if (send(fd_client, &cur_time, sizeof(cur_time), 0) < 0)
    thread_err_exit("send fail");
  //  Закрытие дескриптора
  if (close(fd_client) < 0) thread_err_exit("close fail");
  //}
  return NULL;
}

void *thread_handler(void *arg) {
  while (1) {
    pthread_mutex_lock(&mutex);
    int *fd_client = dequeue();
    pthread_mutex_unlock(&mutex);
    if (fd_client != NULL) {
      client_handler(fd_client);
    }
  }
}

int main() {
  int fd_server, fd_client;
  struct sockaddr_in server, client;
  pthread_t thread_pool[THREAD_MAX];

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

  //  Создание набора потоков
  for (int i = 0; i < THREAD_MAX; i++) {
    pthread_create(&thread_pool[i], NULL, thread_handler, &fd_client);
  }

  printf("Server started. Wating for connection...\n");

  //  Принятие подключения
  while (1) {
    socklen_t client_size = sizeof(client);
    fd_client = accept(fd_server, (struct sockaddr *)&client, &client_size);
    if (fd_client < 0) err_exit("accept fail");
    int *thread_socket = malloc(sizeof(int));
    *thread_socket = fd_client;
    pthread_mutex_lock(&mutex);
    enqueue(thread_socket);
    pthread_mutex_unlock(&mutex);
  }
  close(fd_client);
  return 0;
}
