#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

#define MAX_MSG 256

int main() {
  struct mq_attr attr;
  mqd_t mqd;
  char buff[MAX_MSG + 1];
  attr.mq_flags = 0;
  attr.mq_maxmsg = 5;
  attr.mq_msgsize = MAX_MSG;
  attr.mq_curmsgs = 0;

  mqd = mq_open("/test_queue", O_CREAT | O_RDWR, 0600, &attr);  //  Open queue
  if (mqd == (mqd_t) -1) err_exit("mqd");
  mq_send(mqd, "Hello!", MAX_MSG, 0);                           //  Send msg
  if (mqd == -1) err_exit("mq_send");
  mq_receive(mqd, buff, MAX_MSG, NULL);                         //  Recive msg
  if (mqd == -1) err_exit("mq_recive");
  printf("Client send: %s\n", buff);
  mq_close(mqd);                                                //  Close msg queue descriptor
  if (mqd == -1) err_exit("mq_close");
  mq_unlink("/test_queue");                                     //  Remove queue
  if (mqd == -1) err_exit("mq_unlink");
  return 0;  
}