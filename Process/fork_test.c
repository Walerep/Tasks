#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void terminated() { printf("Program terminated... at pid = %d\n", getpid()); }

int main() {
  pid_t child_pid;
  int status;
  atexit(terminated);
  child_pid = fork();
  if (child_pid == 0) {
    printf("Child pid = %d\n", getpid());
     execl("/bin/ls", "ls", "-a", NULL);
    exit(EXIT_SUCCESS);
  } else {
    printf("Parent pid = %d\n", getpid());
    waitpid(child_pid, &status, 0);
  }
  return 0;
}
