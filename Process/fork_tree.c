#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define TREE_DEPTH 2

//  Строит левую половину древа
//  Принимает на вход желаемую глубину (depth)
//  и строит ветви вызывая саму себя дважды
//  разрастаясь в обе стороны
//  пока не обнулит глубину
void tree_branch_left(int depth) {
  if (depth <= 0) return;
  pid_t branch_pid;
  int status;
  branch_pid = fork();
  if (branch_pid == 0) {
    printf("Child pid = %d    parent pid = %d\n", getpid(), getppid());
    tree_branch_left(depth - 1);
    tree_branch_left(depth - 1);
    exit(EXIT_SUCCESS);
  } else {
    waitpid(branch_pid, &status, 0);
  }
}

//  Строит правую половину древа
//  Принимает на вход желаемую глубину (depth)
//  и строит ветви вызывая саму себя
//  разрастаясь по линейной ветви
//  пока не обнулит глубину
void tree_branch_right(int depth) {
  if (depth == 0) return;
  pid_t branch_pid;
  int status;
  branch_pid = fork();
  if (branch_pid == 0) {
    printf("Child pid = %d    parent pid = %d\n", getpid(), getppid());
    tree_branch_right(depth - 1);
    exit(EXIT_SUCCESS);
  } else {
    waitpid(branch_pid, &status, 0);
  }
}

int main() {
  tree_branch_left(TREE_DEPTH);
  printf("----------------------------\n");
  tree_branch_right(TREE_DEPTH);
  return 0;
}
