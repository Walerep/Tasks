#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_head.h"

int main() {
  key_t shm_key, sem_key;
  int shm_id, sem_id;
  char *addr;
  shm_key = ftok(FTOK_PATH, 0);  //  генерация ключа разделяемой памяти
  if (shm_key == -1) err_exit("ftok");
  shm_id = shmget(shm_key, MSG_SIZE, 0);  //  id памяти
  if (shm_id == -1) err_exit("msgget");

  sem_key = ftok(FTOK_PATH, 1);  //  генерация ключа семфора
  if (sem_key == -1) err_exit("ftok");
  sem_id = semget(sem_key, 1, 0);  //  id семафора
  if (sem_id == -1) err_exit("msgget");
  
  //  Привязываем рзделенную память в адресное пространство
  addr = shmat(shm_id, NULL, 0);
  if (addr == (char *) -1) err_exit("shmat");

  //  Блокируем семафор
  semop(sem_id, &lock_sem, 1);

  //  Считываем строку из разделенной памяти
  printf("Recived %s\n", addr);

  //  Записываем строку в разделяемую память
  strcpy(addr, "Hi");
  printf("Sended %s\n", addr);

  //  Разблокируем семафор
  semop(sem_id, &unlock_sem, 1);

  //  Отсоеденяем разделяемую память
  if(shmdt(addr) == -1) err_exit("shmdt");
  return 0;
}
