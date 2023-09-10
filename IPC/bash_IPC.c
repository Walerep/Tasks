#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)


//  Считывание строки из потока ввода
char* readline() {
  char* line;
  ssize_t buff;
  getline(&line, &buff, stdin);
  return line;
}

//  Разбиение строки ввода на команды по разделителю "|"
char** parcer_command(char* in_line) {
  int position = 0;
  char* token;
  char** out_tokens = malloc(sizeof(char) * 256);
  const char* separator = "|";
  token = strtok(in_line, separator);
  while (token != NULL) {
    out_tokens[position++] = token;
    token = strtok(NULL, separator);
  }
  out_tokens[position] = NULL;
  return out_tokens;
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

  //------------------------------------------
  //  ДЛЯ ПРОВЕРКИ УДАЛИТЬ ПО ЗАВЕРШЕНИЮ
  /*
  for (size_t i = 0; i < sizeof(out_tokens); i++)
  {
    printf("out -->  %s\n", out_tokens[i]);
  }
  */
  //------------------------------------------
  return out_tokens;
}
//-------------------------------------------------------------------------------
//  Проблема: вроде функции работают по отдельности 
//  но вывод первой не передается на вход второй
//  Видимо проблемы с передачей дескриптора
void pipe_work(int pipefd[],char** token_list){
  if (pipe(pipefd) == -1) {
    err_exit("pipe");
  }

  if (write(pipefd[1], stdout, 100*sizeof(char)) == -1)
  {
    err_exit("dup2_1");
  }
  close(pipefd[1]);
  if (dup2(pipefd[0], 0) == -1)
  {
    err_exit("dup2_0");
  }
  close(pipefd[0]);

  
  if (execvp(token_list[0], token_list) == -1)
  {
    err_exit("pipe_work --> execvp");
  }
  
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


//  Основная работа
void processing(char** tokens_list) {
  pid_t child_pid;
  int status;
  int pipefd[2];
  child_pid = fork();
  if (child_pid == 0) {
    pipe_work(pipefd,tokens_list);
  } else {
    waitpid(child_pid, &status, 0);
  }
}

//  Основная работа
void processing_old(char** tokens_list) {
  pid_t child_pid;
  int status;
  child_pid = fork();
  if (child_pid == 0) {
    execvp(tokens_list[0], tokens_list);
  } else {
    waitpid(child_pid, &status, 0);
  }
}

//  По идее должна принимать строку из parcer_command 
//  и вызывать parcer для каждой команды через pipe
void making_pipes(char** token_list) {
  char* buffer[sizeof(token_list)];
  for (int i = 0; i < sizeof(token_list); i++)
  {
    buffer[i] =  token_list[i];            //  Передача в парсер токенов отдельных комманд
  }
  for (size_t i = 0; i < sizeof(token_list); i++)
  {
    processing(parcer(buffer[i]));         //  Передача в парсер токенов отдельных комманд
  }
}





int main() {
  //processing(parcer(parcer_command(readline())));
  making_pipes(parcer_command(readline()));
  return 0;
}