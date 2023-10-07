#ifndef HEADER_H
#define HEADER_H

#define MAX_USERS 10
#define MAX_USERNAME 123
#define MAX_MSG 1024

#define err_msg(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

char Username[MAX_USERNAME];  //  пока храним имя пользователя глобально в

char Message_buff[MAX_MSG];  //  буффер для вводимых сообщений

//  структура для сообщений system V
struct message {
  long mtype; /*  1 - сервер -> клиент
                  2 - клиент -> сервер*/
  int id;     //  id отправителя (id = pid)
  // char muser[MAX_USERNAME];   //  Имя отправителя
  char mtext[MAX_MSG];  //  Текст сообщения
};

//  структура для хранения клиентов
struct client {
  int id;              //  id отправителя (id = pid)
  char name[MAX_MSG];  //  Имя отправителя
  //int msgqid;          //  id очереди клиента
};

#endif