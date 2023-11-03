#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shared_head.h"


int main() {
  int shmfd;
  char *shmbuff;
  sem_t *sem1, *sem2;

  //  Создаем сегмент разделяемой памяти
  shmfd = shm_open(SHM_PATH, O_RDWR, 0);
  if (shmfd == -1) err_exit("shm_open");

  //  Создаем семафор
  sem1 = sem_open(SEM_PATH_1, 0);
  if (sem1 == SEM_FAILED) err_exit("sem_open");
  sem2 = sem_open(SEM_PATH_2, 0);
  if (sem1 == SEM_FAILED) err_exit("sem_open");

  //  Проецируем буффер в разделяемую память
  shmbuff = mmap(NULL, MAX_MSG, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  
  //  Получаем сообщение
  if (sem_wait(sem2) == -1) err_exit("sem_wait");

  printf("Recived : %s\n", shmbuff);

  //  Записываем сообщение

  strcpy(shmbuff, "Hi");
  printf("Sending : %s\n", shmbuff);

  if(sem_post(sem1) == -1) err_exit("sem_post");

  munmap(shmbuff, MAX_MSG);

  // Уничтожаем проецированный буффер и разделенную память
  sem_close(sem1);
  sem_close(sem2);
  return 0;  
}