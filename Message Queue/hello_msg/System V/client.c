#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define MSG_SIZE 256

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

struct msg_buf {
  long mtype;
  char mtext[MSG_SIZE];
};

int main() {
  key_t queue_key = ftok("server.c", 3);  //  генерация ключа очереди
  if (queue_key == -1) err_exit("ftok");
  int queue_id = msgget(queue_key, 0);  //  id очереди
  if (queue_id == -1) err_exit("msgget");
  struct msg_buf send_msg,  //  структура для исходящего сообщения
      res_msg;  //  структура для входящего сообщения

  snprintf(send_msg.mtext, MSG_SIZE, "%s\n", "Client msg : Hi");
  if (msgsnd(queue_id, &send_msg, sizeof(send_msg.mtext), 0) == -1)
    err_exit("msgsnd");  //  отправка сообщения
  msgrcv(queue_id, &res_msg, sizeof(res_msg.mtext), 0, 0);
  printf("Client recived : %s\n", res_msg.mtext);
  printf("Sended : %s\n", send_msg.mtext);
  return 0;
}
