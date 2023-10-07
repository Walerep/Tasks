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
  int queue_id = msgget(queue_key, IPC_CREAT | 0666);  //  id очереди
  if (queue_id == -1) err_exit("msgget");
  struct msg_buf send_msg,  //  структура для исходящего сообщения
      res_msg;  //  структура для входящего сообщения
  msgrcv(queue_id, &res_msg, sizeof(res_msg.mtext), 0, 0);
  printf("Recived : %s\n", res_msg.mtext);
  snprintf(send_msg.mtext, MSG_SIZE, "%s\n", "Server msg : Hello");
  if (msgsnd(queue_id, &send_msg, sizeof(send_msg.mtext), 0) == -1)
    err_exit("msgsnd");  //  отправка сообщения
  printf("Sended : %s\n", send_msg.mtext);
  if (msgctl(queue_id, IPC_RMID, NULL))
    err_exit("msgctl");  //  закрытие очереди
  return 0;
}
