#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

void msg_transfer(char *msg) {
  char buff;
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    err_exit("pipe");
  }
  write(pipefd[1], msg, strlen(msg));     //  записывает сообщение в канал
  close(pipefd[1]);                       // закрывает канал на запись

  while (read(pipefd[0], &buff, 1) > 0)   //  пока в канале чтения что то есть
    write(1, &buff, 1);                   //  хаписывет в стандартный поток
  close(pipefd[0]);
  printf("\n");
  wait(NULL);

  exit(0);
}

int main() {
  char msg[] = "Liquid!!!!!!!";
  char answ[] = "SSSnaake!!!!!";
  int cpid;
  cpid = fork();
  if (cpid == -1) {
    err_exit("fork");
  }
  if (cpid == 0) {
    msg_transfer(answ);
  } else {
    msg_transfer(msg);
  }
}
