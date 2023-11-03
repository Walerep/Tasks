#include "header.h"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

struct client clients[MAX_USERS];
int num_clients = -1;
int server_msgqid;

void *client_handler(void *client_i) {
  struct client *client = (struct client *)client_i;
  struct message msg;
  printf("clients -------\n");
  for (int i = 0; i <= num_clients; i++) {
    printf("(%ld) %s\n", clients[i].id, clients[i].name);
  }
  printf("---------------\n");

  // Отправляем приветственное сообщение клиенту

  while (1) {
    // Получаем сообщение от клиента
    if (msgrcv(client->id, &msg, sizeof(struct message), 0, 0) == -1) {
      err_exit("msgrcv client_handler_1");
    }

    // Проверяем, является ли сообщение запросом на получение списка активных
    // пользователей
    switch (msg.mtype) {
      case GET_USERS_MSG:
        // Отправляем список активных пользователей клиенту
        char user_list[MAX_MSG] = "";
        for (int i = 0; i <= num_clients; i++) {
          strcat(user_list, clients[i].name);
          strcat(user_list, "\n  ");
        }
        strcpy(msg.mtext, user_list);
        msg.mtype = GET_USERS_MSG;
        printf("client-handler client msg mtype (id): %ld\n", msg.mtype);
        printf("sending userlist: %s\n", msg.mtext);
        if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                   0) == -1) {
          err_exit("msgsnd get_users");
        }
        break;
      // Проверяем, является ли сообщение запросом на отправку сообщения всем
      // пользователям
      case SEND_ALL_MSG:
        // Отправляем сообщение всем клиентам
        printf("Получен запрос на отправку всем клиентам\n");
        msg.mtype = SEND_ALL_MSG;

        printf("client-handler client msg mtype : %ld\n", msg.mtype);
        for (int i = 0; i <= num_clients; i++) {
          if (msgsnd(clients[i].id, &msg, sizeof(struct message) - sizeof(long),
                     0) == -1) {
            err_exit("msgsnd send_all");
          }
        }
        break;
      // Проверяем, является ли сообщение запросом на отправку сообщения
      // конкретному пользователю
      case SEND_MSG:
        // Извлекаем имя пользователя из сообщения
        char recipient_name[MAX_MSG];
        // sscanf(msg.mtext, "send %s", recipient_name);

        printf("Получен запрос на отправку клиенту\n");
        strcpy(msg.mtext, "Enter reciver name: ");
        //  Отправляем ответ на запрос
        msg.mtype = SERVICE_MSG;
        printf("client-handler client msg mtype (id): %ld\n", msg.mtype);
        if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                   0) == -1) {
          err_exit("msgsnd send_all");
        }
        // Получаем имя получателя
        if (msgrcv(client->id, &msg, sizeof(struct message), client->id, 0) ==
            -1) {
          err_exit("msgrcv send_all");
        }
        strcpy(recipient_name, msg.mtext);
        printf("Полученно : %s\n", msg.mtext);
        printf("Имя получателя : %s\n", recipient_name);

        // Ищем получателя в списке активных пользователей
        int recipient_id = -1;
        for (int i = 0; i <= num_clients; i++) {
          if (strcmp(clients[i].name, recipient_name) == 0) {
            recipient_id = clients[i].id;
            break;
          }
        }

        // Если получатель найден, отправляем ответ и ждем сообщение
        if (recipient_id != -1) {
          printf("Получатель найден\n");
          strcpy(msg.mtext, "ACCEPT");
          //  Отправляем подтверждение существования получателя
          msg.mtype = SERVICE_MSG;
          printf("client-handler client msg mtype (id): %ld\n", msg.mtype);
          if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                     0) == -1) {
            err_exit("msgsnd send_all");
          }
          //  Получение текста сообщения
          if (msgrcv(client->id, &msg, sizeof(struct message), client->id, 0) ==
              -1) {
            err_exit("msgrcv send_all");
          }
          printf("Полученно : %s\n", msg.mtext);

          //  Отправляем сообщение
          msg.mtype = SERVICE_MSG;
          printf("client-handler client msg mtype (id): %ld\n", msg.mtype);
          if (msgsnd(clients[recipient_id].id, &msg,
                     sizeof(struct message) - sizeof(long), 0) == -1) {
            err_exit("msgsnd send");
          }
        }
        break;
      // Проверяем, является ли сообщение запросом на отключение от чата
      case QUIT_MSG:
        printf("Получен запрос на отключение\n");
        // Удаляем клиента из списка активных пользователей
        for (int i = 0; i < num_clients; i++) {
          if (clients[i].id == msg.id) {
            for (int j = i; j < num_clients - 1; j++) {
              clients[j] = clients[j + 1];
            }
            if (msgctl(msg.id, IPC_RMID, NULL) == -1) {
              perror("msgctl_user");
              exit(1);
            } else
              printf("Очередь клиента %ld удалена\n", msg.id);
            num_clients--;
            break;
          }
        }
        /*
        // Отправляем новый список активных пользователей остальным клиентам
        for (int i = 0; i < num_clients; i++) {
          msg.mtype = clients[i].id;
          strcpy(msg.mtext, "user_list_changed");
          if (msgsnd(clients[i].id, &msg,
                     sizeof(struct message) - sizeof(long), 0) == -1) {
            err_exit("msgsnd user_list_changed");
          }
        }
        */
        break;
        break;
      default:
        break;
    }
  }

  pthread_exit(NULL);
}

