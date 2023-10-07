#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

#define MAX_MSG 256

int main(){
  mqd_t mqd;
  char buff[MAX_MSG];

  mqd = mq_open("/test_queue", O_RDWR);   //  Open queue
  if (mqd == (mqd_t) -1) err_exit("mqd");
  if (mqd == -1) err_exit("mqd");
  mq_send(mqd, "Holla!", MAX_MSG, 0);     //  Send msg
  if (mqd == -1) err_exit("mq_send");
  mq_receive(mqd, buff, MAX_MSG, NULL);   //  Recive msg
  if (mqd == -1) err_exit("mq_recive");
  printf("Server sent: %s\n", buff);
  mq_close(mqd);                          //  Close msg queue descriptor
  if (mqd == -1) err_exit("mq_close");
  return 0;
}