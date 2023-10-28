#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)
#define SHM_PATH "/shmpath"
#define SEM_PATH_1 "/semaphore1"
#define SEM_PATH_2 "/semaphore2"
#define MAX_MSG 256