void *server_startup() {
  key_t key;
  struct message msg;
  // Генерируем уникальный ключ
  key = ftok(FTOK_PATH, 3);

  // Создаем очередь сообщений сервера
  server_msgqid = msgget(key, IPC_CREAT | 0666);
  if (server_msgqid == -1) err_exit("msgget");
  printf("msqid %d\n", server_msgqid);
  printf("Сервер чата запущен.\n");

  // Принимаем запрос на регистрацию клиента
  while (msgrcv(server_msgqid, &msg, sizeof(struct message), REGISTER_MSG, 0)) {
    // Проверяем, является ли сообщение запросом на регистрацию
    if (msg.mtype != REGISTER_MSG) printf("Not registration type");
    // Извлекаем имя клиента из сообщения
    printf("Запрос регистрации\n");
    char client_name[MAX_USERNAME];
    strcpy(client_name, msg.muser);
    printf("nicname %s\n", client_name);

    // Создаем новую очередь сообщений для клиента
    msg.id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    printf("Generated id for user : %ld\n", msg.id);
    // Добавляем клиента в список активных пользователей
    num_clients++;
    clients[num_clients].id = msg.id;  //  id очереди клиента
    strcpy(clients[num_clients].name, client_name);
    printf("Client %d\n", num_clients);
    printf("server_startup clients id: %ld\n", clients[num_clients].id);
    printf("name: %s\n", clients[num_clients].name);
    memset(&msg, 0, sizeof(msg));
    // Отправляем клиенту его ID очереди

    msg.mtype = REGISTER_MSG;
    msg.id = clients[num_clients].id;
    strcpy(msg.muser, clients[num_clients].name);
    strcpy(msg.mtext, "Welcome to chat");

    printf("отправка\n");
    printf("sending msg id %ld\n", msg.id);
    printf("to user : %s\n", msg.muser);
    printf("as type : %ld\n", msg.mtype);
    if (msgsnd(server_msgqid, &msg, sizeof(struct message), 0) == -1) {
      err_exit("msgsnd id_send");
    }
    memset(&msg, 0, sizeof(struct message));
    pthread_t tid[MAX_USERS];
    pthread_create(&tid[num_clients], NULL, client_handler,
                   (void *)&clients[num_clients]);
  }
}

int main() {
  printf("server\n");
  // key_t key;
  pthread_t tid;
  // struct message msg;

  // Создаем новый поток для обработки сообщений клиента
  pthread_create(&tid, NULL, server_startup, NULL);
  // server_startup();
  while (getchar() != 'q')
    ;
  pthread_cancel(tid);
  pthread_join(tid, NULL);
  // Уничтожаем очередь сообщений сервера
  if (msgctl(server_msgqid, IPC_RMID, NULL) == -1) {
    perror("msgctl_server");
    exit(1);
  }
  printf("Сервер чата завершен.\n");

  return 0;
}
