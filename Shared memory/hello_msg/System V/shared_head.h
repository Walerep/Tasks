#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define MSG_SIZE 256
#define FTOK_PATH "server.c"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

struct msg_buf {
  long mtype;
  char mtext[MSG_SIZE];
};

union semun
{
  int val;
  struct semid_ds * buf;
  unsigned short * array;
  struct seminfo * __buf;
};

struct sembuf unlock_sem = {0, 1, 0};
struct sembuf lock_sem = {0, -1, 0};
