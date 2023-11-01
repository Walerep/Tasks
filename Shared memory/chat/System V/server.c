#include "header.h"
#include "sem_ftok.h"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

struct client clients[MAX_USERS];
int num_clients = -1;


int server_sem;


int main() {
  printf("server\n");


  // Генерируем уникальный ключ
  userlist_key = ftok(FTOK_PATH_ULK, 3);
  chat_key= ftok(FTOK_PATH_CK, 3);

  // Создаем очередь сообщений сервера
  userlist_shm = shmget(userlist_key, MAX_MSG, IPC_CREAT | 0666);
  if (userlist_shm == -1) err_exit("shmget userlist");
  chat_shm = shmget(chat_key, MAX_MSG, IPC_CREAT | 0666);
  if (chat_shm == -1) err_exit("shmget chat");
  printf("userlist_shm %d\n", userlist_shm);
  printf("chat_shm %d\n", chat_shm);
  printf("Сервер чата запущен.\n");

  //  Привязываем рзделенную память в адресное пространство
  userlist_addr = shmat(userlist_shm, NULL, 0);
  if (userlist_addr == (char *) -1) err_exit("shmat userlist");
  chat_addr = shmat(chat_shm, NULL, 0);
  if (chat_addr == (char *) -1) err_exit("shmat chat");

  //  Генерируем семафоры
  //  Семфор чата
  sem_chat_key = ftok(FTOK_PATH_CK, 1);
  if (sem_chat_key == -1) err_exit("ftok");
  chat_sem = semget(sem_chat_key, 1, IPC_CREAT | 0666);
  if (chat_sem == -1) err_exit("ftok");

  // Семафор списка пользователей
  sem_userlist_key = ftok(FTOK_PATH_ULK, 2);
  if (sem_userlist_key == -1) err_exit("ftok");
  userlist_sem = semget(sem_userlist_key, 1, IPC_CREAT | 0666);
  if (userlist_sem == -1) err_exit("semget");

  if (semctl(chat_sem, 0, SETVAL, 1) == -1) err_exit("semctl");
  if (semctl(userlist_sem, 0, SETVAL, 1) == -1) err_exit("semctl");

  
  //  Запись строки Users online в пространство userlist_addr
  semop(userlist_sem, &lock_sem, 1);
  strcpy(userlist_addr, "Users online:\n");
  semop(userlist_sem, &unlock_sem, 1);

  semop(chat_sem, &lock_sem, 1);
  strcpy(chat_addr, "---Chat started---\n");
  semop(chat_sem, &unlock_sem, 1);

  //  Отключение сервера
  while (getchar() != 'q');
  //  Отсоеденяем разделяемую память
  //  удаляем сегмент разделяемой памяти и семафор
  if(shmdt(userlist_addr) == -1) err_exit("shmdt");
  if(shmctl(userlist_shm, IPC_RMID, NULL) == -1) err_exit("shmctl");
  if(semctl(chat_sem, 0, IPC_RMID) == -1) err_exit("semctl");
  if(semctl(userlist_sem, 0, IPC_RMID) == -1) err_exit("semctl");

  printf("Сервер чата завершен.\n");

  return 0;
}