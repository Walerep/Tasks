#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shared_head.h"


int main() {
  int shmfd;
  char *shmbuff;
  sem_t *sem1, *sem2;
  
  //  Создаем семафор
  sem1 = sem_open(SEM_PATH_1, O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR, 1);
  if (sem1 == SEM_FAILED) err_exit("sem_open");

  sem2 = sem_open(SEM_PATH_2, O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR, 0);
  if (sem2 == SEM_FAILED) err_exit("sem_open");
  
  //  Создаем сегмент разделяемой памяти
  shmfd = shm_open(SHM_PATH, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
  if (shmfd == -1) err_exit("shm_open");
  
  ftruncate(shmfd, MAX_MSG);
  //  Проецируем буффер в разделяемую память
  shmbuff = mmap(NULL, MAX_MSG, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

  //  Записываем сообщение
  if (sem_wait(sem1) == -1) err_exit("sem_wait");

  strcpy(shmbuff, "Hello");
  printf("Sending : %s\n", shmbuff);

  if(sem_post(sem2) == -1) err_exit("sem_post");

  //  Получаем сообщение
  if (sem_wait(sem1) == -1) err_exit("sem_wait");

  printf("Recived : %s\n", shmbuff);

  munmap(shmbuff, MAX_MSG);

  // Уничтожаем проецированный буффер и разделенную память
  sem_close(sem1);
  sem_close(sem2);
  sem_unlink(SEM_PATH_1);
  sem_unlink(SEM_PATH_2);
  shm_unlink(SHM_PATH);
  
  return 0;  
}