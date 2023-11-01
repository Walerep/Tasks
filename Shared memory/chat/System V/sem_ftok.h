#ifndef SEM_FTOK_H
#define SEM_FTOK_H

#include <sys/sem.h>  //  sem
#include <sys/shm.h>  //  shared memory

#define FTOK_PATH_ULK "server.c"
#define FTOK_PATH_CK "header.h"

struct sembuf unlock_sem = {0, -1, 0};
struct sembuf lock_sem = {0, 1, 0};

key_t userlist_key, chat_key, 
      sem_userlist_key, sem_chat_key;  //  Ключи для shmget и semget
int userlist_shm, chat_shm,
    userlist_sem, chat_sem; //  int для id shmget
char *userlist_addr, *chat_addr;  //  адреса привязанной памяти shm


#endif