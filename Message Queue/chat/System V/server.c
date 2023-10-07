#include <stdio.h>    //  perror printf
#include <stdlib.h>   //  exit
#include <string.h>   //  strcpy
#include <sys/ipc.h>  //  ftok
#include <sys/msg.h>  //  msgsnd
#include <unistd.h>   //  getpid
// #include <sys/sem.h>    //
// #include <sys/types.h>  //
#include <errno.h>    //  perror
#include <pthread.h>  //  pthread_create
#include "header.h"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

struct client clients[MAX_USERS];
int num_clients = 0;
int server_msgqid;

void *client_handler(void *arg) {
  struct client *client = (struct client *)arg;
  struct message msg;
  printf("clients -------\n");
  for (int i = 0; i < num_clients; i++)
  {
    printf("%s\n", clients[i].name);
  }
  printf("---------------\n");
  
  // Отправляем приветственное сообщение клиенту
  strcpy(msg.mtext, "Добро пожаловать в чат!\n\0");
  msg.mtype = client->id;
  printf("id: %ld\n",msg.mtype);
  if (msgsnd(client->id, &msg, sizeof(struct message), 0) == -1) {
    err_exit("msgsnd: Приветствие не отправленно");
  }
  
  while (1) {
    // Получаем сообщение от клиента
    if (msgrcv(client->id, &msg, sizeof(struct message), 0, 0) == -1) {
      err_exit("msgrcv client_handler_1");
    }

    // Проверяем, является ли сообщение запросом на получение списка активных
    // пользователей
    if (strcmp(msg.mtext, "get_users") == 0) {
      // Отправляем список активных пользователей клиенту
      char user_list[MAX_MSG] = "";
      for (int i = 0; i < num_clients; i++) {
        strcat(user_list, clients[i].name);
        strcat(user_list, "\n");
      }
      strcpy(msg.mtext, user_list);
      if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                 0) == -1) {
        err_exit("msgsnd get_users");
      }
    }
    // Проверяем, является ли сообщение запросом на отправку сообщения всем
    // пользователям
    else if (strcmp(msg.mtext, "send_all ") == 0) {
      // Отправляем сообщение всем клиентам
      printf("Получен запрос на отправку всем клиентам\n");
      strcpy(msg.mtext, "Введите сообщение: ");
      if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                 0) == -1) {
        err_exit("msgsnd send_all");
      }
      if (msgrcv(client->id, &msg, sizeof(struct message), client->id, 0) == -1) {
        err_exit("msgrcv send_all");
      }
      printf("Полученно : %s\n", msg.mtext);
      //sscanf(msg.mtext, "send_all %s", msg.mtext);
      //printf("Выделено: %s\n", msg.mtext);
      for (int i = 0; i < num_clients; i++) {
        if (msgsnd(clients[i].id, &msg, sizeof(struct message) - sizeof(long), 0) == -1) {
          err_exit("msgsnd send_all");
        }
      }
    }
    // Проверяем, является ли сообщение запросом на отправку сообщения
    // конкретному пользователю
    else if (strncmp(msg.mtext, "send ", 5) == 0) {
      // Извлекаем имя пользователя из сообщения
      char recipient_name[MAX_MSG];
      sscanf(msg.mtext, "send %s", recipient_name);

      printf("Получен запрос на отправку клиенту\n");
      strcpy(msg.mtext, "Введите имя получателя: ");
      //  Отправляем ответ на запрос
      if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                 0) == -1) {
        err_exit("msgsnd send_all");
      }
      // Получаем имя получателя
      if (msgrcv(client->id, &msg, sizeof(struct message), client->id, 0) == -1) {
        err_exit("msgrcv send_all");
      }
      strcpy(recipient_name, msg.mtext);
      printf("Полученно : %s\n", msg.mtext);
      printf("Имя получателя : %s\n", recipient_name);
      
      // Ищем получателя в списке активных пользователей
      int recipient_id = -1;
      for (int i = 0; i < num_clients; i++) {
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
        if (msgsnd(client->id, &msg, sizeof(struct message) - sizeof(long),
                 0) == -1) {
        err_exit("msgsnd send_all");
        }
        //  Получение текста сообщения
        if (msgrcv(client->id, &msg, sizeof(struct message), client->id, 0) == -1) {
        err_exit("msgrcv send_all");
        }
        printf("Полученно : %s\n", msg.mtext);
        
        //  Отправляем сообщение
        msg.mtype = recipient_id;
        if (msgsnd(clients[recipient_id].id, &msg,
                   sizeof(struct message) - sizeof(long), 0) == -1) {
          err_exit("msgsnd send");
        }
      }
    }
    // Проверяем, является ли сообщение запросом на отключение от чата
    else if (strcmp(msg.mtext, "quit") == 0) {
      printf("Получен запрос на отключение\n");
      // Удаляем клиента из списка активных пользователей
      for (int i = 0; i < num_clients; i++) {
        if (clients[i].id == msg.mtype) {
          for (int j = i; j < num_clients - 1; j++) {
            clients[j] = clients[j + 1];
          }
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
    }
    if (num_clients == 0)
    {
      printf("Клинетов не осталось\n");
      pthread_exit(NULL);
    }
    
  }

  pthread_exit(NULL);
}

