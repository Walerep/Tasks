#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared_head.h"

int main() {
  key_t shm_key, sem_key;
  int shm_id, sem_id;
  union semun arg;
  char *addr;
  shm_key = ftok(FTOK_PATH, 0);  //  генерация ключа разделяемой памяти
  if (shm_key == -1) err_exit("ftok");
  shm_id = shmget(shm_key, MSG_SIZE, IPC_CREAT | 0666);  //  id памяти
  if (shm_id == -1) err_exit("msgget");

  sem_key = ftok(FTOK_PATH, 1);  //  генерация ключа семфора
  if (sem_key == -1) err_exit("ftok");
  sem_id = semget(sem_key, 1, IPC_CREAT | 0666);  //  id семафора
  if (sem_id == -1) err_exit("msgget");
  
  //  Инициализируем семафор
  arg.val = 1;
  if (semctl(sem_id, 0, SETVAL, arg) == -1) err_exit("semctl");
  
  //  Привязываем рзделенную память в адресное пространство
  addr = shmat(shm_id, NULL, 0);
  if (addr == (char *) -1) err_exit("shmat");

  //  Блокируем семафор
  semop(sem_id, &lock_sem, 1);

  //  Записываем строку в разделяемую память
  strcpy(addr, "Hello");
  printf("Sended %s\n", addr);

  //  Разблокируем семафор
  semop(sem_id, &unlock_sem, 1);

  sleep(2);
  
  //  Считываем ответ из разделенной памяти
  semop(sem_id, &lock_sem, 1);
  printf("Recived %s\n", addr);
  semop(sem_id, &unlock_sem, 1);

  //  Отсоеденяем разделяемую память
  //  удаляем сегмент разделяемой памяти и семафор
  if(shmdt(addr) == -1) err_exit("shmdt");
  if(shmctl(shm_id, IPC_RMID, NULL) == -1) err_exit("shmctl");
  if(semctl(sem_id, 0, IPC_RMID) == -1) err_exit("semctl");
  return 0;
}
