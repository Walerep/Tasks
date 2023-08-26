#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

//  Считывание строки из потока ввода
char* readline() {
  char* line;
  ssize_t buff;
  getline(&line, &buff, stdin);
  return line;
}

//  Разбиение строки ввода на токены
char** parcer(char* in_line) {
  int position = 0;
  char* token;
  char** out_tokens = malloc(sizeof(char) * 256);
  const char* separator = " \n";
  token = strtok(in_line, separator);
  while (token != NULL) {
    out_tokens[position++] = token;
    token = strtok(NULL, separator);
  }
  out_tokens[position] = NULL;
  return out_tokens;
}

//  Основная работа
void processing(char** tokens_list) {
  pid_t child_pid;
  int status;
  child_pid = fork();
  if (child_pid == 0) {
    execvp(tokens_list[0], tokens_list);
  } else {
    waitpid(child_pid, &status, 0);
  }
}

int main() {
  processing(parcer(readline()));
  return 0;
}