int main(int argc, const char **argv) {
  printf("server\n");
  key_t key;
  pthread_t tid;
  struct message msg;

  // Генерируем уникальный ключ
  key = ftok("server.c", 3);

  // Создаем очередь сообщений сервера
  server_msgqid = msgget(key, IPC_CREAT | 0666);
  if (server_msgqid == -1) err_exit("msgget");
  printf("msqid %d\n", server_msgqid);
  printf("Сервер чата запущен.\n");

  while (1) {
    // Принимаем запрос на регистрацию клиента
    if (msgrcv(server_msgqid, &msg, sizeof(struct message), 2, 0) == -1) {
      err_exit("msgrcv registration");
    }
    /*
    printf("Получено\n");
        printf("msqid %d\n", server_msgqid);
        printf("type %ld\n", msg.mtype);
        printf("id %d\n", msg.id);
        printf("text %s\n", msg.mtext);
    */
    // Проверяем, является ли сообщение запросом на регистрацию
    if (strncmp(msg.mtext, "register ", 9) == 0) {
      // Извлекаем имя клиента из сообщения
      printf("Запрос регистрации\n");
      char client_name[MAX_MSG];
      sscanf(msg.mtext, "register %s", client_name);

      //  Извлекаем id клиента из сообщения
      //int client_id = msg.id;

      // Создаем новую очередь сообщений для клиента
      
      // Добавляем клиента в список активных пользователей
      struct client new_client;
      new_client.id = msg.id;   //  id очереди клиента
      strcpy(new_client.name, client_name);
      //new_client.msgqid = client_msgqid;
      //clients[num_clients++] = new_client;  // запись клиента в список
      memcpy(&clients[num_clients++], &new_client, sizeof(struct client));
      // Отправляем клиенту его ID очереди

      msg.mtype = 1;
      strcpy(msg.mtext, new_client.name);
      printf("отправка\n");
      //printf("client_msqid %d\n", client_msgqid);
      //printf("type %ld\n", msg.mtype);
      printf("id %d\n", msg.id);
      //printf("text %s\n", msg.mtext);
      if (msgsnd(msg.id, &msg, sizeof(struct message) - sizeof(long),
                 0) == -1) {
        err_exit("msgsnd id_send");
      }

      // Создаем новый поток для обработки сообщений клиента
      pthread_create(&tid, NULL, client_handler, (void *)&new_client);
    }
  }

  // Уничтожаем очередь сообщений сервера
  if (msgctl(server_msgqid, IPC_RMID, NULL) == -1) {
    perror("msgctl");
    exit(1);
  }

  printf("Сервер чата завершен.\n");

  return 0;
}