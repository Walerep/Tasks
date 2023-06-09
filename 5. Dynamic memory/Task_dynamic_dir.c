#include <malloc.h>
#include <stdlib.h>

#include "stdio.h"
#include "string.h"

// база для справочника
struct directory {
  int id;
  char firstname[20];
  char surname[20];
  char telephone[12];
} directory;

void menu() {
  printf("1. Просмотреть справочник\n");
  printf("2. Добавить абонента\n");
  printf("3. Удалить абонента\n");
  printf("4. Найти абонента\n");
  printf("5. Выход\n");
}

// вывести весь справочник
void show_all(struct directory *directorys, int size) {
  printf("id | Имя | Фамилия | Телефон \n\n");
  for (int i = 0; i < size; i++) {
    printf("%d | %s | %s | %s\n", directorys[i].id, directorys[i].firstname,
           directorys[i].surname, directorys[i].telephone);
  }
  printf("\n----------Конец списка---------\n");
}

// добаивть абонента
void add_field(struct directory **directorys, int *size) {
  *directorys = realloc(*directorys, (*size + 1) * sizeof(directory));
  if (directorys == NULL) {
    perror("----Memory error----\n");
    exit(1);
  }
  (*directorys)[*size].id = (*size);
  printf("Введите имя: ");
  scanf("%s", &(*directorys)[*size].firstname);
  printf("Введите фамилию: ");
  scanf("%s", &(*directorys)[*size].surname);
  printf("Введите телефон: ");
  scanf("%s", &(*directorys)[*size].telephone);
  (*size)++;
}

// удалить абонента
void delete_field(struct directory **directorys, int *size) {
  int option;
  printf("Выберите опцию:\n");
  printf("1. Удалить по id\n");
  printf("2. Удалить по имени\n");
  printf("3. Удалить по фамилии\n");
  scanf("%d", &option);
  switch (option) {
    case 1:
      int id_chose;
      printf("Введите id: ");
      scanf("%d", &id_chose);
      strcpy(&(*directorys)[id_chose].firstname, "");
      strcpy(&(*directorys)[id_chose].surname, "");
      strcpy(&(*directorys)[id_chose].telephone, "");
      printf("Абонент удален\n");
      directorys = realloc(*directorys, (*size) * sizeof(directory));
      if (directorys == NULL) {
        perror("----Memory error----\n");
        exit(1);
      }
      (*size)--;
      break;
    case 2:
      char name_target[20];
      printf("Введите имя: ");
      scanf("%s", name_target);
      for (int i = 0; i != *size; i++) {
        if (strcmp(&(*directorys)[i].firstname, name_target) == 0) {
          strcpy(&(*directorys)[i].firstname, "");
          strcpy(&(*directorys)[i].surname, "");
          strcpy(&(*directorys)[i].telephone, "");
          printf("Абонент удален\n");
        }
      }
      directorys = realloc(*directorys, (*size) * sizeof(directory));
      if (directorys == NULL) {
        perror("----Memory error----\n");
        exit(1);
      }
      (*size)--;
      break;
    case 3:
      char surname_target[20];
      printf("Введите фамилию: ");
      scanf("%s", surname_target);
      for (int i = 0; i != *size; i++) {
        if (strcmp(&(*directorys)[i].surname, surname_target) == 0) {
          strcpy(&(*directorys)[i].firstname, "");
          strcpy(&(*directorys)[i].surname, "");
          strcpy(&(*directorys)[i].telephone, "");
          printf("Абонент удален\n");
        }
      }
      directorys = realloc(*directorys, (*size) * sizeof(directory));
      if (directorys == NULL) {
        perror("----Memory error----\n");
        exit(1);
      }
      (*size)--;
      break;
    default:
      printf("Oopsie...\n");
      break;
  }
}

// найти абонента
void search_field(struct directory *directorys, int size) {
  int option;
  printf("Выберите опцию:\n");
  printf("1. Найти по id\n");
  printf("2. Найти по имени\n");
  printf("3. Найти по фамилии\n");
  scanf("%d", &option);
  switch (option) {
    case 1:
      int id_chose;
      printf("Введите id: ");
      scanf("%d", &id_chose);
      printf("%d | %s | %s | %s\n", directorys[id_chose].id,
             directorys[id_chose].firstname, directorys[id_chose].surname,
             directorys[id_chose].telephone);
      break;
    case 2:
      char firstname_target[30];
      printf("Введите имя для поиска: ");
      scanf("%s", firstname_target);
      for (int i = 0; i != size; i++) {
        if (strcmp(firstname_target, directorys[i].firstname) == 0) {
          printf("%d | %s | %s | %s\n", directorys[i].id,
                 directorys[i].firstname, directorys[i].surname,
                 directorys[i].telephone);
        }
      }
      break;
    case 3:
      char surname_target[20];
      printf("Введите фамилию: ");
      scanf("%s", surname_target);
      for (int i = 0; i != size; i++) {
        if (strcmp(directorys[i].surname, surname_target) == 0) {
          printf("%d | %s | %s | %s\n", directorys[i].id,
                 directorys[i].firstname, directorys[i].surname,
                 directorys[i].telephone);
        }
      }
      break;
    default:
      printf("Oopsie...\n");
      break;
  }
}

int main() {
  int option;
  int size = 0;
  struct directory *directorys = malloc((size + 1) * sizeof(directory));
  do {
    menu();
    scanf("%d", &option);
    switch (option) {
      case 1:
        show_all(directorys, size);
        break;
      case 2:
        add_field(&directorys, &size);
        break;
      case 3:
        delete_field(&directorys, &size);
        break;
      case 4:
        search_field(directorys, size);
        break;
      case 5:
        free(directorys);
        return 0;
      default:
        printf("Ошибка\n");
        break;
    }
  } while (1);
}
