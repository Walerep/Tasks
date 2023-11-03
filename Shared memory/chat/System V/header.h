#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>    //  perror printf
#include <stdlib.h>   //  exit
#include <string.h>   //  strcpy
#include <sys/ipc.h>  //  ftok
#include <sys/types.h>  //
#include <errno.h>    //  perror
#include <pthread.h>  //  pthread_create

#define MAX_USERS 10
#define MAX_MSG 1024
#define MAX_USERNAME 64
#define MAX_TEXT 959

#define err_msg(msg)    \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)


char Username[MAX_USERNAME];  //  пока храним имя пользователя глобально в

char Message_buff[MAX_MSG];  //  буффер для вводимых сообщений


#endif