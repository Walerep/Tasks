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
  printf("Enter command: ");
  getline(&line, &buff, stdin);
  line[strcspn(line, "\n")] - '\0';
  return line;
}



//  Разбиение строки ввода на токены
char** parcer(char* in_line, const char* separator) {
  int position = 0;
  char* token;
  char** out_tokens = malloc(sizeof(char) * 1024);
  token = strtok(in_line, separator);
  while (token != NULL) {
    out_tokens[position++] = token;
    token = strtok(NULL, separator);
  }
  //out_tokens[position] = NULL;

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
//void pipe_work(int pipefd[],char** token_list){
  
  
//}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//  Выполнение команды с записью в дескриптор
void exec_write(int filedes[], char** cmd){
    if (filedes[1] != STDOUT_FILENO)
    {
        if (dup2(filedes[1], STDOUT_FILENO) == -1) err_exit("exec_write:  dup2");
        if (close(filedes[1]) == -1) err_exit("exec_write:  close 1");
    }
    if(execvp(cmd[0], cmd) == -1) err_exit("write exec");
}

//  Выполнение команды с прочитыванием из дескриптора
void exec_read(int filedes[], char** cmd){
    if (filedes[0] != STDIN_FILENO)
    {
        if (dup2(filedes[0], STDIN_FILENO) == -1) err_exit("dup2");
        if (close(filedes[0]) == -1) err_exit("close 2");
    }
    
    if(execvp(cmd[0], cmd) == -1) err_exit("read exec");
}


//  Основная работа
void processing(char* line) {
  int pipefd[2];
  if (pipe(pipefd) == -1) err_exit("pipe");
  char **command_buff;                  //  Буффер разеделния по "|"
  char **token_buff;                    //  Буффер разделения по пробелам
  command_buff = parcer(line, "|");     //  Делим ввод на комнады через "|"
  for (size_t i = 0; i < sizeof(command_buff); i++)
  {
    printf("out -->  %s\n", command_buff[i]);
  }
  switch(fork()){
    case -1:
        err_exit("fork");
        break;
    case 0:                             /*  Дочерний exec: "ls" с записью в дескриптор */
        close(pipefd[0]);
        token_buff = parcer(command_buff[0], " ");  //  Делим ввод на комнады через пробелы
        for (size_t i = 0; i < sizeof(token_buff); i++)
        {
          printf("token_buff_write -->  %s\n", token_buff[i]);
        }
        exec_write(pipefd, token_buff);
        break;
    default:                            /*  Подительский exec: "grep" с считыванием из дескриптор */
        close(pipefd[1]);
        token_buff = parcer(command_buff[1], " ");
        for (size_t i = 0; i < sizeof(token_buff); i++)
        {
          printf("token_buff_read -->  %s\n", token_buff[i]);
        }
        exec_read(pipefd, token_buff);
        break;
    }
}

int main() {
  
  char * buff = readline();
  char ** token_buff;
  processing(buff);

  return 0;
}