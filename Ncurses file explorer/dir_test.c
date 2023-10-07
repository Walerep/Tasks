#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>

#define err_exit(msg) do {perror(msg); exit(EXIT_FAILURE); } while(0)
#define LISTING_MAX 128

//  Структура хранящая листинг файлов в текущем каталоге
struct LISTING
{
  int position; // position of smth in that struct
  char name[128]; //  name of folder  / file in struct
  unsigned char type; // type of smth in that struct
};

struct LISTING listing[LISTING_MAX]; 

void whats_inside(const char * path) {
  struct dirent * namelist;
  int i = 1;
  int index = 0;
  DIR * dir = opendir(path);
  char * dir_inside[100];
  //if (dir == NULL) err_msg("opendir");
  while ((namelist = readdir(dir)) != NULL){
    dir_inside[i] = namelist->d_name;
    strncpy(listing[i].name, dir_inside[i], 64);
    strcpy(&listing[i].type, &namelist[i].d_type);
    listing[i].position = i;
    i++;
  }
  for (int n = 0; n < i; n++)
  {
    printf("name: %s    type: %d\n", listing[n].name, listing->type);
  }
  
  closedir(dir);
}


int main(){
  char line[100];
  scanf("%s", line);
  DIR * directory;
  struct dirent **namelist;
  int n;
  whats_inside(".");
  //directory = opendir(line);
  //if (directory == NULL)
  //{
  //  err_exit("opendir");
  //}
  //namelist = readdir(directory);
  //n = scandir(line,&namelist, NULL, NULL);
  //if (namelist == NULL)
  //{
  //  err_exit("readdir");
  //}
  
  
  //for (int i = 0; i < sizeof(namelist); i++)
  //{
  //  printf("dir %d: %s\n", i, namelist[i]->d_name);
  //    }
  
}