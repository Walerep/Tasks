#include <stdio.h>
#include <pthread.h>

#define RES_MAX 1000000
#define THREADS_MAX 1000

int GLOB_RES = 0;

void *calcus(){
  while (GLOB_RES < RES_MAX)
  {
    GLOB_RES++;
    printf("GLOB = %d\n", GLOB_RES);
  }
}

int main(){
  int a[THREADS_MAX];
  pthread_t thread[THREADS_MAX];
  for (int i = 0; i < THREADS_MAX; i++)
  {
    a[i] = i;
    pthread_create(&thread[i], NULL, calcus, NULL);
  }
  for (int i = 0; i < THREADS_MAX; i++)
  {
    pthread_join(thread[i], NULL);
  }
  printf("Result %d\n", GLOB_RES);
  return 0;
}