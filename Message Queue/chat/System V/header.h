#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>    //  perror printf
#include <stdlib.h>   //  exit
#include <string.h>   //  strcpy
#include <sys/ipc.h>  //  ftok
#include <sys/msg.h>  //  msgsnd
#include <unistd.h>   //  getpid
#include <semaphore.h>  // sem
// #include <sys/types.h>  //
#include <errno.h>    //  perror
#include <pthread.h>  //  pthread_create

#define MAX_USERS 10
#define MAX_USERNAME 64
#define MAX_MSG 1024

#define FTOK_PATH "server.c"

//  Типы сообщений
#define SERVICE_MSG 1
#define REGISTER_MSG 2
#define GET_USERS_MSG 3
#define SEND_ALL_MSG 4
#define SEND_MSG 5
#define QUIT_MSG 6

#define err_msg(msg)    \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

char Username[MAX_USERNAME];  //  пока храним имя пользователя глобально в

char Message_buff[MAX_MSG];  //  буффер для вводимых сообщений

//  структура для сообщений system V
struct message {
  long mtype;                /*  1 - сервер -> клиент
                                 2 - клиент -> сервер*/
  long id;                   //  id отправителя (id = msquid)
  char muser[MAX_USERNAME];  //  Имя отправителя
  char mtext[MAX_MSG];       //  Текст сообщения
};

//  структура для хранения клиентов
struct client {
  long id;                  //  id отправителя (id = msqid)
  char name[MAX_USERNAME];  //  Имя отправителя
  // int msgqid;          //  id очереди клиента
};

#endif