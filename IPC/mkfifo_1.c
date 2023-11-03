#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

int main() {
  int fifofd_1, fifofd_2;
  char msg[] = "Liquid!!!!!!!\n";
  char buff;
  remove(".fifotmp_1");
  if (mkfifo(".fifotmp_1", O_RDWR) == -1) {
    err_exit("fifo_create_fail");
  }
  if (mkfifo(".fifotmp_2", O_RDWR) == -1) {
    err_exit("fifo_create_fail");
  }
  if ((fifofd_1 = open(".fifotmp_1", O_WRONLY)) == -1) {
    err_exit("fifo_open_fail");
  }
  if (write(fifofd_1, msg, strlen(msg)) == -1) {
    err_exit("write() fail");
  }
  if (close(fifofd_1) == -1) {
    err_exit("fifo_close_fail");
  }

  if ((fifofd_2 = open(".fifotmp_2", O_RDONLY)) == -1) {
    err_exit("fifo_open_fail");
  }
  while (read(fifofd_2, &buff, 1) > 0) write(1, &buff, 1);

  if (close(fifofd_2) == -1) {
    err_exit("fifo_close_fail");
  }
  if (remove(".fifotmp_1") == -1) {
    err_exit("revome");
  }
  if (remove(".fifotmp_2") == -1) {
    err_exit("revome");
  }
}