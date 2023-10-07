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
/*
#define MAX_USERS 10
#define MAX_USERNAME 30
#define MAX_MSG 100

char Username[MAX_USERNAME];  //  пока храним имя пользователя глобально в
                              //  клиенте
char Message_buff[MAX_MSG];  //  буффер для вводимых сообщений

struct message {
  long mtype;           //  1 - сервер -> клиент 2 - клиент -> сервер
  int id;               //  id отправителя (id = pid)
  char mtext[MAX_MSG];  //  Текст сообщения
};
*/

int main(int argc, const char **argv) {
  printf("client\n");
  key_t key;
  int server_msqid;   //  id очереди сервера
  struct message msg;
  //int client_id = getpid();

  //  id очереди клиента для получения ответов от сервера
  int client_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
      if (client_id == -1) err_exit("msgget client_msqid");

  // Генерируем уникальный ключ
  key = ftok("server.c", 3);

  // Создаем очередь сообщений сервера
  server_msqid = msgget(key, 0666);
  if (server_msqid == -1) err_exit("msgget");

  printf("Введите свое имя: ");
  fgets(Username, MAX_USERNAME, stdin);
  Username[strcspn(Username, "\n")] = '\0';

  // Отправляем запрос на регистрацию в чате
  strcpy(msg.mtext, "register ");
  strcat(msg.mtext, Username);
  msg.mtype = 2;
  msg.id = client_id;
/*
  printf("отправка\n");
        printf("msqid %d\n", server_msqid);
        printf("type %ld\n", msg.mtype);
        printf("id %d\n", msg.id);
        printf("text %s\n", msg.mtext);
        printf("size %ld / %ld\n", sizeof(msg) - sizeof(long), sizeof(struct
  message) - sizeof(long));
*/
  printf("id: %d\n",msg.id);
  if ((msgsnd(server_msqid, &msg, sizeof(struct message), 0)) == -1) {
    err_exit("msgsnd register");
  }

  // Получаем ответ от сервера с ID клиента
  if (msgrcv(client_id, &msg, sizeof(struct message), 1, 0) == -1) {
    err_exit("msgrcv register");
  }
  
  //  получаем приветственное сообщение
  if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
    err_exit("msgrcv get_greetings");
  }
  printf("%s",msg.mtext);
  
  while (1) {
    printf("Выберите действие:\n");
    printf("1. Получить список активных пользователей\n");
    printf("2. Отправить сообщение всем пользователям\n");
    printf("3. Отправить сообщение конкретному пользователю\n");
    printf("4. Выйти из чата\n");
    printf("Введите номер действия: ");
    int choice;
    scanf("%d", &choice);
    getchar();  // Чтение символа новой строки после ввода числа

    switch (choice) {
      case 1:
        // Отправляем запрос на получение списка активных пользователей
        strcpy(msg.mtext, "get_users");
        msg.mtype = client_id;
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd get_users");
        }

        // Получаем список активных пользователей от сервера
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get_users");
        }

        printf("Список активных пользователей:\n%s\n", msg.mtext);
        break;
      case 2:
        // Отправляем запрос на отправку сообщения всем пользователям
        strcpy(msg.mtext, "send_all ");
        msg.mtype = client_id;
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send_all");
        }
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get send_all");
        }

        printf("%s", msg.mtext);
        fgets(Message_buff, MAX_MSG, stdin);
        Message_buff[strcspn(Message_buff, "\n")] = '\0';
        strcpy(msg.mtext, Message_buff);

        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send_all");
        }

        //  Получение отправленного сообщения (проверка)
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get send_all");
        }

        printf("Сообщение:\n%s\n", msg.mtext);
        break;
      case 3:
        strcpy(msg.mtext, "send ");
        msg.mtype = client_id;
        //  Отправка запроса
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        //  Получение ответа на запрос
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get send");
        }
        
        //  Вводим имя получателя
        printf("%s", msg.mtext);
        fgets(msg.mtext, MAX_USERNAME, stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        //  Отправляем имя получателя
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        //  Получаем подтверждение существования получателя
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get send");
        }

        printf("Введите сообщение: ");
        fgets(msg.mtext, sizeof(msg.mtext), stdin);
        msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        //  Отправляем текст сообщение
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        //  Получение отправленного сообщения (проверка)
        if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
          err_exit("msgrcv get send_all");
        }

        printf("Сообщение:\n%s\n", msg.mtext);
        
        break;
      case 4:
        // Отправляем запрос на отключение от чата
        strcpy(msg.mtext, "quit");
        msg.mtype = client_id;
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd quit");
        }

        printf("Вы успешно вышли из чата.\n");
        exit(0);
      default:
        printf("Неверный номер действия.\n");
    }
  }

  return 0;
}